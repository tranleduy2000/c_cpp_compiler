/* Test Octagonal_Shape::constraints(): we compute the system of
   constraints of an octagon  that is defined by a system of
   constraints that contains only a trivially false constraint.
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

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(0*A == 1);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  print_constraints(oct2, "*** oct2 ***");

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test02() {
  TOctagonal_Shape oct1(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(x == 1);
  oct1.add_constraint(y <= 3);
  oct1.add_constraint(y - x >= 5);
  oct1.add_constraint(z + x == 2);
  oct1.add_constraint(z - y <= 7);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test04() {
  TOctagonal_Shape oct1(0, EMPTY);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs.space_dimension(), EMPTY);


  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test05() {
  TOctagonal_Shape oct1(0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);

  oct1.add_constraint(A >= 0);
  oct1.add_constraint(B >= 0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);

  oct1.add_constraint(A == 0);
  oct1.add_constraint(B >= 1);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);

  oct1.add_constraint(A + B == 0);
  oct1.add_constraint(B <= 1);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);

  oct1.add_constraint(A - B == 0);
  oct1.add_constraint(B <= 1);
  oct1.add_constraint(C + A <= 2);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct1(5);
  oct1.add_constraint(A == 1);
  oct1.add_constraint(B + C == 1);
  oct1.add_constraint(E - D == 1);
  oct1.add_constraint(A + C <= 1);
  oct1.add_constraint(B - D <= 1);
  oct1.add_constraint(E + C >= -1);
  oct1.add_constraint(A - D >= -1);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);

  oct1.add_constraint(A - B == 0);
  oct1.add_constraint(B <= 1);
  oct1.add_constraint(-C - A <= 2);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.constraints();
  TOctagonal_Shape oct2(cs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test12() {

  TOctagonal_Shape oct1(0);
  oct1.add_constraint(Linear_Expression(1) == 0);

  TOctagonal_Shape oct2(0, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpq_class> known_result(oct2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(3);

  oct.refine_with_constraint(A > 0);
  oct.refine_with_constraint(B < 0);
  oct.refine_with_constraint(A + B > 1);
  oct.refine_with_constraint(A + 2*B > 1);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(A + B >= 1);

  bool ok = check_result(oct, known_result);

  print_constraints(oct, "*** oct ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A > 0);
  cs.insert(B < 0);
  cs.insert(A + B > 1);
  cs.insert(A + 2*B > 1);

  TOctagonal_Shape oct(3);
  oct.refine_with_constraints(cs);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(A + B >= 1);

  bool ok = check_result(oct, known_result);

  print_constraints(oct, "*** oct ***");

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
END_MAIN

