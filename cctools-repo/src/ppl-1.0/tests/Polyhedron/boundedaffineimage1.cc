/* Test Polyhedron::bounded_affine_image().
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
  ph.add_constraint(B >= 0);
  ph.add_constraint(B <= 4);
  ph.add_constraint(A - B <= 2);
  ph.add_constraint(A - B >= -2);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron kr1 = ph;
  C_Polyhedron kr2 = ph;

  ph.bounded_affine_image(A, 7-B, B+3);

  kr1.generalized_affine_image(A, GREATER_OR_EQUAL, 7-B);
  kr2.generalized_affine_image(A, LESS_OR_EQUAL, B+3);
  kr1.intersection_assign(kr2);

  bool ok = (ph == kr1);

  print_generators(ph,
		   "*** ph.bounded_affine_image(A, 7-B, B+3) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(B >= 0);
  ph.add_constraint(B <= 4);
  ph.add_constraint(A - B <= 2);
  ph.add_constraint(A - B >= -2);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron kr1 = ph;
  C_Polyhedron kr2 = ph;

  ph.bounded_affine_image(A, 7-3*A+2*B, B+5*A-3);

  kr1.generalized_affine_image(A, GREATER_OR_EQUAL, 7-3*A+2*B);
  kr2.generalized_affine_image(A, LESS_OR_EQUAL, B+5*A-3);
  kr1.intersection_assign(kr2);

  bool ok = (ph == kr1);

  print_generators(ph,
		   "*** ph.bounded_affine_image(A, 7-3*A+2*B, B+5*A-3) ***");
  print_generators(kr1,
		   "*** kr1.bounded_affine_image(A, 7-3*A+2*B, B+5*A-3) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(A == B);

  print_constraints(ph, "*** ph ***");

  ph.bounded_affine_image(A, A+5, B);

  C_Polyhedron kr1(2, EMPTY);

  bool ok = (ph == kr1);

  print_generators(ph, "*** ph.bounded_affine_image(A, A+5, B) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);

  ph.add_constraint(x <= 1);
  ph.add_constraint(x >= 0);
  ph.add_constraint(y <= 2);
  ph.add_constraint(y >= -1);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron kr1(ph);
  C_Polyhedron kr2(ph);

  ph.bounded_affine_image(x, -2*x + y + 1, -2*x + y + 1, -1);

  kr1.generalized_affine_image(x, GREATER_OR_EQUAL, -2*x + y + 1, -1);
  kr2.generalized_affine_image(x, LESS_OR_EQUAL, -2*x + y + 1, -1);
  kr1.intersection_assign(kr2);

  bool ok = (kr1 == ph);

  print_constraints(ph,
		    "*** ph.bounded_affine_image("
		    "x, -2*x+y+1, -2*x+y+1, -1) ***");
  print_constraints(kr1,
		    "*** kr1 ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);

  ph.add_constraint(x <= 1);
  ph.add_constraint(x >= 0);
  ph.add_constraint(y <= 2);
  ph.add_constraint(y >= -1);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron kr1(ph);
  C_Polyhedron kr2(ph);

  ph.bounded_affine_image(x, 2*x + y + 1, 2*x + y + 1, -1);

  kr1.generalized_affine_image(x, GREATER_OR_EQUAL, 2*x + y + 1, -1);
  kr2.generalized_affine_image(x, LESS_OR_EQUAL, 2*x + y + 1, -1);
  kr1.intersection_assign(kr2);

  bool ok = (kr1 == ph);

  print_constraints(ph,
		    "*** ph.bounded_affine_image("
		    "x, 2*x+y+1, 2*x+y+1, -1) ***");
  print_constraints(kr1,
		    "*** kr1 ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);

  ph.add_constraint(x <= 1);
  ph.add_constraint(x >= 0);
  ph.add_constraint(y <= 2);
  ph.add_constraint(y >= -1);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron kr1(ph);
  C_Polyhedron kr2(ph);

  ph.bounded_affine_image(x, -2*x + y + 1, -2*x + y + 1);

  kr1.generalized_affine_image(x, GREATER_OR_EQUAL, -2*x + y + 1);
  kr2.generalized_affine_image(x, LESS_OR_EQUAL, -2*x + y + 1);
  kr1.intersection_assign(kr2);

  bool ok = (kr1 == ph);

  print_constraints(ph,
		    "*** ph.bounded_affine_image("
		    "x, -2*x+y+1, -2*x+y+1) ***");
  print_constraints(kr1,
		    "*** kr1 ***");

  return ok;
}

bool
test07() {
  Variable A(0);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 1);
  ph.add_constraint(A <= 0);

  print_constraints(ph, "*** ph ***");

  ph.bounded_affine_image(A, A, 2*A, 3);

  C_Polyhedron kr1(1, EMPTY);

  bool ok = (ph == kr1);

  print_generators(ph, "*** ph.bounded_affine_image(A, A, 2*A, 3) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
END_MAIN
