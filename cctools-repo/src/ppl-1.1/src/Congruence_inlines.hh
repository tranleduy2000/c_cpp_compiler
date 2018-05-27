/* Congruence class implementation: inline functions.
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

#ifndef PPL_Congruence_inlines_hh
#define PPL_Congruence_inlines_hh 1

#include <sstream>

namespace Parma_Polyhedra_Library {

inline
Congruence::Congruence(Representation r)
  : expr(r) {
  PPL_ASSERT(OK());
}

inline
Congruence::Congruence(const Congruence& cg)
  : expr(cg.expr), modulus_(cg.modulus_) {
}

inline
Congruence::Congruence(const Congruence& cg, Representation r)
  : expr(cg.expr, r), modulus_(cg.modulus_) {
}

inline
Congruence::Congruence(const Congruence& cg,
                       dimension_type new_space_dimension)
  : expr(cg.expr, new_space_dimension), modulus_(cg.modulus_) {
  PPL_ASSERT(OK());
}

inline
Congruence::Congruence(const Congruence& cg,
                       dimension_type new_space_dimension,
                       Representation r)
  : expr(cg.expr, new_space_dimension, r), modulus_(cg.modulus_) {
  PPL_ASSERT(OK());
}

inline Representation
Congruence::representation() const {
  return expr.representation();
}

inline void
Congruence::set_representation(Representation r) {
  expr.set_representation(r);
}

inline Congruence::expr_type
Congruence::expression() const {
  return expr_type(expr);
}

inline void
Congruence::set_space_dimension(dimension_type n) {
  expr.set_space_dimension(n);
  PPL_ASSERT(OK());
}

inline void
Congruence::shift_space_dimensions(Variable v, dimension_type n) {
  expr.shift_space_dimensions(v, n);
}

inline
Congruence::~Congruence() {
}

inline
Congruence::Congruence(Linear_Expression& le,
                       Coefficient_traits::const_reference m,
                       Recycle_Input)
  : modulus_(m) {
  PPL_ASSERT(m >= 0);
  swap(expr, le);

  PPL_ASSERT(OK());
}

inline Congruence
Congruence::create(const Linear_Expression& e,
                   Coefficient_traits::const_reference n,
                   Representation r) {
  Linear_Expression diff(e, r);
  diff -= n;
  const Congruence cg(diff, 1, Recycle_Input());
  return cg;
}

inline Congruence
Congruence::create(Coefficient_traits::const_reference n,
                   const Linear_Expression& e,
                   Representation r) {
  Linear_Expression diff(e, r);
  diff -= n;
  const Congruence cg(diff, 1, Recycle_Input());
  return cg;
}

/*! \relates Parma_Polyhedra_Library::Congruence */
inline Congruence
operator%=(const Linear_Expression& e1, const Linear_Expression& e2) {
  return Congruence::create(e1, e2);
}

/*! \relates Parma_Polyhedra_Library::Congruence */
inline Congruence
operator%=(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  return Congruence::create(e, n);
}

/*! \relates Parma_Polyhedra_Library::Congruence */
inline Congruence
operator/(const Congruence& cg, Coefficient_traits::const_reference k) {
  Congruence ret = cg;
  ret /= k;
  return ret;
}

inline const Congruence&
Congruence::zero_dim_integrality() {
  return *zero_dim_integrality_p;
}

inline const Congruence&
Congruence::zero_dim_false() {
  return *zero_dim_false_p;
}

inline Congruence&
Congruence::operator=(const Congruence& y) {
  Congruence tmp = y;
  swap(*this, tmp);
  return *this;
}

/*! \relates Congruence */
inline Congruence
operator/(const Constraint& c, Coefficient_traits::const_reference m) {
  Congruence ret(c);
  ret /= m;
  return ret;
}

inline Congruence&
Congruence::operator/=(Coefficient_traits::const_reference k) {
  if (k >= 0)
    modulus_ *= k;
  else
    modulus_ *= -k;
  return *this;
}

/*! \relates Congruence */
inline bool
operator==(const Congruence& x, const Congruence& y) {
  if (x.space_dimension() != y.space_dimension())
    return false;
  Congruence x_temp(x);
  Congruence y_temp(y);
  x_temp.strong_normalize();
  y_temp.strong_normalize();
  return x_temp.expr.is_equal_to(y_temp.expr)
    && x_temp.modulus() == y_temp.modulus();
}

/*! \relates Congruence */
inline bool
operator!=(const Congruence& x, const Congruence& y) {
  return !(x == y);
}

inline dimension_type
Congruence::max_space_dimension() {
  return Linear_Expression::max_space_dimension();
}

inline dimension_type
Congruence::space_dimension() const {
  return expr.space_dimension();
}

inline Coefficient_traits::const_reference
Congruence::coefficient(const Variable v) const {
  if (v.space_dimension() > space_dimension())
    throw_dimension_incompatible("coefficient(v)", "v", v);
  return expr.coefficient(v);
}

inline void
Congruence::permute_space_dimensions(const std::vector<Variable>& cycles) {
  expr.permute_space_dimensions(cycles);
}

inline Coefficient_traits::const_reference
Congruence::inhomogeneous_term() const {
  return expr.inhomogeneous_term();
}

inline Coefficient_traits::const_reference
Congruence::modulus() const {
  return modulus_;
}

inline void
Congruence::set_modulus(Coefficient_traits::const_reference m) {
  modulus_ = m;
  PPL_ASSERT(OK());
}

inline bool
Congruence::is_proper_congruence() const {
  return modulus() > 0;
}

inline bool
Congruence::is_equality() const {
  return modulus() == 0;
}

inline bool
Congruence::is_equal_at_dimension(Variable v,
                                  const Congruence& cg) const {
  return coefficient(v) * cg.modulus() == cg.coefficient(v) * modulus();
}

inline memory_size_type
Congruence::external_memory_in_bytes() const {
  return expr.external_memory_in_bytes()
         + Parma_Polyhedra_Library::external_memory_in_bytes(modulus_);
}

inline memory_size_type
Congruence::total_memory_in_bytes() const {
  return external_memory_in_bytes() + sizeof(*this);
}

inline void
Congruence::m_swap(Congruence& y) {
  using std::swap;
  swap(expr, y.expr);
  swap(modulus_, y.modulus_);
}

inline void
Congruence::swap_space_dimensions(Variable v1, Variable v2) {
  expr.swap_space_dimensions(v1, v2);
}

/*! \relates Congruence */
inline void
swap(Congruence& x, Congruence& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Congruence_inlines_hh)
