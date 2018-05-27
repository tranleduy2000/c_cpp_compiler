/* Congruence_System class implementation (non-inline functions).
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
#include "Congruence_System_defs.hh"
#include "Congruence_System_inlines.hh"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "Congruence_defs.hh"
#include "Grid_Generator_defs.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "assert.hh"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace PPL = Parma_Polyhedra_Library;

PPL::Congruence_System::Congruence_System(const Constraint_System& cs,
                                          Representation r)
  : rows(),
    space_dimension_(cs.space_dimension()),
    representation_(r) {
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i)
    if (i->is_equality())
      insert(*i);
}

void
PPL::Congruence_System
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  for (dimension_type k = rows.size(); k-- > 0; ) {
    Congruence& rows_k = rows[k];
    rows_k.permute_space_dimensions(cycle);
  }
  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::remove_rows(const dimension_type first,
                                    const dimension_type last,
                                    bool keep_sorted) {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= num_rows());
  const dimension_type n = last - first;

  // Swap the rows in [first, last) with the rows in [size() - n, size())
  // (note that these intervals may not be disjunct).
  if (keep_sorted) {
    for (dimension_type i = last; i < rows.size(); ++i)
      swap(rows[i], rows[i - n]);
  }
  else {
    const dimension_type offset = rows.size() - n - first;
    for (dimension_type i = first; i < last; ++i)
      swap(rows[i], rows[i + offset]);
  }

  rows.resize(rows.size() - n);
  PPL_ASSERT(OK());
}

bool
PPL::Congruence_System
::set_space_dimension(const dimension_type new_space_dim) {
  if (space_dimension() != new_space_dim) {
    space_dimension_ = new_space_dim;
    for (dimension_type i = num_rows(); i-- > 0; )
      rows[i].set_space_dimension(new_space_dim);
  }
  PPL_ASSERT(OK());
  return true;
}

void
PPL::Congruence_System::swap_space_dimensions(Variable v1, Variable v2) {
  for (dimension_type k = num_rows(); k-- > 0; )
    rows[k].swap_space_dimensions(v1, v2);
}

void
PPL::Congruence_System::insert_verbatim(Congruence& cg, Recycle_Input) {
  // TODO: Remove this.
  PPL_ASSERT(cg.OK());

  cg.set_representation(representation());

  if (cg.space_dimension() >= space_dimension())
    set_space_dimension(cg.space_dimension());
  else
    cg.set_space_dimension(space_dimension());

  rows.resize(num_rows() + 1);

  swap(cg, rows.back());

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::insert(const Constraint& c) {
  if (c.space_dimension() > space_dimension())
    set_space_dimension(c.space_dimension());
  Congruence cg(c, space_dimension(), representation());
  cg.strong_normalize();
  rows.resize(num_rows() + 1);

  swap(cg, rows.back());

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::insert(Congruence_System& cgs, Recycle_Input) {
  const dimension_type old_num_rows = num_rows();
  const dimension_type cgs_num_rows = cgs.num_rows();
  if (space_dimension() < cgs.space_dimension())
    set_space_dimension(cgs.space_dimension());
  rows.resize(old_num_rows + cgs_num_rows);
  for (dimension_type i = cgs_num_rows; i-- > 0; ) {
    cgs.rows[i].set_space_dimension(space_dimension());
    cgs.rows[i].set_representation(representation());
    swap(cgs.rows[i], rows[old_num_rows + i]);
  }
  cgs.clear();

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::insert(const Congruence_System& y) {
  Congruence_System& x = *this;

  const dimension_type x_num_rows = x.num_rows();
  const dimension_type y_num_rows = y.num_rows();

  // Grow to the required size.
  if (space_dimension() < y.space_dimension())
    set_space_dimension(y.space_dimension());

  rows.resize(rows.size() + y_num_rows);

  // Copy the rows of `y', with the new space dimension.
  for (dimension_type i = y_num_rows; i-- > 0; ) {
    Congruence copy(y[i], space_dimension(), representation());
    swap(copy, x.rows[x_num_rows+i]);
  }
  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::normalize_moduli() {
  const Congruence_System& cgs = *this;
  dimension_type row = cgs.num_rows();
  if (row > 0) {
    // Calculate the LCM of all the moduli.
    PPL_DIRTY_TEMP_COEFFICIENT(lcm);
    // Find last proper congruence.
    while (true) {
      --row;
      lcm = cgs[row].modulus();
      if (lcm > 0)
        break;
      if (row == 0)
        // All rows are equalities.
        return;
    }
    while (row > 0) {
      --row;
      const Coefficient& modulus = cgs[row].modulus();
      if (modulus > 0)
        lcm_assign(lcm, lcm, modulus);
    }

    // Represent every row using the LCM as the modulus.
    PPL_DIRTY_TEMP_COEFFICIENT(factor);
    for (row = num_rows(); row-- > 0; ) {
      const Coefficient& modulus = cgs[row].modulus();
      if (modulus <= 0 || modulus == lcm)
        continue;
      exact_div_assign(factor, lcm, modulus);
      rows[row].scale(factor);
    }
  }
  PPL_ASSERT(OK());
}

bool
PPL::Congruence_System::is_equal_to(const Congruence_System& cgs) const {
  return (*this) == cgs;
}

bool
PPL::Congruence_System::has_linear_equalities() const {
  const Congruence_System& cgs = *this;
  for (dimension_type i = cgs.num_rows(); i-- > 0; )
    if (cgs[i].modulus() == 0)
      return true;
  return false;
}

void
PPL::Congruence_System::const_iterator::skip_forward() {
  const Swapping_Vector<Congruence>::const_iterator csp_end = csp->end();
  while (i != csp_end && (*this)->is_tautological())
    ++i;
}

PPL::dimension_type
PPL::Congruence_System::num_equalities() const {
  const Congruence_System& cgs = *this;
  dimension_type n = 0;
  for (dimension_type i = num_rows(); i-- > 0 ; )
    if (cgs[i].is_equality())
      ++n;
  return n;
}

PPL::dimension_type
PPL::Congruence_System::num_proper_congruences() const {
  const Congruence_System& cgs = *this;
  dimension_type n = 0;
  for (dimension_type i = num_rows(); i-- > 0 ; ) {
    const Congruence& cg = cgs[i];
    if (cg.is_proper_congruence())
      ++n;
  }
  return n;
}

bool
PPL::Congruence_System::
satisfies_all_congruences(const Grid_Generator& g) const {
  PPL_ASSERT(g.space_dimension() <= space_dimension());

  const Congruence_System& cgs = *this;
  PPL_DIRTY_TEMP_COEFFICIENT(sp);
  if (g.is_line())
    for (dimension_type i = cgs.num_rows(); i-- > 0; ) {
      const Congruence& cg = cgs[i];
      Scalar_Products::assign(sp, g, cg);
      if (sp != 0)
        return false;
    }
  else {
    const Coefficient& divisor = g.divisor();
    for (dimension_type i = cgs.num_rows(); i-- > 0; ) {
      const Congruence& cg = cgs[i];
      Scalar_Products::assign(sp, g, cg);
      if (cg.is_equality()) {
        if (sp != 0)
          return false;
      }
      else if (sp % (cg.modulus() * divisor) != 0)
        return false;
    }
  }
  return true;
}

bool
PPL::Congruence_System::has_a_free_dimension() const {
  // Search for a dimension that is free of any congruence or equality
  // constraint.  Assumes a minimized system.
  std::set<dimension_type> candidates;
  for (dimension_type i = space_dimension(); i-- > 0; )
    candidates.insert(i + 1);

  for (dimension_type i = num_rows(); i-- > 0; ) {
    rows[i].expression().has_a_free_dimension_helper(candidates);
    if (candidates.empty())
      return false;
  }
  return !candidates.empty();
}

void
PPL::Congruence_System::
affine_preimage(Variable v,
                const Linear_Expression& expr,
                Coefficient_traits::const_reference denominator) {
  PPL_ASSERT(v.space_dimension() <= space_dimension());
  PPL_ASSERT(expr.space_dimension() <= space_dimension());
  PPL_ASSERT(denominator > 0);

  for (dimension_type i = num_rows(); i-- > 0; )
    rows[i].affine_preimage(v, expr, denominator);
}

void
PPL::Congruence_System::ascii_dump(std::ostream& s) const {
  const Congruence_System& x = *this;
  const dimension_type x_num_rows = x.num_rows();
  const dimension_type x_space_dim = x.space_dimension();
  s << x_num_rows << " x " << x_space_dim << " ";
  Parma_Polyhedra_Library::ascii_dump(s, representation());
  s << std::endl;
  for (dimension_type i = 0; i < x_num_rows; ++i)
    x[i].ascii_dump(s);
}

PPL_OUTPUT_DEFINITIONS(Congruence_System)

bool
PPL::Congruence_System::ascii_load(std::istream& s) {
  std::string str;
  dimension_type num_rows;
  dimension_type space_dim;
  if (!(s >> num_rows))
    return false;
  if (!(s >> str) || str != "x")
    return false;
  if (!(s >> space_dim))
    return false;
  clear();
  space_dimension_ = space_dim;

  if (!Parma_Polyhedra_Library::ascii_load(s, representation_))
    return false;

  Congruence c;
  for (dimension_type i = 0; i < num_rows; ++i) {
    if (!c.ascii_load(s))
      return false;
    insert_verbatim(c, Recycle_Input());
  }

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

const PPL::Congruence_System* PPL::Congruence_System::zero_dim_empty_p = 0;

void
PPL::Congruence_System::initialize() {
  PPL_ASSERT(zero_dim_empty_p == 0);
  zero_dim_empty_p
    = new Congruence_System(Congruence::zero_dim_false());
}

void
PPL::Congruence_System::finalize() {
  PPL_ASSERT(zero_dim_empty_p != 0);
  delete zero_dim_empty_p;
  zero_dim_empty_p = 0;
}

bool
PPL::Congruence_System::OK() const {
  // All rows must have space dimension `space_dimension()'
  // and representation `representation()'.
  for (dimension_type i = num_rows(); i-- > 0; ) {
    const Congruence& cg = rows[i];
    if (cg.space_dimension() != space_dimension())
      return false;
    if (cg.representation() != representation())
      return false;
    if (!cg.OK())
      return false;
  }
  // All checks passed.
  return true;
}

/*! \relates Parma_Polyhedra_Library::Congruence_System */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Congruence_System& cgs) {
  Congruence_System::const_iterator i = cgs.begin();
  const Congruence_System::const_iterator cgs_end = cgs.end();
  if (i == cgs_end)
    return s << "true";
  while (true) {
    Congruence cg = *i;
    cg.strong_normalize();
    s << cg;
    ++i;
    if (i == cgs_end)
      return s;
    s << ", ";
  }
}

/*! \relates Parma_Polyhedra_Library::Congruence_System */
bool
PPL::operator==(const Congruence_System& x, const Congruence_System& y) {
  if (x.num_rows() != y.num_rows())
    return false;
  for (dimension_type i = x.num_rows(); i-- > 0; ) {
    // NOTE: this also checks for space dimension.
    if (x[i] != y[i])
      return false;
  }
  return true;
}

void
PPL::Congruence_System
::add_unit_rows_and_space_dimensions(dimension_type dims) {
  const dimension_type old_num_rows = num_rows();
  set_space_dimension(space_dimension() + dims);

  rows.resize(rows.size() + dims);

  // Swap the added rows to the front of the vector.
  for (dimension_type row = old_num_rows; row-- > 0; )
    swap(rows[row], rows[row + dims]);

  const dimension_type dim = space_dimension();
  // Set the space dimension and the diagonal element of each added row.
  for (dimension_type row = dims; row-- > 0; ) {
    Linear_Expression expr(representation());
    expr.set_space_dimension(space_dimension());
    PPL_ASSERT(dim >= row + 1);
    expr += Variable(dim - row - 1);
    // This constructor steals the contents of `expr'.
    Congruence cg(expr, Coefficient_zero(), Recycle_Input());
    swap(rows[row], cg);
  }

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::concatenate(const Congruence_System& y) {
  // TODO: this implementation is just an executable specification.
  Congruence_System cgs = y;

  const dimension_type added_rows = cgs.num_rows();
  const dimension_type added_columns = cgs.space_dimension();

  const dimension_type old_num_rows = num_rows();
  const dimension_type old_space_dim = space_dimension();

  set_space_dimension(space_dimension() + added_columns);

  rows.resize(rows.size() + added_rows);

  // Move the congruences into *this from `cgs', shifting the
  // coefficients along into the appropriate columns.
  for (dimension_type i = added_rows; i-- > 0; ) {
    Congruence& cg_old = cgs.rows[i];
    Congruence& cg_new = rows[old_num_rows + i];
    cg_old.set_representation(representation());
    cg_old.shift_space_dimensions(Variable(0), old_space_dim);
    swap(cg_old, cg_new);
  }
}
