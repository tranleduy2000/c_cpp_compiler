/* Test Octagonal_Shape::affine_image().
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

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(x, y);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x - y == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, y) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(x, -y);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x + y == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, -y) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(x, x + 4);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x - y <= 7);
  known_result.add_constraint(x <= 6);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, x + 4) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(x, Linear_Expression(4));

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x == 4);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, 4) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  Octagonal_Shape<mpq_class> known_result(oc1);

  oc1.affine_image(x, x);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, x) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(x, 2*x - 2, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(x - y <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, 2*x -2, 2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(y, 2*x, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y - x == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(y, 2*x, 2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(y, 3*x + 3, 3);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y - x == 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(y, 3*x+ 3, 3) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + B >= 2);

   print_constraints(oc, "*** oc ***");

  oc.affine_image(A, 2*A +2, 2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 3);
  known_result.add_constraint(B >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, 2*A + 2, 2) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x - y >= 0);
  oct.add_constraint(x >= 0);
  oct.add_constraint(x <= 2);

  Linear_Expression coeff1 = x + 1;

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::affine_image(v, e, d): it is illegal to apply
    // the method to a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    oct.affine_image(y, coeff1, d);
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
test11() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x + y >= 2);

  try {
    // This is an invalid use of method
    // Octagonal_Shape::affine_image(v, expr, d): it is illegal to
    // apply this method to a variable that is not in the space of
    // the polyhedron.
    oct.affine_image(y, z - 2);
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
test12() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x - y >= 0);
  oct.add_constraint(x >= 0);
  oct.add_constraint(x <= 2);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::affine_image(v, expr, d): it is illegal to
    // use a variable in the expression that does not appear in the
    // space of the polyhedron.
    oct.affine_image(x, y - z + 1);
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
test13() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x - y >= 0);
  oct.add_constraint(x >= 0);
  oct.add_constraint(x <= 2);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::affine_image(v, expr, d): it is illegal to
    // apply this method to a variable that is not in the space
    // of the polyhedron.
    oct.affine_image(z, y - x + 1);
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
test14() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B <= 0);
  oc.add_constraint(B - A >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, 2*A +2, 2);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, 2*A + 2, 2) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A <= 3);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(B - A >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -2*A + 3, 2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A <= -1);
  known_result.add_constraint(2*A >= -3);
  known_result.add_constraint(B >= 4);
  known_result.add_constraint(2*A + 2*B >= 7);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, -2*A + 3, 2) ***");

  return ok;
}

bool
test16() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(x <= 2);
  oc.add_constraint(x - y <= 3);
  oc.add_constraint(y <= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(y, 2*x - 4, -2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x + y == 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc1.affine_image(y, 2*x - 4, -2) ***");

  return ok;
}

bool
test17() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(x <= 2);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  oct1.affine_image(y, y + 6);

  TOctagonal_Shape known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 6);
  known_result.add_constraint(y <= 8);

  bool ok = (oct1 == known_result);

  print_constraints(oct1, "*** oct1.affine_image(y, y + 6) ***");

  return ok;
}

bool
test18() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(x <= 2);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  oct1.affine_image(y, -y + 6);

  TOctagonal_Shape known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 4);
  known_result.add_constraint(y <= 6);

  bool ok = (oct1 == known_result);

  print_constraints(oct1, "*** oct1.affine_image(y, -y + 6) ***");

  return ok;
}

bool
test19() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x + y == 0);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_image(x, -x);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x - y == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_image(x, y) ***");

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
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
END_MAIN
