/* Polyhedron class implementation: inline functions.
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

#ifndef PPL_Polyhedron_inlines_hh
#define PPL_Polyhedron_inlines_hh 1

#include "Generator.defs.hh"
#include "compiler.hh"
#include <algorithm>
#include <deque>

namespace Parma_Polyhedra_Library {

inline memory_size_type
Polyhedron::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline dimension_type
Polyhedron::space_dimension() const {
  return space_dim;
}

inline int32_t
Polyhedron::hash_code() const {
  return hash_code_from_dimension(space_dimension());
}

inline dimension_type
Polyhedron::max_space_dimension() {
  using std::min;
  // One dimension is reserved to have a value of type dimension_type
  // that does not represent a legal dimension.
  return min(std::numeric_limits<dimension_type>::max() - 1,
	     min(Constraint_System::max_space_dimension(),
		 Generator_System::max_space_dimension()
		 )
	     );
}

inline Topology
Polyhedron::topology() const {
  // We can check either one of the two matrices.
  // (`con_sys' is slightly better, since it is placed at offset 0.)
  return con_sys.topology();
}

inline bool
Polyhedron::is_discrete() const {
  return affine_dimension() == 0;
}

inline bool
Polyhedron::is_necessarily_closed() const {
  // We can check either one of the two matrices.
  // (`con_sys' is slightly better, since it is placed at offset 0.)
  return con_sys.is_necessarily_closed();
}

inline void
Polyhedron::upper_bound_assign(const Polyhedron& y) {
  poly_hull_assign(y);
}

inline void
Polyhedron::difference_assign(const Polyhedron& y) {
  poly_difference_assign(y);
}

inline void
Polyhedron::widening_assign(const Polyhedron& y, unsigned* tp) {
  H79_widening_assign(y, tp);
}

inline
Polyhedron::~Polyhedron() {
}

inline void
Polyhedron::m_swap(Polyhedron& y) {
  if (topology() != y.topology())
    throw_topology_incompatible("swap(y)", "y", y);
  using std::swap;
  swap(con_sys, y.con_sys);
  swap(gen_sys, y.gen_sys);
  swap(sat_c, y.sat_c);
  swap(sat_g, y.sat_g);
  swap(status, y.status);
  swap(space_dim, y.space_dim);
}

/*! \relates Polyhedron */
inline void
swap(Polyhedron& x, Polyhedron& y) {
  x.m_swap(y);
}

inline bool
Polyhedron::can_recycle_constraint_systems() {
  return true;
}

inline bool
Polyhedron::can_recycle_congruence_systems() {
  return false;
}

inline bool
Polyhedron::marked_empty() const {
  return status.test_empty();
}

inline bool
Polyhedron::constraints_are_up_to_date() const {
  return status.test_c_up_to_date();
}

inline bool
Polyhedron::generators_are_up_to_date() const {
  return status.test_g_up_to_date();
}

inline bool
Polyhedron::constraints_are_minimized() const {
  return status.test_c_minimized();
}

inline bool
Polyhedron::generators_are_minimized() const {
  return status.test_g_minimized();
}

inline bool
Polyhedron::sat_c_is_up_to_date() const {
  return status.test_sat_c_up_to_date();
}

inline bool
Polyhedron::sat_g_is_up_to_date() const {
  return status.test_sat_g_up_to_date();
}

inline bool
Polyhedron::has_pending_constraints() const {
  return status.test_c_pending();
}

inline bool
Polyhedron::has_pending_generators() const {
  return status.test_g_pending();
}

inline bool
Polyhedron::has_something_pending() const {
  return status.test_c_pending() || status.test_g_pending();
}

inline bool
Polyhedron::can_have_something_pending() const {
  return constraints_are_minimized()
    && generators_are_minimized()
    && (sat_c_is_up_to_date() || sat_g_is_up_to_date());
}

inline bool
Polyhedron::is_empty() const {
  if (marked_empty())
    return true;
  // Try a fast-fail test: if generators are up-to-date and
  // there are no pending constraints, then the generator system
  // (since it is well formed) contains a point.
  if (generators_are_up_to_date() && !has_pending_constraints())
    return false;
  return !minimize();
}

inline void
Polyhedron::set_constraints_up_to_date() {
  status.set_c_up_to_date();
}

inline void
Polyhedron::set_generators_up_to_date() {
  status.set_g_up_to_date();
}

inline void
Polyhedron::set_constraints_minimized() {
  set_constraints_up_to_date();
  status.set_c_minimized();
}

inline void
Polyhedron::set_generators_minimized() {
  set_generators_up_to_date();
  status.set_g_minimized();
}

inline void
Polyhedron::set_constraints_pending() {
  status.set_c_pending();
}

inline void
Polyhedron::set_generators_pending() {
  status.set_g_pending();
}

inline void
Polyhedron::set_sat_c_up_to_date() {
  status.set_sat_c_up_to_date();
}

inline void
Polyhedron::set_sat_g_up_to_date() {
  status.set_sat_g_up_to_date();
}

inline void
Polyhedron::clear_empty() {
  status.reset_empty();
}

inline void
Polyhedron::clear_constraints_minimized() {
  status.reset_c_minimized();
}

inline void
Polyhedron::clear_generators_minimized() {
  status.reset_g_minimized();
}

inline void
Polyhedron::clear_pending_constraints() {
  status.reset_c_pending();
}

inline void
Polyhedron::clear_pending_generators() {
  status.reset_g_pending();
}

inline void
Polyhedron::clear_sat_c_up_to_date() {
  status.reset_sat_c_up_to_date();
  // Can get rid of sat_c here.
}

inline void
Polyhedron::clear_sat_g_up_to_date() {
  status.reset_sat_g_up_to_date();
  // Can get rid of sat_g here.
}

inline void
Polyhedron::clear_constraints_up_to_date() {
  clear_pending_constraints();
  clear_constraints_minimized();
  clear_sat_c_up_to_date();
  clear_sat_g_up_to_date();
  status.reset_c_up_to_date();
  // Can get rid of con_sys here.
}

inline void
Polyhedron::clear_generators_up_to_date() {
  clear_pending_generators();
  clear_generators_minimized();
  clear_sat_c_up_to_date();
  clear_sat_g_up_to_date();
  status.reset_g_up_to_date();
  // Can get rid of gen_sys here.
}

inline bool
Polyhedron::process_pending() const {
  PPL_ASSERT(space_dim > 0 && !marked_empty());
  PPL_ASSERT(has_something_pending());

  Polyhedron& x = const_cast<Polyhedron&>(*this);

  if (x.has_pending_constraints())
    return x.process_pending_constraints();

  PPL_ASSERT(x.has_pending_generators());
  x.process_pending_generators();
  return true;
}

inline bool
Polyhedron::bounds_from_above(const Linear_Expression& expr) const {
  return bounds(expr, true);
}

inline bool
Polyhedron::bounds_from_below(const Linear_Expression& expr) const {
  return bounds(expr, false);
}

inline bool
Polyhedron::maximize(const Linear_Expression& expr,
		     Coefficient& sup_n, Coefficient& sup_d,
		     bool& maximum) const {
  Generator g(point());
  return max_min(expr, true, sup_n, sup_d, maximum, g);
}

inline bool
Polyhedron::maximize(const Linear_Expression& expr,
		     Coefficient& sup_n, Coefficient& sup_d, bool& maximum,
		     Generator& g) const {
  return max_min(expr, true, sup_n, sup_d, maximum, g);
}

inline bool
Polyhedron::minimize(const Linear_Expression& expr,
		     Coefficient& inf_n, Coefficient& inf_d,
		     bool& minimum) const {
  Generator g(point());
  return max_min(expr, false, inf_n, inf_d, minimum, g);
}

inline bool
Polyhedron::minimize(const Linear_Expression& expr,
		     Coefficient& inf_n, Coefficient& inf_d, bool& minimum,
		     Generator& g) const {
  return max_min(expr, false, inf_n, inf_d, minimum, g);
}

inline Constraint_System
Polyhedron::simplified_constraints() const {
  PPL_ASSERT(constraints_are_up_to_date());
  Constraint_System cs(con_sys);
  if (cs.num_pending_rows() > 0)
    cs.unset_pending_rows();
  if (has_pending_constraints() || !constraints_are_minimized())
    cs.simplify();
  return cs;
}

inline Congruence_System
Polyhedron::congruences() const {
  return Congruence_System(minimized_constraints());
}

inline Congruence_System
Polyhedron::minimized_congruences() const {
  return Congruence_System(minimized_constraints());
}

inline void
Polyhedron::add_recycled_congruences(Congruence_System& cgs) {
  add_congruences(cgs);
}

template <typename FP_Format, typename Interval_Info>
inline void
Polyhedron::generalized_refine_with_linear_form_inequality(
	    const Linear_Form< Interval<FP_Format, Interval_Info> >& left,
	    const Linear_Form< Interval<FP_Format, Interval_Info> >& right,
            const Relation_Symbol relsym) {
  switch (relsym) {
  case EQUAL:
    // TODO: see if we can handle this case more efficiently.
    refine_with_linear_form_inequality(left, right, false);
    refine_with_linear_form_inequality(right, left, false);
    break;
  case LESS_THAN:
    refine_with_linear_form_inequality(left, right, true);
    break;
  case LESS_OR_EQUAL:
    refine_with_linear_form_inequality(left, right, false);
    break;
  case GREATER_THAN:
    refine_with_linear_form_inequality(right, left, true);
    break;
  case GREATER_OR_EQUAL:
    refine_with_linear_form_inequality(right, left, false);
    break;
  case NOT_EQUAL:
    break;
  default:
    PPL_UNREACHABLE;
    break;
  }
}

template <typename FP_Format, typename Interval_Info>
inline void
Polyhedron::
refine_fp_interval_abstract_store(
       Box< Interval<FP_Format, Interval_Info> >& store) const {

  // Check that FP_Format is indeed a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<FP_Format>::is_exact,
                     "Polyhedron::refine_fp_interval_abstract_store:"
                     " T not a floating point type.");

  typedef Interval<FP_Format, Interval_Info> FP_Interval_Type;
  store.intersection_assign(Box<FP_Interval_Type>(*this));

}

/*! \relates Polyhedron */
inline bool
operator!=(const Polyhedron& x, const Polyhedron& y) {
  return !(x == y);
}

inline bool
Polyhedron::strictly_contains(const Polyhedron& y) const {
  const Polyhedron& x = *this;
  return x.contains(y) && !y.contains(x);
}

inline void
Polyhedron::drop_some_non_integer_points(Complexity_Class complexity) {
  const Variables_Set* p_vs = 0;
  drop_some_non_integer_points(p_vs, complexity);
}

inline void
Polyhedron::drop_some_non_integer_points(const Variables_Set& vars,
					 Complexity_Class complexity) {
  drop_some_non_integer_points(&vars, complexity);
}


namespace Interfaces {

inline bool
is_necessarily_closed_for_interfaces(const Polyhedron& ph) {
  return ph.is_necessarily_closed();
}

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Polyhedron_inlines_hh)
