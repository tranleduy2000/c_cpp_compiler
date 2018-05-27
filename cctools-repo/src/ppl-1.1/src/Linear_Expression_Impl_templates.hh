/* Linear_Expression_Impl class implementation: non-inline template functions.
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

#ifndef PPL_Linear_Expression_Impl_templates_hh
#define PPL_Linear_Expression_Impl_templates_hh 1

#include "Dense_Row_defs.hh"
#include "Sparse_Row_defs.hh"

#include "Constraint_defs.hh"
#include "Generator_defs.hh"
#include "Grid_Generator_defs.hh"
#include "Congruence_defs.hh"
#include <stdexcept>
#include <iostream>

namespace Parma_Polyhedra_Library {

template <typename Row>
Linear_Expression_Impl<Row>
::Linear_Expression_Impl(const Linear_Expression_Impl& e) {
  construct(e);
}

template <typename Row>
template <typename Row2>
Linear_Expression_Impl<Row>
::Linear_Expression_Impl(const Linear_Expression_Impl<Row2>& e) {
  construct(e);
}

template <typename Row>
Linear_Expression_Impl<Row>
::Linear_Expression_Impl(const Linear_Expression_Interface& e) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&e)) {
    construct(*p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&e)) {
    construct(*p);
  }
  else {
    // Add implementations for other derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
Linear_Expression_Impl<Row>
::Linear_Expression_Impl(const Linear_Expression_Interface& e,
                         dimension_type space_dim) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&e)) {
    construct(*p, space_dim);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&e)) {
    construct(*p, space_dim);
  }
  else {
    // Add implementations for other derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Impl<Row2>& y, Variable i) {
  PPL_ASSERT(space_dimension() == y.space_dimension());
  PPL_ASSERT(i.space_dimension() <= space_dimension());
  linear_combine(y, i.space_dimension());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Impl<Row2>& y, dimension_type i) {
  const Linear_Expression_Impl& x = *this;
  PPL_ASSERT(i < x.space_dimension() + 1);
  PPL_ASSERT(x.space_dimension() == y.space_dimension());
  Coefficient_traits::const_reference x_i = x.row.get(i);
  Coefficient_traits::const_reference y_i = y.row.get(i);
  PPL_ASSERT(x_i != 0);
  PPL_ASSERT(y_i != 0);
  PPL_DIRTY_TEMP_COEFFICIENT(normalized_x_v);
  PPL_DIRTY_TEMP_COEFFICIENT(normalized_y_v);
  normalize2(x_i, y_i, normalized_x_v, normalized_y_v);
  neg_assign(normalized_x_v);
  linear_combine(y, normalized_y_v, normalized_x_v);
  // We cannot use x_i here because it may have been invalidated by
  // linear_combine().
  assert(x.row.get(i) == 0);
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Impl<Row2>& y,
                 Coefficient_traits::const_reference c1,
                 Coefficient_traits::const_reference c2) {
  PPL_ASSERT(c1 != 0);
  PPL_ASSERT(c2 != 0);
  if (space_dimension() < y.space_dimension())
    set_space_dimension(y.space_dimension());
  linear_combine(y, c1, c2, 0, y.space_dimension() + 1);
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::linear_combine_lax(const Linear_Expression_Impl<Row2>& y,
                     Coefficient_traits::const_reference c1,
                     Coefficient_traits::const_reference c2) {
  if (space_dimension() < y.space_dimension())
    set_space_dimension(y.space_dimension());
  linear_combine_lax(y, c1, c2, 0, y.space_dimension() + 1);
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
int
Linear_Expression_Impl<Row>
::compare(const Linear_Expression_Impl<Row2>& y) const {
  const Linear_Expression_Impl& x = *this;
  // Compare all the coefficients of the row starting from position 1.
  // NOTE: x and y may be of different size.
  typename Row::const_iterator i = x.row.lower_bound(1);
  typename Row::const_iterator i_end = x.row.end();
  typename Row2::const_iterator j = y.row.lower_bound(1);
  typename Row2::const_iterator j_end = y.row.end();
  while (i != i_end && j != j_end) {
    if (i.index() < j.index()) {
      const int s = sgn(*i);
      if (s != 0)
        return 2*s;
      ++i;
      continue;
    }
    if (i.index() > j.index()) {
      const int s = sgn(*j);
      if (s != 0)
        return -2*s;
      ++j;
      continue;
    }
    PPL_ASSERT(i.index() == j.index());
    const int s = cmp(*i, *j);
    if (s < 0)
      return -2;
    if (s > 0)
      return 2;
    PPL_ASSERT(s == 0);
    ++i;
    ++j;
  }
  for ( ; i != i_end; ++i) {
    const int s = sgn(*i);
    if (s != 0)
      return 2*s;
  }
  for ( ; j != j_end; ++j) {
    const int s = sgn(*j);
    if (s != 0)
      return -2*s;
  }

  // If all the coefficients in `x' equal all the coefficients in `y'
  // (starting from position 1) we compare coefficients in position 0,
  // i.e., inhomogeneous terms.
  const int comp = cmp(x.row.get(0), y.row.get(0));
  if (comp > 0)
    return 1;
  if (comp < 0)
    return -1;
  PPL_ASSERT(comp == 0);

  // `x' and `y' are equal.
  return 0;
}

template <typename Row>
Linear_Expression_Impl<Row>::Linear_Expression_Impl(const Variable v) {
  if (v.space_dimension() > max_space_dimension())
    throw std::length_error("Linear_Expression_Impl::"
                            "Linear_Expression_Impl(v):\n"
                            "v exceeds the maximum allowed "
                            "space dimension.");
  set_space_dimension(v.space_dimension());
  (*this) += v;
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
bool
Linear_Expression_Impl<Row>
::is_equal_to(const Linear_Expression_Impl<Row2>& x) const {
  return row == x.row;
}

template <typename Row>
void
Linear_Expression_Impl<Row>::get_row(Dense_Row& row) const {
  row = this->row;
}

template <typename Row>
void
Linear_Expression_Impl<Row>::get_row(Sparse_Row& row) const {
  row = this->row;
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  const dimension_type n = cycle.size();
  if (n < 2)
    return;

  if (n == 2) {
    row.swap_coefficients(cycle[0].space_dimension(),
                          cycle[1].space_dimension());
  }
  else {
    PPL_DIRTY_TEMP_COEFFICIENT(tmp);
    tmp = row.get(cycle.back().space_dimension());
    for (dimension_type i = n - 1; i-- > 0; )
      row.swap_coefficients(cycle[i + 1].space_dimension(),
                            cycle[i].space_dimension());
    if (tmp == 0)
      row.reset(cycle[0].space_dimension());
    else {
      using std::swap;
      swap(tmp, row[cycle[0].space_dimension()]);
    }
  }
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator+=(const Linear_Expression_Impl<Row2>& e) {
  linear_combine(e, Coefficient_one(), Coefficient_one());
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator+=(const Variable v) {
  const dimension_type v_space_dim = v.space_dimension();
  if (v_space_dim > Linear_Expression_Impl<Row>::max_space_dimension())
    throw std::length_error("Linear_Expression_Impl& "
                            "operator+=(e, v):\n"
                            "v exceeds the maximum allowed space dimension.");
  if (space_dimension() < v_space_dim)
    set_space_dimension(v_space_dim);
  typename Row::iterator itr = row.insert(v_space_dim);
  ++(*itr);
  if (*itr == 0)
    row.reset(itr);
  PPL_ASSERT(OK());
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
template <typename Row2>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator-=(const Linear_Expression_Impl<Row2>& e2) {
  linear_combine(e2, Coefficient_one(), -1);
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator-=(const Variable v) {
  const dimension_type v_space_dim = v.space_dimension();
  if (v_space_dim > Linear_Expression_Impl<Row>::max_space_dimension())
    throw std::length_error("Linear_Expression_Impl& "
                            "operator-=(e, v):\n"
                            "v exceeds the maximum allowed space dimension.");
  if (space_dimension() < v_space_dim)
    set_space_dimension(v_space_dim);
  typename Row::iterator itr = row.insert(v_space_dim);
  --(*itr);
  if (*itr == 0)
    row.reset(itr);
  PPL_ASSERT(OK());
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator*=(Coefficient_traits::const_reference n) {
  if (n == 0) {
    row.clear();
    PPL_ASSERT(OK());
    return *this;
  }
  for (typename Row::iterator i = row.begin(),
         i_end = row.end(); i != i_end; ++i)
    (*i) *= n;
  PPL_ASSERT(OK());
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator/=(Coefficient_traits::const_reference n) {
  typename Row::iterator i = row.begin();
  const typename Row::iterator& i_end = row.end();
  while (i != i_end) {
    (*i) /= n;
    if (*i == 0)
      i = row.reset(i);
    else
      ++i;
  }
  PPL_ASSERT(OK());
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
void
Linear_Expression_Impl<Row>::negate() {
  for (typename Row::iterator i = row.begin(),
         i_end = row.end(); i != i_end; ++i)
    neg_assign(*i);
  PPL_ASSERT(OK());
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::add_mul_assign(Coefficient_traits::const_reference n,
                                            const Variable v) {
  const dimension_type v_space_dim = v.space_dimension();
  if (v_space_dim > Linear_Expression_Impl<Row>::max_space_dimension())
    throw std::length_error("Linear_Expression_Impl& "
                            "add_mul_assign(e, n, v):\n"
                            "v exceeds the maximum allowed space dimension.");
  if (space_dimension() < v_space_dim)
    set_space_dimension(v_space_dim);
  if (n == 0)
    return *this;
  typename Row::iterator itr = row.insert(v_space_dim);
  (*itr) += n;
  if (*itr == 0)
    row.reset(itr);
  PPL_ASSERT(OK());
  return *this;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>
::sub_mul_assign(Coefficient_traits::const_reference n,
                 const Variable v) {
  const dimension_type v_space_dim = v.space_dimension();
  if (v_space_dim > Linear_Expression_Impl<Row>::max_space_dimension())
    throw std::length_error("Linear_Expression_Impl& "
                            "sub_mul_assign(e, n, v):\n"
                            "v exceeds the maximum allowed space dimension.");
  if (space_dimension() < v_space_dim)
    set_space_dimension(v_space_dim);
  if (n == 0)
    return *this;
  typename Row::iterator itr = row.insert(v_space_dim);
  (*itr) -= n;
  if (*itr == 0)
    row.reset(itr);
  PPL_ASSERT(OK());
  return *this;
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::add_mul_assign(Coefficient_traits::const_reference factor,
                 const Linear_Expression_Impl<Row2>& y) {
  if (factor != 0)
    linear_combine(y, Coefficient_one(), factor);
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::sub_mul_assign(Coefficient_traits::const_reference factor,
                 const Linear_Expression_Impl<Row2>& y) {
  if (factor != 0)
    linear_combine(y, Coefficient_one(), -factor);
}

template <typename Row>
void
Linear_Expression_Impl<Row>::print(std::ostream& s) const {
  PPL_DIRTY_TEMP_COEFFICIENT(ev);
  bool first = true;
  for (typename Row::const_iterator i = row.lower_bound(1), i_end = row.end();
       i != i_end; ++i) {
    ev = *i;
    if (ev == 0)
      continue;
    if (!first) {
      if (ev > 0)
        s << " + ";
      else {
        s << " - ";
        neg_assign(ev);
      }
    }
    else
      first = false;
    if (ev == -1)
      s << "-";
    else if (ev != 1)
      s << ev << "*";
    IO_Operators::operator<<(s, Variable(i.index() - 1));
  }
  // Inhomogeneous term.
  PPL_DIRTY_TEMP_COEFFICIENT(it);
  it = row[0];
  if (it != 0) {
    if (!first) {
      if (it > 0)
        s << " + ";
      else {
        s << " - ";
        neg_assign(it);
      }
    }
    else
      first = false;
    s << it;
  }

  if (first)
    // The null linear expression.
    s << Coefficient_zero();
}

template <typename Row>
Coefficient_traits::const_reference
Linear_Expression_Impl<Row>::get(dimension_type i) const {
  return row.get(i);
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::set(dimension_type i, Coefficient_traits::const_reference n) {
  if (n == 0)
    row.reset(i);
  else
    row.insert(i, n);
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::exact_div_assign(Coefficient_traits::const_reference c,
                   dimension_type start, dimension_type end) {
  // NOTE: Since all coefficients in [start,end) are multiple of c,
  // each of the resulting coefficients will be nonzero iff the initial
  // coefficient was.
  for (typename Row::iterator i = row.lower_bound(start),
         i_end = row.lower_bound(end); i != i_end; ++i)
    Parma_Polyhedra_Library::exact_div_assign(*i, *i, c);
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::mul_assign(Coefficient_traits::const_reference c,
                   dimension_type start, dimension_type end) {
  if (c == 0) {
    typename Row::iterator i = row.lower_bound(start);
    const typename Row::iterator& i_end = row.end();
    while (i != i_end && i.index() < end)
      i = row.reset(i);
  }
  else {
    for (typename Row::iterator
      i = row.lower_bound(start), i_end = row.lower_bound(end); i != i_end; ++i)
      (*i) *= c;
  }
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Impl<Row2>& y,
                 Coefficient_traits::const_reference c1,
                 Coefficient_traits::const_reference c2,
                 dimension_type start, dimension_type end) {
  Parma_Polyhedra_Library::linear_combine(row, y.row, c1, c2, start, end);
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::linear_combine_lax(const Linear_Expression_Impl<Row2>& y,
                     Coefficient_traits::const_reference c1,
                     Coefficient_traits::const_reference c2,
                     dimension_type start, dimension_type end) {
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= row.size());
  PPL_ASSERT(end <= y.row.size());
  if (c1 == 0) {
    if (c2 == 0) {
      PPL_ASSERT(c1 == 0);
      PPL_ASSERT(c2 == 0);
      typename Row::iterator i = row.lower_bound(start);
      const typename Row::iterator& i_end = row.end();
      while (i != i_end && i.index() < end)
        i = row.reset(i);
    }
    else {
      PPL_ASSERT(c1 == 0);
      PPL_ASSERT(c2 != 0);

      typename Row::iterator i = row.lower_bound(start);
      const typename Row::iterator& i_end = row.end();
      typename Row2::const_iterator j = y.row.lower_bound(start);
      typename Row2::const_iterator j_last = y.row.lower_bound(end);

      while (i != i_end && i.index() < end && j != j_last) {
        if (i.index() < j.index()) {
          i = row.reset(i);
          continue;
        }
        if (i.index() > j.index()) {
          i = row.insert(i, j.index(), *j);
          (*i) *= c2;
          ++i;
          ++j;
          continue;
        }
        PPL_ASSERT(i.index() == j.index());
        (*i) = (*j);
        (*i) *= c2;
        ++i;
        ++j;
      }
      while (i != i_end && i.index() < end)
        i = row.reset(i);
      while (j != j_last) {
        i = row.insert(i, j.index(), *j);
        (*i) *= c2;
        // No need to increment i here.
        ++j;
      }
    }
  }
  else {
    if (c2 == 0) {
      PPL_ASSERT(c1 != 0);
      PPL_ASSERT(c2 == 0);
      for (typename Row::iterator i = row.lower_bound(start),
             i_end = row.lower_bound(end); i != i_end; ++i)
        (*i) *= c1;
    }
    else {
      PPL_ASSERT(c1 != 0);
      PPL_ASSERT(c2 != 0);
      Parma_Polyhedra_Library::linear_combine(row, y.row, c1, c2, start, end);
    }
  }
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_Expression_Impl<Row>::sign_normalize() {
  typename Row::iterator i = row.lower_bound(1);
  typename Row::iterator i_end = row.end();

  for ( ; i != i_end; ++i)
    if (*i != 0)
      break;

  if (i != i_end && *i < 0) {
    for ( ; i != i_end; ++i)
      neg_assign(*i);
    // Negate the first coefficient, too.
    typename Row::iterator first = row.begin();
    if (first != row.end() && first.index() == 0)
      neg_assign(*first);
  }
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_Expression_Impl<Row>::negate(dimension_type first, dimension_type last) {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= row.size());
  typename Row::iterator i = row.lower_bound(first);
  typename Row::iterator i_end = row.lower_bound(last);
  for ( ; i != i_end; ++i)
    neg_assign(*i);
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>::construct(const Linear_Expression_Impl<Row2>& e) {
  row = e.row;
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>::construct(const Linear_Expression_Impl<Row2>& e,
                                       dimension_type space_dim) {
  Row x(e.row, space_dim + 1, space_dim + 1);
  swap(row, x);
  PPL_ASSERT(OK());
}

template <typename Row>
template <typename Row2>
void
Linear_Expression_Impl<Row>
::scalar_product_assign(Coefficient& result,
                        const Linear_Expression_Impl<Row2>& y,
                        dimension_type start, dimension_type end) const {
  const Linear_Expression_Impl<Row>& x = *this;
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= x.row.size());
  PPL_ASSERT(end <= y.row.size());
  result = 0;
  typename Row ::const_iterator x_i = x.row.lower_bound(start);
  typename Row ::const_iterator x_end = x.row.lower_bound(end);
  typename Row2::const_iterator y_i = y.row.lower_bound(start);
  typename Row2::const_iterator y_end = y.row.lower_bound(end);
  while (x_i != x_end && y_i != y_end) {
    if (x_i.index() == y_i.index()) {
      Parma_Polyhedra_Library::add_mul_assign(result, *x_i, *y_i);
      ++x_i;
      ++y_i;
    }
    else {
      if (x_i.index() < y_i.index()) {
        PPL_ASSERT(y.row.get(x_i.index()) == 0);
        // (*x_i) * 0 == 0, nothing to do.
        ++x_i;
      }
      else {
        PPL_ASSERT(x.row.get(y_i.index()) == 0);
        // 0 * (*y_i) == 0, nothing to do.
        ++y_i;
      }
    }
  }
  // In the remaining positions (if any) at most one row is nonzero, so
  // there's nothing left to do.
}

template <typename Row>
template <typename Row2>
int
Linear_Expression_Impl<Row>
::scalar_product_sign(const Linear_Expression_Impl<Row2>& y,
                      dimension_type start, dimension_type end) const {
  PPL_DIRTY_TEMP_COEFFICIENT(result);
  scalar_product_assign(result, y, start, end);
  return sgn(result);
}

template <typename Row>
template <typename Row2>
bool
Linear_Expression_Impl<Row>
::is_equal_to(const Linear_Expression_Impl<Row2>& y,
              dimension_type start, dimension_type end) const {
  const Linear_Expression_Impl<Row>& x = *this;
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= x.row.size());
  PPL_ASSERT(end <= y.row.size());

  typename Row::const_iterator i = x.row.lower_bound(start);
  typename Row::const_iterator i_end = x.row.lower_bound(end);
  typename Row2::const_iterator j = y.row.lower_bound(start);
  typename Row2::const_iterator j_end = y.row.lower_bound(end);
  while (i != i_end && j != j_end) {
    if (i.index() == j.index()) {
      if (*i != *j)
        return false;
      ++i;
      ++j;
    }
    else {
      if (i.index() < j.index()) {
        if (*i != 0)
          return false;
        ++i;
      }
      else {
        PPL_ASSERT(i.index() > j.index());
        if (*j != 0)
          return false;
        ++j;
      }
    }
  }
  for ( ; i != i_end; ++i)
    if (*i != 0)
      return false;
  for ( ; j != j_end; ++j)
    if (*j != 0)
      return false;
  return true;
}

template <typename Row>
template <typename Row2>
bool
Linear_Expression_Impl<Row>
::is_equal_to(const Linear_Expression_Impl<Row2>& y,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  const Linear_Expression_Impl<Row>& x = *this;
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= x.row.size());
  PPL_ASSERT(end <= y.row.size());

  // Deal with trivial cases.
  if (c1 == 0) {
    if (c2 == 0)
      return true;
    else
      return y.all_zeroes(start, end);
  }
  if (c2 == 0)
    return x.all_zeroes(start, end);

  PPL_ASSERT(c1 != 0);
  PPL_ASSERT(c2 != 0);
  typename Row::const_iterator i = x.row.lower_bound(start);
  typename Row::const_iterator i_end = x.row.lower_bound(end);
  typename Row2::const_iterator j = y.row.lower_bound(start);
  typename Row2::const_iterator j_end = y.row.lower_bound(end);
  while (i != i_end && j != j_end) {
    if (i.index() == j.index()) {
      if ((*i) * c1 != (*j) * c2)
        return false;
      ++i;
      ++j;
    }
    else {
      if (i.index() < j.index()) {
        if (*i != 0)
          return false;
        ++i;
      }
      else {
        PPL_ASSERT(i.index() > j.index());
        if (*j != 0)
          return false;
        ++j;
      }
    }
  }
  for ( ; i != i_end; ++i)
    if (*i != 0)
      return false;
  for ( ; j != j_end; ++j)
    if (*j != 0)
      return false;
  return true;
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Interface& y, Variable v) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    linear_combine(*p, v);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    linear_combine(*p, v);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Interface& y,
                 Coefficient_traits::const_reference c1,
                 Coefficient_traits::const_reference c2) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    linear_combine(*p, c1, c2);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    linear_combine(*p, c1, c2);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::linear_combine_lax(const Linear_Expression_Interface& y,
                     Coefficient_traits::const_reference c1,
                     Coefficient_traits::const_reference c2) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    linear_combine_lax(*p, c1, c2);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    linear_combine_lax(*p, c1, c2);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
bool
Linear_Expression_Impl<Row>
::is_equal_to(const Linear_Expression_Interface& y) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return is_equal_to(*p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return is_equal_to(*p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return false;
  }
}

template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>
::operator+=(const Linear_Expression_Interface& y) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return operator+=(*p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return operator+=(*p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return *this;
  }
}

template <typename Row>
Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>
::operator-=(const Linear_Expression_Interface& y) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return operator-=(*p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return operator-=(*p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return *this;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::add_mul_assign(Coefficient_traits::const_reference factor,
                 const Linear_Expression_Interface& y) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    add_mul_assign(factor, *p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    add_mul_assign(factor, *p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::sub_mul_assign(Coefficient_traits::const_reference factor,
                 const Linear_Expression_Interface& y) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    sub_mul_assign(factor, *p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    sub_mul_assign(factor, *p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Interface& y, dimension_type i) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    linear_combine(*p, i);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    linear_combine(*p, i);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::linear_combine(const Linear_Expression_Interface& y,
                 Coefficient_traits::const_reference c1,
                 Coefficient_traits::const_reference c2,
                 dimension_type start, dimension_type end) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    linear_combine(*p, c1, c2, start, end);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    linear_combine(*p, c1, c2, start, end);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::linear_combine_lax(const Linear_Expression_Interface& y,
                     Coefficient_traits::const_reference c1,
                     Coefficient_traits::const_reference c2,
                     dimension_type start, dimension_type end) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    linear_combine_lax(*p, c1, c2, start, end);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    linear_combine_lax(*p, c1, c2, start, end);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
int
Linear_Expression_Impl<Row>
::compare(const Linear_Expression_Interface& y) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return compare(*p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return compare(*p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return 0;
  }
}


template <typename Row>
void
Linear_Expression_Impl<Row>::construct(const Linear_Expression_Interface& y) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return construct(*p);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return construct(*p);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>::construct(const Linear_Expression_Interface& y,
                                       dimension_type space_dim) {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return construct(*p, space_dim);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return construct(*p, space_dim);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
void
Linear_Expression_Impl<Row>
::scalar_product_assign(Coefficient& result,
                        const Linear_Expression_Interface& y,
                        dimension_type start, dimension_type end) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    scalar_product_assign(result, *p, start, end);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    scalar_product_assign(result, *p, start, end);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
  }
}

template <typename Row>
int
Linear_Expression_Impl<Row>
::scalar_product_sign(const Linear_Expression_Interface& y,
                      dimension_type start, dimension_type end) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return scalar_product_sign(*p, start, end);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return scalar_product_sign(*p, start, end);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return 0;
  }
}

template <typename Row>
bool
Linear_Expression_Impl<Row>
::is_equal_to(const Linear_Expression_Interface& y,
              dimension_type start, dimension_type end) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return is_equal_to(*p, start, end);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return is_equal_to(*p, start, end);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return false;
  }
}

template <typename Row>
bool
Linear_Expression_Impl<Row>
::is_equal_to(const Linear_Expression_Interface& y,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return is_equal_to(*p, c1, c2, start, end);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return is_equal_to(*p, c1, c2, start, end);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return false;
  }
}

template <typename Row>
bool
Linear_Expression_Impl<Row>
::have_a_common_variable(const Linear_Expression_Interface& y,
                         Variable first, Variable last) const {
  typedef const Linear_Expression_Impl<Dense_Row>* Dense_Ptr;
  typedef const Linear_Expression_Impl<Sparse_Row>* Sparse_Ptr;
  if (const Dense_Ptr p = dynamic_cast<Dense_Ptr>(&y)) {
    return have_a_common_variable(*p, first, last);
  }
  else if (const Sparse_Ptr p = dynamic_cast<Sparse_Ptr>(&y)) {
    return have_a_common_variable(*p, first, last);
  }
  else {
    // Add implementations for new derived classes here.
    PPL_UNREACHABLE;
    return false;
  }
}

template <typename Row>
Linear_Expression_Interface::const_iterator_interface*
Linear_Expression_Impl<Row>::begin() const {
  return new const_iterator(row, 1);
}

template <typename Row>
Linear_Expression_Interface::const_iterator_interface*
Linear_Expression_Impl<Row>::end() const {
  return new const_iterator(row, row.size());
}

template <typename Row>
Linear_Expression_Interface::const_iterator_interface*
Linear_Expression_Impl<Row>::lower_bound(Variable v) const {
  return new const_iterator(row, v.space_dimension());
}

template <typename Row>
Linear_Expression_Impl<Row>::const_iterator
::const_iterator(const Row& row1, dimension_type i)
  : row(&row1), itr(row1.lower_bound(i)) {
  skip_zeroes_forward();
}

template <typename Row>
Linear_Expression_Interface::const_iterator_interface*
Linear_Expression_Impl<Row>::const_iterator
::clone() const {
  return new const_iterator(*this);
}

template <typename Row>
void
Linear_Expression_Impl<Row>::const_iterator
::operator++() {
  ++itr;
  skip_zeroes_forward();
}

template <typename Row>
void
Linear_Expression_Impl<Row>::const_iterator
::operator--() {
  --itr;
  skip_zeroes_backward();
}

template <typename Row>
typename Linear_Expression_Impl<Row>::const_iterator::reference
Linear_Expression_Impl<Row>::const_iterator
::operator*() const {
  return *itr;
}

template <typename Row>
Variable
Linear_Expression_Impl<Row>::const_iterator
::variable() const {
  const dimension_type i = itr.index();
  PPL_ASSERT(i != 0);
  return Variable(i - 1);
}

template <typename Row>
bool
Linear_Expression_Impl<Row>::const_iterator
::operator==(const const_iterator_interface& x) const {
  const const_iterator* const p = dynamic_cast<const const_iterator*>(&x);
  // Comparing iterators belonging to different rows is forbidden.
  PPL_ASSERT(p != 0);
  PPL_ASSERT(row == p->row);
  return itr == p->itr;
}

template <typename Row>
void
Linear_Expression_Impl<Row>::ascii_dump(std::ostream& s) const {
  s << "size " << (space_dimension() + 1) << " ";
  for (dimension_type i = 0; i < row.size(); ++i) {
    s << row.get(i);
    if (i != row.size() - 1)
      s << ' ';
  }
}

template <typename Row>
bool
Linear_Expression_Impl<Row>::ascii_load(std::istream& s) {
  std::string str;

  if (!(s >> str))
    return false;
  if (str != "size")
    return false;

  dimension_type new_size;
  if (!(s >> new_size))
    return false;

  row.resize(0);
  row.resize(new_size);

  PPL_DIRTY_TEMP_COEFFICIENT(c);

  for (dimension_type j = 0; j < new_size; ++j) {
    if (!(s >> c))
      return false;
    if (c != 0)
      row.insert(j, c);
  }

  PPL_ASSERT(OK());
  return true;
}

template <typename Row>
bool
Linear_Expression_Impl<Row>::OK() const {
  return row.OK();
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Linear_Expression_Impl_templates_hh)
