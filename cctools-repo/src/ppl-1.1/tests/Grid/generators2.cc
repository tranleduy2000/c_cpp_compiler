/* Test class Grid_Generator_System.
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

// recycling_insert(g) where the dimension of g is greater than the
// generator system.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs(1);

  Grid_Generator g = grid_point(A + B);

  gs.insert(g, Recycle_Input());

  print_generators(gs, "*** gs ***");

  Grid_Generator_System known_gs(grid_point(A + B));

  bool ok = (gs == known_gs);

  print_generators(known_gs, "*** known_gs ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
