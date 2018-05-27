/* Test Polyhedron::add_space_dimensions_and_project()
   and  Polyhedron::add_space_dimensions_and_embed().
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
  Variable z(2);
  Variable w(3);

  Constraint_System cs;
  cs.insert(x > 2);
  cs.insert(y > 2);
  cs.insert(x < 6);
  cs.insert(y < 6);

  NNC_Polyhedron ph(cs);

  ph.generators();

  print_constraints(ph, "*** ph ***");
  print_generators(ph, "*** ph ***");

  ph.add_space_dimensions_and_project(2);

  NNC_Polyhedron known_result(4);
  known_result.add_constraint(z == 0);
  known_result.add_constraint(w == 0);
  known_result.add_constraint(x > 2);
  known_result.add_constraint(y > 2);
  known_result.add_constraint(x < 6);
  known_result.add_constraint(y < 6);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after add_space_dimensions_and_project ***");
  print_generators(ph, "*** after add_space_dimensions_and_project ***");

  return ok;
}

bool
test02() {
  Variable x(0);

  NNC_Polyhedron ph1(1);

  ph1.add_constraint(x > 1);
  ph1.add_constraint(x < 5);

  print_constraints(ph1, "*** ph1 ***");

  ph1.add_space_dimensions_and_embed(2);

  Generator_System gs;
  gs.insert(point(2*x));
  gs.insert(closure_point(x));
  gs.insert(closure_point(5*x));

  NNC_Polyhedron ph2(gs);

  print_generators(ph2, "*** ph2 ***");

  ph2.add_space_dimensions_and_embed(2);

  bool ok = (ph1 == ph2);

  print_constraints(ph1, "*** ph1 after add_space_dimensions_and_embed ***");
  print_generators(ph2, "*** ph2 after add_space_dimensions_and_embed ***");

  return ok;
}

bool
test03() {
  Variable x(0);

  NNC_Polyhedron ph1(1);

  ph1.add_constraint(x > -3);
  ph1.add_constraint(x < 3);

  print_constraints(ph1, "*** ph1 ***");

  ph1.add_space_dimensions_and_project(2);

  Generator_System gs;
  gs.insert(point());
  gs.insert(closure_point(-3*x));
  gs.insert(closure_point(3*x));

  NNC_Polyhedron ph2(gs);

  print_generators(ph2, "*** ph2 ***");

  ph2.add_space_dimensions_and_project(2);

  bool ok = (ph1 == ph2);

  print_constraints(ph1, "*** ph1 after add_space_dimensions_and_embed ***");
  print_generators(ph2, "*** ph2 after add_space_dimensions_and_embed ***");

  return ok;
}

bool
test04() {
  Variable A(0);

  NNC_Polyhedron ph(1, EMPTY);
  ph.add_generator(point(A));
  ph.add_generator(closure_point());
  ph.add_generator(closure_point(3*A));

  print_generators(ph, "*** ph ***");

  ph.add_space_dimensions_and_embed(1);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A > 0);
  known_result.add_constraint(A < 3);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.add_space_dimensions_and_embed(1) ***");
  print_generators(ph, "*** after ph.add_space_dimensions_and_embed(1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point(A));
  NNC_Polyhedron ph(gs1);

  Generator_System gs2;
  gs2.insert(point(A));
  gs2.insert(ray(A));
  ph.add_generators(gs2);

  print_generators(ph, "*** ph ***");

  ph.add_space_dimensions_and_embed(1);

  Generator_System known_gs;
  known_gs.insert(point(A));
  known_gs.insert(line(B));
  known_gs.insert(ray(A));
  NNC_Polyhedron known_result(known_gs);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point(A + B));
  gs1.insert(closure_point());
  gs1.insert(ray(A));
  gs1.insert(ray(B));
  NNC_Polyhedron ph1(gs1);

  ph1.minimized_constraints();

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.add_space_dimensions_and_embed(1);

  NNC_Polyhedron known_result(3);
  known_result.add_constraint(A > 0);
  known_result.add_constraint(B > 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1,
		    "*** after ph1.add_space_dimensions_and_embed(1) ***");
  print_generators(ph1, "*** after ph1.add_space_dimensions_and_embed(1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);

  // This will change the size of the rows, but not their capacity.
  ph1.add_space_dimensions_and_embed(1);

  // Assigning the polyhedron to itself:
  // this used to recompute the row capacity based on row size,
  // without actually increasing the capacity of the rows,
  // leading to an inconsistent state.
  ph1 = ph1;

  return ph1.OK();
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
END_MAIN
