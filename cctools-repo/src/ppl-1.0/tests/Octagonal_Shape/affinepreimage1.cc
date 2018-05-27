/* Test Octagonal_Shape::affine_preimage().
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

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(x <= 2);
  oc1.add_constraint(x - y <= 3);
  oc1.add_constraint(y <= 2);

  print_constraints(oc1, "*** oc1 ***");

  oc1.affine_preimage(x, y);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(y <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.affine_preimage(x, y) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A - B - 3 >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, B - 1);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, B-1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(A + B >= 2);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, 2*A + 2, 2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(A, 2*A + 2, 2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, Linear_Expression(3));

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, 3) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, B - 2);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(B >= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, B - 2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(x + y >= 0);
  oct.add_constraint(x >= 0);
  oct.add_constraint(z <= 2);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::affine_preimage(v, e, d): it is illegal to apply
    // the method to a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    oct.affine_preimage(y, y + 1, d);
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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x - y >= 2);
  oct.add_constraint(y <= 7);

  try {
    // This is an invalid use of method
    // Octagonal_Shape::affine_preimage(v, expr, d): it is illegal to
    // apply this method to a variable that is not in the space of
    // the polyhedron.
    oct.affine_preimage(x, z - 2);
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
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x - y >= 2);
  oct.add_constraint(y <= 7);

  try {
    // This is an invalid use of method
    // Octagonal_Shape::affine_preimage(v, expr, d): it is illegal to
    // apply this method to a variable that is not in the space of
    // the polyhedron.
    oct.affine_preimage(z, x - 2);
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
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 2);
  oc.add_constraint(B <= -2);
  oc.add_constraint(B - A >= 0);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(B, A + 3);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.affine_preimage(B, A + 3) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= -1);

  print_constraints(oc, "*** oc ***");

  oc.affine_preimage(A, 3*A, 2);

  Octagonal_Shape<mpq_class> mpq_known_result(3);
  mpq_known_result.add_constraint(3*A <= -2);

  bool ok = check_result(oc, mpq_known_result,
                         "9.54e-8", "9.54e-8", "9.54e-8");

  print_constraints(oc, "*** oc.affine_preimage(A, 3*A, 2) ***");

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
END_MAIN
