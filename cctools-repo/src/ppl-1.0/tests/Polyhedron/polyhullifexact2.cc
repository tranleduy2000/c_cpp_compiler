/* Test NNC_Polyhedron::upper_bound_assign_if_exact().
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

  NNC_Polyhedron ph1(1, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 1);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(1, UNIVERSE);
  ph2.add_constraint(x == 0);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(1, UNIVERSE);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 1);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 2);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y == 1);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x <= 2);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);
  ph1.add_constraint(x + y > 0);
  ph1.add_constraint(x + y < 4);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x == y);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y >= 0);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, UNIVERSE);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x - y >= 0);
  ph1.add_constraint(x + y >= 0);
  ph1.add_constraint(4*x < 1);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x - y >= 0);
  ph2.add_constraint(x + y >= 0);
  ph2.add_constraint(4*x > 1);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(y > 0);
  ph1.add_constraint(x < 2);
  ph1.add_constraint(y < 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x == 2);
  ph2.add_constraint(y == 1);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(y > 0);
  ph1.add_constraint(x < 2);
  ph1.add_constraint(y < 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x == 2);
  ph2.add_constraint(y > 0);
  ph2.add_constraint(y < 2);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, UNIVERSE);
  known_result.add_constraint(x > 0);
  known_result.add_constraint(y > 0);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y < 2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x < 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x == 2);
  ph2.add_constraint(y == 0);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x < 1);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x > 1);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y >= 0);
  ph2.add_constraint(y <= 2);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(0*x + 0*y));
  ph1.add_generator(point(0*x + 1*y));
  ph1.add_generator(closure_point(2*x + 2*y));
  ph1.add_generator(closure_point(3*x + 0*y));

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point(4*x + 0*y));
  ph2.add_generator(point(4*x + 1*y));
  ph2.add_generator(closure_point(2*x + 2*y));
  ph2.add_generator(closure_point(1*x + 0*y));

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);
  known_result.add_generator(point(0*x + 0*y));
  known_result.add_generator(point(0*x + 1*y));
  known_result.add_generator(point(4*x + 0*y));
  known_result.add_generator(point(4*x + 1*y));
  known_result.add_generator(closure_point(2*x + 2*y));

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x < 1);
  ph1.add_constraint(y > 0);
  ph1.add_constraint(y < 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x > 1);
  ph2.add_constraint(x < 2);
  ph2.add_constraint(y > 0);
  ph2.add_constraint(y < 2);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x < 1);
  ph1.add_constraint(y > 0);
  ph1.add_constraint(y < 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 1);
  ph2.add_constraint(x < 2);
  ph2.add_constraint(y > 0);
  ph2.add_constraint(y < 2);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, UNIVERSE);
  known_result.add_constraint(x > 0);
  known_result.add_constraint(x < 2);
  known_result.add_constraint(y > 0);
  known_result.add_constraint(y < 2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  NNC_Polyhedron ph1(3, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 1);
  ph1.add_constraint(y > 0);
  ph1.add_constraint(y < 2);
  ph1.add_constraint(z > 0);
  ph1.add_constraint(z < 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(3, UNIVERSE);
  ph2.add_constraint(x >= 1);
  ph2.add_constraint(x < 2);
  ph2.add_constraint(y > 0);
  ph2.add_constraint(y < 2);
  ph2.add_constraint(z > 0);
  ph2.add_constraint(z < 2);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(3, UNIVERSE);
  known_result.add_constraint(x > 0);
  known_result.add_constraint(x < 2);
  known_result.add_constraint(y > 0);
  known_result.add_constraint(y < 2);
  known_result.add_constraint(z > 0);
  known_result.add_constraint(z < 2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  NNC_Polyhedron ph1(3, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 1);
  ph1.add_constraint(y > 0);
  ph1.add_constraint(y < 2);
  ph1.add_constraint(z > 0);
  ph1.add_constraint(z <= 2);
  ph1.add_constraint(x + z < 3);

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph1.minimized_generators(), "*** ph1 ***");

  NNC_Polyhedron ph2(3, UNIVERSE);
  ph2.add_constraint(x >= 1);
  ph2.add_constraint(x < 2);
  ph2.add_constraint(y > 0);
  ph2.add_constraint(y < 2);
  ph2.add_constraint(z > 0);
  ph2.add_constraint(z <= 2);
  ph1.add_constraint(x - z > -1);

  print_constraints(ph2, "*** ph2 ***");
  print_generators(ph2.minimized_generators(), "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

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
END_MAIN
