/* Constraint class implementation (non-inline functions).
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
#include "Constraint_defs.hh"
#include "Variable_defs.hh"
#include "Variables_Set_defs.hh"
#include "Congruence_defs.hh"
#include "math_utilities_defs.hh"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Constraint::throw_invalid_argument(const char* method,
                                        const char* message) const {
  std::ostringstream s;
  s << "PPL::Constraint::" << method << ":" << std::endl
    << message;
  throw std::invalid_argument(s.str());
}

void
PPL::Constraint::throw_dimension_incompatible(const char* method,
                                              const char* name_var,
                                              const Variable v) const {
  std::ostringstream s;
  s << "PPL::Constraint::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension() << ", "
    << name_var << ".space_dimension() == " << v.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

PPL::Constraint
PPL::Constraint::construct_epsilon_geq_zero() {
  Linear_Expression e;
  Constraint c(e, NONSTRICT_INEQUALITY, NOT_NECESSARILY_CLOSED);
  c.set_epsilon_coefficient(Coefficient_one());
  PPL_ASSERT(c.OK());
  return c;
}

PPL::Constraint::Constraint(const Congruence& cg, Representation r)
  : expr(cg.expression(), r),
    kind_(LINE_OR_EQUALITY),
    topology_(NECESSARILY_CLOSED) {
  if (!cg.is_equality())
    throw_invalid_argument("Constraint(cg)",
                           "congruence cg must be an equality.");
  // Enforce normalization.
  strong_normalize();
  PPL_ASSERT(OK());
}

void
PPL::Constraint::swap_space_dimensions(Variable v1, Variable v2) {
  PPL_ASSERT(v1.space_dimension() <= space_dimension());
  PPL_ASSERT(v2.space_dimension() <= space_dimension());
  expr.swap_space_dimensions(v1, v2);
  // *this is still normalized but it may not be strongly normalized.
  sign_normalize();
  PPL_ASSERT(OK());
}

void
PPL::Constraint
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  if (cycle.size() < 2)
    // No-op. No need to call sign_normalize().
    return;

  expr.permute_space_dimensions(cycle);
  // *this is still normalized but may be not strongly normalized:
  // sign normalization is necessary.
  sign_normalize();
  PPL_ASSERT(OK());
}

bool
PPL::Constraint::is_tautological() const {
  if (expr.all_homogeneous_terms_are_zero())
    if (is_equality())
      return expr.inhomogeneous_term() == 0;
    else
      // Non-strict inequality constraint.
      return expr.inhomogeneous_term() >= 0;
  else
    // There is a non-zero homogeneous coefficient.
    if (is_necessarily_closed())
      return false;
    else {
      // The constraint is NOT necessarily closed.
      const int eps_sign = sgn(epsilon_coefficient());
      if (eps_sign > 0)
        // We have found the constraint epsilon >= 0.
        return true;
      if (eps_sign == 0)
        // One of the `true' dimensions has a non-zero coefficient.
        return false;
      else {
        // Here the epsilon coefficient is negative: strict inequality.
        if (expr.inhomogeneous_term() <= 0)
          // A strict inequality such as `lhs - k > 0',
          // where k is a non negative integer, cannot be trivially true.
          return false;
        // Checking for another non-zero coefficient.
        // If the check succeeds, we have the inequality `k > 0',
        // where k is a positive integer.
        return expression().all_homogeneous_terms_are_zero();
      }
    }
}

bool
PPL::Constraint::is_inconsistent() const {
  if (expr.all_homogeneous_terms_are_zero())
    // The inhomogeneous term is the only non-zero coefficient.
    if (is_equality())
      return expr.inhomogeneous_term() != 0;
    else
      // Non-strict inequality constraint.
      return expr.inhomogeneous_term() < 0;
  else
    // There is a non-zero homogeneous coefficient.
    if (is_necessarily_closed())
      return false;
    else {
      // The constraint is NOT necessarily closed.
      if (epsilon_coefficient() >= 0)
        // If positive, we have found the constraint epsilon >= 0.
        // If zero, one of the `true' dimensions has a non-zero coefficient.
        // In both cases, it is not trivially false.
        return false;
      else {
        // Here the epsilon coefficient is negative: strict inequality.
        if (expr.inhomogeneous_term() > 0)
          // A strict inequality such as `lhs + k > 0',
          // where k is a positive integer, cannot be trivially false.
          return false;
        // Checking for another non-zero coefficient.
        // If the check succeeds, we have the inequality `k > 0',
        // where k is a positive integer.
        return expression().all_homogeneous_terms_are_zero();
      }
    }
}

void
PPL::Constraint::linear_combine(const Constraint& y, dimension_type i) {
  expr.linear_combine(y.expr, i);
  strong_normalize();
}

/*! \relates Parma_Polyhedra_Library::Constraint */
int
PPL::compare(const Constraint& x, const Constraint& y) {
  const bool x_is_line_or_equality = x.is_line_or_equality();
  const bool y_is_line_or_equality = y.is_line_or_equality();
  if (x_is_line_or_equality != y_is_line_or_equality)
    // Equalities (lines) precede inequalities (ray/point).
    return y_is_line_or_equality ? 2 : -2;

  return compare(x.expr, y.expr);
}

bool
PPL::Constraint::is_equivalent_to(const Constraint& y) const {
  const Constraint& x = *this;
  const dimension_type x_space_dim = x.space_dimension();
  if (x_space_dim != y.space_dimension())
    return false;

  const Type x_type = x.type();
  if (x_type != y.type()) {
    // Check for special cases.
    if (x.is_tautological())
      return y.is_tautological();
    else
      return x.is_inconsistent() && y.is_inconsistent();
  }

  if (x_type == STRICT_INEQUALITY) {
    // Due to the presence of epsilon-coefficients, syntactically
    // different strict inequalities may actually encode the same
    // topologically open half-space.
    // First, drop the epsilon-coefficient ...
    Linear_Expression x_expr(x.expression());
    Linear_Expression y_expr(y.expression());
    // ... then, re-normalize ...
    x_expr.normalize();
    y_expr.normalize();
    // ... and finally check for syntactic equality.
    return x_expr.is_equal_to(y_expr);
  }

  // `x' and 'y' are of the same type and they are not strict inequalities;
  // thus, the epsilon-coefficient, if present, is zero.
  // It is sufficient to check for syntactic equality.
  return x.expr.is_equal_to(y.expr);
}

bool
PPL::Constraint::is_equal_to(const Constraint& y) const {
  return expr.is_equal_to(y.expr) && kind_ == y.kind_ && topology() == y.topology();
}

void
PPL::Constraint::sign_normalize() {
  if (is_line_or_equality())
    expr.sign_normalize();
}

bool
PPL::Constraint::check_strong_normalized() const {
  Constraint tmp = *this;
  tmp.strong_normalize();
  return compare(*this, tmp) == 0;
}

const PPL::Constraint* PPL::Constraint::zero_dim_false_p = 0;
const PPL::Constraint* PPL::Constraint::zero_dim_positivity_p = 0;
const PPL::Constraint* PPL::Constraint::epsilon_geq_zero_p = 0;
const PPL::Constraint* PPL::Constraint::epsilon_leq_one_p = 0;

void
PPL::Constraint::initialize() {
  PPL_ASSERT(zero_dim_false_p == 0);
  zero_dim_false_p
    = new Constraint(Linear_Expression::zero() == Coefficient_one());

  PPL_ASSERT(zero_dim_positivity_p == 0);
  zero_dim_positivity_p
    = new Constraint(Linear_Expression::zero() <= Coefficient_one());

  PPL_ASSERT(epsilon_geq_zero_p == 0);
  epsilon_geq_zero_p
    = new Constraint(construct_epsilon_geq_zero());

  PPL_ASSERT(epsilon_leq_one_p == 0);
  epsilon_leq_one_p
    = new Constraint(Linear_Expression::zero() < Coefficient_one());
}

void
PPL::Constraint::finalize() {
  PPL_ASSERT(zero_dim_false_p != 0);
  delete zero_dim_false_p;
  zero_dim_false_p = 0;

  PPL_ASSERT(zero_dim_positivity_p != 0);
  delete zero_dim_positivity_p;
  zero_dim_positivity_p = 0;

  PPL_ASSERT(epsilon_geq_zero_p != 0);
  delete epsilon_geq_zero_p;
  epsilon_geq_zero_p = 0;

  PPL_ASSERT(epsilon_leq_one_p != 0);
  delete epsilon_leq_one_p;
  epsilon_leq_one_p = 0;
}

void
PPL::Constraint::ascii_dump(std::ostream& s) const {
  expr.ascii_dump(s);

  s << " ";
  
  switch (type()) {
  case Constraint::EQUALITY:
    s << "=";
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    s << ">=";
    break;
  case Constraint::STRICT_INEQUALITY:
    s << ">";
    break;
  }
  s << " ";
  if (topology() == NECESSARILY_CLOSED)
    s << "(C)";
  else
    s << "(NNC)";

  s << "\n";
}

bool
PPL::Constraint::ascii_load(std::istream& s) {
  std::string str;
  std::string str2;

  expr.ascii_load(s);

  if (!(s >> str))
    return false;
  if (str == "=")
    set_is_equality();
  else if (str == ">=" || str == ">")
    set_is_inequality();
  else
    return false;

  if (!(s >> str2))
    return false;
  if (str2 == "(NNC)") {
    // TODO: Avoid the mark_as_*() methods if possible.
    if (topology() == NECESSARILY_CLOSED)
      mark_as_not_necessarily_closed();
  }
  else
    if (str2 == "(C)") {
      // TODO: Avoid the mark_as_*() methods if possible.
      if (topology() == NOT_NECESSARILY_CLOSED)
        mark_as_necessarily_closed();
    }
    else
      return false;

  // Checking for equality of actual and declared types.
  switch (type()) {
  case EQUALITY:
    if (str != "=")
      return false;
    break;
  case NONSTRICT_INEQUALITY:
    if (str != ">=")
      return false;
    break;
  case STRICT_INEQUALITY:
    if (str != ">")
      return false;
    break;
  }

  return true;
}

/*! \relates Parma_Polyhedra_Library::Constraint */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Constraint& c) {
  PPL_DIRTY_TEMP_COEFFICIENT(cv);
  bool first = true;
  for (Constraint::expr_type::const_iterator i = c.expression().begin(),
         i_end = c.expression().end(); i != i_end; ++i) {
    cv = *i;
    if (!first) {
      if (cv > 0)
        s << " + ";
      else {
        s << " - ";
        neg_assign(cv);
      }
    }
    else
      first = false;
    if (cv == -1)
      s << "-";
    else if (cv != 1)
      s << cv << "*";
    s << i.variable();
  }
  if (first)
    s << Coefficient_zero();
  const char* relation_symbol = 0;
  switch (c.type()) {
  case Constraint::EQUALITY:
    relation_symbol = " = ";
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    relation_symbol = " >= ";
    break;
  case Constraint::STRICT_INEQUALITY:
    relation_symbol = " > ";
    break;
  }
  s << relation_symbol << -c.inhomogeneous_term();
  return s;
}

/*! \relates Parma_Polyhedra_Library::Constraint */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Constraint::Type& t) {
  const char* n = 0;
  switch (t) {
  case Constraint::EQUALITY:
    n = "EQUALITY";
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    n = "NONSTRICT_INEQUALITY";
    break;
  case Constraint::STRICT_INEQUALITY:
    n = "STRICT_INEQUALITY";
    break;
  }
  s << n;
  return s;
}

PPL_OUTPUT_DEFINITIONS(Constraint)

bool
PPL::Constraint::OK() const {
  // Topology consistency checks.
  if (is_not_necessarily_closed() && expr.space_dimension() == 0) {
#ifndef NDEBUG
    std::cerr << "Constraint has fewer coefficients than the minimum "
              << "allowed by its topology."
              << std::endl;
#endif
    return false;
  }

  if (is_equality() && is_not_necessarily_closed()
      && epsilon_coefficient() != 0) {
#ifndef NDEBUG
    std::cerr << "Illegal constraint: an equality cannot be strict."
              << std::endl;
#endif
    return false;
  }

  // Normalization check.
  Constraint tmp = *this;
  tmp.strong_normalize();
  if (tmp != *this) {
#ifndef NDEBUG
    std::cerr << "Constraint is not strongly normalized as it should be."
              << std::endl;
#endif
    return false;
  }

  // All tests passed.
  return true;
}
