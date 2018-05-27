/* Test NNC_Polyhedron::contains(const Polyhedron&).
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

  // Building a square without vertices.
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(y >= 0);
  cs.insert(y <= 1);
  cs.insert(x + y > 0);
  cs.insert(x + y < 2);
  cs.insert(x - y < 1);
  cs.insert(x - y > -1);

  NNC_Polyhedron ph1(cs);

  // Building an equivalent square.
  Generator_System gs;
  gs.insert(closure_point());
  gs.insert(closure_point(x));
  gs.insert(closure_point(y));
  gs.insert(closure_point(x + y));
  gs.insert(point(x, 10));
  gs.insert(point(y, 10));
  gs.insert(point(x + 10*y, 10));
  gs.insert(point(10*x + y, 10));

  NNC_Polyhedron ph2(gs);

  bool ok = ph1.contains(ph2);

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A > 0);
  ph1.add_constraint(B > 0);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(line(A + B));
  NNC_Polyhedron ph2(gs2);

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  bool ok = !ph1.contains(ph2);

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= B);
  cs.insert(B >= 0);
  C_Polyhedron ph1(cs);
  C_Polyhedron ph2(ph1);
  ph2.add_constraint(A == B);

  bool ok = ph1.contains(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= B);
  cs.insert(B >= 0);
  NNC_Polyhedron ph1(cs);
  NNC_Polyhedron ph2(ph1);
  ph2.add_constraint(A > B);

  bool ok = ph1.strictly_contains(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs1;
  cs1.insert(A >= B);
  cs1.insert(B >= 0);
  C_Polyhedron ph1(cs1);
  Constraint_System cs2;
  cs2.insert(A >= B);
  cs2.insert(A <= 0);
  C_Polyhedron ph2(cs2);

  bool ok = (ph1 != ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B >= 0);
  NNC_Polyhedron ph1(cs);
  NNC_Polyhedron ph2(ph1);
  ph2.add_constraint(A > 0);

  bool ok = ph1.strictly_contains(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

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
