/* Test Grid::intersection_assign().
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

// Simple grids, one dimensions.
bool
test01() {
  Variable A(0);

  Congruence_System cgs1;
  cgs1.insert((A %= 1) / 2);

  Congruence_System cgs2;
  cgs2.insert((A %= 0) / 3);

  Grid gr1(cgs1);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(cgs2);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 3) / 6);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr.intersection_assign(gr2) ***");

  return ok;
}

// Simple grids, many dimensions.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs1;
  cgs1.insert((A + 0*C %= 0) / 2);

  Congruence_System cgs2;
  cgs2.insert((B + 0*C %= 0) / 2);

  Grid gr1(cgs1);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(cgs2);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((A + 0*C %= 0) / 2);
  known_cgs.insert((B %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr.intersection_assign(gr2) ***");

  return ok;
}

// First grid empty.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((B + 0*C %= 0) / 2);

  Grid gr1(3, EMPTY);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(cgs);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Grid known_gr(3, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr.intersection_assign(gr2) ***");

  return ok;
}

// Second grid empty.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((B + 0*C %= 0) / 2);

  Grid gr1(cgs);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(3, EMPTY);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Grid known_gr(3, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.intersection_assign(gr2) ***");

  return ok;
}

// First grid universe.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((5*A + 3*B + C %= 7) / 9);

  Grid gr1(3, UNIVERSE);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(cgs);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((5*A + 3*B + C %= 7) / 9);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.intersection_assign(gr2) ***");

  return ok;
}

// Second grid universe.
bool
test06() {
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((2*B + 2*C %= 1) / 3);

  Grid gr1(cgs);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(3, UNIVERSE);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((2*B + 2*C %= 1) / 3);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.intersection_assign(gr2) ***");

  return ok;
}

// Zero dimension grids.

bool
test07() {
  Congruence_System cgs;
  Linear_Expression le;
  cgs.insert((le %= le) / 1);	// (0 %= 0) / 1

  Grid gr1(cgs);
  print_congruences(gr1, "*** gr1 ***");
  Grid gr2(0, UNIVERSE);
  print_congruences(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Grid known_gr(cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.intersection_assign(gr2) ***");

  return ok;
}

// Many dimensioned grids from generators.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs1;
  gs1.insert(grid_point(A + C));
  gs1.insert(grid_line(C));

  Grid_Generator_System gs2;
  gs2.insert(grid_point(A));
  gs2.insert(grid_point(A + B));

  Grid gr1(gs1);
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(3, EMPTY);
  gr2.add_grid_generators(gs2);
  print_generators(gr2, "*** gr2 ***");

  gr1.intersection_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((C == 0) / 0);
  known_cgs.insert((A == 1) / 0);
  known_cgs.insert((B == 0) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.intersection_assign(gr2) ***");

  return ok;
}

// Space dimension exception.
bool
test09() {
  Grid gr1(1, EMPTY);
  gr1.add_grid_generator(grid_point());

  Grid gr2(3);

  try {
    gr1.intersection_assign(gr2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Both empty and both not in minimal form.
bool
test10() {
  Variable A(0);
  Grid gr1(1);
  gr1.add_congruence((A %= 1) / 2);
  gr1.add_congruence((A %= 0) / 2);

  Grid gr2(1);
  gr2.add_congruence((A %= 1) / 2);
  gr2.add_congruence((A %= 0) / 2);

  gr1.intersection_assign(gr2);
  bool ok = (gr1.is_empty());

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

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
END_MAIN
