/* Test BD_Shape::affine_preimage().
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

  TBD_Shape bds(3);
  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y <= 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(x, y);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(x, y) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B >= 0);
  bds.add_constraint(A - B - 3 >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, B-1);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, B-1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 2);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, 2*A + 2, 2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, 2*A + 2, 2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 2);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(B, Linear_Expression(3));

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(B, 3) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // BD_Shape::affine_preimage(v, expr, d): it is illegal
    // to apply it to a expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    bds.affine_preimage(x, x + 1, d);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // BD_Shape::affine_preimage(v, expr, d): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the BDS.
    bds.affine_preimage(y, z);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, 4*B + 6*C + 2, -2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, 4*B + 6*C + 2, -2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, 2*A + 3*C + 2, 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -2);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, 2*A + 3*C + 2, 2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, 2*A + 3*C + 2, 2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -2);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, -3*A + C - 1, 2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, 3*A + C - 1, -2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, 3*A + C - 1, -2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(A, -3*A + C - 1, -2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(A, -3*A + C - 1, -2) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(4);
  bds.add_constraint(A == 2);
  bds.add_constraint(B == 0);
  bds.add_constraint(C >= -3);
  bds.add_constraint(D <= 5);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(D, 4*A - B + 2*C + 5*D - 1, 3);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A == 2);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C >= -3);
  known_result.add_constraint(5*D <= 14);

  bool ok = check_result(bds, known_result, "7.63e-7", "3.82e-7", "1.91e-7");

  print_constraints(bds, "*** bds.affine_preimage"
                        "(D, 4*A - B + 2*C + 5*D - 1, 3) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  bds.affine_preimage(B, -B);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(B, -B) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds);

  bds.affine_preimage(B, -B, -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(B, -B, -1) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= -1);
  bds.add_constraint(B <= 0);
  bds.add_constraint(C >= 0);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B >= 2);
  known_result.add_constraint(C >= 0);

  bds.affine_preimage(B, -3*B + 6, 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_preimage(B, -3*B + 6, 3) ***");

  return ok;
}

bool
test16() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A <= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(A, 2*A, 3);

  BD_Shape<mpq_class> mpq_known_result(1);
  mpq_known_result.add_constraint(3*A <= -2);

  bool ok = check_result(bds, mpq_known_result,
                         "9.54e-8", "9.54e-8", "9.54e-8");

  print_constraints(bds, "*** bds.affine_image(A, 2*A, 3) ***");

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
END_MAIN
