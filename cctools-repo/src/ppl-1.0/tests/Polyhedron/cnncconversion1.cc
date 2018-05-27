/* Test conversions between C_Polyhedron and NNC_Polyhedron.
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
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A - B >= 0);
  ph1.add_constraint(A >= 0);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(ph1);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A - B >= 0);
  known_result.add_constraint(A >= 0);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(3);
  ph1.add_constraint(x >= 5);
  ph1.add_constraint(y > x - 3);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(ph1);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(3);
  known_result.add_constraint(x >= 5);
  known_result.add_constraint(y >= x - 3);

  return ph2 == known_result;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A < 2);
  ph1.add_constraint(B > 0);
  ph1.add_constraint(A - B > 0);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(ph1);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - B >= 0);

  return ph2 == known_result;
}

bool
test04() {

  C_Polyhedron ph(4, UNIVERSE);

  NNC_Polyhedron nnc_ph(ph, POLYNOMIAL_COMPLEXITY);

  NNC_Polyhedron known_result(4);

  bool ok = (known_result == nnc_ph);

  print_constraints(nnc_ph, "*** nnc_ph ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(A - B > 4);
  cs.insert(-C - D >= 0);
  cs.insert(A + B - C == 0);
  cs.insert(-A + B - C < 4);
  cs.insert(A - B - C + D > 1);
  NNC_Polyhedron ph(cs);

  C_Polyhedron c_ph(ph, SIMPLEX_COMPLEXITY);

  Constraint_System cs1;
  cs1.insert(A - B >= 4);
  cs1.insert(-C - D >= 0);
  cs1.insert(A + B - C == 0);
  cs1.insert(-A + B - C <= 4);
  cs1.insert(A - B - C + D >= 1);
  C_Polyhedron  known_result(cs1);

  bool ok = (known_result == c_ph);

  print_constraints(c_ph, "*** c_ph ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
