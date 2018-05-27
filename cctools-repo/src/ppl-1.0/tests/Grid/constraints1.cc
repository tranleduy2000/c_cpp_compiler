/* Test Grid::congruences().
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

// Empty grid.
bool
test01() {
  Grid gr1(7, EMPTY);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.constraints();

  Grid gr2(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Universe grid.
bool
test02() {
  Grid gr1(7);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.constraints();

  Grid gr2(7);
  gr2.add_constraints(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Zero dimension empty grid.
bool
test03() {
  Grid gr1(0, EMPTY);

  Constraint_System cs = gr1.constraints();

  Grid known_gr(Constraint_System::zero_dim_empty());

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1 ***");

  return ok;
}

// Zero dimension universe grid.
bool
test04() {
  Grid gr1(0);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.constraints();

  Grid gr2(0);
  gr2.add_constraints(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Skew grid in 3D.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A + B %= 3) / 7);
  gr1.add_congruence((A %= 0) / 5);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.constraints();

  Grid gr2(3);
  gr2.add_constraints(cs);
  gr2.add_congruence((A + B %= 3) / 7);
  gr2.add_congruence((A %= 0) / 5);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Single point in 2D
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A + B %= 3) / 0);
  gr1.add_congruence((A %= 0) / 0);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.constraints();

  Grid gr2(3);
  gr2.add_constraints(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Linear points in 2D
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A + B %= 3) / 2);
  gr1.add_congruence((A %= 0) / 0);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.constraints();

  Grid gr2(3);
  gr2.add_constraints(cs);
  gr2.add_congruence((A + B %= 3) / 2);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Empty grid.
bool
test08() {
  Grid gr1(7, EMPTY);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.minimized_constraints();

  Grid gr2(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Universe grid.
bool
test09() {
  Grid gr1(7);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.minimized_constraints();

  Grid gr2(7);
  gr2.add_constraints(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Zero dimension empty grid.
bool
test10() {
  Grid gr1(0, EMPTY);

  Constraint_System cs = gr1.minimized_constraints();

  Grid known_gr(Constraint_System::zero_dim_empty());

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1 ***");

  return ok;
}

// Zero dimension universe grid.
bool
test11() {
  Grid gr1(0);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.minimized_constraints();

  Grid gr2(0);
  gr2.add_constraints(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Skew grid in 3D.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A + B %= 3) / 7);
  gr1.add_congruence((A %= 0) / 5);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.minimized_constraints();

  Grid gr2(3);
  gr2.add_constraints(cs);
  gr2.add_congruence((A + B %= 3) / 7);
  gr2.add_congruence((A %= 0) / 5);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Single point in 2D
bool
test13() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A + B %= 3) / 0);
  gr1.add_congruence((A %= 0) / 0);
  gr1.add_congruence((B %= 3) / 0);
  gr1.add_congruence((A - B %= -3) / 0);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.minimized_constraints();

  Grid gr2(3);
  gr2.add_constraints(cs);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Linear points in 2D
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A + B %= 3) / 2);
  gr1.add_congruence((A %= 0) / 0);
  gr1.add_congruence((3*A %= 0) / 0);

  Grid known_gr = gr1;

  Constraint_System cs = gr1.minimized_constraints();

  Grid gr2(3);
  gr2.add_constraints(cs);
  gr2.add_congruence((A + B %= 3) / 2);

  bool ok = (gr2 == known_gr);

  print_constraints(cs, "*** cs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

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
END_MAIN
