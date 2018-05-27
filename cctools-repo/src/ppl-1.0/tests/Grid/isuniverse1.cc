/* Test Grid::is_universe().
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

// One dimension.
bool
test01() {
  Grid gr(1);

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Many dimensions.

bool
test02() {
  Grid gr(6);

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Zero dimension universe.

bool
test03() {
  Grid gr(0);

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Zero dimension empty.

bool
test04() {
  Grid gr(0, EMPTY);

  bool ok = (!gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Empty grid.

bool
test05() {
  Grid gr(2, EMPTY);

  bool ok = (!gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Grid of congruences.

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + B + C %= 0) / 3);

  Grid gr(cgs);

  bool ok = (!gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Universe grid of congruences.

bool
test07() {
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((0*C %= 6) / 3);

  Grid gr(cgs);

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Grid of generators.

bool
test08() {
  Variable A(0);
  Variable E(4);

  Grid_Generator_System gs;
  gs.insert(grid_point(A + 3*E));

  Grid gr(gs);

  bool ok = (!gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Universe grid of generators.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Grid_Generator_System gs;
  gs.insert(grid_point(A + 3*E));
  gs.insert(grid_line(A));
  gs.insert(grid_line(B));
  gs.insert(grid_line(C));
  gs.insert(grid_line(D));
  gs.insert(grid_line(E));

  Grid gr(gs);

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Minimized congruences.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + B + C %= 0) / 3);

  Grid gr(cgs);

  // Minimize the congruences.
  gr.minimized_congruences();

  bool ok = (!gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Minimized universe congruences.
bool
test11() {
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((0*C %= 3) / 3);

  Grid gr(cgs);

  // Minimize the congruences.
  gr.minimized_congruences();

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Minimized universe congruences.
bool
test12() {
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((0*C %= 4) / 2);

  Grid gr(cgs);

  // Minimize the congruences.
  gr.minimized_congruences();

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Universe after remove_space_dimensions.

bool
test13() {
  Variable A(0);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 4) / 2);

  Grid gr(cgs);

  Variables_Set vars;
  vars.insert(A);

  gr.remove_space_dimensions(vars);

  // Minimize the congruences.
  gr.minimized_congruences();

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Empty from a simple constraint.

bool
test14() {
  Variable C(2);

  Congruence_System cgs;
  cgs.insert(0*C == 0);

  Grid gr(cgs);

  // Minimize the congruences.
  gr.minimized_congruences();

  bool ok = (gr.is_universe());

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Congruences before minimization, where a point is required to
// determine that the grid is a strict subset of the universe.

bool
test15() {
  Variable A(0);

  Congruence_System cgs;
  cgs.insert(A == 3);

  Grid gr(cgs);

  bool ok = (!gr.is_universe());

  print_congruences(gr, "*** gr ***");

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
END_MAIN
