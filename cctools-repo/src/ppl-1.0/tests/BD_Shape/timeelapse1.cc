/* Test time_elapse_assign() for particular polyhedra.
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
  TBD_Shape bds1(2, EMPTY);
  TBD_Shape bds2(2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.time_elapse_assign(bds2);

  TBD_Shape bds3(2);
  TBD_Shape bds4(2, EMPTY);

  print_constraints(bds3, "*** bds3 ***");
  print_constraints(bds4, "*** bds4 ***");

  bds3.time_elapse_assign(bds4);

  bool ok = (bds1.is_empty() && bds3.is_empty());

  print_constraints(bds1, "*** bds1_time_elapse_assign(bds2) ***");
  print_constraints(bds3, "*** bds3_time_elapse_assign(bds4) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(y >= 0);

  TBD_Shape bds2(2);
  bds2.add_constraint(x >= 2);
  bds2.add_constraint(x <= 4);
  bds2.add_constraint(y == 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.time_elapse_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(y >= 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1_time_elapse_assign(bds2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x >= 1);
  bds1.add_constraint(x <= 3);
  bds1.add_constraint(y >= 1);
  bds1.add_constraint(y <= 3);

  TBD_Shape bds2(2);
  bds2.add_constraint(y == 5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.time_elapse_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(y >= 1);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1_time_elapse_assign(bds2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(3);
  bds1.add_constraint(x <= 3);
  bds1.add_constraint(y <= 5);

  TBD_Shape bds2(3);
  bds2.add_constraint(x <= 2);
  bds2.add_constraint(y <= 3);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.time_elapse_assign(bds2);

  BD_Shape<mpq_class> known_result(3);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1_time_elapse_assign(bds2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(x <= 2);
  bds1.add_constraint(x >= 1);
  bds1.add_constraint(y <= 5);
  bds1.add_constraint(y >= 10);
  bds1.add_constraint(z >= 1);

  TBD_Shape bds2(3);
  bds2.add_constraint(x <= 9);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(y <= 3);
  bds2.add_constraint(y >= -1);
  bds2.add_constraint(z >= 2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.time_elapse_assign(bds2);

  BD_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.time_elapse_assign(bds2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);

  TBD_Shape bds1(1);
  bds1.add_constraint(x == 1);

  TBD_Shape bds2(2);

  try {
    // This is an invalid use of the method
    // BD_Shape::time_elapse_assign(bds2): it is illegal to apply the
    // method to two polyhedra that are not dimension-compatible.
    bds1.time_elapse_assign(bds2);
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
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x <= 3);
  bds1.add_constraint(x >= 1);
  bds1.add_constraint(y <= 5);
  bds1.add_constraint(y >= 1);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(2);
  bds2.add_constraint(x <= 2);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(y <= 3);
  bds2.add_constraint(y >= 2);

  print_constraints(bds2, "*** bds2 ***");

  bds1.time_elapse_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x >= 1);
  known_result.add_constraint(y >= 1);
  known_result.add_constraint(x - y <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.time_elapse_assign(bds2) ***");

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
END_MAIN
