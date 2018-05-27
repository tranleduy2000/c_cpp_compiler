/* Test Grid::upper_bound_assign().
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

bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs1;
  gs1.insert(grid_point(A + 0*C));

  Grid_Generator_System gs2;
  gs2.insert(grid_point(2*A + 0*C));

  Grid gr1(gs1);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point(A + 0*C));
  known_gs.insert(grid_point(2*A));

  Grid known_gr(known_gs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// upper_bound_assign and upper_bound_assign_if_exact - Two empty grids.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(4, EMPTY);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(4, EMPTY);
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr(4, EMPTY);

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - First grid empty.
bool
test04() {
  Variable A(0);

  Grid gr1(4, EMPTY);
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(4, EMPTY);
  gr2.add_grid_generator(grid_point(2*A));
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - Second grid empty.
bool
test05() {
  Variable B(1);

  Grid gr1(4, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(grid_line(B));
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(4, EMPTY);
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr = gr1;

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - Zero dimension universes.
bool
test06() {
  Grid gr1(0);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(0);
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr = gr1;

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - First included in second.
bool
test07() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_congruence((A %= 0) / 2);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence(A %= 0);
  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - Second included in first.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence(A - B %= 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_constraint(A - B == 0);
  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr1;

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - Exact join.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((B %= 0) / 2);
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((A %= 1) / 2);
  gr2.add_congruence((B %= 1) / 2);
  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr(2);
  known_gr.add_congruence((A - B %= 0) / 2);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - Two points (join adds more points).
bool
test10() {
  Variable A(0);

  Grid_Generator_System gs1;
  gs1.insert(grid_point(A));

  Grid_Generator_System gs2;
  gs2.insert(grid_point(3*A, 5));

  Grid gr1(gs1);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr = gr1;

  bool ok = (!gr1.upper_bound_assign_if_exact(gr2));

  if (ok)
    ok &= (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign_if_exact(gr2) ***");

  return ok;
}

// upper_bound_assign_if_exact - Space dimension exception.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(B + 0*C));

  Grid gr1(gs);

  Grid gr2(4);
  gr2.add_constraint(A == 0);
  gr2.add_constraint(B == 0);
  gr2.add_constraint(C == 0);

  try {
    gr1.upper_bound_assign_if_exact(gr2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// upper_bound_assign - Divisor normalization.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs1;
  gs1.insert(grid_point(0*C));
  gs1.insert(grid_line(A));
  gs1.insert(grid_line(B));

  Grid gr1(gs1);
  print_generators(gr1, "*** gr1 ***");

  Grid_Generator_System gs2;
  gs2.insert(grid_point());
  gs2.insert(grid_point(C, 3));

  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((3*C %= 0) / 1);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
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
