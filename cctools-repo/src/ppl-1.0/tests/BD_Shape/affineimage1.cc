/* Test BD_Shape::affine_image().
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

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, y);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x - y == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, y) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, x + 4);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x - y <= 7);
  known_result.add_constraint(x <= 6);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, x + 4) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, Linear_Expression(4));

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x == 4);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, 4) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.affine_image(x, x);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, x) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, 2*x - 2, 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x - y <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, 2*x - 2, 2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(y, 2*x, 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y - x == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(y, 2*x, 2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(y, 3*x + 3, 3);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y - x == 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(y, 3*x + 3, 3) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(x >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -2*x - 3*y + 1, -5);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(5*x >= -4);
  known_result.add_constraint(5*x <= 7);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);
  known_result.add_constraint(y - x <= 1);
  known_result.add_constraint(5*x - 5*y <= 3);

  bool ok = check_result(bds, known_result, "3.70e-7", "2.10e-7", "1.44e-7");

  print_constraints(bds, "*** bds.affine_image(x, -2*x - 3*y + 1, -5) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);
  bds.add_constraint(z >= 3);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(z, x + 2*y -3*z + 2, 4);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(2*z <= -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(z, x + 2*y -3*z + 2, 4) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(4);
  bds.add_constraint(A <= 1);
  bds.add_constraint(B <= 2);
  bds.add_constraint(B >= 1);
  bds.add_constraint(C <= 0);
  bds.add_constraint(D == 3);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(A, -B + 2*C + 1, -3);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(C <= 0);
  known_result.add_constraint(D == 3);
  known_result.add_constraint(3*B - 3*A <= 5);

  bool ok = check_result(bds, known_result, "7.95e-8", "7.95e-8", "7.95e-8");

  print_constraints(bds, "*** bds.affine_image(A, -B + 2*C + 1, -3) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, 2*y + z + 2, 4);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, 2*y + z + 2, 4) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x - y >= 0);
  bds.add_constraint(x >= 0);
  bds.add_constraint(x <= 2);

  Linear_Expression coeff1 = x + 1;

  try {
    // This is an incorrect use of method
    // BD_Shape::affine_image(v, expr, d): it is illegal to apply
    // the method to a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    bds.affine_image(x, coeff1, d);
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
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(1);
  bds.add_constraint(x >= 1);

  try {
    // This is an invalid used of the method
    // BD_Shape::affine_image(v, expr, d): it is illegal to
    // apply this method to a variable that is not in the space of
    // the polyhedron.
    bds.affine_image(y, x + 1);
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

  TBD_Shape bds(2);
  bds.add_constraint(x >= 1);
  bds.add_constraint(y >= 1);

  try {
    // This is an invalid used of the method
    // BD_Shape::affine_image(v, expr, d): it is illegal to
    // use a variable in the expression that does not appear in the
    // space of the polyhedron.
    bds.affine_image(y, x + z + 1);
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

  TBD_Shape bds(2);
  bds.add_constraint(x - y >= 0);
  bds.add_constraint(x >= 0);
  bds.add_constraint(x <= 2);

  Linear_Expression coeff1 = 2*x + 1;

  try {
    // This is an incorrect use of method
    // BD_Shape::affine_image(v, expr, d): it is illegal to apply
    // the method to a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    bds.affine_image(x, coeff1, d);
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
