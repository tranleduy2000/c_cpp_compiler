/* Test BD_Shape::upper_bound_assign().
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
  Variable x3(2);
  Variable x4(3);

  TBD_Shape bds1(5);
  bds1.add_constraint(x1 <= 5);
  bds1.add_constraint(x2 <= -1);
  bds1.add_constraint(x1 -x2 <= 10);

  TBD_Shape bds2(5);
  bds2.add_constraint(x1  <= 2);
  bds2.add_constraint(x4 <= 7);
  bds2.add_constraint(x1 - x2 <= 20);
  bds2.add_constraint(x4 - x3 <= 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.upper_bound_assign(bds2);

  BD_Shape<mpq_class> known_result(5);
  known_result.add_constraint(x1 <= 5);
  known_result.add_constraint(x1 - x2 <= 20);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign(bds2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(3);
  bds1.add_constraint(x <= 4);
  bds1.add_constraint(-x <= -1);
  bds1.add_constraint(y <= 3);
  bds1.add_constraint(-y <= -1);
  bds1.add_constraint(x - y <= 1);

  TBD_Shape bds2(3);
  bds2.add_constraint(y - x <= -1);
  bds2.add_constraint(x <= 3);
  bds2.add_constraint(-y <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.upper_bound_assign(bds2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 4);
  known_result.add_constraint(y >= -5);
  known_result.add_constraint(x >= -4);
  known_result.add_constraint(y <= 3);
  known_result.add_constraint(x - y <= 8);
  known_result.add_constraint(y - x <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign(bds2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x <= 4);
  bds1.add_constraint(-x <= -1);
  bds1.add_constraint(y <= 3);
  bds1.add_constraint(-y <= -1);
  bds1.add_constraint(x - y <= 1);

  TBD_Shape bds2(2);
  bds2.add_constraint(y - x <= -1);
  bds2.add_constraint(x <= 3);
  bds2.add_constraint(x >= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.upper_bound_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign(bds2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 4);
  bds1.add_constraint(A >= 1);
  bds1.add_constraint(B <= 3);
  bds1.add_constraint(-B <= -1);
  bds1.add_constraint(A - B <= 1);

  TBD_Shape bds2(2);
  bds2.add_constraint(B - A <= -1);
  bds2.add_constraint(A <= 3);
  bds2.add_constraint(-B <= 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.upper_bound_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 4);
  known_result.add_constraint(B >= -5);
  known_result.add_constraint(A >= -4);
  known_result.add_constraint(B <= 3);
  known_result.add_constraint(A - B <= 8);
  known_result.add_constraint(B - A <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.upper_bound_assign(bds2) ***");

  return ok;
}

bool
test05() {
  TBD_Shape bds1(12);
  TBD_Shape bds2(5);

  try {
    // This is an incorrect use of method
    // BD_Shape::upper_bound_assign(bds2): it is impossible to apply
    // this method to two polyhedra of different dimensions.
    bds1.upper_bound_assign(bds2);
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
test06() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x >= y);

  TBD_Shape bds2(3);

  try {
    // This is an invalid use of method
    // BD_Shape::upper_bound_assign(bds2): it is illegal
    // to apply the method to two polyhedra of different dimensions.
    bds1.upper_bound_assign(bds2);
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
test07() {
  Variable A(0);

  TBD_Shape bds1(1);
  bds1.add_constraint(A <= 0);
  bds1.add_constraint(A >= 1);

  TBD_Shape bds2(1);
  bds2.add_constraint(A <= 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.upper_bound_assign(bds2);

  BD_Shape<mpq_class> known_result(1);
  known_result.add_constraint(A <= 3);

  bool ok = check_result(bds1, known_result) ;

  print_constraints(bds1, "*** bds1.upper_bound_assign(bds2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(A - B <= 0);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= 0);
  bds2.add_constraint(A - B <= 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  (void) bds1.minimized_constraints();

  bds1.upper_bound_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(A - B <= 0);

  bool ok = check_result(bds1, known_result) ;

  print_constraints(bds1, "*** bds1.upper_bound_assign(bds2) ***");

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
END_MAIN
