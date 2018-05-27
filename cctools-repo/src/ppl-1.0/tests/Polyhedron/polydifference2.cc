/* Test Polyhedron::difference_assign().
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

  Generator_System gs1;
  gs1.insert(point(0*x + 0*y));
  gs1.insert(point(0*x + 3*y));
  gs1.insert(point(3*x + 0*y));
  gs1.insert(point(3*x + 3*y));

  NNC_Polyhedron ph1(gs1);

  print_constraints(ph1, "*** ph1 ***");

  Constraint_System cs2;
  cs2.insert(1*x + 0*y == 0);

  NNC_Polyhedron ph2(cs2);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron computed_result = ph1;

  computed_result.difference_assign(ph2);

  Generator_System gs_known_result;
  gs_known_result.insert(closure_point(0*x + 0*y));
  gs_known_result.insert(closure_point(0*x + 3*y));
  gs_known_result.insert(point(3*x + 0*y));
  gs_known_result.insert(point(3*x + 3*y));

  NNC_Polyhedron known_result(gs_known_result);

  bool ok = (computed_result == known_result);

  print_generators(computed_result, "*** after difference_assign ***");
  print_generators(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(B >= 0);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A > 2);
  ph2.add_constraint(B >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.difference_assign(ph2);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.difference_assign(ph2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(y >= 0);
  cs1.insert(x <= 4);
  cs1.insert(x - 2*y <= 2);

  C_Polyhedron ph1(cs1);

  print_constraints(ph1, "*** ph1 ***");

  Constraint_System cs2;
  cs2.insert(x >= 0);
  cs2.insert(y >= 0);
  cs2.insert(x <= 4);
  cs2.insert(y <= 5);
  cs2.insert(x - 2*y <= 2);
  cs2.insert(x + y <= 7);

  C_Polyhedron ph2(cs2);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron computed_result = ph1;
  computed_result.difference_assign(ph2);

  Generator_System gs_known_result;
  gs_known_result.insert(point(0*x + 5*y));
  gs_known_result.insert(point(4*x + 3*y));
  gs_known_result.insert(ray(0*x + 1*y));

  C_Polyhedron known_result(gs_known_result);

  bool ok = (computed_result == known_result);

  print_constraints(computed_result, "*** after difference_assign ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test04() {
  Variable A(0);

  C_Polyhedron ph1(1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(A >= 1);
  ph2.add_constraint(A <= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.difference_assign(ph2);

  C_Polyhedron known_result(1);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.difference_assign(ph2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
