/* Test Grid::frequency().
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

  Coefficient num1;
  Coefficient den1;
  Coefficient valn1;
  Coefficient vald1;
  Coefficient num2;
  Coefficient den2;
  Coefficient valn2;
  Coefficient vald2;
  bool ok = (!gr1.frequency(A, num1, den1, valn1, vald1)
             && !gr2.frequency(A, num2, den2, valn2, vald2));
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// 0-dimension grids.
bool
test02() {
  Grid gr1(0);

  Grid gr2(0, EMPTY);

  Coefficient num1;
  Coefficient den1;
  Coefficient valn1;
  Coefficient vald1;
  Coefficient num2;
  Coefficient den2;
  Coefficient valn2;
  Coefficient vald2;
  bool ok = (gr1.frequency(Linear_Expression(0), num1, den1, valn1, vald1)
             && num1 == 0 && den1 == 1 && valn1 == 0 && vald1 == 1
             && !gr2.frequency(Linear_Expression(0), num2, den2, valn2, vald2));
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Non-relational test.
bool
test03() {
  Variable A(0);

  Grid gr(1);
  gr.add_congruence(A %= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(A), num, den, valn, vald)
             && num == 1 && den == 1 && valn == 0 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!gr.frequency(Linear_Expression(B), num, den, valn, vald));
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(A + B - 3), num, den, valn, vald)
             && num == 1 && den == 1 && valn == 0 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((B %= 1) / 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(2*A + B), num, den, valn, vald)
             && num == 2 && den == 1 && valn == 1 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence((B %= 1) / 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(2*A + 3*B), num, den, valn, vald)
             && num == 2 && den == 1 && valn == 1 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(3*A %= 0);
  gr.add_congruence((B %= 0) / 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(2*A + 3*B - 1),
                          num, den, valn, vald)
             && num == 2 && den == 3 && valn == -1 && vald == 3);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 1) / 2);
  gr.add_congruence((B %= 1) / 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(3*A + 2*B + 5),
                          num, den, valn, vald)
             && num == 2 && den == 1 && valn == 0 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test10() {
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

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(3*A - B + 2*C - 17),
                          num, den, valn, vald)
             && num == 5 && den == 1 && valn == -1 && vald == 1);

  Grid known_gr(gr);
  known_gr.add_congruence((3*A - B + 2*C - 17 %= -1) / 5);
  print_congruences(gr, "*** gr ***");

  return ok && (gr == known_gr);
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point((2*A - B), 4));
  gr.add_grid_generator(parameter(B));

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(A),
                          num, den, valn, vald)
             && num == 0 && den == 1 && valn == 1 && vald == 2);

  Grid known_gr(gr);
  known_gr.add_congruence((2*A %= 1) / 0);
  print_congruences(gr, "*** gr ***");

  return ok && (gr == known_gr);
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A == 0);
  gr.add_congruence(B %= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(A), num, den, valn, vald)
             && num == 0 && den == 1 && valn == 0 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A == 0);
  gr.add_constraint(B == 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(A), num, den, valn, vald)
             && num == 0 && den == 1 && valn == 0 && vald == 1);
  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 1);
  gr.add_congruence((2*A - B %= 0) / 2);
  print_congruences(gr, "*** gr ***");

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (gr.frequency(Linear_Expression(B), num, den, valn, vald)
             && num == 2 && den == 1 && valn == 0 && vald == 1);
  print_congruences(gr, "*** gr ***");
  nout << num << den;
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
END_MAIN
