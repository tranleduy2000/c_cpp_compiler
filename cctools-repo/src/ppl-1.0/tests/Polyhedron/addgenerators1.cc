/* Test Polyhedron::add_generators().
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

  Generator_System gs1;
  gs1.insert(point(x));
  gs1.insert(point(y));
  gs1.insert(line(x));
  C_Polyhedron ph1(gs1);

  print_generators(ph1, "*** before ***");

  Generator_System gs2;
  gs2.insert(point());

  ph1.add_generators(gs2);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(point(y));
  known_result.add_generator(line(x));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** add_generators ***");

  return ok;
}

bool
test02() {
  Variable x(0);

  C_Polyhedron ph1(2, EMPTY);

  print_generators(ph1, "*** before ***");

  Generator_System gs;
  gs.insert(point());
  gs.insert(line(x));

  ph1.add_generators(gs);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(line(x));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** add_generators ***");

  return ok;
}

bool
test03() {
  C_Polyhedron ph;
  ph.add_generator(point());

  print_generators(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(point());

  print_generators(gs, "*** gs ***");

  ph.add_generators(gs);

  C_Polyhedron known_result;

  bool ok = (ph == known_result);

  print_generators(ph, "*** after add_generators ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(x >= 1);
  ph.add_constraint(x <= 0);

  print_constraints(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(ray(x));
  gs.insert(point());

  print_generators(gs, "*** gs ***");

  ph.add_generators(gs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(y == 0);
  known_result.add_constraint(x >= 0);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after add_generators ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(x >= 0);
  ph.add_constraint(x <= -1);

  print_constraints(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(ray(x + y));
  gs.insert(point());

  print_generators(gs, "*** gs ***");

  ph.add_generators(gs);

  Generator_System gs_known_result;
  gs_known_result.insert(point());
  gs_known_result.insert(ray(x + y));
  C_Polyhedron known_result(gs_known_result);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after add_generators ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  Generator_System gs1;
  gs1.insert(ray(x + y));
  gs1.insert(point());

  C_Polyhedron ph(gs1);

  print_generators(ph, "*** ph ***");

  Generator_System gs2;
  gs2.insert(ray(x));
  gs2.insert(point());

  print_generators(gs2, "*** gs2 ***");

  ph.add_generators(gs2);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(ray(x));
  known_result.add_generator(ray(x + y));

  bool ok = (ph == known_result);

  print_generators(ph, "*** after add_generators ***");

  return ok;
}

bool
test07() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 3);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  Generator_System gs;
  ph.add_generators(gs);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ph .add_generators(gs) ***");

  return ok;
}

bool
test08() {
  C_Polyhedron ph(0, EMPTY);

  print_generators(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(point());

  print_generators(gs, "*** gs ***");

  ph.add_generators(gs);

  C_Polyhedron known_result;

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ph.add_generators(gs); ***");

  return ok;
}

bool
test09() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  C_Polyhedron copy_ph(ph);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-A));
  Generator_System gs2(gs1);

  ph.add_generators(gs1);
  copy_ph.add_generators(gs2);

  bool ok = (ph == copy_ph);

  print_generators(ph, "*** after ph.add_generators(gs1) ***");
  print_generators(copy_ph, "*** after copy_ph.add_generators(gs2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point());
  ph1.constraints();
  ph1.add_generator(line(A + B));
  C_Polyhedron copy_ph1 = ph1;

  C_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point());
  ph2.constraints();
  ph2.add_generator(ray(A));
  ph2.add_generator(ray(B));

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  Generator_System gs1 = ph2.generators();
  Generator_System gs2 = ph2.generators();

  ph1.add_generators(gs1);
  copy_ph1.add_generators(gs2);

  bool ok = (ph1 == copy_ph1);

  print_generators(ph1, "*** after add_generators ***");
  print_generators(copy_ph1, "*** after add_generators ***");

  return ok;
}

bool
test11() {
  C_Polyhedron ph(1, EMPTY);
  Generator_System gs(point());

  print_generators(gs, "*** gs ***");

  ph.add_generators(gs);

  C_Polyhedron known_result(1, EMPTY);
  known_result.add_generator(point());

  bool ok = (ph == known_result);

  print_generators(ph, "*** add_generators ***");

  return ok;
}

bool
test12() {
  C_Polyhedron ph(0, EMPTY);
  print_constraints(ph, "*** ph ***");

  Generator_System gs(point());
  print_generators(gs, "*** gs ***");

  C_Polyhedron known_result(0, UNIVERSE);

  ph.add_generators(gs);

  bool ok = (ph == known_result);
  print_constraints(ph, "*** ph.add_generators(gs) ***");

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
END_MAIN
