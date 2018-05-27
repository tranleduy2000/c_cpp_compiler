/* Test BD_Shape::is_disjoint_from(const BD_Shape& y).
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

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(B == 0);
  bds1.add_constraint(A >= 1);
  bds1.add_constraint(A <= 2);

  TBD_Shape bds2(2);
  bds2.add_constraint(A == 0);
  bds2.add_constraint(B >= 1);
  bds2.add_constraint(B <= 2);

  bool ok = bds1.is_disjoint_from(bds2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(3);

  TBD_Shape bds2(3);
  bds2.add_constraint(A <= 3);
  bds2.add_constraint(B - A <= -1);
  bds2.add_constraint(B >= -5);

  bool disjoint = bds1.is_disjoint_from(bds2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  return !disjoint;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);

  Constraint_System cs1;
  cs1.insert(x1 >= -4);
  cs1.insert(x2 - x1 <= 0);
  cs1.insert(x1 - x2 <= -5);

  TBD_Shape bds1(2);
  bds1.add_constraints(cs1);

  print_constraints(bds1, "*** bds1 ***");

  Constraint_System cs2;
  cs2.insert(2*x1 >= 1);
  cs2.insert(2*x1 <= 3);
  cs2.insert(6*x2 <= 1);
  cs2.insert(3*x2 >= -2);

  TBD_Shape bds2(2);
  bds2.add_constraints(cs2);

  print_constraints(bds2, "*** bds2 ***");

  bool disjoint = bds1.is_disjoint_from(bds2);

  return disjoint;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x >= y);

  TBD_Shape bds2(3);

  try {
    // This is an invalid use of method
    // BD_Shape::is_disjoint_from(bds2): it is illegal
    // to apply this method to two polyhedra of different dimensions.
    bds1.is_disjoint_from(bds2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2, EMPTY);

  TBD_Shape bds2(2);
  bds2.add_constraint(A - B <= 5);

  bool disjoint = bds1.is_disjoint_from(bds2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  return disjoint;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= -4);

  TBD_Shape bds2(2, EMPTY);

  bool disjoint = bds1.is_disjoint_from(bds2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  return disjoint;
}

bool
test07() {
  Variable A(0);

  TBD_Shape bds1(2);
  bds1.add_constraint(A == 0);

  TBD_Shape bds2(2);
  bds2.add_constraint(A == 1);

  bool ok = bds1.is_disjoint_from(bds2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

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
  DO_TEST(test07);
END_MAIN
