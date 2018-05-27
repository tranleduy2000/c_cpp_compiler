/* Test method which can add multiple generators to a grid.
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

// grid1.cc also tests add_grid_generators.

// add_recycled_grid_generators -- space dimension exception.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point(B));

  Grid gr(1);

  try {
    gr.add_recycled_grid_generators(gs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// add_recycled_grid_generators -- zero dimension universe.
bool
test02() {
  Grid_Generator_System gs;
  gs.insert(grid_point());

  Grid gr(0);

  print_generators(gr, "*** gr ***");

  gr.add_recycled_grid_generators(gs);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_grid_generators(gs) ***");

  return ok;
}

// add_recycled_grid_generators -- zero dimension empty.
bool
test03() {
  Grid_Generator_System gs;
  gs.insert(grid_point());

  Grid gr(0, EMPTY);

  print_generators(gr, "*** gr ***");

  gr.add_recycled_grid_generators(gs);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_grid_generators(gs) ***");

  return ok;
}

// add_recycled_grid_generators -- add system with a single parameter
// generator to the empty grid.
bool
test04() {
  Variable A(0);

  Grid_Generator_System gs;
  gs.insert(parameter(A));

  Grid gr(2, EMPTY);

  try {
    gr.add_recycled_grid_generators(gs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// add_recycled_grid_generators -- add an empty system.
bool
test05() {
  Grid_Generator_System gs;

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());

  print_generators(gr, "*** gr ***");

  Grid known_gr = gr;

  gr.add_recycled_grid_generators(gs);

  bool ok = (gr == known_gr);

  print_generators(gr,
		   "*** gr.add_recycled_grid_generators(gs) ***");

  return ok;
}

// add_grid_generators -- add a zero dimension universe system.
bool
test06() {
  Grid gr(0);

  print_generators(gr, "*** gr ***");

  Grid known_gr = gr;

  gr.add_grid_generators(Grid_Generator_System::zero_dim_univ());

  bool ok = (gr == known_gr);

  print_generators(gr,
		   "*** gr.add_grid_generators(Grid_Generator_System::zero_dim_univ()) ***");

  return ok;
}

// add_grid_generators -- add to a grid where the generator system of the
// grid starts with a parameter (test point finding loop in
// Grid::normalize_divisors(gs, gs)).
bool
test07() {
  Variable A(0);

  Grid_Generator_System gs1;
  gs1.insert(parameter(2*A));
  gs1.insert(grid_point());

  Grid gr(gs1);

  Grid_Generator_System gs2;
  gs2.insert(grid_point());
  gs2.insert(parameter(A));

  gr.add_grid_generators(gs2);

  print_generators(gr, "*** gr ***");

  Grid known_gr(1, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(parameter(A));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// add_recycled_grid_generators -- add to a zero
// dimension universe grid.
bool
test08() {
  Grid gr(0);

  Grid_Generator_System gs2(grid_point());

  gr.add_recycled_grid_generators(gs2);

  print_generators(gr, "*** gr ***");

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.add_grid_generators(gs2) ***");

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
END_MAIN
