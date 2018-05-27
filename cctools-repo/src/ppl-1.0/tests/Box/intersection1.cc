/* Test Box::intersection_assign().
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
  Variable x(0);
  Variable y(1);

  TBox box1(3);
  box1.add_constraint(x <= 3);

  TBox box2(3);
  box2.add_constraint(-y <= -2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result(3);
  known_result.add_constraint(x <= 3);
  known_result.add_constraint(-y <= -2);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(4);
  box1.add_constraint(x <= 3);

  TBox box2(4);
  box2.add_constraint(-y <= -2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result(4);
  known_result.add_constraint(x <= 3);
  known_result.add_constraint(-y <= -2);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  // Variable z(2);

  TBox box1(3);
  box1.add_constraint(x <= 4);
  box1.add_constraint(-x <= -1);
  box1.add_constraint(y <= 3);
  box1.add_constraint(-y <= -1);

  TBox box2(3);
  box2.add_constraint(x <= 3);
  box2.add_constraint(-y <= 5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result(3);
  known_result.add_constraint(x <= 3);
  known_result.add_constraint(-x <= -1);
  known_result.add_constraint(y <= 3);
  known_result.add_constraint(-y <= -1);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  // Variable z(2);

  TBox box1(3);
  box1.add_constraint(x <= 4);
  box1.add_constraint(x >= 5);
  box1.add_constraint(y <= 3);
  box1.add_constraint(y >= 1);

  TBox box2(3);
  box2.add_constraint(x <= 3);
  box2.add_constraint(y >= -5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  // Variable z(2);

  TBox box1(3);
  box1.add_constraint(x <= 4);
  box1.add_constraint(x >= 5);
  box1.add_constraint(y <= 3);
  box1.add_constraint(y >= 1);

  TBox box2(3);
  box2.add_constraint(x <= 3);
  box2.add_constraint(y >= -5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.intersection_assign(box1);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.intersection_assign(box1) ***");

  return ok;
}

bool
test06() {
  TBox box1;
  TBox box2;

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result;

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  // Variable z(2);

  TBox box1(3);

  TBox box2(3);
  box2.add_constraint(x <= 3);
  box2.add_constraint(y >= -5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Rational_Box known_result(box2);

  box1.intersection_assign(box2);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  // Variable z(2);

  TBox box1(3);
  TBox box2(3);
  box2.add_constraint(x <= 3);
  box2.add_constraint(y >= -5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Rational_Box known_result(box2);

  box1.intersection_assign(box2);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);

  TBox box2(3);

  try {
    // This is an invalid use of method
    // Box::intersection_assign(box2): it is illegal
    // to apply this method to two shapes having different dimensions.
    box1.intersection_assign(box2);
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
test10() {
  TBox box1(7);
  TBox box2(15);

  try {
    // This is an invalid use of method
    // Box::intersection_assign(box2): it is illegal
    // to apply this method to two shapes having different dimensions.
    box1.intersection_assign(box2);
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
test11() {
  TBox box1(3, EMPTY);
  TBox box2(3);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box1, known_result) ;

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test12() {
  TBox box1(3);
  TBox box2(3, EMPTY);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.intersection_assign(box2);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box1, known_result) ;

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");

  return ok;
}

bool
test13() {
  Variable x(0);

  TBox box1(1);
  box1.add_constraint(x > 0);
  box1.add_constraint(x <= 1);

  TBox box2(1);
  box2.add_constraint(x == 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  if (box1.is_empty())
    return false;

  box1.intersection_assign(box2);

  TBox known_result(1, EMPTY);

  bool ok = (known_result == box1) ;

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test14() {
  Variable x(0);

  TBox box1(1);
  box1.add_constraint(x > 0);
  box1.add_constraint(x <= 1);

  TBox box2(1);
  box2.add_constraint(x == 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  if (box1.is_empty())
    return false;

  box1.intersection_assign(box2);
  bool non_empty = !box1.is_empty();

  TBox known_result(1, EMPTY);

  bool ok = (known_result == box1) && !non_empty;

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}
bool
test15() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 2);
  box1.add_constraint(y <= 2);

  TBox box2(2);
  box2.add_constraint(x == 1);
  box2.add_constraint(y >= 1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  if (box1.is_empty())
    return false;

  box1.intersection_assign(box2);
  bool non_empty = !box1.is_empty();

  TBox known_result(2);
  known_result.add_constraint(x == 1);
  known_result.add_constraint(y >= 1);
  known_result.add_constraint(y <= 2);

  bool ok = (known_result == box1) && non_empty;

  print_constraints(box1, "*** box1.intersection_assign(box2) ***");
  print_constraints(known_result, "*** known_result ***");

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
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
END_MAIN
