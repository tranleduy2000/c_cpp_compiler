/* Test Grid::remove_space_dimensions().
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

// Testing remove_space_dimensions

// Simple grid.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A - B == 0);
  gr.add_congruence(A %= 0);
  print_congruences(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(B);

  gr.remove_space_dimensions(vars);

  Grid known_gr(1);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// Empty grid.
bool
test02() {
  Variable B(1);

  Grid gr(4, EMPTY);
  print_congruences(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(B);

  gr.remove_space_dimensions(vars);

  Grid known_gr(3, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// Universe grid.
bool
test03() {
  Variable C(2);
  Variable D(3);

  Grid gr(7, UNIVERSE);

  Variables_Set vars;
  vars.insert(C);
  vars.insert(D);

  gr.remove_space_dimensions(vars);

  Grid known_gr(5, UNIVERSE);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

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

  Variables_Set vars;
  vars.insert(B);

  gr.remove_space_dimensions(vars);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// From congruences.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Variables_Set vars;
  vars.insert(B);
  vars.insert(D);

  Congruence_System cgs;
  cgs.insert((A + 2*C %= 0) / 3);
  cgs.insert((B - E %= 0) / 2);

  Grid gr(cgs);
  print_congruences(gr, "*** gr ***");

  gr.remove_space_dimensions(vars);

  Grid_Generator_System known_ggs;
  known_ggs.insert(grid_point());
  known_ggs.insert(grid_line(2*A - B));
  known_ggs.insert(grid_point(3*B, 2));
  known_ggs.insert(grid_line(C));

  Grid known_gr(known_ggs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// Variable set includes first dimension.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(A - B == 0);
  gr.add_congruence(A %= 0);
  print_congruences(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(A);
  vars.insert(C);

  gr.remove_space_dimensions(vars);

  Grid known_gr(1);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// The resulting grid contains a parameter that is all zeros.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_line(C));
  print_generators(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(B);

  gr.remove_space_dimensions(vars);

  Grid known_gr(2);
  known_gr.add_congruence(A %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// The resulting grid contains a line that is all zeros.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(parameter(B));
  gr.add_grid_generator(grid_line(C));
  print_generators(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(C);

  gr.remove_space_dimensions(vars);

  Grid known_gr(2);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence(B %= 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// Empty variable set.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_line(C));
  print_generators(gr, "*** gr ***");

  Variables_Set vars;

  Grid known_gr = gr;

  gr.remove_space_dimensions(vars);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// Zero dimension universe resulting grid.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_line(C));
  print_generators(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(A);
  vars.insert(B);
  vars.insert(C);

  gr.remove_space_dimensions(vars);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.remove_space_dimensions(vars) ***");

  return ok;
}

// Space dimension exception.
bool
test11() {
  Variable B(1);

  Grid gr(1, EMPTY);
  print_congruences(gr, "*** gr ***");

  Variables_Set vars;
  vars.insert(B);

  try {
    gr.remove_space_dimensions(vars);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
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
  DO_TEST(test10);
  DO_TEST(test11);
END_MAIN
