/* Test BD_Shape::limited_CC76_extrapolation_assign().
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
  TBD_Shape bds1(0);
  TBD_Shape bds2(0);
  Constraint_System cs;

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A == -2);

  TBD_Shape bds2(2);
  bds2.add_constraint(A == -2);
  bds2.add_constraint(B == 3);

  Constraint_System cs;
  cs.insert(A <= 0);
  cs.insert(A - B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 4);

  TBD_Shape bds2(2);
  bds2.add_constraint(A == -2);

  Constraint_System cs;
  cs.insert(A <= 0);
  cs.insert(A - B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(B >= 1);

  TBD_Shape bds2(3);
  bds2.add_constraint(A <= -2);
  bds2.add_constraint(B >= 4);

  Constraint_System cs;
  cs.insert(A <= 5);
  cs.insert(A - B + C <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 5);
  known_result.add_constraint(B >= 1);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(B >= 1);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= -2);
  bds2.add_constraint(B >= 4);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A + B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(B >= 1);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= -2);
  bds2.add_constraint(B >= 4);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(A - B <= 6);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(A >= 5);
  bds1.add_constraint(B >= 1);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= -2);
  bds2.add_constraint(A >= 3);
  bds2.add_constraint(B >= 4);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(B >= 1);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= -2);
  bds2.add_constraint(A >= 3);
  bds2.add_constraint(B >= 4);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A - B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds1(4);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(B <= 6);
  bds1.add_constraint(C - D == 5);

  TBD_Shape bds2(4);
  bds2.add_constraint(A <= 4);
  bds2.add_constraint(C - D == 5);
  bds2.add_constraint(B <= 5);

  Constraint_System cs;
  cs.insert(A == 4);
  cs.insert(C - D == 5);
  cs.insert(A - B <= 6);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(C - D == 5);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x <= 1);
  cs1.insert(y >= 4);

  Constraint_System cs2;
  cs2.insert(x == 0);
  cs2.insert(y >= 5);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 20);
  cs.insert(y >= 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 3);
  known_result.add_constraint(y - x >= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(3, EMPTY);
  TBD_Shape bds2(3, EMPTY);

  Constraint_System cs;
  cs.insert(x <= 1);
  cs.insert(y >= 4);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x <= 1);
  cs1.insert(y >= 4);
  cs1.insert(x - y >= 2);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(2, EMPTY);

  Constraint_System cs2;
  cs2.insert(x <= 0);
  cs2.insert(y >= 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs2, "*** cs2 ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.limited_CC76_extrapolation_assign(bds2, cs2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_CC76_extrapolation_assign(bds2, cs2) ***");

  return ok;
}

bool
test13() {
  Variable y(1);

  TBD_Shape bds1(1);
  TBD_Shape bds2(2);

  Constraint_System cs;
  cs.insert(y <= 9);

  try {
    // This is an invalid use of the method
    // BD_Shape::limited_CC76_extrapolation_assign(bds2, cs): it is
    // illegal to apply the method to two polyhedra that are not
    // dimension-compatible.
    bds2.limited_CC76_extrapolation_assign(bds1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test14() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds1(2);
  bds1.add_constraint(x - y >= 0);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(x <= 2);

  TBD_Shape bds2(2);
  bds2.add_constraint(x - y >= 0);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(x <= 5);

  Constraint_System cs;
  cs.insert(z <= 5);

  try {
    // This is an invalid use of the method
    // BD_Shape::limited_CC76_extrapolation_assign(bds, cs): it is
    // illegal to apply the method to a system of constraints that
    // is not dimension-compatible with the two polyhedra.
    bds2.limited_CC76_extrapolation_assign(bds1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test15() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x - y >= 0);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(x <= 2);

  TBD_Shape bds2(2);
  bds2.add_constraint(x - y >= 0);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(x <= 5);

  Constraint_System cs;
  cs.insert(x < 5);

  try {
    // This is an invalid use of the method
    // BD_Shape::limited_CC76_extrapolation_assign(bds, cs): it is
    // illegal to apply the method to a system of constraints that
    // in which there is a strict inequality.
    bds2.limited_CC76_extrapolation_assign(bds1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
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
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
END_MAIN
