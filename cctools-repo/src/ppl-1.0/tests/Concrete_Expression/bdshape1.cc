/* Test BD_Shape::affine_form_image on interval linear forms.
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

// Tests space_dimensions and trivial cases.
bool
test01() {
  Variable A(0);
  Variable B(1);
  FP_BD_Shape bd1(0);
  bool ok1 = false;
  FP_Linear_Form l(A);

  try {
      bd1.affine_form_image(A, l);
  }
  catch(std::invalid_argument e) {
    nout << "bd1_space_dim < lf_space_dim" << endl;
    ok1 = true;
  }

  bool ok2 = false;
  FP_BD_Shape bd2(1);

  try {
    bd2.affine_form_image(B, l);
  }
  catch(std::invalid_argument e) {
    nout << "space_dim < var_id + 1" << endl;
    bd2.affine_form_image(A, l);
    Constraint_System cs(A < A);
    bd2.add_constraints(cs);
    bd2.affine_form_image(A, l);
    ok2 = true;
  }

  return ok1 && ok2;
}


// Tests affine_form_image(A, [-2, 1]).
bool
test02() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(2);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval free_term(-2);
  free_term.join_assign(1);
  FP_Linear_Form l(free_term);
  bd1.affine_form_image(A, l);
  print_constraints(bd1, "*** bd1.affine_form_image(A, [-2, 1]) ***");

  FP_BD_Shape known_result(2);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(-A <= 2);
  known_result.add_constraint(B <= 2);
  print_constraints(bd1, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(A, [-1, -1]*A + [0.5, 2]).
bool test03() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval free_term(static_cast<FP_Interval::boundary_type>(0.5));
  free_term.join_assign(2);
  FP_Linear_Form l(-A);
  l += free_term;
  bd1.affine_form_image(A, l);
  print_constraints(bd1, "*** bd1.affine_form_image(A, -A + [0.5, 2]) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(-2*A <= 3);
  known_result.add_constraint(B <= 2);
  print_constraints(known_result, "*** known_result ***");
  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(B, [1, 1]*B + [-1.5, 3.5]).
bool test04() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval free_term(static_cast<FP_Interval::boundary_type>(-1.5));
  free_term.join_assign(static_cast<FP_Interval::boundary_type>(3.5));
  FP_Linear_Form l(B);
  l += free_term;
  bd1.affine_form_image(B, l);
  print_constraints(bd1, "*** bd1.affine_form_image(B, B + [-1.5, 3.5]) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(2*B <= 11);
  known_result.add_constraint(-2*B + 2*A <= 9);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(A, [1, 1]*B + [-1, 0.5]).
bool test05() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval free_term(-1);
  free_term.join_assign(static_cast<FP_Interval::boundary_type>(0.5));
  FP_Linear_Form l(B);
  l += free_term;
  bd1.affine_form_image(A, l);
  print_constraints(bd1, "*** bd1.affine_form_image(A, B + [-1, 0.5]) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(2*A <= 5);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(B - A <= 1);
  known_result.add_constraint(2*A - 2*B <= 1);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(B, [1, 1]*A + [-3, 1]).
bool test06() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval free_term(-3);
  free_term.join_assign(1);
  FP_Linear_Form l(A);
  l += free_term;
  bd1.affine_form_image(B, l);
  print_constraints(bd1, "*** bd1.affine_form_image(B, A + [-3, 1]) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B <= 3);
  known_result.add_constraint(B - A <= 1);
  known_result.add_constraint(A - B <= 3);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(B, [-1, -1]*A + [0, 4]).
bool test07() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval free_term(0);
  free_term.join_assign(4);
  FP_Linear_Form l(-A);
  l += free_term;
  bd1.affine_form_image(B, l);
  print_constraints(bd1, "*** bd1.affine_form_image(B, -A + [0, 4]) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(-B <= 2);
  known_result.add_constraint(A - B <= 4);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(A, [-1, -1]*B + [0, 2]).
bool test08() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  bd1.add_constraint(B >= -10);
  FP_Interval free_term(0);
  free_term.join_assign(2);
  FP_Linear_Form l(-B);
  l += free_term;
  bd1.affine_form_image(A, l);
  print_constraints(bd1, "*** bd1.affine_form_image(A, -B + [0, 2]) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(-A <= 2);
  known_result.add_constraint(A <= 12);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(B >= -10);
  known_result.add_constraint(-A + B <= 4);
  known_result.add_constraint(A - B <= 22);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(B, [-0.5, 0.5]*A).
bool test09() {
  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(3);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(A - B <= 3);
  bd1.add_constraint(B <= 2);
  FP_Interval coeff(static_cast<FP_Interval::boundary_type>(-0.5));
  coeff.join_assign(static_cast<FP_Interval::boundary_type>(0.5));
  FP_Linear_Form l(A);
  l *= coeff;
  bd1.affine_form_image(B, l);
  print_constraints(bd1, "*** bd1.affine_form_image(B, [-0.5, 0.5]*A) ***");

  FP_BD_Shape known_result(3);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(A - B <= 3);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (bd1 == known_result);

  return ok;
}

// Tests affine_form_image(A, i + i0*A + i1*B) where
// i = [0, 2], i0 = [1, 1] and i1 = [1, 2].
bool test10() {

  Variable A(0);
  Variable B(1);

  FP_BD_Shape bd1(2);
  bd1.add_constraint(A >= 0);
  bd1.add_constraint(A <= 2);
  bd1.add_constraint(B >= 0);
  bd1.add_constraint(B <= 2);
  bd1.add_constraint(A - B >= 0);
  FP_Interval tmp(1);
  FP_Linear_Form l(A);
  l *= tmp;
  tmp.lower() = 0;
  tmp.upper() = 2;
  l += tmp;
  tmp.lower() = 1;
  tmp.upper() = 2;
  l += tmp * FP_Linear_Form(B);
  bd1.affine_form_image(A,l);
  print_constraints(bd1, "*** bd1.affine_form_image(A, i + i0*A + i1*B) ***");

  FP_BD_Shape know_result(2);
  know_result.add_constraint(A >= 0);
  know_result.add_constraint(A <= 8);
  know_result.add_constraint(B >= 0);
  know_result.add_constraint(B <= 2);
  know_result.add_constraint(A - B <= 6);
  know_result.add_constraint(-A + B <= 0);
  print_constraints(know_result, "*** know_result ***");

  bool ok = (bd1 == know_result);

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
