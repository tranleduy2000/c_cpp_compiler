/* Constraint_System class implementation (non-inline functions).
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://bugseng.com/products/ppl/ . */

#include "ppl-config.h"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "Generator_defs.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "Congruence_System_defs.hh"
#include "Congruence_System_inlines.hh"
#include "assert.hh"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace PPL = Parma_Polyhedra_Library;

PPL::Constraint_System::Constraint_System(const Congruence_System& cgs,
                                          Representation r)
  : sys(NECESSARILY_CLOSED, cgs.space_dimension(), r) {
  for (Congruence_System::const_iterator i = cgs.begin(),
         cgs_end = cgs.end(); i != cgs_end; ++i)
    if (i->is_equality()) {
      Constraint tmp(*i);
      insert(tmp, Recycle_Input());
    }
  PPL_ASSERT(OK());
}

bool
PPL::Constraint_System::
adjust_topology_and_space_dimension(const Topology new_topology,
                                    const dimension_type new_space_dim) {
  PPL_ASSERT(space_dimension() <= new_space_dim);

  if (sys.topology() == NOT_NECESSARILY_CLOSED
      && new_topology == NECESSARILY_CLOSED) {
    // A NOT_NECESSARILY_CLOSED constraint system
    // can be converted to a NECESSARILY_CLOSED one
    // only if it does not contain strict inequalities.
    if (has_strict_inequalities())
      return false;
    // Since there were no strict inequalities,
    // the only constraints that may have a non-zero epsilon coefficient
    // are the eps-leq-one and the eps-geq-zero constraints.
    // If they are present, we erase these rows, so that the
    // epsilon column will only contain zeroes: as a consequence,
    // we just decrement the number of columns to be added.
    const bool was_sorted = sys.is_sorted();

    // Note that num_rows() is *not* constant, because it is decreased by
    // remove_row().
    for (dimension_type i = 0; i < num_rows(); )
      if (sys[i].epsilon_coefficient() != 0)
        sys.remove_row(i, false);
      else
        ++i;

    // If `cs' was sorted we sort it again.
    if (was_sorted)
      sys.sort_rows();
  }

  sys.set_topology(new_topology);
  sys.set_space_dimension(new_space_dim);

  // We successfully adjusted space dimensions and topology.
  PPL_ASSERT(OK());
  return true;
}

bool
PPL::Constraint_System::has_equalities() const {
  // We verify if the system has equalities also in the pending part.
  for (dimension_type i = sys.num_rows(); i-- > 0; )
    if (sys[i].is_equality())
      return true;
  return false;
}

bool
PPL::Constraint_System::has_strict_inequalities() const {
  if (sys.is_necessarily_closed())
    return false;
  // We verify if the system has strict inequalities
  // also in the pending part.
  for (dimension_type i = sys.num_rows(); i-- > 0; ) {
    const Constraint& c = sys[i];
    // Optimized type checking: we already know the topology;
    // also, equalities have the epsilon coefficient equal to zero.
    // NOTE: the constraint eps_leq_one should not be considered
    //       a strict inequality.
    if (c.epsilon_coefficient() < 0 && !c.is_tautological())
      return true;
  }
  return false;
}

void
PPL::Constraint_System::insert(const Constraint& r) {
  Constraint tmp = r;
  insert(tmp, Recycle_Input());
}

void
PPL::Constraint_System::insert(Constraint& c, Recycle_Input) {
  // We are sure that the matrix has no pending rows
  // and that the new row is not a pending constraint.
  PPL_ASSERT(sys.num_pending_rows() == 0);

  if (sys.topology() != c.topology()) {
    if (sys.topology() == NECESSARILY_CLOSED)
      sys.set_topology(NOT_NECESSARILY_CLOSED);
    else
      c.set_topology(NOT_NECESSARILY_CLOSED);
  }

  sys.insert(c, Recycle_Input());

  PPL_ASSERT(OK());
}

void
PPL::Constraint_System::insert_pending(const Constraint& r) {
  Constraint tmp = r;
  insert_pending(tmp, Recycle_Input());
}

void
PPL::Constraint_System::insert_pending(Constraint& c, Recycle_Input) {
  if (sys.topology() != c.topology()) {
    if (sys.topology() == NECESSARILY_CLOSED)
      sys.set_topology(NOT_NECESSARILY_CLOSED);
    else
      c.set_topology(NOT_NECESSARILY_CLOSED);
  }

  sys.insert_pending(c, Recycle_Input());
  PPL_ASSERT(OK());
}

PPL::dimension_type
PPL::Constraint_System::num_inequalities() const {
  // We are sure that we call this method only when
  // the matrix has no pending rows.
  PPL_ASSERT(sys.num_pending_rows() == 0);
  const Constraint_System& cs = *this;
  dimension_type n = 0;
  // If the Base happens to be sorted, take advantage of the fact
  // that inequalities are at the bottom of the system.
  if (sys.is_sorted())
    for (dimension_type i = sys.num_rows(); i > 0 && cs[--i].is_inequality(); )
      ++n;
  else
    for (dimension_type i = sys.num_rows(); i-- > 0 ; )
      if (cs[i].is_inequality())
        ++n;
  return n;
}

PPL::dimension_type
PPL::Constraint_System::num_equalities() const {
  // We are sure that we call this method only when
  // the matrix has no pending rows.
  PPL_ASSERT(sys.num_pending_rows() == 0);
  return sys.num_rows() - num_inequalities();
}

void
PPL::Constraint_System_const_iterator::skip_forward() {
  const Linear_System<Constraint>::const_iterator csp_end = csp->end();
  while (i != csp_end && (*this)->is_tautological())
    ++i;
}

bool
PPL::Constraint_System::satisfies_all_constraints(const Generator& g) const {
  PPL_ASSERT(g.space_dimension() <= space_dimension());

  // Setting `sps' to the appropriate scalar product sign operator.
  // This also avoids problems when having _legal_ topology mismatches
  // (which could also cause a mismatch in the number of columns).
  const Topology_Adjusted_Scalar_Product_Sign sps(g);

  if (sys.is_necessarily_closed()) {
    if (g.is_line()) {
      // Lines must saturate all constraints.
      for (dimension_type i = sys.num_rows(); i-- > 0; )
        if (sps(g, sys[i]) != 0)
          return false;
    }
    else
      // `g' is either a ray, a point or a closure point.
      for (dimension_type i = sys.num_rows(); i-- > 0; ) {
        const Constraint& c = sys[i];
        const int sp_sign = sps(g, c);
        if (c.is_inequality()) {
          // As `cs' is necessarily closed,
          // `c' is a non-strict inequality.
          if (sp_sign < 0)
            return false;
        }
        else
          // `c' is an equality.
          if (sp_sign != 0)
            return false;
      }
  }
  else
    // `cs' is not necessarily closed.
    switch (g.type()) {

    case Generator::LINE:
      // Lines must saturate all constraints.
      for (dimension_type i = sys.num_rows(); i-- > 0; )
        if (sps(g, sys[i]) != 0)
          return false;

      break;

    case Generator::POINT:
      // Have to perform the special test
      // when dealing with a strict inequality.
      for (dimension_type i = sys.num_rows(); i-- > 0; ) {
        const Constraint& c = sys[i];
        const int sp_sign = sps(g, c);
        switch (c.type()) {
        case Constraint::EQUALITY:
          if (sp_sign != 0)
            return false;
          break;
        case Constraint::NONSTRICT_INEQUALITY:
          if (sp_sign < 0)
            return false;
          break;
        case Constraint::STRICT_INEQUALITY:
          if (sp_sign <= 0)
            return false;
          break;
        }
      }
      break;

    case Generator::RAY:
      // Intentionally fall through.
    case Generator::CLOSURE_POINT:
      for (dimension_type i = sys.num_rows(); i-- > 0; ) {
        const Constraint& c = sys[i];
        const int sp_sign = sps(g, c);
        if (c.is_inequality()) {
          // Constraint `c' is either a strict or a non-strict inequality.
          if (sp_sign < 0)
            return false;
        }
        else
          // Constraint `c' is an equality.
          if (sp_sign != 0)
            return false;
      }
      break;
    }

  // If we reach this point, `g' satisfies all constraints.
  return true;
}


void
PPL::Constraint_System
::affine_preimage(const Variable v,
                  const Linear_Expression& expr,
                  Coefficient_traits::const_reference denominator) {
  PPL_ASSERT(v.space_dimension() <= sys.space_dimension());
  PPL_ASSERT(expr.space_dimension() <= sys.space_dimension());
  PPL_ASSERT(denominator > 0);

  Coefficient_traits::const_reference expr_v = expr.coefficient(v);

  const dimension_type n_rows = sys.num_rows();
  const bool not_invertible = (v.space_dimension() > expr.space_dimension()
                               || expr_v == 0);

  for (dimension_type i = n_rows; i-- > 0; ) {
    Constraint& row = sys.rows[i];
    Coefficient_traits::const_reference row_v = row.coefficient(v);
    if (row_v != 0) {
      const Coefficient c = row_v;
      if (denominator != 1)
        row.expr *= denominator;
      row.expr.linear_combine(expr, 1, c, 0, expr.space_dimension() + 1);
      if (not_invertible)
        row.expr.set_coefficient(v, Coefficient_zero());
      else
        row.expr.set_coefficient(v, c * expr_v);
      row.strong_normalize();
      PPL_ASSERT(row.OK());
    }
  }

  // Strong normalization also resets the sortedness flag.
  sys.strong_normalize();

  PPL_ASSERT(sys.OK());
}

void
PPL::Constraint_System::ascii_dump(std::ostream& s) const {
  sys.ascii_dump(s);
}

PPL_OUTPUT_DEFINITIONS(Constraint_System)

bool
PPL::Constraint_System::ascii_load(std::istream& s) {
  if (!sys.ascii_load(s))
    return false;

  PPL_ASSERT(OK());
  return true;
}

const PPL::Constraint_System* PPL::Constraint_System::zero_dim_empty_p = 0;

void
PPL::Constraint_System::initialize() {
  PPL_ASSERT(zero_dim_empty_p == 0);
  zero_dim_empty_p
    = new Constraint_System(Constraint::zero_dim_false());
}

void
PPL::Constraint_System::finalize() {
  PPL_ASSERT(zero_dim_empty_p != 0);
  delete zero_dim_empty_p;
  zero_dim_empty_p = 0;
}

bool
PPL::Constraint_System::OK() const {
  return sys.OK();
}

/*! \relates Parma_Polyhedra_Library::Constraint_System */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Constraint_System& cs) {
  Constraint_System_const_iterator i = cs.begin();
  const Constraint_System_const_iterator cs_end = cs.end();
  if (i == cs_end)
    s << "true";
  else {
    while (i != cs_end) {
      s << *i;
      ++i;
      if (i != cs_end)
        s << ", ";
    }
  }
  return s;
}
