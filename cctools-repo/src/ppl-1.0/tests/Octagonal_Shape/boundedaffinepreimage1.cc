/* Test Octagonal_Shape::bounded_affine_preimage().
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

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.bounded_affine_preimage(A, Linear_Expression(7), Linear_Expression(3));

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 7, 3) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);

  oct.bounded_affine_preimage(A, Linear_Expression(3), Linear_Expression(7));

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 3, 7) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.bounded_affine_preimage(A, Linear_Expression(3), Linear_Expression(7),
                              -1);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 3, 7, -1) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.bounded_affine_preimage(A, Linear_Expression(7),
                              Linear_Expression(3), -1);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 7, 3, -1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(B - A <= 3);
  known_result.add_constraint(A + B >= 2);

  oct.bounded_affine_preimage(A, Linear_Expression(3), A+2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 3, A+2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);

  oct.bounded_affine_preimage(A, Linear_Expression(3), -A+2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 3, -A+2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);

  oct.bounded_affine_preimage(A, Linear_Expression(3), B+5);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 3, B+5) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(2*B <= 7);

  oct.bounded_affine_preimage(A, Linear_Expression(3), B-5, -1);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 3, B-5, -1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(4*A >= -5);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(4*(B - A) <= 21);
  known_result.add_constraint(4*(A + B) >= -5);

  oct.bounded_affine_preimage(A, Linear_Expression(2), 4*A + 3*B - 5);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 2, "
                    "4*A + 3*B - 5) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(4*A <= 5);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(4*(A + B) <= 9);

  oct.bounded_affine_preimage(A, Linear_Expression(2), 4*A + 3*B - 5, -2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 2, "
                    "4*A + 3*B - 5, -2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(4*A >= -7);
  known_result.add_constraint(4*A <= 9);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(4*(A + B) <= 13);
  known_result.add_constraint(4*(B - A) <= 23);

  oct.bounded_affine_preimage(A, 4*A + 3*B - 5, 4*A + 3*B - 5);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 4*A + 3*B - 5, "
                    "4*A + 3*B - 5) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(oct);

  oct.bounded_affine_preimage(A, A, A);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, A, A) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A >= 1);
  known_result.add_constraint(4*A <= 9);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(4*B <= 11);
  known_result.add_constraint(4*(A + B) <= 13);

  oct.bounded_affine_preimage(A, 4*A + 3*B - 5, 2*A - 5*B - 1);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 4*A + 3*B - 5, "
                    "2*A - 5*B - 1) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);

  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(4*A >= -15);
  known_result.add_constraint(2*A <= 21);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(4*(A + B) >= -3);
  known_result.add_constraint(2*(A - B) <= 13);

  oct.bounded_affine_preimage(A, 4*A + 3*B - 5, 2*A - 5*B - 1, -2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_preimage(A, 4*A + 3*B - 5, "
                    "2*A - 5*B - 1, -2) ***");

  return ok;
}

bool
test15() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::bounded_affine_preimage(v, lb_expr, ub_expr, d):
    // it is illegal to apply this method to an expression having a
    // space dimension greater than the OS's space dimension.
    oct.bounded_affine_preimage(y, x, z);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test16() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::bounded_affine_preimage(v, lb_expr, ub_expr, d):
    // it is illegal to apply this method to an expression having a
    // space dimension greater than the OS's space dimension.
    oct.bounded_affine_preimage(y, z, x);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test17() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::bounded_affine_preimage(v, lb_expr, ub_expr, d):
    // it is illegal to apply this method to an expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    oct.bounded_affine_preimage(x, Linear_Expression(0), x + 1, d);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test18() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct(1);
  oct.add_constraint(x >= 1);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::bounded_affine_preimage(v, lb_expr, ub_epxr, d):
    // it is illegal to apply this method to a variable
    // that is not in the space of the octagon.
    oct.bounded_affine_preimage(y, x + 1, Linear_Expression(8));
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
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
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
END_MAIN
