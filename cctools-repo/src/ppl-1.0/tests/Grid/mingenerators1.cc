/* Test Grid::minimized_grid_generators().
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
  Grid gr1(17, EMPTY);
  print_generators(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// Universe grid.

bool
test02() {
  Grid gr1(1);
  print_generators(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// Zero dimension empty grid.
bool
test03() {
  Grid gr1(0, EMPTY);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// Zero dimension universe grid.
bool
test04() {
  Grid gr1(0);
  print_generators(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// Skew grid in 3D.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*B));
  gr1.add_grid_generator(grid_point(5*A + 2*B));
  gr1.add_grid_generator(grid_point(10*A + B));
  gr1.add_grid_generator(grid_point(7*B));
  gr1.add_grid_generator(grid_point(3*B + 4*C));
  gr1.add_grid_generator(grid_line(C));
  print_generators(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// 3D rectilinear grid defined by congruences.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 10);
  gr1.add_congruence((B %= 10) / 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// Get a reference to the empty generators, add a point, use the
// reference to create a new grid.
bool
test07() {
  Grid gr1(2, EMPTY);

  const Grid_Generator_System& gs = gr1.minimized_grid_generators();

  // Add a point.  The idea is to check that `gs' still refers to a
  // generator system that matches the grid.
  gr1.add_grid_generator(grid_point());
  print_generators(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// In zero dimensions get a reference to the universe generators,
// empty the grid, and then use the reference to create a new grid.
bool
test08() {
  Grid gr1(0);

  const Grid_Generator_System& gs = gr1.minimized_grid_generators();

  // Empty the grid.  The idea is to check that `gs' still refers to a
  // generator system that matches the grid.
  gr1.add_congruence(Congruence::zero_dim_false());
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

  return ok;
}

// Empty grid, where updating the generators finds the grid empty.
bool
test09() {
  Variable C(2);

  Grid gr1(4);
  gr1.add_constraint(C == 4);
  gr1.add_constraint(C == 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.minimized_grid_generators();

  Grid gr2(gs);

  bool ok = (known_gr == gr2);

  print_generators(gr2, "*** gr2(gs) ***");

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
