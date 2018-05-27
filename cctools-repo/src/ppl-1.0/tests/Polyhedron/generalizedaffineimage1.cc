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

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(B <= 5);
  ph.add_constraint(A <= B);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(B, GREATER_OR_EQUAL, A+2);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(2*B));
  known_result.add_generator(point(4*A + 6*B));
  known_result.add_generator(ray(B));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, GREATER_OR_EQUAL, A+2)"
		   " ***");

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

  ph.generalized_affine_image(B, GREATER_OR_EQUAL, A+2, -2);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(-B));
  known_result.add_generator(point(4*A - 3*B));
  known_result.add_generator(ray(B));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, GREATER_OR_EQUAL,"
		   " A+2, -2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(2*A <= 3);
  ph.add_constraint(7*A >= 2);
  ph.add_constraint(3*B >= 1);
  ph.add_constraint(2*A >= B);

  print_generators(ph, "*** ph ***");

  ph.generalized_affine_image(B, LESS_OR_EQUAL, A-B+2, -3);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(9*A - B, 6));
  known_result.add_generator(point(2*A - 4*B, 7));
  known_result.add_generator(ray(-B));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(B, LESS_OR_EQUAL,"
		   " A-B+2, -3) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A - B >= 0);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  ph.generalized_affine_image(A, EQUAL, A + 2);

  known_result.affine_image(A, A + 2);

  bool ok = (ph == known_result);

  print_generators(ph,
		   "*** after ph.generalized_affine_image"
		   "(A, EQUAL, A + 2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A + B));
  ph.add_generator(point(3*A + B));
  ph.add_generator(point(A + 3*B));
  ph.add_generator(point(3*A + 3*B));

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(A + B >= 1);

  ph.generalized_affine_image(A + B, GREATER_OR_EQUAL, 2*A - B + 2);

  bool ok = (ph == known_result);

  print_generators(ph,
		   "*** after ph.generalized_affine_image"
		   "(A + B, GREATER_OR_EQUAL, 2*A - B + 2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  ph1.generalized_affine_image(A, LESS_OR_EQUAL, B + 1);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.generalized_affine_image"
		    "(A, LESS_OR_EQUAL, B + 1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  ph1.generalized_affine_image(A + B, GREATER_OR_EQUAL, A + B + 1);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.generalized_affine_image"
		    "(A + B, GREATER_OR_EQUAL, A + B + 1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= -2);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(A, GREATER_OR_EQUAL, A+2);

  C_Polyhedron known_result(1, EMPTY);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image(A, GREATER_OR_EQUAL, A+2)"
		   " ***");

  return ok;
}

bool
test09() {
  Variable A(0);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= -2);

  print_constraints(ph, "*** ph ***");

  ph.generalized_affine_image(A+1, GREATER_OR_EQUAL, A+2);

  C_Polyhedron known_result(1, EMPTY);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_image"
                   "(A+1, GREATER_OR_EQUAL, A+2)"
		   " ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST_F8(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
