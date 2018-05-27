/* Test Octagonal_Shape::bounded_affine_image().
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

  oct.bounded_affine_image(A, Linear_Expression(7), Linear_Expression(3));

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 7, 3) ***");

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
  known_result.add_constraint(A <= 7);
  known_result.add_constraint(A >= 3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);

  oct.bounded_affine_image(A, Linear_Expression(3), Linear_Expression(7));

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 3, 7) ***");

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

  oct.bounded_affine_image(A, Linear_Expression(3), Linear_Expression(7), -1);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 3, 7, -1) ***");

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

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= -7);
  known_result.add_constraint(A <= -3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);

  oct.bounded_affine_image(A, Linear_Expression(7), Linear_Expression(3), -1);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 7, 3, -1) ***");

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
  known_result.add_constraint(A >= 3);
  known_result.add_constraint(A <= 6);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(A - B <= 4);

  oct.bounded_affine_image(A, Linear_Expression(3), A+2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 3, A+2) ***");

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

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  oct.bounded_affine_image(A, Linear_Expression(3), -A+2);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 3, -A+2) ***");

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

  TOctagonal_Shape oct1 = oct;
  TOctagonal_Shape oct2 = oct;


  oct.bounded_affine_image(A, Linear_Expression(3), B+5);

  oct1.generalized_affine_image(A, GREATER_OR_EQUAL, Linear_Expression(3));
  oct2.generalized_affine_image(A, LESS_OR_EQUAL, B+5);
  oct1.intersection_assign(oct2);

  bool ok = (oct == oct1);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 3, B+5) ***");

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
  known_result.add_constraint(A >= -3);
  known_result.add_constraint(A <= 5);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(A + B <= 5);

  oct.bounded_affine_image(A, Linear_Expression(3), B-5, -1);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 3, B-5, -1) ***");

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
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(A <= 23);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(A - B <= 19);

  oct.bounded_affine_image(A, Linear_Expression(2), 4*A + 3*B - 5);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 2, 4*A + 3*B - 5) ***");

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
  known_result.add_constraint(A >= -1);
  known_result.add_constraint(2*A <= 5);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(2*(A + B) <= 5);

  oct.bounded_affine_image(A, Linear_Expression(2), 4*A + 3*B - 5, -2);

  bool ok = check_result(oct, known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 2, "
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
  known_result.add_constraint(A >= -5);
  known_result.add_constraint(A <= 23);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(A - B <= 19);
  known_result.add_constraint(B - A <= 5);

  oct.bounded_affine_image(A, 4*A + 3*B - 5, 4*A + 3*B - 5);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 4*A + 3*B - 5, "
                    "4*A + 3*B - 5) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(C <= 5);
  oct.add_constraint(C >= -1);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);
  oct.add_constraint(A - C <= 3);
  oct.add_constraint(B - C <= 7);
  oct.add_constraint(A - C >= 1);
  oct.add_constraint(B - C >= -8);


  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);

  known_result.add_constraint(A <= 9);
  known_result.add_constraint(A >= 4);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(C <= 3);
  known_result.add_constraint(C >= -1);
  known_result.add_constraint(C - B <= 1);

  oct.bounded_affine_image(A, 4*A + 4, Linear_Expression(9));

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 4*A + 4, "
                    "9) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A >= 0);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 0);
  oct.add_constraint(B <= 4);
  oct.add_constraint(C <= 5);
  oct.add_constraint(C >= -1);
  oct.add_constraint(A - B <= 2);
  oct.add_constraint(A - B >= -2);
  oct.add_constraint(A - C <= 3);
  oct.add_constraint(B - C <= 7);
  oct.add_constraint(A - C >= 1);
  oct.add_constraint(B - C >= -8);


  print_constraints(oct, "*** oct ***");

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= 9);
  known_result.add_constraint(A >= -1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(C <= 3);
  known_result.add_constraint(C >= -1);
  known_result.add_constraint(A - C >= 0);
  known_result.add_constraint(B - C >= -1);

  oct.bounded_affine_image(A, 4*C + 3, Linear_Expression(9));

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, 4*C + 3, "
                    "9) ***");

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

  Octagonal_Shape<mpq_class> known_result(oct);

  oct.bounded_affine_image(A, A, A);

  bool ok = (Octagonal_Shape<mpq_class>(oct) == known_result);

  print_constraints(oct,
                    "*** oct.bounded_affine_image(A, A, A) ***");

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
    // Octagonal_Shape::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to apply this method to an expression having a
    // space dimension greater than the OS's space dimension.
    oct.bounded_affine_image(y, x, z);
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
    // Octagonal_Shape::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to apply this method to an expression having a
    // space dimension greater than the OS's space dimension.
    oct.bounded_affine_image(y, z, x);
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
    // Octagonal_Shape::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to apply this method to an expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    oct.bounded_affine_image(x, Linear_Expression(0), x + 1, d);
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
    // Octagonal_Shape::bounded_affine_image(v, lb_expr, ub_epxr, d):
    // it is illegal to apply this method to a variable
    // that is not in the space of the octagon.
    oct.bounded_affine_image(y, x + 1, Linear_Expression(8));
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
