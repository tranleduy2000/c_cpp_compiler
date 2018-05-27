/* Test Grid::widening_assign().
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

  gr2.widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// Both empty.
bool
test02() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(A == 1);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1, EMPTY);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  gr2.widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// Zero dimension.
bool
test03() {
  Grid gr1(0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0);

  print_congruences(gr2, "*** gr2 ***");

  gr2.widening_assign(gr1);

  Grid known_gr(0);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// The next set of tests use the widening example in fm06.

// Both congruence systems are up-to-date.
// Neither generator systems are up-to-date.
// Should use the congruence widening..
bool
test04() {
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

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 2);

  gr2.widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// Initial grid - only the congruence system is up-to-date but not minimized.
// Second grid - only the generator system is up-to-date but not minimized.
// Should use the congruence widening..
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);
  gr1.add_congruence((A + B %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(parameter(2*A));
  gr2.add_grid_generator(parameter(A + B));
  gr2.add_grid_generator(parameter(3*A + B));

  print_generators(gr2, "*** gr2 ***");

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 2);

  gr2.widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// Initial grid - only the generator system is up-to-date but not minimized.
// Second grid - only the congruence system is up-to-date but not minimized.
// Should use the congruence widening..
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(parameter(2*A));
  gr1.add_grid_generator(parameter(2*B));
  gr1.add_grid_generator(parameter(2*A + 2*B));

  print_generators(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  gr2.add_congruence((A + B %= 0) / 2);
  gr2.add_congruence((A + B %= 0));

  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 2);

  gr2.widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// Neither congruence systems are up-to-date.
// Should use the generator widening..
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(parameter(2*A));
  gr1.add_grid_generator(parameter(2*B));

  print_generators(gr1, "*** gr1 ***");

  Grid gr2(2, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(parameter(2*A));
  gr2.add_grid_generator(parameter(A + B));

  print_generators(gr2, "*** gr2 ***");

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 0) / 2);

  gr2.widening_assign(gr1);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2.widening_assign(gr1) ***");

  return ok;
}

// Tokens.
bool
test08() {
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

  gr2.widening_assign(gr1, &tokens);

  bool ok = ((gr2 == known_gr) && (tokens == 3));

  nout << "*** `tokens' which should be should be 3 are *** "
       << tokens << endl;

  print_congruences(gr2, "*** gr2.widening_assign(gr1, &tokens) ***");

  return ok;
}

// Space dimension exception.
bool
test09() {
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
    gr2.widening_assign(gr1);
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
END_MAIN
