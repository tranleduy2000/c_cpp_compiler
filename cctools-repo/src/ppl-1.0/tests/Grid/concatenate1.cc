/* Test Grid::concatenate_assign().
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

// From congruences.
bool
test01() {
  Variable B(1);
  Variable A(0);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);

  Grid gr1(cgs);

  print_congruences(gr1, "*** gr1 ***");

  cgs.clear();
  cgs.insert((A %= 1) / 2);

  Grid gr2(cgs);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 0) / 2);
  known_cgs.insert((B %= 1) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// First grid empty.
bool
test02() {
  Variable A(0);
  Variable C(2);

  Grid gr1(2, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 0) / 2);

  Grid gr2(cgs);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid known_gr(5, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// Second grid empty.
bool
test03() {
  Variable A(0);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 0) / 2);

  Grid gr1(cgs);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, EMPTY);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid known_gr(5, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// First grid a universe.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr1(1, UNIVERSE);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));
  gs.insert(grid_point(A + C));

  print_generators(gr1, "*** gr1 ***");

  Grid gr2(gs);

  print_generators(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point(B));
  known_gs.insert(grid_point(B + D));
  known_gs.insert(grid_line(A));

  Grid known_gr(known_gs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// From generators.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));
  gs.insert(grid_point(A + C));

  Grid gr1(gs);

  print_generators(gr1, "*** gr1 ***");

  gs.clear();
  gs.insert(grid_point(0*B));
  gs.insert(grid_point(B));

  Grid gr2(gs);

  print_generators(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Congruence_System known_cgs;
  known_cgs.insert((A == 1) / 0);
  known_cgs.insert((B == 0) / 0);
  known_cgs.insert((C %= 0) / 1);
  known_cgs.insert((D == 0) / 0);
  known_cgs.insert((E %= 0) / 1);

  Grid known_gr(known_cgs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// First grid empty via the congruence system.
bool
test06() {
  Variable A(0);

  Grid gr1(1);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((A %= 1) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid known_gr(3, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// Second grid empty via the congruence system.
bool
test07() {
  Variable A(0);

  Grid gr1(2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence((A %= 0) / 2);
  gr2.add_congruence((A %= 1) / 2);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid known_gr(3, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// Zero dimension universe.
bool
test08() {
  Variable A(0);

  Grid gr1(0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence((A %= 0) / 2);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid known_gr(1);
  known_gr.add_congruence((A %= 0) / 2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// Zero dimension empty.
bool
test09() {
  Variable A(0);

  Grid gr1(0, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(1);
  gr2.add_congruence((A %= 0) / 2);

  print_congruences(gr2, "*** gr2 ***");

  gr1.concatenate_assign(gr2);

  Grid known_gr(1, EMPTY);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.concatenate_assign(gr2) ***");

  return ok;
}

// Space dimension exception.
bool
test11() {
  Grid gr1(7);

  Grid gr2(1);
  gr2.add_congruence(Congruence::zero_dim_integrality());
  gr2.minimized_congruences();
  gr2.ascii_dump();
  // This needs to allocate a lot of memory, in order to create the
  // integrality congruence.  The presence of the integrality
  // congruence is required by the conversion.

  gr2.add_space_dimensions_and_project(Grid::max_space_dimension() - 1);

  try {
    gr1.concatenate_assign(gr2);
  }
  catch (const std::length_error& e) {
    nout << "max_space_dimension_exceeded: " << e.what() << endl;
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
  // DO_TEST(test11);
END_MAIN
