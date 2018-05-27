/* Test Grid::congruence_widening_assign().
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

// Initially empty.
bool
test01() {
  Variable A(0);

  Grid gr1(2, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  gr2.congruence_widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

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

  Grid known_gr = gr2;

  gr2.congruence_widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Both empty.
bool
test03() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(A == 1);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1, EMPTY);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  gr2.congruence_widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// An equality that becomes a congruence.
bool
test04() {
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

  gr2.congruence_widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Keep all congruences.
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
  gr2.add_congruence((A %= 0) / 2);
  gr2.add_congruence(C %= 0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  gr2.congruence_widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Keep some congruences.
bool
test06() {
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

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(3);
  known_gr.add_congruence((B %= 0) / 2);
  known_gr.add_congruence(C %= 0);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Already minimized.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);
  gr1.add_congruence((C %= 0) / 3);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((B %= 0) / 2);
  gr2.add_congruence(C %= 0);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(3);
  known_gr.add_congruence((B %= 0) / 2);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Both universe.
bool
test08() {
  Grid gr1(5);
  Grid gr2(5);

  print_congruences(gr1, "*** gr1 ***");

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(5);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Widening when increasing to universe.
bool
test09() {
  Variable A(0);

  Grid gr1(4);
  gr1.add_congruence((A %= 0) / 4);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(4);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(4);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Tokens.
bool
test10() {
  using namespace IO_Operators;

  Variable A(0);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 4);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);

  print_congruences(gr2, "*** gr2 ***");

  unsigned int tokens = 4;

  Grid known_gr = gr2;

  gr2.congruence_widening_assign(gr1, &tokens);

  bool ok = ((gr2 == known_gr) && (tokens == 3));

  nout << "*** `tokens' should be 3 ***" << tokens << endl;

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1, &tokens) ***");

  return ok;
}

// Zero dimension.
bool
test11() {
  Grid gr1(0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(0);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// First case of Example 8 in Section 6 of [BagnaraDHMZ05TR].
bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(5*A + B %= 0);
  gr1.add_congruence(22*A %= 0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(5*A + B %= 0);
  gr2.add_congruence(44*A %= 0);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(2);
  known_gr.add_congruence(5*A + B %= 0);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Second case in Example 8 in Section 6 of [BagnaraDHMZ05TR].
bool
test13() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(9*A + B %= 0);
  gr1.add_congruence(22*A %= 0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(9*A + B %= 0);
  gr2.add_congruence(44*A %= 0);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(2);
  known_gr.add_congruence(9*A + B %= 0);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// First case in Example 9 in Section 6 of [BagnaraDHMZ05TR].
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((A + B %= 0) / 2);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 2);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Second case in Example 9 in Section 6 of [BagnaraDHMZ05TR]
bool
test15() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((3*A + B %= 0) / 2);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 2);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// Congruences and equalities.
bool
test16() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((A - B == 0) / 6);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((A - B == 0) / 3);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(2);
  known_gr.add_congruence((A - B == 0) / 3);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}

// From generators.
bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(C, 3));
  gr1.add_grid_generator(grid_point(C + A - 2*B, 3));

  print_generators(gr1, "*** gr1 ***");

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(C, 3));
  gr2.add_grid_generator(grid_point(2*C + A - 2*B, 6));

  print_generators(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(C, 3));
  known_gr.add_grid_generator(grid_line(A - 2*B));

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

  return ok;
}


// Space dimension exception.
bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(C, 3));
  gr1.add_grid_generator(grid_point(C + A - 2*B, 3));

  print_generators(gr1, "*** gr1 ***");

  Grid gr2(4, EMPTY);
  gr2.add_grid_generator(grid_point(C, 3));
  gr2.add_grid_generator(grid_point(2*C + A - 2*B, 6));

  print_generators(gr2, "*** gr2 ***");

  try {
    gr2.congruence_widening_assign(gr1);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Minimizing the first congruence system finds the empty grid.
bool
test19() {
  Variable A(0);

  Grid gr1(3);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(A == 1);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_constraint(A == 0);
  gr2.add_constraint(A == 1);

  print_congruences(gr2, "*** gr2 ***");

  gr2.congruence_widening_assign(gr1);

  Grid known_gr(3, EMPTY);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.congruence_widening_assign(gr1) ***");

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
