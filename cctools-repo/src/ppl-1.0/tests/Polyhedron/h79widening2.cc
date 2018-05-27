/* Test Polyhedron::H79_widening_assign().
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

// This is the example of Figure 3 in [BagnaraRZH02TR].
bool
test01() {
  Variable A(0);

  NNC_Polyhedron ph1(1);
  ph1.add_constraint(A > 0);
  ph1.add_constraint(A < 2);

  NNC_Polyhedron ph4(1);
  ph4.add_constraint(4*A >= 1);
  ph4.add_constraint(4*A <= 3);

  NNC_Polyhedron ph = ph4;
  ph.intersection_assign(ph1);
  // At this point, ph and ph4 are two different representations
  // of the same NNC polyhedron.

  print_constraints(ph4, "*** ph4 ***");
  print_constraints(ph, "*** ph ***");

  NNC_Polyhedron known_result(ph4);

  ph.H79_widening_assign(ph4);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after H79_widening_assign ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
