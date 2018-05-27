/* Test BD_Shape::BD_Shape(const Box<Interval>&).
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

// Universe box.
bool
test01() {
  Rational_Box box(2);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// A 2D box which is a line parallel to the x axis.
bool
test02() {
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(3*B == 2);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);
  known_bds.add_constraint(3*B == 2);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// A 2D box that is a point, with divisors.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(3*A == -2);
  box.add_constraint(B == -10);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);
  known_bds.add_constraint(3*A == -2);
  known_bds.add_constraint(B == -10);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");
  print_constraints(known_bds, "*** bds(box) ***");

  return ok;
}

// A 3D box which is a 2D plane.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Rational_Box box(3);
  box.add_constraint(5*C == 15);

  TBD_Shape bds(box);

  TBD_Shape known_bds(3);
  known_bds.add_constraint(5*C == 15);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// Zero-dimensional box.
bool
test05() {
  Rational_Box box(0);

  TBD_Shape bds(box);

  TBD_Shape known_bds;

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// Empty box in 2D.
bool
test06() {
  Rational_Box box(2);
  box.set_empty();

  TBD_Shape bds(box);

  TBD_Shape known_bds(2, EMPTY);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// A 4D box containing a single 3D space.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Rational_Box box(4);
  box.add_constraint(D == 4);

  TBD_Shape bds(box);

  TBD_Shape known_bds(4);
  known_bds.add_constraint(D == 4);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// Unit square.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(A <= 1);
  box.add_constraint(B >= 0);
  box.add_constraint(B <= 1);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);
  known_bds.add_constraint(A >= 0);
  known_bds.add_constraint(A <= 1);
  known_bds.add_constraint(B >= 0);
  known_bds.add_constraint(B <= 1);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// Simple box with divisor and an interval bounded only from below.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(2*B == 1);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);
  known_bds.add_constraint(A >= 0);
  known_bds.add_constraint(2*B == 1);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// Box with a dimension bounded only from above.
bool
test10() {
  Variable A(0);
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(7*A <= 3);
  box.add_constraint(2*B == 1);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);
  known_bds.add_constraint(7*A <= 3);
  known_bds.add_constraint(2*B == 1);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// A box having a dimension with an open bound, where
// the open bound makes the box empty.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(7*A == 3);
  box.add_constraint(2*B >= 1);
  box.add_constraint(2*B <= 0);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2, EMPTY);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// Zero-dimensional empty box.
bool
test12() {
  Rational_Box box(0);
  box.set_empty();

  TBD_Shape bds(box);

  TBD_Shape known_bds(0, EMPTY);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// A box from a higher dimension.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  Rational_Box box(6);
  box.add_constraint(3*A == -2);
  box.add_constraint(4*B == -11);
  box.add_constraint(3*D == 18);
  box.add_constraint(7*E == 15);
  box.add_constraint(7*F == -15);

  TBD_Shape bds(box);

  TBD_Shape known_bds(6);
  known_bds.add_constraint(3*A == -2);
  known_bds.add_constraint(4*B == -11);
  known_bds.add_constraint(3*D == 18);
  known_bds.add_constraint(7*E == 15);
  known_bds.add_constraint(7*F == -15);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

// A box having a dimension with an open bound, where
// the open bound does not make the box empty.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Rational_Box box(2);
  box.add_constraint(7*A == 3);
  box.add_constraint(2*B >= 1);
  box.add_constraint(B >= 1);

  TBD_Shape bds(box);

  TBD_Shape known_bds(2);
  known_bds.add_constraint(7*A == 3);
  known_bds.add_constraint(2*B >= 1);
  known_bds.add_constraint(B >= 1);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

  return ok;
}

bool
test15() {
  Variable A(0);

  Rational_Box box(1);
  box.add_constraint(A >= 5);
  box.add_constraint(A <= -5);

  TBD_Shape bds(box);

  TBD_Shape known_bds(1, EMPTY);

  bool ok = (bds == known_bds);

  print_constraints(bds, "*** bds(box) ***");

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
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
END_MAIN
