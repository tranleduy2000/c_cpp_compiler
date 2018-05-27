/* Test Box::concatenate_assign().
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
  // Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  // Variable x4(3);
  Variable x5(4);
  Variable x6(5);
  // Variable x7(6);
  Variable x8(7);
  Variable x9(8);

  TBox box1(6);
  box1.add_constraint(x3 <= 2);
  box1.add_constraint(x5 <= 3);

  TBox box2(3);
  box2.add_constraint(x3 <= 7);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.concatenate_assign(box2);

  Rational_Box known_result(9);
  known_result.add_constraint(x3 <= 2);
  known_result.add_constraint(x5 <= 3);
  known_result.add_constraint(x9 <= 7);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.concatenate_assign(box2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 3);

  TBox box2(0, EMPTY);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.concatenate_assign(box1);

  Rational_Box known_result(2, EMPTY);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.concatenate_assign(box1) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x <= 0);
  cs.insert(y == 3);
  cs.insert(3*x - 3*y <= 5);

  TBox box1(2);
  box1.refine_with_constraints(cs);

  TBox box2(0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.concatenate_assign(box2);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 0);
  known_result.add_constraint(y == 3);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.concatenate_assign(box2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box1(3);
  box1.add_constraint(A >= 0);
  box1.add_constraint(B >= 0);
  box1.add_constraint(C >= 0);

  TBox box2(2);
  box2.add_constraint(A >= 0);
  box2.add_constraint(A <= 1);
  box2.add_constraint(B >= 0);
  box2.add_constraint(B <= 2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.concatenate_assign(box2);

  Rational_Box known_result(5);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(D <= 1);
  known_result.add_constraint(E >= 0);
  known_result.add_constraint(E <= 2);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.concatenate_assign(box2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 3);

  TBox box2(0, EMPTY);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.concatenate_assign(box2);

  Rational_Box known_result(2, EMPTY);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.concatenate_assign(box2) ***");

  return ok;
}

/* Concatenate an empty box to a universe */
/*
   This shows a bug in either concatenate_assign() or OK()
   When executing box1.concatenate_assign(box2),
   the assertion `box1.OK()' fails.
*/
bool
test06() {
  Variable x(0);

  TBox box1(1);

  TBox box2(1);
  box2.add_constraint(x <= 0);
  box2.add_constraint(x >= 1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.OK();
  box2.OK();

  box1.concatenate_assign(box2);

  Rational_Box known_result(2, EMPTY);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.concatenate_assign(box2) ***");

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
