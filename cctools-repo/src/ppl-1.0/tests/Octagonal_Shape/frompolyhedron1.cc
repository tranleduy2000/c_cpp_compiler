/* Test Octagonal_Shape::Octagonal_Shape(const C_Polyhedron&).
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

  Constraint_System cs;
  cs.insert(A + 2*B <= 5);
  cs.insert(A + 2*B >= -10);
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(3*A - 5*B <= 18);
  C_Polyhedron ph(cs);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(11*A <= 61);
  known_result.add_constraint(2*B <= 5);
  known_result.add_constraint(5*B >= -18);
  known_result.add_constraint(11*A - 11*B <= 64);
  known_result.add_constraint(11*A + 11*B <= 58);

  bool ok = (oct1 == oct2
             && check_result(oct1, known_result,
                             "1.21e-6", "5.76e-7", "3.91e-7"));

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (oct1 == oct2
             && check_result(oct1, known_result));

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test03() {
  C_Polyhedron ph(0, UNIVERSE);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(0);

  bool ok = (oct1 == oct2
             && check_result(oct1, known_result));

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test04() {
//   Variable A(0);
//   Variable B(1);
//   Variable C(2);
//   Variable D(3);

  C_Polyhedron ph(4, UNIVERSE);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(4);

  bool ok = (oct1 == oct2
             && check_result(oct1, known_result));

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(2*A - 3*B >= 4);
  cs.insert(Linear_Expression(-3) >= 0);
  cs.insert(Linear_Expression(7) == 0);
  C_Polyhedron ph(cs);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (oct1 == oct2
             && check_result(oct1, known_result));

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(A - B >= 4);
  cs.insert(-C - D >= 0);
  cs.insert(A + B - C == 0);
  cs.insert(-A + B - C < 4);
  cs.insert(A - B - C + D > 1);
  NNC_Polyhedron ph(cs);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A >= -2);
  known_result.add_constraint(4*B <= -5);
  known_result.add_constraint(A - B >= 4);
  known_result.add_constraint(4*(A - C) >= 5);
  known_result.add_constraint(B + D <= 2);
  known_result.add_constraint(B - C <= 2);
  known_result.add_constraint(C + D <= 0);

  bool ok = (oct1 == oct2
             && check_result(oct1, known_result));

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(2*A - 3*B >= 4);
  cs.insert(-3*A >= 0);
  cs.insert(B + 2 == 0);
  C_Polyhedron ph(cs);

  TOctagonal_Shape oct1(ph, POLYNOMIAL_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B == -2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1 ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 4);
  cs.insert(B >= 10);
  cs.insert(B <= 0);
  C_Polyhedron ph(cs);

  TOctagonal_Shape oct1(ph, SIMPLEX_COMPLEXITY);
  TOctagonal_Shape oct2(ph, ANY_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (oct1 == oct2
             && Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
END_MAIN
