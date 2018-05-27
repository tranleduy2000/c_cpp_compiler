/* Test Polyhedron::affine_image().
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

  C_Polyhedron ph(3);
  ph.add_constraint(C == -2);
  ph.add_constraint(A == 0);

  print_constraints(ph, "*** ph ***");

  ph.affine_image(B, A+2, 1);

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point(2*B - 2*C));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_image(B, A+2, 1) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= B);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A <= 3);

  print_constraints(ph, "*** ph ***");

  ph.affine_image(A, A+B+1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A -2*B - 1 >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - B <= 4);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph after ph.affine_image(A, A+B+1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(ray(B));
  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.affine_image(A, A + 1, 2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(2*A >= 1);
  known_result.add_constraint(B >= 0);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ph.affine_image(A, A + 1, 2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A));

  print_constraints(ph, "*** ph ***");

  ph.affine_image(A, B+2, -3);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(-2*A, 3));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_image(A, B+2, -3) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 2);
  ph.add_constraint(A <= 3);
  ph.add_constraint(B >= 1);
  ph.add_constraint(2*A >= B);

  print_constraints(ph, "*** ph ***");

  ph.affine_image(B, A-B+2, -3);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(2*A));
  known_result.add_generator(point(2*A - B));
  known_result.add_generator(point(9*A + B, 3));
  known_result.add_generator(point(9*A - 4*B, 3));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_image(B, A-B+2, -3) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  ph1.affine_image(A, 2*A + B + 1);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.affine_image(A, 2*A + B + 1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A));
  gs.insert(point(B));
  gs.insert(point(A + B));
  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.affine_image(A, -A - 1, -1);

  Generator_System known_gs;
  known_gs.insert(point(A));
  known_gs.insert(point(2*A));
  known_gs.insert(point(A + B));
  known_gs.insert(point(2*A + B));
  C_Polyhedron known_result(known_gs);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ph.affine_image(A, -A - 1, -1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  C_Polyhedron copy_ph(ph);

  print_constraints(ph, "*** ph ***");

  ph.affine_image(A, A + 1);
  copy_ph.affine_image(A, -A - 1, -1);

  bool ok = (ph == copy_ph);

  print_generators(ph, "*** after ph.affine_image(A, A + 1) ***");
  print_generators(copy_ph,
		   "*** after copy_ph.affine_image(A, -A - 1, -1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  C_Polyhedron copy_ph(ph);

  print_constraints(ph, "*** ph ***");

  ph.affine_image(B, A + 1);
  copy_ph.affine_image(B, -A - 1, -1);

  bool ok = (ph == copy_ph);

  print_generators(ph, "*** after ph.affine_image(B, A + 1) ***");
  print_generators(copy_ph,
		   "*** after copy_ph.affine_image(B, -A - 1, -1) ***");

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
END_MAIN
