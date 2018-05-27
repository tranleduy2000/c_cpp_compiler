/* Grid class implementation: inline functions.
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

#ifndef PPL_Grid_inlines_hh
#define PPL_Grid_inlines_hh 1

#include "Grid_Generator.defs.hh"
#include "Grid_Generator_System.defs.hh"
#include "Grid_Generator_System.inlines.hh"
#include <algorithm>

namespace Parma_Polyhedra_Library {

inline bool
Grid::marked_empty() const {
  return status.test_empty();
}

inline bool
Grid::congruences_are_up_to_date() const {
  return status.test_c_up_to_date();
}

inline bool
Grid::generators_are_up_to_date() const {
  return status.test_g_up_to_date();
}

inline bool
Grid::congruences_are_minimized() const {
  return status.test_c_minimized();
}

inline bool
Grid::generators_are_minimized() const {
  return status.test_g_minimized();
}

inline void
Grid::set_generators_up_to_date() {
  status.set_g_up_to_date();
}

inline void
Grid::set_congruences_up_to_date() {
  status.set_c_up_to_date();
}

inline void
Grid::set_congruences_minimized() {
  set_congruences_up_to_date();
  status.set_c_minimized();
}

inline void
Grid::set_generators_minimized() {
  set_generators_up_to_date();
  status.set_g_minimized();
}

inline void
Grid::clear_empty() {
  status.reset_empty();
}

inline void
Grid::clear_congruences_minimized() {
  status.reset_c_minimized();
}

inline void
Grid::clear_generators_minimized() {
  status.reset_g_minimized();
}

inline void
Grid::clear_congruences_up_to_date() {
  clear_congruences_minimized();
  status.reset_c_up_to_date();
  // Can get rid of con_sys here.
}

inline void
Grid::clear_generators_up_to_date() {
  clear_generators_minimized();
  status.reset_g_up_to_date();
  // Can get rid of gen_sys here.
}

inline dimension_type
Grid::max_space_dimension() {
  // One dimension is reserved to have a value of type dimension_type
  // that does not represent a legal dimension.
  return std::min(std::numeric_limits<dimension_type>::max() - 1,
		  std::min(Congruence_System::max_space_dimension(),
			   Grid_Generator_System::max_space_dimension()
			   )
		  );
}

inline
Grid::Grid(dimension_type num_dimensions,
	   const Degenerate_Element kind)
  : con_sys(),
    gen_sys(check_space_dimension_overflow(num_dimensions,
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(n, k)",
                                           "n exceeds the maximum "
                                           "allowed space dimension")) {
  construct(num_dimensions, kind);
  PPL_ASSERT(OK());
}

inline
Grid::Grid(const Congruence_System& cgs)
  : con_sys(check_space_dimension_overflow(cgs.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(cgs)",
                                           "the space dimension of cgs "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(cgs.space_dimension()) {
  Congruence_System cgs_copy(cgs);
  construct(cgs_copy);
}

inline
Grid::Grid(Congruence_System& cgs, Recycle_Input)
  : con_sys(check_space_dimension_overflow(cgs.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(cgs, recycle)",
                                           "the space dimension of cgs "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(cgs.space_dimension()) {
  construct(cgs);
}

inline
Grid::Grid(const Grid_Generator_System& ggs)
  : con_sys(check_space_dimension_overflow(ggs.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(ggs)",
                                           "the space dimension of ggs "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(ggs.space_dimension()) {
  Grid_Generator_System ggs_copy(ggs);
  construct(ggs_copy);
}

inline
Grid::Grid(Grid_Generator_System& ggs, Recycle_Input)
  : con_sys(check_space_dimension_overflow(ggs.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(ggs, recycle)",
                                           "the space dimension of ggs "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(ggs.space_dimension()) {
  construct(ggs);
}

template <typename U>
inline
Grid::Grid(const BD_Shape<U>& bd, Complexity_Class)
  : con_sys(check_space_dimension_overflow(bd.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(bd)",
                                           "the space dimension of bd "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(bd.space_dimension()) {
  Congruence_System cgs = bd.congruences();
  construct(cgs);
}

template <typename U>
inline
Grid::Grid(const Octagonal_Shape<U>& os, Complexity_Class)
  : con_sys(check_space_dimension_overflow(os.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(os)",
                                           "the space dimension of os "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(os.space_dimension()) {
  Congruence_System cgs = os.congruences();
  construct(cgs);
}

inline
Grid::~Grid() {
}

inline dimension_type
Grid::space_dimension() const {
  return space_dim;
}

inline memory_size_type
Grid::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline int32_t
Grid::hash_code() const {
  return hash_code_from_dimension(space_dimension());
}

inline Constraint_System
Grid::constraints() const {
  return Constraint_System(congruences());
}

inline Constraint_System
Grid::minimized_constraints() const {
  return Constraint_System(minimized_congruences());
}

inline void
Grid::m_swap(Grid& y) {
  using std::swap;
  swap(con_sys, y.con_sys);
  swap(gen_sys, y.gen_sys);
  swap(status, y.status);
  swap(space_dim, y.space_dim);
  swap(dim_kinds, y.dim_kinds);
}

inline void
Grid::add_congruence(const Congruence& cg) {
  // Dimension-compatibility check.
  if (space_dim < cg.space_dimension())
    throw_dimension_incompatible("add_congruence(cg)", "cg", cg);

  if (!marked_empty())
    add_congruence_no_check(cg);
}

inline void
Grid::add_congruences(const Congruence_System& cgs) {
  // TODO: this is just an executable specification.
  // Space dimension compatibility check.
  if (space_dim < cgs.space_dimension())
    throw_dimension_incompatible("add_congruences(cgs)", "cgs", cgs);

  if (!marked_empty()) {
    Congruence_System cgs_copy = cgs;
    add_recycled_congruences(cgs_copy);
  }
}

inline void
Grid::refine_with_congruence(const Congruence& cg) {
  add_congruence(cg);
}

inline void
Grid::refine_with_congruences(const Congruence_System& cgs) {
  add_congruences(cgs);
}

inline bool
Grid::can_recycle_constraint_systems() {
  return true;
}

inline bool
Grid::can_recycle_congruence_systems() {
  return true;
}

inline void
Grid::add_constraint(const Constraint& c) {
  // Space dimension compatibility check.
  if (space_dim < c.space_dimension())
    throw_dimension_incompatible("add_constraint(c)", "c", c);
  if (!marked_empty())
    add_constraint_no_check(c);
}

inline void
Grid::add_recycled_constraints(Constraint_System& cs) {
  // TODO: really recycle the constraints.
  add_constraints(cs);
}

inline bool
Grid::bounds_from_above(const Linear_Expression& expr) const {
  return bounds(expr, "bounds_from_above(e)");
}

inline bool
Grid::bounds_from_below(const Linear_Expression& expr) const {
  return bounds(expr, "bounds_from_below(e)");
}

inline bool
Grid::maximize(const Linear_Expression& expr,
	       Coefficient& sup_n, Coefficient& sup_d, bool& maximum) const {
  return max_min(expr, "maximize(e, ...)", sup_n, sup_d, maximum);
}

inline bool
Grid::maximize(const Linear_Expression& expr,
	       Coefficient& sup_n, Coefficient& sup_d, bool& maximum,
	       Generator& point) const {
  return max_min(expr, "maximize(e, ...)", sup_n, sup_d, maximum, &point);
}

inline bool
Grid::minimize(const Linear_Expression& expr,
	       Coefficient& inf_n, Coefficient& inf_d, bool& minimum) const {
  return max_min(expr, "minimize(e, ...)", inf_n, inf_d, minimum);
}

inline bool
Grid::minimize(const Linear_Expression& expr,
	       Coefficient& inf_n, Coefficient& inf_d, bool& minimum,
	       Generator& point) const {
  return max_min(expr, "minimize(e, ...)", inf_n, inf_d, minimum, &point);
}

inline void
Grid::normalize_divisors(Grid_Generator_System& sys) {
  PPL_DIRTY_TEMP_COEFFICIENT(divisor);
  divisor = 1;
  normalize_divisors(sys, divisor);
}

/*! \relates Grid */
inline bool
operator!=(const Grid& x, const Grid& y) {
  return !(x == y);
}

inline bool
Grid::strictly_contains(const Grid& y) const {
  const Grid& x = *this;
  return x.contains(y) && !y.contains(x);
}

inline void
Grid::topological_closure_assign() {
}

/*! \relates Grid */
inline void
swap(Grid& x, Grid& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Grid_inlines_hh)
