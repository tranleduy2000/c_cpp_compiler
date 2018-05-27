/* Sparse_Row class implementation: inline functions.
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

#ifndef PPL_Sparse_Row_inlines_hh
#define PPL_Sparse_Row_inlines_hh 1

#include <algorithm>

namespace Parma_Polyhedra_Library {

inline
Sparse_Row::Sparse_Row(dimension_type n)
  : size_(n) {
  PPL_ASSERT(OK());
}

inline
Sparse_Row::Sparse_Row(dimension_type n, dimension_type)
  : size_(n) {
  PPL_ASSERT(OK());
}

inline
Sparse_Row::Sparse_Row(const Sparse_Row& y, dimension_type)
  : tree(y.tree), size_(y.size_) {
}

inline
Sparse_Row::Sparse_Row(const Sparse_Row& y, dimension_type sz, dimension_type)
  : tree(y.begin(),
         std::distance(y.begin(), y.lower_bound(std::min(y.size(), sz)))),
    size_(sz) {
  PPL_ASSERT(OK());
}

inline void
Sparse_Row::m_swap(Sparse_Row& x) {
  using std::swap;
  swap(tree, x.tree);
  swap(size_, x.size_);
  PPL_ASSERT(OK());
  PPL_ASSERT(x.OK());
}

inline dimension_type
Sparse_Row::size() const {
  return size_;
}

inline dimension_type
Sparse_Row::num_stored_elements() const {
  return tree.size();
}

inline void
Sparse_Row::resize(dimension_type n) {
  if (n < size_)
    reset_after(n);
  size_ = n;
  PPL_ASSERT(OK());
}

inline void
Sparse_Row::shrink(dimension_type n) {
  PPL_ASSERT(size() >= n);
  resize(n);
}

inline void
Sparse_Row::expand_within_capacity(dimension_type n) {
  PPL_ASSERT(size() <= n);
  resize(n);
}

inline void
Sparse_Row::delete_element_and_shift(dimension_type i) {
  PPL_ASSERT(i < size_);
  tree.erase_element_and_shift_left(i);
  --size_;
  PPL_ASSERT(OK());
}

inline void
Sparse_Row::add_zeroes_and_shift(dimension_type n, dimension_type i) {
  PPL_ASSERT(i <= size_);
  tree.increase_keys_from(i, n);
  size_ += n;
  PPL_ASSERT(OK());
}

inline Sparse_Row::iterator
Sparse_Row::begin() {
  return tree.begin();
}

inline const Sparse_Row::iterator&
Sparse_Row::end() {
  return tree.end();
}

inline Sparse_Row::const_iterator
Sparse_Row::begin() const {
  return tree.cbegin();
}

inline const Sparse_Row::const_iterator&
Sparse_Row::end() const {
  return tree.cend();
}

inline Sparse_Row::const_iterator
Sparse_Row::cbegin() const {
  return tree.cbegin();
}

inline const Sparse_Row::const_iterator&
Sparse_Row::cend() const {
  return tree.cend();
}

inline dimension_type
Sparse_Row::max_size() {
  return CO_Tree::max_size();
}

inline void
Sparse_Row::clear() {
  tree.clear();
}

inline Coefficient&
Sparse_Row::operator[](dimension_type i) {
  PPL_ASSERT(i < size_);
  iterator itr = insert(i);
  return *itr;
}

inline Coefficient_traits::const_reference
Sparse_Row::operator[](dimension_type i) const {
  return get(i);
}

inline Coefficient_traits::const_reference
Sparse_Row::get(dimension_type i) const {
  PPL_ASSERT(i < size_);
  if (tree.empty())
    return Coefficient_zero();
  const_iterator itr = find(i);
  if (itr != end())
    return *itr;
  else
    return Coefficient_zero();
}

inline Sparse_Row::iterator
Sparse_Row::find(dimension_type i) {
  PPL_ASSERT(i < size());

  iterator itr = tree.bisect(i);

  if (itr != end() && itr.index() == i)
    return itr;

  return end();
}

inline Sparse_Row::iterator
Sparse_Row::find(iterator hint, dimension_type i) {
  PPL_ASSERT(i < size());

  iterator itr = tree.bisect_near(hint, i);

  if (itr != end() && itr.index() == i)
    return itr;

  return end();
}

inline Sparse_Row::const_iterator
Sparse_Row::find(dimension_type i) const {
  PPL_ASSERT(i < size());

  const_iterator itr = tree.bisect(i);

  if (itr != end() && itr.index() == i)
    return itr;

  return end();
}

inline Sparse_Row::const_iterator
Sparse_Row::find(const_iterator hint, dimension_type i) const {
  PPL_ASSERT(i < size());

  const_iterator itr = tree.bisect_near(hint, i);

  if (itr != end() && itr.index() == i)
    return itr;

  return end();
}

inline Sparse_Row::iterator
Sparse_Row::lower_bound(dimension_type i) {
  PPL_ASSERT(i <= size());

  iterator itr = tree.bisect(i);

  if (itr == end())
    return end();

  if (itr.index() < i)
    ++itr;

  PPL_ASSERT(itr == end() || itr.index() >= i);

  return itr;
}

inline Sparse_Row::iterator
Sparse_Row::lower_bound(iterator hint, dimension_type i) {
  PPL_ASSERT(i <= size());

  iterator itr = tree.bisect_near(hint, i);

  if (itr == end())
    return end();

  if (itr.index() < i)
    ++itr;

  PPL_ASSERT(itr == end() || itr.index() >= i);

  return itr;
}

inline Sparse_Row::const_iterator
Sparse_Row::lower_bound(dimension_type i) const {
  PPL_ASSERT(i <= size());

  const_iterator itr = tree.bisect(i);

  if (itr == end())
    return end();

  if (itr.index() < i)
    ++itr;

  PPL_ASSERT(itr == end() || itr.index() >= i);

  return itr;
}

inline Sparse_Row::const_iterator
Sparse_Row::lower_bound(const_iterator hint, dimension_type i) const {
  PPL_ASSERT(i <= size());

  const_iterator itr = tree.bisect_near(hint, i);

  if (itr == end())
    return end();

  if (itr.index() < i)
    ++itr;

  PPL_ASSERT(itr == end() || itr.index() >= i);

  return itr;
}

inline Sparse_Row::iterator
Sparse_Row::insert(dimension_type i, Coefficient_traits::const_reference x) {
  PPL_ASSERT(i < size_);
  return tree.insert(i, x);
}

inline Sparse_Row::iterator
Sparse_Row::insert(iterator itr, dimension_type i,
                   Coefficient_traits::const_reference x) {
  PPL_ASSERT(i < size_);
  return tree.insert(itr, i, x);
}

inline Sparse_Row::iterator
Sparse_Row::insert(dimension_type i) {
  PPL_ASSERT(i < size_);
  return tree.insert(i);
}

inline Sparse_Row::iterator
Sparse_Row::insert(iterator itr, dimension_type i) {
  PPL_ASSERT(i < size_);
  return tree.insert(itr, i);
}

inline void
Sparse_Row::swap_coefficients(iterator i, iterator j) {
  PPL_ASSERT(i != end());
  PPL_ASSERT(j != end());
  using std::swap;
  swap(*i, *j);
  PPL_ASSERT(OK());
}

inline void
Sparse_Row::fast_swap(dimension_type i, iterator itr) {
  PPL_ASSERT(lower_bound(i) == itr);
  PPL_ASSERT(itr != end());
  tree.fast_shift(i, itr);
  PPL_ASSERT(OK());
}

inline Sparse_Row::iterator
Sparse_Row::reset(iterator i) {
  iterator res = tree.erase(i);
  PPL_ASSERT(OK());
  return res;
}

inline void
Sparse_Row::reset(dimension_type i) {
  PPL_ASSERT(i < size());

  tree.erase(i);
  PPL_ASSERT(OK());
}

inline memory_size_type
Sparse_Row::external_memory_in_bytes() const {
  return tree.external_memory_in_bytes();
}

inline memory_size_type
Sparse_Row::external_memory_in_bytes(dimension_type /* capacity */) const {
  return external_memory_in_bytes();
}

inline memory_size_type
Sparse_Row::total_memory_in_bytes() const {
  return external_memory_in_bytes() + sizeof(*this);
}

inline memory_size_type
Sparse_Row::total_memory_in_bytes(dimension_type /* capacity */) const {
  return total_memory_in_bytes();
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
inline void
swap(Sparse_Row& x, Sparse_Row& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Sparse_Row_inlines_hh)
