/* Test BD_Shape::upper_bound_assign_if_exact().
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
  TBox box_empty(0, EMPTY);
  TBox box_universe(0, UNIVERSE);

  // Testing all combinations for 0-dim polyhedra.
  bool ok = true;
  TBox box;

  // empty, empty
  box = box_empty;
  ok &= box.upper_bound_assign_if_exact(box_empty);
  ok &= (box == box_empty);
  print_constraints(box, "*** empty union empty ***");

  // empty, universe
  box = box_empty;
  ok &= box.upper_bound_assign_if_exact(box_universe);
  ok &= (box == box_universe);
  print_constraints(box, "*** empty union universe ***");

  // universe, empty
  box = box_universe;
  ok &= box.upper_bound_assign_if_exact(box_empty);
  ok &= (box == box_universe);
  print_constraints(box, "*** universe union empty ***");

  // universe, universe
  box = box_universe;
  ok &= box.upper_bound_assign_if_exact(box_universe);
  ok &= (box == box_universe);
  print_constraints(box, "*** universe union universe ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x >= -2);
  box1.add_constraint(x <= -1);
  box1.add_constraint(y >= 0);
  box1.add_constraint(y <= 2);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(x >= 1);
  box2.add_constraint(x <= 2);
  box2.add_constraint(y >= 0);
  box2.add_constraint(y <= 2);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(box1);

  bool ok = !box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x >= -2);
  box1.add_constraint(x <= 0);
  box1.add_constraint(y >= 0);
  box1.add_constraint(y <= 2);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(x >= 0);
  box2.add_constraint(x <= 2);
  box2.add_constraint(y >= 0);
  box2.add_constraint(y <= 2);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(2, UNIVERSE);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x == 0);
  box1.add_constraint(y == 0);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(x >= 0);
  box2.add_constraint(x <= 2);
  box2.add_constraint(y >= -2);
  box2.add_constraint(y <= 2);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(box2);

  bool ok = box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x >= 0);
  box1.add_constraint(y == 0);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(x >= 0);
  box2.add_constraint(y >= 2);
  box2.add_constraint(y <= 4);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(box1);

  bool ok = !box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x == 0);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(y == 0);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(box1);

  bool ok = !box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(y >= 0);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(y <= 5);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(2, UNIVERSE);

  bool ok = box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 4);
  box1.add_constraint(y >= 1);
  box1.add_constraint(y <= 3);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(x >= 1);
  box2.add_constraint(x <= 3);
  box2.add_constraint(y >= 0);
  box2.add_constraint(y <= 4);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(box1);

  bool ok = !box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  TBox box1(2, UNIVERSE);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 1);
  box1.add_constraint(y >= 0);
  box1.add_constraint(y <= 2);

  print_constraints(box1, "*** box1 ***");

  TBox box2(2, UNIVERSE);
  box2.add_constraint(x >= 0);
  box2.add_constraint(x <= 3);
  box2.add_constraint(y >= 1);
  box2.add_constraint(y <= 2);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(box1);

  bool ok = !box1.upper_bound_assign_if_exact(box2);
  ok &= (box1 == known_result);

  print_constraints(box1, "*** box1.upper_bound_assign_if_exact(box2) ***");

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
