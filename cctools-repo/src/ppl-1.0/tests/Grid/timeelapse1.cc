/* Test Grid::time_elapse_assign().
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

// Zero dimension.
bool
test01() {
  Grid gr1(0);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(0);
  print_generators(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(0);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// Zero dimension, second grid empty.
bool
test02() {
  Grid gr1(0);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(0, EMPTY);
  print_generators(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(0, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// First grid empty.
bool
test03() {
  Variable A(0);

  Grid gr1(4, EMPTY);
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(4);
  gr2.add_congruence(A %= 3);
  print_congruences(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(4, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// Second grid empty.
bool
test04() {
  Variable A(0);

  Grid gr1(4);
  gr1.add_congruence(A %= 3);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(4, EMPTY);
  print_generators(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(4, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// Second grid a single point at the origin.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A + 2*B %= 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, EMPTY);
  gr2.add_grid_generator(grid_point());
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr(gr1);

  gr1.time_elapse_assign(gr2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}


// One dimension grids of equalities.
bool
test06() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_constraint(A == 2);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_constraint(A == 1);
  print_congruences(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(1);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// One dimension grids with congruences.
bool
test07() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_constraint(A == 2);
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence((A %= 0) / 3);
  print_congruences(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(1);
  known_gr.add_congruence((A %= 2) / 3);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// Multi-dimension grids.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A - B %= 1) / 6);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((A %= 0) / 2);
  gr2.add_constraint(B == 0);
  print_congruences(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 1) / 2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// Multi-dimension grids with denominators, in timeelapse2.

// Multi-dimension grids from generators in sub-optimal form.
bool
test09() {
  Variable A(0);
  Variable D(3);

  Grid gr1(4, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_point(2*A));
  gr1.add_grid_generator(grid_point(4*A));
  gr1.add_grid_generator(grid_point(D));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(4, EMPTY);
  gr2.add_grid_generator(grid_point(A));
  print_generators(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(4, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(D));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// A grid of a single point, where the position of the second grid
// causes the result to be more dense than the second grid.
bool
test10() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_constraint(A == 2);
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence((A %= 1) / 3);
  print_congruences(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(1);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

  return ok;
}

// Space dimension exception.
bool
test11() {
  Grid gr1(1, EMPTY);
  gr1.add_grid_generator(grid_point());
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(19, EMPTY);
  print_generators(gr2, "*** gr2 ***");

  try {
    gr1.time_elapse_assign(gr2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Multi-dimension grids with denominators.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_point(A + 2*B - 3*C, 3));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(3*A - B + 4*C, 7));
  print_generators(gr2, "*** gr2 ***");

  gr1.time_elapse_assign(gr2);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A + 2*B - 3*C, 3));
  known_gr.add_grid_generator(grid_point(3*A - B + 4*C, 7));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.time_elapse_assign(gr2) ***");

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
  DO_TEST_F8(test12);
END_MAIN
