/* Test Polyhedron::fold_space_dimensions() for non-closed polyhedra.
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

// Test with an empty polyhedron.
bool
test01() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(3, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  ph1.fold_space_dimensions(to_fold, B);

  NNC_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after folding {A} into B ***");

  return ok;
}

// Trivial fold.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  NNC_Polyhedron ph1(3);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A + B + C < 2);

  print_constraints(ph1, "*** ph1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;

  ph1.fold_space_dimensions(to_fold, B);

  NNC_Polyhedron known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A + B + C < 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after folding {} into B ***");

  return ok;
}

// Test as given in [GopanDMDRS04] on page 519 but with strict constraints.
bool
test03() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A > 1);
  ph1.add_constraint(A < 3);
  ph1.add_constraint(B > 7);
  ph1.add_constraint(B < 12);

  print_constraints(ph1, "*** ph1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  ph1.fold_space_dimensions(to_fold, B);

  NNC_Polyhedron known_result(1);
  known_result.add_constraint(A > 1);
  known_result.add_constraint(A < 12);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after folding {A} into B ***");

  return ok;
}

// Test folding several dimensions into a higher dimension.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  NNC_Polyhedron ph1(3);
  ph1.add_constraint(A > 1);
  ph1.add_constraint(A <= 3);
  ph1.add_constraint(B > 7);
  ph1.add_constraint(B < 12);
  ph1.add_constraint(C == 15);

  print_constraints(ph1, "*** ph1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);
  to_fold.insert(B);

  ph1.fold_space_dimensions(to_fold, C);

  NNC_Polyhedron known_result(1);
  known_result.add_constraint(A > 1);
  known_result.add_constraint(A <= 15);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after folding {A,B} into C ***");

  return ok;
}

// Test folding dimensions into a lower dimension.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  NNC_Polyhedron ph1(4);
  ph1.add_constraint(A > 0);
  ph1.add_constraint(A + B < 2);
  ph1.add_constraint(C > 0);
  ph1.add_constraint(C + B < 2);
  ph1.add_constraint(D > 0);
  ph1.add_constraint(D + B <= 2);

  print_constraints(ph1, "*** ph1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(C);
  to_fold.insert(D);

  ph1.fold_space_dimensions(to_fold, A);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A > 0);
  known_result.add_constraint(A + B <= 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after folding {C,D} into A ***");

  return ok;
}

// Test folding dimensions into an intermediate dimension.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  NNC_Polyhedron ph1(4);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(B > 0);
  ph1.add_constraint(A + B <= 2);
  ph1.add_constraint(C > 0);
  ph1.add_constraint(C + B <= 2);
  ph1.add_constraint(D > 0);
  ph1.add_constraint(D + B <= 2);

  print_constraints(ph1, "*** ph1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);
  to_fold.insert(D);

  ph1.fold_space_dimensions(to_fold, C);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A < 2);
  known_result.add_constraint(B > 0);
  known_result.add_constraint(B < 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after folding {B,D} into C ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST_F8(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
