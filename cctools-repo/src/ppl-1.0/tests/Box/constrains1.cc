/* Test Box::constrains().
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

#include "ppl_test.hh"

#define TEST_PREDICATE_TRUE(pred)		\
  if (!pred) {					\
    nout << "!" #pred << endl;			\
    ok = false;					\
  }

#define TEST_PREDICATE_FALSE(pred)		\
  if (pred) {					\
    nout << #pred << endl;			\
    ok = false;					\
  }

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(0*A == 0);

  bool ok = true;

  TEST_PREDICATE_FALSE(box.constrains(A));
  TEST_PREDICATE_FALSE(box.constrains(B));

  box.add_constraint(0*A == 1);

  TEST_PREDICATE_TRUE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));

  box = TBox(2);
  box.add_constraint(A == 2);
  box.add_constraint(B == 4);

  TEST_PREDICATE_TRUE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));

  box = TBox(2);

  TEST_PREDICATE_FALSE(box.constrains(A));
  TEST_PREDICATE_FALSE(box.constrains(B));

  box.add_constraint(A >= 1);

  TEST_PREDICATE_TRUE(box.constrains(A));
  TEST_PREDICATE_FALSE(box.constrains(B));

  box.add_constraint(B >= 2);

  TEST_PREDICATE_TRUE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));

  box.add_constraint(A <= 2);

  TEST_PREDICATE_TRUE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3, EMPTY);

  bool ok = true;

  TEST_PREDICATE_TRUE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));
  TEST_PREDICATE_TRUE(box.constrains(C));

  return ok;
}

bool
test03() {
  Variable A(0);

  TBox box(0);
  box.add_constraint(Linear_Expression::zero() == 1);

  try {
    (void) box.constrains(A);
  }
  catch (std::invalid_argument& e) {
    return true;
  }
  catch (...) {
    return false;
  }

  return false;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(B >= 0);
  box.add_constraint(B >= 7);

  bool ok = true;

  TEST_PREDICATE_FALSE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A == 0);
  box.add_constraint(B == 0);
  box.unconstrain(A);

  bool ok = true;

  TEST_PREDICATE_FALSE(box.constrains(A));
  TEST_PREDICATE_TRUE(box.constrains(B));

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(C >= 1);
  box.add_constraint(C <= 0);

  bool ok = true;

  TEST_PREDICATE_TRUE(box.constrains(C));
  TEST_PREDICATE_TRUE(box.constrains(B));
  TEST_PREDICATE_TRUE(box.constrains(A));

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
