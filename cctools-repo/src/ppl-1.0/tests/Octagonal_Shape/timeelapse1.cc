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
  TOctagonal_Shape oc1(2, EMPTY);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");


  TOctagonal_Shape oc3(2);

  print_constraints(oc3, "*** oc3 ***");

  TOctagonal_Shape oc4(2, EMPTY);

  print_constraints(oc4, "*** oc4 ***");

  oc3.time_elapse_assign(oc4);

  print_constraints(oc3, "*** oc3.time_elapse_assign(oc4) ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result) &&
    (Octagonal_Shape<mpq_class>(oc3) == known_result);

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x >= 0);
  oc1.add_constraint(y >= 0);
  oc1.add_constraint(x + y - 2 <= 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x >= 2);
  oc2.add_constraint(x <= 4);
  oc2.add_constraint(y == 3);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(y >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x >= 1);
  oc1.add_constraint(x <= 3);
  oc1.add_constraint(y >= 1);
  oc1.add_constraint(y <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(y == 5);

  oc1.time_elapse_assign(oc2);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(y >= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x == 3);
  oc1.add_constraint(y >= 2);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x >= 3);
  oc2.add_constraint(y >= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 3);
  oc1.add_constraint(y <= 5);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x <= 2);
  oc2.add_constraint(y <= 3);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(3);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 3);
  oc1.add_constraint(x >= 1);
  oc1.add_constraint(y <= 5);
  oc1.add_constraint(y >= 6);
  oc1.add_constraint(z >= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x <= 2);
  oc2.add_constraint(x >= 0);
  oc2.add_constraint(y <= 3);
  oc2.add_constraint(y >= 2);
  oc2.add_constraint(z >= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 3);
  oc1.add_constraint(x >= 1);
  oc1.add_constraint(y <= 5);
  oc1.add_constraint(y >= 1);
  oc1.add_constraint(z >= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x <= 2);
  oc2.add_constraint(x >= 0);
  oc2.add_constraint(y <= 3);
  oc2.add_constraint(y >= 2);
  oc2.add_constraint(z >= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x >= 1);
  known_result.add_constraint(y >= 1);
  known_result.add_constraint(z >= 1);
  known_result.add_constraint(x - y <= 2);
  known_result.add_constraint(x - z <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 3);
  oc1.add_constraint(x >= 1);
  oc1.add_constraint(y <= 7);
  oc1.add_constraint(y >= 6);
  oc1.add_constraint(z >= 1);

  print_constraints(oc1, "*** oc1 ***");

  // The octagon oc2 is empty.
  TOctagonal_Shape oc2(3);
  oc2.add_constraint(x <= 2);
  oc2.add_constraint(x >= 0);
  oc2.add_constraint(y <= 3);
  oc2.add_constraint(y >= 4);
  oc2.add_constraint(z >= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  TOctagonal_Shape oc1(6);
  oc1.add_constraint(A - B <= 1);
  oc1.add_constraint(C + D <= 3);
  oc1.add_constraint(A + E <= 5);
  oc1.add_constraint(E - F <= 7);
  oc1.add_constraint(F + E <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(6);
  oc2.add_constraint(A + B <= 0);
  oc2.add_constraint(C + B <= 9);
  oc2.add_constraint(A + E <= 5);
  oc2.add_constraint(F + E <= 2);
  oc2.add_constraint(E - F <= 8);

  print_constraints(oc2, "*** oc2 ***");

  oc1.time_elapse_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(6);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1_time_elapse_assign(oc2) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct1(1);
  oct1.add_constraint(x == 1);

  TOctagonal_Shape oct2(3);
  oct2.add_constraint(y + z <= 6);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::time_elapse_assign(oct1): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    oct2.time_elapse_assign(oct1);
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
  DO_TEST(test10);
END_MAIN
