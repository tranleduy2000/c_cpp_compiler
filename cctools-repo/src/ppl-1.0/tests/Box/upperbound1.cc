/* Test Box::upper_bound_assign().
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
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);

  TBox box1(5);
  box1.add_constraint(x1 <= 5);
  box1.add_constraint(x2 <= -1);

  TBox box2(5);
  box2.add_constraint(x1 <= 2);
  box2.add_constraint(x4 <= 7);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.upper_bound_assign(box2);

  Rational_Box known_result(5);
  known_result.add_constraint(x1 <= 5);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.upper_bound_assign(box2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

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

  box1.upper_bound_assign(box2);

  Rational_Box known_result(3);
  known_result.add_constraint(x <= 4);
  known_result.add_constraint(y >= -5);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.upper_bound_assign(box2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 4);
  box1.add_constraint(-x <= -1);
  box1.add_constraint(y <= 3);
  box1.add_constraint(-y <= -1);

  TBox box2(2);
  box2.add_constraint(x <= 3);
  box2.add_constraint(x >= 5);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Rational_Box known_result(box1);

  box1.upper_bound_assign(box2);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.upper_bound_assign(box2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A <= 4);
  box1.add_constraint(A >= 1);
  box1.add_constraint(B <= 3);
  box1.add_constraint(-B <= -1);

  TBox box2(2);
  box2.add_constraint(A <= 8);
  box2.add_constraint(-B <= 5);
  box2.add_constraint(B <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.upper_bound_assign(box2);

  Rational_Box known_result(2);
  known_result.add_constraint(A <= 8);
  known_result.add_constraint(B >= -5);
  known_result.add_constraint(B <= 3);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.upper_bound_assign(box2) ***");

  return ok;
}

bool
test05() {
  TBox box1(12);
  TBox box2(5);

  try {
    // This is an incorrect use of method
    // Box::upper_bound_assign(box2): it is impossible to apply
    // this method to two polyhedra of different dimensions.
    box1.upper_bound_assign(box2);
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
test06() {
  Variable A(0);

  TBox box1(1);
  box1.add_constraint(A <= 0);
  box1.add_constraint(A >= 1);

  TBox box2(1);
  box2.add_constraint(A <= 3);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.upper_bound_assign(box2);

  Rational_Box known_result(1);
  known_result.add_constraint(A <= 3);

  bool ok = check_result(box1, known_result) ;

  print_constraints(box1, "*** box1.upper_bound_assign(box2) ***");

  return ok;
}

bool
test07() {
  Variable x1(0);

  TBox box1(1);
  box1.add_constraint(x1 <= 5);

  TBox box2(1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.upper_bound_assign(box2);

  return true;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(A == 0);
  box1.add_constraint(B == 1);

  TBox box2(box1);
  box1.affine_image(B, Linear_Expression(4));

  box2.add_constraint(A > 1);
  box1.upper_bound_assign(box2);

  TBox known_result(2, UNIVERSE);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B == 4);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "=== box1 ===");
  print_constraints(box2, "=== known_result ===");
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
END_MAIN
