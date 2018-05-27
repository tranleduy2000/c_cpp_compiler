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

bool
test01() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(x <= 2);
  ph1.add_constraint(y <= 2);

  C_Polyhedron ph2(2);
  // Add inconsistent constraints to ph2.
  ph2.add_constraint(x+y <= 0);
  ph2.add_constraint(x+y >= 2);

  C_Polyhedron ph1_1(ph1);
  print_constraints(ph1_1, "*** ph1_1 ***");
  C_Polyhedron ph2_1(ph2);
  print_constraints(ph2_1, "*** ph2_1 ***");
  ph1_1.H79_widening_assign(ph2_1);
  print_generators(ph1_1, "*** after H79_widening_assign ***");
  C_Polyhedron ph1_2(ph1);
  bool ok = (ph1_1 == ph1_2);

  C_Polyhedron ph2_2(ph2);
  print_constraints(ph2_2, "*** ph2_2 ***");
  C_Polyhedron ph2_3(ph2);
  print_constraints(ph2_3, "*** ph2_3 ***");
  ph2_2.H79_widening_assign(ph2_3);
  print_generators(ph2_2, "*** after H79_widening_assign ***");
  C_Polyhedron ph2_4(ph2);
  ok = ok && (ph2_2 == ph2_4);

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 2);
  ph1.add_constraint(B >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(A-B >= 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.H79_widening_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after H79_widening_assign ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(ray(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(A));
  gs2.insert(ray(A + 2*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.H79_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after ph2.H79_widening_assign(ph1) ***");

  return ok;
}

bool
test04() {
  C_Polyhedron ph1;
  C_Polyhedron ph2(0, EMPTY);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.H79_widening_assign(ph2);

  C_Polyhedron known_result;
  known_result = ph1;

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.H79_widening_assign(ph2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(-A - 2*B >= -6);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(A - 2*B >= 2);

  C_Polyhedron ph2(2);
  ph2.add_constraint(-A - 2*B >= -10);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(A - 2*B >= 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph2.H79_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - 2*B >= 2);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after ph2.H79_widening_assign(ph1) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(-A + B >= 2);
  ph1.add_constraint(A >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(-A + B >= 3);
  ph2.add_constraint(A - B >= -8);
  ph2.add_constraint(A >= 1);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.H79_widening_assign(ph2);

  bool ok = ph1.is_universe();

  print_constraints(ph1, "*** after H79_widening_assign ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(B >= 0);
  // The addition of the following generator shows a bug
  // that was affecting PPL version 0.6.1 (now corrected).
  ph1.add_generator(line(A));

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(B >= 0);

  // Avoid computing the constraints of ph1.
  C_Polyhedron ph1_copy = ph1;
  print_constraints(ph1_copy, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.H79_widening_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after H79_widening_assign ***");

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
