/* Test Polyhedron::upper_bound_assign_if_exact().
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
  C_Polyhedron ph_empty(0, EMPTY);
  C_Polyhedron ph_universe(0, UNIVERSE);

  // Testing all combinations for 0-dim polyhedra.
  bool ok = true;
  C_Polyhedron ph;

  // empty, empty
  ph = ph_empty;
  ok &= ph.upper_bound_assign_if_exact(ph_empty);
  ok &= (ph == ph_empty);
  print_constraints(ph, "*** empty union empty ***");

  // empty, universe
  ph = ph_empty;
  ok &= ph.upper_bound_assign_if_exact(ph_universe);
  ok &= (ph == ph_universe);
  print_constraints(ph, "*** empty union universe ***");

  // universe, empty
  ph = ph_universe;
  ok &= ph.upper_bound_assign_if_exact(ph_empty);
  ok &= (ph == ph_universe);
  print_constraints(ph, "*** universe union empty ***");

  // universe, universe
  ph = ph_universe;
  ok &= ph.upper_bound_assign_if_exact(ph_universe);
  ok &= (ph == ph_universe);
  print_constraints(ph, "*** universe union universe ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= -2);
  ph1.add_constraint(x <= -1);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 1);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y >= 0);
  ph2.add_constraint(y <= 2);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= -2);
  ph1.add_constraint(x <= 0);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y >= 0);
  ph2.add_constraint(y <= 2);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(2, UNIVERSE);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x == 0);
  ph1.add_constraint(y == 0);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y >= -2);
  ph2.add_constraint(y <= 2);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(y == 0);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(y >= 2);
  ph2.add_constraint(y <= 4);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x == y);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x == 0);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= y);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 0);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= y);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x <= y);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(2, UNIVERSE);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test09() {
  Variable w(0);
  Variable x(1);
  Variable y(2);
  Variable z(3);

  Constraint_System cs;
  cs.insert(4*x - 2*y - z + 14 >= 0);
  cs.insert(4*x + 2*y - z + 2 >= 0);
  cs.insert(x + y - 1 >= 0);
  cs.insert(x + y + 2*z - 5 >= 0);
  cs.insert(x + 1 >= 0);
  cs.insert(x + z - 1 >= 0);
  cs.insert(2*x + y -2*z + 7 >= 0);
  cs.insert(x - y + 2*z + 1 >= 0);
  cs.insert(x - y + 5 >= 0);
  cs.insert(2*x - y - 2*z + 13 >= 0);
  cs.insert(-2*x - y + 2*z + 1 >= 0);
  cs.insert(-x + y - 1 >= 0);
  cs.insert(-x + y -2*z + 7 >= 0);
  cs.insert(-4*x + 2*y + z - 4 >= 0);
  cs.insert(-2*x + y + 2*z - 5 >= 0);
  cs.insert(-x + 1 >= 0);
  cs.insert(-x - z + 5 >= 0);
  cs.insert(-4*x - 2*y + z + 8 >= 0);
  cs.insert(-x - y + 5 >= 0);
  cs.insert(-x - y -2*z +13 >= 0);

  C_Polyhedron icosahedron1(4);
  icosahedron1.add_constraints(cs);
  icosahedron1.add_constraint(w >= 0);
  icosahedron1.add_constraint(w <= 5);

  C_Polyhedron icosahedron2(4);
  icosahedron2.add_constraints(cs);
  icosahedron2.add_constraint(w >= -7);
  icosahedron2.add_constraint(w <= 2);

  C_Polyhedron known_result(4);
  known_result.add_constraints(cs);
  known_result.add_constraint(w >= -7);
  known_result.add_constraint(w <= 5);

  C_Polyhedron computed_result = icosahedron1;
  bool ok = computed_result.poly_hull_assign_if_exact(icosahedron2);
  ok &= (computed_result == known_result);

  print_constraints(icosahedron1, "*** icosahedron1 ***");
  print_constraints(icosahedron2, "*** icosahedron2 ***");
  print_constraints(computed_result, "*** computed_result ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test10() {
  Variable X01(0);
  Variable X02(1);
  Variable X03(2);
  Variable X04(3);
  Variable X05(4);
  Variable X06(5);
  Variable X07(6);
  Variable X08(7);
  Variable X09(8);
  Variable X10(9);
  Variable X11(10);
  Variable X12(11);
  Variable X13(12);
  Variable X14(13);
  Variable X15(14);
  Variable X16(15);
  Variable X17(16);
  Variable X18(17);
  Variable X19(18);
  Variable X20(19);
  Variable X21(20);
  Variable X22(21);
  Variable X23(22);
  Variable X24(23);
  Variable X25(24);
  Variable X26(25);
  Variable X27(26);
  Variable X28(27);
  Variable X29(28);
  Variable X30(29);
  Variable X31(30);
  Variable X32(31);
  Variable X33(32);
  Variable X34(33);
  Variable X35(34);
  Variable X36(35);
  Variable X37(36);
  Variable X38(37);
  Variable X39(38);

  Constraint_System cs;
  cs.insert(X01 - X02 - X03 + 0*X39 == 0);
  cs.insert(Coefficient("2386907802506363")*X01 - X04 == 0);
  cs.insert(-X01 >= -80);
  cs.insert(X02 - Coefficient("3152519739159347")*X14 >= 0);
  cs.insert(X06 + X07 + X08 + X09 - X14 - X15 == 0);
  cs.insert(Coefficient("2386907802506363")*X06
	    + Coefficient("2386907802506363")*X07
	    + Coefficient("1080863910568919")*X08
	    + Coefficient("7746191359077253")*X09
	    - X16 == 0);
  cs.insert(-X06 + X10 >= -80);
  cs.insert(-X07 + X11 >= 0);
  cs.insert(-X08 + X12 >= 0);
  cs.insert(-X09 + X13 >= 0);
  cs.insert(X22 - X23 - X24 - X25 == 0);
  cs.insert(Coefficient("7746191359077253")*X22 - X26 == 0);
  cs.insert(-X22 >= -500);
  cs.insert(X23 - Coefficient("3152519739159347")*X36 >= 0);
  cs.insert(Coefficient("7746191359077253")*X28
	    + Coefficient("7746191359077253")*X29
	    + Coefficient("3512807709348987")*X30
	    + Coefficient("3332663724254167")*X31
	    - X38 == 0);
  cs.insert(X28 + X29 + X30 + X31 - X36 + X37 + X39 == 44);
  cs.insert(-X28 + X32 >= -500);
  cs.insert(-X29 + X33 >= 0);
  cs.insert(-X30 + X34 >= 0);
  cs.insert(-X31 + X35 >= 0);
  cs.insert(Coefficient("-2661627379775963")*X10
	    - Coefficient("2686397177726501")*X11
	    - Coefficient("5422333951354077")*X12
	    - Coefficient("5469621747441467")*X13
	    + X25
	    - Coefficient("2466846695892189")*X32
	    - Coefficient("4996743786567565")*X33
	    - Coefficient("5064297780978123")*X34
	    - Coefficient("641481471923585")*X35 >= 0);
  cs.insert(X03 - Coefficient("7854277750134145")*X22 >= 0);
  cs.insert(X15
	    - Coefficient("7854277750134145")*X28
	    - Coefficient("7782220156096217")*X29
	    - Coefficient("7782220156096217")*X30
	    - Coefficient("7710162562058289")*X31 >= 0);
  cs.insert(Coefficient("-5422333951354077")*X01 + X24 >= 0);
  cs.insert(X21 >= 2);
  cs.insert(-X16 - X38 >= -300);
  for (dimension_type i = X01.id(); i <= X39.id(); ++i)
    cs.insert(Variable(i) >= 0);

  C_Polyhedron ph1(cs);
  ph1.add_constraint(X25 - X22 <= 5);

  C_Polyhedron ph2(cs);
  ph1.add_constraint(X25 - X22 >= 1);

  C_Polyhedron known_result(cs);

  bool ok = ph1.poly_hull_assign_if_exact(ph2);
  ok &= (ph1 == known_result);
  return ok;
}

bool
test11() {
  const dimension_type dim = 5;
  Constraint_System cs;
  for (dimension_type i = 1; i < dim; ++i) {
    Variable x(i);
    cs.insert(x >= 0);
    cs.insert(x <= 4);
  }

  Variable x(0);

  C_Polyhedron hypercube1(cs);
  hypercube1.add_constraint(x >= 0);
  hypercube1.add_constraint(x <= 4);

  C_Polyhedron hypercube2(cs);
  hypercube2.add_constraint(x >= 2);
  hypercube2.add_constraint(x <= 6);

  bool ok = hypercube1.upper_bound_assign_if_exact(hypercube2);

  print_generators(hypercube1, "*** hyp1 ***");
  print_generators(hypercube2, "*** hyp2 ***");

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
  DO_TEST_F8(test09);
  DO_TEST_F64(test10);
  DO_TEST(test11);
END_MAIN
