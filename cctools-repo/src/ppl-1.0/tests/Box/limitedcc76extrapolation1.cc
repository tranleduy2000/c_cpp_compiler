/* Test Box::limited_CC76_extrapolation_assign().
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
  TBox box1(0);
  TBox box2(0);
  Constraint_System cs;

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Rational_Box known_result(box1);

  box1.limited_CC76_extrapolation_assign(box2, cs);

  bool ok = check_result(box1, known_result);

  print_constraints(box1,
                    "*** box1.limited_CC76_extrapolation_assign(box2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A == -2);

  TBox box2(2);
  box2.add_constraint(A == -2);
  box2.add_constraint(B == 3);

  Constraint_System cs;
  cs.insert(A <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  Rational_Box known_result(box1);

  box1.limited_CC76_extrapolation_assign(box2, cs);

  bool ok = check_result(box1, known_result);

  print_constraints(box1,
                    "*** box1.limited_CC76_extrapolation_assign(box2, cs) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x <= 1);
  cs1.insert(y >= 4);
  TBox box1(cs1);

  Constraint_System cs2;
  cs2.insert(x == 0);
  cs2.insert(y >= 5);
  TBox box2(cs2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Constraint_System cs;
  cs.insert(x >= 20);
  cs.insert(y >= 3);

  print_constraints(cs, "*** cs ***");

  box1.limited_CC76_extrapolation_assign(box2, cs);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 3);

  bool ok = check_result(box1, known_result);

  print_constraints(box1,
                    "*** box1.limited_CC76_extrapolation_assign(box2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box1(3);
  box1.add_constraint(A <= 4);
  box1.add_constraint(B >= 1);

  TBox box2(3);
  box2.add_constraint(A <= -2);
  box2.add_constraint(B >= 4);

  Constraint_System cs;
  cs.insert(A <= 5);
  cs.insert(A - B + C <= 6);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  box1.limited_CC76_extrapolation_assign(box2, cs);

  Rational_Box known_result(3);
  known_result.add_constraint(A <= 5);
  known_result.add_constraint(B >= 1);

  bool ok = check_result(box1, known_result);

  print_constraints(box1,
                    "*** box1.limited_CC76_extrapolation_assign(box2, cs) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x <= 1);
  cs1.insert(y >= 4);

  TBox box1(cs1);
  TBox box2(2, EMPTY);

  Constraint_System cs;
  cs.insert(x <= 0);
  cs.insert(y >= 3);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  Rational_Box known_result(box1);

  box1.limited_CC76_extrapolation_assign(box2, cs);

  bool ok = check_result(box1, known_result);

  print_constraints(box1,
                    "*** box1.limited_CC76_extrapolation_assign(box2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, EMPTY);
  TBox box2(2, EMPTY);

  Constraint_System cs;
  cs.insert(x <= 0);
  cs.insert(y >= 3);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  Rational_Box known_result(box1);

  box1.limited_CC76_extrapolation_assign(box2, cs);

  bool ok = check_result(box1, known_result);

  print_constraints(box1,
                    "*** box1.limited_CC76_extrapolation_assign(box2) ***");

  return ok;
}

bool
test07() {
  Variable y(1);

  TBox box1(1);
  TBox box2(2);

  Constraint_System cs;
  cs.insert(y <= 9);

  try {
    // This is an invalid use of the method
    // Box::limited_CC76_extrapolation_assign(box2, cs): it is
    // illegal to apply the method to two polyhedra that are not
    // dimension-compatible.
    box2.limited_CC76_extrapolation_assign(box1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(2);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 2);

  TBox box2(2);
  box2.add_constraint(x >= 0);
  box2.add_constraint(x <= 5);

  Constraint_System cs;
  cs.insert(z <= 5);

  try {
    // This is an invalid use of the method
    // Box::limited_CC76_extrapolation_assign(box, cs): it is
    // illegal to apply the method to a system of constraints that
    // is not dimension-compatible with the two polyhedra.
    box2.limited_CC76_extrapolation_assign(box1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 2);

  TBox box2(2);
  box2.add_constraint(x >= 0);
  box2.add_constraint(x < 5);

  Constraint_System cs;
  cs.insert(x < 5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  Rational_Box known_result(box2);

  box2.limited_CC76_extrapolation_assign(box1, cs);

  bool ok = check_result(box2, known_result);

  print_constraints(box2,
                    "*** box2.limited_CC76_extrapolation_assign(box1) ***");

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
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
