/* Test Grid::topological_closure_assign().
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
  Grid gr(6, EMPTY);
  print_generators(gr, "*** gr ***");

  Grid known_gr = gr;

  gr.topological_closure_assign();

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.topological_closure_assign() ***");

  return ok;
}

// Universe
bool
test02() {
  Grid gr(5);
  print_generators(gr, "*** gr ***");

  Grid known_gr = gr;

  gr.topological_closure_assign();

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.topological_closure_assign() ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(0*C));
  gs.insert(grid_line(A));
  gs.insert(grid_line(B));
  gs.insert(parameter(-C));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  Grid known_gr = gr;

  gr.topological_closure_assign();

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.topological_closure_assign() ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
