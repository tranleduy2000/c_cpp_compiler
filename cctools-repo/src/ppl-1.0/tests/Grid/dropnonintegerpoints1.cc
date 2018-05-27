/* Test Grid::drop_some_non_integer_points().
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

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

// Universe and empty grid.
bool
test01() {
  Variable A(0);

  Grid gr1(1);

  Grid gr2(1, EMPTY);

  gr1.drop_some_non_integer_points(ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(ANY_COMPLEXITY);
  Grid known_gr1(1);
  known_gr1.add_congruence(A %= 0);
  bool ok = (gr2.is_empty() && gr1 == known_gr1);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// 0-dimension grids.
bool
test02() {
  Grid gr1(0);

  Grid gr2(0, EMPTY);

  gr1.drop_some_non_integer_points(ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(ANY_COMPLEXITY);
  bool ok = (gr2.is_empty() && gr1.is_universe());
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Non-relational test.
bool
test03() {
  Variable A(0);

  Grid gr1(1);
  Grid gr2(1);
  gr1.add_congruence(2*A %= 0);
  gr2.add_congruence((A %= 1) / 2);
  Grid known_gr1(1);
  known_gr1.add_congruence(A %= 0);
  Grid known_gr2(1);
  known_gr2.add_congruence((A %= 1) / 2);

  gr1.drop_some_non_integer_points(ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(ANY_COMPLEXITY);
  bool ok = (gr1 == known_gr1 && gr2 == known_gr2);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  Grid gr2(2);
  gr1.add_congruence(2*A %= 0);
  gr2.add_congruence((A %= 1) / 2);
  Grid known_gr1(2);
  Grid known_gr2(2);
  known_gr1.add_congruence(A %= 0);
  known_gr1.add_congruence(B %= 0);
  known_gr2.add_congruence((A %= 1) / 2);
  known_gr2.add_congruence(B %= 0);

  gr1.drop_some_non_integer_points(ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(ANY_COMPLEXITY);
  bool ok = (gr1 == known_gr1 && gr2 == known_gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 1) / 2);
  gr.add_congruence((B %= 1) / 2);
  Grid known_gr(2);
  known_gr.add_congruence((A %= 1) / 2);
  known_gr.add_congruence((B %= 1) / 2);
  gr.drop_some_non_integer_points(ANY_COMPLEXITY);
  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4, EMPTY);
  gr.add_grid_generator(grid_point((-2*A - C + 2*D), 2));
  gr.add_grid_generator(parameter(10*A, 2));
  gr.add_grid_generator(grid_line(2*B + C));
  gr.add_grid_generator(parameter(5*C, 2));
  gr.add_grid_generator(parameter(10*D, 2));
  Grid known_gr(gr);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence(B %= 0);
  known_gr.add_congruence(C %= 0);
  known_gr.add_congruence(D %= 0);

  gr.drop_some_non_integer_points(ANY_COMPLEXITY);
  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr ***");

  return ok;
}

// Universe and empty grid with variable set A.
bool
test07() {
  Variable A(0);

  Grid gr1(1);

  Grid gr2(1, EMPTY);

  Variables_Set vars;
  vars.insert(A);

  gr1.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  Grid known_gr1(1);
  known_gr1.add_congruence(A %= 0);
  bool ok = (gr2.is_empty() && gr1 == known_gr1);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// 0-dimension grids with empty variable set.

bool
test08() {
  Grid gr1(0);

  Grid gr2(0, EMPTY);

  Variables_Set vars;

  gr1.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  bool ok = (gr2.is_empty() && gr1.is_universe());
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Non-relational test with variable set A.
bool
test09() {
  Variable A(0);

  Grid gr1(1);
  Grid gr2(1);
  gr1.add_congruence(2*A %= 0);
  gr2.add_congruence((A %= 1) / 2);
  Grid known_gr1(1);
  known_gr1.add_congruence(A %= 0);
  Grid known_gr2(1);
  known_gr2.add_congruence((A %= 1) / 2);

  Variables_Set vars;
  vars.insert(A);

  gr1.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  bool ok = (gr1 == known_gr1 && gr2 == known_gr2);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Non-relational test with variable set A.
bool
test10() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  Grid gr2(2);
  gr1.add_congruence(2*A %= 0);
  gr2.add_congruence((A %= 1) / 2);
  Grid known_gr1(2);
  Grid known_gr2(2);
  known_gr1.add_congruence(A %= 0);
  known_gr2.add_congruence((A %= 1) / 2);

  Variables_Set vars;
  vars.insert(A);

  gr1.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  gr2.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  bool ok = (gr1 == known_gr1 && gr2 == known_gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Non-relational test with variable set A.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((B %= 1) / 2);
  Grid known_gr(2);
  known_gr.add_congruence(A %= 1);
  known_gr.add_congruence((B %= 1) / 2);

  Variables_Set vars;
  vars.insert(A);

  gr.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Relational test with variable set {A, C}.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4, EMPTY);
  gr.add_grid_generator(grid_point((-2*A - C + 2*D), 2));
  gr.add_grid_generator(parameter(10*A, 2));
  gr.add_grid_generator(grid_line(2*B + C));
  gr.add_grid_generator(parameter(5*C, 2));
  gr.add_grid_generator(parameter(10*D, 2));
  Grid known_gr(gr);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence(C %= 0);

  Variables_Set vars;
  vars.insert(A);
  vars.insert(C);

  gr.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr ***");

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
END_MAIN
