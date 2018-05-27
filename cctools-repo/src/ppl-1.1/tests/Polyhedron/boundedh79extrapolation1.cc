/* Test Polyhedron::bounded_H79_extrapolation_assign().
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

bool
test01() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(x-3 >= 0);
  ph1.add_constraint(x-3 <= 1);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 1);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2);
  ph2.add_constraint(2*x-5 >= 0);
  ph2.add_constraint(x-3 <= 1);
  ph2.add_constraint(2*y+3 >= 0);
  ph2.add_constraint(2*y-5 <= 0);

  print_constraints(ph2, "*** ph2 ***");

  Constraint_System cs;
  cs.insert(x >= y);

  print_constraints(cs, "*** cs ***");

  ph2.bounded_H79_extrapolation_assign(ph1, cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(x >= 2);
  known_result.add_constraint(x <= 4);
  known_result.add_constraint(y >= -2);
  known_result.add_constraint(x >= y);

  bool ok = (ph2 == known_result);

  print_constraints(ph2,
                    "*** after ph2.bounded_H79_extrapolation_assign(ph1, cs) "
                    "***");

  return ok;
}

} // namespace

BEGIN_MAIN
// Note: test01() only overflows on NNC polyhedra
// (with 8-bit coefficients and assertions turned on).
#ifdef DERIVED_TEST
  DO_TEST_F8A(test01);
#else
  DO_TEST(test01);
#endif
END_MAIN
