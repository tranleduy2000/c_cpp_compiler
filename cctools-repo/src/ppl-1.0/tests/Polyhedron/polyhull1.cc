/* Test Polyhedron::upper_bound_assign().
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
  gs1.insert(point());
  gs1.insert(ray(x));
  gs1.insert(ray(y));

  C_Polyhedron ph1(gs1);

  print_generators(ph1, "*** ph1 ***");

  Generator_System gs2;
  gs2.insert(point(-x + y));
  gs2.insert(point(x + y));
  gs2.insert(point(3*x));

  C_Polyhedron ph2(gs2);

  print_generators(ph2, "*** ph2 ***");

  C_Polyhedron computed_result = ph1;

  computed_result.upper_bound_assign(ph2);

  Generator_System gs_known_result;
  gs_known_result.insert(point());
  gs_known_result.insert(point(-x + y));
  gs_known_result.insert(ray(x));
  gs_known_result.insert(ray(y));

  C_Polyhedron known_result(gs_known_result);

  print_generators(computed_result, "*** ph1.upper_bound_assign(ph2) ***");

  return computed_result == known_result;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, EMPTY);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(x + y));
  C_Polyhedron ph2(gs);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  C_Polyhedron computed_result1(ph1);

  computed_result1.upper_bound_assign(ph2);

  C_Polyhedron known_result(ph2);

  bool ok = (computed_result1 == known_result);

  print_generators(computed_result1,
		   "*** after upper_bound_assign ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(x <= 2);
  ph1.add_constraint(y <= 2);

  C_Polyhedron ph2(2);
  ph2.add_constraint(y >= 2);
  ph2.add_constraint(y <= 4);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.upper_bound_assign(ph2);

  print_generators(ph1, "*** after upper_bound_assign ***");

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(point(2*x));
  known_result.add_generator(point(4*y));
  known_result.add_generator(point(2*x + 4*y));

  bool ok = (ph1 == known_result);

  return ok;
}

bool
aux_test04(C_Polyhedron& ph1, const C_Polyhedron& ph2,
	   // Note intentional call-by-value!
	   C_Polyhedron known_result) {
  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.upper_bound_assign(ph2);

  print_generators(ph1, "*** after upper_bound_assign ***");

  return ph1 == known_result;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1_1(2);
  ph1_1.add_constraint(x >= 0);
  ph1_1.add_constraint(y >= 0);
  ph1_1.add_constraint(x <= 2);
  ph1_1.add_constraint(y <= 2);
  C_Polyhedron ph1_2(ph1_1);

  C_Polyhedron ph2_1(2);
  ph2_1.add_constraint(x+y <= 0);
  ph2_1.add_constraint(x+y >= 2);
  C_Polyhedron ph2_2(ph2_1);
  C_Polyhedron ph2_3(ph2_1);
  C_Polyhedron ph2_4(ph2_1);

  bool ok = aux_test04(ph1_1, ph2_1, ph1_1)
    && aux_test04(ph2_2, ph1_2, ph1_2)
    && aux_test04(ph2_3, ph2_4, ph2_3);

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(A));
  ph1.add_generator(ray(A));
  ph1.add_generator(ray(B));
  C_Polyhedron ph2(2, EMPTY);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  ph1.upper_bound_assign(ph2);

  bool ok = (ph1 == known_result);

  print_generators(ph1,
		   "*** after ph1.upper_bound_assign(ph2) ***");

  return ok;
}

bool
test06() {
  C_Polyhedron ph1;
  C_Polyhedron ph2;

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  ph1.upper_bound_assign(ph2);

  bool ok = (ph1 == known_result);

  print_generators(ph1,
		   "*** after ph1.upper_bound_assign(ph2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(point(B));
  C_Polyhedron ph1(gs1);
  ph1.generators();
  ph1.constraints();

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(B));
  C_Polyhedron ph2(gs2);

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.upper_bound_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (ph1 == known_result);

  print_generators(ph1,
		   "*** after ph1.upper_bound_assign(ph2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A - B >= 0);
  C_Polyhedron ph2(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  ph1.upper_bound_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.upper_bound_assign(ph2) ***");

  return ok;
}

bool
test09() {
  C_Polyhedron ph1;
  C_Polyhedron ph2;

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  ph1.upper_bound_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.upper_bound_assign(ph2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.generators();
  ph1.add_constraint(A == B);
  C_Polyhedron copy_ph1 = ph1;

  C_Polyhedron ph2(2);
  ph2.generators();
  ph2.add_constraint(A >= B + 1);
  C_Polyhedron copy_ph2 = ph2;

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.upper_bound_assign(ph2);
  copy_ph1.upper_bound_assign(copy_ph2);

  bool ok = (ph1 == copy_ph1);

  print_generators(ph1, "*** after upper_bound_assign ***");
  print_generators(copy_ph1,
		    "*** after upper_bound_assign ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point());
  ph1.constraints();
  ph1.add_generator(line(A + B));
  C_Polyhedron copy_ph1 = ph1;

  C_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point());
  ph2.constraints();
  ph2.add_generator(ray(A));
  ph2.add_generator(ray(B));

  C_Polyhedron copy_ph2 = ph2;

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.upper_bound_assign(ph2);
  copy_ph1.upper_bound_assign(copy_ph2);

  bool ok = (ph1 == copy_ph1);

  print_generators(ph1, "*** after upper_bound_assign ***");
  print_generators(copy_ph1,
		    "*** after upper_bound_assign ***");

  return ok;
}

bool
test12() {
  // Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron p(3);
  p.add_constraint(B >= 0);
  p.add_constraint(C >= 0);

  C_Polyhedron q(3);
  q.add_constraint(C >= 0);

  print_constraints(p, "*** p ***");
  print_constraints(q, "*** q ***");

  p.upper_bound_assign(q);

  bool ok = (p == q);

  print_constraints(p, "*** p.upper_bound_assign(q) ***");

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
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
END_MAIN
