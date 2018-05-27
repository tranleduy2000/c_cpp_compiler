/* Test Box::affine_image().
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

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 4);

  print_constraints(box, "*** box ***");

  box.affine_image(A, B);

  Rational_Box known_result(3);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B <= 4);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(A, B) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 2);

  print_constraints(box, "*** box ***");

  box.affine_image(A, A + 4);

  Rational_Box known_result(3);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(A <= 6);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(A, A + 4) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 2);

  print_constraints(box, "*** box ***");

  box.affine_image(A, Linear_Expression(4));

  Rational_Box known_result(3);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(A == 4);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(A, 4) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 2);

  print_constraints(box, "*** box ***");

  Rational_Box known_result(box);

  box.affine_image(A, A);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(A, A) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 2);

  print_constraints(box, "*** box ***");

  box.affine_image(A, 2*A - 2, 2);

  Rational_Box known_result(3);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(A, 2*A - 2, 2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 4);

  print_constraints(box, "*** box ***");

  box.affine_image(B, 2*A, 2);

  Rational_Box known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(B, 2*A, 2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBox box(3);
  box.add_constraint(A <= 2);
  box.add_constraint(B <= 4);

  print_constraints(box, "*** box ***");

  box.affine_image(B, 3*A + 3, 3);

  Rational_Box known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B <= 3);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(B, 3*A + 3, 3) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A <= 1);
  box.add_constraint(A >= 0);
  box.add_constraint(B <= 2);
  box.add_constraint(B >= -1);

  print_constraints(box, "*** box ***");

  box.affine_image(A, -2*A - 3*B + 1, -5);

  Rational_Box known_result(2);
  known_result.add_constraint(5*A >= -4);
  known_result.add_constraint(5*A <= 7);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(B >= -1);

  bool ok = check_result(box, known_result, "1.08e-7", "9.62e-8", "9.54e-8");

  print_constraints(box, "*** box.affine_image(A, -2*A - 3*B + 1, -5) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A <= 1);
  box.add_constraint(B <= 2);
  box.add_constraint(C >= 3);

  print_constraints(box, "*** box ***");

  box.affine_image(C, A + 2*B -3*C + 2, 4);

  Rational_Box known_result(3);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(2*C <= -1);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(C, A + 2*B -3*C + 2, 4) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(4);
  box.add_constraint(A <= 1);
  box.add_constraint(B <= 2);
  box.add_constraint(B >= 1);
  box.add_constraint(C <= 0);
  box.add_constraint(D == 3);

  print_constraints(box, "*** box ***");

  box.affine_image(A, -B + 2*C + 1, -3);

  Rational_Box known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(C <= 0);
  known_result.add_constraint(D == 3);

  bool ok = check_result(box, known_result, "7.95e-8", "7.95e-8", "7.95e-8");

  print_constraints(box, "*** box.affine_image(A, -B + 2*C + 1, -3) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A <= 1);
  box.add_constraint(B <= 2);

  print_constraints(box, "*** box ***");

  box.affine_image(A, 2*B + C + 2, 4);

  Rational_Box known_result(3);
  known_result.add_constraint(B <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_image(A, 2*B + C + 2, 4) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(A <= 2);

  Linear_Expression coeff1 = A + 1;

  try {
    // This is an incorrect use of method
    // Box::affine_image(v, expr, d): it is illegal to apply
    // the method to a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    box.affine_image(A, coeff1, d);
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
test13() {
  Variable A(0);
  Variable B(1);

  TBox box(1);
  box.add_constraint(A >= 1);

  try {
    // This is an invalid used of the method
    // Box::affine_image(v, expr, d): it is illegal to
    // apply this method to a variable that is not in the space of
    // the polyhedron.
    box.affine_image(B, A + 1);
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
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(2);
  box.add_constraint(A >= 1);
  box.add_constraint(B >= 1);

  try {
    // This is an invalid used of the method
    // Box::affine_image(v, expr, d): it is illegal to
    // use a variable in the expression that does not appear in the
    // space of the polyhedron.
    box.affine_image(B, A + C + 1);
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
test15() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(A <= 2);

  Linear_Expression coeff1 = 2*A + 1;

  try {
    // This is an incorrect use of method
    // Box::affine_image(v, expr, d): it is illegal to apply
    // the method to a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    box.affine_image(A, coeff1, d);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
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
