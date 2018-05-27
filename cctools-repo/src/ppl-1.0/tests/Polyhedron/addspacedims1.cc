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

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(x));
  gs.insert(point(y));
  gs.insert(point(x + y));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.add_space_dimensions_and_project(1);

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(point(x));
  known_result.add_generator(point(y));
  known_result.add_generator(point(x + y));

  bool ok = (ph == known_result);

  print_generators(ph, "*** after add_space_dimensions_and_project ***");

  return ok;
}

bool
test02() {
  C_Polyhedron ph(3, EMPTY);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron computed_result1(ph);
  C_Polyhedron computed_result2(ph);

  computed_result1.add_space_dimensions_and_project(4);
  computed_result2.add_space_dimensions_and_embed(4);

  C_Polyhedron known_result(7, EMPTY);

  bool ok = (computed_result1 == known_result
	     && computed_result2 == known_result);

  print_constraints(computed_result1, "*** computed_result1 ***");
  print_constraints(computed_result2, "*** computed_result2 ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable u(3);
  Variable v(4);
  Variable w(5);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(x + y));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  Constraint_System cs = ph.constraints();

  ph.add_space_dimensions_and_embed(2);

  print_generators(ph, "*** after add_space_dimensions_and_embed(2) ***");

  ph.add_space_dimensions_and_embed(2);

  C_Polyhedron known_result(6, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(ray(x + y));
  known_result.add_generator(line(z));
  known_result.add_generator(line(u));
  known_result.add_generator(line(v));
  known_result.add_generator(line(w));

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph ***");

  return ok;
}

bool
test04() {
  C_Polyhedron ph1;

  print_generators(ph1, "*** ph1 ***");

  ph1.add_space_dimensions_and_project(3);

  print_generators(ph1, "*** after add_space_dimensions_and_project(3) ***");

  C_Polyhedron ph2;
  Constraint_System cs = ph2.constraints();

  print_generators(ph2, "*** ph2 ***");

  ph2.add_space_dimensions_and_project(3);

  bool ok = (ph1 == ph2);

  print_generators(ph2, "*** after add_space_dimensions_and_project(3) ***");

  return ok;
}

bool
test05() {
  Variable C(2);

  C_Polyhedron ph(2);

  print_constraints(ph, "*** ph ***");

  ph.add_space_dimensions_and_project(1);

  C_Polyhedron known_result(3);
  known_result.add_constraint(C == 0);

  bool ok = (ph == known_result);

  print_constraints(ph,
		    "*** after ph.add_space_dimensions_and_project(1) ***");

  return ok;
}

bool
test06() {
  Variable A(0);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);

  C_Polyhedron ph2(ph1);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.add_space_dimensions_and_embed(0);
  ph2.add_space_dimensions_and_project(0);

  bool ok = (ph1 == ph2);

  print_constraints(ph1,
		    "*** after ph1.add_space_dimensions_and_embed(0) ***");
  print_constraints(ph2,
		    "*** after ph2.add_space_dimensions_and_project(0) ***");

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
END_MAIN
