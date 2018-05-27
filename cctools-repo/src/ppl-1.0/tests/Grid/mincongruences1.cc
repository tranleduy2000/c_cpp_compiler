/* Test Grid::minimized_congruences().
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

// Empty grid.
bool
test01() {
  Grid gr1(4, EMPTY);
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.minimized_congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Universe grid.
bool
test02() {
  Grid gr1(6);

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.minimized_congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Zero dimension empty grid.
bool
test03() {
  Grid gr1(0, EMPTY);
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.minimized_congruences();

  gr1.OK();

  print_congruences(gr1, "*** gr1(cgs) ***");

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Zero dimension universe grid.
bool
test04() {
  Grid gr1(0);
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.minimized_congruences();

  gr1.OK();

  print_congruences(gr1, "*** gr1(cgs) ***");

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Skew grid in 3D.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A - B %= 3) / 7);
  gr1.add_congruence((A - B %= 3) / 14);
  gr1.add_congruence((A %= 0) / 5);
  gr1.add_congruence((A %= 0) / 10);

  Grid known_gr = gr1;

  Congruence_System cgs = gr1.minimized_congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Get a reference to the minimized_congruences, empty the grid, use the
// reference to create a new grid.
bool
test06() {
  Grid gr1(3);
  gr1.add_congruence(Congruence::zero_dim_integrality());
  print_congruences(gr1, "*** gr1 ***");

  const Congruence_System& cgs = gr1.minimized_congruences();

  // Empty the grid.  The idea is to check that `cgs' still refers to
  // a congruence system that matches the grid.
  gr1.add_congruence(Congruence::zero_dim_false());

  Grid known_gr = gr1;

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// In zero dimensions get a reference to the universe minimized_congruences,
// empty the grid, use the reference to create a new grid.
bool
test07() {
  Grid gr1(0);
  gr1.add_congruence(Congruence::zero_dim_integrality());
  print_congruences(gr1, "*** gr1 ***");

  const Congruence_System& cgs = gr1.minimized_congruences();

  // Empty the grid.  The idea is to check that `cgs' still refers to
  // a congruence system that matches the grid.
  gr1.add_congruence(Congruence::zero_dim_false());

  Grid known_gr = gr1;

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

  return ok;
}

// Empty grid, where minimizing the congruences finds the grid empty.
bool
test08() {
  Variable A(0);

  Grid gr1(4);
  gr1.add_constraint(A == 1);
  gr1.add_constraint(A == 0);
  print_congruences(gr1, "*** gr1 ***");

  Grid known_gr(4, EMPTY);

  Congruence_System cgs = gr1.minimized_congruences();

  Grid gr2(cgs);

  bool ok = (gr2 == known_gr);

  print_congruences(gr2, "*** gr2(cgs) ***");

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
END_MAIN
