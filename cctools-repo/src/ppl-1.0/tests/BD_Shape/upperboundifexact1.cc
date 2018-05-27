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
  TBD_Shape bds_empty(0, EMPTY);
  TBD_Shape bds_universe(0, UNIVERSE);

  // Testing all combinations for 0-dim polyhedra.
  bool ok = true;
  TBD_Shape bds;

  // empty, empty
  bds = bds_empty;
  ok &= bds.upper_bound_assign_if_exact(bds_empty);
  ok &= (bds == bds_empty);
  print_constraints(bds, "*** empty union empty ***");

  // empty, universe
  bds = bds_empty;
  ok &= bds.upper_bound_assign_if_exact(bds_universe);
  ok &= (bds == bds_universe);
  print_constraints(bds, "*** empty union universe ***");

  // universe, empty
  bds = bds_universe;
  ok &= bds.upper_bound_assign_if_exact(bds_empty);
  ok &= (bds == bds_universe);
  print_constraints(bds, "*** universe union empty ***");

  // universe, universe
  bds = bds_universe;
  ok &= bds.upper_bound_assign_if_exact(bds_universe);
  ok &= (bds == bds_universe);
  print_constraints(bds, "*** universe union universe ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x >= -2);
  bds1.add_constraint(x <= -1);
  bds1.add_constraint(y >= 0);
  bds1.add_constraint(y <= 2);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x >= 1);
  bds2.add_constraint(x <= 2);
  bds2.add_constraint(y >= 0);
  bds2.add_constraint(y <= 2);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(bds1);

  bool ok = !bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x >= -2);
  bds1.add_constraint(x <= 0);
  bds1.add_constraint(y >= 0);
  bds1.add_constraint(y <= 2);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(x <= 2);
  bds2.add_constraint(y >= 0);
  bds2.add_constraint(y <= 2);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(2, UNIVERSE);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x == 0);
  bds1.add_constraint(y == 0);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(x <= 2);
  bds2.add_constraint(y >= -2);
  bds2.add_constraint(y <= 2);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(bds2);

  bool ok = bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(y == 0);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(y >= 2);
  bds2.add_constraint(y <= 4);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(bds1);

  bool ok = !bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x == y);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x == 0);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(bds1);

  bool ok = !bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x >= y);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x >= 0);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(bds1);

  bool ok = !bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x >= y);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x <= y);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(2, UNIVERSE);

  bool ok = bds1.upper_bound_assign_if_exact(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3, UNIVERSE);
  bds.add_constraint(x >= 0);
  bds.add_constraint(x <= 2);
  bds.add_constraint(y >= 0);
  bds.add_constraint(y <= 4);
  bds.add_constraint(z >= 0);
  bds.add_constraint(z <= 4);
  bds.add_constraint(x - y <= 2);
  bds.add_constraint(z - y <= 2);

  TBD_Shape bds1(bds);
  bds1.add_constraint(z <= 3);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(bds);
  bds2.add_constraint(x - y <= 1);

  bool ok = bds1.upper_bound_assign_if_exact(bds2);

  ok &= (bds1 == bds);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2, UNIVERSE);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(x <= 3);
  bds1.add_constraint(y >= 0);
  bds1.add_constraint(y <= 2);
  bds1.add_constraint(x - y <= 2);

  TBD_Shape bds2(2, UNIVERSE);
  bds2.add_constraint(x >= 3);
  bds2.add_constraint(x <= 6);
  bds2.add_constraint(y >= 0);
  bds2.add_constraint(y <= 2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(bds1);

  bool ok = !bds1.upper_bound_assign_if_exact(bds2);

  ok &= (bds1 == known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign_if_exact(bds2) ***");

  return ok;
}

bool
test11() {
  // const dimension_type dim = 500;
  const dimension_type dim = 50;
  Constraint_System cs;
  for (dimension_type i = 1; i < dim; ++i) {
    Variable x(i);
    cs.insert(x >= 0);
    cs.insert(x <= 4);
  }

  Variable x(0);

  TBD_Shape hypercube1(cs);
  hypercube1.add_constraint(x >= 0);
  hypercube1.add_constraint(x <= 4);

  TBD_Shape hypercube2(cs);
  hypercube2.add_constraint(x >= 2);
  hypercube2.add_constraint(x <= 6);

  TBD_Shape known_result(cs);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 6);

  bool ok = hypercube1.upper_bound_assign_if_exact(hypercube2);

  ok &= (hypercube1 == known_result);

  print_constraints(hypercube1, "*** hyp1 ***");
  print_constraints(hypercube2, "*** hyp2 ***");

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
