/* Powerset class implementation: inline functions.
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

#ifndef PPL_Powerset_inlines_hh
#define PPL_Powerset_inlines_hh 1

#include <algorithm>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

template <typename D>
inline typename Powerset<D>::iterator
Powerset<D>::begin() {
  return sequence.begin();
}

template <typename D>
inline typename Powerset<D>::iterator
Powerset<D>::end() {
  return sequence.end();
}

template <typename D>
inline typename Powerset<D>::const_iterator
Powerset<D>::begin() const {
  return sequence.begin();
}

template <typename D>
inline typename Powerset<D>::const_iterator
Powerset<D>::end() const {
  return sequence.end();
}

template <typename D>
inline typename Powerset<D>::reverse_iterator
Powerset<D>::rbegin() {
  return reverse_iterator(end());
}

template <typename D>
inline typename Powerset<D>::reverse_iterator
Powerset<D>::rend() {
  return reverse_iterator(begin());
}

template <typename D>
inline typename Powerset<D>::const_reverse_iterator
Powerset<D>::rbegin() const {
  return const_reverse_iterator(end());
}

template <typename D>
inline typename Powerset<D>::const_reverse_iterator
Powerset<D>::rend() const {
  return const_reverse_iterator(begin());
}

template <typename D>
inline typename Powerset<D>::size_type
Powerset<D>::size() const {
  return sequence.size();
}

template <typename D>
inline bool
Powerset<D>::empty() const {
  return sequence.empty();
}

template <typename D>
inline typename Powerset<D>::iterator
Powerset<D>::drop_disjunct(iterator position) {
  return sequence.erase(position.base);
}

template <typename D>
inline void
Powerset<D>::drop_disjuncts(iterator first, iterator last) {
  sequence.erase(first.base, last.base);
}

template <typename D>
inline void
Powerset<D>::clear() {
  sequence.clear();
}

template <typename D>
inline
Powerset<D>::Powerset(const Powerset& y)
  : sequence(y.sequence), reduced(y.reduced) {
}

template <typename D>
inline Powerset<D>&
Powerset<D>::operator=(const Powerset& y) {
  sequence = y.sequence;
  reduced = y.reduced;
  return *this;
}

template <typename D>
inline void
Powerset<D>::m_swap(Powerset& y) {
  using std::swap;
  swap(sequence, y.sequence);
  swap(reduced, y.reduced);
}

template <typename D>
inline
Powerset<D>::Powerset()
  : sequence(), reduced(true) {
}

template <typename D>
inline
Powerset<D>::Powerset(const D& d)
  : sequence(), reduced(false) {
  sequence.push_back(d);
  PPL_ASSERT_HEAVY(OK());
}

template <typename D>
inline
Powerset<D>::~Powerset() {
}

template <typename D>
inline void
Powerset<D>::add_non_bottom_disjunct_preserve_reduction(const D& d) {
  // !d.is_bottom() is asserted by the callee.
  add_non_bottom_disjunct_preserve_reduction(d, begin(), end());
}

template <typename D>
inline void
Powerset<D>::add_disjunct(const D& d) {
  sequence.push_back(d);
  reduced = false;
}

/*! \relates Powerset */
template <typename D>
inline
bool operator!=(const Powerset<D>& x, const Powerset<D>& y) {
  return !(x == y);
}

template <typename D>
inline bool
Powerset<D>::is_top() const {
  // Must perform omega-reduction for correctness.
  omega_reduce();
  const_iterator xi = begin();
  const_iterator x_end = end();
  return xi != x_end && xi->is_top() && ++xi == x_end;
}

template <typename D>
inline bool
Powerset<D>::is_bottom() const {
  // Must perform omega-reduction for correctness.
  omega_reduce();
  return empty();
}

template <typename D>
inline void
Powerset<D>::collapse() {
  if (!empty())
    collapse(sequence.begin());
}

template <typename D>
inline void
Powerset<D>::meet_assign(const Powerset& y) {
  pairwise_apply_assign(y, std::mem_fun_ref(&D::meet_assign));
}

template <typename D>
inline void
Powerset<D>::upper_bound_assign(const Powerset& y) {
  least_upper_bound_assign(y);
}

template <typename D>
inline bool
Powerset<D>::upper_bound_assign_if_exact(const Powerset& y) {
  least_upper_bound_assign(y);
  return true;
}

template <typename D>
inline memory_size_type
Powerset<D>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

/*! \relates Powerset */
template <typename D>
inline void
swap(Powerset<D>& x, Powerset<D>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Powerset_inlines_hh)
