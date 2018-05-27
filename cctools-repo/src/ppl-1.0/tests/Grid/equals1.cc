/* Test operator==(const Grid&, const Grid&)
   and operator!=(const Grid&, const Grid&).
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

// Grids the same, gr defined by generators and known_gr by congruences.
bool
test01() {
  Variable A(0);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(3*A));

  Grid gr(gs);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 0) / 3);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");
  print_congruences(known_gr, "*** known_gr ***");

  return ok;
}

// Grids the same, gr defined by congruences and known_gr by generators.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert(A - B %= 0);
  cgs.insert((C %= 0) / 7);

  Grid gr(cgs);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_line(A + B));
  gs.insert(grid_point(B));
  gs.insert(grid_point(7*C));

  Grid known_gr(gs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");
  print_congruences(known_gr, "*** known_gr ***");

  return ok;
}

// Grids differ, gr defined by congruences and known_gr by generators.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert(A - B %= 0);
  cgs.insert((C %= 0) / 7);

  Grid gr(cgs);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_line(A - B));
  gs.insert(grid_point(B));
  gs.insert(grid_point(7*C, 3));

  Grid known_gr(gs);

  bool ok = (gr != known_gr);

  print_congruences(gr, "*** gr ***");
  print_congruences(known_gr, "*** known_gr ***");

  return ok;
}

// Grids differ by the affine dimension and are defined by points.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point(0*A));
  gs.insert(grid_point(1*A));

  Grid gr(gs);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point(0*A + 0*B));
  known_gs.insert(grid_point(1*A + 0*B));
  known_gs.insert(grid_point(0*A + 1*B));

  Grid known_gr(known_gs);

  bool ok = (gr != known_gr);

  print_congruences(gr, "*** gr ***");
  print_congruences(known_gr, "*** known_gr ***");

  return ok;
}

// Where the equality of two grids is decided by comparing the number
// of equalities.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_constraint(A == 0);

  gr1.minimized_congruences();

  Grid gr2(3);
  gr2.add_congruence(B %= 0);

  gr2.minimized_congruences();

  bool ok = (gr1 != gr2);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Where the equality of two grids is decided by comparing the number
// of generators.
bool
test06() {
  Variable A(0);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());

  gr1.minimized_grid_generators();

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(grid_line(A));

  gr2.minimized_grid_generators();

  bool ok = (gr1 != gr2);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Where the equality of two grids is decided by comparing the number
// of lines.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(parameter(B));

  gr1.minimized_grid_generators();

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(grid_line(A));

  gr2.minimized_grid_generators();

  bool ok = (gr1 != gr2);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// No equalities and no lines, number of congruences and generators
// are the same. But the generators are different.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(parameter(A));
  gr1.add_grid_generator(parameter(B));
  gr1.add_grid_generator(parameter(C));

  gr1.minimized_grid_generators();

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(parameter(2*A));
  gr2.add_grid_generator(parameter(2*B));
  gr2.add_grid_generator(parameter(2*C));

  gr2.minimized_grid_generators();

  bool ok = (gr1 != gr2);
  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// No equalities and no lines, number of congruences and generators
// are the same. But the generators are syntactically the same.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point());
  gr1.add_grid_generator(parameter(A));
  gr1.add_grid_generator(parameter(B));
  gr1.add_grid_generator(parameter(C));

  gr1.minimized_grid_generators();

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point());
  gr2.add_grid_generator(parameter(A));
  gr2.add_grid_generator(parameter(B));
  gr2.add_grid_generator(parameter(C));

  gr2.minimized_grid_generators();

  bool ok = (gr1 == gr2);
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
END_MAIN
