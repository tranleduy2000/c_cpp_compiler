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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 21);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C <= 2);
  oct.add_constraint(A >= 2);
  oct.add_constraint(B >= -1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 21);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(C >= -2);
  known_result.add_constraint(4*B <= 5);
  known_result.add_constraint(4*(A + B) <= 51);
  known_result.add_constraint(4*(C + B) <= 1);

  oct.generalized_affine_image(B, LESS_OR_EQUAL, -2*A - 3*C + 3, 4);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "LESS_OR_EQUAL, -2*A - 3*C + 3, 4) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 21);
  oct.add_constraint(B <= 1);
  oct.add_constraint(A >= 2);
  oct.add_constraint(B >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 21);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(B - C <= 12);

  oct.generalized_affine_image(B, LESS_OR_EQUAL, A + 2*C + 3, 2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "LESS_OR_EQUAL, A + 2*C + 3, 2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C <= 2);
  oct.add_constraint(B >= -1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(C >= -2);
  known_result.add_constraint(4*(B - A) <= 9);

  oct.generalized_affine_image(B, LESS_OR_EQUAL, 4*A - 3*C + 3, 4);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "LESS_OR_EQUAL, 4*A - 3*C + 3, 4) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 21);
  oct.add_constraint(B <= 1);
  oct.add_constraint(A >= 2);
  oct.add_constraint(B >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 21);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(2*(B + C) <= -5);

  oct.generalized_affine_image(B, LESS_OR_EQUAL, A + 2*C + 3, -2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "LESS_OR_EQUAL, A + 2*C + 3, -2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C <= 2);
  oct.add_constraint(B >= -1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(C >= -2);
  known_result.add_constraint(4*(B + A) <= 9);

  oct.generalized_affine_image(B, LESS_OR_EQUAL, -4*A - 3*C + 3, 4);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "LESS_OR_EQUAL, -4*A - 3*C + 3, 4) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C <= 2);
  oct.add_constraint(B >= -1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(C >= -2);
  known_result.add_constraint(4*(B - A) >= -3);

  oct.generalized_affine_image(B, GREATER_OR_EQUAL, 4*A - 3*C + 3, 4);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "GREATER_OR_EQUAL, 4*A - 3*C + 3, 4) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 21);
  oct.add_constraint(B <= 1);
  oct.add_constraint(A >= 2);
  oct.add_constraint(B >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 21);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(2*(B - C) >= 5);

  oct.generalized_affine_image(B, GREATER_OR_EQUAL, A + 2*C + 3, 2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "GREATER_OR_EQUAL, A + 2*C + 3, 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 21);
  oct.add_constraint(B <= 1);
  oct.add_constraint(B >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 21);

  oct.generalized_affine_image(B, GREATER_OR_EQUAL, A + 2*C + 3, 2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "GREATER_OR_EQUAL, A + 2*C + 3, 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C <= 2);
  oct.add_constraint(B >= -1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(C >= -2);
  known_result.add_constraint(4*(B + A) >= -9);

  oct.generalized_affine_image(B, GREATER_OR_EQUAL, 4*A - 3*C + 3, -4);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "GREATER_OR_EQUAL, 4*A - 3*C + 3, -4) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 21);
  oct.add_constraint(B <= 1);
  oct.add_constraint(A >= 2);
  oct.add_constraint(B >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 21);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(2*(B + C) >= 5);

  oct.generalized_affine_image(B, GREATER_OR_EQUAL, A - 2*C + 3, 2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_image(B, "
                    "GREATER_OR_EQUAL, A - 2*C + 3, 2) ***");

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
END_MAIN
