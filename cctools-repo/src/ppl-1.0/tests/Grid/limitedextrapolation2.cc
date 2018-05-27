/* Test Grid::limited_generator_extrapolation_assign().
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

// These tests are clones of tests in limitedextrapolation1.cc but
// using the generator_widening_assign() instead of the
// congruence_widening_assign().

namespace {

// Initially empty.
bool
test01() {
  Variable A(0);

  Grid gr1(2, EMPTY);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  print_congruences(gr2, "*** gr2 ***");

  Congruence_System cgs((A %= 0) / 2);

  Grid known_gr = gr2;

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Empty after minimization.
bool
test02() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(A == 1);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence(A %= 0);
  print_congruences(gr2, "*** gr2 ***");

  Congruence_System cgs((A %= 0) / 3);

  Grid known_gr = gr2;

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Equivalent of just widening.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(B == 1);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_constraint(A == 0);
  gr2.add_congruence(B %= 1);
  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  Congruence_System cgs(B %= 0);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Keep all congruences, including a limiting congruence.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);
  gr1.add_congruence(C %= 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_congruence((A %= 0) / 2);
  gr2.add_congruence(C %= 0);
  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  Congruence_System cgs(C %= 0);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Keep some congruences.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);
  gr1.add_congruence(C %= 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((B %= 0) / 2);
  gr2.add_congruence(C %= 0);
  print_congruences(gr2, "*** gr2 ***");

  Congruence_System cgs(A + 0*C %= 0);

  Grid known_gr = gr2;

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Both universe.
bool
test06() {
  Variable E(4);

  Grid gr1(5);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(5);

  Congruence_System cgs(0*E %= 0);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  Grid known_gr(5);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Keeping many limiting congruences.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 4);
  gr1.add_congruence((B %= 0) / 4);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((A %= 0) / 2);
  gr2.add_congruence(B %= 0);
  print_congruences(gr2, "*** gr2 ***");

  Congruence_System cgs;
  cgs.insert(A %= 0);
  cgs.insert(B %= 0);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  Grid known_gr(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Tokens.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 4);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  print_congruences(gr2, "*** gr2 ***");

  unsigned int tokens = 6;

  Congruence_System cgs;
  cgs.insert((A + 0*B %= 0) / 2);

  Grid known_gr = gr2;

  gr2.limited_generator_extrapolation_assign(gr1, cgs, &tokens);

#undef TOKENS
#define TOKENS 5

  bool ok = (gr2 == known_gr) && (tokens == TOKENS);

  nout << "tokens: before " << TOKENS << ", after " << tokens << endl;

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs, &tokens) ***");

  return ok;
}

// 0 tokens.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 4);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  print_congruences(gr2, "*** gr2 ***");

#undef TOKENS
#define TOKENS 0

  unsigned int tokens = TOKENS;

  Congruence_System cgs;
  cgs.insert((A + 0*B %= 0) / 2);

  Grid known_gr(2);

  gr2.limited_generator_extrapolation_assign(gr1, cgs, &tokens);

  bool ok = (gr2 == known_gr) && (tokens == TOKENS);

  nout << "tokens: before " << TOKENS << ", after " << tokens << endl;

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs, &tokens) ***");

  return ok;
}

// Zero dimension.
bool
test10() {
  Grid gr1(0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0);
  print_congruences(gr2, "*** gr2 ***");

  Congruence_System cgs(
     (Linear_Expression::zero() %= Linear_Expression::zero()) / 4);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  Grid known_gr(0);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Congruences and equalities.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence((A - B == 0) / 27);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((A - B == 0) / 9);
  print_congruences(gr2, "*** gr2 ***");

  Congruence_System cgs;
  cgs.insert((A - B == 0) / 3);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  Grid known_gr(2);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence((A - B == 0) / 3);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// From generators, with a limiting equality.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(C, 3));
  gr1.add_grid_generator(grid_line(A - 2*B));
  gr1.add_grid_generator(grid_point(C + 3*A + 3*C, 3));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(C, 3));
  gr2.add_grid_generator(grid_line(A - 2*B));
  gr2.add_grid_generator(grid_line(A + C));
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  Congruence_System cgs;
  cgs.insert(A - 0*C == 3);

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

  return ok;
}

// Exception due to space dimensions of grids.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(C, 3));
  gr1.add_grid_generator(grid_point(C + A - 2*B, 3));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(5, EMPTY);
  gr2.add_grid_generator(grid_point(C, 3));
  gr2.add_grid_generator(grid_point(2*C + A - 2*B, 6));
  print_generators(gr2, "*** gr2 ***");

  Congruence_System cgs;
  cgs.insert(A - 0*C == 3);

  try {
    gr2.limited_generator_extrapolation_assign(gr1, cgs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Exception due to space dimension of congruence system.
bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(2, EMPTY);
  gr1.add_grid_generator(grid_point(A));
  gr1.add_grid_generator(parameter(B, 3));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(2, EMPTY);
  gr2.add_grid_generator(grid_point(A));
  gr2.add_grid_generator(parameter(B, 6));
  print_generators(gr2, "*** gr2 ***");

  Congruence_System cgs;
  cgs.insert(A - 0*C == 3);

  try {
    gr2.limited_generator_extrapolation_assign(gr1, cgs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Limit with an empty congruence system.
bool
test15() {
  Variable A(0);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(parameter(A, 3));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(parameter(A, 6));
  print_generators(gr2, "*** gr2 ***");

  Congruence_System cgs;

  gr2.limited_generator_extrapolation_assign(gr1, cgs);

  Grid known_gr = gr2;
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A));

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.limited_generator_extrapolation_assign(gr1, cgs) ***");

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
END_MAIN
