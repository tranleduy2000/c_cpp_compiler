/* Test Grid::contains().
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

// Grid of points containing empty grid.
bool
test01() {
  Variable A(0);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));

  Grid gr1(gs);

  Grid gr2(1, EMPTY);

  bool ok = (gr1.contains(gr2));

  print_generators(gr1, "*** gr1 ***");
  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Empty grid and grid of points.
bool
test02() {
  Variable B(1);

  Grid gr1(2, EMPTY);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(B));

  Grid gr2(gs);

  bool ok = (!gr1.contains(gr2));

  print_generators(gr1, "*** gr1 ***");
  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Both empty.
bool
test03() {
  Grid gr1(4, EMPTY);

  Grid gr2(4, EMPTY);

  bool ok = (gr1.contains(gr2));

  print_generators(gr1, "*** gr1 ***");
  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Zero dimension universes.
bool
test04() {
  Grid gr1(0);

  Grid gr2(0);

  bool ok = (gr1.contains(gr2));

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Self containment.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence(A - B %= 0);
  gr.add_congruence(C %= 0);

  bool ok = (gr.contains(gr));

  print_congruences(gr, "*** gr ***");

  return ok;
}

// A grid strictly containing another.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence(A - B %= 0);
  gr1.add_congruence(C %= 0);

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(grid_point(2*A + 2*B + 2*C));

  bool ok = (gr1.contains(gr2));

  print_congruences(gr1, "*** gr1 ***");
  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// test06, the other way round.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_point(2*A + 2*B + 2*C));

  Grid gr2(3);
  gr2.add_congruence(A - B %= 0);
  gr2.add_congruence(C %= 0);

  bool ok = (!gr1.contains(gr2));

  print_generators(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// CHINA example that showed an error in cgs::is_included_in.
bool
test08() {
  Variable A(0);

  Grid gr1(1, EMPTY);
  gr1.add_grid_generator(grid_point());

  Grid gr2(1, EMPTY);
  gr2.add_grid_generator(grid_point(A));

  bool ok = !gr1.contains(gr2) && !gr2.contains(gr1);

  print_generators(gr1, "*** gr1 ***");
  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Space dimension exception
bool
test09() {
  Grid gr1(1, EMPTY);
  gr1.add_grid_generator(grid_point());

  Grid gr2(21, EMPTY);

  try {
    gr1.contains(gr2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
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
