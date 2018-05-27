/* Test Octagonal_Shape::contains().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
  TOctagonal_Shape oc1;
  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2;
  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return contained;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(C >= 0);
  oc1.add_constraint(B + C >= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(A == 0);
  oc2.add_constraint(C >= 0);
  oc2.add_constraint(B + C >= 1);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return contained;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(D >= 0);
  cs.insert(C >= 0);
  cs.insert(B >= 0);
  cs.insert(A >= 0);

  TOctagonal_Shape oc1(cs);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(cs);
  oc2.add_constraint(A + B >= 0);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return contained;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs1;
  cs1.insert(A + D >= 0);
  cs1.insert(C >= 0);
  cs1.insert(B >= 0);
  cs1.insert(A == 0);
  TOctagonal_Shape oc1(cs1);

  print_constraints(oc1, "*** oc1 ***");

  Constraint_System cs2;
  cs2.insert(D == 0);
  cs2.insert(C == 0);
  cs2.insert(B == 0);
  cs2.insert(A == 0);
  TOctagonal_Shape oc2(cs2);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return contained;
}

bool
test05() {
  TOctagonal_Shape oc1(0);
  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(0, EMPTY);
  print_constraints(oc2, "*** oc2 ***");

  TOctagonal_Shape oc3(0);
  print_constraints(oc3, "*** oc3 ***");

  bool contained1 = oc1.contains(oc2);
  bool contained2 = oc1.contains(oc3);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained1 ? "true" : "false") << endl;
  nout << "*** oc1.contains(oc3) ***" << endl;
  nout << (contained2 ? "true" : "false") << endl;

  return contained1 && contained2;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs1;
  cs1.insert(C + D == 0);
  cs1.insert(A - B <= 0);
  cs1.insert(A == 0);
  TOctagonal_Shape oc1(4);
  oc1.add_constraints(cs1);

  print_constraints(oc1, "*** oc1 ***");

  Constraint_System cs2(cs1);
  cs2.insert(D == 0);
  TOctagonal_Shape oc2(4);
  oc2.add_constraints(cs2);

  print_constraints(oc2, "*** oc2 ***");

  Constraint_System cs3(cs2);
  cs3.insert(B == 0);
  cs3.insert(B + C <= 0);
  TOctagonal_Shape oc3(4);
  oc3.add_constraints(cs3);

  print_constraints(oc3, "*** oc3 ***");

  bool contained1 = oc1.contains(oc2);
  bool contained2 = oc1.contains(oc3);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained1 ? "true" : "false") << endl;
  nout << "*** oc1.contains(oc3) ***" << endl;
  nout << (contained2 ? "true" : "false") << endl;

  return contained1 && contained2;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs1;
  cs1.insert(A - B <= 0);
  TOctagonal_Shape oc1(4);
  oc1.add_constraints(cs1);

  print_constraints(oc1, "*** oc1 ***");

  Constraint_System cs2(cs1);
  cs2.insert(D == 0);
  cs2.insert(A - B <= 0);
  cs2.insert(B - C <= 3);
  cs2.insert(C - D <= -1);
  cs2.insert(D - A <= -3);
  TOctagonal_Shape oc2(4);

  // The octagon oc2 is empty, since it has a negative cycle.
  oc2.add_constraints(cs2);

  print_constraints(oc2, "*** oc2 ***");

  Constraint_System cs3(cs1);
  cs3.insert(B <= 2);
  cs3.insert(B >= 5);
  TOctagonal_Shape oc3(4);

  // The octagon oc3 is empty.
  oc3.add_constraints(cs3);

  print_constraints(oc3, "*** oc3 ***");

  bool contained1 = oc1.contains(oc2);
  bool contained2 = oc1.contains(oc3);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained1 ? "true" : "false") << endl;
  nout << "*** oc1.contains(oc3) ***" << endl;
  nout << (contained2 ? "true" : "false") << endl;

  return contained1 && contained2;
}

bool
test08() {
  TOctagonal_Shape oc1(3, UNIVERSE);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3, EMPTY);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);
  bool strictly_contained = oc1.strictly_contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  nout << "*** oc1.strictly_contains(oc2) ***" << endl;
  nout << (strictly_contained ? "true" : "false") << endl;

  return contained && strictly_contained;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x - y >= 0);

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x - y == 0);

  try {
    // This is an invalid use of Polyhedron::contains(): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    oc1.contains(oc2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test10() {
  TOctagonal_Shape oc1(0, EMPTY);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(0);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (!contained ? "true" : "false") << endl;

  return !contained;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(C <= 0);
  oc1.add_constraint(B + C <= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(A == 0);
  oc2.add_constraint(C <= 0);
  oc2.add_constraint(B + C <= 2);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (!contained ? "true" : "false") << endl;

  return !contained;
}

bool
test12() {
  TOctagonal_Shape oc1(3, EMPTY);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3, UNIVERSE);

  print_constraints(oc2, "*** oc2 ***");

  bool contained = oc1.contains(oc2);

  nout << "*** oc1.contains(oc2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return !contained;
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
