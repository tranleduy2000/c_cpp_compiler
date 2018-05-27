/* Creation of a one-point polyhedron.
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

bool
test01() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  C_Polyhedron ph(3, EMPTY);
  ph.add_generator(point(-1000*x - 9*y, 16));

  print_generators(ph, "*** generators ***");
  print_constraints(ph, "*** constraints ***");

  C_Polyhedron known_result(3);
  known_result.add_constraint(2*x == -125);
  known_result.add_constraint(16*y == -9);
  known_result.add_constraint(z == 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
END_MAIN
