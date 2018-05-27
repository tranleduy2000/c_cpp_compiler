/* Test class Grid_Certificate.
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

// Compare a grid to one that is more constrained (due to equalities).
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence(A + C %= 0);
  gr1.add_constraint(B == 3);

  Grid_Certificate grc1(gr1);

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(3*B + A));
  gr2.add_grid_generator(grid_point(3*B + A + C));

  Grid_Certificate grc2(gr2);

  bool ok = grc1.compare(grc2) == -1;

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Compare a grid to one that is more constrained (due to proper
// congruences).
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence(A + C %= 0);
  gr1.add_constraint(B == 3);

  Grid_Certificate grc1(gr1);

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(3*B + A));
  gr2.add_grid_generator(grid_point(3*B + A + C));

  bool ok = grc1.compare(gr2) == -1;

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Compare a grid to an equally constrained one.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence(A + C %= 0);
  gr1.add_constraint(B == 3);

  Grid_Certificate grc1(gr1);

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(3*B));
  gr2.add_grid_generator(grid_line(A - C));
  gr2.add_grid_generator(grid_point(3*B + A));

  Grid_Certificate grc2(gr2);

  bool ok = grc1.compare(grc2) == 0
    && !grc1.is_stabilizing(gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Compare a grid to one that is less constrained (due to equalities).
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*B + A));
  gr1.add_grid_generator(grid_point(3*B + A + C));

  Grid_Certificate grc1(gr1);

  Grid gr2(3);
  gr2.add_congruence(A + C %= 0);
  gr2.add_constraint(B == 3);

  bool ok = grc1.compare(gr2) == 1
    && grc1.is_stabilizing(gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Compare a grid to one that is less constrained (due to proper
// congruences).
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence((A + C %= 0) / 2);
  gr1.add_congruence((B %= 0) / 3);
  gr1.add_congruence(A %= 0);

  Grid_Certificate grc1(gr1);

  Grid gr2(3);
  gr2.add_congruence((A + C %= 0) / 2);
  gr2.add_congruence((B %= 0) / 3);

  Grid_Certificate grc2(gr2);

  Grid_Certificate::Compare cmp;

  bool ok = cmp(grc1, grc2)
    && grc1.is_stabilizing(gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Compare certificates for zero dimension universe grids.
bool
test06() {
  Grid gr1(0);

  Grid_Certificate grc1(gr1);

  Grid gr2(0);

  Grid_Certificate grc2(gr2);

  bool ok = (grc1.compare(grc2) == 0);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Compare a grid to one that is more constrained, where the minimized
// generators are used for the comparison.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence(A + C %= 0);
  gr1.add_constraint(B == 3);

  Grid_Certificate grc1(gr1);

  Grid gr2(3, EMPTY);
  gr2.add_grid_generator(grid_point(3*B + A + C));
  gr2.add_grid_generator(grid_point(3*B + A));

  // Ensure up to date congruences and minimized generators.
  gr2.affine_image(A, 1*A);
  gr2.minimized_grid_generators();

  bool ok = (grc1.compare(gr2) == -1);

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
END_MAIN
