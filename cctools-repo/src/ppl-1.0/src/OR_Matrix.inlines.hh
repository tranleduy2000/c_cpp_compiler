/* OR_Matrix class implementation: inline functions.
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

#ifndef PPL_OR_Matrix_inlines_hh
#define PPL_OR_Matrix_inlines_hh 1

#include "globals.defs.hh"
#include "Checked_Number.defs.hh"
#include "C_Polyhedron.defs.hh"
#include "distances.defs.hh"
#include "assert.hh"
#include "checked.defs.hh"
#include <algorithm>

namespace Parma_Polyhedra_Library {

template <typename T>
inline dimension_type
OR_Matrix<T>::row_first_element_index(const dimension_type k) {
  return ((k + 1)*(k + 1))/2;
}

template <typename T>
inline dimension_type
OR_Matrix<T>::row_size(const dimension_type k) {
  return k + 2 - k % 2;
}

#if PPL_OR_MATRIX_EXTRA_DEBUG

template <typename T>
template <typename U>
inline dimension_type
OR_Matrix<T>::Pseudo_Row<U>::size() const {
  return size_;
}

#endif // PPL_OR_MATRIX_EXTRA_DEBUG

template <typename T>
template <typename U>
inline
OR_Matrix<T>::Pseudo_Row<U>::Pseudo_Row()
  : first(0)
#if PPL_OR_MATRIX_EXTRA_DEBUG
  , size_(0)
#endif
{
}

template <typename T>
template <typename U>
inline
OR_Matrix<T>::Pseudo_Row<U>::Pseudo_Row(U& y
#if PPL_OR_MATRIX_EXTRA_DEBUG
		, dimension_type s
#endif
		)
  : first(&y)
#if PPL_OR_MATRIX_EXTRA_DEBUG
  , size_(s)
#endif
{
}

template <typename T>
template <typename U>
template <typename V>
inline
OR_Matrix<T>::Pseudo_Row<U>::Pseudo_Row(const Pseudo_Row<V>& y)
  : first(y.first)
#if PPL_OR_MATRIX_EXTRA_DEBUG
    , size_(y.size_)
#endif
{
}

template <typename T>
template <typename U>
inline OR_Matrix<T>::Pseudo_Row<U>&
OR_Matrix<T>::Pseudo_Row<U>::operator=(const Pseudo_Row& y) {
  first = y.first;
#if PPL_OR_MATRIX_EXTRA_DEBUG
  size_ = y.size_;
#endif
  return *this;
}

template <typename T>
template <typename U>
inline
OR_Matrix<T>::Pseudo_Row<U>::~Pseudo_Row() {
}

template <typename T>
template <typename U>
inline U&
OR_Matrix<T>::Pseudo_Row<U>::operator[](const dimension_type k) const {
#if PPL_OR_MATRIX_EXTRA_DEBUG
  PPL_ASSERT(k < size_);
#endif
  return *(first + k);
}

template <typename T>
template <typename U>
inline
OR_Matrix<T>::any_row_iterator<U>
::any_row_iterator(const dimension_type n_rows)
  : value(),
    e(n_rows)
    // Field `i' is intentionally not initialized here.
{
#if PPL_OR_MATRIX_EXTRA_DEBUG
  // Turn `value' into a valid object.
  value.size_ = OR_Matrix::row_size(e);
#endif
}

template <typename T>
template <typename U>
inline
OR_Matrix<T>::any_row_iterator<U>::any_row_iterator(U& base)
  :  value(base
#if PPL_OR_MATRIX_EXTRA_DEBUG
	   , OR_Matrix<T>::row_size(0)
#endif
	   ),
     e(0),
     i(0) {
}

template <typename T>
template <typename U>
template <typename V>
inline
OR_Matrix<T>::any_row_iterator<U>
::any_row_iterator(const any_row_iterator<V>& y)
  : value(y.value),
    e(y.e),
    i(y.i) {
}

template <typename T>
template <typename U>
template <typename V>
inline typename OR_Matrix<T>::template any_row_iterator<U>&
OR_Matrix<T>::any_row_iterator<U>::operator=(const any_row_iterator<V>& y) {
  value = y.value;
  e = y.e;
  i = y.i;
  return *this;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>::reference
OR_Matrix<T>::any_row_iterator<U>::operator*() const {
  return value;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>::pointer
OR_Matrix<T>::any_row_iterator<U>::operator->() const {
  return &value;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>&
OR_Matrix<T>::any_row_iterator<U>::operator++() {
  ++e;
  dimension_type increment = e;
  if (e % 2 != 0)
    ++increment;
#if PPL_OR_MATRIX_EXTRA_DEBUG
  else {
    value.size_ += 2;
  }
#endif
  i += increment;
  value.first += increment;
  return *this;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>
OR_Matrix<T>::any_row_iterator<U>::operator++(int) {
  any_row_iterator old = *this;
  ++(*this);
  return old;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>&
OR_Matrix<T>::any_row_iterator<U>::operator--() {
  dimension_type decrement = e + 1;
  --e;
  if (e % 2 != 0) {
    ++decrement;
#if PPL_OR_MATRIX_EXTRA_DEBUG
    value.size_ -= 2;
#endif
  }
  i -= decrement;
  value.first -= decrement;
  return *this;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>
OR_Matrix<T>::any_row_iterator<U>::operator--(int) {
  any_row_iterator old = *this;
  --(*this);
  return old;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>&
OR_Matrix<T>::any_row_iterator<U>::operator+=(const difference_type m) {
  difference_type e_dt = static_cast<difference_type>(e);
  difference_type i_dt = static_cast<difference_type>(i);
  difference_type increment = m + (m * m) / 2 + m * e_dt;
  if (e_dt % 2 == 0 && m % 2 != 0)
    ++increment;
  e_dt += m;
  i_dt += increment;
  e = static_cast<dimension_type>(e_dt);
  i = static_cast<dimension_type>(i_dt);
  value.first += increment;
#if PPL_OR_MATRIX_EXTRA_DEBUG
  difference_type value_size_dt = static_cast<difference_type>(value.size_);
  value_size_dt += (m - m % 2);
  value.size_ = static_cast<dimension_type>(value_size_dt);
#endif
  return *this;
}

template <typename T>
template <typename U>
template <typename Unsigned>
inline typename
Enable_If<(static_cast<Unsigned>(-1) > 0),
            typename OR_Matrix<T>::template any_row_iterator<U>& >::type
OR_Matrix<T>::any_row_iterator<U>::operator+=(Unsigned m) {
  dimension_type n = m;
  dimension_type increment = n + (n*n)/2 + n*e;
  if (e % 2 == 0 && n % 2 != 0)
    ++increment;
  e += n;
  i += increment;
  value.first += increment;
#if PPL_OR_MATRIX_EXTRA_DEBUG
  value.size_ = value.size_ + n - n % 2;
#endif
  return *this;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>&
OR_Matrix<T>::any_row_iterator<U>::operator-=(difference_type m) {
  return *this += -m;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>::difference_type
OR_Matrix<T>::any_row_iterator<U>::operator-(const any_row_iterator& y) const {
  return e - y.e;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>
OR_Matrix<T>::any_row_iterator<U>::operator+(difference_type m) const {
  any_row_iterator r = *this;
  r += m;
  return r;
}

template <typename T>
template <typename U>
template <typename Unsigned>
inline typename
Enable_If<(static_cast<Unsigned>(-1) > 0),
            typename OR_Matrix<T>::template any_row_iterator<U> >::type
OR_Matrix<T>::any_row_iterator<U>::operator+(Unsigned m) const {
  any_row_iterator r = *this;
  r += m;
  return r;
}

template <typename T>
template <typename U>
inline typename OR_Matrix<T>::template any_row_iterator<U>
OR_Matrix<T>::any_row_iterator<U>::operator-(const difference_type m) const {
  any_row_iterator r = *this;
  r -= m;
  return r;
}

template <typename T>
template <typename U>
inline bool
OR_Matrix<T>::any_row_iterator<U>
::operator==(const any_row_iterator& y) const {
  return e == y.e;
}

template <typename T>
template <typename U>
inline bool
OR_Matrix<T>::any_row_iterator<U>
::operator!=(const any_row_iterator& y) const {
  return e != y.e;
}

template <typename T>
template <typename U>
inline bool
OR_Matrix<T>::any_row_iterator<U>::operator<(const any_row_iterator& y) const {
  return e < y.e;
}

template <typename T>
template <typename U>
inline bool
OR_Matrix<T>::any_row_iterator<U>
::operator<=(const any_row_iterator& y) const {
  return e <= y.e;
}

template <typename T>
template <typename U>
inline bool
OR_Matrix<T>::any_row_iterator<U>::operator>(const any_row_iterator& y) const {
  return e > y.e;
}

template <typename T>
template <typename U>
inline bool
OR_Matrix<T>::any_row_iterator<U>
::operator>=(const any_row_iterator& y) const {
  return e >= y.e;
}

template <typename T>
template <typename U>
inline dimension_type
OR_Matrix<T>::any_row_iterator<U>::row_size() const {
  return OR_Matrix::row_size(e);
}

template <typename T>
template <typename U>
inline dimension_type
OR_Matrix<T>::any_row_iterator<U>::index() const {
  return e;
}

template <typename T>
inline typename OR_Matrix<T>::row_iterator
OR_Matrix<T>::row_begin() {
  return num_rows() == 0 ? row_iterator(0) : row_iterator(vec[0]);
}

template <typename T>
inline typename OR_Matrix<T>::row_iterator
OR_Matrix<T>::row_end() {
  return row_iterator(num_rows());
}

template <typename T>
inline typename OR_Matrix<T>::const_row_iterator
OR_Matrix<T>::row_begin() const {
  return num_rows() == 0 ? const_row_iterator(0) : const_row_iterator(vec[0]);
}

template <typename T>
inline typename OR_Matrix<T>::const_row_iterator
OR_Matrix<T>::row_end() const {
  return const_row_iterator(num_rows());
}

template <typename T>
inline typename OR_Matrix<T>::element_iterator
OR_Matrix<T>::element_begin() {
  return vec.begin();
}

template <typename T>
inline typename OR_Matrix<T>::element_iterator
OR_Matrix<T>::element_end() {
  return vec.end();
}

template <typename T>
inline typename OR_Matrix<T>::const_element_iterator
OR_Matrix<T>::element_begin() const {
  return vec.begin();
}

template <typename T>
inline typename OR_Matrix<T>::const_element_iterator
OR_Matrix<T>::element_end() const {
  return vec.end();
}

template <typename T>
inline void
OR_Matrix<T>::m_swap(OR_Matrix& y) {
  using std::swap;
  swap(vec, y.vec);
  swap(space_dim, y.space_dim);
  swap(vec_capacity, y.vec_capacity);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns the integer square root of \p x.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
inline dimension_type
isqrt(dimension_type x) {
  dimension_type r = 0;
  const dimension_type FIRST_BIT_MASK = 0x40000000U;
  for (dimension_type t = FIRST_BIT_MASK; t != 0; t >>= 2) {
    dimension_type s = r + t;
    if (s <= x) {
      x -= s;
      r = s + t;
    }
    r >>= 1;
  }
  return r;
}

template <typename T>
inline dimension_type
OR_Matrix<T>::max_num_rows() {
  // Compute the maximum number of rows that are contained in a DB_Row
  // that allocates a pseudo-triangular matrix.
  dimension_type k = isqrt(2*DB_Row<T>::max_size() + 1);
  return (k - 1) - (k - 1) % 2;
}

template <typename T>
inline memory_size_type
OR_Matrix<T>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename T>
inline
OR_Matrix<T>::OR_Matrix(const dimension_type num_dimensions)
  : vec(2*num_dimensions*(num_dimensions + 1)),
    space_dim(num_dimensions),
    vec_capacity(vec.size()) {
}

template <typename T>
inline
OR_Matrix<T>::~OR_Matrix() {
}

template <typename T>
inline typename OR_Matrix<T>::row_reference_type
OR_Matrix<T>::operator[](dimension_type k) {
  return row_reference_type(vec[row_first_element_index(k)]
#if PPL_OR_MATRIX_EXTRA_DEBUG
			    , row_size(k)
#endif
			    );
}

template <typename T>
inline typename OR_Matrix<T>::const_row_reference_type
OR_Matrix<T>::operator[](dimension_type k) const {
  return const_row_reference_type(vec[row_first_element_index(k)]
#if PPL_OR_MATRIX_EXTRA_DEBUG
				  , row_size(k)
#endif
				  );
}

template <typename T>
inline dimension_type
OR_Matrix<T>::space_dimension() const {
  return space_dim;
}

template <typename T>
inline dimension_type
OR_Matrix<T>::num_rows() const {
  return 2*space_dimension();
}

template <typename T>
inline void
OR_Matrix<T>::clear() {
  OR_Matrix<T>(0).m_swap(*this);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline bool
operator==(const OR_Matrix<T>& x, const OR_Matrix<T>& y) {
  return x.space_dim == y.space_dim && x.vec == y.vec;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline bool
operator!=(const OR_Matrix<T>& x, const OR_Matrix<T>& y) {
  return !(x == y);
}

template <typename T>
inline
OR_Matrix<T>::OR_Matrix(const OR_Matrix& y)
  : vec(y.vec),
    space_dim(y.space_dim),
    vec_capacity(compute_capacity(y.vec.size(),
                                  DB_Row<T>::max_size())) {
}

template <typename T>
template <typename U>
inline
OR_Matrix<T>::OR_Matrix(const OR_Matrix<U>& y)
  : vec(),
    space_dim(y.space_dim),
    vec_capacity(compute_capacity(y.vec.size(),
                                  DB_Row<T>::max_size())) {
  vec.construct_upward_approximation(y.vec, vec_capacity);
  PPL_ASSERT(OK());
}

template <typename T>
inline OR_Matrix<T>&
OR_Matrix<T>::operator=(const OR_Matrix& y) {
  vec = y.vec;
  space_dim = y.space_dim;
  vec_capacity = compute_capacity(y.vec.size(), DB_Row<T>::max_size());
  return *this;
}

template <typename T>
inline void
OR_Matrix<T>::grow(const dimension_type new_dim) {
  PPL_ASSERT(new_dim >= space_dim);
  if (new_dim > space_dim) {
    const dimension_type new_size = 2*new_dim*(new_dim + 1);
    if (new_size <= vec_capacity) {
      // We can recycle the old vec.
      vec.expand_within_capacity(new_size);
      space_dim = new_dim;
    }
    else {
      // We cannot recycle the old vec.
      OR_Matrix<T> new_matrix(new_dim);
      element_iterator j = new_matrix.element_begin();
      for (element_iterator i = element_begin(),
	     mend = element_end(); i != mend; ++i, ++j)
	assign_or_swap(*j, *i);
      m_swap(new_matrix);
    }
  }
}

template <typename T>
inline void
OR_Matrix<T>::shrink(const dimension_type new_dim) {
  PPL_ASSERT(new_dim <= space_dim);
  const dimension_type new_size = 2*new_dim*(new_dim + 1);
  vec.shrink(new_size);
  space_dim = new_dim;
}

template <typename T>
inline void
OR_Matrix<T>::resize_no_copy(const dimension_type new_dim) {
  if (new_dim > space_dim) {
    const dimension_type new_size = 2*new_dim*(new_dim + 1);
    if (new_size <= vec_capacity) {
      // We can recycle the old vec.
      vec.expand_within_capacity(new_size);
      space_dim = new_dim;
    }
    else {
      // We cannot recycle the old vec.
      OR_Matrix<T> new_matrix(new_dim);
      m_swap(new_matrix);
    }
  }
  else if (new_dim < space_dim)
    shrink(new_dim);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Specialization, typename Temp, typename To, typename T>
inline bool
l_m_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
		    const OR_Matrix<T>& x,
		    const OR_Matrix<T>& y,
		    const Rounding_Dir dir,
		    Temp& tmp0,
		    Temp& tmp1,
		    Temp& tmp2) {
  if (x.num_rows() != y.num_rows())
    return false;
  assign_r(tmp0, 0, ROUND_NOT_NEEDED);
  for (typename OR_Matrix<T>::const_element_iterator
	 i = x.element_begin(), j = y.element_begin(),
	 mat_end = x.element_end(); i != mat_end; ++i, ++j) {
    const T& x_i = *i;
    const T& y_i = *j;
    if (is_plus_infinity(x_i)) {
      if (is_plus_infinity(y_i))
	continue;
      else {
      pinf:
	assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
	return true;
      }
    }
    else if (is_plus_infinity(y_i))
      goto pinf;

    const Temp* tmp1p;
    const Temp* tmp2p;
    if (x_i > y_i) {
      maybe_assign(tmp1p, tmp1, x_i, dir);
      maybe_assign(tmp2p, tmp2, y_i, inverse(dir));
    }
    else {
      maybe_assign(tmp1p, tmp1, y_i, dir);
      maybe_assign(tmp2p, tmp2, x_i, inverse(dir));
    }
    sub_assign_r(tmp1, *tmp1p, *tmp2p, dir);
    PPL_ASSERT(sgn(tmp1) >= 0);
    Specialization::combine(tmp0, tmp1, dir);
  }

  Specialization::finalize(tmp0, dir);
  assign_r(r, tmp0, dir);
  return true;
}


#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const OR_Matrix<T>& x,
			    const OR_Matrix<T>& y,
			    const Rounding_Dir dir,
			    Temp& tmp0,
			    Temp& tmp1,
			    Temp& tmp2) {
  return
    l_m_distance_assign<Rectilinear_Distance_Specialization<Temp> >(r, x, y,
								    dir,
								    tmp0,
								    tmp1,
								    tmp2);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const OR_Matrix<T>& x,
			  const OR_Matrix<T>& y,
			  const Rounding_Dir dir,
			  Temp& tmp0,
			  Temp& tmp1,
			  Temp& tmp2) {
  return
    l_m_distance_assign<Euclidean_Distance_Specialization<Temp> >(r, x, y,
								  dir,
								  tmp0,
								  tmp1,
								  tmp2);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const OR_Matrix<T>& x,
			   const OR_Matrix<T>& y,
			   const Rounding_Dir dir,
			   Temp& tmp0,
			   Temp& tmp1,
			   Temp& tmp2) {
  return
    l_m_distance_assign<L_Infinity_Distance_Specialization<Temp> >(r, x, y,
								   dir,
								   tmp0,
								   tmp1,
								   tmp2);
}

/*! \relates OR_Matrix */
template <typename T>
inline void
swap(OR_Matrix<T>& x, OR_Matrix<T>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_OR_Matrix_inlines_hh)
