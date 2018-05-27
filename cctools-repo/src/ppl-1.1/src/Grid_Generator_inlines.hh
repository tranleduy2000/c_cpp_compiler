/* Grid Generator class implementation: inline functions.
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

#ifndef PPL_Grid_Generator_inlines_hh
#define PPL_Grid_Generator_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline bool
Grid_Generator::is_necessarily_closed() const {
  return true;
}

inline bool
Grid_Generator::is_not_necessarily_closed() const {
  return false;
}

inline bool
Grid_Generator::is_line_or_equality() const {
  return (kind_ == LINE_OR_EQUALITY);
}

inline bool
Grid_Generator::is_ray_or_point_or_inequality() const {
  return (kind_ == RAY_OR_POINT_OR_INEQUALITY);
}

inline Topology
Grid_Generator::topology() const {
  return NECESSARILY_CLOSED;
}

inline void
Grid_Generator::set_is_line_or_equality() {
  kind_ = LINE_OR_EQUALITY;
}

inline void
Grid_Generator::set_is_ray_or_point_or_inequality() {
  kind_ = RAY_OR_POINT_OR_INEQUALITY;
}

inline void
Grid_Generator::set_topology(Topology x) {
  PPL_USED(x);
  PPL_ASSERT(x == NECESSARILY_CLOSED);
}

inline void
Grid_Generator::set_necessarily_closed() {
  set_topology(NECESSARILY_CLOSED);
}

inline void
Grid_Generator::set_not_necessarily_closed() {
  set_topology(NOT_NECESSARILY_CLOSED);
}

inline
Grid_Generator::Grid_Generator(Linear_Expression& e, Type type) {
  swap(expr, e);
  if (type == LINE)
    kind_ = LINE_OR_EQUALITY;
  else
    kind_ = RAY_OR_POINT_OR_INEQUALITY;
  PPL_ASSERT(OK());
}

inline
Grid_Generator::Grid_Generator(Representation r)
  : expr(Coefficient_one(), r),
    kind_(RAY_OR_POINT_OR_INEQUALITY) {
  expr.set_space_dimension(1);
  PPL_ASSERT(OK());
}

inline
Grid_Generator::Grid_Generator(const Grid_Generator& g)
  : expr(g.expr),
    kind_(g.kind_) {
}

inline
Grid_Generator::Grid_Generator(const Grid_Generator& g, Representation r)
  : expr(g.expr, r),
    kind_(g.kind_) {
}

inline
Grid_Generator::Grid_Generator(dimension_type space_dim, Kind kind,
                               Topology topology, Representation r)
  : expr(r),
    kind_(kind) {
  PPL_USED(topology);
  PPL_ASSERT(topology == NECESSARILY_CLOSED);
  expr.set_space_dimension(space_dim + 1);
  PPL_ASSERT(space_dimension() == space_dim);
}

inline
Grid_Generator::Grid_Generator(const Grid_Generator& g,
                               dimension_type space_dim)
  : expr(g.expr, space_dim + 1),
    kind_(g.kind_) {
  PPL_ASSERT(OK());
  PPL_ASSERT(space_dimension() == space_dim);
}

inline
Grid_Generator::Grid_Generator(const Grid_Generator& g,
                               dimension_type space_dim, Representation r)
  : expr(g.expr, space_dim + 1, r),
    kind_(g.kind_) {
  PPL_ASSERT(OK());
  PPL_ASSERT(space_dimension() == space_dim);
}

inline
Grid_Generator::~Grid_Generator() {
}

inline Grid_Generator::expr_type
Grid_Generator::expression() const {
  return expr_type(expr, true);
}

inline Representation
Grid_Generator::representation() const {
  return expr.representation();
}

inline void
Grid_Generator::set_representation(Representation r) {
  expr.set_representation(r);
}

inline dimension_type
Grid_Generator::max_space_dimension() {
  return Linear_Expression::max_space_dimension() - 1;
}

inline dimension_type
Grid_Generator::space_dimension() const {
  return expression().space_dimension();
}

inline void
Grid_Generator::set_space_dimension(dimension_type space_dim) {
  const dimension_type old_space_dim = space_dimension();
  if (space_dim > old_space_dim) {
    expr.set_space_dimension(space_dim + 1);
    expr.swap_space_dimensions(Variable(space_dim), Variable(old_space_dim));
  }
  else {
    expr.swap_space_dimensions(Variable(space_dim), Variable(old_space_dim));
    expr.set_space_dimension(space_dim + 1);
  }
  PPL_ASSERT(space_dimension() == space_dim);
}

inline void
Grid_Generator::set_space_dimension_no_ok(dimension_type space_dim) {
  set_space_dimension(space_dim);
}

inline void
Grid_Generator::shift_space_dimensions(Variable v, dimension_type n) {
  expr.shift_space_dimensions(v, n);
}

inline Grid_Generator::Type
Grid_Generator::type() const {
  if (is_line())
    return LINE;
  return is_point() ? POINT : PARAMETER;
}

inline bool
Grid_Generator::is_line() const {
  return is_line_or_equality();
}

inline bool
Grid_Generator::is_parameter() const {
  return is_parameter_or_point() && is_line_or_parameter();
}

inline bool
Grid_Generator::is_line_or_parameter() const {
  return expr.inhomogeneous_term() == 0;
}

inline bool
Grid_Generator::is_point() const {
  return !is_line_or_parameter();
}

inline bool
Grid_Generator::is_parameter_or_point() const {
  return is_ray_or_point_or_inequality();
}

inline void
Grid_Generator::set_divisor(Coefficient_traits::const_reference d) {
  PPL_ASSERT(!is_line());
  if (is_line_or_parameter())
    expr.set_coefficient(Variable(space_dimension()), d);
  else
    expr.set_inhomogeneous_term(d);
}

inline Coefficient_traits::const_reference
Grid_Generator::divisor() const {
  if (is_line())
    throw_invalid_argument("divisor()", "*this is a line");
  if (is_line_or_parameter())
    return expr.coefficient(Variable(space_dimension()));
  else
    return expr.inhomogeneous_term();
}

inline bool
Grid_Generator::is_equal_at_dimension(dimension_type dim,
                                      const Grid_Generator& y) const {
  const Grid_Generator& x = *this;
  return x.expr.get(dim) * y.divisor() == y.expr.get(dim) * x.divisor();
}

inline void
Grid_Generator::set_is_line() {
  set_is_line_or_equality();
}

inline void
Grid_Generator::set_is_parameter_or_point() {
  set_is_ray_or_point_or_inequality();
}

inline Grid_Generator&
Grid_Generator::operator=(const Grid_Generator& g) {
  Grid_Generator tmp = g;
  swap(*this, tmp);

  return *this;
}

inline Coefficient_traits::const_reference
Grid_Generator::coefficient(const Variable v) const {
  if (v.space_dimension() > space_dimension())
    throw_dimension_incompatible("coefficient(v)", "v", v);
  return expr.coefficient(v);
}

inline memory_size_type
Grid_Generator::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline memory_size_type
Grid_Generator::external_memory_in_bytes() const {
  return expr.external_memory_in_bytes();
}

inline const Grid_Generator&
Grid_Generator::zero_dim_point() {
  PPL_ASSERT(zero_dim_point_p != 0);
  return *zero_dim_point_p;
}

inline void
Grid_Generator::strong_normalize() {
  PPL_ASSERT(!is_parameter());
  expr.normalize();
  sign_normalize();
}

inline void
Grid_Generator::m_swap(Grid_Generator& y) {
  using std::swap;
  swap(expr, y.expr);
  swap(kind_, y.kind_);
}

/*! \relates Grid_Generator */
inline bool
operator==(const Grid_Generator& x, const Grid_Generator& y) {
  return x.is_equivalent_to(y);
}

/*! \relates Grid_Generator */
inline bool
operator!=(const Grid_Generator& x, const Grid_Generator& y) {
  return !(x == y);
}

/*! \relates Grid_Generator */
inline Grid_Generator
grid_line(const Linear_Expression& e, Representation r) {
  return Grid_Generator::grid_line(e, r);
}

/*! \relates Grid_Generator */
inline Grid_Generator
parameter(const Linear_Expression& e,
          Coefficient_traits::const_reference d, Representation r) {
  return Grid_Generator::parameter(e, d, r);
}

/*! \relates Grid_Generator */
inline Grid_Generator
parameter(Representation r) {
  return Grid_Generator::parameter(r);
}

/*! \relates Grid_Generator */
inline Grid_Generator
parameter(const Linear_Expression& e, Representation r) {
  return Grid_Generator::parameter(e, r);
}

/*! \relates Grid_Generator */
inline Grid_Generator
grid_point(const Linear_Expression& e,
           Coefficient_traits::const_reference d, Representation r) {
  return Grid_Generator::grid_point(e, d, r);
}

/*! \relates Grid_Generator */
inline Grid_Generator
grid_point(Representation r) {
  return Grid_Generator::grid_point(r);
}

/*! \relates Grid_Generator */
inline Grid_Generator
grid_point(const Linear_Expression& e, Representation r) {
  return Grid_Generator::grid_point(e, r);
}

/*! \relates Grid_Generator */
inline void
swap(Grid_Generator& x, Grid_Generator& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Grid_Generator_inlines_hh)
