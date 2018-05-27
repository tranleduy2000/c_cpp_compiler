/* Test Polyhedron::limited_H79_extrapolation_assign().
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

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);

  C_Polyhedron ph1(cs1);

  print_constraints(ph1, "*** ph1 ***");

  Constraint_System cs2;
  cs2.insert(x >= 0);
  cs2.insert(x <= 2);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);

  C_Polyhedron ph2(cs2);

  print_constraints(ph2, "*** ph2 ***");

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);

  print_constraints(cs, "*** cs ***");

  C_Polyhedron computed_result = ph2;
  computed_result.limited_H79_extrapolation_assign(ph1, cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(x - y >= 0);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x <= 5);

  print_constraints(computed_result,
		    "*** after limited_H79_extrapolation_assign ***");

  return computed_result == known_result;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y == 0);

  C_Polyhedron ph1(cs1);

  print_constraints(ph1, "*** ph1 ***");

  Constraint_System cs2;
  cs2.insert(x <= 2);
  cs2.insert(y >= 0);
  cs2.insert(y <= x);

  C_Polyhedron ph2(cs2);

  print_constraints(ph2, "*** ph2 ***");

  Constraint_System cs;
  cs.insert(y <= -1);
  cs.insert(x <= 5);

  print_constraints(cs, "*** cs ***");

  C_Polyhedron computed_result = ph2;
  computed_result.limited_H79_extrapolation_assign(ph1, cs);

  C_Polyhedron known_result = ph2;
  known_result.add_generator(point(5*x));
  known_result.add_generator(point(5*x + 5*y));

  print_constraints(computed_result,
		    "*** after limited_H79_extrapolation_assign ***");

  return computed_result == known_result;
}

bool
test03() {
  Variable x(0);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  C_Polyhedron ph1(cs1);

  print_constraints(ph1, "*** ph1 ***");

  Constraint_System cs2;
  cs2.insert(x == 0);
  C_Polyhedron ph2(cs2);

  print_constraints(ph2, "*** ph2 ***");

  Constraint_System cs;
  cs.insert(x >= 0);

  print_constraints(cs, "*** cs ***");

  C_Polyhedron computed_result = ph1;
  computed_result.limited_H79_extrapolation_assign(ph2, cs);

  C_Polyhedron known_result(cs);

  print_constraints(computed_result,
		    "*** after limited_H79_extrapolation_assign ***");

  return computed_result == known_result;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  C_Polyhedron ph2(2);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(A - B >= 0);
  ph2.add_constraint(A <= 2);

  Constraint_System cs;
  cs.insert(B <= 4);

  C_Polyhedron known_result(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  ph2.limited_H79_extrapolation_assign(ph1, cs);

  bool ok = (ph2 == known_result);

  print_constraints(ph2,
		    "*** after ph2.limited_H79_extrapolation_assign(ph1, cs)"
		    " ***");
  return ok;
}

bool
test05() {
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  C_Polyhedron ph2(2, EMPTY);

  Constraint_System cs;
  cs.insert(B <= 4);

  C_Polyhedron known_result(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  ph2.limited_H79_extrapolation_assign(ph1, cs);

  bool ok = (ph2 == known_result);

  print_constraints(ph2,
		    "*** after ph2.limited_H79_extrapolation_assign(ph1, cs)"
		    " ***");
  return ok;
}

bool
test06() {
  C_Polyhedron ph1;

  C_Polyhedron ph2;

  Constraint_System cs;
  cs.insert(Linear_Expression(2) <= 4);

  C_Polyhedron known_result(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  ph2.limited_H79_extrapolation_assign(ph1, cs);

  bool ok = (ph2 == known_result);

  print_constraints(ph2,
		    "*** after ph2.limited_H79_extrapolation_assign(ph1, cs)"
		    " ***");
  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 2);
  ph1.add_constraint(A <= -2);
  ph1.add_constraint(B == 0);
  C_Polyhedron ph2(2);
  ph1.add_constraint(A >= 2);

  Constraint_System cs;
  cs.insert(B <= 4);

  C_Polyhedron known_result(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  ph2.limited_H79_extrapolation_assign(ph1, cs);

  bool ok = (ph2 == known_result);

  print_constraints(ph2,
		    "*** after ph2.limited_H79_extrapolation_assign(ph1, cs)"
		    " ***");
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
END_MAIN
