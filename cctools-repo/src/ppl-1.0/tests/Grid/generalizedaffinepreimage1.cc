/* Test Grid::generalized_affine_preimage(var, ...).
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

  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence(B %= 0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2 = gr1;

  // Equality expression.
  gr1.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1, 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1, 0 ***");

  if (ok) {
    print_congruences(gr2, "*** gr2 ***");

    // Congruence expression.
    gr2.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1, 2);
    ok = (gr2 == known_gr);
  }

  print_congruences(gr2, "*** gr2.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1, 2) ***");

  return ok;
}

// Simple expression, including positive modulus.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence((B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2 = gr1;

  // Equality expression.
  gr1.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1) ***");

  if (ok) {
    print_congruences(gr2, "*** gr2 ***");

    // Congruence expression.
    gr2.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1, 3);
    ok = (gr2 == known_gr);
  }

  print_congruences(gr2, "*** gr2.generalized_affine_preimage(B, EQUAL, Linear_Expression::zero(), 1, 3) ***");

  return ok;
}

// Simple expression, including negative modulus.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence((B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2 = gr1;

  // Equality expression.
  gr1.generalized_affine_preimage(B, EQUAL, A + 1, 1, 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(-A));
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.generalized_affine_preimage(B, EQUAL, A + 1, 1, 0) ***");

  if (ok) {
    print_congruences(gr2, "*** gr2 ***");

    known_gr.add_grid_generator(grid_point());

    // Congruence expression.
    gr2.generalized_affine_preimage(B, EQUAL, A + 1, 1, -7);
    ok = (gr2 == known_gr);
  }

  print_congruences(gr2, "*** gr2.generalized_affine_preimage(B, EQUAL, A + 1, 1, -7) ***");

  return ok;
}

// Simple expression, with denominator.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence((A + B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2 = gr1;

  // Equality expression.
  gr1.generalized_affine_preimage(B, EQUAL, A + 1, 2, 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(-3*A));
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.generalized_affine_preimage(B, EQUAL, A + 1, 2, 0) ***");

  if (ok) {
    print_congruences(gr2, "*** gr2 ***");

    known_gr.add_grid_generator(grid_point(-A));

    // Congruence expression.
    gr2.generalized_affine_preimage(B, EQUAL, A + 1, 2, 3);
    ok = (gr2 == known_gr);
  }

  print_congruences(gr2, "*** gr2.generalized_affine_preimage(B, EQUAL, A + 1, 2, 3) ***");

  return ok;
}

// Negative denominator.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence((A - B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2 = gr1;

  // Equality expression.
  gr1.generalized_affine_preimage(B, EQUAL, A + 2, -2, 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(-2*A));
  known_gr.add_grid_generator(grid_point(2*A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.generalized_affine_preimage(B, EQUAL, A + 2, -2, 0) ***");

  if (ok) {
    print_congruences(gr2, "*** gr2 ***");

    known_gr.add_grid_generator(grid_point());

    // Congruence expression.
    gr2.generalized_affine_preimage(B, EQUAL, A + 2, -2, 1);
    ok = (gr2 == known_gr);
  }

  print_congruences(gr2, "*** gr2.generalized_affine_preimage(B, EQUAL, A + 2, -2, 1) ***");

  return ok;
}

// Expression of many variables.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((B %= 0) / 3);
  gr.add_congruence((A - 2*C %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A, EQUAL, A - C + 2, 1, 5);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(2*A));
  known_gr.add_grid_generator(grid_point(2*A + 3*B));
  known_gr.add_grid_generator(grid_line(3*A + C));
  known_gr.add_grid_generator(grid_point(4*A));  // Original modulus.
  known_gr.add_grid_generator(grid_point(7*A));  // Transformation modulus.

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A, EQUAL, A - C + 2, 1, 5) ***");

  return ok;
}

// Equality expression.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((B %= 0) / 3);
  gr.add_congruence((A - 2*C %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A, EQUAL, A - C + 2, 1, 0);

  Grid known_gr = Grid(3);
  known_gr.add_congruence((B %= 0) / 3);
  known_gr.add_congruence((A - 2*C %= 0) / 2);

  known_gr.affine_preimage(A, A - C + 2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A, EQUAL, A - C + 2, 1, 0) ***");

  return ok;
}

// Empty grid.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(5, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A, EQUAL, A - 2*C + 3, 4, 7);

  Grid known_gr(5, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A, EQUAL, A - 2*C + 3, 4, 7) ***");

  return ok;
}

// Empty with congruences.
bool
test09() {
  Variable A(0);

  Grid gr(1);
  gr.add_constraint(A == 0);
  gr.add_constraint(A == 3);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A, EQUAL, A + 2, 1, 1);

  Grid known_gr(1, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A, EQUAL, A + 2, 1, 1) ***");

  return ok;
}

// Universe.
bool
test10() {
  Variable A(0);

  Grid gr(1);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(A, EQUAL, A + 2, 1, 1);

  Grid known_gr(1);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A, EQUAL, A + 2, 1, 1) ***");

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
    gr.generalized_affine_preimage(B, EQUAL, A + 2, 0);
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
    gr.generalized_affine_preimage(B, EQUAL, D + 2, 1);
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
    gr.generalized_affine_preimage(D, EQUAL, A + 2, 1);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Expression with a 0 modulus, where the variable occurs in
// the expression.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  // Equality expression.
  gr.generalized_affine_preimage(B, EQUAL, A + B, 1, 0);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(parameter(2*B));
  known_gr.add_grid_generator(parameter(A + B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(B, EQUAL, A + B, 1, 0) ***");

  return ok;
}

// Expression with a negative modulus, where the variable occurs in
// the expression.
bool
test15() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((B %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(B, EQUAL, A + B, 1, -7);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(parameter(2*B));
  known_gr.add_grid_generator(parameter(A + B));
  known_gr.add_grid_generator(parameter(B));

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(B, EQUAL, A + B, 1, -7) ***");

  return ok;
}

// Test similar to test01 in
// tests/Polyhedron/generalizedaffinepreimage2.cc
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(A + B));
  gr.add_grid_generator(grid_point(2*A));
  gr.add_grid_generator(grid_point(2*A + 2*B));
  gr.add_grid_generator(grid_point(3*A + B));

  Grid known_gr(gr);

  print_congruences(gr, "*** gr ***");

  gr.generalized_affine_preimage(B, EQUAL, B+2, 1, 5);

  // A longer way of computing the generalized affine preimage below.
  known_gr.add_space_dimensions_and_embed(1);
  known_gr.add_congruence((B %= C+2) / 5);
  Variables_Set vset;
  vset.insert(B);
  known_gr.remove_space_dimensions(vset);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.generalized_affine_preimage(A + 2*B, EQUAL, A - B, 3) ***");

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
END_MAIN
