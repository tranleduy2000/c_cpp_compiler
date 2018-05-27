/* Test Grid::bounds_from_above() and Grid::bounds_from_below().
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

// Empty.
bool
test01() {
  Grid gr(7, EMPTY);

  bool ok = (gr.bounds_from_above(Linear_Expression(0))
	     && gr.bounds_from_below(Linear_Expression(0)));

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Zero dimension empty.
bool
test02() {
  Grid gr(0, EMPTY);

  bool ok = (gr.bounds_from_above(Linear_Expression(3))
	     && gr.bounds_from_below(Linear_Expression(3)));

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Zero dimension universe.
bool
test03() {
  Grid gr(0);

  bool ok = (gr.bounds_from_above(Linear_Expression(1))
	     && gr.bounds_from_below(Linear_Expression(1)));

  print_congruences(gr, "*** gr ***");

  return ok;
}

// Point.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point(3*A + 2*B, 3));

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point(3*A + 2*B, 3));

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(A == 1);
  gr_cgs_needs_min.add_constraint(3*B == 2);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = A + B;
  bool ok = gr_gs_min.bounds_from_above(le)
    && gr_gs_min.bounds_from_below(le)
    && gr_gs_needs_min.bounds_from_above(le)
    && gr_gs_needs_min.bounds_from_below(le)
    && gr_cgs_needs_min.bounds_from_above(le)
    && gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// Rectilinear line.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(B));

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(B));

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(A == 0);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A - B;

  bool ok = !gr_gs_min.bounds_from_above(le)
    && !gr_gs_min.bounds_from_below(le)
    && !gr_gs_needs_min.bounds_from_above(le)
    && !gr_gs_needs_min.bounds_from_below(le)
    && !gr_cgs_needs_min.bounds_from_above(le)
    && !gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// Line.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(2*A + B));

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(2*A + B));

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(A - 2*B == 0);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A + B;
  bool ok = !gr_gs_min.bounds_from_above(le)
    && !gr_gs_min.bounds_from_below(le)
    && !gr_gs_needs_min.bounds_from_above(le)
    && !gr_gs_needs_min.bounds_from_below(le)
    && !gr_cgs_needs_min.bounds_from_above(le)
    && !gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// A line along expr in the grid.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(A + 2*B));

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(A + 2*B));

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(2*A - B == 0);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A - B;
  bool ok = gr_gs_min.bounds_from_above(le)
    && gr_gs_min.bounds_from_below(le)
    && gr_gs_needs_min.bounds_from_above(le)
    && gr_gs_needs_min.bounds_from_below(le)
    && gr_cgs_needs_min.bounds_from_above(le)
    && gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}


// A parameter along expr in the grid.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_point(A + 2*B));

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_point(A + 2*B));

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(2*A - B == 0);
  gr_cgs_needs_min.add_congruence((B %= 0) / 2);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A - B;
  bool ok = gr_gs_min.bounds_from_above(le)
    && gr_gs_min.bounds_from_below(le)
    && gr_gs_needs_min.bounds_from_above(le)
    && gr_gs_needs_min.bounds_from_below(le)
    && gr_cgs_needs_min.bounds_from_above(le)
    && gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// Two lines which combine to cover any line bounded by expr.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(A));
  gr_gs_min.add_grid_generator(grid_line(B));

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(A));
  gr_gs_needs_min.add_grid_generator(grid_line(B));

  Grid gr_cgs_needs_min(2);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = A - B;
  bool ok = !gr_gs_min.bounds_from_above(le)
    && !gr_gs_min.bounds_from_below(le)
    && !gr_gs_needs_min.bounds_from_above(le)
    && !gr_gs_needs_min.bounds_from_below(le)
    && !gr_cgs_needs_min.bounds_from_above(le)
    && !gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// In three dimensions, lines and parameters which combine to include
// expr.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr_gs_min(3, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(A));
  gr_gs_min.add_grid_generator(grid_point(B + C));

  Grid gr_gs_needs_min(3, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(A));
  gr_gs_needs_min.add_grid_generator(grid_point(B + C));

  Grid gr_cgs_needs_min(3);
  gr_cgs_needs_min.add_constraint(B - C == 0);
  gr_cgs_needs_min.add_congruence(B %= 0);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A + B - C;
   bool ok = !gr_gs_min.bounds_from_above(le)
     && !gr_gs_min.bounds_from_below(le)
     && !gr_gs_needs_min.bounds_from_above(le)
     && !gr_gs_needs_min.bounds_from_below(le)
     && !gr_cgs_needs_min.bounds_from_above(le)
     && !gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// Grid which bounds a 3D expr.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr_gs_min(3, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(3*B + C));
  gr_gs_min.add_grid_generator(grid_line(A - 2*B));

  Grid gr_gs_needs_min(3, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(3*B + C));
  gr_gs_needs_min.add_grid_generator(grid_line(A - 2*B));

  Grid gr_cgs_needs_min(3);
  gr_cgs_needs_min.add_constraint(2*A + B - 3*C == 0);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A + B - 3*C;
  bool ok = gr_gs_min.bounds_from_above(le)
    && gr_gs_min.bounds_from_below(le)
    && gr_gs_needs_min.bounds_from_above(le)
    && gr_gs_needs_min.bounds_from_below(le)
    && gr_cgs_needs_min.bounds_from_above(le)
    && gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// Point in 6D.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  Grid gr_gs_min(6, EMPTY);
  gr_gs_min.add_grid_generator(grid_point(7*A - 11*B + 19*F));

  Grid gr_gs_needs_min(6, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point(7*A - 11*B + 19*F));

  Grid gr_cgs_needs_min(6);
  gr_cgs_needs_min.add_constraint(A == 7);
  gr_cgs_needs_min.add_constraint(B == -11);
  gr_cgs_needs_min.add_constraint(C == 0);
  gr_cgs_needs_min.add_constraint(D == 0);
  gr_cgs_needs_min.add_constraint(E == 0);
  gr_cgs_needs_min.add_constraint(F == 19);

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = A + 2*B + 3*C + 4*D + 6*F;
  bool ok = gr_gs_min.bounds_from_above(le)
    && gr_gs_min.bounds_from_below(le)
    && gr_gs_needs_min.bounds_from_above(le)
    && gr_gs_needs_min.bounds_from_below(le)
    && gr_cgs_needs_min.bounds_from_above(le)
    && gr_cgs_needs_min.bounds_from_below(le);

  print_congruences(gr_gs_min, "*** gr_gs_min **");
  print_congruences(gr_gs_needs_min, "*** gr_gs_needs_min **");
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min **");

  return ok;
}

// Space dimension exception.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable C(2);

  Grid gr(3, EMPTY);

  Linear_Expression le = A + 2*B + 3*C + 4*D + 6*F;

  try {
    gr.bounds_from_above(le);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// The generator system is up-to-date but not minimized.
bool
test14() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(A, 2));
  print_generators(gr, "*** gr generators before ***");

  bool ok = !gr.bounds_from_above(A) && !gr.bounds_from_below(A);
  print_generators(gr, "*** gr generators after ***");

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
  DO_TEST(test13);
  DO_TEST(test14);
END_MAIN
