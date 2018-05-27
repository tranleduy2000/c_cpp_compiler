/* Swapping_Vector class declaration.
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

#ifndef PPL_Swapping_Vector_defs_hh
#define PPL_Swapping_Vector_defs_hh 1

#include "Swapping_Vector_types.hh"
#include "globals_defs.hh"

#include <vector>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A wrapper for std::vector that calls a swap() method instead of copying
//! elements, when possible.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Swapping_Vector {

public:
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::size_type size_type;

  Swapping_Vector();
  explicit Swapping_Vector(dimension_type new_size);
  Swapping_Vector(dimension_type new_size, const T& x);

  void clear();
  void reserve(dimension_type new_capacity);
  void resize(dimension_type new_size);
  void resize(dimension_type new_size, const T& x);

  dimension_type size() const;
  dimension_type capacity() const;
  bool empty() const;

  void m_swap(Swapping_Vector& v);

  T& operator[](dimension_type i);
  const T& operator[](dimension_type i) const;

  T& back();
  const T& back() const;

  void push_back(const T& x);
  void pop_back();

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  iterator erase(iterator itr);
  iterator erase(iterator first, iterator last);

  // This is defined only if T has an external_memory_in_bytes() method.
  memory_size_type external_memory_in_bytes() const;

  dimension_type max_num_rows();

private:
  std::vector<T> impl;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Swapping_Vector */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
void swap(Swapping_Vector<T>& x, Swapping_Vector<T>& y);

} // namespace Parma_Polyhedra_Library

#include "Swapping_Vector_inlines.hh"

#endif // !defined(PPL_Swapping_Vector_defs_hh)
