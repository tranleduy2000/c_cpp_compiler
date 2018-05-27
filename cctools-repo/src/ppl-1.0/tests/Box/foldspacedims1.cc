/* Test Box::fold_space_dimensions().
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

  TBox box(3);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  box.fold_space_dimensions(to_fold, B);

  TBox known_result(2);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {A} into B ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box(3, EMPTY);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  box.fold_space_dimensions(to_fold, B);

  TBox known_result(2, EMPTY);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {A} into B ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A >= 0);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;

  box.fold_space_dimensions(to_fold, B);

  TBox known_result(3);
  known_result.add_constraint(A >= 0);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {} into B ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 1);
  box.add_constraint(A <= 3);
  box.add_constraint(B >= 7);
  box.add_constraint(B <= 12);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  box.fold_space_dimensions(to_fold, B);

  TBox known_result(1);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 12);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {A} into B ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A >= 1);
  box.add_constraint(A <= 3);
  box.add_constraint(B >= 7);
  box.add_constraint(B <= 12);
  box.add_constraint(C == 15);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);
  to_fold.insert(B);

  box.fold_space_dimensions(to_fold, C);

  TBox known_result(1);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 15);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {A, B} into C ***");

  return ok;
}

// Test folding dimensions into a lower dimension.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(4);
  box.add_constraint(A >= 0);
  box.add_constraint(C >= 0);
  box.add_constraint(D >= 0);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(C);
  to_fold.insert(D);

  box.fold_space_dimensions(to_fold, A);

  TBox known_result(2);
  known_result.add_constraint(A >= 0);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {C, D} into A ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(4);
  box.add_constraint(A >= 0);
  box.add_constraint(B == 0);
  box.add_constraint(C >= 0);
  box.add_constraint(D >= 0);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);
  to_fold.insert(D);

  box.fold_space_dimensions(to_fold, C);

  TBox known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {B, D} into C ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A >= 0);
  box.add_constraint(A <= -1);
  box.add_constraint(C >= 0);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);

  box.fold_space_dimensions(to_fold, A);

  TBox known_result(2, EMPTY);

  bool ok = (box == known_result);

  print_constraints(box, "*** after folding {B, D} into C ***");

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
END_MAIN
