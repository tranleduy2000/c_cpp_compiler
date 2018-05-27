/* Test Polyhedron::affine_form_image on interval linear forms.
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

// Tests ph.affine_form_image(B, 3).
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  FP_Interval_Abstract_Store store(3);
  FP_Interval tmp(0);
  store.set_interval(B, tmp);
  tmp.lower() = -2;
  tmp.upper() = 2;
  store.set_interval(A, tmp);
  tmp.lower() = -1;
  tmp.upper() = 1;
  store.set_interval(C, tmp);
  C_Polyhedron ph(3);
  ph.add_constraint(C == 0);
  ph.add_constraint(A == -2);
  tmp = 3;
  FP_Linear_Form l(tmp);

  ph.affine_form_image(B, l);
  print_constraints(ph, "*** ph.affine_form_image(B, 3) ***");

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point(-2 * A + 3 * B));
  print_constraints(known_result, "*** know_result ***");

  bool ok = (ph == known_result);
  return ok;
}

// Tests ph.affine_form_image(A, A + B + 1).
bool
test02() {
  Variable A(0);
  Variable B(1);
  FP_Interval_Abstract_Store store(2);
  FP_Interval tmp(1);
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  C_Polyhedron ph(2);
  ph.add_constraint(A >= B);
  ph.add_constraint(B >= 0);
  ph.add_constraint(A <= 3);
  FP_Linear_Form l(A);
  l += B;
  l += tmp;
  ph.affine_form_image(A, l);
  print_constraints(ph, "*** ph.affine_form_image(A, A + B + 1) ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(A - 2*B - 1 >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - B <= 4);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (ph == known_result);

  return ok;
}

// Tests ph.affine_form_image(A, (A + 1) / 2).
bool
test03() {
  Variable A(0);
  Variable B(1);
  FP_Interval_Abstract_Store store(2);
  FP_Interval tmp(FP_Interval::boundary_type(1 / 2.0));
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(ray(B));
  C_Polyhedron ph(gs);
  FP_Linear_Form l(A);
  l *= tmp;
  l += tmp;
  ph.affine_form_image(A, l);
  print_constraints(ph, "*** ph.affine_form_image(A, (A + 1) / 2) ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(2*A >= 1);
  known_result.add_constraint(B >= 0);
  print_constraints(known_result, "*** known_result ***");

  bool ok = (ph == known_result);

  return ok;
}

// Tests ph.affine_form_image(A, (B + 2) / (-3)).
bool
test04() {
  Variable A(0);
  Variable B(1);
  FP_Interval_Abstract_Store store(2);
  FP_Interval tmp(-1);
  tmp /= FP_Interval(3);
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A));
  FP_Linear_Form l(B);
  l *= tmp;
  tmp += tmp;
  l += tmp;
  ph.affine_form_image(A, l);
  print_constraints(ph, "*** ph.affine_form_image(A, (B + 2) / (-3)) ***");

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(-2*A, 3));
  print_constraints(known_result, "*** known_result ***");

  bool ok = (ph.contains(known_result));

  return ok;
}

// Tests ph.affine_form_image(B, (A - B + 2) / (-3)).
bool
test05() {
  Variable A(0);
  Variable B(1);
  FP_Interval_Abstract_Store store(2);
  FP_Interval tmp(2);
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  C_Polyhedron ph(2);
  ph.add_constraint(A >= 2);
  ph.add_constraint(A <= 3);
  ph.add_constraint(B >= 1);
  ph.add_constraint(2*A >= B);
  FP_Linear_Form l(A);
  l += tmp;
  l -= B;
  l /= FP_Interval(-3);

  ph.affine_form_image(B, l);
  print_constraints(ph,
    "*** ph.affine_form_image(B, (A - B + 2) / (-3)) ***");

  C_Polyhedron known_result(2, EMPTY);
  known_result.add_generator(point(2*A));
  known_result.add_generator(point(2*A - B));
  known_result.add_generator(point(9*A + B, 3));
  known_result.add_generator(point(9*A - 4*B, 3));
  print_constraints(known_result, "*** known_result ***");

  bool ok = (ph.contains(known_result));

  return ok;
}

// Tests ph.affine_form_image(A, (-A - 1) / (-1)).
bool
test06() {
  Variable A(0);
  Variable B(1);
  FP_Interval_Abstract_Store store(2);
  FP_Interval tmp(-1);
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  FP_Linear_Form l(-A);
  l += tmp;
  l /= tmp;
  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A));
  gs.insert(point(B));
  gs.insert(point(A + B));
  C_Polyhedron ph(gs);
  ph.affine_form_image(A, l);
  print_constraints(ph,
    "*** ph.affine_form_image(A, (-A - 1) / (-1)) ***");

  Generator_System known_gs;
  known_gs.insert(point(A));
  known_gs.insert(point(2*A));
  known_gs.insert(point(A + B));
  known_gs.insert(point(2*A + B));
  C_Polyhedron known_result(known_gs);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** known_result ***");

  return ok;
}

// Tests affine_form_image on NNC_Polyhedron.
bool
test07() {
  Variable A(0);
  Variable B(1);
  FP_Interval_Abstract_Store store(2);
  FP_Interval tmp(2);
  store.set_interval(A, tmp);
  store.set_interval(B, tmp);
  FP_Linear_Form l(A);

  NNC_Polyhedron p1(2);
  p1.add_constraint(B == 0);
  p1.add_constraint(-A > 0);

  NNC_Polyhedron p2(2);
  p2.add_constraint(B == 0);
  p2.add_constraint(A >= 0);

  p1.affine_form_image(B, l);
  print_constraints(p1, "*** p1.affine_form_image(B, A) ***");
  NNC_Polyhedron known_result1(2);
  known_result1.add_constraint(A - B == 0);
  known_result1.add_constraint(A < 0);
  print_constraints(p1, "*** known_result1 ***");
  bool ok1 = (p1 == known_result1);

  l += tmp;
  p2.affine_form_image(B, l);
  print_constraints(p2, "*** p2.affine_form_image(B, A + 2) ***");
  NNC_Polyhedron known_result2(2);
  known_result2.add_constraint(A - B == -2);
  known_result2.add_constraint(A >= 0);
  print_constraints(p2, "*** known_result2 ***");
  bool ok2 = (p2 == known_result2);

  return ok1 && ok2;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  if (sizeof(ANALYZER_FP_FORMAT) == 4) {
    DO_TEST_F32A(test04);
    DO_TEST_F64A(test05);
  }
  else if (sizeof(ANALYZER_FP_FORMAT) == 8) {
    DO_TEST_F64A(test04);
    DO_TEST_F64(test05);
  }
  else {
    DO_TEST_F64(test04);
    DO_TEST_F64(test05);
  }
  DO_TEST(test06);
  DO_TEST(test07);
END_MAIN
