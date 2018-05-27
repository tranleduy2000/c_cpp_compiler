/* Test BD_Shape::generalized_affine_image().
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

  TBD_Shape bds(2);
  bds.add_constraint(x <= 4);
  bds.add_constraint(x >= -6);
  bds.add_constraint(y == 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(y, LESS_OR_EQUAL, -y + 1);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 4);
  known_result.add_constraint(x >= -6);
  known_result.add_constraint(y <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(y, "
                        "LESS_OR_EQUAL, -y + 1) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 4);
  bds.add_constraint(x >= -6);
  bds.add_constraint(y == 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(x, GREATER_OR_EQUAL, -x - 3);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x >= -7);
  known_result.add_constraint(y == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(x, "
                        "GREATER_OR_EQUAL, -x - 3) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B <= 1);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(B, LESS_OR_EQUAL, 3*B + 1, 2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(B, "
                        "LESS_OR_EQUAL, 3*B + 1, 2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A == 0);
  bds.add_constraint(B >= 1);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(B, GREATER_OR_EQUAL, B - 2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B >= -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(B, "
                        "GREATER_OR_EQUAL, B - 2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(B <= 1);
  bds.add_constraint(A - B == 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(A, GREATER_OR_EQUAL, 2*A + 3, 2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(2*B - 2*A <= -3);

  bool ok = check_result(bds, known_result, "7.63e-17", "3.82e-17", "1.91e-17");

  print_constraints(bds, "*** bds.generalized_affine_image(A, "
                        "GREATER_OR_EQUAL, 2*A + 3, 2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(C, LESS_OR_EQUAL, 2*C + 1, 5);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A - B == 0);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(5*C <= 7);
  known_result.add_constraint(A <= 1);

  bool ok = check_result(bds, known_result, "9.54e-8", "9.54e-8", "9.54e-8");

  print_constraints(bds, "*** bds.generalized_affine_image(C, "
                        "LESS_OR_EQUAL, 2*C + 1, 5) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.generalized_affine_image(C, EQUAL, 5*C - 3, 4);

  known_result.affine_image(C, 5*C - 3, 4);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(C, "
                        "EQUAL, 5*C - 3, 4) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(B, GREATER_OR_EQUAL, -B - 2, 3);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= -1);
  known_result.add_constraint(C - A <= 2);
  known_result.add_constraint(A <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(B, "
                        "GREATER_OR_EQUAL, -B - 2, 3) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(B, LESS_OR_EQUAL, 4*A -2*C + 3, -3);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(C - A <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(B, "
                        "LESS_OR_EQUAL, 4*A - 2*C + 3, -3) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <=2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.generalized_affine_image(B, EQUAL, 2*A - 4*B + C + 3, 3);

  known_result.affine_image(B, 2*A - 4*B + C + 3, 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(B, "
                        "EQUAL, 2*A - 4*B + C + 3, 3) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(A);
  Linear_Expression e2(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(e1, EQUAL, e2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A == 1);
  known_result.add_constraint(B <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(A, EQUAL, 1) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(B - 3);
  Linear_Expression e2(B + 1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(e1, EQUAL, e2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B <= 9);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_image(B-3, EQUAL, B+1) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_image(v, r, expr, d): it is illegal
    // to use a strict relation symbol.
    bds.generalized_affine_image(x, LESS_THAN, x + 1);
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

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_image(v, r, expr, d): it is illegal
    // to use a strict relation symbol.
    bds.generalized_affine_image(x, GREATER_THAN, x + 1);
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
  bds.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_image(v, r, expr, d): it is illegal
    // to apply it to a expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    bds.generalized_affine_image(x, LESS_OR_EQUAL, x + 1, d);
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
  Variable z(2);

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_image(v, r, expr, d): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the BDS's space dimension.
    bds.generalized_affine_image(y, GREATER_OR_EQUAL, z);
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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_image(lhs, r, rhs):
    // it is illegal to use a variable in the `rhs' expression that
    // does not appear in the BDS.

    bds.generalized_affine_image(A + B, GREATER_OR_EQUAL, B + C);
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
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // BD_Shape::generalized_affine_image(lhs, r, rhs):
    // it is illegal to use a variable in the `lhs' expression that
    // does not appear in the BDS.
    bds.generalized_affine_image(B + C, LESS_OR_EQUAL, A + 1);
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
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
END_MAIN
