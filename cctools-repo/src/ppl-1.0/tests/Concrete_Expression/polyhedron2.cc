/* Test Polyhedron::refine_fp_interval_abstract_store,
   Polyhedron::refine_with_linear_form_inequality and
   Polyhedron::generalized_refine_with_linear_form_inequality.
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

// Tests incompatible dimensions.
bool
test01() {
  C_Polyhedron pol(1);
  FP_Interval_Abstract_Store store(1);
  store.set_interval(Variable(0), FP_Interval(0));
  try {
    FP_Linear_Form l1(Variable(3));
    FP_Linear_Form l2;
    pol.refine_with_linear_form_inequality(l1, l2);
  }
  catch (std::invalid_argument) {
    try {
      FP_Linear_Form l1;
      FP_Linear_Form l2(Variable(3));
      pol.refine_with_linear_form_inequality(l1, l2);
    }
    catch (std::invalid_argument) {
      nout << "incompatible dimensions." << endl;
      return true;
    }
  }

  return false;
}

// Tests A <= [57, 57].
bool
test02() {
  C_Polyhedron ph(1);
  FP_Interval_Abstract_Store store(1);
  store.set_interval(Variable(0), FP_Interval(static_cast<FP_Interval::boundary_type>(1.5)));
  FP_Interval interval(57);
  FP_Linear_Form lf1(Variable(0));
  FP_Linear_Form lf2(interval);
  ph.refine_with_linear_form_inequality(lf1, lf2);
  print_constraints(ph, "*** A <= [57, 57] ***");
  C_Polyhedron known_result(1);
  known_result.refine_with_constraint(Variable(0) <= 57);
  print_constraints(known_result, "*** known_result ***");

  return ph == known_result;
}

// Tests -A <= 0 && A <= 2 && -A <= 1 && -B <= -1 && C = 0.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  FP_Interval_Abstract_Store store(3);
  FP_Interval tmp(0);
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  store.set_interval(C, tmp);

  C_Polyhedron ph(3);
  ph.refine_with_linear_form_inequality(-FP_Linear_Form(A),
				 FP_Linear_Form(FP_Interval(0)));
  ph.refine_with_linear_form_inequality(FP_Linear_Form(A),
				 FP_Linear_Form(FP_Interval(2)));
  ph.refine_with_linear_form_inequality(-FP_Linear_Form(A),
				 FP_Linear_Form(FP_Interval(1)));
  ph.refine_with_linear_form_inequality(-FP_Linear_Form(B),
				 FP_Linear_Form(FP_Interval(-1)));
  ph.generalized_refine_with_linear_form_inequality(
    FP_Linear_Form(C), FP_Linear_Form(tmp), EQUAL);
  ph.generalized_refine_with_linear_form_inequality(
    FP_Linear_Form(C), FP_Linear_Form(tmp), NOT_EQUAL);
  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(C == 0);
  print_constraints(known_result, "*** known_result ***");

  return ph == known_result;
}

// Tests -A <= -1/3 && A <= 2/3 && -B <= 0 && B <= 1/3
// and refine_fp_interval_abstract_store.
bool
test04() {
  Variable A(0);
  Variable B(1);
  FP_Interval tmp0(0);
  tmp0.join_assign(10);
  FP_Interval_Abstract_Store store(2);
  store.set_interval(A, tmp0);
  store.set_interval(B, tmp0);
  FP_Interval tmp(tmp0);
  tmp = 2;
  tmp /= FP_Interval(3);
  FP_Linear_Form la(A);
  FP_Linear_Form lb(B);
  FP_Linear_Form lk(tmp);

  C_Polyhedron ph(2);
  ph.generalized_refine_with_linear_form_inequality(
    lk, la, GREATER_THAN);
  tmp = 1;
  lk -= tmp;
  ph.generalized_refine_with_linear_form_inequality(
    lk, -la, GREATER_OR_EQUAL);
  ph.generalized_refine_with_linear_form_inequality(
    lb, -lk, LESS_OR_EQUAL);
  tmp = 0;
  lk = FP_Linear_Form(tmp);
  ph.generalized_refine_with_linear_form_inequality(
    -lb, lk, LESS_THAN);
  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result1(2);
  known_result1.add_constraint(3*A >= 1);
  known_result1.add_constraint(3*A <= 2);
  known_result1.add_constraint(B >= 0);
  known_result1.add_constraint(3*B <= 1);
  print_constraints(known_result1, "*** known_result1 ***");

  bool ok1 = ph.contains(known_result1);

  ph.refine_fp_interval_abstract_store(store);
  nout << "*** FP_Interval_Abstract_Store ***" << endl;

  nout << "A = " << store.get_interval(A) << endl;
  bool ok2 = tmp0.contains(store.get_interval(A));

  nout << "B = " << store.get_interval(B) << endl;
  bool ok3 = tmp0.contains(store.get_interval(B));

  return ok1 && ok2 && ok3;

}

// Tests (2/3)*B + [-0.5, 0.5] >= (1/3)*A
// where A = B = [-1, 1].
bool
test05() {
  Variable A(0);
  Variable B(1);
  FP_Interval tmp0(-1);
  tmp0.join_assign(1);
  FP_Interval_Abstract_Store store(2);
  store.set_interval(A, tmp0);
  store.set_interval(B, tmp0);
  FP_Interval tmp(1);
  tmp /= FP_Interval(3);
  FP_Linear_Form la(A);
  la *= tmp;
  FP_Linear_Form lb(B);
  tmp += tmp;
  lb *= tmp;
  tmp.lower() = static_cast<FP_Interval::boundary_type>(-0.5);
  tmp.upper() = static_cast<FP_Interval::boundary_type>(0.5);
  lb += tmp;

  NNC_Polyhedron ph(2);
  FP_Linear_Form l(tmp0);
  ph.affine_form_image(A, l);
  ph.affine_form_image(B, l);
  ph.generalized_refine_with_linear_form_inequality(
    lb, la, GREATER_OR_EQUAL);
  print_constraints(ph, "*** ph ***");

  NNC_Polyhedron known_result1(2);
  known_result1.add_constraint(2*A <= 4*B + 3);
  known_result1.add_constraint(A >= -1);
  known_result1.add_constraint(A <= 1);
  known_result1.add_constraint(B >= -1);
  known_result1.add_constraint(B <= 1);
  print_constraints(known_result1, "*** known_result1 ***");

  bool ok1 = ph.contains(known_result1);

  ph.refine_fp_interval_abstract_store(store);
  nout << "*** FP_Interval_Abstract_Store ***" << endl;

  nout << "A = " << store.get_interval(A) << endl;
  bool ok2 = tmp0.contains(store.get_interval(A));

  nout << "B = " << store.get_interval(B) << endl;
  bool ok3 = tmp0.contains(store.get_interval(B));

  return ok1 && ok2 && ok3;

  return ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  if (sizeof(ANALYZER_FP_FORMAT) == 4) {
    DO_TEST_F32(test04);
#ifdef NDEBUG
    DO_TEST_F16(test05);
#else
    DO_TEST_F64A(test05);
#endif
  }
  else if (sizeof(ANALYZER_FP_FORMAT) == 8) {
    DO_TEST_F64(test04);
    DO_TEST_F64A(test05);
  }
  else {
    DO_TEST_F64(test04);
    DO_TEST_F64(test05);
  }
END_MAIN
