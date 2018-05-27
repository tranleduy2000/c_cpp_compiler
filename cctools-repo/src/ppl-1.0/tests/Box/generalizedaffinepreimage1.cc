/*Test Box::generalized_affine_preimage().
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
#include <limits>

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(2*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, GREATER_OR_EQUAL,
                                  Linear_Expression(-1));
  ph.generalized_affine_preimage(B, GREATER_OR_EQUAL,
                                 Linear_Expression(-1));

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, -1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, -1) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(2*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, Linear_Expression(-1));
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, Linear_Expression(-1));

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(4*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -B+1);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -B+1);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -B+1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -B+1) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(4*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, B+1);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, B+1);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, B+1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, B+1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(2*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, 2*B+1);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, 2*B+1);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, 2*B+1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, 2*B+1) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(5*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  NNC_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -2*B+1);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -2*B+1);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -2*B+1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -2*B+1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(5*A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  NNC_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, 3*A-2*B+1);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, 3*A-2*B+1);

  bool ok = check_result(box, Rational_Box(ph),
                         "6.71e-8", "6.71e-8", "6.71e-8");

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, 3*A-2*B+1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A == 1);
  box.add_constraint(B >= 5);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B+1);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B+1);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B+1) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B+1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(B >= 0);
  box.add_constraint(2*C <= 1);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B+7*C+1, 2);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B+7*C+1, 2);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B+7*C+1, 2) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B+7*C+1, 2) ***");

  return ok;
}

bool
test10() {
  // If the Coefficient type is not wide enough, do nothing.
  if (std::numeric_limits<Coefficient>::is_bounded
      && (std::numeric_limits<Coefficient>::min() > -203
          || std::numeric_limits<Coefficient>::max() < 629))
    return true;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(B >= 7);
  box.add_constraint(C >= 7);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B-7*C+1, 3);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B-7*C+1, 3);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B-7*C+1, 3) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B-7*C+1, 3) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(4*A == 1);
  box.add_constraint(2*B >= 7);
  box.add_constraint(5*C <= 7);

  print_constraints(box, "*** box ***");
  NNC_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B+7*C+1, -2);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B+7*C+1, -2);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B+7*C+1, -2) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B+7*C+1, -2) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(B >= 7);
  box.add_constraint(C >= 0);
  box.add_constraint(C <= 1);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B-7*C+1, -2);
  ph.generalized_affine_preimage(B, LESS_OR_EQUAL, -3*A-2*B-7*C+1, -2);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B-7*C+1, -3) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "LESS_OR_EQUAL, -3*A-2*B-7*C+1, -3) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(B <= 4);
  box.add_constraint(C <= 2);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, GREATER_OR_EQUAL, -3*A-2*B+C+1, -2);
  ph.generalized_affine_preimage(B, GREATER_OR_EQUAL, -3*A-2*B+C+1, -2);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, -3*A-2*B+C+1, -2) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, -3*A-2*B+C+1, -2) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(2*B <= 3);
  box.add_constraint(C <= 9);
  box.add_constraint(C >= 7);

  print_constraints(box, "*** box ***");
  C_Polyhedron ph(box);

  box.generalized_affine_preimage(B, GREATER_OR_EQUAL, 3*A-2*B-C+1, -3);
  ph.generalized_affine_preimage(B, GREATER_OR_EQUAL, 3*A-2*B-C+1, -3);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, 3*A-2*B-C+1, -3) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, 3*A-2*B-C+1, -3) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(2*B >= 7);
  box.add_constraint(C <= 7);
  box.add_constraint(5*C >= 7);

  print_constraints(box, "*** box ***");
  NNC_Polyhedron ph(box);

  box.generalized_affine_preimage(B, GREATER_OR_EQUAL, 3*A-2*B-7*C+1, -3);
  ph.generalized_affine_preimage(B, GREATER_OR_EQUAL, 3*A-2*B-7*C+1, -3);

  bool ok = check_result(box, Rational_Box(ph));

  print_constraints(box,
                    "*** box.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, 3*A-2*B-7*C+1, -3) ***");
  print_constraints(ph,
                    "*** ph.generalized_affine_preimage(B, "
                    "GREATER_OR_EQUAL, 3*A-2*B-7*C+1, -3) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
DO_TEST(test01);
DO_TEST(test02);
DO_TEST(test03);
DO_TEST(test04);
DO_TEST(test05);
DO_TEST_MAY_OVERFLOW_IF_INEXACT(test06, TBox);
DO_TEST_F8A_MAY_OVERFLOW_IF_INEXACT(test07, TBox);
DO_TEST(test08);
DO_TEST(test09);
DO_TEST(test10);
DO_TEST_F8_MAY_OVERFLOW_IF_INEXACT(test11, TBox);
DO_TEST(test12);
DO_TEST(test13);
DO_TEST(test14);
DO_TEST_F8_MAY_OVERFLOW_IF_INEXACT(test15, TBox);
END_MAIN
