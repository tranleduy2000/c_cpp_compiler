/* Test Octagonal_Shape::BHMZ05_widening_assign().
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

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x <= 1);
  oc1.add_constraint(x - y <= 2);
  oc1.add_constraint(y - x <= 7);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x - y <= 2);
  oc2.add_constraint(-x <= 3);
  oc2.add_constraint(x <= 0);
  oc2.add_constraint(y - x <= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x - y <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test02() {
  TOctagonal_Shape oc1;
  TOctagonal_Shape oc2(0, EMPTY);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result;

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(A >= 1);
  oc1.add_constraint(B >= 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(A >= 0);
  oc2.add_constraint(B >= 0);
  oc2.add_constraint(A - B >= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A >= 0);
  oc1.add_constraint(B >= 0);
  oc1.add_constraint(C >= 1);
  oc1.add_constraint(D >= 0);
  oc1.add_constraint(E >= 0);
  oc1.add_constraint(C - D <= 76);
  oc1.add_constraint(C - E <= 76);
  oc1.add_constraint(E - D == 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A >= 0);
  oc2.add_constraint(B >= 0);
  oc2.add_constraint(C >= 1);
  oc2.add_constraint(D >= 0);
  oc2.add_constraint(E >= 0);
  oc2.add_constraint(C - D <= 75);
  oc2.add_constraint(C - E <= 75);
  oc2.add_constraint(E - D == 0);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(5);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 1);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(E - D == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oc1(4);
  oc1.add_constraint(A >= 0);
  oc1.add_constraint(B >= 0);
  oc1.add_constraint(B <= 39);
  oc1.add_constraint(C >= 0);
  oc1.add_constraint(C <= 40);
  oc1.add_constraint(D >= 0);
  oc1.add_constraint(D <= 40);
  oc1.add_constraint(B - A <= 39);
  oc1.add_constraint(C - A <= 40);
  oc1.add_constraint(D - A <= 40);
  oc1.add_constraint(B - C <= 0);
  oc1.add_constraint(C - B <= 1);
  oc1.add_constraint(B - D <= 0);
  oc1.add_constraint(D - B <= 2);
  oc1.add_constraint(C - D <= 0);
  oc1.add_constraint(D - C <= 1);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(4);
  oc2.add_constraint(A >= 0);
  oc2.add_constraint(B >= 0);
  oc2.add_constraint(B <= 38);
  oc2.add_constraint(C >= 0);
  oc2.add_constraint(C <= 39);
  oc2.add_constraint(D >= 0);
  oc2.add_constraint(D <= 40);
  oc2.add_constraint(B - A <= 38);
  oc2.add_constraint(C - A <= 39);
  oc2.add_constraint(D - A <= 40);
  oc2.add_constraint(B - C <= 0);
  oc2.add_constraint(C - B <= 1);
  oc2.add_constraint(B - D <= 0);
  oc2.add_constraint(D - B <= 2);
  oc2.add_constraint(C - D <= 0);
  oc2.add_constraint(D - C <= 1);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(B - C <= 0);
  known_result.add_constraint(C - B <= 1);
  known_result.add_constraint(C - D <= 0);
  known_result.add_constraint(D - C <= 1);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oc1(4);
  oc1.add_constraint(B <= 1);
  oc1.add_constraint(C <= 7);
  oc1.add_constraint(D >= 0);
  oc1.add_constraint(A + B <= 10);
  oc1.add_constraint(C - A <= 10);
  oc1.add_constraint(D - A <= 10);
  oc1.add_constraint(B - C <= 0);
  oc1.add_constraint(C - B <= 1);
  oc1.add_constraint(B - D <= 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(4);
  oc2.add_constraint(B <= 1);
  oc2.add_constraint(C <= 7);
  oc2.add_constraint(D >= 0);
  oc2.add_constraint(A + B <= 9);
  oc2.add_constraint(C - A <= 9);
  oc2.add_constraint(D - A <= 10);
  oc2.add_constraint(B - C <= 0);
  oc2.add_constraint(C - B <= 1);
  oc2.add_constraint(B - D <= 0);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(C <= 7);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(D - A <= 10);
  known_result.add_constraint(B - C <= 0);
  known_result.add_constraint(C - B <= 1);
  known_result.add_constraint(B - D <= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A + B >= 9);
  oc1.add_constraint(B + C >= 11);
  oc1.add_constraint(C + D >= 12);
  oc1.add_constraint(D + E >= 5);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A + B >= 10);
  oc2.add_constraint(B + C >= 11);
  oc2.add_constraint(C + D >= 12);
  oc2.add_constraint(D + E >= 13);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(5);
  known_result.add_constraint(B + C >= 11);
  known_result.add_constraint(C + D >= 12);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A - B <= 2);
  oc1.add_constraint(B - C <= 0);
  oc1.add_constraint(C - A <= -2);
  oc1.add_constraint(A - D <= 2);
  oc1.add_constraint(E - D <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A - B <= 2);
  oc2.add_constraint(B - C <= 0);
  oc2.add_constraint(C - A <= -2);
  oc2.add_constraint(B - A <= 3);
  oc2.add_constraint(A - D <= 1);
  oc2.add_constraint(E - D <= 3);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(5);
  known_result.add_constraint(A - B <= 2);
  known_result.add_constraint(B - C <= 0);
  known_result.add_constraint(C - A <= -2);
  known_result.add_constraint(E - D <= 3);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A - B <= 2);
  oc1.add_constraint(B - C <= 0);
  oc1.add_constraint(C - A <= -2);
  oc1.add_constraint(A - D <= 2);
  oc1.add_constraint(E - D <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A - B <= 2);
  oc2.add_constraint(B - A <= -7);
  oc2.add_constraint(C - A == -2);
  oc2.add_constraint(A - D <= 1);
  oc2.add_constraint(E - D <= 3);

  print_constraints(oc2, "*** oc2 ***");

  Octagonal_Shape<mpq_class> known_result(oc1);

  oc1.BHMZ05_widening_assign(oc2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A <= 2);
  oc1.add_constraint(B <= 0);
  oc1.add_constraint(C <= -2);
  oc1.add_constraint(D <= 2);
  oc1.add_constraint(E <= 3);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A <= 2);
  oc2.add_constraint(B <= -7);
  oc2.add_constraint(C <= -2);
  oc2.add_constraint(D <= 2);
  oc2.add_constraint(E <= 3);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(5);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(C <= -2);
  known_result.add_constraint(D <= 2);
  known_result.add_constraint(E <= 3);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A <= 2);
  oc1.add_constraint(A + C <= -1);
  oc1.add_constraint(A + D <= 1);
  oc1.add_constraint(B <= 0);
  oc1.add_constraint(C <= -3);
  oc1.add_constraint(D <= 2);
  oc1.add_constraint(E <= 4);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A <= 2);
  oc2.add_constraint(A + D <= 1);
  oc2.add_constraint(B <= -1);
  oc2.add_constraint(C <= -3);
  oc2.add_constraint(D <= 2);
  oc2.add_constraint(E <= 3);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(5);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(A + D <= 1);
  known_result.add_constraint(C <= -3);
  known_result.add_constraint(D <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A <= 2);
  oc1.add_constraint(C - A <= -1);
  oc1.add_constraint(B <= 0);
  oc1.add_constraint(C >= 2);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(3, EMPTY);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2);

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y == 0);
  TOctagonal_Shape oct1(cs1);

  print_constraints(oct1, "*** oct1 ***");

  Constraint_System cs2;
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(y - x <= 0);
  TOctagonal_Shape oct2(cs2);


  print_constraints(oct2, "*** oct2 ***");

  oct2.BHMZ05_widening_assign(oct1);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y - x <= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(oct2, "*** oct2.BHMZ05_widening_assign(oct1) ***");

  return ok;
}

bool
test14() {
  TOctagonal_Shape oc1(5);
  TOctagonal_Shape oc2(10);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::BHMZ05_widening_assign(oc2): it is illegal to apply
    // this method to two polyhedra that are not dimensional
    // compatible.
    oc2.BHMZ05_widening_assign(oc1);
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
  Variable A(0);
  Variable B(1);

  unsigned tokens = 6;

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(A >= 1);
  oc1.add_constraint(B >= 0);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(A >= 0);
  oc2.add_constraint(B >= 0);
  oc2.add_constraint(A - B >= 2);

  print_constraints(oc2, "*** oc2 ***");

  oc1.BHMZ05_widening_assign(oc2, &tokens);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A >= 1);

#undef TOKENS
#define TOKENS 5

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result)
    && (tokens == TOKENS);

  nout << "tokens: before " << TOKENS << ", after " << tokens <<
    endl;

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test16() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x <= 1);
  oc1.add_constraint(x - y <= 2);
  oc1.add_constraint(y - x <= 7);

  print_constraints(oc1, "*** oc1 ***");

  TOctagonal_Shape oc2(2);
  oc2.add_constraint(x - y <= 2);
  oc2.add_constraint(-x <= 3);
  oc2.add_constraint(x <= 0);
  oc2.add_constraint(y - x <= 2);

  print_constraints(oc2, "*** oc2 ***");

#undef TOKENS
#define TOKENS 0

  unsigned tokens = TOKENS;

  oc1.BHMZ05_widening_assign(oc2, &tokens);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x - y <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result)
    && (tokens == TOKENS);

  nout << "tokens: before " << TOKENS << ", after " << tokens <<
    endl;

  print_constraints(oc1, "*** oc1.BHMZ05_widening_assign(oc2) ***");

  return ok;
}

bool
test17() {
  Variable x(0);
  Variable y(1);

  unsigned tokens = 6;

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y == 0);
  TOctagonal_Shape oct1(cs1);

  print_constraints(oct1, "*** oct1 ***");

  Constraint_System cs2;
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(y - x <= 0);
  TOctagonal_Shape oct2(cs2);

  print_constraints(oct2, "*** oct2 ***");

  oct2.BHMZ05_widening_assign(oct1, &tokens);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y - x <= 0);

#undef TOKENS
#define TOKENS 6

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result)
    && (tokens == TOKENS);

  nout << "tokens: before " << TOKENS << ", after " << tokens <<
    endl;

  print_constraints(oct2, "*** oct2.BHMZ05_widening_assign(oct1) ***");

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
END_MAIN


