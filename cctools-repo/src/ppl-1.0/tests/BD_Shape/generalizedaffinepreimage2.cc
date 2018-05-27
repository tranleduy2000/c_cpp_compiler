/* Test BD_Shape::generalized_affine_preimage().
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
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(2*A == 1);
  bds.add_constraint(B >= 5);
  bds.add_constraint(3*C <= 7);
  bds.add_constraint(5*C >= 7);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(B, EQUAL, 3*A+2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(2*A == 1);
  known_result.add_constraint(3*C <= 7);
  known_result.add_constraint(5*C >= 7);

  bool ok = check_result(bds, known_result, "3.66e-7", "2.28e-7", "1.59e-7");

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B, "
                    "EQUAL, -1) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(2*A == 1);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(B, GREATER_OR_EQUAL, Linear_Expression(-1));

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A == 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, -1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(2*A == 1);
  bds.add_constraint(B <= 5);
  bds.add_constraint(3*C <= 8);
  bds.add_constraint(2*C >= 7);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(B, EQUAL, 3*A+2);

  BD_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = check_result(bds,  known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B, "
                    "EQUAL, 3*A+2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(B - A <= 2);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(A, LESS_OR_EQUAL, B, 5);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B <= 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A, "
                    "LESS_OR_EQUAL, B, 5) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(B - A <= 2);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(A, GREATER_OR_EQUAL, B + 3);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A, "
                    "GREATER_OR_EQUAL, B + 3) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(A);
  Linear_Expression e2(B + 3);

  TBD_Shape bds(2);
  bds.add_constraint(B - A <= 2);
  bds.add_constraint(B <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A, "
                    "GREATER_OR_EQUAL, B + 3) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(A - B);
  Linear_Expression e2(B + C + 2);

  TBD_Shape bds(3);
  bds.add_constraint(B - A <= 2);
  bds.add_constraint(C <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A - B, "
                    "GREATER_OR_EQUAL, B + C + 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(A - B);
  Linear_Expression e2(2);

  TBD_Shape bds(3);
  bds.add_constraint(B - A <= 2);
  bds.add_constraint(C <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A - B, "
                    "GREATER_OR_EQUAL, 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(A - B);
  Linear_Expression e2(C + 2);

  TBD_Shape bds(3);
  bds.add_constraint(B - A <= 2);
  bds.add_constraint(C <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A - B, "
                    "GREATER_OR_EQUAL, C + 2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(0);
  Linear_Expression e2(1);

  TBD_Shape bds(3);
  bds.add_constraint(A <= 2);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(C <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(0, "
                    "GREATER_OR_EQUAL, 1) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(B + C);
  Linear_Expression e2(3);

  TBD_Shape bds(3);
  bds.add_constraint(A <= 2);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(C <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B + C, "
                    "GREATER_OR_EQUAL, 3) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(B + C);
  Linear_Expression e2(B + C);

  TBD_Shape bds(3);
  bds.add_constraint(A <= 2);
  bds.add_constraint(B <= 2);
  bds.add_constraint(B - C <= 3);
  bds.add_constraint(C <= 5);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B + C, "
                    "GREATER_OR_EQUAL, B + C) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(B - A);
  Linear_Expression e2(A);

  TBD_Shape bds(3);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A >= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B - A, "
                    "GREATER_OR_EQUAL, A) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(B - A);
  Linear_Expression e2(2*A);

  TBD_Shape bds(3);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A >= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(B - A, "
                    "EQUAL, 2*A) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(A - B);
  Linear_Expression e2(B - C);

  TBD_Shape bds(3);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A >= 2);
  bds.add_constraint(C >= -2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C >= -2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A - B, "
                    "LESS_OR_EQUAL, B - C) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(C);
  Linear_Expression e2(A + B);

  TBD_Shape bds(3);
  bds.add_constraint(B <= 5);
  bds.add_constraint(A <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 5);
  known_result.add_constraint(A <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(C, "
                    "LESS_OR_EQUAL, A + B) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(C);
  Linear_Expression e2(A + 2*B + C);

  TBD_Shape bds(3);
  bds.add_constraint(B <= 1);
  bds.add_constraint(A <= 2);
  bds.add_constraint(C <= 2);
  bds.add_constraint(A - B <= 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, EQUAL, e2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(A - B <= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(C, "
                    "EQUAL, A + 2*B + C) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(A - B);
  Linear_Expression e2(10);

  TBD_Shape bds(2);
  bds.add_constraint(B <= 1);
  bds.add_constraint(A >= 2);
  bds.add_constraint(A - B >= 11);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(A - B, "
                    "LESS_OR_EQUAL, 10) ***");

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Linear_Expression e1(2*A - 2*C);
  Linear_Expression e2(6);

  TBD_Shape bds(4);
  bds.add_constraint(D - A <= 1);
  bds.add_constraint(C - B <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(D - B <= 6);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(2*A - 2*C, "
                    "LESS_OR_EQUAL, 6) ***");

  return ok;
}

bool
test20() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Linear_Expression e1(4*A - 2*B + 3*C);
  Linear_Expression e2(1 + C);

  TBD_Shape bds(4);
  bds.add_constraint(D - A <= 1);
  bds.add_constraint(C - B <= 2);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, EQUAL, e2);

  BD_Shape<mpq_class> known_result(4);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(4*A - 2*B + 3*C, "
                    "EQUAL, 1 + C) ***");

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
