/* Test Polyhedron::affine_preimage().
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

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A + B));
  gs.insert(point(A));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.affine_preimage(A, A+2);

  Generator_System gs_known_result;
  gs_known_result.insert(point(-2*A));
  gs_known_result.insert(ray(A + B));
  gs_known_result.insert(point(-A));

  C_Polyhedron known_result(gs_known_result);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_preimage(A, A+2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(A + C == 0);
  ph.add_constraint(A + B >= 0);
  ph.add_constraint(A + B - 1 >= 0);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(A, A + B);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A + B + C == 0);
  known_result.add_constraint(A + 2*B >= 0);
  known_result.add_constraint(A + 2*B -1 >= 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph after ph.affine_preimage(A, A+B) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A + B - 3 >= 0);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(A, B+1);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(B));
  known_result.add_generator(line(A));
  known_result.add_generator(ray(B));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_preimage(A, B+2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Generator_System gs;
  gs.insert(point(x + y));
  gs.insert(ray(x + 2*y));
  gs.insert(ray(x));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.affine_preimage(x, y+1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(y >= 1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after affine_preimage(x, y+1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 2);
  ph.add_constraint(B >= 0);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(A, A + 1, 2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 3);
  known_result.add_constraint(B >= 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.affine_preimage(A, A + 1, 2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 2);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(B, A + 1, 2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 3);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.affine_preimage(B, A + 1, 2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(ray(A + B));
  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.affine_preimage(B, A - B, -1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A - B <= 0);
  known_result.add_constraint(2*A - B >= 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.affine_preimage(B, A - B, -1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(A, 2*A +B + 1);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.affine_preimage(A, 2*A + B + 1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A <= 1);
  ph.add_constraint(B <= 1);

  ph.affine_preimage(A, -A - 1, -1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(A >= -1);
  known_result.add_constraint(B >= 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.affine_preimage(A, -A - 1, -1) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A <= 1);
  ph.add_constraint(B <= 1);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(B, -A - 1, -1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A == 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.affine_preimage(B, -A - 1, -1) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  C_Polyhedron copy_ph(ph);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(A, A + 1);
  copy_ph.affine_preimage(A, -A - 1, -1);

  bool ok = (ph == copy_ph);

  print_generators(ph, "*** after ph.affine_preimage(A, A + 1) ***");
  print_generators(copy_ph,
		   "*** after copy_ph.affine_preimage(A, -A - 1, -1) ***");
  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);
  C_Polyhedron copy_ph(ph);

  print_constraints(ph, "*** ph ***");

  ph.affine_preimage(B, A + 1);
  copy_ph.affine_preimage(B, -A - 1, -1);

  bool ok = (ph == copy_ph);

  print_generators(ph, "*** after ph.affine_preimage(B, A + 1) ***");
  print_generators(copy_ph,
		   "*** after copy_ph.affine_preimage(B, -A - 1, -1) ***");
  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A + B));
  gs.insert(point(A));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.affine_preimage(B, Linear_Expression(1));

  Generator_System gs_known_result;
  gs_known_result.insert(point(A));
  gs_known_result.insert(point(2*A));
  gs_known_result.insert(line(B));

  C_Polyhedron known_result(gs_known_result);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_preimage(B, 1) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(0*A + 3*B));
  gs.insert(point(3*A + 0*B));
  gs.insert(point(3*A + 3*B));

  C_Polyhedron ph(gs);

  C_Polyhedron known_result = ph;

  print_generators(ph, "*** ph before ph.affine_image(A, A + 2*B + 4) ***");

  ph.affine_image(A, A + 2*B + 4);

  print_generators(ph, "*** ph after ph.affine_image(A, A + 2*B + 4) ***");

  ph.affine_preimage(A, A + 2*B + 4);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_preimage(A, A + 2*B + 4) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(0*A + 3*B));
  gs.insert(point(3*A + 0*B));
  gs.insert(point(3*A + 3*B));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph before ph.affine_image(A, B) ***");

  ph.affine_image(A, B);

  print_generators(ph, "*** ph after ph.affine_image(A, B) ***");

  ph.affine_preimage(A, B);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 3);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after ph.affine_preimage(A, B) ***");

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
END_MAIN
