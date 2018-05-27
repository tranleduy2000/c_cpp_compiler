/* Dense_Row class implementation: inline functions.
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

#ifndef PPL_Dense_Row_inlines_hh
#define PPL_Dense_Row_inlines_hh 1

#include "assert.hh"
#include <cstddef>
#include <limits>
#include <algorithm>

namespace Parma_Polyhedra_Library {

inline
Dense_Row::Impl::Impl()
  : size(0), capacity(0), coeff_allocator(), vec(0) {
}

inline
Dense_Row::Impl::~Impl() {
  while (size != 0) {
    --size;
    vec[size].~Coefficient();
  }
  coeff_allocator.deallocate(vec, capacity);
}

inline dimension_type
Dense_Row::max_size() {
  return std::numeric_limits<size_t>::max() / sizeof(Coefficient);
}

inline dimension_type
Dense_Row::size() const {
  return impl.size;
}

inline dimension_type
Dense_Row::capacity() const {
  return impl.capacity;
}

inline
Dense_Row::Dense_Row()
  : impl() {

  PPL_ASSERT(OK());
}

inline
Dense_Row::Dense_Row(const dimension_type sz,
                     const dimension_type capacity)
  : impl() {

  resize(sz, capacity);

  PPL_ASSERT(size() == sz);
  PPL_ASSERT(impl.capacity == capacity);
  PPL_ASSERT(OK());
}

inline
Dense_Row::Dense_Row(const dimension_type sz)
  : impl() {

  resize(sz);

  PPL_ASSERT(size() == sz);
  PPL_ASSERT(OK());
}

inline
Dense_Row::Dense_Row(const Dense_Row& y)
  : impl() {
  impl.coeff_allocator = y.impl.coeff_allocator;
  if (y.impl.vec != 0) {
    impl.capacity = y.capacity();
    impl.vec = impl.coeff_allocator.allocate(impl.capacity);
    while (impl.size != y.size()) {
      new (&impl.vec[impl.size]) Coefficient(y[impl.size]);
      ++impl.size;
    }
  }
  PPL_ASSERT(size() == y.size());
  PPL_ASSERT(capacity() == y.capacity());
  PPL_ASSERT(OK());
}

inline
Dense_Row::Dense_Row(const Dense_Row& y,
                     const dimension_type capacity)
  : impl() {
  PPL_ASSERT(y.size() <= capacity);
  PPL_ASSERT(capacity <= max_size());

  impl.capacity = capacity;
  impl.coeff_allocator = y.impl.coeff_allocator;
  impl.vec = impl.coeff_allocator.allocate(impl.capacity);

  if (y.impl.vec != 0) {
    while (impl.size != y.size()) {
      new (&impl.vec[impl.size]) Coefficient(y[impl.size]);
      ++impl.size;
    }
  }

  PPL_ASSERT(size() == y.size());
  PPL_ASSERT(impl.capacity == capacity);
  PPL_ASSERT(OK());
}

inline
Dense_Row::Dense_Row(const Dense_Row& y,
                     const dimension_type sz,
                     const dimension_type capacity)
  : impl() {
  PPL_ASSERT(sz <= capacity);
  PPL_ASSERT(capacity <= max_size());
  PPL_ASSERT(capacity != 0);

  impl.capacity = capacity;
  impl.coeff_allocator = y.impl.coeff_allocator;
  impl.vec = impl.coeff_allocator.allocate(impl.capacity);

  const dimension_type n = std::min(sz, y.size());
  while (impl.size != n) {
    new (&impl.vec[impl.size]) Coefficient(y[impl.size]);
    ++impl.size;
  }
  while (impl.size != sz) {
    new (&impl.vec[impl.size]) Coefficient();
    ++impl.size;
  }

  PPL_ASSERT(size() == sz);
  PPL_ASSERT(impl.capacity == capacity);
  PPL_ASSERT(OK());
}

inline
Dense_Row::~Dense_Row() {
  // The `impl' field will be destroyed automatically.
}

inline void
Dense_Row::destroy() {
  resize(0);
  impl.coeff_allocator.deallocate(impl.vec, impl.capacity);
}

inline void
Dense_Row::m_swap(Dense_Row& y) {
  using std::swap;
  swap(impl.size, y.impl.size);
  swap(impl.capacity, y.impl.capacity);
  swap(impl.coeff_allocator, y.impl.coeff_allocator);
  swap(impl.vec, y.impl.vec);
  PPL_ASSERT(OK());
  PPL_ASSERT(y.OK());
}

inline Dense_Row&
Dense_Row::operator=(const Dense_Row& y) {

  if (this != &y && size() == y.size()) {
    // Avoid reallocation.

    for (dimension_type i = size(); i-- > 0; )
      (*this)[i] = y[i];

    return *this;
  }

  Dense_Row x(y);
  swap(*this, x);

  return *this;
}

inline Coefficient&
Dense_Row::operator[](const dimension_type k) {
  PPL_ASSERT(impl.vec != 0);
  PPL_ASSERT(k < size());
  return impl.vec[k];
}

inline Coefficient_traits::const_reference
Dense_Row::operator[](const dimension_type k) const {
  PPL_ASSERT(impl.vec != 0);
  PPL_ASSERT(k < size());
  return impl.vec[k];
}

inline void
Dense_Row::swap_coefficients(dimension_type i, dimension_type j) {
  std::swap((*this)[i], (*this)[j]);
}

inline void
Dense_Row::swap_coefficients(iterator i, iterator j) {
  std::swap(*i, *j);
}

inline void
Dense_Row::reset(dimension_type i) {
  (*this)[i] = 0;
}

inline Dense_Row::iterator
Dense_Row::reset(iterator itr) {
  *itr = 0;
  ++itr;
  return itr;
}

inline Dense_Row::iterator
Dense_Row::begin() {
  return iterator(*this, 0);
}

inline Dense_Row::const_iterator
Dense_Row::begin() const {
  return const_iterator(*this, 0);
}

inline Dense_Row::iterator
Dense_Row::end() {
  return iterator(*this, size());
}

inline Dense_Row::const_iterator
Dense_Row::end() const {
  return const_iterator(*this, size());
}

inline Coefficient_traits::const_reference
Dense_Row::get(dimension_type i) const {
  return (*this)[i];
}

inline Dense_Row::iterator
Dense_Row::find(dimension_type i) {
  return iterator(*this, i);
}

inline Dense_Row::const_iterator
Dense_Row::find(dimension_type i) const {
  return const_iterator(*this, i);
}

inline Dense_Row::iterator
Dense_Row::find(iterator itr, dimension_type i) {
  (void)itr;
  return iterator(*this, i);
}

inline Dense_Row::const_iterator
Dense_Row::find(const_iterator itr, dimension_type i) const {
  (void)itr;
  return const_iterator(*this, i);
}

inline Dense_Row::iterator
Dense_Row::lower_bound(dimension_type i) {
  return find(i);
}

inline Dense_Row::const_iterator
Dense_Row::lower_bound(dimension_type i) const {
  return find(i);
}

inline Dense_Row::iterator
Dense_Row::lower_bound(iterator itr, dimension_type i) {
  return find(itr, i);
}

inline Dense_Row::const_iterator
Dense_Row::lower_bound(const_iterator itr, dimension_type i) const {
  return find(itr, i);
}

inline Dense_Row::iterator
Dense_Row::insert(dimension_type i,
                  Coefficient_traits::const_reference x) {
  (*this)[i] = x;
  return find(i);
}

inline Dense_Row::iterator
Dense_Row::insert(dimension_type i) {
  return find(i);
}

inline Dense_Row::iterator
Dense_Row::insert(iterator itr, dimension_type i,
                  Coefficient_traits::const_reference x) {
  (void)itr;
  (*this)[i] = x;
  return find(i);
}

inline Dense_Row::iterator
Dense_Row::insert(iterator itr, dimension_type i) {
  (void)itr;
  return find(i);
}

inline memory_size_type
Dense_Row::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline memory_size_type
Dense_Row::total_memory_in_bytes(dimension_type capacity) const {
  return sizeof(*this) + external_memory_in_bytes(capacity);
}

/*! \relates Dense_Row */
inline bool
operator!=(const Dense_Row& x, const Dense_Row& y) {
  return !(x == y);
}


inline
Dense_Row::iterator::iterator()
  : row(NULL), i(0) {
  PPL_ASSERT(OK());
}

inline
Dense_Row::iterator::iterator(Dense_Row& row1,dimension_type i1)
  : row(&row1), i(i1) {
  PPL_ASSERT(OK());
}

inline Coefficient&
Dense_Row::iterator::operator*() {
  PPL_ASSERT(i < row->size());
  return (*row)[i];
}

inline Coefficient_traits::const_reference
Dense_Row::iterator::operator*() const {
  PPL_ASSERT(i < row->size());
  return (*row)[i];
}

inline dimension_type
Dense_Row::iterator::index() const {
  return i;
}

inline Dense_Row::iterator&
Dense_Row::iterator::operator++() {
  PPL_ASSERT(i < row->size());
  ++i;
  PPL_ASSERT(OK());
  return *this;
}

inline Dense_Row::iterator
Dense_Row::iterator::operator++(int) {
  iterator tmp(*this);
  ++(*this);
  return tmp;
}

inline Dense_Row::iterator&
Dense_Row::iterator::operator--() {
  PPL_ASSERT(i > 0);
  --i;
  PPL_ASSERT(OK());
  return *this;
}

inline Dense_Row::iterator
Dense_Row::iterator::operator--(int) {
  iterator tmp(*this);
  --(*this);
  return tmp;
}

inline bool
Dense_Row::iterator::operator==(const iterator& x) const {
  return (row == x.row) && (i == x.i);
}

inline bool
Dense_Row::iterator::operator!=(const iterator& x) const {
  return !(*this == x);
}

inline
Dense_Row::iterator::operator const_iterator() const {
  return const_iterator(*row, i);
}

inline bool
Dense_Row::iterator::OK() const {
  if (row == NULL)
    return true;
  // i can be equal to row.size() for past-the-end iterators
  return (i <= row->size());
}


inline
Dense_Row::const_iterator::const_iterator()
  : row(NULL), i(0) {
  PPL_ASSERT(OK());
}

inline
Dense_Row::const_iterator::const_iterator(const Dense_Row& row1,
                                          dimension_type i1)
  : row(&row1), i(i1) {
  PPL_ASSERT(OK());
}

inline Coefficient_traits::const_reference
Dense_Row::const_iterator::operator*() const {
  PPL_ASSERT(i < row->size());
  return (*row)[i];
}

inline dimension_type
Dense_Row::const_iterator::index() const {
  return i;
}

inline Dense_Row::const_iterator&
Dense_Row::const_iterator::operator++() {
  PPL_ASSERT(i < row->size());
  ++i;
  PPL_ASSERT(OK());
  return *this;
}

inline Dense_Row::const_iterator
Dense_Row::const_iterator::operator++(int) {
  const_iterator tmp(*this);
  ++(*this);
  return tmp;
}

inline Dense_Row::const_iterator&
Dense_Row::const_iterator::operator--() {
  PPL_ASSERT(i > 0);
  --i;
  PPL_ASSERT(OK());
  return *this;
}

inline Dense_Row::const_iterator
Dense_Row::const_iterator::operator--(int) {
  const_iterator tmp(*this);
  --(*this);
  return tmp;
}

inline bool
Dense_Row::const_iterator::operator==(const const_iterator& x) const {
  return (row == x.row) && (i == x.i);
}

inline bool
Dense_Row::const_iterator::operator!=(const const_iterator& x) const {
  return !(*this == x);
}

inline bool
Dense_Row::const_iterator::OK() const {
  if (row == NULL)
    return true;
  // i can be equal to row.size() for past-the-end iterators
  return (i <= row->size());
}

inline void
linear_combine(Dense_Row& x, const Dense_Row& y,
               Coefficient_traits::const_reference coeff1,
               Coefficient_traits::const_reference coeff2) {
  x.linear_combine(y, coeff1, coeff2);
}

inline void
linear_combine(Dense_Row& x, const Dense_Row& y,
               Coefficient_traits::const_reference c1,
               Coefficient_traits::const_reference c2,
               dimension_type start, dimension_type end) {
  x.linear_combine(y, c1, c2, start, end);
}

/*! \relates Dense_Row */
inline void
swap(Dense_Row& x, Dense_Row& y) {
  x.m_swap(y);
}

/*! \relates Dense_Row */
inline void
iter_swap(std::vector<Dense_Row>::iterator x,
          std::vector<Dense_Row>::iterator y) {
  swap(*x, *y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Dense_Row_inlines_hh)
