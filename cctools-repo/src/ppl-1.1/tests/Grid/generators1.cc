/* Test Grid::generators().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

// Empty grid.
bool
test01() {
  Grid gr1(7, EMPTY);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Universe grid.
bool
test02() {
  Grid gr1(7);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Zero dimension empty grid.
bool
test03() {
  Grid gr1(0, EMPTY);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Zero dimension universe grid.
bool
test04() {
  Grid gr1(0);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Skew grid in 3D.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*B));
  gr1.add_grid_generator(grid_point(5*A + 2*B));
  gr1.add_grid_generator(grid_point(7*B));
  gr1.add_grid_generator(grid_line(C));

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// 3D rectilinear grid defined by congruences.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 10);
  gr1.add_congruence((B %= 10) / 0);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Get a reference to the empty generators, add a point, use the
// reference to create a new grid.
bool
test07() {
  Grid gr1(3, EMPTY);

  const Grid_Generator_System& gs = gr1.grid_generators();

  // Add a point.  The idea is to check that `gs' still refers to a
  // generator system that matches the grid.
  gr1.add_grid_generator(grid_point());

  Grid known_gr = gr1;

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// In zero dimensions get a reference to the universe generators,
// empty the grid, and then use the reference to create a new grid.
bool
test08() {
  Grid gr1(0);

  const Grid_Generator_System& gs = gr1.grid_generators();

  // Empty the grid.  The idea is to check that `gs' still refers to a
  // generator system that matches the grid.
  gr1.add_congruence(Congruence::zero_dim_false());

  Grid known_gr = gr1;

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Empty grid, where updating the generators finds the grid empty.
bool
test09() {
  Variable A(0);

  Grid gr1(7);
  gr1.add_constraint(A == 1);
  gr1.add_constraint(A == 0);

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Skew grid in 3D defined with generators with a non-integral parameter.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*B, 2));
  gr1.add_grid_generator(grid_point(5*A + 2*B));
  gr1.add_grid_generator(parameter(11*B, 2));
  gr1.add_grid_generator(grid_line(C));

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Grid defined with an integral point but non-integral parameter.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*B));
  gr1.add_grid_generator(grid_point(5*A + 2*B));
  gr1.add_grid_generator(parameter(11*B, 2));
  gr1.add_grid_generator(grid_line(C));

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// Grid defined with an integral point and parameter.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*B));
  gr1.add_grid_generator(grid_point(5*A + 2*B));
  gr1.add_grid_generator(parameter(11*B));
  gr1.add_grid_generator(grid_line(C));

  Grid known_gr = gr1;

  Grid_Generator_System gs = gr1.grid_generators();

  Grid gr2(gs);

  bool ok = (gr2 == known_gr);

  print_generators(gr2, "*** gr2 ***");

  return ok;
}

// inserted generator system has more space dimensions that
// the old generator system.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(3*A, 2));

  Grid_Generator_System gs1;
  gs1.insert(grid_point(5*A + 2*B, 3));
  gs1.insert(parameter(11*C, 2));

  gs.insert(gs1, Recycle_Input());

  Grid gr(gs);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(3*A, 2));
  known_gr.add_grid_generator(grid_point(5*A + 2*B, 3));
  known_gr.add_grid_generator(parameter(11*C, 2));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr ***");

  return ok;
}

// test clear() for nonempty generator system.
bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(3*A, 2));
  gs.insert(grid_point(5*A + 2*B, 3));
  gs.insert(parameter(11*C, 2));

  print_generators(gs, "*** gs ***");

  gs.clear();

  Grid gr(gs);

  Grid known_gr(0, EMPTY);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr ***");
  nout << "gr.space_dimension = " << gr.space_dimension() << endl;

  return ok;
}

// test clear() for empty generator system.
// This test showed a bug in the code for clear() in
// Grid_Generator_System_inlines.hh which is now corrected.
bool
test15() {
  Grid_Generator_System gs;

  print_generators(gs, "*** gs ***");

  gs.clear();

  Grid gr(gs);

  Grid known_gr(0, EMPTY);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr ***");
  nout << "gr.space_dimension = " << gr.space_dimension() << endl;

  return ok;
}

// space_dimension
bool
test16() {
  Variable A(3);
  Variable B(7);
  Variable C(4);

  Grid_Generator_System ggs(grid_point(A - 3*B + 21*C, 21));

  bool ok = (ggs.space_dimension() == 8);

  print_generators(ggs, "*** ggs ***");

  return ok;
}

// Recycling_insert
bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System ggs1;
  ggs1.insert(parameter(11*B));
  ggs1.insert(grid_line(C));
  ggs1.insert(grid_point(3*B));

  Grid_Generator_System ggs;
  ggs.insert(ggs1, Recycle_Input());
  print_generators(ggs, "*** ggs ***");

  Grid gr(ggs);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(3*B));
  known_gr.add_grid_generator(parameter(11*B));
  known_gr.add_grid_generator(grid_line(C));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr(ggs} ***");

  return ok;
}

// Comparing Systems with different space dimensions
bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable E(4);

  Grid_Generator_System ggs;
  ggs.insert(grid_point(3*B));
  ggs.insert(grid_line(E));
  print_generators(ggs, "*** ggs ***");

  Grid gr(ggs);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(3*B));
  known_gr.add_grid_generator(parameter(11*B));
  known_gr.add_grid_generator(grid_line(C));

  bool ok = (gr != known_gr);

  print_generators(gr, "*** gr(ggs} ***");

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
  DO_TEST_F8(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
END_MAIN
