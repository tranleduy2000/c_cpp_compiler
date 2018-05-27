/* Test Grid::map_space_dimensions().
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

// Empty grid, empty mapping.
bool
test01() {
  Partial_Function function;

  Grid gr(3, EMPTY);
  print_congruences(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid known_gr(0, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping all dimensions.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Partial_Function function;
  function.insert(0, 2);
  function.insert(2, 0);
  function.insert(1, 1);

  Grid_Generator_System gs;
  gs.insert(grid_point(2*C));
  gs.insert(grid_point(4*C));
  gs.insert(grid_point(A));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point(2*A));
  known_gs.insert(grid_point(4*A));
  known_gs.insert(grid_point(C));

  Grid known_gr(known_gs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping all dimensions, with overlap.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(2, 0);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(2*A));
  gs.insert(grid_line(A - C));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point());
  known_gs.insert(grid_point(2*B));
  known_gs.insert(grid_line(B - A));
  Grid known_gr(known_gs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping more dimensions than there are in the grid.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Partial_Function function;
  function.insert(2, 0);
  function.insert(3, 2);
  function.insert(4, 1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(A));
  gs.insert(grid_point(B));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_point(0*C));
  Grid known_gr(known_gs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping all dimensions to themselves.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(0, 0);
  function.insert(1, 1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(A));
  gs.insert(grid_point(B));
  gs.insert(grid_point(A + B));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");
  Grid known_gr(gr);

  gr.map_space_dimensions(function);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping all additional dimensions (in the mapping) to themselves.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);
  function.insert(2, 2);
  function.insert(3, 3);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(A));
  gs.insert(grid_point(2*B));
  gs.insert(grid_point(A + 2*B));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid known_gr(4, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(B));
  known_gr.add_grid_generator(grid_point(2*A));
  known_gr.add_grid_generator(grid_point(2*A + B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping new dimensions over existing ones.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(0, 0);
  function.insert(2, 1);
  function.insert(3, 2);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(A));
  gs.insert(grid_line(B));
  gs.insert(grid_line(A + B));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// Mapping away a dimension in an empty grid.
bool
test08() {
  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

  return ok;
}

// A minimized grid in which the point contains factors and the
// divisor is greater than one.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(4*A, 2));
  gr.add_grid_generator(grid_point(4*A + B, 2));

  // Force minimization.
  gr.minimized_grid_generators();
  print_generators(gr, "*** gr ***");

  gr.map_space_dimensions(function);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(4*B, 2));
  known_gr.add_grid_generator(grid_point(4*B + A, 2));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.map_space_dimensions(function) ***");

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
