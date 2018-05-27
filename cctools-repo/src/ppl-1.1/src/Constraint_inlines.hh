/* Constraint class implementation: inline functions.
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

#ifndef PPL_Constraint_inlines_hh
#define PPL_Constraint_inlines_hh 1

#include "Linear_Expression_defs.hh"

namespace Parma_Polyhedra_Library {

inline bool
Constraint::is_necessarily_closed() const {
  return (topology_ == NECESSARILY_CLOSED);
}

inline bool
Constraint::is_not_necessarily_closed() const {
  return !is_necessarily_closed();
}

inline Constraint::expr_type
Constraint::expression() const {
  return expr_type(expr, is_not_necessarily_closed());
}

inline dimension_type
Constraint::space_dimension() const {
  return expression().space_dimension();
}

inline void
Constraint::shift_space_dimensions(Variable v, dimension_type n) {
  expr.shift_space_dimensions(v, n);
}

inline bool
Constraint::is_line_or_equality() const {
  return (kind_ == LINE_OR_EQUALITY);
}

inline bool
Constraint::is_ray_or_point_or_inequality() const {
  return (kind_ == RAY_OR_POINT_OR_INEQUALITY);
}

inline Topology
Constraint::topology() const {
  return topology_;
}

inline void
Constraint::set_is_line_or_equality() {
  kind_ = LINE_OR_EQUALITY;
}

inline void
Constraint::set_is_ray_or_point_or_inequality() {
  kind_ = RAY_OR_POINT_OR_INEQUALITY;
}

inline void
Constraint::set_topology(Topology x) {
  if (topology() == x)
    return;
  if (topology() == NECESSARILY_CLOSED) {
    // Add a column for the epsilon dimension.
    expr.set_space_dimension(expr.space_dimension() + 1);
  }
  else {
    PPL_ASSERT(expr.space_dimension() != 0);
    expr.set_space_dimension(expr.space_dimension() - 1);
  }
  topology_ = x;
}

inline void
Constraint::mark_as_necessarily_closed() {
  PPL_ASSERT(is_not_necessarily_closed());
  topology_ = NECESSARILY_CLOSED;
}

inline void
Constraint::mark_as_not_necessarily_closed() {
  PPL_ASSERT(is_necessarily_closed());
  topology_ = NOT_NECESSARILY_CLOSED;
}

inline void
Constraint::set_necessarily_closed() {
  set_topology(NECESSARILY_CLOSED);
}

inline void
Constraint::set_not_necessarily_closed() {
  set_topology(NOT_NECESSARILY_CLOSED);
}

inline
Constraint::Constraint(Representation r)
  : expr(r),
    kind_(RAY_OR_POINT_OR_INEQUALITY),
    topology_(NECESSARILY_CLOSED) {
  PPL_ASSERT(OK());
}

inline
Constraint::Constraint(dimension_type space_dim, Kind kind, Topology topology,
                       Representation r)
  : expr(r),
    kind_(kind),
    topology_(topology) {
  expr.set_space_dimension(space_dim + 1);
  PPL_ASSERT(space_dimension() == space_dim);
  PPL_ASSERT(OK());
}

inline
Constraint::Constraint(Linear_Expression& e, Kind kind, Topology topology)
  : kind_(kind),
    topology_(topology) {
  PPL_ASSERT(kind != RAY_OR_POINT_OR_INEQUALITY || topology == NOT_NECESSARILY_CLOSED);
  swap(expr, e);
  if (topology == NOT_NECESSARILY_CLOSED)
    // Add the epsilon dimension.
    expr.set_space_dimension(expr.space_dimension() + 1);
  strong_normalize();
  PPL_ASSERT(OK());
}

inline
Constraint::Constraint(Linear_Expression& e, Type type, Topology topology)
  : topology_(topology) {
  PPL_ASSERT(type != STRICT_INEQUALITY || topology == NOT_NECESSARILY_CLOSED);
  swap(expr, e);
  if (topology == NOT_NECESSARILY_CLOSED)
    expr.set_space_dimension(expr.space_dimension() + 1);
  if (type == EQUALITY)
    kind_ = LINE_OR_EQUALITY;
  else
    kind_ = RAY_OR_POINT_OR_INEQUALITY;
  strong_normalize();
  PPL_ASSERT(OK());
}

inline
Constraint::Constraint(const Constraint& c)
  : expr(c.expr),
    kind_(c.kind_),
    topology_(c.topology_) {
  // NOTE: This does not call PPL_ASSERT(OK()) because this is called by OK().
}

inline
Constraint::Constraint(const Constraint& c, Representation r)
  : expr(c.expr, r),
    kind_(c.kind_),
    topology_(c.topology_) {
  PPL_ASSERT(OK());
}

inline
Constraint::Constraint(const Constraint& c, const dimension_type space_dim)
  : expr(c.expr, c.is_necessarily_closed() ? space_dim : (space_dim + 1)),
    kind_(c.kind_), topology_(c.topology_) {
  PPL_ASSERT(space_dimension() == space_dim);
  PPL_ASSERT(OK());
}

inline
Constraint::Constraint(const Constraint& c, const dimension_type space_dim,
                       Representation r)
  : expr(c.expr, c.is_necessarily_closed() ? space_dim : (space_dim + 1), r),
    kind_(c.kind_), topology_(c.topology_) {
  PPL_ASSERT(space_dimension() == space_dim);
  PPL_ASSERT(OK());
}

inline
Constraint::~Constraint() {
}

inline Constraint&
Constraint::operator=(const Constraint& c) {
  Constraint tmp = c;
  swap(*this, tmp);

  return *this;
}

inline Representation
Constraint::representation() const {
  return expr.representation();
}

inline void
Constraint::set_representation(Representation r) {
  expr.set_representation(r);
}

inline dimension_type
Constraint::max_space_dimension() {
  return Linear_Expression::max_space_dimension();
}

inline void
Constraint::set_space_dimension_no_ok(dimension_type space_dim) {
  const dimension_type old_expr_space_dim = expr.space_dimension();
  if (topology() == NECESSARILY_CLOSED) {
    expr.set_space_dimension(space_dim);
  }
  else {
    const dimension_type old_space_dim = space_dimension();
    if (space_dim > old_space_dim) {
      expr.set_space_dimension(space_dim + 1);
      expr.swap_space_dimensions(Variable(space_dim), Variable(old_space_dim));
    }
    else {
      expr.swap_space_dimensions(Variable(space_dim), Variable(old_space_dim));
      expr.set_space_dimension(space_dim + 1);
    }
  }
  PPL_ASSERT(space_dimension() == space_dim);
  if (expr.space_dimension() < old_expr_space_dim)
    strong_normalize();
}

inline void
Constraint::set_space_dimension(dimension_type space_dim) {
  set_space_dimension_no_ok(space_dim);
  PPL_ASSERT(OK());
}

inline bool
Constraint::remove_space_dimensions(const Variables_Set& vars) {
  expr.remove_space_dimensions(vars);
  return true;
}

inline bool
Constraint::is_equality() const {
  return is_line_or_equality();
}

inline bool
Constraint::is_inequality() const {
  return is_ray_or_point_or_inequality();
}

inline Constraint::Type
Constraint::type() const {
  if (is_equality())
    return EQUALITY;
  if (is_necessarily_closed())
    return NONSTRICT_INEQUALITY;
  if (epsilon_coefficient() < 0)
    return STRICT_INEQUALITY;
  else
    return NONSTRICT_INEQUALITY;
}

inline bool
Constraint::is_nonstrict_inequality() const {
  return type() == NONSTRICT_INEQUALITY;
}

inline bool
Constraint::is_strict_inequality() const {
  return type() == STRICT_INEQUALITY;
}

inline void
Constraint::set_is_equality() {
  set_is_line_or_equality();
}

inline void
Constraint::set_is_inequality() {
  set_is_ray_or_point_or_inequality();
}

inline Coefficient_traits::const_reference
Constraint::coefficient(const Variable v) const {
  if (v.space_dimension() > space_dimension())
    throw_dimension_incompatible("coefficient(v)", "v", v);
  return expr.coefficient(v);
}

inline Coefficient_traits::const_reference
Constraint::inhomogeneous_term() const {
  return expr.inhomogeneous_term();
}

inline memory_size_type
Constraint::external_memory_in_bytes() const {
  return expr.external_memory_in_bytes();
}

inline memory_size_type
Constraint::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline void
Constraint::strong_normalize() {
  expr.normalize();
  sign_normalize();
}

/*! \relates Constraint */
inline bool
operator==(const Constraint& x, const Constraint& y) {
  return x.is_equivalent_to(y);
}

/*! \relates Constraint */
inline bool
operator!=(const Constraint& x, const Constraint& y) {
  return !x.is_equivalent_to(y);
}

/*! \relates Constraint */
inline Constraint
operator==(const Linear_Expression& e1, const Linear_Expression& e2) {
  Linear_Expression diff(e1,
                         std::max(e1.space_dimension(), e2.space_dimension()),
                         Constraint::default_representation);
  diff -= e2;
  return Constraint(diff, Constraint::EQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator==(Variable v1, Variable v2) {
  if (v1.space_dimension() > v2.space_dimension())
    swap(v1, v2);
  PPL_ASSERT(v1.space_dimension() <= v2.space_dimension());

  Linear_Expression diff(v1, Constraint::default_representation);
  diff -= v2;
  return Constraint(diff, Constraint::EQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>=(const Linear_Expression& e1, const Linear_Expression& e2) {
  Linear_Expression diff(e1,
                         std::max(e1.space_dimension(), e2.space_dimension()),
                         Constraint::default_representation);
  diff -= e2;
  return Constraint(diff, Constraint::NONSTRICT_INEQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>=(const Variable v1, const Variable v2) {
  Linear_Expression diff(Constraint::default_representation);
  diff.set_space_dimension(std::max(v1.space_dimension(),
                                    v2.space_dimension()));
  diff += v1;
  diff -= v2;
  return Constraint(diff, Constraint::NONSTRICT_INEQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>(const Linear_Expression& e1, const Linear_Expression& e2) {
  Linear_Expression diff(e1, Constraint::default_representation);
  diff -= e2;
  Constraint c(diff, Constraint::STRICT_INEQUALITY, NOT_NECESSARILY_CLOSED);

  // NOTE: this also enforces normalization.
  c.set_epsilon_coefficient(-1);
  PPL_ASSERT(c.OK());

  return c;
}

/*! \relates Constraint */
inline Constraint
operator>(const Variable v1, const Variable v2) {
  Linear_Expression diff(Constraint::default_representation);
  diff.set_space_dimension(std::max(v1.space_dimension(),
                                    v2.space_dimension()));
  diff += v1;
  diff -= v2;
  Constraint c(diff, Constraint::STRICT_INEQUALITY, NOT_NECESSARILY_CLOSED);

  c.set_epsilon_coefficient(-1);
  PPL_ASSERT(c.OK());

  return c;
}

/*! \relates Constraint */
inline Constraint
operator==(Coefficient_traits::const_reference n, const Linear_Expression& e) {
  Linear_Expression diff(e, Constraint::default_representation);
  neg_assign(diff);
  diff += n;
  return Constraint(diff, Constraint::EQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>=(Coefficient_traits::const_reference n, const Linear_Expression& e) {
  Linear_Expression diff(e, Constraint::default_representation);
  neg_assign(diff);
  diff += n;
  return Constraint(diff, Constraint::NONSTRICT_INEQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>(Coefficient_traits::const_reference n, const Linear_Expression& e) {
  Linear_Expression diff(e, Constraint::default_representation);
  neg_assign(diff);
  diff += n;
  Constraint c(diff, Constraint::STRICT_INEQUALITY, NOT_NECESSARILY_CLOSED);

  // NOTE: this also enforces normalization.
  c.set_epsilon_coefficient(-1);
  PPL_ASSERT(c.OK());

  return c;
}

/*! \relates Constraint */
inline Constraint
operator==(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  Linear_Expression diff(e, Constraint::default_representation);
  diff -= n;
  return Constraint(diff, Constraint::EQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>=(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  Linear_Expression diff(e, Constraint::default_representation);
  diff -= n;
  return Constraint(diff, Constraint::NONSTRICT_INEQUALITY, NECESSARILY_CLOSED);
}

/*! \relates Constraint */
inline Constraint
operator>(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  Linear_Expression diff(e, Constraint::default_representation);
  diff -= n;
  Constraint c(diff, Constraint::STRICT_INEQUALITY, NOT_NECESSARILY_CLOSED);

  // NOTE: this also enforces normalization.
  c.set_epsilon_coefficient(-1);
  PPL_ASSERT(c.OK());

  return c;
}

/*! \relates Constraint */
inline Constraint
operator<=(const Linear_Expression& e1, const Linear_Expression& e2) {
  return e2 >= e1;
}

/*! \relates Constraint */
inline Constraint
operator<=(const Variable v1, const Variable v2) {
  return v2 >= v1;
}

/*! \relates Constraint */
inline Constraint
operator<=(Coefficient_traits::const_reference n, const Linear_Expression& e) {
  return e >= n;
}

/*! \relates Constraint */
inline Constraint
operator<=(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  return n >= e;
}

/*! \relates Constraint */
inline Constraint
operator<(const Linear_Expression& e1, const Linear_Expression& e2) {
  return e2 > e1;
}

/*! \relates Constraint */
inline Constraint
operator<(const Variable v1, const Variable v2) {
  return v2 > v1;
}

/*! \relates Constraint */
inline Constraint
operator<(Coefficient_traits::const_reference n, const Linear_Expression& e) {
  return e > n;
}

/*! \relates Constraint */
inline Constraint
operator<(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  return n > e;
}

inline const Constraint&
Constraint::zero_dim_false() {
  PPL_ASSERT(zero_dim_false_p != 0);
  return *zero_dim_false_p;
}

inline const Constraint&
Constraint::zero_dim_positivity() {
  PPL_ASSERT(zero_dim_positivity_p != 0);
  return *zero_dim_positivity_p;
}

inline const Constraint&
Constraint::epsilon_geq_zero() {
  PPL_ASSERT(epsilon_geq_zero_p != 0);
  return *epsilon_geq_zero_p;
}

inline const Constraint&
Constraint::epsilon_leq_one() {
  PPL_ASSERT(epsilon_leq_one_p != 0);
  return *epsilon_leq_one_p;
}

inline void
Constraint::m_swap(Constraint& y) {
  using std::swap;
  swap(expr, y.expr);
  swap(kind_, y.kind_);
  swap(topology_, y.topology_);
}

inline Coefficient_traits::const_reference
Constraint::epsilon_coefficient() const {
  PPL_ASSERT(is_not_necessarily_closed());
  return expr.coefficient(Variable(expr.space_dimension() - 1));
}

inline void
Constraint::set_epsilon_coefficient(Coefficient_traits::const_reference n) {
  PPL_ASSERT(is_not_necessarily_closed());
  expr.set_coefficient(Variable(expr.space_dimension() - 1), n);
}

/*! \relates Constraint */
inline void
swap(Constraint& x, Constraint& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Constraint_inlines_hh)
