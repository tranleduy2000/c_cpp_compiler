/* Test Octagonal_Shape::limited_CC76_extrapolation_assign().
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
  cs1.insert(x <= 1);
  cs1.insert(y >= 4);
  TOctagonal_Shape oct1(cs1);

  Constraint_System cs2;
  cs2.insert(x == 0);
  cs2.insert(y >= 5);
  TOctagonal_Shape oct2(cs2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(x >= 20);
  cs.insert(y >= 3);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 3);
  known_result.add_constraint(y - x >= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test02() {
  TOctagonal_Shape oct1(0);
  TOctagonal_Shape oct2(0);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(0);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A == -2);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A == -2);
  oct2.add_constraint(B == 3);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A <= 0);
  cs.insert(A - B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A == -2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 4);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A == -2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A <= 0);
  cs.insert(A - B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);


  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(B >= 1);

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(A <= -2);
  oct2.add_constraint(B >= 4);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A <= 5);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 5);
  known_result.add_constraint(B >= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(B >= 1);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A <= -2);
  oct2.add_constraint(B >= 4);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A + B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(B >= 1);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A <= -2);
  oct2.add_constraint(B >= 4);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(A - B <= 6);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(A >= 5);
  oct1.add_constraint(B >= 1);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A <= -2);
  oct2.add_constraint(A >= 3);
  oct2.add_constraint(B >= 4);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(B >= 1);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A <= -2);
  oct2.add_constraint(A >= 3);
  oct2.add_constraint(B >= 4);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B >= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oct1(4);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(B <= 6);
  oct1.add_constraint(C - D == 5);

  TOctagonal_Shape oct2(4);
  oct2.add_constraint(A <= 4);
  oct2.add_constraint(C - D == 5);
  oct2.add_constraint(B <= 5);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A == 4);
  cs.insert(C - D == 5);
  cs.insert(A - B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(C - D == 5);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 4);
  oct1.add_constraint(B >= 1);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A <= -2);
  oct2.add_constraint(A >= 3);
  oct2.add_constraint(B >= 4);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - 2*B <= 6);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B >= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 2);
  oct1.add_constraint(A >= 3);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A <= 1);
  oct2.add_constraint(B == -1);
  oct2.add_constraint(A >= 4);
  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A <= 2);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(C <= 4);
  oct1.add_constraint(B >= 2);

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(C <= 3);
  oct2.add_constraint(B >= 3);
  oct2.add_constraint(A <= 3);
  oct2.add_constraint(A >= 5);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(C <= 2);

  print_constraints(cs, "*** cs ***");

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 4);
  known_result.add_constraint(B >= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

   return ok;
}

bool
test14() {
  Variable y(1);

  TOctagonal_Shape oc1(1);
  TOctagonal_Shape oc2(2);

  Constraint_System cs;
  cs.insert(y <= 9);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::limited_CC76_extrapolation_assign(oc2, cs): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    oc2.limited_CC76_extrapolation_assign(oc1, cs);
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
test15() {
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
    // Octagonal_Shape::limited_CC76_extrapolation_assign(oc, cs): it is
    // illegal to apply this method to a system of constraints that
    // is not dimension-compatible with the two polyhedra.
    oc2.limited_CC76_extrapolation_assign(oc1, cs);
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
test16() {
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
    // Octagonal_Shape::limited_CC76_extrapolation_assign(bd, cs): it is
    // illegal to apply this method to a system of constraints that
    // has a strict-inequality.
    oc2.limited_CC76_extrapolation_assign(oc1, cs);
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
test17() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(3, EMPTY);
  TOctagonal_Shape oct2(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A + B <= 0);

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A - B <= 1);

  TOctagonal_Shape oct2(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Constraint_System cs;
  cs.insert(A - B <= 2);

  oct1.limited_CC76_extrapolation_assign(oct2, cs);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A - B <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1,
                    "*** oct1.limited_CC76_extrapolation_assign(oct2) ***");

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
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
END_MAIN

