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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(x + y >= 0);
  oct.add_constraint(y >= 0);
  oct.add_constraint(z <= 2);
  oct.add_constraint(z - x >= 9);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::generalized_affine_preimage(v, e, d):
    // it is illegal to apply the method with denominator equal to zero.
    Coefficient d = 0;
    oct.generalized_affine_preimage(y, LESS_OR_EQUAL, y + 1, d);
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
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::generalized_affine_preimage(v, r, expr, d):
    // it is illegal to use a strict relation symbol.
    oct.generalized_affine_preimage(x, LESS_THAN, x + 1);
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
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::generalized_affine_preimage(v, r, expr, d):
    // it is illegal to use a strict relation symbol.
    oct.generalized_affine_preimage(x, GREATER_THAN, x + 1);
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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::generalized_affine_preimage(v, r, expr, d):
    // it is illegal to apply the method to an expression having
    // space dimension greater than the octagon's space dimension.
    oct.generalized_affine_preimage(y, GREATER_OR_EQUAL, z);
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
  Variable C(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::generalized_affine_preimage(v, r, expr, d):
    // it is illegal to use a variable in the 'expr' expression that
    // does not appear in the octagon.
    oct.generalized_affine_preimage(A, GREATER_OR_EQUAL, B + C);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);

  try {
    // This is an incorrect use of method
    // Octagonal_Shape::generalized_affine_preimage(v, r, expr, d):
    // it is illegal to apply the method to a variable having space dimension
    // greater than the octagon's space dimension.
    oct.generalized_affine_preimage(C, GREATER_OR_EQUAL, B - A);
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
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B >= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 4);

  oct.generalized_affine_preimage(A, GREATER_OR_EQUAL, B + 3, 2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "GREATER_OR_EQUAL, B + 3, 2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B <= 4);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(2*B >= 3);

  oct.generalized_affine_preimage(A, GREATER_OR_EQUAL, -B + 3, 2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "GREATER_OR_EQUAL, -B + 3, 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.generalized_affine_preimage(A, GREATER_OR_EQUAL, -B + 3, 5);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A, "
                    "GREATER_OR_EQUAL, -B + 3, 5) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(2);
  Linear_Expression e2(-B + 3);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B == 1);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(2, "
                    "GREATER_OR_EQUAL, -B + 3) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(-3);
  Linear_Expression e2(B + 12);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B >= -15);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(B - A >= 2);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-3, "
                    "LESS_OR_EQUAL, B + 12) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(-3);
  Linear_Expression e2(B + 12);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B == -15);
  known_result.add_constraint(B - A >= 2);
  known_result.add_constraint(B + A <= 0);

  oct.generalized_affine_preimage(e1, EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-3, "
                    "EQUAL, B + 12) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(A - 1);
  Linear_Expression e2(B - 2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A >= 2);
  oct.add_constraint(B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);
  known_result.generalized_affine_preimage(A, GREATER_OR_EQUAL, e2 + 1);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(A - 1, "
                    "GREATER_OR_EQUAL, B - 2) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Linear_Expression e1(-A - 1);
  Linear_Expression e2(2*C + B - 2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(B - A <= 2);
  oct.add_constraint(B <= 1);
  oct.add_constraint(C + B <= 1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);
  known_result.generalized_affine_preimage(A, GREATER_OR_EQUAL,
                                           2*C + B - 1, -1);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-A - 1, "
                    "LESS_OR_EQUAL, 2*C + B - 2) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Linear_Expression e1(-A - 1);
  Linear_Expression e2(D - 3*C - B - 2);

  TOctagonal_Shape oct(4);
  oct.add_constraint(B + A <= 2);
  oct.add_constraint(B >= 1);
  oct.add_constraint(B - D >= 1);
  oct.add_constraint(C <= 1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);
  known_result.generalized_affine_preimage(A, EQUAL, D - 3*C - B - 1, -1);

  oct.generalized_affine_preimage(e1, EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-A - 1, "
                    "EQUAL, D - 3*C - B - 2) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Linear_Expression e1(B - A - 1);
  Linear_Expression e2(3*C - 2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(B + A <= 2);
  oct.add_constraint(B >= 1);
  oct.add_constraint(C <= 1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(C >= -2);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B - A - 1, "
                    "GREATER_OR_EQUAL, 3*C - 2) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Linear_Expression e1(B - A - 1);
  Linear_Expression e2(B + 3*C - 2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 2);
  oct.add_constraint(B + A <= 2);
  oct.add_constraint(B >= 1);
  oct.add_constraint(C <= 1);
  oct.add_constraint(C >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(C <= 1);
  known_result.add_constraint(C >= -2);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B - A - 1, "
                    "LESS_OR_EQUAL, B + 3*C - 2) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1(B - A);
  Linear_Expression e2(A);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 2);
  oct.add_constraint(B <= 5);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 3);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B - A, "
                    "GREATER_OR_EQUAL, A) ***");

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Linear_Expression e1(B - A);
  Linear_Expression e2(B + 1);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A >= 1);
  oct.add_constraint(A <= 2);
  oct.add_constraint(B <= 3);
  oct.add_constraint(C <= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(C <= -1);

  oct.generalized_affine_preimage(e1, GREATER_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(B - A, "
                    "GREATER_OR_EQUAL, B + 1) ***");

  return ok;
}

bool
test20() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Linear_Expression e1(-C + B - 2);
  Linear_Expression e2(B + 1);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A >= 1);
  oct.add_constraint(A <= 2);
  oct.add_constraint(B <= 3);
  oct.add_constraint(C >= -1);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 2);

  oct.generalized_affine_preimage(e1, LESS_OR_EQUAL, e2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.generalized_affine_preimage(-C + B - 2, "
                    "LESS_OR_EQUAL, B + 1) ***");

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
