/* Test Box::Box(const Polyhedron&, Complexity_Class).
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

// A non-bounded closed polyhedron consisting of the line x = y.
// The bounding box is the xy plane - the universal polyhedron.
bool
test01() {
  Variable x(0);
  Variable y(1);
  C_Polyhedron ph(2);
  ph.add_constraint(x - y >= 0);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2, UNIVERSE);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A non-bounded closed polyhedron in 2D consisting of a wedge bounded
// by y >= 0 and x >= y.
bool
test02() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(x >= y);
  ph.add_constraint(y >= 0);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2);
  known_box.add_constraint(x >= 0);
  known_box.add_constraint(y >= 0);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A bounded non-rectangular closed polyhedron in 2D.
bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(3*x + y >= 2);
  ph.add_constraint(x <= 4);
  ph.add_constraint(y <= 4);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2);
  known_box.add_constraint(3*x >= -2);
  known_box.add_constraint(x <= 4);
  known_box.add_constraint(y >= -10);
  known_box.add_constraint(y <= 4);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// An unbounded closed polyhedron in 4D but bounded in 2D.
bool
test04() {
  Variable x(1);
  Variable y(2);
  Variable z(3);

  C_Polyhedron ph(4);
  ph.add_constraint(3*x + y >= 2);
  ph.add_constraint(x <= 4);
  ph.add_constraint(y <= 4);
  ph.add_constraint(z >= 5);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(4);
  known_box.add_constraint(3*x >= -2);
  known_box.add_constraint(x <= 4);
  known_box.add_constraint(y >= -10);
  known_box.add_constraint(3*y <= 12);
  known_box.add_constraint(3*z >= 15);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// This is the universal, 2-dimensional closed polyhedron.
bool
test05() {
  C_Polyhedron ph(2);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2, UNIVERSE);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A zero-dimensional closed polyhedron.
bool
test06() {
  C_Polyhedron ph;

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(0);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// An empty closed polyhedron in 2D.
bool
test07() {
  C_Polyhedron ph(2, EMPTY);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(ph.space_dimension(), EMPTY);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A bounded polyhedron that is a single point.
bool
test08() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(x == 2);
  ph.add_constraint(y == 4);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2);
  known_box.add_constraint(x == 2);
  known_box.add_constraint(y == 4);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A unit square closed polyhedron.
bool
test09() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(y >= 0);
  cs.insert(y <= 1);

  C_Polyhedron ph(cs);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2);
  known_box.add_constraint(x >= 0);
  known_box.add_constraint(x <= 1);
  known_box.add_constraint(y >= 0);
  known_box.add_constraint(y <= 1);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A bounded rectangular closed polyhedron;
bool
test10() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(x >= 1);
  ph.add_constraint(x <= 3);
  ph.add_constraint(y <= 3);
  ph.add_constraint(y >= 1);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox nbox(ph);

  TBox known_box(2);
  known_box.add_constraint(x >= 1);
  known_box.add_constraint(x <= 3);
  known_box.add_constraint(y <= 3);
  known_box.add_constraint(y >= 1);

  bool ok = (nbox == known_box && pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(nbox, "*** nbox ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A bounded polyhedron having redundant constraints.
bool
test11() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(x == 3);
  ph.add_constraint(y == 1);
  ph.add_constraint(x + y == 4);
  ph.add_constraint(x - y == 2);
  ph.add_constraint(3*x + y == 10);
  ph.add_constraint(x >= 0);
  ph.add_constraint(y <= 5);
  ph.add_constraint(x + 2*y >= 5);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);

  TBox known_box(2);
  known_box.add_constraint(x == 3);
  known_box.add_constraint(y == 1);

  bool ok = (pbox == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(pbox, "*** pbox ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + 2*B <= 5);
  cs.insert(A + 2*B >= -10);
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(3*A - 5*B <= 18);
  C_Polyhedron ph(cs);

  TBox sbox(ph, SIMPLEX_COMPLEXITY);
  TBox nbox(ph, ANY_COMPLEXITY);

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(11*A <= 61);
  known_result.add_constraint(2*B <= 5);
  known_result.add_constraint(5*B >= -18);

  bool ok = (sbox == nbox
             && check_result(sbox, known_result,
                             "7.50e-7", "4.89e-7", "4.34e-7"));

  print_constraints(sbox, "*** sbox ***");
  print_constraints(nbox, "*** nbox ***");

  return ok;
}

bool
test13() {
  C_Polyhedron ph(1, EMPTY);

  TBox box(ph);

  TBox known_box(1, EMPTY);

  bool ok (box == known_box);

  print_constraints(ph, "*** ph ***");
  print_constraints(box, "*** box ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

bool
test14() {
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

  C_Polyhedron ph(cs);

  TBox pbox(ph, POLYNOMIAL_COMPLEXITY);
  TBox sbox(ph, SIMPLEX_COMPLEXITY);
  TBox nbox(ph, ANY_COMPLEXITY);

  Rational_Box known_result(2);

  bool ok = true;

  print_constraints(pbox, "*** pbox ***");
  print_constraints(sbox, "*** sbox ***");
  print_constraints(nbox, "*** nbox ***");

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
  DO_TEST_F8(test12);
  DO_TEST(test13);
  DO_TEST_F64(test14);
END_MAIN
