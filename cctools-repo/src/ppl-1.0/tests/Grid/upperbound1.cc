/* Test Grid::upper_bound_assign()
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

// Two grids each with one point and two lines
bool
test01() {
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs1;
  gs1.insert(grid_point(C));

  Grid_Generator_System gs2;
  gs2.insert(grid_point(B + 0*C));

  Grid gr1(gs1);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point(C));
  known_gs.insert(grid_point(B));

  Grid known_gr(known_gs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Two universe grids.
bool
test02() {
  Grid gr1(3);
  print_generators(gr1, "*** gr1 ***");
  Grid gr2(3);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid known_gr(3);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Second grid universe.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  print_generators(gr1, "*** gr1 ***");

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_line(A));
  gs.insert(grid_line(B));
  gs.insert(grid_line(-C));

  Grid gr2(gs);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid known_gr(3);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Inserting a parameter.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs1;
  gs1.insert(grid_point(0*C));
  gs1.insert(grid_line(A));
  gs1.insert(grid_line(B));

  Grid gr1(gs1);

  gr1.add_grid_generator(parameter(-C));
  print_generators(gr1, "*** gr1 ***");

  Grid_Generator_System gs2;
  gs2.insert(grid_point(0*C));

  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid known_gr(3);
  known_gr.add_congruence(C %= 0);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Out-of-date generators in the first grid.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(B == 0);
  gr1.add_constraint(C == 0);
  print_generators(gr1, "*** gr1 ***");

  Grid_Generator_System gs2;
  gs2.insert(grid_point(B + 0*C));

  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point());
  known_gs.insert(grid_point(B + 0*C));

  Grid known_gr(known_gs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Out-of-date generators in the second grid.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(B + 0*C));

  Grid gr1(gs);
  print_generators(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_constraint(A == 0);
  gr2.add_constraint(B == 0);
  gr2.add_constraint(C == 0);
  print_congruences(gr2, "*** gr2 ***");

  gr1.upper_bound_assign(gr2);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point());
  known_gs.insert(grid_point(B + 0*C));

  Grid known_gr(known_gs);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Space dimension exception.
bool
test07() {
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
    gr1.upper_bound_assign(gr2);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Out-of-date generators in the first grid, which is empty.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_constraint(A == 0);
  gr1.add_constraint(A == 1);
  print_congruences(gr1, "*** gr1 ***");

  Grid_Generator_System gs2;
  gs2.insert(grid_point(B + 0*C));

  Grid gr2(gs2);
  print_generators(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  gr1.upper_bound_assign(gr2);

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.upper_bound_assign(gr2) ***");

  return ok;
}

// Based on an example in a paper by Muller-Olm and Seidl in SAS 2005
// Here there is an input and output version of each variable
// A, B being input and A1, B1 the output.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  // initial point
  Grid gr0(4);
  gr0.add_constraint(A == 2);
  gr0.add_constraint(B == 0);

  // a pass through the procedure may do nothing
  Grid gr1(gr0);
  gr1.add_constraint(A == C);
  gr1.add_constraint(B == D);

  // one non-trivial pass through procedure
  Grid gr2(gr0);
  gr2.add_constraint(15 * A == C);
  gr2.add_constraint(18 * A + B == D);
  // combine alternative paths 1 and 2
  gr2.upper_bound_assign(gr1);

  // two non-trivial passes through procedure
  Grid gr3(gr0);
  gr3.add_constraint(225 * A == C);
  gr3.add_constraint(282 * A + B == D);
  // combine alternative paths 1, 2 and 3
  gr3.upper_bound_assign(gr2);

  Variables_Set vars;
  vars.insert(A);
  vars.insert(B);

  gr3.remove_space_dimensions(vars);

  // as in paper
  Grid known_gr(2);

  known_gr.add_congruence((A %= 2) / 28);
  known_gr.add_congruence((B %= 0) / 12);

  bool ok = (gr3 == known_gr);

  print_congruences(gr3, "*** gr3.remove_space_dimensions(vars) ***");

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
  DO_TEST_F8(test09);
END_MAIN
