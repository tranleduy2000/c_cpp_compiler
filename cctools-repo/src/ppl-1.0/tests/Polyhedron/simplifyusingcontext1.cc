/* Test Polyhedron::simplify_using_context_assign().
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

  C_Polyhedron icosahedron(3);
  icosahedron.add_constraint(4*x - 2*y - z + 14 >= 0);
  icosahedron.add_constraint(4*x + 2*y - z + 2 >= 0);
  icosahedron.add_constraint(x + y - 1 >= 0);
  icosahedron.add_constraint(x + y + 2*z - 5 >= 0);
  icosahedron.add_constraint(x + 1 >= 0);
  icosahedron.add_constraint(x + z - 1 >= 0);
  icosahedron.add_constraint(2*x + y -2*z + 7 >= 0);
  icosahedron.add_constraint(x - y + 2*z + 1 >= 0);
  icosahedron.add_constraint(x - y + 5 >= 0);
  icosahedron.add_constraint(2*x - y - 2*z + 13 >= 0);
  icosahedron.add_constraint(-2*x - y + 2*z + 1 >= 0);
  icosahedron.add_constraint(-x + y - 1 >= 0);
  icosahedron.add_constraint(-x + y -2*z + 7 >= 0);
  icosahedron.add_constraint(-4*x + 2*y + z - 4 >= 0);
  icosahedron.add_constraint(-2*x + y + 2*z - 5 >= 0);
  icosahedron.add_constraint(-x + 1 >= 0);
  icosahedron.add_constraint(-x - z + 5 >= 0);
  icosahedron.add_constraint(-4*x - 2*y + z + 8 >= 0);
  icosahedron.add_constraint(-x - y + 5 >= 0);
  icosahedron.add_constraint(-x - y -2*z +13 >= 0);

  C_Polyhedron column(3);
  column.add_constraint(y >= 2);
  column.add_constraint(y <= 4);
  column.add_constraint(x >= 0);
  column.add_constraint(x <= 1);

  C_Polyhedron computed_result = icosahedron;
  computed_result.simplify_using_context_assign(column);

  C_Polyhedron known_result(3);
  known_result.add_constraint(-4*x - 2*y + z >= -8);
  known_result.add_constraint(-4*x + 2*y + z >= 4);
  known_result.add_constraint(-2*x - y + 2*z >= -1);
  known_result.add_constraint(-2*x + y + 2*z >= 5);
  known_result.add_constraint(-x - y - 2*z >= -13);
  known_result.add_constraint(-x - z >= -5);
  known_result.add_constraint(-x + y - 2*z >= -7);

  bool ok = (computed_result == known_result);

  print_constraints(icosahedron, "*** icosahedron ***");
  print_constraints(column, "*** column ***");
  print_constraints(computed_result, "*** computed_result ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  C_Polyhedron icosahedron1(3);
  icosahedron1.add_constraint(4*x - 2*y - z + 14 >= 0);
  icosahedron1.add_constraint(4*x + 2*y - z + 2 >= 0);
  icosahedron1.add_constraint(x + y - 1 >= 0);
  icosahedron1.add_constraint(x + y + 2*z - 5 >= 0);
  icosahedron1.add_constraint(x + 1 >= 0);
  icosahedron1.add_constraint(x + z - 1 >= 0);
  icosahedron1.add_constraint(2*x + y -2*z + 7 >= 0);
  icosahedron1.add_constraint(x - y + 2*z + 1 >= 0);
  icosahedron1.add_constraint(x - y + 5 >= 0);
  icosahedron1.add_constraint(2*x - y - 2*z + 13 >= 0);
  icosahedron1.add_constraint(-2*x - y + 2*z + 1 >= 0);
  icosahedron1.add_constraint(-x + y - 1 >= 0);
  icosahedron1.add_constraint(-x + y -2*z + 7 >= 0);
  icosahedron1.add_constraint(-4*x + 2*y + z - 4 >= 0);
  icosahedron1.add_constraint(-2*x + y + 2*z - 5 >= 0);
  icosahedron1.add_constraint(-x + 1 >= 0);
  icosahedron1.add_constraint(-x - z + 5 >= 0);
  icosahedron1.add_constraint(-4*x - 2*y + z + 8 >= 0);
  icosahedron1.add_constraint(-x - y + 5 >= 0);
  icosahedron1.add_constraint(-x - y -2*z +13 >= 0);

  C_Polyhedron icosahedron2 = icosahedron1;
  icosahedron2.affine_image(x, x+5);


  C_Polyhedron computed_result = icosahedron1;
  computed_result.simplify_using_context_assign(icosahedron2);

  C_Polyhedron known_result(3);
  known_result.add_constraint(-4*x - 2*y + z >= -8);

  bool ok = (computed_result == known_result);

  print_constraints(icosahedron1, "*** icosahedron1 ***");
  print_constraints(icosahedron2, "*** icosahedron2 ***");
  print_constraints(computed_result, "*** computed_result ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test03() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x <= 6);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(1);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 5);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron computed_result = ph1;

  computed_result.simplify_using_context_assign(ph2);

  C_Polyhedron known_result(1);

  bool ok = (computed_result == known_result);

  print_constraints(computed_result,
                    "*** ph1.simplify_using_context_assign ***");

  return ok;
}

bool
test04() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph1 = C_Polyhedron(3, UNIVERSE);
  ph1.add_constraint(i >= 1);
  ph1.add_constraint(i <= 10);
  ph1.add_constraint(j >= 1);
  ph1.add_constraint(j <= 10);
  ph1.add_constraint(k == 0);

  C_Polyhedron ph2 = C_Polyhedron(3, UNIVERSE);
  ph2.add_constraint(i >= 0);
  ph2.add_constraint(i <= 2);
  ph2.add_constraint(j >= 2);
  ph2.add_constraint(j <= 9);
  ph2.add_constraint(k == 0);

  print_constraints(ph1, "=== ph1 ===");
  print_constraints(ph2, "=== ph2 ===");

  C_Polyhedron known_result = C_Polyhedron(3, UNIVERSE);
  known_result.add_constraint(i >= 1);

  ph1.simplify_using_context_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "=== ph1.simplify_using_context_assign(ph2) ===");

  return ok;
}

bool
test05() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph1(3, UNIVERSE);
  ph1.add_constraint(i == 1);
  ph1.add_constraint(j + 1 == 0);
  ph1.add_constraint(k == 3);

  C_Polyhedron ph2(3, UNIVERSE);
  ph2.add_constraint(i == 1);
  ph2.add_constraint(j + k == 2);
  ph2.add_constraint(k >= 0);
  ph2.add_constraint(k <= 3);

  C_Polyhedron known_result(3, UNIVERSE);
  known_result.add_constraint(k == 3);
  // PolyLib 5.22.3 does not simplify away the following equality.
  // known_result.add_constraint(j + 1 == 0);

  ph1.simplify_using_context_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "=== ph1.simplify_using_context_assign(ph2) ===");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph1(3, UNIVERSE);
  ph1.add_constraint(A == 0);
  ph1.add_constraint(B == C);
  ph1.add_constraint(B >= 2);
  print_constraints(ph1, "\n=== ph1 ===");

  C_Polyhedron ph2(3, UNIVERSE);
  ph2.add_constraint(A == 0);
  ph2.add_constraint(C >= 2);
  print_constraints(ph2, "\n=== ph2 ===");

  ph1.simplify_using_context_assign(ph2);

  C_Polyhedron known_result(3, UNIVERSE);
  known_result.add_constraint(B == C);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "\n=== ph1.simplify_using_context_assign(ph2) ===");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2);
  p.add_constraint(x == 0);
  p.add_constraint(y == 0);

  C_Polyhedron q(2);
  q.add_constraint(x >= 0);
  q.add_constraint(y >= 0);

  C_Polyhedron known_result(p);

  print_constraints(p, "*** p ***");
  print_constraints(q, "*** q ***");

  (void) p.simplify_using_context_assign(q);

  bool ok = (p == known_result);

  print_constraints(p, "*** p.simplify_using_context_assign(q) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2);
  p.add_constraint(x <= 0);
  p.add_constraint(y <= 0);

  C_Polyhedron q(2);
  q.add_constraint(x >= 0);
  q.add_constraint(y >= 0);

  C_Polyhedron known_result(p);

  print_constraints(p, "*** p ***");
  print_constraints(q, "*** q ***");

  (void) p.simplify_using_context_assign(q);

  bool ok = (p == known_result);

  print_constraints(p, "*** p.simplify_using_context_assign(q) ***");

  return ok;
}

bool
test09() {
  Variable A(0);

  C_Polyhedron ph1(1);
  C_Polyhedron ph2(1);

  ph2.add_constraint(A == 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result = ph1;

  ph1.simplify_using_context_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1,
                    "*** after ph1.simplify_using_context_assign(ph2) ***");

  return ok;
}

bool
test10() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph1(3, UNIVERSE);
  ph1.add_constraint(i >= 1);
  ph1.add_constraint(i <= 10);
  ph1.add_constraint(j >= 1);
  ph1.add_constraint(j <= 10);
  ph1.add_constraint(k == 0);

  C_Polyhedron ph2(3, UNIVERSE);
  ph2.add_constraint(i <= 25);
  ph2.add_constraint(j <= 25);
  ph2.add_constraint(i + j >= 25);
  ph2.add_constraint(k == 0);

  C_Polyhedron known_result(3, UNIVERSE);
  known_result.add_constraint(i <= 10);
  known_result.add_constraint(j <= 10);

  ph1.simplify_using_context_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1);

  return ok;
}

bool
test11() {
  C_Polyhedron ph1(0, EMPTY);
  C_Polyhedron ph2;

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result = ph1;

  ph1.simplify_using_context_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1,
            "*** after ph1.simplify_using_context_assign(ph2) ***");

  return ok;
}

bool
test12() {

  C_Polyhedron ph1(0, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(0, EMPTY);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(0, UNIVERSE);

  bool ok = !ph1.simplify_using_context_assign(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1,
                   "*** ph1.simplify_using_context_assign(ph2) ***");
  return ok;
}

bool
test13() {

  C_Polyhedron ph1(0, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(0, UNIVERSE);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(0, EMPTY);

  bool ok = !ph1.simplify_using_context_assign(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1,
                   "*** ph1.simplify_using_context_assign(ph2) ***");
  return ok;
}

bool
test14() {

  C_Polyhedron ph1(0, UNIVERSE);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(0, EMPTY);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(0, UNIVERSE);

  bool ok = !ph1.simplify_using_context_assign(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1,
                   "*** ph1.simplify_using_context_assign(ph2) ***");
  return ok;
}

bool
test15() {

  C_Polyhedron ph1(0, UNIVERSE);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(0, UNIVERSE);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(0, UNIVERSE);

  bool ok = ph1.simplify_using_context_assign(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1,
                   "*** ph1.simplify_using_context_assign(ph2) ***");
  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8A(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
END_MAIN
