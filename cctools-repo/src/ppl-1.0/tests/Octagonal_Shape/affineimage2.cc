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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C >= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + B >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, C + B, 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 1);
  known_result.add_constraint(B - A <= -1);
  known_result.add_constraint(C - A <= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, C + B, 1) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C <= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + B >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, C + B, 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(A - B <= 1);
  known_result.add_constraint(A - C >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, C + B, 1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C <= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + B >= 2);
  oc.add_constraint(A >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -A, 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(A <= -2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, -A, 1) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C <= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + B <= 2);
  oc.add_constraint(-A + B <= 1);
  oc.add_constraint(A >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -A, 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(A <= -2);
  known_result.add_constraint(-A + B <= 2);
  known_result.add_constraint(A + B <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, -A, 1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oc(4);
  oc.add_constraint(C <= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + C <= 3);
  oc.add_constraint(A <= 2);
  oc.add_constraint(A >= 1);
  oc.add_constraint(D >= 1);
  oc.add_constraint(D <= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -A + 2*D, 1);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(D >= 1);
  known_result.add_constraint(D <= 2);
  known_result.add_constraint(A <= 3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A - D <= 1);
  known_result.add_constraint(D - A <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, -A + 2*D, 1) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C <= 1);
  oc.add_constraint(B <= 0);
  oc.add_constraint(C >= -1);
  oc.add_constraint(B >= -2);
  oc.add_constraint(A + B <= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -C - B, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(2*A >= -1);
  known_result.add_constraint(2*A <= 3);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(C >= -1);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(2*(B + A) >= -3);
  known_result.add_constraint(2*(C + A) >= -1);
  known_result.add_constraint(2*(B + A) <= 1);
  known_result.add_constraint(2*(C + A) <= 3);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, -C - B, 2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(B <= 0);
  oc.add_constraint(B >= -2);
  oc.add_constraint(A + B <= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -B, 2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(B + A >= -1);
  known_result.add_constraint(B + A <= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_image(A, -B, 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(B <= 0);
  oc.add_constraint(B >= -2);
  oc.add_constraint(A + B <= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, B + 3, 2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A >= 1);
  known_result.add_constraint(2*A <= 3);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(2*(A - B) >= 3);
  known_result.add_constraint(2*(A - B) <= 5);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, B + 3, 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(B <= 0);
  oc.add_constraint(B >= -2);
  oc.add_constraint(A + B <= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, B + 1, -2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A >= -1);
  known_result.add_constraint(2*A <= 1);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(2*(B + A) >= -3);
  known_result.add_constraint(2*(B + A) <= -1);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, B + 1, -2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(B <= 0);
  oc.add_constraint(B >= -2);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(A, -B + 1, -2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A >= -3);
  known_result.add_constraint(2*A <= -1);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(2*(A - B) <= 1);
  known_result.add_constraint(2*(B - A) <= 1);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, -B + 1, -2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= 0);
  oc.add_constraint(C <= 10);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, -A + C + 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(C <= 10);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(-A - B <= -1);
  known_result.add_constraint(A + B <= 11);
  known_result.add_constraint(B - C >= 1);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(B, -A + C +1) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 0);
  oc.add_constraint(C <= 10);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, -A + C + 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 11);
  known_result.add_constraint(C <= 10);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(-A - B <= -1);
  known_result.add_constraint(A + B <= 11);
  known_result.add_constraint(B - C <= 1);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(B, -A + C + 1) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A - B >= 0);
  oc.add_constraint(C - B <= 10);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, A + C + 1);
  print_constraints(oc, "*** oc ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C - A <= 10);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(B, -A + C + 1) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 0);
  oc.add_constraint(C <= 10);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, A + C + 1);
  print_constraints(oc, "*** oc ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(C <= 10);
  known_result.add_constraint(C - A <= 10);
  known_result.add_constraint(B - A <= 11);
  known_result.add_constraint(B - C >= 1);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(B, A + C + 1) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 0);
  oc.add_constraint(C <= 10);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, A + C + 1, -1);
  print_constraints(oc, "*** oc ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(C <= 10);
  known_result.add_constraint(C - A <= 10);
  known_result.add_constraint(A + B >= -11);
  known_result.add_constraint(B + C <= -1);


  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(B, A + C + 1, -1) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C >= 1);
  oc.add_constraint(A <= 10);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, A + C + 1);
  print_constraints(oc, "*** oc ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C >= 1);
  known_result.add_constraint(A <= 10);
  known_result.add_constraint(B - A >= 2);
  known_result.add_constraint(B - C <= 11);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, B + C + 1) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(C >= 1);
  oc.add_constraint(A <= 10);

  print_constraints(oc, "*** oc ***");

  oc.affine_image(B, A + C + 1, -1);
  print_constraints(oc, "*** oc ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C >= 1);
  known_result.add_constraint(A <= 10);
  known_result.add_constraint(B + A <= -2);
  known_result.add_constraint(B + C >= -11);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_image(A, B + C + 1, -1) ***");

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
END_MAIN
