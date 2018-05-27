/* Test Octagonal_Shape::poly_hull_assign().
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
  Variable x1(0);
  Variable x2(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(x1 >= 1);
  oct1.add_constraint(x1 - x2 <= 3);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(x2 <= 1);
  oct2.add_constraint(x1 - x2 <= 2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x1 - x2 <= 3);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test02() {
  Variable x1(0);
  Variable x2(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(x1 >= 0);
  oct1.add_constraint(x1 + x2 >= 1);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(x2 >= 1);
  oct2.add_constraint(x1 + x2 >= 3);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x1 + x2 >= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);

  TOctagonal_Shape oct1(5);
  oct1.add_constraint(x1 <= 5);
  oct1.add_constraint(x2 <= -1);
  oct1.add_constraint(x1 - x2 <= 10);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(5);
  oct2.add_constraint(x1  <= 2);
  oct2.add_constraint(x4 <= 7);
  oct2.add_constraint(x1 - x2 <= 20);
  oct2.add_constraint(x4 - x3 <= 3);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(5);
  known_result.add_constraint(x1 <= 5);
  known_result.add_constraint(x1 - x2 <= 20);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test04() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(x1 <= 1);
  oct1.add_constraint(- x1 - x2 >= 1);
  oct1.add_constraint(x3 == 2 );

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(x2  >= 1);
  oct2.add_constraint(x1 + x2 <= 2);
  oct2.add_constraint(- x1 - x3 <= 2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x1 <= 1);
  known_result.add_constraint(-x3 <= 3);
  known_result.add_constraint(x1 + x2 <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A <= 1);
  oct1.add_constraint(B >= 7);
  oct1.add_constraint(C <= 3);
  oct1.add_constraint(A >= 3);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(A  >= 1);
  oct2.add_constraint(B <= 2);
  oct2.add_constraint(C <= 2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(oct2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs1;
  cs1.insert(A == 1);
  cs1.insert(C - A <= 2);
  cs1.insert(C - B <= 7);
  cs1.insert(B - A <= 3);
  TOctagonal_Shape oct1(cs1);

  print_constraints(oct1, "*** oct1 ***");

  Constraint_System cs2;
  cs2.insert(A == 1);
  cs2.insert(C - A <= 3);
  cs2.insert(C - A >= 7);
  cs2.insert(B - A <= 2);

  TOctagonal_Shape oct2(3);
  oct2.add_constraints(cs2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(cs1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test07() {
  TOctagonal_Shape oct1(0, EMPTY);
  TOctagonal_Shape oct2;

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result;

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test08() {
  TOctagonal_Shape oct1(8);
  TOctagonal_Shape oct2(8);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(8);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test09() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(x1 <= 2);
  oct1.add_constraint(- x1 - x2 >= 1);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(x2  >= 1);
  oct2.add_constraint(x1 + x2 <= 2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x1 <= 2);
  known_result.add_constraint(x1 + x2 <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

  return ok;
}

bool
test10() {
  TOctagonal_Shape oc1(12);
  TOctagonal_Shape oc2(5);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::upper_bound_assign(oc2): it is impossible to apply
    // this method to two polyhedra of different dimensions.
    oc1.upper_bound_assign(oc2);
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

// CHEKME: is this a duplication of test10?
bool
test11() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x >= y);

  TOctagonal_Shape oc2(3);

  try {
    // This is an invalid use of method
    // Octagonal_Shape::upper_bound_assign(oc2): it is illegal
    // to apply this method to two polyhedra of different dimensions.
    oc1.upper_bound_assign(oc2);
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
test12() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(x1 <= 1);
  oct1.add_constraint(- x1 - x2 >= 1);
  oct1.add_constraint(x3 == 2 );

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(x2  >= 1);
  oct2.add_constraint(x1 + x2 <= 2);
  oct2.add_constraint(- x1 - x3 <= 2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.upper_bound_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x1 <= 1);
  known_result.add_constraint(-x3 <= 3);
  known_result.add_constraint(x1 + x2 <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.upper_bound_assign(oct2) ***");

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

