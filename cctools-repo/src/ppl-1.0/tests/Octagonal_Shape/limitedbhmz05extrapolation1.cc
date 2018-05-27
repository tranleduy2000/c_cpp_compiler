/* Test Octagonal_Shape::limited_BHMZ05_extrapolation_assign().
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

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y == 0);

  TOctagonal_Shape oct1(cs1);
  print_constraints(oct1, "*** oct1 ***");

  Constraint_System cs2;
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(y - x <= 0);

  TOctagonal_Shape oct2(cs2);
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(y <= -1);
  cs.insert(x <= 5);

  print_constraints(cs, "*** cs ***");

  oct2.limited_BHMZ05_extrapolation_assign(oct1, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y - x <= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(oct2,
                    "*** oct2.limited_BHMZ05_extrapolation_assign(oct1) ***");

   return ok;
}

bool
test02() {
  Variable x(0);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  TOctagonal_Shape oct1(cs1);

  Constraint_System cs2;
  cs2.insert(x == 0);
  TOctagonal_Shape oct2(cs2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(x >= 0);

  oct1.limited_BHMZ05_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(1);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_BHMZ05_extrapolation_assign(oct2) ***");

  return ok;
}

bool
test03() {
  TOctagonal_Shape oct1(0, EMPTY);
  TOctagonal_Shape oct2(0, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;

  oct1.limited_BHMZ05_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(0, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_BHMZ05_extrapolation_assign(oct2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(3, EMPTY);
  TOctagonal_Shape oct2(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A + B <= 0);

  oct1.limited_BHMZ05_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_BHMZ05_extrapolation_assign(oct2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A - B <= 1);

  TOctagonal_Shape oct2(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A - B <= 2);

  oct1.limited_BHMZ05_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A - B <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_BHMZ05_extrapolation_assign(oct2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);

  TOctagonal_Shape oct1(7);
  oct1.add_constraint(A - B <= 1);
  oct1.add_constraint(A - C <= 2);
  oct1.add_constraint(C - B <= 2);
  oct1.add_constraint(D - E == 0);
  oct1.add_constraint(G + F == 0);

  TOctagonal_Shape oct2(7);
  oct2.add_constraint(A - B <= 0);
  oct2.add_constraint(A - C <= 1);
  oct2.add_constraint(C - B <= 2);
  oct2.add_constraint(D - E == 0);
  oct2.add_constraint(G + F == 0);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(C - B <= 2);
  cs.insert(D - E == 0);
  cs.insert(F + G == 0);

  oct1.limited_BHMZ05_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(7);
  known_result.add_constraint(C - B <= 2);
  known_result.add_constraint(D - E == 0);
  known_result.add_constraint(F + G == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_BHMZ05_extrapolation_assign(oct2) ***");

  return ok;
}

bool
test07() {
  Variable y(1);

  TOctagonal_Shape oc1(1);
  TOctagonal_Shape oc2(2);

  Constraint_System cs;
  cs.insert(y <= 9);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::limited_BHMZ05_extrapolation_assign(oc2, cs): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    oc2.limited_BHMZ05_extrapolation_assign(oc1, cs);
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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x - y >= 0);
  oc1.add_constraint(x >= 0);
  oc1.add_constraint(x <= 2);

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x - y >= 0);
  oc2.add_constraint(x >= 0);
  oc2.add_constraint(x <= 5);

  Constraint_System cs;
  cs.insert(z <= 5);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::limited_BHMZ05_extrapolation_assign(oc, cs): it is
    // illegal to apply this method to a system of constraints that
    // is not dimension-compatible with the two polyhedra.
    oc2.limited_BHMZ05_extrapolation_assign(oc1, cs);
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
test09() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x - y >= 0);
  oc1.add_constraint(x >= 0);
  oc1.add_constraint(x <= 2);

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x - y >= 0);
  oc2.add_constraint(x >= 0);
  oc2.add_constraint(x <= 5);

  Constraint_System cs;
  cs.insert(x < 5);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::limited_BHMZ05_extrapolation_assign(oc, cs): it is
    // illegal to apply this method to a system of constraints that
    // has a strict-inequality.
    oc2.limited_BHMZ05_extrapolation_assign(oc1, cs);
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

