/* Test Box::refine_with_constraint(const Constraint&).
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

  TBox box(2);
  box.refine_with_constraint(A >= 0);

  print_constraints(box, "*** box.refine_with_constraint(A >= 0) ***");

  box.refine_with_constraint(B >= A);

  print_constraints(box, "*** box.refine_with_constraint(B >= A) ***");

  box.refine_with_constraint(11*A < 127);

  print_constraints(box, "*** box.refine_with_constraint(11*A < 127) ***");

  box.refine_with_constraint(7*A - 15*B > 8);

  print_constraints(box, "*** box.refine_with_constraint(7*A - 15*B > 8) ***");

  box.refine_with_constraint(3*B > 2*A);

  print_constraints(box, "*** box.refine_with_constraint(3*B > 2*A) ***");

  box.refine_with_constraint(A == B);

  print_constraints(box, "*** box.refine_with_constraint(A == B) ***");

  Rational_Box known_result(2);
  known_result.add_constraint(7*A > 8);
  known_result.add_constraint(55*A < 267);
  known_result.add_constraint(7*B > 8);
  known_result.add_constraint(55*B < 267);

  bool ok = check_result(box, known_result, "2.33e-6", "1.55e-6", "1.10e-6");

  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(B >= -2);
  box.add_constraint(B <= 4);

  print_constraints(box, "*** box ***");

  box.refine_with_constraint(A - 2*B >= 0);

  Rational_Box known_result(2);
  known_result.add_constraint(A >= -4);
  known_result.add_constraint(B >= -2);
  known_result.add_constraint(B <= 4);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.refine_with_constraint(A - 2*B >= 0) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test03() {
  Variable A(0);

  Constraint_System cs;

  TBox box(1);
  print_constraints(box, "*** box ***");

  Constraint c(A == 1);
  print_constraint(c, "*** c ***");

  box.refine_with_constraint(c);

  Rational_Box known_result(1);
  known_result.add_constraint(A == 1);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.refine_with_constraint(c) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test04() {
  Variable A(0);

  TBox box(1);
  print_constraints(box, "*** box ***");

  Constraint c(Linear_Expression(1) <= 0);
  print_constraint(c, "*** c ***");

  box.refine_with_constraint(c);

  Rational_Box known_result(1, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.refine_with_constraint(c) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
