/* Test Octagonal_Shape::concatenate_assign().
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
  Variable x5(4);
  Variable x6(5);
  Variable x7(6);
  Variable x8(7);
  Variable x9(8);

  TOctagonal_Shape oc1(6);
  oc1.add_constraint(x2 - x3 <= 0);
  oc1.add_constraint(x3 <= 2);
  oc1.add_constraint(x6 - x5 <= 2);
  oc1.add_constraint(x5 <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x2 - x3 <= 2);
  oc2.add_constraint(x3 <= 7);

  print_constraints(oc2, "*** oc2 ***");

  oc1.concatenate_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(9);
  known_result.add_constraint(x2 - x3 <= 0);
  known_result.add_constraint(x3 <= 2);
  known_result.add_constraint(x6 - x5 <= 2);
  known_result.add_constraint(x5 <= 3);
  known_result.add_constraint(x8 - x9 <= 2);
  known_result.add_constraint(x9 <= 7);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.concatenate_assign(oc2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x <= 3);
  oc1.add_constraint(x - y <= 4);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(0, EMPTY);

  print_constraints(oc2, "*** oc2 ***");

  oc2.concatenate_assign(oc1);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc2) == known_result);

  print_constraints(oc2, "*** oc2.concatenate_assign(oc1) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(0, EMPTY);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x <= 3);
  oc2.add_constraint(x - y <= 4);

  print_constraints(oc2, "*** oc2 ***");

  oc2.concatenate_assign(oc1);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc2) == known_result);

  print_constraints(oc2, "*** oc2.concatenate_assign(oc1) ***");

  return ok;
}

bool
test04() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  Variable x6(5);
  Variable x7(6);
  Variable x8(7);
  Variable x9(8);

  TOctagonal_Shape oc1(6);
  oc1.add_constraint(x2 + x3 <= 1);
  oc1.add_constraint(x3 <= 2);
  oc1.add_constraint(x6 - x5 <= 2);
  oc1.add_constraint(x5 <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x2 + x3 <= 77);
  oc2.add_constraint(x3 <= 7);

  print_constraints(oc2, "*** oc2 ***");

  oc1.concatenate_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(9);
  known_result.add_constraint(x2 + x3 <= 1);
  known_result.add_constraint(x3 <= 2);
  known_result.add_constraint(x6 - x5 <= 2);
  known_result.add_constraint(x5 <= 3);
  known_result.add_constraint(x8 + x9 <= 77);
  known_result.add_constraint(x9 <= 7);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.concatenate_assign(oc2) ***");

  return ok;
}

bool
test05() {
  TOctagonal_Shape oc1(4);
  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(0);
  print_constraints(oc2, "*** oc2 ***");

  oc1.concatenate_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(4);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.concatenate_assign(oc2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc1(4);
  oc1.add_constraint(A - B <= 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(0);

  print_constraints(oc2, "*** oc2 ***");

  oc2.concatenate_assign(oc1);

  Octagonal_Shape<mpq_class> known_result(oc1);

  bool ok = (Octagonal_Shape<mpq_class>(oc2) == known_result);

  print_constraints(oc2, "*** oc2.concatenate_assign(oc1) ***");

  return ok;
}

bool
test07() {
  TOctagonal_Shape oc1(0, EMPTY);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(0, EMPTY);

  print_constraints(oc2, "*** oc2 ***");

  oc1.concatenate_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(0, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  return ok;
}

bool
test08() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  Variable x6(5);
  Variable x7(6);
  Variable x8(7);
  Variable x9(8);

  TOctagonal_Shape oc1(6);
  oc1.add_constraint(x2 - x3 <= 1);
  oc1.add_constraint(x3 <= 12);
  oc1.add_constraint(x6 - x5 <= 2);
  oc1.add_constraint(x5 <= 23);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x2 - x3 <= 2);
  oc2.add_constraint(x3 <= 8);

  print_constraints(oc2, "*** oc2 ***");

  oc1.concatenate_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(9);
  known_result.add_constraint(x2 - x3 <= 1);
  known_result.add_constraint(x3 <= 12);
  known_result.add_constraint(x6 - x5 <= 2);
  known_result.add_constraint(x5 <= 23);
  known_result.add_constraint(x8 - x9 <= 2);
  known_result.add_constraint(x9 <= 8);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.concatenate_assign(oc2) ***");

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
