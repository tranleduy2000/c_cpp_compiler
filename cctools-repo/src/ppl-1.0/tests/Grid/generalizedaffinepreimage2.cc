/* Test Grid::generalized_affine_preimage(lhs, rhs, modulus).
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

// Expressions both constants.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(Linear_Expression::zero(), EQUAL,
				 Linear_Expression(1), 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(Linear_Expression::zero(), EQUAL, Linear_Expression(1), 1) ***");

  return ok;
}

// Left hand side constant.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(Linear_Expression::zero(), EQUAL, A - B, 5);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A + B));
  known_gr.add_grid_generator(grid_point(5*A));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(Linear_Expression::zero(), EQUAL, A - B, 5) ***");

  return ok;
}

// Expressions with unique variables.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(A - B == 0);
  gr.add_congruence((C %= 0) / 3);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A - B, EQUAL, C, 1);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A));
  known_gr.add_grid_generator(grid_line(B));
  known_gr.add_grid_generator(grid_point(3*C));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Simple expressions having common variables.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A - B, EQUAL, A, 0);

  Grid known_gr(2);
  known_gr.add_constraint(A == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A - B, EQUAL, A, 0) ***");

  return ok;
}

// Expressions having common variables.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 1);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A + 2*B, EQUAL, A - B, 3);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(A + B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Expressions having common variables.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((C %= 0) / 3);
  gr.add_constraint(A - 2*B == 1);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A - B + C, EQUAL, 2*A - B - C, 5);

  Grid known_gr(3);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A - B + C, EQUAL, 2*A - B - C, 5) ***");

  return ok;
}

// Expressions having common variables, where
// generalized_affine_preimage must minimize the grid.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A - B, EQUAL, 2*A - 2*B, 5);

  Grid known_gr(2);
  known_gr.add_congruence((2*A - 2*B %= 0) / 5);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A - B, EQUAL, 2*A - 2*B, 5) ***");

  return ok;
}

// Expressions having common variables, where
// generalized_affine_preimage must minimize the grid.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(2*A - 2*B, EQUAL, A - B, 5);

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 0) / 5);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(2*A - 2*B, EQUAL, A - B, 5) ***");

  return ok;
}

// Right hand side expression of greater space dimension than the
// grid.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence(C %= -2);

  try {
    gr.generalized_affine_preimage(B + C, EQUAL, D + 2, 1);
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
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(3);
  gr.add_congruence((C == -2) / 0);

  try {
    gr.generalized_affine_preimage(A + D, EQUAL, A + 2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Expressions having common variables, with a negative modulus.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((C %= 0) / 3);
  gr.add_constraint(A - B == 0);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A - B, EQUAL, C, -5);

  Grid known_gr(3);
  known_gr.add_congruence((C %= 0) / 15);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A - B, EQUAL, C, -5) ***");

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
END_MAIN
