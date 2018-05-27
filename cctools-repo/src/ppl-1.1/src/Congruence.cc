/* Congruence class implementation (non-inline functions).
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
#include "Congruence_defs.hh"
#include "Variable_defs.hh"
#include "Constraint_defs.hh"
#include "assert.hh"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace PPL = Parma_Polyhedra_Library;

PPL::Congruence::Congruence(const Constraint& c, Representation r)
  : expr(c.expression(), c.space_dimension(), r),
    modulus_(0) {
  if (!c.is_equality())
    throw_invalid_argument("Congruence(c, r)",
                           "constraint c must be an equality.");
}

PPL::Congruence::Congruence(const Constraint& c,
                            dimension_type new_space_dimension,
                            Representation r)
  : expr(c.expression(), new_space_dimension, r),
    modulus_(0) {
  if (!c.is_equality())
    throw_invalid_argument("Congruence(c, space_dim, r)",
                           "constraint c must be an equality.");
}

void
PPL::Congruence::sign_normalize() {
  expr.sign_normalize();
}

void
PPL::Congruence::normalize() {
  PPL_ASSERT(OK());
  sign_normalize();

  if (modulus_ == 0)
    return;

  PPL_DIRTY_TEMP_COEFFICIENT(c);
  c = expr.inhomogeneous_term();
  // Factor the modulus out of the inhomogeneous term.
  c %= modulus_;
  if (c < 0)
    // Make inhomogeneous term positive.
    c += modulus_;
  expr.set_inhomogeneous_term(c);

  PPL_ASSERT(OK());
}

void
PPL::Congruence::strong_normalize() {
  normalize();
  
  Coefficient gcd = expr.gcd(0, expr.space_dimension() + 1);
  if (gcd == 0)
    gcd = modulus_;
  else
    gcd_assign(gcd, modulus_, gcd);
  
  if (gcd != 0 && gcd != 1) {
    expr /= gcd;
    modulus_ /= gcd;
  }
  PPL_ASSERT(OK());
}

void
PPL::Congruence::scale(Coefficient_traits::const_reference factor) {
  if (factor == 1)
    // Nothing to do.
    return;
  
  expr *= factor;
  modulus_ *= factor;
}

void
PPL::Congruence
::affine_preimage(Variable v, const Linear_Expression& e,
                  Coefficient_traits::const_reference denominator) {
  PPL_DIRTY_TEMP_COEFFICIENT(c);
  c = expr.get(v);

  if (c == 0)
    return;

  scale(denominator);

  expr.linear_combine(e, 1, c, 0, e.space_dimension() + 1);

  if (v.space_dimension() > e.space_dimension() || e.get(v) == 0)
    // Not invertible
    expr.set(v, Coefficient_zero());
  else {
    c *= e.get(v);
    expr.set(v, c);
  }
}

PPL::Congruence
PPL::Congruence::create(const Linear_Expression& e1,
                        const Linear_Expression& e2,
                        Representation r) {
  Linear_Expression e(e1,
                      std::max(e1.space_dimension(), e2.space_dimension()),
                      r);
  e -= e2;
  return Congruence(e, 1, Recycle_Input());
}

void
PPL::Congruence::throw_invalid_argument(const char* method,
                                        const char* message) const {
  std::ostringstream s;
  s << "PPL::Congruence::" << method << ":" << std::endl
    << message;
  throw std::invalid_argument(s.str());
}

void
PPL::Congruence::throw_dimension_incompatible(const char* method,
                                              const char* v_name,
                                              const Variable v) const {
  std::ostringstream s;
  s << "this->space_dimension() == " << space_dimension() << ", "
    << v_name << ".space_dimension() == " << v.space_dimension() << ".";
  const std::string str = s.str();
  throw_invalid_argument(method, str.c_str());
}

/*! \relates Parma_Polyhedra_Library::Congruence */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Congruence& c) {
  const dimension_type num_variables = c.space_dimension();
  PPL_DIRTY_TEMP_COEFFICIENT(cv);
  bool first = true;
  const Congruence::expr_type c_e = c.expression();
  for (Congruence::expr_type::const_iterator i = c_e.begin(),
        i_end = c_e.lower_bound(Variable(num_variables)); i != i_end; ++i) {
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
  s << " = " << -c.inhomogeneous_term();
  if (c.is_proper_congruence())
    s << " (mod " << c.modulus() << ")";
  return s;
}

bool
PPL::Congruence::is_tautological() const {
  if (is_equality())
    return (inhomogeneous_term() == 0) && expr.all_homogeneous_terms_are_zero();
  
  return (inhomogeneous_term() % modulus() == 0) && expr.all_homogeneous_terms_are_zero();
}

bool
PPL::Congruence::is_inconsistent() const {
  if (is_equality())
    return (inhomogeneous_term() != 0) && expr.all_homogeneous_terms_are_zero();
  
  return (inhomogeneous_term() % modulus() != 0) && expr.all_homogeneous_terms_are_zero();
}

void
PPL::Congruence::ascii_dump(std::ostream& s) const {
  expr.ascii_dump(s);
  s << " m " << modulus_ << std::endl;
}

PPL_OUTPUT_DEFINITIONS(Congruence)

bool
PPL::Congruence::ascii_load(std::istream& s) {
  expr.ascii_load(s);

  std::string str;
  if (!(s >> str) || str != "m")
    return false;

  if (!(s >> modulus_))
    return false;

  PPL_ASSERT(OK());
  return true;
}

bool
PPL::Congruence::OK() const {
  // Modulus check.
  if (modulus() < 0) {
#ifndef NDEBUG
    std::cerr << "Congruence has a negative modulus " << modulus() << "."
              << std::endl;
#endif
    return false;
  }

  // All tests passed.
  return true;
}

const PPL::Congruence* PPL::Congruence::zero_dim_false_p = 0;
const PPL::Congruence* PPL::Congruence::zero_dim_integrality_p = 0;

void
PPL::Congruence::initialize() {
  PPL_ASSERT(zero_dim_false_p == 0);
  zero_dim_false_p
    = new Congruence((Linear_Expression::zero() %= Coefficient(-1)) / 0);

  PPL_ASSERT(zero_dim_integrality_p == 0);
  zero_dim_integrality_p
    = new Congruence(Linear_Expression::zero() %= Coefficient(-1));
}

void
PPL::Congruence::finalize() {
  PPL_ASSERT(zero_dim_false_p != 0);
  delete zero_dim_false_p;
  zero_dim_false_p = 0;

  PPL_ASSERT(zero_dim_integrality_p != 0);
  delete zero_dim_integrality_p;
  zero_dim_integrality_p = 0;
}
