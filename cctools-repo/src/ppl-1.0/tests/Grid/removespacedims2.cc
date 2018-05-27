/* Test Grid::remove_higher_space_dimensions().
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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + 2*C %= 0) / 3);

  Grid gr(cgs);
  print_congruences(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(2);

  Grid_Generator_System known_ggs;
  known_ggs.insert(grid_point(0*B));
  known_ggs.insert(grid_line(A));
  known_ggs.insert(grid_line(B));

  Grid known_gr(known_ggs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(2) ***");

  return ok;
}

// Empty grid.
bool
test02() {
  Grid gr(2, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(1);

  Grid known_gr(1, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(1) ***");

  return ok;
}

// Universe grid.
bool
test03() {
  Grid gr(7);
  print_generators(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(3);

  Grid known_gr(3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(3) ***");

  return ok;
}

// From generators.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System ggs;
  ggs.insert(grid_point(0*A));
  ggs.insert(grid_point(2*A));
  ggs.insert(grid_point(3*B));

  Grid gr(ggs);
  print_generators(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(1);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(1) ***");

  return ok;
}

// Resulting grid the same.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_line(C));
  print_generators(gr, "*** gr ***");

  Grid known_gr = gr;

  gr.remove_higher_space_dimensions(gr.space_dimension());

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions() ***");

  return ok;
}

// Zero dimension universe resulting grid.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_line(C));
  print_generators(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(0);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(0) ***");

  return ok;
}

// Space dimension exception.
bool
test07() {
  Grid gr(1, EMPTY);
  print_generators(gr, "*** gr ***");

  try {
    gr.remove_higher_space_dimensions(6);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// From congruences.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(B - C == 0);
  gr.add_congruence(B %= 0);
  gr.add_constraint(A == 4);
  print_congruences(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(2);

  Grid_Generator_System known_ggs;
  known_ggs.insert(grid_point(4*A));
  known_ggs.insert(parameter(B));

  Grid known_gr(known_ggs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(2) ***");

  return ok;
}

// From congruences.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(B - C == 0);
  gr.add_congruence(B %= 0);
  gr.add_constraint(A == 4);
  print_congruences(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(1);

  Grid_Generator_System known_ggs;
  known_ggs.insert(grid_point(4*A));

  Grid known_gr(known_ggs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(2) ***");

  return ok;
}

// Where the redundant row with the lowest dim_kinds entry is a
// congruence or equality.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A %= 0) / 2);
  gr.add_congruence((A - C %= 0) / 2);
  print_congruences(gr, "*** gr ***");

  gr.remove_higher_space_dimensions(2);

  Grid known_gr(2);
  known_gr.add_congruence((A %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_higher_space_dimensions(2) ***");

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
