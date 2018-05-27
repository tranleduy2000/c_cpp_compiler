/* Test BD_Shape::generalized_affine_preimage().
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
  Variable D(3);
  Linear_Expression e1(3*B + C);
  Linear_Expression e2(2*A);

  TBD_Shape bds(4);
  bds.add_constraint(A <= 1);
  bds.add_constraint(B <= 13);
  bds.add_constraint(C <= 5);
  bds.add_constraint(D <= 2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B >= 9);
  bds.add_constraint(C >= 3);
  bds.add_constraint(D >= 0);

  print_constraints(bds, "*** bds ***");

  bds.generalized_affine_preimage(e1, EQUAL, e2);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(D <= 2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(D >= 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.generalized_affine_preimage(3*B + C, "
                    "EQUAL, 2*A) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a variable in the `rhs' expression that
    // does not appear in the BDS.

    bds.generalized_affine_preimage(A + B, GREATER_OR_EQUAL, B + C);
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
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // BD_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a variable in the `lhs' expression that
    // does not appear in the BDS.
    bds.generalized_affine_preimage(B + C, LESS_OR_EQUAL, A + 1);
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
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 0);

  try {
    // This is an incorrect use of the method
    // BD_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a strict relation symbol.
    bds.generalized_affine_preimage(A + B, GREATER_THAN, B + C);
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
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // BD_Shape::generalized_affine_preimage(lhs, r, rhs):
    // it is illegal to use a strict relation symbol.
    bds.generalized_affine_preimage(B + C, LESS_THAN, A + 1);
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
END_MAIN

