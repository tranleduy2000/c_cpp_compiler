/* Test Box::affine_preimage().
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

  TBox box(3);
  box.add_constraint(x <= 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");

  box.affine_preimage(x, y);

  Rational_Box known_result(3);
  known_result.add_constraint(y <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_preimage(x, y) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(B >= 0);

  print_constraints(box, "*** box ***");

  box.affine_preimage(A, B-1);

  Rational_Box known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_preimage(A, B-1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 2);
  box.add_constraint(B >= 0);

  print_constraints(box, "*** box ***");

  box.affine_preimage(A, 2*A + 2, 2);

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_preimage(A, 2*A + 2, 2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 2);
  box.add_constraint(B >= 0);

  print_constraints(box, "*** box ***");

  box.affine_preimage(B, Linear_Expression(3));

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.affine_preimage(B, 3) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBox box(2);

  try {
    // This is an incorrect use of the method
    // Box::affine_preimage(v, expr, d): it is illegal
    // to apply it to a expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    box.affine_preimage(x, x + 1, d);
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

  TBox box(2);

  try {
    // This is an incorrect use of the method
    // Box::affine_preimage(v, expr, d): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the box.
    box.affine_preimage(y, z);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
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
END_MAIN
