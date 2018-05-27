/* Test Grid::expand_space_dimension().
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

// Universe.
bool
test01() {
  Variable A(0);

  Grid gr(3);
  print_congruences(gr, "*** gr ***");

  gr.expand_space_dimension(A, 1);

  Grid known_gr(4);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(A, 1) ***");

  return ok;
}

// Empty.
bool
test02() {
  Variable B(1);

  Grid gr(3, EMPTY);
  print_congruences(gr, "*** gr ***");

  gr.expand_space_dimension(B, 1);

  Grid known_gr(4, EMPTY);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(B, 1) ***");

  return ok;
}

// Trivial expansion.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(A + B %= 2);

  gr.expand_space_dimension(A, 0);

  Grid known_gr(2);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence(A + B %= 2);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(A, 0) ***");

  return ok;
}

// From generators, expanding one dimension.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(A + 2*B));
  gr.add_grid_generator(grid_point());
  print_generators(gr, "*** gr ***");

  gr.expand_space_dimension(A, 1);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(A + 2*B));
  known_gr.add_grid_generator(grid_point(C));

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(A, 1) ***");

  return ok;
}

// From congruences, expanding one dimension.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(2);
  gr.add_congruence((A + B %= 2) / 7);
  print_generators(gr, "***  ***");

  gr.expand_space_dimension(A, 1);

  Grid known_gr(3);
  known_gr.add_congruence((A + B     %= 2) / 7);
  known_gr.add_congruence((    B + C %= 2) / 7);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(A, 1) ***");

  return ok;
}

// From congruences, expanding two dimensions.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(2);
  gr.add_congruence((A + 2*B %= 3) / 5);
  print_generators(gr, "***  ***");

  gr.expand_space_dimension(B, 2);

  Grid known_gr(4);
  known_gr.add_congruence((A + 2*B             %= 3) / 5);
  known_gr.add_congruence((A       + 2*C       %= 3) / 5);
  known_gr.add_congruence((A             + 2*D %= 3) / 5);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(B, 2) ***");

  return ok;
}

// From congruences, with an equality.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Grid gr(3);
  gr.add_constraint(2*C == 1);
  gr.add_congruence(A - B %= 0);

  gr.expand_space_dimension(A, 1);
  gr.expand_space_dimension(C, 1);

  Grid known_gr(5);
  known_gr.add_constraint(2*C == 1);
  known_gr.add_constraint(2*E == 1);
  known_gr.add_congruence(A - B         %= 0);
  known_gr.add_congruence(  - B + D     %= 0);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.expand_space_dimension(...) ***");

  return ok;
}

// Variable of higher space dimension than grid.
bool
test08() {
  Variable B(1);

  Grid gr(1, EMPTY);

  try {
    gr.expand_space_dimension(B, 3);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Add more dimensions than are available.
bool
test09() {
  Variable B(1);

  Grid gr(10, EMPTY);

  try {
    gr.expand_space_dimension(B, Grid::max_space_dimension());
  }
  catch (const std::length_error& e) {
    nout << "length_error: " << e.what() << endl;
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
