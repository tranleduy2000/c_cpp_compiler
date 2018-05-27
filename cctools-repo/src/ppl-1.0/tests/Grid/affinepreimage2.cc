/* Test Grid::affine_preimage().
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

// Denominator, with generators having a variety of divisors.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(2*A, 15));
  gs.insert(grid_point(5*B, 10));

  // The divisors are normalized on construction.
  Grid gr(gs);

  print_generators(gr, "*** gr ***");

  // All divisors should change, even when the coefficient of A is 0.
  gr.affine_preimage(A, 2*A, 5);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A, 3));
  known_gr.add_grid_generator(grid_point(B, 2));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(A, 2*A, 5) ***");

  return ok;
}

// Negative denominator.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(-2*A, 3));

  gr.affine_preimage(A, B + 2, -3);

  print_generators(gr, "*** gr ***");

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(A, B + 2, -3) ***");

  return ok;
}

// Empty grid.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, 11*A - B + 1);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, 11*A - B + 1) ***");

  return ok;
}

// A negative coefficient of the variable to transform and a negative
// denominator.
// Congruences are out-of-date
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(5*A + 4*B, 7));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(B, A - B, -1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(5*A + 9*B, 7));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(B, A - B, -1) ***");

  return ok;
}

// A negative coefficient of the variable to transform and a negative
// denominator.
// Congruences are up-to-date
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr = Grid(2);
  gr.add_congruence((7*A == 5) / 0);
  gr.add_congruence((7*B == 4) / 0);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(B, A - B, -1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(5*A + 9*B, 7));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(B, A - B, -1) ***");

  return ok;
}

// Shift a rectilinear pointed grid along A.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(3);
  gr.add_congruence((A %= 0) / 4);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, A + 3);

  Grid known_gr(3);
  known_gr.add_congruence((A %= -3) / 4);
  known_gr.add_congruence((B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, A + 3) ***");

  return ok;
}

// Slant a rectilinear pointed grid along A == -B.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 4);
  gr.add_congruence((B %= 0) / 2);

  gr.affine_preimage(A, A + B);

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 4);
  known_gr.add_congruence((A %= 0) / 2);
  known_gr.add_congruence((B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, A + B) ***");

  return ok;
}

// Expand a rectilinear pointed grid to a sequence of lines.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 4);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, B);

  Grid known_gr(2);
  known_gr.add_congruence((B %= 0) / 4);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, B) ***");

  return ok;
}

// Transform a grid into the empty grid.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A == 3) / 0);
  gr.add_congruence((B == 0) / 0);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(B, A);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(B, A) ***");

  return ok;
}

// Transform away the constraint on a dimension (B).
bool
test10() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A == 3) / 0);
  gr.add_congruence((B == 3) / 0);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(B, A);

  Grid known_gr(2);
  known_gr.add_congruence((A == 3) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(B, A) ***");

  return ok;
}

// Zero denominator.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.affine_preimage(B, A + 2, 0);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Expression of a greater space dimension than the grid.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.affine_preimage(B, D + 2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Variable of a greater space dimension than the grid.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.affine_preimage(D, A + 2);
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
  DO_TEST(test13);
END_MAIN
