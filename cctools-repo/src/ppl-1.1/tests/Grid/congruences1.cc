/* Test Grid::congruences().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

  Congruence_System cgs = gr1.congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Universe grid.
bool
test02() {
  Grid gr1(7);

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.congruences();

  print_congruences(cgs, "*** cgs ***");

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Zero dimension empty grid.
bool
test03() {
  Grid gr1(0, EMPTY);

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Zero dimension universe grid.
bool
test04() {
  Grid gr1(0);

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
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

  Congruence_System cgs = gr1.congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// 3D rectilinear grid defined by generators.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_grid_generator(grid_point(10*B));
  gr1.add_grid_generator(grid_point(10*A + 10*B));

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Get a reference to the congruences, empty the grid, use the
// reference to create a new grid.
bool
test07() {
  Grid gr1(3);
  gr1.add_congruence(Congruence::zero_dim_integrality());

  const Congruence_System& cgs = gr1.congruences();

  // Empty the grid.  The idea is to check that `cgs' still refers to
  // a congruence system that matches the grid.
  gr1.add_congruence(Congruence::zero_dim_false());

  Grid known_gr = gr1;

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// In zero dimensions get a reference to the universe congruences,
// empty the grid, use the reference to create a new grid.
bool
test08() {
  Grid gr1(0);
  gr1.add_congruence(Congruence::zero_dim_integrality());

  const Congruence_System& cgs = gr1.congruences();

  // Empty the grid.  The idea is to check that `cgs' still refers to
  // a congruence system that matches the grid.
  gr1.add_congruence(Congruence::zero_dim_false());

  Grid known_gr = gr1;

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(cgs, "*** cgs ***");
  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// add congruence systems to a congruence system with smaller space
// dimension.
// This test showed a bug in Congruence_System insert(), now corrected.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  Congruence_System cgs1;
  cgs1.insert((A %= 0) / 2);
  cgs.insert(cgs1);
  cgs1.insert((A + B %= 0) / 2);
  cgs.insert(cgs1, Recycle_Input());

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2);
  known_gr.add_congruence((A %= 0) / 2);
  known_gr.add_congruence((A + B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// add congruence systems to a congruence system
// with larger space dimension.
bool
test10() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  Congruence_System cgs1;
  cgs.insert((A + B %= 0) / 2);
  cgs1.insert((A %= 0) / 2);
  cgs.insert(cgs1);
  print_congruences(cgs, "*** cgs ***");
  print_congruences(cgs1, "*** cgs1 ***");

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2);
  known_gr.add_congruence((A %= 0) / 2);
  known_gr.add_congruence((A + B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// Test is_equal_to() for same congruence systems.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs1;
  cgs1.insert((A %= 0) / 2);
  cgs1.insert((A + B %= 0) / 2);
  Congruence_System cgs(cgs1);
  bool ok = cgs.is_equal_to(cgs1);
  print_congruences(cgs, "*** cgs ***");
  print_congruences(cgs1, "*** cgs1 ***");

  return ok;
}

// Test is_equal_to() for congruence systems with different numbers
// numbers of congruences.
// This test showed a bug in Congruence_System is_equal_to(), now corrected.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs1;
  cgs1.insert((A %= 0) / 2);
  cgs1.insert((A + B %= 0) / 2);
  Congruence_System cgs(cgs1);
  cgs1.insert((B %= 0) / 2);

  bool ok = !cgs.is_equal_to(cgs1);
  print_congruences(cgs, "*** cgs ***");
  print_congruences(cgs1, "*** cgs1 ***");

  return ok;
}

// Test is_equal_to() for different congruence systems with the same
// number of congruences.
bool
test13() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs1;
  Congruence_System cgs2;
  cgs1.insert((A %= 0) / 2);
  cgs1.insert((A + B %= 0) / 2);
  cgs2.insert((B %= 0) / 2);
  cgs2.insert((A + B %= 0) / 2);
  bool ok = !cgs1.is_equal_to(cgs2);
  print_congruences(cgs1, "*** cgs1 ***");
  print_congruences(cgs2, "*** cgs2 ***");

  return ok;
}

// Test has_linear_equalities() for congruence systems.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);
  print_congruences(cgs, "*** cgs.insert((A + B %= 0) / 2) ***");
  bool ok = !cgs.has_linear_equalities();

  cgs.insert(A == 0);
  print_congruences(cgs, "*** cgs.insert(A == 0) ***");
  ok &= cgs.has_linear_equalities();

  return ok;
}

// Test num_equalities() for congruence systems.
bool
test15() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);
  cgs.insert(A == 0);
  print_congruences(cgs, "*** cgs ***");

  bool ok = ((cgs.num_equalities() == 1)
               && (cgs.num_proper_congruences() == 1));

  return ok;
}

// Add to a non-empty congruence system a nonempty constraint system
bool
test16() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert(A %= 0);
  cgs.insert(B == 0);

  Congruence_System known_cgs;
  known_cgs.insert(B == 0);
  known_cgs.insert(A %= 0);

  print_congruences(cgs, "*** cgs ***");

  Grid gr(cgs);
  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

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
  DO_TEST(test15);
  DO_TEST(test16);
END_MAIN
