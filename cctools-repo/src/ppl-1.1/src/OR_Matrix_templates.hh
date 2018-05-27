/* OR_Matrix class implementation: non-inline template functions.
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

#ifndef PPL_OR_Matrix_templates_hh
#define PPL_OR_Matrix_templates_hh 1

#include <iostream>

namespace Parma_Polyhedra_Library {

template <typename T>
memory_size_type
OR_Matrix<T>::external_memory_in_bytes() const{
  return vec.external_memory_in_bytes();
}

template <typename T>
bool
OR_Matrix<T>::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif
  // The right number of cells should be in use.
  const dimension_type dim = space_dimension();
  if (vec.size() != 2*dim*(dim + 1)) {
#ifndef NDEBUG
    cerr << "OR_Matrix has a wrong number of cells:\n"
         << "vec.size() is " << vec.size()
         << ", expected size is " << (2*dim*(dim+1)) << "!\n";
#endif
    return false;
  }

  // The underlying DB_Row should be OK.
  if (!vec.OK(vec.size(), vec_capacity))
    return false;

  // All checks passed.
  return true;
}

template <typename T>
void
OR_Matrix<T>::ascii_dump(std::ostream& s) const {
  const OR_Matrix<T>& x = *this;
  const char separator = ' ';
  dimension_type space = x.space_dimension();
  s << space << separator << "\n";
  for (const_row_iterator i = x.row_begin(),
         x_row_end = x.row_end(); i != x_row_end; ++i) {
    const_row_reference_type r = *i;
    dimension_type rs = i.row_size();
    for (dimension_type j = 0; j < rs; ++j) {
      using namespace IO_Operators;
      s << r[j] << separator;
    }
    s << "\n";
  }
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS(T, OR_Matrix<T>)

template <typename T>
bool
OR_Matrix<T>::ascii_load(std::istream& s) {
  dimension_type space;
  if (!(s >> space))
    return false;
  resize_no_copy(space);
  for (row_iterator i = row_begin(),
         this_row_end = row_end(); i != this_row_end; ++i) {
    row_reference_type r_i = *i;
    const dimension_type rs = i.row_size();
    for (dimension_type j = 0; j < rs; ++j) {
      Result r = input(r_i[j], s, ROUND_CHECK);
      if (result_relation(r) != VR_EQ || is_minus_infinity(r_i[j]))
        return false;
    }
  }
  PPL_ASSERT(OK());
  return true;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Parma_Polyhedra_Library::OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
std::ostream&
IO_Operators::operator<<(std::ostream& s, const OR_Matrix<T>& m) {
  for (typename OR_Matrix<T>::const_row_iterator m_iter = m.row_begin(),
         m_end = m.row_end(); m_iter != m_end; ++m_iter) {
    typename OR_Matrix<T>::const_row_reference_type r_m = *m_iter;
    const dimension_type mr_size = m_iter.row_size();
    for (dimension_type j = 0; j < mr_size; ++j)
      s << r_m[j] << " ";
    s << "\n";
  }
  return s;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_OR_Matrix_templates_hh)
