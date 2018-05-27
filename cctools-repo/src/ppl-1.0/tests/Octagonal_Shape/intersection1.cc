/* Test Octagonal_Shape::intersection_assign().
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

  TOctagonal_Shape oc1(3);

  oc1.add_constraint(x <= 3);
  oc1.add_constraint(x - y <= 4);
  oc1.add_constraint(x + y <= 6);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(-y <= -2);
  oc2.add_constraint(x - y <= 5);
  oc2.add_constraint(x + y <= 7);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 3);
  known_result.add_constraint(-y <= -2);
  known_result.add_constraint(x - y <= 4);
  known_result.add_constraint(x + y <= 6);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A <= 2);
  oc1.add_constraint(A - B <= -1);
  oc1.add_constraint(A + B <= 10);
  oc1.add_constraint(A + C <= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(C <= 2);
  oc2.add_constraint(A - B <= 5);
  oc2.add_constraint(A - C <= 5);
  oc2.add_constraint(A + B <= 7);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(A - B <= -1);
  known_result.add_constraint(A + B <= 7);
  known_result.add_constraint(A - C <= 5);
  known_result.add_constraint(A + C <= 1);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A <= 5);
  oc1.add_constraint(A - B <= -1);
  oc1.add_constraint(A + B <= -1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(C <= 2);
  oc2.add_constraint(A - B <= 4);
  oc2.add_constraint(A + B <= 7);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(A - B <= -1);
  known_result.add_constraint(A + B <= -1);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A <= 5);
  oc1.add_constraint(A - B <= -1);
  oc1.add_constraint(A - B >= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(A - B <= 4);
  oc2.add_constraint(A + B <= 7);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test05() {
  TOctagonal_Shape oc1(0, EMPTY);
  TOctagonal_Shape oc2;

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(0, EMPTY);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test06() {
  TOctagonal_Shape oc1;
  TOctagonal_Shape oc2;

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(0);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x >= y);

  TOctagonal_Shape oc2(3);

  try {
    // This is an invalid use of method
    // Octagonal_Shape::intersection_assign(oc2): it is illegal
    // to apply this method to two polyhedra of different dimensions.
    oc1.intersection_assign(oc2);
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
test08() {
  Variable A(0);
  Variable B(1);
  // Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A <= 5);
  oc1.add_constraint(A + B <= 1);
  oc1.add_constraint(A + B >= -1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3, EMPTY);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  oc1.intersection_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A <= 5);
  cs.insert(A - B <= -1);
  cs.insert(A + B <= -1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraints(cs);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(C <= 2);
  oc2.add_constraint(A - B <= -2);
  oc2.add_constraint(A + B <= -3);

  print_constraints(oc2, "*** oc2 ***");

  oc1.intersection_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(C <= 2);
  known_result.add_constraint(A - B <= -2);
  known_result.add_constraint(A + B <= -3);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.intersection_assign(oc2) ***");

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
END_MAIN
