/* Test Octagonal_Shape::difference_assign(): if `oct1' is
   contained in `oct2', the result of `oct1.difference_assign(oct2)'
   is an empty octagon.
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
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(A <= -2);
  oct1.add_constraint(B == 0);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A >= 0);
  oct2.add_constraint(A <= 2);
  oct2.add_constraint(B >= 0);
  oct2.add_constraint(B <= 2);

  print_constraints(oct2, "*** oct2 ***");

  oct1.difference_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.difference_assign(oc2) ***");

  return ok;
}

bool
test02() {
  TOctagonal_Shape oct1(0);
  TOctagonal_Shape oct2(0);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  oct1.difference_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(0, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.intersection_assign(oc2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);

  TOctagonal_Shape oct1(1);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(A <= 7);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(1);
  oct2.add_constraint(A == 5);

  print_constraints(oct2, "*** oct2 ***");

  oct1.difference_assign(oct2);

  Octagonal_Shape<mpq_class> known_result(1);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 7);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.intersection_assign(oc2) ***");

  return ok;
}

bool
test04() {
  TOctagonal_Shape oc1(3);
  TOctagonal_Shape oc2(5);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::difference_assign(oc2): it is impossible to apply
    // this method to two polyhedra of different dimensions.
    oc1.difference_assign(oc2);
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
test05() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A <= 0);
  oct1.add_constraint(A >= -2);
  oct1.add_constraint(B == 0);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(2, EMPTY);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  oct1.difference_assign(oct2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.difference_assign(oc2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oc1(4);
  oc1.add_constraint(D == 0);
  oc1.add_constraint(C == 0);
  oc1.add_constraint(B == 0);
  oc1.add_constraint(A == 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(4);
  oc1.add_constraint(A + D >= 0);
  oc1.add_constraint(C >= 0);
  oc1.add_constraint(B >= 0);
  oc1.add_constraint(A == 0);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(4, EMPTY);

  oc1.difference_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.difference_assign(oc2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A >= 1);
  oct1.add_constraint(B >= 0);
  oct1.add_constraint(A + B <= 3);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A + B <= 10);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);
  print_constraints(oct2, "*** oct2 ***");

  oct1.difference_assign(oct2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.difference_assign(oct2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A + B <= 3);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A + B >= 1);
  oct2.add_constraint(A + B <= 10);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A + B <= 1);

  oct1.difference_assign(oct2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.difference_assign(oct2) ***");

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
