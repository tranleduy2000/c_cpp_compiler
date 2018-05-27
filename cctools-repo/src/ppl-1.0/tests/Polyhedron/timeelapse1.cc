/* Test Polyhedron::time_elapse_assign().
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

  C_Polyhedron ph1(2);
  ph1.add_constraint(x >= 1);
  ph1.add_constraint(x <= 3);
  ph1.add_constraint(y >= 1);
  ph1.add_constraint(y <= 3);

  C_Polyhedron ph2(2);
  ph2.add_constraint(y == 5);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(y));
  known_result.add_generator(ray(y));
  known_result.add_generator(line(x));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** ph1_time_elapse_assign(ph2) ***");

  return ok;
}

bool
test02() {
  C_Polyhedron ph1(0, EMPTY);
  C_Polyhedron ph2;

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  C_Polyhedron ph3(2, EMPTY);
  C_Polyhedron ph4(2);

  print_constraints(ph3, "*** ph3 ***");
  print_constraints(ph4, "*** ph4 ***");

  ph3.time_elapse_assign(ph4);

  C_Polyhedron ph5(2);
  C_Polyhedron ph6(2, EMPTY);

  print_constraints(ph5, "*** ph5 ***");
  print_constraints(ph6, "*** ph6 ***");

  ph5.time_elapse_assign(ph6);

  bool ok = ph1.is_empty() && ph3.is_empty() && ph5.is_empty();

  print_generators(ph1, "*** ph1_time_elapse_assign(ph2) ***");
  print_generators(ph3, "*** ph3_time_elapse_assign(ph4) ***");
  print_generators(ph5, "*** ph5_time_elapse_assign(ph6) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point(A));
  gs1.insert(point(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point(0*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  ph1.time_elapse_assign(ph2);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** ph1 ***");

  return ok;
}

bool
test04() {
  C_Polyhedron ph1;
  C_Polyhedron ph2(0, EMPTY);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.time_elapse_assign(ph2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.generators();
  ph1.add_constraint(A == 0);
  ph1.add_constraint(B == 0);

  C_Polyhedron ph2(2);
  ph2.generators();
  ph2.add_constraint(A == 2);
  ph2.add_constraint(B == 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  C_Polyhedron ph3(2, EMPTY);
  ph3.add_generator(point());
  ph3.constraints();

  C_Polyhedron ph4(2, EMPTY);
  ph4.add_generator(point(2*A + 2*B));

  print_generators(ph3, "*** ph3 ***");
  print_generators(ph4, "*** ph4 ***");

  ph3.time_elapse_assign(ph4);

  bool ok = (ph1 ==  ph3);

  print_generators(ph1, "*** after ph1.time_elapse_assign(ph2) ***");
  print_generators(ph3, "*** after ph3.time_elapse_assign(ph4) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2);

  ph1.add_constraint(x >= 0);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(x + y - 2 <= 0);

  C_Polyhedron ph2(2);

  ph2.add_constraint(x == -1);
  ph2.add_constraint(y == -1);

  Pointset_Powerset<C_Polyhedron> ps1(2, EMPTY);
  ps1.add_disjunct(ph1);
  ps1.add_disjunct(ph2);

  C_Polyhedron ph3(2);

  ph3.add_constraint(x >= 2);
  ph3.add_constraint(x <= 4);
  ph3.add_constraint(y == 3);

  Pointset_Powerset<C_Polyhedron> ps2(2, EMPTY);
  ps2.add_disjunct(ph3);

  print_constraints(ps1, "*** ps1 ***");
  print_constraints(ps2, "*** ps2 ***");

  ps1.time_elapse_assign(ps2);

  Generator_System known_gs;
  known_gs.insert(point());
  known_gs.insert(point(2*x));
  known_gs.insert(point(2*y));
  known_gs.insert(ray(2*x + 3*y));
  known_gs.insert(ray(4*x + 3*y));

  C_Polyhedron known_ph1(known_gs);

  known_gs.clear();
  known_gs.insert(point(-x - y));
  known_gs.insert(ray(2*x + 3*y));
  known_gs.insert(ray(4*x + 3*y));
  C_Polyhedron known_ph2(known_gs);

  Pointset_Powerset<C_Polyhedron> known_result(2, EMPTY);
  // Inserting out of order on purpose.
  known_result.add_disjunct(known_ph2);
  known_result.add_disjunct(known_ph1);

  bool ok = (ps1 == known_result);

  print_constraints(ps1, "*** ps1.time_elapse_assign(ps2) ***");

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
