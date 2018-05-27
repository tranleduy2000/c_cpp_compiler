/* Test Polyhedron::refine_with_constraints().
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
  ph.refine_with_constraint(A >= 0);
  ph.refine_with_constraint(B >= 0);

  Constraint_System cs;

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  C_Polyhedron known_result(ph);

  ph.refine_with_constraints(cs);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs1;
  cs1.insert(A + B >= 0);
  C_Polyhedron ph(cs1);

  print_constraints(ph, "*** ph ***");

  Linear_Expression e(1);
  Constraint_System cs2;
  cs2.insert(e == 0);
  ph.refine_with_constraints(cs2);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.refine_with_constraints(cs2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(3);
  ph.refine_with_constraint(A >= 1);

  print_constraints(ph, "*** ph ***");

  Constraint_System cs;
  cs.insert(A >= 0);

  C_Polyhedron computed_result(ph);

  computed_result.add_constraints(cs);

  bool ok = (computed_result == ph);

  print_constraints(computed_result,
		    "*** after refine_with_constraints ***");

  return ok;
}

bool
test04() {
  Variable A(0);

  C_Polyhedron ph(3, EMPTY);

  Constraint_System cs;
  cs.insert(A >= 4);

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.refine_with_constraints(cs);

  C_Polyhedron computed_result(3, EMPTY);

  bool ok = (ph == computed_result);

  print_constraints(ph, "*** after refine_with_constraints(cs) ***");

  return ok;
}

bool
test05() {
  C_Polyhedron ph;
  ph.refine_with_constraint(Linear_Expression(-2) >= 0);

  Constraint_System cs;
  cs.insert(Linear_Expression(-1) >= 0);

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.refine_with_constraints(cs);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** after refine_with_constraints ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(ray(A + B));

  C_Polyhedron ph(gs);

  Constraint_System cs;
  cs.insert(A < 3);

  print_generators(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.refine_with_constraints(cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - B >= 0);
  known_result.add_constraint(A <= 3);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** after refine_with_constraints ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);

  Constraint_System cs;
  cs.insert(A - B == 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");

  ph1.refine_with_constraints(cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A - B == 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.refine_with_constraints(cs) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);

  Constraint_System cs;
  cs.insert(A - B > 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");

  ph1.refine_with_constraints(cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A - B >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.refine_with_constraints(cs) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);

  Constraint_System cs;
  cs.insert(A - B > 1);
  cs.insert(A + B >= 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");

  ph1.refine_with_constraints(cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A - B >= 1);
  known_result.add_constraint(A + B >= 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.refine_with_constraints(cs) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);

  Constraint_System cs;
  cs.insert(A - B > 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");

  ph1.refine_with_constraints(cs);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A - B > 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.refine_with_constraints(cs) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);


  NNC_Polyhedron ph1(2);
  Constraint_System cs;
  cs.insert(A - B > 1);
  cs.insert(A + B >= 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");

  ph1.refine_with_constraints(cs);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A - B > 1);
  known_result.add_constraint(A + B >= 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.refine_with_constraints(cs) ***");

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
