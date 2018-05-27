/* DB_Row class implementation: non-inline template functions.
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

#ifndef PPL_DB_Row_templates_hh
#define PPL_DB_Row_templates_hh 1

#include "globals.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename T>
template <typename U>
void
DB_Row_Impl_Handler<T>::Impl::construct_upward_approximation(const U& y) {
  const dimension_type y_size = y.size();
#if PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  // Construct in direct order: will destroy in reverse order.
  for (dimension_type i = 0; i < y_size; ++i) {
    construct(vec_[i], y[i], ROUND_UP);
    bump_size();
  }
#else // PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  if (y_size > 0) {
    assign_r(vec_[0], y[0], ROUND_UP);
    bump_size();
    // Construct in direct order: will destroy in reverse order.
    for (dimension_type i = 1; i < y_size; ++i) {
      construct(vec_[i], y[i], ROUND_UP);
      bump_size();
    }
  }
#endif // PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
}

template <typename T>
void
DB_Row_Impl_Handler<T>::
Impl::expand_within_capacity(const dimension_type new_size) {
  PPL_ASSERT(size() <= new_size && new_size <= max_size());
#if !PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  if (size() == 0 && new_size > 0) {
    // vec_[0] is already constructed: we just need to assign +infinity.
    assign_r(vec_[0], PLUS_INFINITY, ROUND_NOT_NEEDED);
    bump_size();
  }
#endif
  // Construct in direct order: will destroy in reverse order.
  for (dimension_type i = size(); i < new_size; ++i) {
    new (&vec_[i]) T(PLUS_INFINITY, ROUND_NOT_NEEDED);
    bump_size();
  }
}

template <typename T>
void
DB_Row_Impl_Handler<T>::Impl::shrink(dimension_type new_size) {
  const dimension_type old_size = size();
  PPL_ASSERT(new_size <= old_size);
  // Since ~T() does not throw exceptions, nothing here does.
  set_size(new_size);
#if !PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  // Make sure we do not try to destroy vec_[0].
  if (new_size == 0)
    ++new_size;
#endif
  // We assume construction was done "forward".
  // We thus perform destruction "backward".
  for (dimension_type i = old_size; i-- > new_size; )
    vec_[i].~T();
}

template <typename T>
void
DB_Row_Impl_Handler<T>::Impl::copy_construct_coefficients(const Impl& y) {
  const dimension_type y_size = y.size();
#if PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  // Construct in direct order: will destroy in reverse order.
  for (dimension_type i = 0; i < y_size; ++i) {
    new (&vec_[i]) T(y.vec_[i]);
    bump_size();
  }
#else // PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  if (y_size > 0) {
    vec_[0] = y.vec_[0];
    bump_size();
    // Construct in direct order: will destroy in reverse order.
    for (dimension_type i = 1; i < y_size; ++i) {
      new (&vec_[i]) T(y.vec_[i]);
      bump_size();
    }
  }
#endif // PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
}

template <typename T>
memory_size_type
DB_Row_Impl_Handler<T>::Impl::external_memory_in_bytes() const {
  memory_size_type n = 0;
  for (dimension_type i = size(); i-- > 0; )
    n += Parma_Polyhedra_Library::external_memory_in_bytes(vec_[i]);
  return n;
}

template <typename T>
bool
DB_Row<T>::OK(const dimension_type row_size,
	      const dimension_type
#if PPL_DB_ROW_EXTRA_DEBUG
	      row_capacity
#endif
	      ) const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  const DB_Row<T>& x = *this;
  bool is_broken = false;

#if PPL_DB_ROW_EXTRA_DEBUG
# if !PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  if (x.capacity_ == 0) {
    cerr << "Illegal row capacity: is 0, should be at least 1"
	 << endl;
    is_broken = true;
  }
  else if (x.capacity_ == 1 && row_capacity == 0)
    // This is fine.
    ;
  else
# endif // !PPL_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS
  if (x.capacity_ != row_capacity) {
    cerr << "DB_Row capacity mismatch: is " << x.capacity_
	 << ", should be " << row_capacity << "."
	 << endl;
    is_broken = true;
  }
#endif // PPL_DB_ROW_EXTRA_DEBUG

  if (x.size() != row_size) {
#ifndef NDEBUG
    cerr << "DB_Row size mismatch: is " << x.size()
	 << ", should be " << row_size << "."
	 << endl;
#endif
    is_broken = true;
  }

#if PPL_DB_ROW_EXTRA_DEBUG
  if (x.capacity_ < x.size()) {
#ifndef NDEBUG
    cerr << "DB_Row is completely broken: capacity is " << x.capacity_
	 << ", size is " << x.size() << "."
	 << endl;
#endif
    is_broken = true;
  }
#endif // PPL_DB_ROW_EXTRA_DEBUG

  for (dimension_type i = x.size(); i-- > 0; ) {
    const T& element = x[i];
    // Not OK is bad.
    if (!element.OK()) {
      is_broken = true;
      break;
    }
    // In addition, nans should never occur.
    if (is_not_a_number(element)) {
#ifndef NDEBUG
      cerr << "Not-a-number found in DB_Row."
	   << endl;
#endif
      is_broken = true;
      break;
    }
  }

  return !is_broken;
}

/*! \relates DB_Row */
template <typename T>
bool
operator==(const DB_Row<T>& x, const DB_Row<T>& y) {
  if (x.size() != y.size())
    return false;
  for (dimension_type i = x.size(); i-- > 0; )
    if (x[i] != y[i])
      return false;
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_DB_Row_templates_hh)
