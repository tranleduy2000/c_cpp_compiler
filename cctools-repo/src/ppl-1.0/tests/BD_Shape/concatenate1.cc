/* Test BD_Shape::concatenate_assign().
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
  // Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  // Variable x4(3);
  Variable x5(4);
  Variable x6(5);
  // Variable x7(6);
  Variable x8(7);
  Variable x9(8);

  TBD_Shape bds1(6);
  bds1.add_constraint(x2 - x3 <= 0);
  bds1.add_constraint(x3 <= 2);
  bds1.add_constraint(x6 - x5 <= 2);
  bds1.add_constraint(x5 <= 3);

  TBD_Shape bds2(3);
  bds2.add_constraint(x2 - x3 <= 2);
  bds2.add_constraint(x3 <= 7);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.concatenate_assign(bds2);

  BD_Shape<mpq_class> known_result(9);
  known_result.add_constraint(x2 - x3 <= 0);
  known_result.add_constraint(x3 <= 2);
  known_result.add_constraint(x6 - x5 <= 2);
  known_result.add_constraint(x5 <= 3);
  known_result.add_constraint(x8 - x9 <= 2);
  known_result.add_constraint(x9 <= 7);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.concatenate_assign(bds2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x <= 3);
  bds1.add_constraint(x - y <= 4);

  TBD_Shape bds2(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds2.concatenate_assign(bds1);

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds2, "*** bds2.concatenate_assign(bds1) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x <= 0);
  cs.insert(y == 3);
  cs.insert(3*x - 3*y <= 5);

  TBD_Shape bds1(2);
  bds1.add_constraints(cs);

  TBD_Shape bds2(0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.concatenate_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 0);
  known_result.add_constraint(y == 3);
  known_result.add_constraint(x - y <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.concatenate_assign(bds2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds1(3);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(C >= 0);

  TBD_Shape bds2(2);
  bds2.add_constraint(A >= 0);
  bds2.add_constraint(A <= 1);
  bds2.add_constraint(B >= 0);
  bds2.add_constraint(B <= 2);
  bds2.add_constraint(A - B <= 0);
  bds2.add_constraint(B - A <= 1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.concatenate_assign(bds2);

  BD_Shape<mpq_class> known_result(5);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(D <= 1);
  known_result.add_constraint(E >= 0);
  known_result.add_constraint(E <= 2);
  known_result.add_constraint(D - E <= 0);
  known_result.add_constraint(E - D <= 1);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.concatenate_assign(bds2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x <= 3);
  bds1.add_constraint(x - y <= 4);

  TBD_Shape bds2(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.concatenate_assign(bds2);

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.concatenate_assign(bds2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
