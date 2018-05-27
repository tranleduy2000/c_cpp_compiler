/* DB_Row class implementation: inline functions.
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

#ifndef PPL_DB_Row_inlines_hh
#define PPL_DB_Row_inlines_hh 1

#include "checked.defs.hh"
#include "assert.hh"
#include <cstddef>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Parma_Polyhedra_Library {

template <typename T>
inline void*
DB_Row_Impl_Handler<T>::Impl::operator new(const size_t fixed_size,
					   const dimension_type capacity) {
#if PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  return ::operator new(fixed_size + capacity*sizeof(T));
#else
  PPL_ASSERT(capacity >= 1);
  return ::operator new(fixed_size + (capacity-1)*sizeof(T));
#endif
}

template <typename T>
inline void
DB_Row_Impl_Handler<T>::Impl::operator delete(void* p) {
  ::operator delete(p);
}

template <typename T>
inline void
DB_Row_Impl_Handler<T>::Impl::operator delete(void* p, dimension_type) {
  ::operator delete(p);
}

template <typename T>
inline memory_size_type
DB_Row_Impl_Handler<T>::Impl
::total_memory_in_bytes(dimension_type capacity) const {
  return
    sizeof(*this)
    + capacity*sizeof(T)
#if !PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
    - 1*sizeof(T)
#endif
    + external_memory_in_bytes();
}

template <typename T>
inline memory_size_type
DB_Row_Impl_Handler<T>::Impl::total_memory_in_bytes() const {
  // In general, this is a lower bound, as the capacity of *this
  // may be strictly greater than `size_'
  return total_memory_in_bytes(size_);
}

template <typename T>
inline dimension_type
DB_Row_Impl_Handler<T>::Impl::max_size() {
  return std::numeric_limits<size_t>::max() / sizeof(T);
}

template <typename T>
inline dimension_type
DB_Row_Impl_Handler<T>::Impl::size() const {
  return size_;
}

template <typename T>
inline void
DB_Row_Impl_Handler<T>::Impl::set_size(const dimension_type new_sz) {
  size_ = new_sz;
}

template <typename T>
inline void
DB_Row_Impl_Handler<T>::Impl::bump_size() {
  ++size_;
}

template <typename T>
inline
DB_Row_Impl_Handler<T>::Impl::Impl()
  : size_(0) {
}

template <typename T>
inline
DB_Row_Impl_Handler<T>::Impl::~Impl() {
  shrink(0);
}

template <typename T>
inline
DB_Row_Impl_Handler<T>::DB_Row_Impl_Handler()
  : impl(0) {
#if PPL_DB_ROW_EXTRA_DEBUG
  capacity_ = 0;
#endif
}

template <typename T>
inline
DB_Row_Impl_Handler<T>::~DB_Row_Impl_Handler() {
  delete impl;
}

template <typename T>
inline T&
DB_Row_Impl_Handler<T>::Impl::operator[](const dimension_type k) {
  PPL_ASSERT(k < size());
  return vec_[k];
}

template <typename T>
inline const T&
DB_Row_Impl_Handler<T>::Impl::operator[](const dimension_type k) const {
  PPL_ASSERT(k < size());
  return vec_[k];
}

template <typename T>
inline dimension_type
DB_Row<T>::max_size() {
  return DB_Row_Impl_Handler<T>::Impl::max_size();
}

template <typename T>
inline dimension_type
DB_Row<T>::size() const {
  return this->impl->size();
}

#if PPL_DB_ROW_EXTRA_DEBUG
template <typename T>
inline dimension_type
DB_Row<T>::capacity() const {
  return this->capacity_;
}
#endif // PPL_DB_ROW_EXTRA_DEBUG

template <typename T>
inline
DB_Row<T>::DB_Row()
  : DB_Row_Impl_Handler<T>() {
}

template <typename T>
inline void
DB_Row<T>::allocate(
#if PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
	       const
#endif
	       dimension_type capacity) {
  DB_Row<T>& x = *this;
  PPL_ASSERT(capacity <= max_size());
#if !PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  if (capacity == 0)
    ++capacity;
#endif
  PPL_ASSERT(x.impl == 0);
  x.impl = new (capacity) typename DB_Row_Impl_Handler<T>::Impl();
#if PPL_DB_ROW_EXTRA_DEBUG
  PPL_ASSERT(x.capacity_ == 0);
  x.capacity_ = capacity;
#endif
}

template <typename T>
inline void
DB_Row<T>::expand_within_capacity(const dimension_type new_size) {
  DB_Row<T>& x = *this;
  PPL_ASSERT(x.impl);
#if PPL_DB_ROW_EXTRA_DEBUG
  PPL_ASSERT(new_size <= x.capacity_);
#endif
  x.impl->expand_within_capacity(new_size);
}

template <typename T>
inline void
DB_Row<T>::copy_construct_coefficients(const DB_Row& y) {
  DB_Row<T>& x = *this;
  PPL_ASSERT(x.impl && y.impl);
#if PPL_DB_ROW_EXTRA_DEBUG
  PPL_ASSERT(y.size() <= x.capacity_);
#endif
  x.impl->copy_construct_coefficients(*(y.impl));
}

template <typename T>
template <typename U>
inline void
DB_Row<T>::construct_upward_approximation(const DB_Row<U>& y,
					  const dimension_type capacity) {
  DB_Row<T>& x = *this;
  PPL_ASSERT(y.size() <= capacity && capacity <= max_size());
  allocate(capacity);
  PPL_ASSERT(y.impl);
  x.impl->construct_upward_approximation(*(y.impl));
}

template <typename T>
inline void
DB_Row<T>::construct(const dimension_type sz,
		     const dimension_type capacity) {
  PPL_ASSERT(sz <= capacity && capacity <= max_size());
  allocate(capacity);
  expand_within_capacity(sz);
}

template <typename T>
inline void
DB_Row<T>::construct(const dimension_type sz) {
  construct(sz, sz);
}

template <typename T>
inline
DB_Row<T>::DB_Row(const dimension_type sz,
		  const dimension_type capacity)
  : DB_Row_Impl_Handler<T>() {
  construct(sz, capacity);
}

template <typename T>
inline
DB_Row<T>::DB_Row(const dimension_type sz) {
  construct(sz);
}

template <typename T>
inline
DB_Row<T>::DB_Row(const DB_Row& y)
  : DB_Row_Impl_Handler<T>() {
  if (y.impl != 0) {
    allocate(compute_capacity(y.size(), max_size()));
    copy_construct_coefficients(y);
  }
}

template <typename T>
inline
DB_Row<T>::DB_Row(const DB_Row& y,
		  const	dimension_type capacity)
  : DB_Row_Impl_Handler<T>() {
  PPL_ASSERT(y.impl);
  PPL_ASSERT(y.size() <= capacity && capacity <= max_size());
  allocate(capacity);
  copy_construct_coefficients(y);
}

template <typename T>
inline
DB_Row<T>::DB_Row(const DB_Row& y,
		  const dimension_type sz,
		  const	dimension_type capacity)
  : DB_Row_Impl_Handler<T>() {
  PPL_ASSERT(y.impl);
  PPL_ASSERT(y.size() <= sz && sz <= capacity && capacity <= max_size());
  allocate(capacity);
  copy_construct_coefficients(y);
  expand_within_capacity(sz);
}

template <typename T>
inline
DB_Row<T>::~DB_Row() {
}

template <typename T>
inline void
DB_Row<T>::shrink(const dimension_type new_size) {
  DB_Row<T>& x = *this;
  PPL_ASSERT(x.impl);
  x.impl->shrink(new_size);
}

template <typename T>
inline void
DB_Row<T>::m_swap(DB_Row& y) {
  using std::swap;
  DB_Row<T>& x = *this;
  swap(x.impl, y.impl);
#if PPL_DB_ROW_EXTRA_DEBUG
  swap(x.capacity_, y.capacity_);
#endif
}

template <typename T>
inline void
DB_Row<T>::assign(DB_Row& y) {
  DB_Row<T>& x = *this;
  x.impl = y.impl;
#if PPL_DB_ROW_EXTRA_DEBUG
  x.capacity_ = y.capacity_;
#endif
}

template <typename T>
inline DB_Row<T>&
DB_Row<T>::operator=(const DB_Row& y) {
  DB_Row tmp(y);
  m_swap(tmp);
  return *this;
}

template <typename T>
inline T&
DB_Row<T>::operator[](const dimension_type k) {
  DB_Row<T>& x = *this;
  return (*x.impl)[k];
}

template <typename T>
inline const T&
DB_Row<T>::operator[](const dimension_type k) const {
  const DB_Row<T>& x = *this;
  return (*x.impl)[k];
}

template <typename T>
inline typename DB_Row<T>::iterator
DB_Row<T>::begin() {
  DB_Row<T>& x = *this;
  return iterator(x.impl->vec_);
}

template <typename T>
inline typename DB_Row<T>::iterator
DB_Row<T>::end() {
  DB_Row<T>& x = *this;
  return iterator(x.impl->vec_ + x.impl->size_);
}

template <typename T>
inline typename DB_Row<T>::const_iterator
DB_Row<T>::begin() const {
  const DB_Row<T>& x = *this;
  return const_iterator(x.impl->vec_);
}

template <typename T>
inline typename DB_Row<T>::const_iterator
DB_Row<T>::end() const {
  const DB_Row<T>& x = *this;
  return const_iterator(x.impl->vec_ + x.impl->size_);
}

template <typename T>
inline memory_size_type
DB_Row<T>::external_memory_in_bytes(dimension_type capacity) const {
  const DB_Row<T>& x = *this;
  return x.impl->total_memory_in_bytes(capacity);
}

template <typename T>
inline memory_size_type
DB_Row<T>::total_memory_in_bytes(dimension_type capacity) const {
  return sizeof(*this) + external_memory_in_bytes(capacity);
}

template <typename T>
inline memory_size_type
DB_Row<T>::external_memory_in_bytes() const {
  const DB_Row<T>& x = *this;
#if PPL_DB_ROW_EXTRA_DEBUG
  return x.impl->total_memory_in_bytes(x.capacity_);
#else
  return x.impl->total_memory_in_bytes();
#endif
}

template <typename T>
inline memory_size_type
DB_Row<T>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

/*! \relates DB_Row */
template <typename T>
inline bool
operator!=(const DB_Row<T>& x, const DB_Row<T>& y) {
  return !(x == y);
}

/*! \relates DB_Row */
template <typename T>
inline void
swap(DB_Row<T>& x, DB_Row<T>& y) {
  x.m_swap(y);
}

/*! \relates DB_Row */
template <typename T>
inline void
iter_swap(typename std::vector<DB_Row<T> >::iterator x,
	  typename std::vector<DB_Row<T> >::iterator y) {
  swap(*x, *y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_DB_Row_inlines_hh)
