/* BD_Shape class implementation: inline functions.
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

#ifndef PPL_BD_Shape_inlines_hh
#define PPL_BD_Shape_inlines_hh 1

#include "Constraint_System.defs.hh"
#include "Constraint_System.inlines.hh"
#include "C_Polyhedron.defs.hh"
#include "Grid.defs.hh"
#include "Octagonal_Shape.defs.hh"
#include "Poly_Con_Relation.defs.hh"
#include "Poly_Gen_Relation.defs.hh"
#include "Temp.defs.hh"
#include "meta_programming.hh"
#include "wrap_assign.hh"
#include "assert.hh"
#include <vector>
#include <iostream>
#include <algorithm>

namespace Parma_Polyhedra_Library {

template <typename T>
inline dimension_type
BD_Shape<T>::max_space_dimension() {
  // One dimension is reserved to have a value of type dimension_type
  // that does not represent a legal dimension.
  return std::min(DB_Matrix<N>::max_num_rows() - 1,
		  DB_Matrix<N>::max_num_columns() - 1);
}

template <typename T>
inline bool
BD_Shape<T>::marked_zero_dim_univ() const {
  return status.test_zero_dim_univ();
}

template <typename T>
inline bool
BD_Shape<T>::marked_empty() const {
  return status.test_empty();
}

template <typename T>
inline bool
BD_Shape<T>::marked_shortest_path_closed() const {
  return status.test_shortest_path_closed();
}

template <typename T>
inline bool
BD_Shape<T>::marked_shortest_path_reduced() const {
  return status.test_shortest_path_reduced();
}

template <typename T>
inline void
BD_Shape<T>::set_zero_dim_univ() {
  status.set_zero_dim_univ();
}

template <typename T>
inline void
BD_Shape<T>::set_empty() {
  status.set_empty();
}

template <typename T>
inline void
BD_Shape<T>::set_shortest_path_closed() {
  status.set_shortest_path_closed();
}

template <typename T>
inline void
BD_Shape<T>::set_shortest_path_reduced() {
  status.set_shortest_path_reduced();
}

template <typename T>
inline void
BD_Shape<T>::reset_shortest_path_closed() {
  status.reset_shortest_path_closed();
}

template <typename T>
inline void
BD_Shape<T>::reset_shortest_path_reduced() {
  status.reset_shortest_path_reduced();
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const dimension_type num_dimensions,
		      const Degenerate_Element kind)
  : dbm(num_dimensions + 1), status(), redundancy_dbm() {
  if (kind == EMPTY)
    set_empty();
  else {
    if (num_dimensions > 0)
      // A (non zero-dim) universe BDS is closed.
      set_shortest_path_closed();
  }
  PPL_ASSERT(OK());
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const BD_Shape& y, Complexity_Class)
  : dbm(y.dbm), status(y.status), redundancy_dbm() {
  if (y.marked_shortest_path_reduced())
    redundancy_dbm = y.redundancy_dbm;
}

template <typename T>
template <typename U>
inline
BD_Shape<T>::BD_Shape(const BD_Shape<U>& y, Complexity_Class)
  // For maximum precision, enforce shortest-path closure
  // before copying the DB matrix.
  : dbm((y.shortest_path_closure_assign(), y.dbm)),
    status(),
    redundancy_dbm() {
  // TODO: handle flags properly, possibly taking special cases into account.
  if (y.marked_empty())
    set_empty();
  else if (y.marked_zero_dim_univ())
    set_zero_dim_univ();
}

template <typename T>
inline Congruence_System
BD_Shape<T>::congruences() const {
  return minimized_congruences();
}

template <typename T>
inline void
BD_Shape<T>::add_constraints(const Constraint_System& cs) {
  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); i != cs_end; ++i)
    add_constraint(*i);
}

template <typename T>
inline void
BD_Shape<T>::add_recycled_constraints(Constraint_System& cs) {
  add_constraints(cs);
}

template <typename T>
inline void
BD_Shape<T>::add_congruences(const Congruence_System& cgs) {
  for (Congruence_System::const_iterator i = cgs.begin(),
	 cgs_end = cgs.end(); i != cgs_end; ++i)
    add_congruence(*i);
}

template <typename T>
inline void
BD_Shape<T>::add_recycled_congruences(Congruence_System& cgs) {
  add_congruences(cgs);
}

template <typename T>
inline void
BD_Shape<T>::refine_with_constraint(const Constraint& c) {
  const dimension_type c_space_dim = c.space_dimension();
  // Dimension-compatibility check.
  if (c_space_dim > space_dimension())
    throw_dimension_incompatible("refine_with_constraint(c)", c);

  if (!marked_empty())
    refine_no_check(c);
}

template <typename T>
inline void
BD_Shape<T>::refine_with_constraints(const Constraint_System& cs) {
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
BD_Shape<T>::refine_with_congruence(const Congruence& cg) {
  const dimension_type cg_space_dim = cg.space_dimension();
  // Dimension-compatibility check.
  if (cg_space_dim > space_dimension())
    throw_dimension_incompatible("refine_with_congruence(cg)", cg);

  if (!marked_empty())
    refine_no_check(cg);
}

template <typename T>
void
BD_Shape<T>::refine_with_congruences(const Congruence_System& cgs) {
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
BD_Shape<T>::refine_no_check(const Congruence& cg) {
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
BD_Shape<T>::can_recycle_constraint_systems() {
  return false;
}


template <typename T>
inline bool
BD_Shape<T>::can_recycle_congruence_systems() {
  return false;
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const Constraint_System& cs)
  : dbm(cs.space_dimension() + 1), status(), redundancy_dbm() {
  if (cs.space_dimension() > 0)
    // A (non zero-dim) universe BDS is shortest-path closed.
    set_shortest_path_closed();
  add_constraints(cs);
}

template <typename T>
template <typename Interval>
inline
BD_Shape<T>::BD_Shape(const Box<Interval>& box,
                      Complexity_Class)
  : dbm(box.space_dimension() + 1), status(), redundancy_dbm() {
  // Check emptiness for maximum precision.
  if (box.is_empty())
    set_empty();
  else if (box.space_dimension() > 0) {
    // A (non zero-dim) universe BDS is shortest-path closed.
    set_shortest_path_closed();
    refine_with_constraints(box.constraints());
  }
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const Grid& grid,
                      Complexity_Class)
  : dbm(grid.space_dimension() + 1), status(), redundancy_dbm() {
  if (grid.space_dimension() > 0)
    // A (non zero-dim) universe BDS is shortest-path closed.
    set_shortest_path_closed();
  // Taking minimized congruences ensures maximum precision.
  refine_with_congruences(grid.minimized_congruences());
}

template <typename T>
template <typename U>
inline
BD_Shape<T>::BD_Shape(const Octagonal_Shape<U>& os,
                      Complexity_Class)
  : dbm(os.space_dimension() + 1), status(), redundancy_dbm() {
  // Check for emptiness for maximum precision.
  if (os.is_empty())
    set_empty();
  else if (os.space_dimension() > 0) {
    // A (non zero-dim) universe BDS is shortest-path closed.
    set_shortest_path_closed();
    refine_with_constraints(os.constraints());
    // After refining, shortest-path closure is possibly lost
    // (even when `os' was strongly closed: recall that U
    // is possibly different from T).
  }
}

template <typename T>
inline BD_Shape<T>&
BD_Shape<T>::operator=(const BD_Shape& y) {
  dbm = y.dbm;
  status = y.status;
  if (y.marked_shortest_path_reduced())
    redundancy_dbm = y.redundancy_dbm;
  return *this;
}

template <typename T>
inline
BD_Shape<T>::~BD_Shape() {
}

template <typename T>
inline void
BD_Shape<T>::m_swap(BD_Shape& y) {
  using std::swap;
  swap(dbm, y.dbm);
  swap(status, y.status);
  swap(redundancy_dbm, y.redundancy_dbm);
}

template <typename T>
inline dimension_type
BD_Shape<T>::space_dimension() const {
  return dbm.num_rows() - 1;
}

template <typename T>
inline bool
BD_Shape<T>::is_empty() const {
  shortest_path_closure_assign();
  return marked_empty();
}

template <typename T>
inline bool
BD_Shape<T>::bounds_from_above(const Linear_Expression& expr) const {
  return bounds(expr, true);
}

template <typename T>
inline bool
BD_Shape<T>::bounds_from_below(const Linear_Expression& expr) const {
  return bounds(expr, false);
}

template <typename T>
inline bool
BD_Shape<T>::maximize(const Linear_Expression& expr,
		      Coefficient& sup_n, Coefficient& sup_d,
		      bool& maximum) const {
  return max_min(expr, true, sup_n, sup_d, maximum);
}

template <typename T>
inline bool
BD_Shape<T>::maximize(const Linear_Expression& expr,
		      Coefficient& sup_n, Coefficient& sup_d, bool& maximum,
		      Generator& g) const {
  return max_min(expr, true, sup_n, sup_d, maximum, g);
}

template <typename T>
inline bool
BD_Shape<T>::minimize(const Linear_Expression& expr,
		      Coefficient& inf_n, Coefficient& inf_d,
		      bool& minimum) const {
  return max_min(expr, false, inf_n, inf_d, minimum);
}

template <typename T>
inline bool
BD_Shape<T>::minimize(const Linear_Expression& expr,
		      Coefficient& inf_n, Coefficient& inf_d, bool& minimum,
		      Generator& g) const {
  return max_min(expr, false, inf_n, inf_d, minimum, g);
}

template <typename T>
inline bool
BD_Shape<T>::is_topologically_closed() const {
  return true;
}

template <typename T>
inline bool
BD_Shape<T>::is_discrete() const {
  return affine_dimension() == 0;
}

template <typename T>
inline void
BD_Shape<T>::topological_closure_assign() {
}

/*! \relates BD_Shape */
template <typename T>
inline bool
operator==(const BD_Shape<T>& x, const BD_Shape<T>& y) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty())
      return y.marked_empty();
    else
      return !y.marked_empty();
  }

  // The exact equivalence test requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal
  // if and only if the other BDS is empty too.
  if (x.marked_empty())
    return y.marked_empty();
  if (y.marked_empty())
    return false;
  // Check for syntactic equivalence of the two (shortest-path closed)
  // systems of bounded differences.
  return x.dbm == y.dbm;
}

/*! \relates BD_Shape */
template <typename T>
inline bool
operator!=(const BD_Shape<T>& x, const BD_Shape<T>& y) {
  return !(x == y);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const BD_Shape<T>& x,
			    const BD_Shape<T>& y,
			    const Rounding_Dir dir,
			    Temp& tmp0,
			    Temp& tmp1,
			    Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal if and only if
  // the other BDS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  return rectilinear_distance_assign(r, x.dbm, y.dbm, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const BD_Shape<T>& x,
			    const BD_Shape<T>& y,
			    const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return rectilinear_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const BD_Shape<T>& x,
			    const BD_Shape<T>& y,
			    const Rounding_Dir dir) {
  return rectilinear_distance_assign<To, To, T>(r, x, y, dir);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const BD_Shape<T>& x,
			  const BD_Shape<T>& y,
			  const Rounding_Dir dir,
			  Temp& tmp0,
			  Temp& tmp1,
			  Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal if and only if
  // the other BDS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  return euclidean_distance_assign(r, x.dbm, y.dbm, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const BD_Shape<T>& x,
			  const BD_Shape<T>& y,
			  const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return euclidean_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const BD_Shape<T>& x,
			  const BD_Shape<T>& y,
			  const Rounding_Dir dir) {
  return euclidean_distance_assign<To, To, T>(r, x, y, dir);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const BD_Shape<T>& x,
			   const BD_Shape<T>& y,
			   const Rounding_Dir dir,
			   Temp& tmp0,
			   Temp& tmp1,
			   Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal if and only if
  // the other BDS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  return l_infinity_distance_assign(r, x.dbm, y.dbm, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const BD_Shape<T>& x,
			   const BD_Shape<T>& y,
			   const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return l_infinity_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const BD_Shape<T>& x,
			   const BD_Shape<T>& y,
			   const Rounding_Dir dir) {
  return l_infinity_distance_assign<To, To, T>(r, x, y, dir);
}

template <typename T>
inline void
BD_Shape<T>::add_dbm_constraint(const dimension_type i,
				const dimension_type j,
				const N& k) {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(i <= space_dimension() && j <= space_dimension() && i != j);
  N& dbm_ij = dbm[i][j];
  if (dbm_ij > k) {
    dbm_ij = k;
    if (marked_shortest_path_closed())
      reset_shortest_path_closed();
  }
}

template <typename T>
inline void
BD_Shape<T>::add_dbm_constraint(const dimension_type i,
				const dimension_type j,
				Coefficient_traits::const_reference numer,
				Coefficient_traits::const_reference denom) {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(i <= space_dimension() && j <= space_dimension() && i != j);
  PPL_ASSERT(denom != 0);
  PPL_DIRTY_TEMP(N, k);
  div_round_up(k, numer, denom);
  add_dbm_constraint(i, j, k);
}

template <typename T>
inline void
BD_Shape<T>::time_elapse_assign(const BD_Shape& y) {
  // Dimension-compatibility check.
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("time_elapse_assign(y)", y);
  // Compute time-elapse on polyhedra.
  // TODO: provide a direct implementation.
  C_Polyhedron ph_x(constraints());
  C_Polyhedron ph_y(y.constraints());
  ph_x.time_elapse_assign(ph_y);
  BD_Shape<T> x(ph_x);
  m_swap(x);
  PPL_ASSERT(OK());
}

template <typename T>
inline bool
BD_Shape<T>::strictly_contains(const BD_Shape& y) const {
  const BD_Shape<T>& x = *this;
  return x.contains(y) && !y.contains(x);
}

template <typename T>
inline bool
BD_Shape<T>::upper_bound_assign_if_exact(const BD_Shape& y) {
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("upper_bound_assign_if_exact(y)", y);
#if 0
  return BFT00_upper_bound_assign_if_exact(y);
#else
  const bool integer_upper_bound = false;
  return BHZ09_upper_bound_assign_if_exact<integer_upper_bound>(y);
#endif
}

template <typename T>
inline bool
BD_Shape<T>::integer_upper_bound_assign_if_exact(const BD_Shape& y) {
  PPL_COMPILE_TIME_CHECK(std::numeric_limits<T>::is_integer,
                         "BD_Shape<T>::integer_upper_bound_assign_if_exact(y):"
                         " T in not an integer datatype.");
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("integer_upper_bound_assign_if_exact(y)", y);
  const bool integer_upper_bound = true;
  return BHZ09_upper_bound_assign_if_exact<integer_upper_bound>(y);
}

template <typename T>
inline void
BD_Shape<T>
::remove_higher_space_dimensions(const dimension_type new_dimension) {
  // Dimension-compatibility check: the variable having
  // maximum index is the one occurring last in the set.
  const dimension_type space_dim = space_dimension();
  if (new_dimension > space_dim)
    throw_dimension_incompatible("remove_higher_space_dimensions(nd)",
				 new_dimension);

  // The removal of no dimensions from any BDS is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from a zero-dim space BDS.
  if (new_dimension == space_dim) {
    PPL_ASSERT(OK());
    return;
  }

  // Shortest-path closure is necessary as in remove_space_dimensions().
  shortest_path_closure_assign();
  dbm.resize_no_copy(new_dimension + 1);

  // Shortest-path closure is maintained.
  // TODO: see whether or not reduction can be (efficiently!) maintained too.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();

  // If we removed _all_ dimensions from a non-empty BDS,
  // the zero-dim universe BDS has been obtained.
  if (new_dimension == 0 && !marked_empty())
    set_zero_dim_univ();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::wrap_assign(const Variables_Set& vars,
                         Bounded_Integer_Type_Width w,
                         Bounded_Integer_Type_Representation r,
                         Bounded_Integer_Type_Overflow o,
                         const Constraint_System* cs_p,
                         unsigned complexity_threshold,
                         bool wrap_individually) {
  Implementation::wrap_assign(*this,
                              vars, w, r, o, cs_p,
                              complexity_threshold, wrap_individually,
                              "BD_Shape");
}

template <typename T>
inline void
BD_Shape<T>::CC76_extrapolation_assign(const BD_Shape& y, unsigned* tp) {
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
BD_Shape<T>::H79_widening_assign(const BD_Shape& y, unsigned* tp) {
  // Compute the H79 widening on polyhedra.
  // TODO: provide a direct implementation.
  C_Polyhedron ph_x(constraints());
  C_Polyhedron ph_y(y.constraints());
  ph_x.H79_widening_assign(ph_y, tp);
  BD_Shape x(ph_x);
  m_swap(x);
  PPL_ASSERT(OK());
}

template <typename T>
inline void
BD_Shape<T>::widening_assign(const BD_Shape& y, unsigned* tp) {
  H79_widening_assign(y, tp);
}

template <typename T>
inline void
BD_Shape<T>::limited_H79_extrapolation_assign(const BD_Shape& y,
					      const Constraint_System& cs,
					      unsigned* tp) {
  // Compute the limited H79 extrapolation on polyhedra.
  // TODO: provide a direct implementation.
  C_Polyhedron ph_x(constraints());
  C_Polyhedron ph_y(y.constraints());
  ph_x.limited_H79_extrapolation_assign(ph_y, cs, tp);
  BD_Shape x(ph_x);
  m_swap(x);
  PPL_ASSERT(OK());
}

template <typename T>
inline memory_size_type
BD_Shape<T>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename T>
inline int32_t
BD_Shape<T>::hash_code() const {
  return hash_code_from_dimension(space_dimension());
}

template <typename T>
template <typename Interval_Info>
inline void
BD_Shape<T>::generalized_refine_with_linear_form_inequality(
	     const Linear_Form<Interval<T, Interval_Info> >& left,
	     const Linear_Form<Interval<T, Interval_Info> >& right,
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
  }
}

template <typename T>
template <typename Interval_Info>
inline void
BD_Shape<T>
::refine_fp_interval_abstract_store(Box<Interval<T, Interval_Info> >&
                                    store) const {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                     "BD_Shape<T>::refine_fp_interval_abstract_store:"
                     " T not a floating point type.");

  typedef Interval<T, Interval_Info> FP_Interval_Type;
  store.intersection_assign(Box<FP_Interval_Type>(*this));
}

template <typename T>
inline void
BD_Shape<T>::drop_some_non_integer_points_helper(N& elem) {
  if (!is_integer(elem)) {
    Result r = floor_assign_r(elem, elem, ROUND_DOWN);
    PPL_USED(r);
    PPL_ASSERT(r == V_EQ);
    reset_shortest_path_closed();
  }
}

/*! \relates BD_Shape */
template <typename T>
inline void
swap(BD_Shape<T>& x, BD_Shape<T>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_BD_Shape_inlines_hh)
