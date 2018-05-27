/* Test Grid::bounded_affine_image().
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

// Non-empty grid.
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A ==  0) / 0);
  gr.add_congruence((B ==  0) / 0);
  gr.add_congruence((C == -2) / 0);

  print_congruences(gr, "*** gr ***");

  gr.bounded_affine_image(A, 7-B, B+3);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(-2*C));
  known_gr.add_grid_generator(grid_line(A));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.bounded_affine_image(A, 7-B, B+3) ***");

  return ok;
}

// Empty grid.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A ==  0) / 0);
  gr.add_congruence((A ==  1) / 0);
  gr.add_congruence((C == -2) / 0);

  print_congruences(gr, "*** gr ***");

  gr.bounded_affine_image(A, 7-B, B+3);

  Grid known_gr(3, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.bounded_affine_image(A, 7-B, B+3) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A - B %= 0);

  try {
    // This is an incorrect use of function
    // Grid::bounded_affine_image(v, lb_expr, ub_expr, d):
    // any call with a denominator equal to zero is illegal.
    Coefficient d = 0;
    gr.bounded_affine_image(B, A - 7, B + 2, d);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(1);
  gr.add_congruence(A %= 0);

  try {
    // This is an incorrect use of function
    // Grid::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to use a variable in the lower bounding expression
    // that does not appear in the grid.
    gr.bounded_affine_image(A, B, A + 7);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(1);
  gr.add_congruence(A %= 0);

  try {
    // This is an incorrect use of function
    // Grid::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to use a variable in the upper bounding expression
    // that does not appear in the grid.
    gr.bounded_affine_image(A, A + 7, B);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(1);
  gr.add_congruence(A %= 1);

  try {
    // This is an incorrect use of function
    // Grid::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to bound a variable not occurring in the
    // vector space embedding the grid.
    gr.bounded_affine_image(B, A - 7, 2*A - 2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
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
END_MAIN
