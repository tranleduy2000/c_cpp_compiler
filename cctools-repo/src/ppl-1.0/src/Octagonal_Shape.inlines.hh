/* Octagonal_Shape class implementation: inline functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Octagonal_Shape_inlines_hh
#define PPL_Octagonal_Shape_inlines_hh 1

#include "Constraint_System.defs.hh"
#include "Constraint_System.inlines.hh"
#include "C_Polyhedron.defs.hh"
#include "Grid.defs.hh"
#include "BD_Shape.defs.hh"
#include "Poly_Con_Relation.defs.hh"
#include "Poly_Gen_Relation.defs.hh"
#include "wrap_assign.hh"
#include "assert.hh"
#include <algorithm>

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Octagonal_Shapes {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns the index coherent to \p i.
/*! \relates Parma_Polyhedra_Library::Octagonal_Shape */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
inline dimension_type
coherent_index(const dimension_type i) {
  return (i % 2 != 0) ? (i-1) : (i+1);
}

} // namespace Octagonal_Shapes

} // namespace Implementation

template <typename T>
inline dimension_type
Octagonal_Shape<T>::max_space_dimension() {
  return OR_Matrix<N>::max_num_rows()/2;
}

template <typename T>
inline bool
Octagonal_Shape<T>::marked_zero_dim_univ() const {
  return status.test_zero_dim_univ();
}

template <typename T>
inline bool
Octagonal_Shape<T>::marked_strongly_closed() const {
  return status.test_strongly_closed();
}

template <typename T>
inline bool
Octagonal_Shape<T>::marked_empty() const {
  return status.test_empty();
}

template <typename T>
inline void
Octagonal_Shape<T>::set_zero_dim_univ() {
  status.set_zero_dim_univ();
}

template <typename T>
inline void
Octagonal_Shape<T>::set_empty() {
  status.set_empty();
}

template <typename T>
inline void
Octagonal_Shape<T>::set_strongly_closed() {
  status.set_strongly_closed();
}

template <typename T>
inline void
Octagonal_Shape<T>::reset_strongly_closed() {
  status.reset_strongly_closed();
}

template <typename T>
inline
Octagonal_Shape<T>::Octagonal_Shape(const dimension_type num_dimensions,
				    const Degenerate_Element kind)
  : matrix(num_dimensions), space_dim(num_dimensions), status() {
  if (kind == EMPTY)
    set_empty();
  else if (num_dimensions > 0)
    // A (non zero-dim) universe octagon is strongly closed.
    set_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
inline
Octagonal_Shape<T>::Octagonal_Shape(const Octagonal_Shape& y, Complexity_Class)
  : matrix(y.matrix), space_dim(y.space_dim), status(y.status) {
}

template <typename T>
template <typename U>
inline
Octagonal_Shape<T>::Octagonal_Shape(const Octagonal_Shape<U>& y,
                                    Complexity_Class)
  // For maximum precision, enforce shortest-path closure
  // before copying the DB matrix.
  : matrix((y.strong_closure_assign(), y.matrix)),
    space_dim(y.space_dim),
    status() {
  // TODO: handle flags properly, possibly taking special cases into account.
  if (y.marked_empty())
    set_empty();
  else if (y.marked_zero_dim_univ())
    set_zero_dim_univ();
}

template <typename T>
inline
Octagonal_Shape<T>::Octagonal_Shape(const Constraint_System& cs)
  : matrix(cs.space_dimension()),
    space_dim(cs.space_dimension()),
    status() {
  if (cs.space_dimension() > 0)
    // A (non zero-dim) universe octagon is strongly closed.
    set_strongly_closed();
  add_constraints(cs);
}

template <typename T>
inline
Octagonal_Shape<T>::Octagonal_Shape(const Congruence_System& cgs)
  : matrix(cgs.space_dimension()),
    space_dim(cgs.space_dimension()),
    status() {
  if (cgs.space_dimension() > 0)
    // A (non zero-dim) universe octagon is strongly closed.
    set_strongly_closed();
  add_congruences(cgs);
}

template <typename T>
template <typename Interval>
inline
Octagonal_Shape<T>::Octagonal_Shape(const Box<Interval>& box,
                                    Complexity_Class)
  : matrix(box.space_dimension()),
    space_dim(box.space_dimension()),
    status() {
  // Check for emptiness for maximum precision.
  if (box.is_empty())
    set_empty();
  else if (box.space_dimension() > 0) {
    // A (non zero-dim) universe OS is strongly closed.
    set_strongly_closed();
    refine_with_constraints(box.constraints());
  }
}

template <typename T>
inline
Octagonal_Shape<T>::Octagonal_Shape(const Grid& grid,
                                    Complexity_Class)
  : matrix(grid.space_dimension()),
    space_dim(grid.space_dimension()),
    status() {
  if (grid.space_dimension() > 0)
    // A (non zero-dim) universe OS is strongly closed.
    set_strongly_closed();
  // Taking minimized congruences ensures maximum precision.
  refine_with_congruences(grid.minimized_congruences());
}

template <typename T>
template <typename U>
inline
Octagonal_Shape<T>::Octagonal_Shape(const BD_Shape<U>& bd,
                                    Complexity_Class)
  : matrix(bd.space_dimension()),
    space_dim(bd.space_dimension()),
    status() {
  // Check for emptiness for maximum precision.
  if (bd.is_empty())
    set_empty();
  else if (bd.space_dimension() > 0) {
    // A (non zero-dim) universe OS is strongly closed.
    set_strongly_closed();
    refine_with_constraints(bd.constraints());
  }
}

template <typename T>
inline Congruence_System
Octagonal_Shape<T>::congruences() const {
  return minimized_congruences();
}

template <typename T>
inline Octagonal_Shape<T>&
Octagonal_Shape<T>::operator=(const Octagonal_Shape& y) {
  matrix = y.matrix;
  space_dim = y.space_dim;
  status = y.status;
  return *this;
}

template <typename T>
inline
Octagonal_Shape<T>::~Octagonal_Shape() {
}

template <typename T>
inline void
Octagonal_Shape<T>::m_swap(Octagonal_Shape& y) {
  using std::swap;
  swap(matrix, y.matrix);
  swap(space_dim, y.space_dim);
  swap(status, y.status);
}

template <typename T>
inline dimension_type
Octagonal_Shape<T>::space_dimension() const {
  return space_dim;
}

template <typename T>
inline bool
Octagonal_Shape<T>::is_discrete() const {
  return affine_dimension() == 0;
}

template <typename T>
inline bool
Octagonal_Shape<T>::is_empty() const {
  strong_closure_assign();
  return marked_empty();
}

template <typename T>
inline bool
Octagonal_Shape<T>::bounds_from_above(const Linear_Expression& expr) const {
  return bounds(expr, true);
}

template <typename T>
inline bool
Octagonal_Shape<T>::bounds_from_below(const Linear_Expression& expr) const {
  return bounds(expr, false);
}

template <typename T>
inline bool
Octagonal_Shape<T>::maximize(const Linear_Expression& expr,
			     Coefficient& sup_n, Coefficient& sup_d,
			     bool& maximum) const {
  return max_min(expr, true, sup_n, sup_d, maximum);
}

template <typename T>
inline bool
Octagonal_Shape<T>::maximize(const Linear_Expression& expr,
			     Coefficient& sup_n, Coefficient& sup_d,
			     bool& maximum,
			     Generator& g) const {
  return max_min(expr, true, sup_n, sup_d, maximum, g);
}

template <typename T>
inline bool
Octagonal_Shape<T>::minimize(const Linear_Expression& expr,
			     Coefficient& inf_n, Coefficient& inf_d,
			     bool& minimum) const {
  return max_min(expr, false, inf_n, inf_d, minimum);
}

template <typename T>
inline bool
Octagonal_Shape<T>::minimize(const Linear_Expression& expr,
			     Coefficient& inf_n, Coefficient& inf_d,
			     bool& minimum,
			     Generator& g) const {
  return max_min(expr, false, inf_n, inf_d, minimum, g);
}

template <typename T>
inline bool
Octagonal_Shape<T>::is_topologically_closed() const {
  return true;
}

template <typename T>
inline void
Octagonal_Shape<T>::topological_closure_assign() {
}

/*! \relates Octagonal_Shape */
template <typename T>
inline bool
operator==(const Octagonal_Shape<T>& x, const Octagonal_Shape<T>& y) {
  if (x.space_dim != y.space_dim)
    // Dimension-incompatible OSs are different.
    return false;

  // Zero-dim OSs are equal if and only if they are both empty or universe.
  if (x.space_dim == 0) {
    if (x.marked_empty())
      return y.marked_empty();
    else
      return !y.marked_empty();
  }

  x.strong_closure_assign();
  y.strong_closure_assign();
  // If one of two octagons is empty, then they are equal if and only if
  // the other octagon is empty too.
  if (x.marked_empty())
    return y.marked_empty();
  if (y.marked_empty())
    return false;
  // Strong closure is a canonical form.
  return x.matrix == y.matrix;
}

/*! \relates Octagonal_Shape */
template <typename T>
inline bool
operator!=(const Octagonal_Shape<T>& x, const Octagonal_Shape<T>& y) {
  return !(x == y);
}

template <typename T>
inline const typename Octagonal_Shape<T>::coefficient_type&
Octagonal_Shape<T>::matrix_at(const dimension_type i,
			      const dimension_type j) const {
  PPL_ASSERT(i < matrix.num_rows() && j < matrix.num_rows());
  using namespace Implementation::Octagonal_Shapes;
  return (j < matrix.row_size(i))
    ? matrix[i][j]
    : matrix[coherent_index(j)][coherent_index(i)];
}

template <typename T>
inline typename Octagonal_Shape<T>::coefficient_type&
Octagonal_Shape<T>::matrix_at(const dimension_type i,
			      const dimension_type j) {
  PPL_ASSERT(i < matrix.num_rows() && j < matrix.num_rows());
  using namespace Implementation::Octagonal_Shapes;
  return (j < matrix.row_size(i))
    ? matrix[i][j]
    : matrix[coherent_index(j)][coherent_index(i)];
}

template <typename T>
inline Constraint_System
Octagonal_Shape<T>::minimized_constraints() const {
  strong_reduction_assign();
  return constraints();
}

template <typename T>
inline void
Octagonal_Shape<T>::add_octagonal_constraint(const dimension_type i,
					     const dimension_type j,
					     const N& k) {
  // Private method: the caller has to ensure the following.
#ifndef NDEBUG
  PPL_ASSERT(i < 2*space_dim && j < 2*space_dim && i != j);
  typename OR_Matrix<N>::row_iterator m_i = matrix.row_begin() + i;
  PPL_ASSERT(j < m_i.row_size());
#endif
  N& r_i_j = matrix[i][j];
  if (r_i_j > k) {
    r_i_j = k;
    if (marked_strongly_closed())
      reset_strongly_closed();
  }
}

template <typename T>
inline void
Octagonal_Shape<T>
::add_octagonal_constraint(const dimension_type i,
			   const dimension_type j,
			   Coefficient_traits::const_reference numer,
			   Coefficient_traits::const_reference denom) {
#ifndef NDEBUG
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(i < 2*space_dim && j < 2*space_dim && i != j);
  typename OR_Matrix<N>::row_iterator m_i = matrix.row_begin() + i;
  PPL_ASSERT(j < m_i.row_size());
  PPL_ASSERT(denom != 0);
#endif
  PPL_DIRTY_TEMP(N, k);
  div_round_up(k, numer, denom);
  add_octagonal_constraint(i, j, k);
}

template <typename T>
inline void
Octagonal_Shape<T>::add_constraints(const Constraint_System& cs) {
  for (Constraint_System::const_iterator i = cs.begin(),
         i_end = cs.end(); i != i_end; ++i)
    add_constraint(*i);
}

template <typename T>
inline void
Octagonal_Shape<T>::add_recycled_constraints(Constraint_System& cs) {
  add_constraints(cs);
}

template <typename T>
inline void
Octagonal_Shape<T>::add_recycled_congruences(Congruence_System& cgs) {
  add_congruences(cgs);
}

template <typename T>
inline void
Octagonal_Shape<T>::add_congruences(const Congruence_System& cgs) {
  for (Congruence_System::const_iterator i = cgs.begin(),
         cgs_end = cgs.end(); i != cgs_end; ++i)
    add_congruence(*i);
}

template <typename T>
inline void
Octagonal_Shape<T>::refine_with_constraint(const Constraint& c) {
  // Dimension-compatibility check.
  if (c.space_dimension() > space_dimension())
    throw_dimension_incompatible("refine_with_constraint(c)", c);

  if (!marked_empty())
    refine_no_check(c);
}

template <typename T>
inline void
Octagonal_Shape<T>::refine_with_constraints(const Constraint_System& cs) {
  // Dimension-compatibility check.
  if (cs.space_dimension() > space_dimension())
    throw_invalid_argument("refine_with_constraints(cs)",
                           "cs and *this are space-dimension incompatible");

  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); !marked_empty() && i != cs_end; ++i)
    refine_no_check(*i);
}

template <typename T>
inline void
Octagonal_Shape<T>::refine_with_congruence(const Congruence& cg) {
  const dimension_type cg_space_dim = cg.space_dimension();
  // Dimension-compatibility check.
  if (cg_space_dim > space_dimension())
    throw_dimension_incompatible("refine_with_congruence(cg)", cg);

  if (!marked_empty())
    refine_no_check(cg);
}

template <typename T>
void
Octagonal_Shape<T>::refine_with_congruences(const Congruence_System& cgs) {
  // Dimension-compatibility check.
  if (cgs.space_dimension() > space_dimension())
    throw_invalid_argument("refine_with_congruences(cgs)",
                           "cgs and *this are space-dimension incompatible");

  for (Congruence_System::const_iterator i = cgs.begin(),
	 cgs_end = cgs.end(); !marked_empty() && i != cgs_end; ++i)
    refine_no_check(*i);
}

template <typename T>
inline void
Octagonal_Shape<T>::refine_no_check(const Congruence& cg) {
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(cg.space_dimension() <= space_dimension());

  if (cg.is_proper_congruence()) {
    if (cg.is_inconsistent())
      set_empty();
    // Other proper congruences are just ignored.
    return;
  }

  PPL_ASSERT(cg.is_equality());
  Constraint c(cg);
  refine_no_check(c);
}

template <typename T>
inline bool
Octagonal_Shape<T>::can_recycle_constraint_systems() {
  return false;
}

template <typename T>
inline bool
Octagonal_Shape<T>::can_recycle_congruence_systems() {
  return false;
}

template <typename T>
inline void
Octagonal_Shape<T>
::remove_higher_space_dimensions(const dimension_type new_dimension) {
  // Dimension-compatibility check.
  if (new_dimension > space_dim)
    throw_dimension_incompatible("remove_higher_space_dimension(nd)",
				 new_dimension);
  // The removal of no dimensions from any octagon is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from an octagon in a 0-dim space.
  if (new_dimension == space_dim) {
    PPL_ASSERT(OK());
    return;
  }

  strong_closure_assign();
  matrix.shrink(new_dimension);
  // When we remove all dimensions from a non-empty octagon,
  // we obtain the zero-dimensional universe octagon.
  if (new_dimension == 0 && !marked_empty())
    set_zero_dim_univ();
  space_dim = new_dimension;
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::wrap_assign(const Variables_Set& vars,
                                Bounded_Integer_Type_Width w,
                                Bounded_Integer_Type_Representation r,
                                Bounded_Integer_Type_Overflow o,
                                const Constraint_System* cs_p,
                                unsigned complexity_threshold,
                                bool wrap_individually) {
  Implementation::wrap_assign(*this,
                              vars, w, r, o, cs_p,
                              complexity_threshold, wrap_individually,
                              "Octagonal_Shape");
}

template <typename T>
inline void
Octagonal_Shape<T>::widening_assign(const Octagonal_Shape& y, unsigned* tp) {
  BHMZ05_widening_assign(y, tp);
}

template <typename T>
inline void
Octagonal_Shape<T>::CC76_extrapolation_assign(const Octagonal_Shape& y,
					      unsigned* tp) {
  static N stop_points[] = {
    N(-2, ROUND_UP),
    N(-1, ROUND_UP),
    N( 0, ROUND_UP),
    N( 1, ROUND_UP),
    N( 2, ROUND_UP)
  };
  CC76_extrapolation_assign(y,
			    stop_points,
			    stop_points
			    + sizeof(stop_points)/sizeof(stop_points[0]),
			    tp);
}

template <typename T>
inline void
Octagonal_Shape<T>::time_elapse_assign(const Octagonal_Shape& y) {
  // Dimension-compatibility check.
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("time_elapse_assign(y)", y);
  // Compute time-elapse on polyhedra.
  // TODO: provide a direct implementation.
  C_Polyhedron ph_x(constraints());
  C_Polyhedron ph_y(y.constraints());
  ph_x.time_elapse_assign(ph_y);
  Octagonal_Shape<T> x(ph_x);
  m_swap(x);
  PPL_ASSERT(OK());
}

template <typename T>
inline bool
Octagonal_Shape<T>::strictly_contains(const Octagonal_Shape& y) const {
  const Octagonal_Shape<T>& x = *this;
  return x.contains(y) && !y.contains(x);
}

template <typename T>
template <typename Interval_Info>
inline void
Octagonal_Shape<T>::generalized_refine_with_linear_form_inequality(
		    const Linear_Form< Interval<T, Interval_Info> >& left,
		    const Linear_Form< Interval<T, Interval_Info> >& right,
                    const Relation_Symbol relsym) {
  switch (relsym) {
  case EQUAL:
    // TODO: see if we can handle this case more efficiently.
    refine_with_linear_form_inequality(left, right);
    refine_with_linear_form_inequality(right, left);
    break;
  case LESS_THAN:
  case LESS_OR_EQUAL:
    refine_with_linear_form_inequality(left, right);
    break;
  case GREATER_THAN:
  case GREATER_OR_EQUAL:
    refine_with_linear_form_inequality(right, left);
    break;
  case NOT_EQUAL:
    break;
  default:
    PPL_UNREACHABLE;
    break;
  }
}

template <typename T>
template <typename Interval_Info>
inline void
Octagonal_Shape<T>::
refine_fp_interval_abstract_store(
	  Box< Interval<T, Interval_Info> >& store) const {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                     "Octagonal_Shape<T>::refine_fp_interval_abstract_store:"
                     " T not a floating point type.");

  typedef Interval<T, Interval_Info> FP_Interval_Type;
  store.intersection_assign(Box<FP_Interval_Type>(*this));

}

/*! \relates Octagonal_Shape */
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const Octagonal_Shape<T>& x,
			    const Octagonal_Shape<T>& y,
			    const Rounding_Dir dir,
			    Temp& tmp0,
			    Temp& tmp1,
			    Temp& tmp2) {
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    return false;

  // Zero-dim OSs are equal if and only if they are both empty or universe.
  if (x.space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires strong closure.
  x.strong_closure_assign();
  y.strong_closure_assign();

  // If one of two OSs is empty, then they are equal if and only if
  // the other OS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  return rectilinear_distance_assign(r, x.matrix, y.matrix, dir,
				     tmp0, tmp1, tmp2);
}

/*! \relates Octagonal_Shape */
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const Octagonal_Shape<T>& x,
			    const Octagonal_Shape<T>& y,
			    const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return rectilinear_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Octagonal_Shape */
template <typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const Octagonal_Shape<T>& x,
			    const Octagonal_Shape<T>& y,
			    const Rounding_Dir dir) {
  return rectilinear_distance_assign<To, To, T>(r, x, y, dir);
}

/*! \relates Octagonal_Shape */
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const Octagonal_Shape<T>& x,
			  const Octagonal_Shape<T>& y,
			  const Rounding_Dir dir,
			  Temp& tmp0,
			  Temp& tmp1,
			  Temp& tmp2) {
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    return false;

  // Zero-dim OSs are equal if and only if they are both empty or universe.
  if (x.space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires strong closure.
  x.strong_closure_assign();
  y.strong_closure_assign();

  // If one of two OSs is empty, then they are equal if and only if
  // the other OS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  return euclidean_distance_assign(r, x.matrix, y.matrix, dir,
				   tmp0, tmp1, tmp2);
}

/*! \relates Octagonal_Shape */
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const Octagonal_Shape<T>& x,
			  const Octagonal_Shape<T>& y,
			  const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return euclidean_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Octagonal_Shape */
template <typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const Octagonal_Shape<T>& x,
			  const Octagonal_Shape<T>& y,
			  const Rounding_Dir dir) {
  return euclidean_distance_assign<To, To, T>(r, x, y, dir);
}

/*! \relates Octagonal_Shape */
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const Octagonal_Shape<T>& x,
			   const Octagonal_Shape<T>& y,
			   const Rounding_Dir dir,
			   Temp& tmp0,
			   Temp& tmp1,
			   Temp& tmp2) {
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    return false;

  // Zero-dim OSs are equal if and only if they are both empty or universe.
  if (x.space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires strong closure.
  x.strong_closure_assign();
  y.strong_closure_assign();

  // If one of two OSs is empty, then they are equal if and only if
  // the other OS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  return l_infinity_distance_assign(r, x.matrix, y.matrix, dir,
				    tmp0, tmp1, tmp2);
}

/*! \relates Octagonal_Shape */
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const Octagonal_Shape<T>& x,
			   const Octagonal_Shape<T>& y,
			   const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return l_infinity_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Octagonal_Shape */
template <typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const Octagonal_Shape<T>& x,
			   const Octagonal_Shape<T>& y,
			   const Rounding_Dir dir) {
  return l_infinity_distance_assign<To, To, T>(r, x, y, dir);
}

template <typename T>
inline memory_size_type
Octagonal_Shape<T>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename T>
inline int32_t
Octagonal_Shape<T>::hash_code() const {
  return hash_code_from_dimension(space_dimension());
}

template <typename T>
inline void
Octagonal_Shape<T>::drop_some_non_integer_points_helper(N& elem) {
  if (!is_integer(elem)) {
#ifndef NDEBUG
    Result r =
#endif
    floor_assign_r(elem, elem, ROUND_DOWN);
    PPL_ASSERT(r == V_EQ);
    reset_strongly_closed();
  }
}

/*! \relates Octagonal_Shape */
template <typename T>
inline void
swap(Octagonal_Shape<T>& x, Octagonal_Shape<T>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Octagonal_Shape_inlines_hh)
