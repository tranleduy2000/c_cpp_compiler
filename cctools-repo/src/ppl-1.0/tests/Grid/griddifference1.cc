/* Test Grid::difference_assign() (a.k.a. Grid::difference_assign()).
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

// Simple grids.
bool
test01() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_congruence(A %= 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence((A %= 0) / 2);

  gr1.difference_assign(gr2);

  Grid known_gr(1, EMPTY);
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(3*A));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Both universe.
bool
test02() {
  Grid gr1;
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2;

  gr1.difference_assign(gr2);

  Grid known_gr(0, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// First contained in second.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A - B %= 0) / 3);
  gr1.add_congruence((A %= 0) / 2);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence(B %= 0);

  gr1.difference_assign(gr2);

  Grid known_gr(2, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Second contains single equality.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A - B %= 0) / 3);
  gr1.add_congruence((A %= 0) / 2);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_constraint(A == 5);

  Grid known_gr(gr1);

  gr1.difference_assign(gr2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// First empty.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A + 2*B %= 0);

  Grid gr2(2, EMPTY);

  Grid known_gr(gr1);

  gr1.difference_assign(gr2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Second empty.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2, EMPTY);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A + 2*B %= 0);

  gr1.difference_assign(gr2);

  Grid known_gr(2, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// More complex example, from generators.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_point(A - 2*C));
  gr1.add_grid_generator(grid_point(3*B));
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(A));
  gr2.add_grid_generator(grid_point(A + 2*C));
  gr2.add_grid_generator(grid_point(3*A));
  gr2.add_grid_generator(grid_point(A + 3*B));

  gr1.difference_assign(gr2);

  Grid known_gr(3);
  known_gr.add_constraint(2*A + C == 0);
  known_gr.add_congruence((B %= 0) / 3);
  known_gr.add_congruence((A %= 0) / 2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Zero dimension grids.
bool
test08() {
  Grid gr1(0);

  Grid gr2(0);

  gr1.difference_assign(gr2);

  Grid known_gr(0, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// The smallest resulting grid is the first grid, even though the
// first grid has more points than the second.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_line(A));
  gr1.add_grid_generator(grid_point(B));
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((B %= 0) / 3);

  Grid known_gr(gr1);

  gr1.difference_assign(gr2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Simpler example where the resulting grid contains points.

bool
test10() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_point(A));
  gr1.add_grid_generator(grid_point(B));
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((A - B %= 0) / 2);
  gr2.add_congruence(A %= 0);

  gr1.difference_assign(gr2);

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 1) / 2);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Where the 2-complements of more than one congruence are added to
// the result.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_constraint(B == 0);
  gr1.add_constraint(C == 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_congruence((A + C %= 0) / 4);
  gr2.add_congruence((A + B %= 0) / 4);

  gr1.difference_assign(gr2);

  Grid known_gr(3);
  known_gr.add_congruence((A %= 2) / 4);
  known_gr.add_constraint(B == 0);
  known_gr.add_constraint(C == 0);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.difference_assign(gr2) ***");

  return ok;
}

// Space dimension exception.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(B + 0*C));

  Grid gr1(gs);

  Grid gr2(4);
  gr2.add_constraint(A == 0);
  gr2.add_constraint(B == 0);
  gr2.add_constraint(C == 0);

  try {
    gr1.difference_assign(gr2);
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
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
END_MAIN
