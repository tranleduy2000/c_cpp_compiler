/* Test the Sparse_Matrix class.
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

#include "ppl_test.hh"

#if PPL_USE_SPARSE_MATRIX

#include <vector>
#include <algorithm>
#include <set>

namespace {

bool
test01() {

  Sparse_Row row(5);

  if (row.size() != 5)
    return false;

  row.resize(4);

  if (row.size() != 4)
    return false;

  return true;
}

bool
test02() {

  Sparse_Row row(5);

  row[1] = 5;
  row[3] = 6;

  Sparse_Row::iterator itr = row.begin();

  if (itr == row.end())
    return false;

  if (itr.index() != 1)
    return false;

  if (*itr != 5)
    return false;

  ++itr;

  if (itr == row.end())
    return false;

  if (itr.index() != 3)
    return false;

  if (*itr != 6)
    return false;

  ++itr;

  if (itr != row.end())
    return false;

  --itr;

  if (itr == row.end())
    return false;

  if (itr.index() != 3)
    return false;

  if (*itr != 6)
    return false;

  Sparse_Row::const_iterator citr = row.cbegin();

  if (citr == row.cend())
    return false;

  if (citr.index() != 1)
    return false;

  if (*citr != 5)
    return false;

  ++citr;

  if (citr == row.cend())
    return false;

  if (citr.index() != 3)
    return false;

  if (*citr != 6)
    return false;

  ++citr;

  if (citr != row.cend())
    return false;

  --citr;

  if (citr == row.cend())
    return false;

  if (citr.index() != 3)
    return false;

  if (*citr != 6)
    return false;

  return true;
}

bool
test03() {

  Sparse_Row row(10);

  row[1] = 2;
  row[3] = 4;
  row[5] = 6;
  row[7] = 8;
  row[9] = 10;

  Sparse_Row::iterator itr = row.find(3);

  row.reset(itr);

  if (row.get(3) != 0)
    return false;

  row[3] = 4;

  itr = row.find(3);
  Sparse_Row::iterator itr2 = row.find(7);

  row.reset(itr, itr2);

  if (row.get(1) != 2)
    return false;

  if (row.get(3) != 0)
    return false;

  if (row.get(5) != 0)
    return false;

  if (row.get(7) != 8)
    return false;

  return true;
}

bool
test04() {

  Sparse_Row row(2);

  row[1] = 2;

  if (row.get(1) != 2)
    return false;

  if (row.find(0) != row.end())
    return false;

  row.swap_coefficients(0, 1);

  if (row.get(0) != 2)
    return false;

  if (row.find(1) != row.end())
    return false;

  row.swap_coefficients(0, 1);

  if (row.get(1) != 2)
    return false;

  if (row.find(0) != row.end())
    return false;

  row[0] = 3;

  if (row.get(1) != 2)
    return false;

  if (row.get(0) != 3)
    return false;

  row.swap_coefficients(0, 1);

  if (row.get(1) != 3)
    return false;

  if (row.get(0) != 2)
    return false;

  row.clear();

  if (row.find(1) != row.end())
    return false;

  if (row.find(0) != row.end())
    return false;

  row.swap_coefficients(0, 1);

  if (row.find(1) != row.end())
    return false;

  if (row.find(0) != row.end())
    return false;

  if (row.begin() != row.end())
    return false;

  if (row.cbegin() != row.cend())
    return false;

  return true;
}

class test05_functor {
public:

  inline void
  operator()(Coefficient& x, const Coefficient& y) const {
    x *= 2 - y;
  }

  inline void
  operator()(Coefficient& x) const {
    x *= 2;
  }
};

bool
test05() {

  Sparse_Row x(9);

  // x: ***000111
  // (`*' is an unstored zero)

  x[3] = 0;
  x[4] = 0;
  x[5] = 0;
  x[6] = 1;
  x[7] = 1;
  x[8] = 1;

  Sparse_Row y(9);

  // y: *01*01*01
  // (`*' is an unstored zero)

  y[1] = 0;
  y[2] = 1;
  y[4] = 0;
  y[5] = 1;
  y[7] = 0;
  y[8] = 1;

  // x *= 2 - y

  x.combine_needs_first(y, test05_functor(), test05_functor());

  // x: ******221
  // (`*' is an unstored zero)

  if (x.find(0) != x.end())
    return false;

  if (x.find(1) != x.end())
    return false;

  if (x.find(2) != x.end())
    return false;

  if (x.find(3) != x.end())
    return false;

  if (x.find(4) != x.end())
    return false;

  if (x.find(5) != x.end())
    return false;

  if (x.get(6) != 2)
    return false;

  if (x.get(7) != 2)
    return false;

  if (x.get(8) != 1)
    return false;

  return true;
}

class test06_functor {
public:

  inline void
  operator()(Coefficient& x, const Coefficient& y) const {
    x += y;
  }

  inline void
  operator()(Coefficient& /* x */) const {
  }
};

bool
test06() {

  Sparse_Row x(3);

  x[1] = 0;
  x[2] = 1;

  // x: *01

  x.combine(x, test06_functor(), test06_functor(), test06_functor());

  // x: *02

  if (x.find(0) != x.end())
    return false;

  if (x.get(1) != 0)
    return false;

  if (x.get(2) != 2)
    return false;

  return true;
}

class test07_functor_1 {
public:

  inline void
  operator()(Coefficient& x, const Coefficient& y) const {
    x = (x % 2) - (y % 2);
  }

  inline void
  operator()(Coefficient& x) const {
    x %= 2;
  }
};

class test07_functor_2 {
public:

  inline void
  operator()(Coefficient& x, const Coefficient& y) const {
    PPL_ASSERT(x == 0);
    x = -(y % 2);
  }
};

bool
test07() {

  Sparse_Row x(16);

  //    0123456789ABCDEF
  // x: ****000011112222

  x[ 4] = 0;
  x[ 5] = 0;
  x[ 6] = 0;
  x[ 7] = 0;
  x[ 8] = 1;
  x[ 9] = 1;
  x[10] = 1;
  x[11] = 1;
  x[12] = 2;
  x[13] = 2;
  x[14] = 2;
  x[15] = 2;

  Sparse_Row y(16);

  //    0123456789ABCDEF
  // y: *012*012*012*012

  y[ 1] = 0;
  y[ 2] = 1;
  y[ 3] = 2;
  y[ 5] = 0;
  y[ 6] = 1;
  y[ 7] = 2;
  y[ 9] = 0;
  y[10] = 1;
  y[11] = 2;
  y[13] = 0;
  y[14] = 1;
  y[15] = 2;

  // x = (x % 2) - (y % 2)

  x.combine(y, test07_functor_1(), test07_functor_1(), test07_functor_2());

  //    0123456789ABCDEF
  // x: ******-**1*1**-*
  //
  // Legend:
  // *: unstored zero
  // -: -1

  if (x.find(0) != x.end()) return false;
  if (x.find(1) != x.end()) return false;
  if (x.get(2) != -1) return false;
  if (x.find(3) != x.end()) return false;
  if (x.find(4) != x.end()) return false;
  if (x.find(5) != x.end()) return false;
  if (x.get(6) != -1) return false;
  if (x.find(7) != x.end()) return false;
  if (x.get(8) != 1) return false;
  if (x.get(9) != 1) return false;
  if (x.find(10) != x.end()) return false;
  if (x.get(11) != 1) return false;
  if (x.find(12) != x.end()) return false;
  if (x.find(13) != x.end()) return false;
  if (x.get(14) != -1) return false;
  if (x.find(15) != x.end()) return false;

  return true;
}

bool
test08() {

  Sparse_Row row(4);

  if (row.lower_bound(2) != row.end())
    return false;

  const Sparse_Row& crow = row;

  if (crow.lower_bound(2) != crow.end())
    return false;

  row[2] = 3;

  if (row.lower_bound(1).index() != 2)
    return false;

  if (crow.lower_bound(1).index() != 2)
    return false;

  if (row.lower_bound(2).index() != 2)
    return false;

  if (crow.lower_bound(2).index() != 2)
    return false;

  if (row.lower_bound(3) != row.end())
    return false;

  if (crow.lower_bound(3) != crow.end())
    return false;

  // Now the same checks with a dummy hint.

  if (row.lower_bound(row.end(), 1).index() != 2)
    return false;

  if (crow.lower_bound(crow.end(), 1).index() != 2)
    return false;

  if (row.lower_bound(row.end(), 2).index() != 2)
    return false;

  if (crow.lower_bound(crow.end(), 2).index() != 2)
    return false;

  if (row.lower_bound(row.end(), 3) != row.end())
    return false;

  if (crow.lower_bound(crow.end(), 3) != crow.end())
    return false;

  return true;
}

bool
test09() {
  // These test the construction of a Sparse_Row from a Dense_Row.
  {
    Dense_Row dense(3);
    Sparse_Row sparse(dense);
    if (sparse.size() != dense.size())
      return false;
    if (sparse.begin() != sparse.end())
      return false;
  }

  {
    Dense_Row dense(5);
    dense[1] = 2;
    dense[3] = 4;
    Sparse_Row sparse(dense);
    if (sparse.size() != dense.size())
      return false;
    Sparse_Row::iterator itr = sparse.begin();

    if (itr == sparse.end())
      return false;
    if (itr.index() != 1)
      return false;
    if (*itr != 2)
      return false;

    ++itr;

    if (itr == sparse.end())
      return false;
    if (itr.index() != 3)
      return false;
    if (*itr != 4)
      return false;

    ++itr;

    if (itr != sparse.end())
      return false;
  }

  {
    Dense_Row dense(5);
    dense[0] = 1;
    dense[2] = 3;
    dense[4] = 5;
    Sparse_Row sparse(dense);
    if (sparse.size() != dense.size())
      return false;
    Sparse_Row::iterator itr = sparse.begin();

    if (itr == sparse.end())
      return false;
    if (itr.index() != 0)
      return false;
    if (*itr != 1)
      return false;

    ++itr;

    if (itr == sparse.end())
      return false;
    if (itr.index() != 2)
      return false;
    if (*itr != 3)
      return false;

    ++itr;

    if (itr == sparse.end())
      return false;
    if (itr.index() != 4)
      return false;
    if (*itr != 5)
      return false;

    ++itr;

    if (itr != sparse.end())
      return false;
  }

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN

#else // !PPL_USE_SPARSE_MATRIX

// A dummy test to avoid compiler warnings in BEGIN_MAIN.
bool test01() {
  return true;
}

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN

#endif // !PPL_USE_SPARSE_MATRIX
