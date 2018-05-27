/* Test Octagonal_Shape::generalized_affine_image().
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
  Linear_Expression e1(x + 2);
  Linear_Expression e2(x - 2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= 4);
  oct.add_constraint(x <= -6);
  oct.add_constraint(y == 0);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.generalized_affine_image(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(x + 2, "
                         "LESS_OR_EQUAL, x - 2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Linear_Expression e1(6);
  Linear_Expression e2(x - 4);

  TOctagonal_Shape oct(2, EMPTY);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);

  oct.generalized_affine_image(e1, EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(6, "
                    "EQUAL, x-4) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(B);
  Linear_Expression e2(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B >= 0);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 1);

  oct.generalized_affine_image(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(B, "
                         "LESS_OR_EQUAL, 1) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(A + 5);
  Linear_Expression e2(4);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A == 0);
  oct.add_constraint(B >= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);
  known_result.affine_image(A, Linear_Expression(-1));

  oct.generalized_affine_image(e1, EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(A+5, "
                    "EQUAL, 4) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(2);
  Linear_Expression e2(B);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A + B == 0);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= -1);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(A + B == 0);

  oct.generalized_affine_image(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(2, "
                         "GREATER_OR_EQUAL, B) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(C + 1);
  Linear_Expression e2(A - 3);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A - B == 0);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C + A <= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A - B == 0);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(C <= -3);
  known_result.add_constraint(A + B <= 2);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(A - C >= 4);

  oct.generalized_affine_image(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(C + 1, "
                         "LESS_OR_EQUAL, A - 3) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(C + 1);
  Linear_Expression e2(A + 2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A - B == 0);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C + A <= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);
  known_result.affine_image(C, A + 1);

  oct.generalized_affine_image(e1, EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(C+1, EQUAL, A+2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(B - 1);
  Linear_Expression e2(A - 2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A - B == 0);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C + A <= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B - A >= -1);
  known_result.add_constraint(C + A <= 2);
  known_result.add_constraint(A <= 1);

  oct.generalized_affine_image(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(B - 1, "
                         "GREATER_OR_EQUAL, A - 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(-C + 3);
  Linear_Expression e2(-B + 4);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A - B == 0);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C + A <= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C >= B - 1);
  known_result.add_constraint(A - B == 0);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(C - A >= -1);
  known_result.add_constraint(A <= 1);

  oct.generalized_affine_image(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(-C + 3, "
                         "LESS_OR_EQUAL, -B + 4) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression e1(-C + 3);
  Linear_Expression e2(-B + 4);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A - B == 0);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C + A <=2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);
  known_result.add_constraint(C <= B - 1);
  known_result.add_constraint(A - B == 0);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(C + A <= 1);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B + A <= 2);

  oct.generalized_affine_image(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(-C + 3, "
                         "GREATER_OR_EQUAL, -B + 4) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Linear_Expression e1(2);
  Linear_Expression e2(B);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A + B == 0);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= -1);
  known_result.add_constraint(B >= 2);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(A + B == 0);

  oct.generalized_affine_image(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct, "*** oct.generalized_affine_image(2, "
                         "LESS_OR_EQUAL, B) ***");

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
END_MAIN
