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

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A > 1);
  ph1.add_constraint(A - B > 0);
  Generator_System gs;
  gs.insert(point(2*A));
  gs.insert(closure_point(A + B));
  gs.insert(ray(-B));
  gs.insert(ray(A + B));
  NNC_Polyhedron ph2(gs);

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.drop_some_non_integer_points();
  ph2.drop_some_non_integer_points();

  bool ok = (ph1 == ph2);

  print_constraints(ph1, "*** after ph1.drop_some_non_integer_points() ***");
  print_generators(ph2, "*** after ph2.drop_some_non_integer_points() ***");

  return ok;
}

bool
test02() {
  NNC_Polyhedron ph1;
  NNC_Polyhedron ph2(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result1(0, EMPTY);
  NNC_Polyhedron known_result2(ph2);

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

  NNC_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A > 0);
  ph.add_constraint(A == B);

  print_constraints(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A == B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.drop_some_non_integer_points() ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A));
  ph.constraints();
  ph.add_generator(closure_point());
  ph.add_generator(ray(A));
  ph.add_generator(ray(B));

  print_generators(ph, "*** ph ***");

  ph.drop_some_non_integer_points();

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);

  bool ok = (ph == known_result);

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
END_MAIN
