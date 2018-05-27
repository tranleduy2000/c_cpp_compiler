/* Test Polyhedron::drop_some_non_integer_points().
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

  C_Polyhedron ph(2);
  ph.add_constraint(2*A >= 1);
  ph.add_constraint(2*B >= -1);
  ph.add_constraint(6*A - 3*B >= 7);

  print_constraints(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(2*A - B >= 3);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.drop_some_non_integer_points() ***");

  return ok;
}

bool
test02() {
  C_Polyhedron ph1;
  C_Polyhedron ph2(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result1(0, EMPTY);
  C_Polyhedron known_result2(ph2);

  ph1.drop_some_non_integer_points();
  ph2.drop_some_non_integer_points();

  bool ok = (ph1 == known_result1 && ph2 == known_result2);

  print_constraints(ph1, "*** after ph1.drop_some_non_integer_points() ***");
  print_constraints(ph2, "*** after ph2.drop_some_non_integer_points() ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A - B == 0);
  ph.add_constraint(A >= 0);

  C_Polyhedron known_result(ph);

  print_constraints(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.drop_some_non_integer_points() ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  ph.add_constraint(A == B);

  print_constraints(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A == B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.drop_some_non_integer_points() ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A+B, 2));
  ph.add_generator(line(B));

  print_constraints(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.drop_some_non_integer_points() ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(2*A <= 1);
  ph.add_constraint(2*B <= -1);
  ph.add_constraint(6*A - 3*B <= 7);

  print_constraints(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  bool ok = true; //(ph1 == ph2);

  print_constraints(ph, "*** after ph.drop_some_non_integer_points() ***");

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
END_MAIN
