/* Test Grid::generalized_affine_image(lhs, rhs, modulus).
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

// Tests 1 to 13 are equivalent to tests 1 to 13 in
// generalizedaffineimage1.cc.

// Simplest expression.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(1*B, EQUAL, Linear_Expression::zero(), 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(1*B, Linear_Expression::zero(), 0) ***");

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

  gr.generalized_affine_image(2*B, EQUAL, Linear_Expression::zero(), 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(B, 2));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(2*B, Linear_Expression::zero()) ***");

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

  gr.generalized_affine_image(1*B, EQUAL, A + 1, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A - B));
  known_gr.add_grid_generator(grid_point(B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(1*B, A + 1) ***");

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

  gr.generalized_affine_image(2*B, EQUAL, A + 1, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(B, 2));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(2*B, A + 1) ***");

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

  gr.generalized_affine_image(2*B, EQUAL, A + 1, 3);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(B, 2));
  known_gr.add_grid_generator(grid_point(A + B));
  known_gr.add_grid_generator(grid_point(2*B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(2*B, A + 1, 3) ***");

  return ok;
}

// Simple expression, with denominator and modulus.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A + B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(2*B, EQUAL, A + 1, 3);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(B, 2));
  known_gr.add_grid_generator(grid_point(A + B));
  known_gr.add_grid_generator(grid_point(2*B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(2*B, A + 1, 3) ***");

  return ok;
}

// Negative denominator.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A - B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(-2*B, EQUAL, A + 2, 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(-B, 2));
  known_gr.add_grid_generator(grid_point(2*A + -3*B, 2));
  known_gr.add_grid_generator(grid_point(-B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(-2*B, A + 2) ***");

  return ok;
}

// Negative modulus.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A - B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(1*B, EQUAL, A + 2, -7);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(2*B));
  known_gr.add_grid_generator(grid_point(A + 3*B));
  known_gr.add_grid_generator(grid_point(9*B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(1*B, A + 2, -7) ***");

  return ok;
}

// Expression of many variables.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((B %= 0) / 3);
  gr.add_congruence((A - 2*C %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(1*A, EQUAL, A - C + 2, 5);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(2*A));
  known_gr.add_grid_generator(grid_point(2*A + 3*B));
  known_gr.add_grid_generator(grid_line(A + C));
  known_gr.add_grid_generator(grid_point(4*A));  // Original modulus.
  known_gr.add_grid_generator(grid_point(7*A));  // Transformation modulus.

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(1*A, A - C + 2, 5) ***");

  return ok;
}

// Equality expression.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((B %= 0) / 3);
  gr.add_congruence((A - 2*C %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(1*A, EQUAL, A - C + 2, 0);

  Grid known_gr = Grid(3);
  known_gr.add_congruence((B %= 0) / 3);
  known_gr.add_congruence((A - 2*C %= 0) / 2);

  known_gr.affine_image(A, A - C + 2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(1*A, A - C + 2, 0) ***");

  return ok;
}

// Empty grid.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(5, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(4*A, EQUAL, A - 2*C + 3, 7);

  Grid known_gr(5, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(1*A, A + 2) ***");

  return ok;
}

// Empty with congruences.
bool
test12() {
  Variable A(0);

  Grid gr(1);
  gr.add_constraint(A == 0);
  gr.add_constraint(A == 3);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(1*A, EQUAL, A + 2, 1);

  Grid known_gr(1, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(1*A, A + 2) ***");

  return ok;
}

// Universe.
bool
test13() {
  Variable A(0);

  Grid gr(1);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(1*A, EQUAL, A + 2, 1);

  Grid known_gr(1);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.generalized_affine_image(1*A, A + 2) ***");

  return ok;
}

// Expressions both constants.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(Linear_Expression::zero(), EQUAL,
			      Linear_Expression(1), 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(Linear_Expression::zero(), EQUAL, Linear_Expression(1), 1) ***");

  return ok;
}

// Left hand side constant.
bool
test15() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(Linear_Expression::zero(), EQUAL, A - B, 5);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A + B));
  known_gr.add_grid_generator(grid_point(5*A));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(Linear_Expression::zero(), EQUAL, A - B, 5) ***");

  return ok;
}

// Expressions with unique variables.
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(A - B == 0);
  gr.add_congruence((C %= 0) / 3);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A - B, EQUAL, C, 1);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A + B));
  known_gr.add_grid_generator(grid_point(B));
  known_gr.add_grid_generator(grid_point(3*C));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A - B, EQUAL, C, 1) ***");

  return ok;
}

// Simple expressions having common variables.
bool
test17() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A - B, EQUAL, A);

  Grid known_gr(2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A - B, EQUAL, A) ***");

  return ok;
}

// Expressions having common variables.
bool
test18() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 1);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A + 2*B, EQUAL, A - B, 3);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(B, 2));
  known_gr.add_grid_generator(grid_line(2*A - B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A + 2*B, EQUAL, A - B, 3) ***");

  return ok;
}

// The equivalent of test1 from Polyhedron/generalizedaffineimage10.cc
// (expressions with unique variables).
bool
test19() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(C == 0);
  gr.add_constraint(A + 3*B == 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_image(A - C, EQUAL, B + 3, 0);

  Grid known_gr(3);
  known_gr.add_constraint(A - C == B + 3);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_image(A - C, EQUAL, B + 3, 0) ***");

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
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
END_MAIN
