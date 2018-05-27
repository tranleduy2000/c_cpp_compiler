/* Test the construction of a polyhedron using generators.
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
  gs.insert(point(x + y, -1));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  Constraint_System cs = ph.constraints();

  C_Polyhedron known_result(2);
  known_result.add_constraint(x == -1);
  known_result.add_constraint(y == -1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph constraints ***");
  print_generators(ph, "*** ph generators ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x + y >= 2);
  cs.insert(x + y <= 1);

  C_Polyhedron known_result(cs);

  const Generator_System& gs = known_result.generators();

  print_generators(gs, "*** gs ***");

  C_Polyhedron ph(gs);

  bool ok = (gs.space_dimension() == known_result.space_dimension()
	     && gs.begin() == gs.end()
	     && ph == known_result);

  print_generators(ph, "*** ph ***");

  return ok;
}

bool
test03() {
  Variable A(0);

  C_Polyhedron ph1(2, EMPTY);

  C_Polyhedron ph2(0);

  C_Polyhedron ph3(2);
  ph3.add_constraint(A >= 1);
  ph3.add_constraint(A <= -1);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
  print_generators(ph3, "*** ph3 ***");

  Generator_System gs1 = ph1.generators();
  Generator_System gs2 = ph2.generators();
  Generator_System gs3 = ph3.generators();

  Generator_System known_result2;
  known_result2.insert(point());

  bool ok = (gs1.begin() == gs1.end()
	     && C_Polyhedron(gs2) == C_Polyhedron(known_result2)
	     && gs3.begin() == gs3.end());

  print_generators(gs1, "*** gs1 ***");
  print_generators(gs2, "*** gs2 ***");
  print_generators(gs3, "*** gs3 ***");

  return ok;
}

bool
test04() {
  const Generator_System gs;

  C_Polyhedron ph(gs);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph ***");

  return ok;
}

bool
test05() {
  Generator_System gs1;
  gs1.insert(point());

  C_Polyhedron ph1(gs1);
  C_Polyhedron known_result(ph1);

  print_generators(ph1, "*** ph1 ***");

  const Generator_System gs2 = ph1.generators();

  C_Polyhedron ph2(gs2);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** ph2 ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.generators();
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(B >= 0);

  C_Polyhedron known_result(ph1);

  Generator_System gs = ph1.generators();
  C_Polyhedron ph2(gs);

  bool ok = (ph2 == known_result);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
  print_generators(gs, "*** gs ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point());
  ph1.constraints();
  ph1.add_generator(ray(A));
  ph1.add_generator(ray(B));

  C_Polyhedron known_result(ph1);

  Generator_System gs = ph1.generators();
  C_Polyhedron ph2(gs);

  bool ok = (ph2 == known_result);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
  print_generators(gs, "*** gs ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point());
  ph.add_generator(ray(A));
  ph.add_generator(ray(B));

  print_generators(ph, "*** ph ***");

  Generator_System gs = ph.generators();
  gs.insert(point(A));
  gs.insert(point(B));

  print_generators(gs, "*** gs ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  return C_Polyhedron(gs) == known_result;
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
END_MAIN
