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
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A <= B);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(B, GREATER_OR_EQUAL, A+2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B - A >= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image"
                        "(B, GREATER_OR_EQUAL, A+2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(B >= 0);
  bds.add_constraint(A - B >= 0);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.generalized_affine_image(A, EQUAL, A + 2);

  known_result.affine_image(A, A + 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image"
                    "(A, EQUAL, A + 2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2, EMPTY);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(A, LESS_OR_EQUAL, B + 1);

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image"
                    "(A, LESS_OR_EQUAL, B + 1) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);

  bds.add_constraint(x >= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(x, GREATER_OR_EQUAL, 2*x - 2, 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x >= 1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(- y <= 1);
  known_result.add_constraint(x - y >= -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(x, "
                        "GREATER_OR_EQUAL, 2*x - 2, 2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(3);
  bds.add_constraint(x >= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(y, GREATER_OR_EQUAL, 2*x - 2, 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x >= 2);
  known_result.add_constraint(x <= 5);
  known_result.add_constraint(y >= x - 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(y, "
                        "GREATER_OR_EQUAL, 2*x - 2, 2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(A);
  Linear_Expression e2(A);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A <= B);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(A, "
                        "GREATER_OR_EQUAL, A) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A - B <= 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(A, GREATER_OR_EQUAL, Linear_Expression(1));

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(A, "
                        "GREATER_OR_EQUAL, 1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A - B <= 0);
  bds.add_constraint(B - C == 2);
  bds.add_constraint(C - A <= -2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(-B, LESS_OR_EQUAL, Linear_Expression(1));

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(A - C == 2);
  known_result.add_constraint(C >= -2);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(B >= -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(-B, "
                        "LESS_OR_EQUAL, 1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(C);
  Linear_Expression e2(A + 1);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);
  bds.add_constraint(C <= 2);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(e1, LESS_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B <= 5);
  known_result.add_constraint(C - A <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(C, "
                        "LESS_OR_EQUAL, A + 1) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(A, LESS_OR_EQUAL, Linear_Expression(1));

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(A, "
                        "LESS_OR_EQUAL, 1) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x == 4);
  bds.add_constraint(y <= 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(3*x + 2, LESS_OR_EQUAL, 2*x - 3);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y <= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_image(3*x + 2, "
                    "LESS_OR_EQUAL, 2*x - 3) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x - y <= 7);
  bds.add_constraint(y >= 1);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.generalized_affine_image(Linear_Expression(6), EQUAL, 3*x - 4);

  known_result.add_constraint(3*x == 10);

  bool ok = check_result(bds, known_result, "3.98e-7", "2.39e-7", "1.59e-7");

  print_constraints(bds,
                    "*** bds.generalized_affine_image(6, EQUAL, 3*x - 4) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(2*B + 3*A,
                               LESS_OR_EQUAL, Linear_Expression(1));

  BD_Shape<mpq_class> known_result(2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(3*A + 2*B, "
                         "LESS_OR_EQUAL, 1) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A == 0);
  bds.add_constraint(B >= 1);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.generalized_affine_image(-2*A + 5, EQUAL, -4*B);

  known_result.affine_image(A, 4*B + 5, 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_image(-2*A + 5, "
                    "EQUAL, -4*B) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(A + 2*B - 5, GREATER_OR_EQUAL, 3*B);

  BD_Shape<mpq_class> known_result(2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(A + 2*B - 5, "
                        "GREATER_OR_EQUAL, 3*B) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(2*B + C + 1, LESS_OR_EQUAL, A - 3*B + 2*C);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_image(2*B + C + 1, "
                    "LESS_OR_EQUAL, A - 3*B + 2*C) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(2*B + C + 1, GREATER_OR_EQUAL, A - 3*B + 2*C);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(2*B + C + 1, "
                        "GREATER_OR_EQUAL, A - 3*B + 2*C) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(-2*A - B - 1,
                              GREATER_OR_EQUAL, 3*A + B + 4*C - 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(-2*A - B - 1, "
                        "GREATER_OR_EQUAL, 3*A + B + 4*C - 2) ***");

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);
  bds.add_constraint(C - A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(-2*C + 3, LESS_OR_EQUAL, -3*B + 4);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A - B == 0);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(A <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.generalized_affine_image(-2*C + 3, "
                        "LESS_OR_EQUAL, -3*B + 4) ***");

  return ok;
}

bool
test20() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(B <= 1);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_image(Linear_Expression(3),
                               GREATER_OR_EQUAL,
                               Linear_Expression(4));

  BD_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_image(3, "
                    "GREATER_OR_EQUAL, 4) ***");

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
  DO_TEST(test20);
END_MAIN
