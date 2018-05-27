/* Test BD_Shape::fold_space_dimensions().
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

  TBD_Shape bds(3);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  bds.fold_space_dimensions(to_fold, B);

  TBD_Shape known_result(2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {A} into B ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(3, EMPTY);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  bds.fold_space_dimensions(to_fold, B);

  TBD_Shape known_result(2, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {A} into B ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A - C <= 2);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;

  bds.fold_space_dimensions(to_fold, B);

  TBD_Shape known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A - C <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {} into B ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);
  bds.add_constraint(A <= 3);
  bds.add_constraint(B >= 7);
  bds.add_constraint(B <= 12);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  bds.fold_space_dimensions(to_fold, B);

  TBD_Shape known_result(1);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 12);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {A} into B ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 1);
  bds.add_constraint(A <= 3);
  bds.add_constraint(B >= 7);
  bds.add_constraint(B <= 12);
  bds.add_constraint(C == 15);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);
  to_fold.insert(B);

  bds.fold_space_dimensions(to_fold, C);

  TBD_Shape known_result(1);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 15);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {A, B} into C ***");

  return ok;
}

// Test folding dimensions into a lower dimension.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(4);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(C >= 0);
  bds.add_constraint(C - B <= 2);
  bds.add_constraint(D >= 0);
  bds.add_constraint(D - B <= 2);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(C);
  to_fold.insert(D);

  bds.fold_space_dimensions(to_fold, A);

  TBD_Shape known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A - B <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {C, D} into A ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(4);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B == 0);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(C >= 0);
  bds.add_constraint(C - B <= 2);
  bds.add_constraint(D >= 0);
  bds.add_constraint(D - B <= 2);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);
  to_fold.insert(D);

  bds.fold_space_dimensions(to_fold, C);

  TBD_Shape known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {B, D} into C ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= -1);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(C >= 0);
  bds.add_constraint(C - B <= 2);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);

  bds.fold_space_dimensions(to_fold, A);

  TBD_Shape known_result(2, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {B, D} into C ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2, EMPTY);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  bds.OK();

  bds.fold_space_dimensions(to_fold, B);

  TBD_Shape known_result(1, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {A} into B ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(1, EMPTY);

  bds.add_space_dimensions_and_embed(1);

  print_constraints(bds, "*** bds ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  bds.OK();

  bds.fold_space_dimensions(to_fold, B);

  TBD_Shape known_result(1, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** after folding {A} into B ***");

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
