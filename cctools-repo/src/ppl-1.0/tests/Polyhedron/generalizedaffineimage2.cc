/* Test Polyhedron::generalized_affine_image().
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

  NNC_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A + B));
  ph.add_generator(closure_point(2*A));
  ph.add_generator(closure_point(2*A + 2*B));
  ph.add_generator(closure_point(3*A + B));

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(B, LESS_THAN, B+2);

  NNC_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(A));
  known_result.add_generator(closure_point(A + 3*B));
  known_result.add_generator(closure_point(2*A + 4*B));
  known_result.add_generator(closure_point(3*A + 3*B));
  known_result.add_generator(ray(-B));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, LESS_THAN, B+2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(B <= 5);
  ph.add_constraint(A <= B);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron ph2 = ph;

  ph.generalized_affine_image(B, GREATER_OR_EQUAL, A+2);
  ph2.generalized_affine_image(-2*B, LESS_OR_EQUAL, -2*(A+2));

  bool ok = (ph == ph2);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, GREATER_OR_EQUAL,"
		   " A+2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(B <= 5);
  ph.add_constraint(A <= B);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron ph2 = ph;

  ph.generalized_affine_image(B, GREATER_OR_EQUAL, A+2, -2);
  ph2.generalized_affine_image(-2*B, LESS_OR_EQUAL, A+2);

  bool ok = (ph == ph2);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, GREATER_OR_EQUAL,"
		   " A+2, -2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(2*A <= 3);
  ph.add_constraint(7*A >= 2);
  ph.add_constraint(3*B >= 1);
  ph.add_constraint(2*A >= B);

  print_generators(ph, "*** ph ***");

  C_Polyhedron ph2 = ph;

  ph.generalized_affine_image(B, LESS_OR_EQUAL, A-B+2, -3);
  ph2.generalized_affine_image(-3*B, GREATER_OR_EQUAL, A-B+2);

  bool ok = (ph == ph2);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, LESS_OR_EQUAL,"
		   " A-B+2, -3) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A + B));
  ph.add_generator(closure_point(2*A));
  ph.add_generator(closure_point(2*A + 2*B));
  ph.add_generator(closure_point(3*A + B));

  print_constraints(ph, "*** ph ***");

  NNC_Polyhedron ph2 = ph;

  ph.generalized_affine_image(B, LESS_THAN, B+2);
  ph2.generalized_affine_image(-2*B, GREATER_THAN, -2*(B+2));

  bool ok = (ph == ph2);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, LESS_THAN, B+2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A - B >= 0);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron ph2 = ph;

  ph.generalized_affine_image(A, EQUAL, A + 2);
  ph2.generalized_affine_image(-2*A, EQUAL, -2*(A + 2));

  bool ok = (ph == ph2);

  print_generators(ph,
		   "*** after ph.generalized_affine_image"
		   "(A, EQUAL, A + 2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A + B));
  ph.add_generator(point(3*A + B));
  ph.add_generator(point(A + 3*B));
  ph.add_generator(point(3*A + 3*B));

  print_constraints(ph, "*** ph ***");

  C_Polyhedron ph2 = ph;

  ph.generalized_affine_image(A + B, GREATER_OR_EQUAL, 2*A - B + 2);
  ph2.generalized_affine_image(-3*(A + B),
			       LESS_OR_EQUAL,
			       -3*(2*A - B + 2));

  bool ok = (ph == ph2);

  print_generators(ph,
		   "*** after ph.generalized_affine_image"
		   "(A + B, GREATER_OR_EQUAL, 2*A - B + 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2);
  ph.add_constraint(A == 1);
  ph.add_constraint(B > 1);
  ph.add_constraint(B < 2);

  print_constraints(ph, "*** ph ***");
  print_generators(ph, "*** ph ***");

  ph.generalized_affine_image(B, GREATER_THAN, A + B + 1);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A == 1);
  known_result.add_constraint(B > 3);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(B, GREATER_THAN, A + B + 1) ***");
  print_generators(ph, "*** after ph.generalized_affine_image"
		   "(B, GREATER_THAN, A + B + 1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A >= 1);

  C_Polyhedron known_result(ph);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(Linear_Expression(2), EQUAL, A + B);

  known_result.add_constraint(A + B == 2);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(Linear_Expression(2), EQUAL, A + B) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A - B >= 1);

  C_Polyhedron known_result(ph);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(Linear_Expression(2),
			      GREATER_OR_EQUAL,
			      A + B);

  known_result.add_constraint(2 >= A + B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(Linear_Expression(2), GREATER_OR_EQUAL, A + B)"
		    "***");
  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A - B >= 1);

  C_Polyhedron known_result(ph);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(Linear_Expression(2), LESS_OR_EQUAL, A + B);

  known_result.add_constraint(2 <= A + B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(Linear_Expression(2), LESS_OR_EQUAL, A + B) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2);
  ph.add_constraint(B > 0);
  ph.add_constraint(A - B > 1);

  NNC_Polyhedron known_result(ph);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(Linear_Expression(2), GREATER_THAN, A + B);

  known_result.add_constraint(2 > A + B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(Linear_Expression(2), GREATER_THAN, A + B) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2);
  ph.add_constraint(B > 0);
  ph.add_constraint(A >= 1);

  NNC_Polyhedron known_result(ph);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(Linear_Expression(2), LESS_THAN, A + B);

  known_result.add_constraint(2 < A + B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(Linear_Expression(2), LESS_THAN, A + B) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(C == 0);
  ph.add_constraint(A + 3*B == 2);

  print_constraints(ph, "*** ph ***");
  print_generators(ph, "*** ph ***");

  ph.generalized_affine_image(A - C, EQUAL, B + 3);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A - C == B + 3);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ph.generalized_affine_image"
		    "(A - C, EQUAL, B + 3) ***");
  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - C, EQUAL, B + 3) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(C == 0);
  ph.add_constraint(A - 2*B >= 2);

  print_constraints(ph, "*** ph ***");
  print_generators(ph, "*** ph ***");

  ph.generalized_affine_image(A - C, GREATER_OR_EQUAL, B + 3);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A - B - C >= 3);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - C, GREATER_OR_EQUAL, B + 3) ***");
  print_generators(ph, "*** after ph.generalized_affine_image"
		    "(A - C, GREATER_OR_EQUAL, B + 3) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(C == 0);
  ph.add_constraint(A >= B);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(A - C, LESS_OR_EQUAL, B - 1);

  C_Polyhedron known_result(ph);
  known_result.add_constraint(A - B - C <= 1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - C, LESS_OR_EQUAL, B - 1) ***");
  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - C, LESS_OR_EQUAL, B - 1) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  NNC_Polyhedron ph(3);
  ph.add_constraint(A - C == 0);
  ph.add_constraint(A >= B);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(A - 2*C, LESS_THAN, B - 1);

  NNC_Polyhedron known_result(ph);
  known_result.add_constraint(A - B - 2*C < 1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - 2*C, LESS_THAN, B - 1) ***");
  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - 2*C, LESS_THAN, B - 1) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  NNC_Polyhedron ph(3);
  ph.add_constraint(A - 2*C == 0);
  ph.add_constraint(A > B - 2);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(A - 2*C + 3, GREATER_THAN, B - 1);

  NNC_Polyhedron known_result(ph);
  known_result.add_constraint(A - B - 2*C + 4 > 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - 2*C + 3, GREATER_THAN, B - 1) ***");
  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A - 2*C + 3, GREATER_THAN, B - 1) ***");

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 2);
  ph.add_constraint(B >= 0);
  ph.add_constraint(B <= 2);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(A + B, LESS_THAN, 2*A - 3*B);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A + B < 4);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.generalized_affine_image"
		    "(A + B, LESS_THAN, 2*A - 3*B) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST_F8(test04);
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
