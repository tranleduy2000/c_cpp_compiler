/* Test Box::bounded_affine_image().
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

// Affine image variable is neither expression, denominator 1.
bool
test01() {
  Variable x(0);
  Variable y(1);

  TBox box(3);
  box.add_constraint(x <= 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(x, y, y);

  Rational_Box known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.bounded_affine_image(x, y, y) ***");

  return ok;
}

// Affine image variable is in neither ub_expr nor lb_expr, negative
// denominator.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box(5);
  box.add_constraint(A <= 0);
  box.add_constraint(B <= 1);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(B, 3*E - 5*D + A - 3*B, 4*A -2*C + 3, -3);

  Rational_Box known_result(5);
  known_result.add_constraint(A <= 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.bounded_affine_image(B, "
                        "3*E - 5*D + A - 3*B, 4*A - 2*C + 3, -3) ***");

  return ok;
}

// Affine image variable is in lb_expr but not ub_expr, negative denominator.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box(5);
  box.add_constraint(A <= 1);
  box.add_constraint(B <= 1);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(C, 3*D - E, 2*C + 1, -5);

  Rational_Box known_result(5);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(A <= 1);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.bounded_affine_image(C, "
                        "3*D - E, 2*C + 1, -5) ***");

  return ok;
}

// Affine image variable is in ub_expr but not lb_expr, positive denominator.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box(5);
  box.add_constraint(A <= 0);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(B, -B - 2, 7*D - E + 5, 3);

  Rational_Box known_result(5);
  known_result.add_constraint(A <= 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.bounded_affine_image(B, "
                        "-B - 2, 7*D - E + 5, 3) ***");

  return ok;
}

// Affine image variable is in ub_expr but not lb_expr, negative denominator.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable F(5);

  TBox box(6);
  box.add_constraint(A <= 4);
  box.add_constraint(A >= -6);
  box.add_constraint(B == 0);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(A, -A - 3, B - C + 6*D + F, -1);

  Rational_Box known_result(6);
  known_result.add_constraint(A >= -3);
  known_result.add_constraint(B == 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.bounded_affine_image(A, "
                        "-A - 3, B - C + 6*D + F, -1) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr and
// lb_expr are bounded by the box.
bool
test06() {
  Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(x <= 1);
  box.add_constraint(x >= 0);
  box.add_constraint(y <= 2);
  box.add_constraint(y >= -1);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(x, -2*x + y + 1, -2*x + y + 1);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 3);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.bounded_affine_image(x, -2*x+y+1, -2*x+y+1) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr and
// lb_expr are bounded by the box, negative denominator.
bool
test07() {
  Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(x <= 1);
  box.add_constraint(x >= 0);
  box.add_constraint(y <= 2);
  box.add_constraint(y >= -1);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);
  box.bounded_affine_image(x, -2*x + y + 1, -2*x + y + 1, -1);
  ph.bounded_affine_image(x, -2*x + y + 1, -2*x + y + 1, -1);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(x >= -3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.bounded_affine_image("
                    "x, -2*x+y+1, -2*x+y+1, -1) ***");
  print_constraints(ph,
                    "*** ph.bounded_affine_image("
                    "x, -2*x+y+1, -2*x+y+1, -1) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr and
// lb_expr are bounded by the box, lower is open.
bool
test08() {
  Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(x <= 1);
  box.add_constraint(x > 0);
  box.add_constraint(y <= 2);
  box.add_constraint(y >= -1);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(x, -2*x + y + 1, -2*x + y + 1);

  Rational_Box known_result(2);
  known_result.add_constraint(x < 3);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.bounded_affine_image("
                    "x, -2*x+y+1, -2*x+y+1) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr has a
// maximum value but the lb_expr has no minimum, negative denominator.
bool
test09() {
  Variable x(0);
  Variable y(1);

  TBox box(3);
  box.add_constraint(3*x <= 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");
  box.bounded_affine_image(x, x + 4, x + 4, -1);

  Rational_Box known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(3*x >= -14);

  bool ok = check_result(box, known_result, "3.18e-7", "3.18e-7", "3.18e-7");

  print_constraints(box,
                    "*** box.bounded_affine_image("
                    "x, x + 4, x + 4, -1) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr has a
// maximum value (which is open) but the lb_expr has no minimum.
bool
test10() {
  Variable x(0);
  Variable y(1);

  TBox box(3);
  box.add_constraint(3*x < 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");
  box.bounded_affine_image(x, x + 4, x + 4);

  Rational_Box known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(3*x < 14);

  bool ok = check_result(box, known_result, "3.18e-7", "3.18e-7", "3.18e-7");

  print_constraints(box,
                    "*** box.bounded_affine_image(x, x + 4, x + 4) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr has a
// maximum value but the lb_expr has no minimum. Box is closed
// and the affine image has a denominator > 1.
bool
test11() {
  Variable x(0);
  Variable y(1);

  TBox box(3);
  box.add_constraint(x <= 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(y, 2*x, 2*x, 2);

  Rational_Box known_result(3);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.bounded_affine_image(y, 2*x, 2*x, 2) ***");

  return ok;
}

// Affine image variable is in both expressions, the ub_expr has a
// maximum value but the lb_expr has no minimum. Box is not closed
// and the affine image has a denominator > 1.
bool
test12() {
  Variable x(0);
  Variable y(1);

  TBox box(3);
  box.add_constraint(x < 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");

  box.bounded_affine_image(x, 2*x - 2, 2*x - 2, 2);

  Rational_Box known_result(3);
  known_result.add_constraint(x < 1);
  known_result.add_constraint(y <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.bounded_affine_image(x, 2*x-2, 2*x-2, 2) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(2);
  box.add_constraint(x >= 1);

  try {
    // This is an incorrect use of the method
    // Box::bounded_affine_image(v, lb_expr, ub_expr, d): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the box's space dimension.
    box.bounded_affine_image(y, x, z);
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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(2);
  box.add_constraint(x >= 1);

  try {
    // This is an incorrect use of the method
    // Box::bounded_affine_image(v, lb_expr, ub_expr, d): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the box's space dimension.
    box.bounded_affine_image(y, z, x);
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
  Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(x >= 1);

  try {
    // This is an incorrect use of the method
    // Box::bounded_affine_image(v, lb_expr, ub_expr, d): it is illegal
    // to apply it to a expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    box.bounded_affine_image(x, Linear_Expression(0), x + 1, d);
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
test16() {
  Variable x(0);
  Variable y(1);

  TBox box(1);
  box.add_constraint(x >= 1);

  try {
    // This is an invalid used of the method
    // Box::bounded_affine_image(v, lb_expr, ub_epxr, d): it is illegal to
    // apply the method to a variable that is not in the space of
    // the polyhedron.
    box.bounded_affine_image(y, x + 1, Linear_Expression(8));
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
test17() {
  Variable x(0);

  TBox box(1);

  print_constraints(box, "*** box ***");
  box.bounded_affine_image(x, 3*x, 3*x);

  Rational_Box known_result(1);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.bounded_affine_image(x, 3*x, 3*x) ***");

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
  DO_TEST_MAY_OVERFLOW_IF_INEXACT(test09, TBox);
  DO_TEST_MAY_OVERFLOW_IF_INEXACT(test10, TBox);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
END_MAIN
