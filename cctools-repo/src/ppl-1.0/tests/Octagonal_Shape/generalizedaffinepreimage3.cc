/* Test Octagonal_Shape::generalized_affine_preimage().
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
  Variable C(2);

  Linear_Expression e1(A - C + B - 2);
  Linear_Expression e2(A + B + 1);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A >= 1);
  oct.add_constraint(A <= 2);
  oct.add_constraint(B <= 3);
  oct.add_constraint(C >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A - C + B - 2, "
                    "LESS_OR_EQUAL, A + B + 1) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a variable in the `rhs' expression that
    // does not appear in the octagon.
    oct.generalized_affine_preimage(A + B, GREATER_OR_EQUAL, B + C);
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
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a variable in the `lhs' expression that
    // does not appear in the octagon.
    oct.generalized_affine_preimage(B + C, LESS_OR_EQUAL, A + 1);
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
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A >= 1);
  oct.add_constraint(C + B >= 1);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a strict relation symbol.
    oct.generalized_affine_preimage(B + C, LESS_THAN, A + 1);
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

  Linear_Expression e1(2*A + 3*B - 1);
  Linear_Expression e2(B - 1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 1);
  oct.add_constraint(B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= -4);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(2*A + 3*B - 1, "
                    "LESS_OR_EQUAL, B - 1) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(A + B);
  Linear_Expression e2(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);
  oct.add_constraint(A + B <= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A + B, "
                    "GREATER_OR_EQUAL, 2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Linear_Expression e1(A + B - 2*C + 2);
  Linear_Expression e2(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);
  oct.add_constraint(C <= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A + B - 2*C + 2, "
                    "LESS_OR_EQUAL, 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Linear_Expression e1(A - B + 2);
  Linear_Expression e2(2);

  TOctagonal_Shape oct(4);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);
  oct.add_constraint(C <= -1);
  oct.add_constraint(D >= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(C <= -1);
  known_result.add_constraint(D >= 2);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A - B + 2, "
                    "LESS_OR_EQUAL, 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Linear_Expression e1(C - D - 5);
  Linear_Expression e2(3);

  TOctagonal_Shape oct(4);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);
  oct.add_constraint(C <= -1);
  oct.add_constraint(D >= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(4, EMPTY);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(C - D - 5, "
                    "GREATER_OR_EQUAL, 3) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Linear_Expression e1(C - D - 5);
  Linear_Expression e2(-11);

  TOctagonal_Shape oct(4);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);
  oct.add_constraint(C <= -1);
  oct.add_constraint(D >= 2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B >= -2);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(C - D - 5, "
                    "GREATER_OR_EQUAL, -11) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(-5);
  Linear_Expression e2(-1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-5, "
                    "GREATER_OR_EQUAL, -1) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(-5);
  Linear_Expression e2(-1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(B >= -2);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-5, "
                    "LESS_OR_EQUAL, -1) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 1);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= -4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.generalized_affine_preimage(A, LESS_OR_EQUAL, B + 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "LESS_OR_EQUAL, B + 2) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(B <= 14);

  oct.generalized_affine_preimage(A, LESS_OR_EQUAL, 2*B + 3, 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "LESS_OR_EQUAL, 2*B + 3, 2) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= -5);
  known_result.add_constraint(A <= 10);

  oct.generalized_affine_preimage(B, LESS_OR_EQUAL, A + 3);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, A + 3) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(4*B <= 5);

  oct.generalized_affine_preimage(A, LESS_OR_EQUAL, 2*B + 3, -2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "LESS_OR_EQUAL, 2*B + 3, -2) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= -6);
  known_result.add_constraint(A <= 5);

  oct.generalized_affine_preimage(B, LESS_OR_EQUAL, -A + 3);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -A + 3) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= -6);
  known_result.add_constraint(A <= 10);

  oct.generalized_affine_preimage(B, GREATER_OR_EQUAL, A + 3);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, A + 3) ***");

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(B <= 14);

  oct.generalized_affine_preimage(A, GREATER_OR_EQUAL, 2*B + 3, -2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "GREATER_OR_EQUAL, 2*B + 3, -2) ***");

  return ok;
}

bool
test20() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 10);
  oct.add_constraint(A >= -11);
  oct.add_constraint(B >= -2);
  oct.add_constraint(B - A <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A >= -1);
  known_result.add_constraint(A <= 10);

  oct.generalized_affine_preimage(B, GREATER_OR_EQUAL, -A + 3);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, -A + 3) ***");

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
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
END_MAIN
