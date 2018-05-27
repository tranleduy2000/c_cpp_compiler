/* Test Grid::add_space_dimensions_and_embed().
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

// From congruences, adding to both congruences and generators.
bool
test01() {
  Variable A(0);
  Variable C(2);
  Variable E(4);

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 0) / 2);

  Grid gr(cgs);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(2);

  Congruence_System known_cgs;
  known_cgs.insert((A + 0*E %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");

  return ok;
}

// An empty grid.
bool
test02() {
  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(3);

  Grid known_gr(5, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(3) ***");

  return ok;
}

// A universe grid.
bool
test03() {
  Grid gr(1);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(4);

  Grid known_gr(5);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(4) ***");

  return ok;
}

// From generators.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable E(4);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));
  gs.insert(grid_point(A + C));

  Grid gr(gs);

  print_generators(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(2);

  Congruence_System known_cgs;
  known_cgs.insert((A == 1) / 0);
  known_cgs.insert((C + 0*E %= 0) / 1);
  known_cgs.insert((B == 0) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(2) ***");

  return ok;
}

// From congruences, where dimensions are only added to the grid's
// congruence system.
bool
test05() {
  Variable A(0);
  Variable C(2);
  Variable E(4);

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 0) / 2);

  Grid gr(cgs);

  print_congruences(gr, "*** gr ***");

  // Add space dimensions directly after creating the grid, to ensure
  // that only the congruences are up to date.

  gr.add_space_dimensions_and_embed(2);

  Congruence_System known_cgs;
  known_cgs.insert((A + 0*E %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(2) ***");

  return ok;
}

// Space dimension exception.
bool
test06() {
  Grid gr(10);

  try {
    gr.add_space_dimensions_and_embed(Grid::max_space_dimension());
  }
  catch (const std::length_error& e) {
    nout << "max_space_dimension_exceeded: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Zero dimension universe grid.
bool
test07() {
  Grid gr(0);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(13);

  Grid known_gr(13);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(13) ***");

  return ok;
}

// Add to a grid which has minimized congruences.
bool
test08() {
  Variable A(0);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  gr.minimized_congruences();

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(2);

  Grid known_gr(4);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(2) ***");

  return ok;
}

// Add to a grid which has out of date congruences and minimized
// generators.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_line(A));

  gr.minimized_grid_generators();

  print_generators(gr, "*** gr ***");

  gr.add_space_dimensions_and_embed(2);

  Grid known_gr(4);
  known_gr.add_congruence(B %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_embed(2) ***");

  return ok;
}

// From congruences.
bool
test10() {
  Variable A(0);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 0) / 2);

  Grid gr(cgs);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(2);

  Congruence_System known_cgs;
  known_cgs.insert((E %= 0) / 0);
  known_cgs.insert((D %= 0) / 0);
  known_cgs.insert((A %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(2) ***");

  return ok;
}

// Empty grid.
bool
test11() {
  Grid gr(2, EMPTY);

  print_generators(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(3);

  Grid known_gr(5, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(3) ***");

  return ok;
}

// Universe grid, compared to congruences.
bool
test12() {
  Grid gr(1);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(4);

  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Congruence_System known_cgs;
  known_cgs.insert((B %= 0) / 0);
  known_cgs.insert((C %= 0) / 0);
  known_cgs.insert((D %= 0) / 0);
  known_cgs.insert((E %= 0) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(4) ***");

  return ok;
}

// Universe grid, compared to generators.
bool
test13() {
  Grid gr(1);

  print_generators(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(3);

  Variable A(0);
  Variable D(3);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point());
  known_gs.insert(grid_line(A + 0*D));

  Grid known_gr(known_gs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr ***");

  return ok;
}

// From generators.
bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));
  gs.insert(grid_point(A + B + C));

  Grid gr(gs);

  print_generators(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(2);

  Congruence_System known_cgs;
  known_cgs.insert(A == 1);
  known_cgs.insert(B - C == 0);
  known_cgs.insert(B %= 0);
  known_cgs.insert(D == 0);
  known_cgs.insert(E == 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(2) ***");

  return ok;
}

// From generators, where dimensions are only added to the grid's
// generator system.
bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));
  gs.insert(grid_point(A + B + C));

  Grid gr(gs);

  print_generators(gr, "*** gr ***");

  // Add space dimensions directly after creating the grid, to ensure
  // that only the generators are up to date.

  gr.add_space_dimensions_and_project(2);

  Congruence_System known_cgs;
  known_cgs.insert((A == 1) / 0);
  known_cgs.insert((B - C %= 0) / 0);
  known_cgs.insert((B %= 0) / 1);
  known_cgs.insert((D %= 0) / 0);
  known_cgs.insert((E %= 0) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(2) ***");

  return ok;
}

// Space dimension exception.
bool
test16() {
  Grid gr(10);

  try {
    gr.add_space_dimensions_and_project(Grid::max_space_dimension());
  }
  catch (const std::length_error& e) {
    nout << "max_space_dimension_exceeded: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Zero dimension universe grid.
bool
test17() {
  Grid gr(0);

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(13);

  Grid known_gr(13);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(13) ***");

  return ok;
}

// Add to a grid which has minimized congruences.
bool
test18() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);

  gr.minimized_congruences();

  print_congruences(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(2);

  Grid known_gr(4, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(parameter(A));
  known_gr.add_grid_generator(grid_line(B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(2) ***");

  return ok;
}

// Add to a grid which has out of date congruences and minimized
// generators.
bool
test19() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_line(A));

  gr.minimized_grid_generators();

  print_generators(gr, "*** gr ***");

  gr.add_space_dimensions_and_project(2);

  Grid known_gr(4);
  known_gr.add_constraint(B == 0);
  known_gr.add_constraint(C == 0);
  known_gr.add_constraint(D == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_space_dimensions_and_project(2) ***");

  return ok;
}

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
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
END_MAIN
