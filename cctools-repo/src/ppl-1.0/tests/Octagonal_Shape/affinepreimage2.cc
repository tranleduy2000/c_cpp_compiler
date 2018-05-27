/* Test Octagonal_Shape::affine_preimage().
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
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, B + C - 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(B + C >= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, B + C - 2) ***");

 return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, -B + C - 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(B - C <= -2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, -B + C - 2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, A - C + 3);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= -1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(C - A <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, A - C + 3) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, -A - C + 3);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(A + C <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, -A - C + 3) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, B - C + 3);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, B - C + 3) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, 2*B - C - 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(2*B >= 1);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(oc, known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, 2*B - C - 1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, 2*B - 2*C - 2, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(C - B <= -1);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, 2*B - 2*C - 2, 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, 2*B - 2*C - 2, -2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(C - B >= -1);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, 2*B - 2*C - 2, -2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B <= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(C, 4*B + 6*C + 2, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(6*C >= -2);

  bool ok = check_result(oc, known_result, "9.54e-8", "9.54e-8", "9.54e-8");

  print_constraints(oc, "*** oc.affine_preimage(C, 4*B + 6*C + 2, 2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B <= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(C, 4*B + 6*C + 2, -2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B <= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(C, 4*B + 6*C + 2, -2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B <= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, 4*B + 6*C + 2, -2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, 4*B + 6*C + 2, -2) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B <= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, -2*A + 3*C + 2, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(C - A <= -2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, -2*A + 3*C + 2, 2) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);
  oc.add_constraint(B <= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, 3*A + C - 1, 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(3*A <= -1);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);
  bool ok = check_result(oc, known_result, "9.54e-8", "9.54e-8", "9.54e-8");

  print_constraints(oc, "*** oc.affine_preimage(A, 3*A + C - 1, 2) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oc(4);
  oc.add_constraint(A == 2);
  oc.add_constraint(B == 0);
  oc.add_constraint(C >= -3);
  oc.add_constraint(D <= 5);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(D, 4*A - B + 2*C + 5*D - 1, 3);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A == 2);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C >= -3);
  known_result.add_constraint(5*D <= 14);
  bool ok = check_result(oc, known_result, "1.98e-6", "9.26e-7", "5.25e-7");

  print_constraints(oc,
                    "*** oc.affine_preimage(D, 4*A - B + 2*C + 5*D - 1, 3) ***");

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
END_MAIN
