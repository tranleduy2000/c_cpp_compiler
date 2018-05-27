/* Box class implementation: inline functions.
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

#ifndef PPL_Box_inlines_hh
#define PPL_Box_inlines_hh 1

#include "Boundary.defs.hh"
#include "Constraint_System.defs.hh"
#include "Constraint_System.inlines.hh"
#include "Congruence_System.defs.hh"
#include "Congruence_System.inlines.hh"
#include "distances.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename ITV>
inline bool
Box<ITV>::marked_empty() const {
  return status.test_empty_up_to_date() && status.test_empty();
}

template <typename ITV>
inline void
Box<ITV>::set_empty() {
  status.set_empty();
  status.set_empty_up_to_date();
}

template <typename ITV>
inline void
Box<ITV>::set_nonempty() {
  status.reset_empty();
  status.set_empty_up_to_date();
}

template <typename ITV>
inline void
Box<ITV>::set_empty_up_to_date() {
  status.set_empty_up_to_date();
}

template <typename ITV>
inline void
Box<ITV>::reset_empty_up_to_date() {
  return status.reset_empty_up_to_date();
}

template <typename ITV>
inline
Box<ITV>::Box(const Box& y, Complexity_Class)
  : seq(y.seq), status(y.status) {
}

template <typename ITV>
inline Box<ITV>&
Box<ITV>::operator=(const Box& y) {
  seq = y.seq;
  status = y.status;
  return *this;
}

template <typename ITV>
inline void
Box<ITV>::m_swap(Box& y) {
  Box& x = *this;
  using std::swap;
  swap(x.seq, y.seq);
  swap(x.status, y.status);
}

template <typename ITV>
inline
Box<ITV>::Box(const Constraint_System& cs, Recycle_Input) {
  // Recycling is useless: just delegate.
  Box<ITV> tmp(cs);
  this->m_swap(tmp);
}

template <typename ITV>
inline
Box<ITV>::Box(const Generator_System& gs, Recycle_Input) {
  // Recycling is useless: just delegate.
  Box<ITV> tmp(gs);
  this->m_swap(tmp);
}

template <typename ITV>
inline
Box<ITV>::Box(const Congruence_System& cgs, Recycle_Input) {
  // Recycling is useless: just delegate.
  Box<ITV> tmp(cgs);
  this->m_swap(tmp);
}

template <typename ITV>
inline memory_size_type
Box<ITV>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename ITV>
inline dimension_type
Box<ITV>::space_dimension() const {
  return seq.size();
}

template <typename ITV>
inline dimension_type
Box<ITV>::max_space_dimension() {
  // One dimension is reserved to have a value of type dimension_type
  // that does not represent a legal dimension.
  return Sequence().max_size() - 1;
}

template <typename ITV>
inline int32_t
Box<ITV>::hash_code() const {
  return hash_code_from_dimension(space_dimension());
}

template <typename ITV>
inline const ITV&
Box<ITV>::operator[](const dimension_type k) const {
  PPL_ASSERT(k < seq.size());
  return seq[k];
}

template <typename ITV>
inline const ITV&
Box<ITV>::get_interval(const Variable var) const {
  if (space_dimension() < var.space_dimension())
    throw_dimension_incompatible("get_interval(v)", "v", var);

  if (is_empty()) {
    static ITV empty_interval(EMPTY);
    return empty_interval;
  }

  return seq[var.id()];
}

template <typename ITV>
inline void
Box<ITV>::set_interval(const Variable var, const ITV& i) {
  const dimension_type space_dim = space_dimension();
  if (space_dim < var.space_dimension())
    throw_dimension_incompatible("set_interval(v, i)", "v", var);

  if (is_empty() && space_dim >= 2)
    // If the box is empty, and has dimension >= 2, setting only one
    // interval will not make it non-empty.
    return;

  seq[var.id()] = i;
  reset_empty_up_to_date();

  PPL_ASSERT(OK());
}

template <typename ITV>
inline bool
Box<ITV>::is_empty() const {
  return marked_empty() || check_empty();
}

template <typename ITV>
inline bool
Box<ITV>::bounds_from_above(const Linear_Expression& expr) const {
  return bounds(expr, true);
}

template <typename ITV>
inline bool
Box<ITV>::bounds_from_below(const Linear_Expression& expr) const {
  return bounds(expr, false);
}

template <typename ITV>
inline bool
Box<ITV>::maximize(const Linear_Expression& expr,
                   Coefficient& sup_n, Coefficient& sup_d,
                   bool& maximum) const {
  return max_min(expr, true, sup_n, sup_d, maximum);
}

template <typename ITV>
inline bool
Box<ITV>::maximize(const Linear_Expression& expr,
                   Coefficient& sup_n, Coefficient& sup_d, bool& maximum,
                   Generator& g) const {
  return max_min(expr, true, sup_n, sup_d, maximum, g);
}

template <typename ITV>
inline bool
Box<ITV>::minimize(const Linear_Expression& expr,
                   Coefficient& inf_n, Coefficient& inf_d,
                   bool& minimum) const {
  return max_min(expr, false, inf_n, inf_d, minimum);
}

template <typename ITV>
inline bool
Box<ITV>::minimize(const Linear_Expression& expr,
                   Coefficient& inf_n, Coefficient& inf_d, bool& minimum,
                   Generator& g) const {
  return max_min(expr, false, inf_n, inf_d, minimum, g);
}

template <typename ITV>
inline bool
Box<ITV>::strictly_contains(const Box& y) const {
  const Box& x = *this;
  return x.contains(y) && !y.contains(x);
}

template <typename ITV>
inline void
Box<ITV>::expand_space_dimension(const Variable var,
                                 const dimension_type m) {
  const dimension_type space_dim = space_dimension();
  // `var' should be one of the dimensions of the vector space.
  if (var.space_dimension() > space_dim)
    throw_dimension_incompatible("expand_space_dimension(v, m)", "v", var);

  // The space dimension of the resulting Box should not
  // overflow the maximum allowed space dimension.
  if (m > max_space_dimension() - space_dim)
    throw_invalid_argument("expand_dimension(v, m)",
                           "adding m new space dimensions exceeds "
                           "the maximum allowed space dimension");

  // To expand the space dimension corresponding to variable `var',
  // we append to the box `m' copies of the corresponding interval.
  seq.insert(seq.end(), m, seq[var.id()]);
  PPL_ASSERT(OK());
}

template <typename ITV>
inline bool
operator!=(const Box<ITV>& x, const Box<ITV>& y) {
  return !(x == y);
}

template <typename ITV>
inline bool
Box<ITV>::has_lower_bound(const Variable var,
                          Coefficient& n, Coefficient& d, bool& closed) const {
  // NOTE: assertion !is_empty() would be wrong;
  // see the calls in method Box<ITV>::constraints().
  PPL_ASSERT(!marked_empty());
  const dimension_type k = var.id();
  PPL_ASSERT(k < seq.size());
  const ITV& seq_k = seq[k];

  if (seq_k.lower_is_boundary_infinity())
    return false;

  closed = !seq_k.lower_is_open();

  PPL_DIRTY_TEMP(mpq_class, lr);
  assign_r(lr, seq_k.lower(), ROUND_NOT_NEEDED);
  n = lr.get_num();
  d = lr.get_den();

  return true;
}

template <typename ITV>
inline bool
Box<ITV>::has_upper_bound(const Variable var,
                          Coefficient& n, Coefficient& d, bool& closed) const {
  // NOTE: assertion !is_empty() would be wrong;
  // see the calls in method Box<ITV>::constraints().
  PPL_ASSERT(!marked_empty());
  const dimension_type k = var.id();
  PPL_ASSERT(k < seq.size());
  const ITV& seq_k = seq[k];

  if (seq_k.upper_is_boundary_infinity())
    return false;

  closed = !seq_k.upper_is_open();

  PPL_DIRTY_TEMP(mpq_class, ur);
  assign_r(ur, seq_k.upper(), ROUND_NOT_NEEDED);
  n = ur.get_num();
  d = ur.get_den();

  return true;
}

template <typename ITV>
inline void
Box<ITV>::add_constraint(const Constraint& c) {
  const dimension_type c_space_dim = c.space_dimension();
  // Dimension-compatibility check.
  if (c_space_dim > space_dimension())
    throw_dimension_incompatible("add_constraint(c)", c);

  add_constraint_no_check(c);
}

template <typename ITV>
inline void
Box<ITV>::add_constraints(const Constraint_System& cs) {
  // Dimension-compatibility check.
  if (cs.space_dimension() > space_dimension())
    throw_dimension_incompatible("add_constraints(cs)", cs);

  add_constraints_no_check(cs);
}

template <typename T>
inline void
Box<T>::add_recycled_constraints(Constraint_System& cs) {
  add_constraints(cs);
}

template <typename ITV>
inline void
Box<ITV>::add_congruence(const Congruence& cg) {
  const dimension_type cg_space_dim = cg.space_dimension();
  // Dimension-compatibility check.
  if (cg_space_dim > space_dimension())
    throw_dimension_incompatible("add_congruence(cg)", cg);

  add_congruence_no_check(cg);
}

template <typename ITV>
inline void
Box<ITV>::add_congruences(const Congruence_System& cgs) {
  if (cgs.space_dimension() > space_dimension())
    throw_dimension_incompatible("add_congruences(cgs)", cgs);
  add_congruences_no_check(cgs);
}

template <typename T>
inline void
Box<T>::add_recycled_congruences(Congruence_System& cgs) {
  add_congruences(cgs);
}

template <typename T>
inline bool
Box<T>::can_recycle_constraint_systems() {
  return false;
}

template <typename T>
inline bool
Box<T>::can_recycle_congruence_systems() {
  return false;
}

template <typename T>
inline void
Box<T>::widening_assign(const Box& y, unsigned* tp) {
  CC76_widening_assign(y, tp);
}

template <typename ITV>
inline Congruence_System
Box<ITV>::minimized_congruences() const {
  // Only equalities can be congruences and these are already minimized.
  return congruences();
}

template <typename ITV>
inline I_Result
Box<ITV>
::refine_interval_no_check(ITV& itv,
                           const Constraint::Type type,
                           Coefficient_traits::const_reference numer,
                           Coefficient_traits::const_reference denom) {
  PPL_ASSERT(denom != 0);
  // The interval constraint is of the form
  // `var + numer / denom rel 0',
  // where `rel' is either the relation `==', `>=', or `>'.
  // For the purpose of refining the interval, this is
  // (morally) turned into `var rel -numer/denom'.
  PPL_DIRTY_TEMP(mpq_class, q);
  assign_r(q.get_num(), numer, ROUND_NOT_NEEDED);
  assign_r(q.get_den(), denom, ROUND_NOT_NEEDED);
  q.canonicalize();
  // Turn `numer/denom' into `-numer/denom'.
  q = -q;

  Relation_Symbol rel_sym;
  switch (type) {
  case Constraint::EQUALITY:
    rel_sym = EQUAL;
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    rel_sym = (denom > 0) ? GREATER_OR_EQUAL : LESS_OR_EQUAL;
    break;
  case Constraint::STRICT_INEQUALITY:
    rel_sym = (denom > 0) ? GREATER_THAN : LESS_THAN;
    break;
  default:
    // Silence compiler warning.
    PPL_UNREACHABLE;
    return I_ANY;
  }
  I_Result res = itv.add_constraint(i_constraint(rel_sym, q));
  PPL_ASSERT(itv.OK());
  return res;
}

template <typename ITV>
inline void
Box<ITV>
::add_interval_constraint_no_check(const dimension_type var_id,
                                   const Constraint::Type type,
                                   Coefficient_traits::const_reference numer,
                                   Coefficient_traits::const_reference denom) {
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(var_id < space_dimension());
  PPL_ASSERT(denom != 0);
  refine_interval_no_check(seq[var_id], type, numer, denom);
  // FIXME: do check the value returned and set `empty' and
  // `empty_up_to_date' as appropriate.
  // This has to be done after reimplementation of intervals.
  reset_empty_up_to_date();
  PPL_ASSERT(OK());
}

template <typename ITV>
inline void
Box<ITV>::refine_with_constraint(const Constraint& c) {
  const dimension_type c_space_dim = c.space_dimension();
  // Dimension-compatibility check.
  if (c_space_dim > space_dimension())
    throw_dimension_incompatible("refine_with_constraint(c)", c);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  refine_no_check(c);
}

template <typename ITV>
inline void
Box<ITV>::refine_with_constraints(const Constraint_System& cs) {
  // Dimension-compatibility check.
  if (cs.space_dimension() > space_dimension())
    throw_dimension_incompatible("refine_with_constraints(cs)", cs);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  refine_no_check(cs);
}

template <typename ITV>
inline void
Box<ITV>::refine_with_congruence(const Congruence& cg) {
  const dimension_type cg_space_dim = cg.space_dimension();
  // Dimension-compatibility check.
  if (cg_space_dim > space_dimension())
    throw_dimension_incompatible("refine_with_congruence(cg)", cg);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  refine_no_check(cg);
}

template <typename ITV>
inline void
Box<ITV>::refine_with_congruences(const Congruence_System& cgs) {
  // Dimension-compatibility check.
  if (cgs.space_dimension() > space_dimension())
    throw_dimension_incompatible("refine_with_congruences(cgs)", cgs);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  refine_no_check(cgs);
}

template <typename ITV>
inline void
Box<ITV>::propagate_constraint(const Constraint& c) {
  const dimension_type c_space_dim = c.space_dimension();
  // Dimension-compatibility check.
  if (c_space_dim > space_dimension())
    throw_dimension_incompatible("propagate_constraint(c)", c);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  propagate_constraint_no_check(c);
}

template <typename ITV>
inline void
Box<ITV>::propagate_constraints(const Constraint_System& cs,
                                const dimension_type max_iterations) {
  // Dimension-compatibility check.
  if (cs.space_dimension() > space_dimension())
    throw_dimension_incompatible("propagate_constraints(cs)", cs);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  propagate_constraints_no_check(cs, max_iterations);
}

template <typename ITV>
inline void
Box<ITV>::unconstrain(const Variable var) {
  const dimension_type var_id = var.id();
  // Dimension-compatibility check.
  if (space_dimension() < var_id + 1)
    throw_dimension_incompatible("unconstrain(var)", var_id + 1);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  // Here the box might still be empty (but we haven't detected it yet):
  // check emptiness of the interval for `var' before cylindrification.
  ITV& seq_var = seq[var_id];
  if (seq_var.is_empty())
    set_empty();
  else
    seq_var.assign(UNIVERSE);

  PPL_ASSERT(OK());
}

/*! \relates Box */
template <typename Temp, typename To, typename ITV>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const Box<ITV>& x,
			    const Box<ITV>& y,
			    const Rounding_Dir dir,
			    Temp& tmp0,
			    Temp& tmp1,
			    Temp& tmp2) {
  return l_m_distance_assign<Rectilinear_Distance_Specialization<Temp> >
    (r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Box */
template <typename Temp, typename To, typename ITV>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const Box<ITV>& x,
			    const Box<ITV>& y,
			    const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return rectilinear_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Box */
template <typename To, typename ITV>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const Box<ITV>& x,
			    const Box<ITV>& y,
			    const Rounding_Dir dir) {
  // FIXME: the following qualification is only to work around a bug
  // in the Intel C/C++ compiler version 10.1.x.
  return Parma_Polyhedra_Library
    ::rectilinear_distance_assign<To, To, ITV>(r, x, y, dir);
}

/*! \relates Box */
template <typename Temp, typename To, typename ITV>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const Box<ITV>& x,
			  const Box<ITV>& y,
			  const Rounding_Dir dir,
			  Temp& tmp0,
			  Temp& tmp1,
			  Temp& tmp2) {
  return l_m_distance_assign<Euclidean_Distance_Specialization<Temp> >
    (r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Box */
template <typename Temp, typename To, typename ITV>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const Box<ITV>& x,
			  const Box<ITV>& y,
			  const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return euclidean_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Box */
template <typename To, typename ITV>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const Box<ITV>& x,
			  const Box<ITV>& y,
			  const Rounding_Dir dir) {
  // FIXME: the following qualification is only to work around a bug
  // in the Intel C/C++ compiler version 10.1.x.
  return Parma_Polyhedra_Library
    ::euclidean_distance_assign<To, To, ITV>(r, x, y, dir);
}

/*! \relates Box */
template <typename Temp, typename To, typename ITV>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const Box<ITV>& x,
			   const Box<ITV>& y,
			   const Rounding_Dir dir,
			   Temp& tmp0,
			   Temp& tmp1,
			   Temp& tmp2) {
  return l_m_distance_assign<L_Infinity_Distance_Specialization<Temp> >
    (r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Box */
template <typename Temp, typename To, typename ITV>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const Box<ITV>& x,
			   const Box<ITV>& y,
			   const Rounding_Dir dir) {
  typedef Checked_Number<Temp, Extended_Number_Policy> Checked_Temp;
  PPL_DIRTY_TEMP(Checked_Temp, tmp0);
  PPL_DIRTY_TEMP(Checked_Temp, tmp1);
  PPL_DIRTY_TEMP(Checked_Temp, tmp2);
  return l_infinity_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates Box */
template <typename To, typename ITV>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const Box<ITV>& x,
			   const Box<ITV>& y,
			   const Rounding_Dir dir) {
  // FIXME: the following qualification is only to work around a bug
  // in the Intel C/C++ compiler version 10.1.x.
  return Parma_Polyhedra_Library
    ::l_infinity_distance_assign<To, To, ITV>(r, x, y, dir);
}

/*! \relates Box */
template <typename ITV>
inline void
swap(Box<ITV>& x, Box<ITV>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Box_inlines_hh)
