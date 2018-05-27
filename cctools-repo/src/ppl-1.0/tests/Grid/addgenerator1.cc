/* Test Grid::add_grid_generator().
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

// grid1.cc also tests add_grid_generator.

// One dimension.
bool
test01() {
  Variable A(0);

  Grid gr(1, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.add_grid_generator(grid_point(-A));

  Grid known_gr(1);
  known_gr.add_congruence((A == -1) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(grid_point(-A)) ***");

  return ok;
}

// Two dimensions.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");
  gr.add_grid_generator(grid_point(A + B));

  Grid known_gr(2);
  known_gr.add_congruence((A == 1) / 0);
  known_gr.add_congruence((B == 1) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(grid_point(A + B)) ***");

  return ok;
}

// Add many generators to grid of two dimensions.

bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A + 2*B));
  gr.add_grid_generator(grid_point(A + B));
  gr.add_grid_generator(grid_point(2*A + 2*B));
  gr.add_grid_generator(grid_line(A));

  Grid known_gr(2);
  known_gr.add_congruence(B %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(...) ***");

  return ok;
}

// Add generators where one has a divisor.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point(7*A, 4));
  gs.insert(grid_line(A - B));

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  for (Grid_Generator_System::const_iterator i = gs.begin(),
	 gs_end = gs.end(); i != gs_end; ++i)
    gr.add_grid_generator(*i);

  Grid known_gr(2);
  known_gr.add_congruence((4*A + 4*B == 7) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(*i) ***");

  return ok;
}

// Add generators to a grid of a higher space dimension.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4, EMPTY);
  print_congruences(gr, "*** gr ***");

  gr.add_grid_generator(grid_point(7*A, 3));
  print_congruences(gr, "*** gr.add_grid_generator(grid_point(7*A, 3)) ***");
  gr.add_grid_generator(grid_line(A - B));

  Grid known_gr(4);

  known_gr.add_congruence((3*A + 3*B == 7) / 0);
  known_gr.add_congruence((C == 0) / 0);
  known_gr.add_congruence((D == 0) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(grid_line(A - B)) ***");

  return ok;
}

// Add a generator to a universe grid.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4);

  print_congruences(gr, "*** gr ***");

  gr.add_grid_generator(grid_point(12*A + 7*D));

  Grid known_gr(4);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_grid_generator(grid_point(12*A + 7*D)) ***");

  return ok;
}

// adding a generator with a divisor to a grid of many generators.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);

  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));

  print_congruences(gr, "*** gr ***");

  // Minimize the grid.

  gr.add_grid_generator(grid_point(B, 3));

  Grid known_gr(2);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence(3*B %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_grid_generator(grid_point(B, 3)) ***");

  return ok;
}

// Space dimension exception.
bool
test08() {
  Variable A(0);
  Variable C(2);

  Grid gr(2);

  try {
    gr.add_grid_generator(grid_point(A + C));
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Zero dimensions empty.
bool
test09() {
  Grid gr(0, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.add_grid_generator(grid_point());

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(grid_point()) ***");

  return ok;
}

// Zero dimension universe.
bool
test10() {
  Grid gr(0);

  print_congruences(gr, "*** gr ***");

  gr.add_grid_generator(grid_point());

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(grid_point()) ***");

  return ok;
}

// Space dimension exception.
bool
test11() {
  Variable A(0);

  Grid gr(2, EMPTY);

  try {
    gr.add_grid_generator(grid_line(A));
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Try add parameter to empty grid.
bool
test12() {
  Grid gr(2, EMPTY);

  try {
    gr.add_grid_generator(parameter());
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Try add parameter to zero dimension empty grid.
bool
test13() {
  Grid gr(0, EMPTY);

  try {
    gr.add_grid_generator(parameter());
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

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
