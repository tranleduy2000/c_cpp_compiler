/* Test Octagonal_Shape::fold_space_dimensions().
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

  TOctagonal_Shape oct1(3);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  oct1.fold_space_dimensions(to_fold, B);

  Octagonal_Shape<mpq_class> known_result(2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {A} into B ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  oct1.fold_space_dimensions(to_fold, B);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {A} into B ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(A - C <= 2);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;

  oct1.fold_space_dimensions(to_fold, B);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A - C <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {} into B ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A >= 1);
  oct1.add_constraint(A <= 3);
  oct1.add_constraint(B >= 7);
  oct1.add_constraint(B <= 12);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  oct1.fold_space_dimensions(to_fold, B);

  Octagonal_Shape<mpq_class> known_result(1);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 12);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {A} into B ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A >= 1);
  oct1.add_constraint(A <= 3);
  oct1.add_constraint(B >= 7);
  oct1.add_constraint(B <= 12);
  oct1.add_constraint(C == 15);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);
  to_fold.insert(B);

  oct1.fold_space_dimensions(to_fold, C);

  Octagonal_Shape<mpq_class> known_result(1);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(A <= 15);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {A, B} into C ***");

  return ok;
}

// Test folding dimensions into a lower dimension.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oct1(4);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(A - B <= 2);
  oct1.add_constraint(C >= 0);
  oct1.add_constraint(C - B <= 2);
  oct1.add_constraint(D >= 0);
  oct1.add_constraint(D - B <= 2);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(C);
  to_fold.insert(D);

  oct1.fold_space_dimensions(to_fold, A);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A - B <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {C, D} into A ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TOctagonal_Shape oct1(4);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(B == 0);
  oct1.add_constraint(A - B <= 2);
  oct1.add_constraint(C >= 0);
  oct1.add_constraint(C - B <= 2);
  oct1.add_constraint(D >= 0);
  oct1.add_constraint(D - B <= 2);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);
  to_fold.insert(D);

  oct1.fold_space_dimensions(to_fold, C);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {B, D} into C ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(A <= -1);
  oct1.add_constraint(A - B <= 2);
  oct1.add_constraint(C >= 0);
  oct1.add_constraint(C - B <= 2);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(B);

  oct1.fold_space_dimensions(to_fold, A);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {B, D} into C ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  //  Variable G(6);

  TOctagonal_Shape oc(5);
  oc.add_constraint(A + D <= 2);
  oc.add_constraint(B - E >= 2);

  Variables_Set to_be_folded;
  to_be_folded.insert(C);
  to_be_folded.insert(D);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::fold_space_dimensions(to_be_folded, v):
    // it is illegal to fold a variable that is not in the space
    // of the octagon.
    oc.fold_space_dimensions(to_be_folded, F);
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
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  // Variable D(3);
  // Variable E(4);
  Variable F(5);
  // Variable G(6);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A + B <= 2);
  oc.add_constraint(C - B >= 2);

  Variables_Set to_be_folded;
  to_be_folded.insert(C);

  try {
    to_be_folded.insert(F);
    // This is an invalid use of the method
    // Octagonal_Shape::fold_space_dimensions(to_be_folded, v):
    // it is illegal to insert in the set `to_be_folded' a variable
    // that is not in the space of the octagon.
    oc.fold_space_dimensions(to_be_folded, A);
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
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A + B <= 2);
  oc.add_constraint(C - B >= 2);

  Variables_Set to_be_folded;
  to_be_folded.insert(C);

  try {
    to_be_folded.insert(A);
    // This is an invalid use of the method
    // Octagonal_Shape::fold_space_dimensions(to_be_folded, v):
    // it is illegal to insert in the set `to_be_folded' the variable
    // `var' that is not in the space of the octagon.
    oc.fold_space_dimensions(to_be_folded, A);
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
test12() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2, EMPTY);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  oct1.OK();

  oct1.fold_space_dimensions(to_fold, B);

  Octagonal_Shape<mpq_class> known_result(1, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {A} into B ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(1, EMPTY);

  oct1.add_space_dimensions_and_embed(1);

  print_constraints(oct1, "*** oct1 ***");

  // This is the set of the variables that we want to fold.
  Variables_Set to_fold;
  to_fold.insert(A);

  oct1.OK();

  oct1.fold_space_dimensions(to_fold, B);

  Octagonal_Shape<mpq_class> known_result(1, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** after folding {A} into B ***");

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
END_MAIN
