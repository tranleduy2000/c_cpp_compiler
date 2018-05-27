/* Test Grid::affine_image().
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

// Denominator, with generators having a variety of divisors.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(A, 3));
  gs.insert(grid_point(B, 2));

  // The divisors are normalized on construction.
  Grid gr(gs);

  print_generators(gr, "*** gr ***");

  // All divisors should change, even when the coefficient of A is 0.
  gr.affine_image(A, 2*A, 5);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(2*A, 15));
  known_gr.add_grid_generator(grid_point(5*B, 10));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_image(A, 2*A, 5) ***");

  return ok;
}

// Negative denominator.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(A));

  print_generators(gr, "*** gr ***");

  gr.affine_image(A, B + 2, -3);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(-2*A, 3));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_image(A, B + 2, -3) ***");

  return ok;
}

// Empty grid.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.affine_image(A, 2*A + B + 1);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_image(A, 2*A + B + 1) ***");

  return ok;
}

// Shift a rectilinear pointed grid along A.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A %= 0) / 4);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.affine_image(A, A + 3);

  Grid known_gr(3);
  known_gr.add_congruence((A %= 3) / 4);
  known_gr.add_congruence((B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_image(A, A + 3) ***");

  return ok;
}

// Slant a rectilinear pointed grid along A == B.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 4);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.affine_image(A, A + B);

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 0) / 4);
  known_gr.add_congruence((A %= 0) / 2);
  known_gr.add_congruence((B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_image(A, A + B) ***");

  return ok;
}

// Compress a rectilinear pointed grid to a line of points.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.add_congruence((A %= 0) / 4);
  gr.add_congruence((B %= 0) / 2);

  gr.affine_image(A, B);

  Grid known_gr(2);
  known_gr.add_congruence((A - B == 0) / 0);
  known_gr.add_congruence((A %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_image(A, B) ***");

  return ok;
}

// Zero denominator.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.affine_image(B, A + 2, 0);
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
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.affine_image(B, D + 2);
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
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.affine_image(D, A + 2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Based on an example in a paper by Muller-Olm and Seidl in SAS 2005
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(2*A + 0*B));

  print_congruences(gr, "*** gr ***");

  Grid gr0 = gr;  // first grid (using trivial transformation)

  Grid gr1 = gr;  // second grid - initial state

  gr1.generalized_affine_image(B, EQUAL, 18*A + B, 1, 0);
  gr1.generalized_affine_image(A, EQUAL, 15*A, 1, 0);
                  // second grid - 1 pass through procedure

  Grid gr2 = gr;  // third grid - initial state

  gr2.affine_image(B, 282*A + B);
  gr2.affine_image(A, 225*A);
                  // third grid - 2 passes through procedure

  gr.upper_bound_assign(gr1); // join of gr0 and gr1

  print_congruences(gr, "*** gr.upper_bound_assign(gr1) ***");

  gr.upper_bound_assign(gr2); // join of gr0, gr1 and gr2

  Grid known_gr(2);

  known_gr.add_congruence((A %= 2) / 28);
  known_gr.add_congruence((B %= 0) / 12);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.upper_bound_assign(gr2) ***");

  return ok;
}

// Example from Muller-Olm and Seidl SAS 2005 paper
bool
test11() {

  Variable A(0);
  Variable B(1);

  Grid_Generator_System ggs;
  ggs.insert(grid_point(A + 0*B));

  Grid gr1(ggs);
  Grid gr(2, EMPTY);

  for(int j = 0; j < 4; j++) {
    gr.add_grid_generators(ggs);

    for(int i = 0; i < j; i++) {
      gr.affine_image(A, 3*A);
      gr.affine_image(B, B + A);
    }
    for(int i = 0; i < j; i++) {
      gr.affine_image(A, 5*A);
      gr.affine_image(B, B + A);
    }
    gr1.upper_bound_assign(gr);
  }

  gr.add_grid_generators(ggs);

  bool ok = (gr == gr1);

  print_congruences(gr1, "*** gr1 ***");
  print_generators(gr1, "*** gr1 ***");
  print_congruences(gr, "*** gr ***");
  print_generators(gr, "*** gr ***");

  return ok;
}

// Example from Muller-Olm and Seidl ESOP 2005 paper
bool
test12() {

  Variable A(0);
  Variable B(1);

  Coefficient* tem1 = new Coefficient("7654321");
  Coefficient* tem2 = new Coefficient("69246289");
  Coefficient* tem3 = new Coefficient("4294967296");

  Grid_Generator_System ggs;
  ggs.insert(grid_point(A));
  ggs.insert(parameter(*tem3*A));
  ggs.insert(parameter(*tem3*B));

  Grid gr1(ggs);
  Grid gr(2, EMPTY);

  for(int j = 0; j < 3; j++) {
    gr.add_grid_generators(ggs);

    for(int i = 0; i < j; i++) {
      gr.affine_image(A, *tem1*A);
      gr.affine_image(B, B + A);
    }
    for(int i = 0; i < j; i++) {
      gr.affine_image(A, *tem2*A);
      gr.affine_image(B, B + A);
    }
    gr1.upper_bound_assign(gr);
  }

  gr.add_grid_generators(ggs);

  delete tem1; delete tem2; delete tem3;

  bool ok = (gr == gr1);

  print_congruences(gr1, "*** gr1 ***");
  print_generators(gr1, "*** gr1 ***");
  print_congruences(gr, "*** gr ***");
  print_generators(gr, "*** gr ***");

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
  DO_TEST_F16(test10);
  DO_TEST_F32(test11);
  DO_TEST_F64(test12);
END_MAIN
