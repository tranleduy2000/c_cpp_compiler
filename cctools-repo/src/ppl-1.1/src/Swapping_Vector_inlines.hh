/* Swapping_Vector class implementation: inline functions.
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

#ifndef PPL_Swapping_Vector_inlines_hh
#define PPL_Swapping_Vector_inlines_hh 1

namespace Parma_Polyhedra_Library {

template <typename T>
inline
Swapping_Vector<T>::Swapping_Vector()
  : impl() {
}

template <typename T>
inline
Swapping_Vector<T>::Swapping_Vector(dimension_type i)
  : impl() {
  // NOTE: This is not the same as constructing impl as `impl(i)', because
  // this implementation calls compute_capacity().
  resize(i);
}

template <typename T>
inline
Swapping_Vector<T>::Swapping_Vector(dimension_type new_size, const T& x)
  : impl() {
  resize(new_size, x);
}

template <typename T>
inline void
Swapping_Vector<T>::clear() {
  impl.clear();
}

template <typename T>
inline void
Swapping_Vector<T>::reserve(dimension_type new_capacity) {
  if (impl.capacity() < new_capacity) {
    // Reallocation will take place.
    std::vector<T> new_impl;

    new_impl.reserve(compute_capacity(new_capacity, max_num_rows()));
    new_impl.resize(impl.size());

    using std::swap;

    // Steal the old elements.
    for (dimension_type i = impl.size(); i-- > 0; )
      swap(new_impl[i], impl[i]);

    // Put the new vector into place.
    swap(impl, new_impl);
  }
}

template <typename T>
inline void
Swapping_Vector<T>::resize(dimension_type new_size) {
  reserve(new_size);
  impl.resize(new_size);
}

template <typename T>
inline void
Swapping_Vector<T>::resize(dimension_type new_size, const T& x) {
  reserve(new_size);
  impl.resize(new_size, x);
}

template <typename T>
inline dimension_type
Swapping_Vector<T>::size() const {
  return impl.size();
}

template <typename T>
inline dimension_type
Swapping_Vector<T>::capacity() const {
  return impl.capacity();
}

template <typename T>
inline bool
Swapping_Vector<T>::empty() const {
  return impl.empty();
}

template <typename T>
inline void
Swapping_Vector<T>::m_swap(Swapping_Vector& v) {
  using std::swap;
  swap(impl, v.impl);
}

template <typename T>
inline T&
Swapping_Vector<T>::operator[](dimension_type i) {
  return impl[i];
}

template <typename T>
inline const T&
Swapping_Vector<T>::operator[](dimension_type i) const {
  return impl[i];
}

template <typename T>
inline T&
Swapping_Vector<T>::back() {
  return impl.back();
}

template <typename T>
inline const T&
Swapping_Vector<T>::back() const {
  return impl.back();
}

template <typename T>
inline void
Swapping_Vector<T>::push_back(const T& x) {
  reserve(size() + 1);
  impl.push_back(x);
}

template <typename T>
inline void
Swapping_Vector<T>::pop_back() {
  impl.pop_back();
}

template <typename T>
inline memory_size_type
Swapping_Vector<T>::external_memory_in_bytes() const {
  // Estimate the size of vector.
  memory_size_type n = impl.capacity() * sizeof(T);
  for (const_iterator i = begin(), i_end = end(); i != i_end; ++i)
    n += i->external_memory_in_bytes();
  return n;
}

template <typename T>
inline typename Swapping_Vector<T>::iterator
Swapping_Vector<T>::begin() {
  return impl.begin();
}

template <typename T>
inline typename Swapping_Vector<T>::iterator
Swapping_Vector<T>::end() {
  return impl.end();
}

template <typename T>
inline typename Swapping_Vector<T>::const_iterator
Swapping_Vector<T>::begin() const {
  return impl.begin();
}

template <typename T>
inline typename Swapping_Vector<T>::const_iterator
Swapping_Vector<T>::end() const {
  return impl.end();
}

template <typename T>
inline typename Swapping_Vector<T>::iterator
Swapping_Vector<T>::erase(iterator itr) {
  PPL_ASSERT(itr >= begin());
  PPL_ASSERT(itr < end());
  const dimension_type old_i = itr - begin();
  dimension_type i = old_i;
  ++i;
  while (i != size())
    swap(impl[i-1], impl[i]);
  impl.pop_back();
  return begin() + old_i;
}

template <typename T>
inline typename Swapping_Vector<T>::iterator
Swapping_Vector<T>::erase(iterator first, iterator last) {
  PPL_ASSERT(begin() <= first);
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= end());
  const iterator old_first = first;
  typedef typename std::iterator_traits<iterator>::difference_type diff_t;
  const diff_t k = last - first;
  const dimension_type n = static_cast<dimension_type>(end() - last);
  using std::swap;
  for (dimension_type i = 0; i < n; ++i, ++first)
    swap(*first, *(first + k));
  impl.erase(end() - k, end());
  return old_first;
}

template <typename T>
inline dimension_type
Swapping_Vector<T>::max_num_rows() {
  return impl.max_size();
}

template <typename T>
inline void
swap(Swapping_Vector<T>& vec1, Swapping_Vector<T>& vec2) {
  vec1.m_swap(vec2);
}

} // namespace Parma_Polyhedra_Library


#endif // !defined(PPL_Swapping_Vector_inlines_hh)
