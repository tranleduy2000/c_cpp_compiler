/* Test BD_Shape::limited_BHMZ05_extrapolation_assign().
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

  Constraint_System cs2;
  cs2.insert(x <= 0);
  cs2.insert(y >= 5);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 20);
  cs.insert(y >= 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(y >= 3);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 2);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);

  Constraint_System cs2;
  cs2.insert(x >= 0);
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x <= 5);
  known_result.add_constraint(y - x <= 0);
  known_result.add_constraint(x >= 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 3);
  cs1.insert(x <= 2);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);

  Constraint_System cs2;
  cs2.insert(x >= 2);
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);

  Constraint_System cs2;
  cs2.insert(x >= 3);
  cs2.insert(x <= 2);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x + y <= 0);
  cs.insert(x - y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);
  BD_Shape<mpq_class> known_result(bds1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test05() {
  TBD_Shape bds1;
  TBD_Shape bds2;
  Constraint_System cs;

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result;

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 2);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);
  cs1.insert(z <= 0);

  Constraint_System cs2;
  cs2.insert(x >= 0);
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);
  cs2.insert(z <= 0);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);
  cs.insert(x - y + z <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x <= 5);
  known_result.add_constraint(y - x <= 0);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(z <= 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs1;
  cs1.insert(x == 0);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);
  cs1.insert(z <= 0);

  Constraint_System cs2;
  cs2.insert(x == 0);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);
  cs2.insert(z <= -1);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x == 0);
  cs.insert(y >= 0);
  cs.insert(y <= 5);
  cs.insert(x - y + z <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y - x <= 0);
  known_result.add_constraint(x == 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 2);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);
  cs1.insert(z <= 0);

  Constraint_System cs2;
  cs2.insert(x >= 0);
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);
  cs2.insert(z <= 0);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);
  cs.insert(2*x - 3*y <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x <= 5);
  known_result.add_constraint(y - x <= 0);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(z <= 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 2);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);
  cs1.insert(z <= 0);

  Constraint_System cs2;
  cs2.insert(x >= 0);
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(x - y >= 0);
  cs2.insert(z <= 0);

  TBD_Shape bds1(cs1);
  TBD_Shape bds2(cs2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y >= 0);
  cs.insert(x <= 5);
  cs.insert(y <= 5);
  cs.insert(x - y >= 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.limited_BHMZ05_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x <= 5);
  known_result.add_constraint(y - x <= 0);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(z <= 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.limited_BHMZ05_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test10() {
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
    // BD_Shape::limited_BHMZ05_extrapolation_assign(bds, cs): it is
    // illegal to apply the method to a system of constraints that
    // is not dimension-compatible with the two polyhedra.
    bds2.limited_BHMZ05_extrapolation_assign(bds1, cs);
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
test11() {
  Variable y(1);

  TBD_Shape bds1(1);
  TBD_Shape bds2(2);

  Constraint_System cs;
  cs.insert(y <= 9);

  try {
    // This is an invalid use of the method
    // BD_Shape::limited_BHMZ05_extrapolation_assign(bds2, cs): it is
    // illegal to apply the method to two polyhedra that are not
    // dimension-compatible.
    bds2.limited_BHMZ05_extrapolation_assign(bds1, cs);
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
test12() {
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
    // BD_Shape::limited_BHMZ05_extrapolation_assign(bds, cs): it is
    // illegal to apply the method to a system of constraints that
    // has a strict-inequality.
    bds2.limited_BHMZ05_extrapolation_assign(bds1, cs);
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
END_MAIN
