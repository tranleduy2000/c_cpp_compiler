/* Test Grid::generalized_affine_image(var, ...).
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

// Simplest expression.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((B %= 0) / 2);

  gr.generalized_affine_image(B, EQUAL, Linear_Expression::zero(), 1, 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, Linear_Expression::zero(), 1, 0) ***");

  return ok;
}

// Simplest expression, with denominator.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(B, EQUAL, Linear_Expression::zero(), 2, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, Linear_Expression::zero(), 2, 1) ***");

  return ok;
}

// Simple expression.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A + B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(B, EQUAL, A + 1, 1, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A - B));
  known_gr.add_grid_generator(grid_point(B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, A + 1, 1, 1) ***");

  return ok;
}

// Simple expression, with denominator.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A + B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(B, EQUAL, A + 1, 2, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(B, 2));
  known_gr.add_grid_generator(grid_point(B + A));
  known_gr.add_grid_generator(grid_point(3*B, 2));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, A + 1, 2, 1) ***");

  return ok;
}

// Simple expression, with denominator and modulus.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A + B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(B, EQUAL, A + 1, 2, 3);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(B, 2));
  known_gr.add_grid_generator(grid_point(A + B));
  known_gr.add_grid_generator(grid_point(7*B, 2));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, A + 1, 2, 3) ***");

  return ok;
}

// Negative denominator.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A - B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(B, EQUAL, A + 2, -2, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(-B));
  known_gr.add_grid_generator(grid_point(-3*B + 2*A, 2));
  known_gr.add_grid_generator(grid_point(-2*B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, A + 2, -2, 1) ***");

  return ok;
}

// Negative modulus.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A - B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(B, EQUAL, A + 2, 1, -7);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(2*B));
  known_gr.add_grid_generator(grid_point(A + 3*B));
  known_gr.add_grid_generator(grid_point(9*B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(B, EQUAL, A + 2, 1, -7) ***");

  return ok;
}

// Expression of many variables.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((B %= 0) / 3);
  gr.add_congruence((A - 2*C %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A, EQUAL, A - C + 2, 1, 5);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(2*A));
  known_gr.add_grid_generator(grid_point(2*A + 3*B));
  known_gr.add_grid_generator(grid_line(A + C));
  known_gr.add_grid_generator(grid_point(4*A));  // Original modulus.
  known_gr.add_grid_generator(grid_point(7*A));  // Transformation modulus.

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A, EQUAL, A - C + 2, 1, 5) ***");

  return ok;
}

// Equality expression comparing
// generalized_affine_image and affine_image
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((B %= 0) / 3);
  gr.add_congruence((A - 2*C %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A, EQUAL, A - C + 2, 1, 0);

  Grid known_gr = Grid(3);
  known_gr.add_congruence((B %= 0) / 3);
  known_gr.add_congruence((A - 2*C %= 0) / 2);

  known_gr.affine_image(A, A - C + 2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A, EQUAL, A - C + 2, 1, 0) ***");

  return ok;
}

// Empty grid.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(5, EMPTY);

  print_generators(gr, "*** gr ***");

  gr.generalized_affine_image(A, EQUAL, A - 2*C + 3, 4, 7);

  Grid known_gr(5, EMPTY);

  bool ok = (gr == known_gr);

  print_generators(gr,
		   "*** gr.generalized_affine_image(A, EQUAL, A - 2*C + 3, 4, 7) ***");

  return ok;
}

// Empty with congruences.
bool
test11() {
  Variable A(0);

  Grid gr(1);
  gr.add_constraint(A == 0);
  gr.add_constraint(A == 3);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A, EQUAL, A + 2, 1, 1);

  Grid known_gr(1, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A, EQUAL, A + 2, 1, 1) ***");

  return ok;
}

// Universe.
bool
test12() {
  Variable A(0);

  Grid gr(1);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A, EQUAL, A + 2, 1, 1);

  Grid known_gr(1);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A, EQUAL, A + 2, 1, 1) ***");

  return ok;
}

// Zero denominator.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.generalized_affine_image(B, EQUAL, A + 2, 0, 1);
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
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.generalized_affine_image(B, EQUAL, D + 2, 1, 1);
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
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);
  gr.add_congruence((A ==  0) / 0);

  try {
    gr.generalized_affine_image(D, EQUAL, A + 2, 1, 1);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Expressions having common variables.
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((C %= 0) / 3);
  gr.add_constraint(A - 2*B == 1);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A - B + C, EQUAL, 2*A - B - C, 5);

  Grid known_gr(3);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A - B + C, EQUAL, 2*A - B - C, 5) ***");

  return ok;
}

// Expressions having common variables, where generalized_affine_image
// must minimize the grid.
bool
test17() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A - B, EQUAL, 2*A - 2*B, 5);

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 0) / 5);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A - B, EQUAL, 2*A - 2*B, 5) ***");

  return ok;
}

// Expressions having common variables.
bool
test18() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(2*A - 2*B, EQUAL, A - B, 5);

  Grid known_gr(2);
  known_gr.add_congruence((2*A - 2*B %= 0) / 5);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(2*A - 2*B, EQUAL, A - B, 5) ***");

  return ok;
}

// Right hand side expression of greater space dimension than the
// grid.
bool
test19() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence(C %= -2);

  try {
    gr.generalized_affine_image(B + C, EQUAL, D + 2, 1);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Left hand side expression of space dimension greater than the grid.
bool
test20() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);

  try {
    gr.generalized_affine_image(A + D, EQUAL, A + 2, 1);
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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
END_MAIN
