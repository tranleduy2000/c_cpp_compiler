/* Test Box::simplify_using_context_assign().
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
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 2);
  box1.add_constraint(y >= 0);
  box1.add_constraint(y <= 2);

  print_constraints(box1, "*** box1 ***");

  TBox box2(box1);
  box2.affine_image(x, x + 6);

  print_constraints(box2, "*** box2 ***");

  TBox known_result(2);
  known_result.add_constraint(x <= 2);

  bool ok = !box1.simplify_using_context_assign(box2);
  ok &= (box1 == known_result);

  print_constraints(box1,
                    "*** box1.simplify_using_context_assign(box2) ***");
  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 2);
  box1.add_constraint(y >= 0);
  box1.add_constraint(y <= 2);

  print_constraints(box1, "*** box1 ***");

  TBox box2(box1);
  box2.affine_image(x, x + 1);
  box2.affine_image(y, y + 6);

  print_constraints(box2, "*** box2 ***");

  // NOTE: this is the result computed when using the current heuristics.
  // It turns out that the current heuristics is not smart enough to see
  // that constraint y <= 2 is not really needed.
  TBox known_result(2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y <= 2);

  bool ok = !box1.simplify_using_context_assign(box2);
  ok &= (box1 == known_result);

  print_constraints(box1,
                    "*** box1.simplify_using_context_assign(box2) ***");
  return ok;
}

bool
test03() {
  Variable x(0);

  TBox box1(1);
  box1.add_constraint(x >= 0);
  box1.add_constraint(x <= 6);

  print_constraints(box1, "*** box1 ***");

  TBox box2(1);
  box2.add_constraint(x >= 0);
  box2.add_constraint(x <= 5);

  print_constraints(box2, "*** box2 ***");

  TBox computed_result = box1;

  computed_result.simplify_using_context_assign(box2);

  TBox known_result(1);

  bool ok = (computed_result == known_result);

  print_constraints(computed_result,
                    "*** box1.simplify_using_context_assign ***");

  return ok;
}

bool
test04() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TBox box1 = TBox(3, UNIVERSE);
  box1.add_constraint(i >= 1);
  box1.add_constraint(i <= 10);
  box1.add_constraint(j >= 1);
  box1.add_constraint(j <= 10);
  box1.add_constraint(k == 0);

  TBox box2 = TBox(3, UNIVERSE);
  box2.add_constraint(i >= 0);
  box2.add_constraint(i <= 2);
  box2.add_constraint(j >= 2);
  box2.add_constraint(j <= 9);
  box2.add_constraint(k == 0);

  print_constraints(box1, "=== box1 ===");
  print_constraints(box2, "=== box2 ===");

  TBox known_result = TBox(3, UNIVERSE);
  known_result.add_constraint(i >= 1);

  box1.simplify_using_context_assign(box2);

  bool ok = (box1 == known_result);

  print_constraints(box1, "=== box1.simplify_using_context_assign(box2) ===");

  return ok;
}

bool
test05() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TBox box1(3, UNIVERSE);
  box1.add_constraint(i == 1);
  box1.add_constraint(j + 1 == 0);
  box1.add_constraint(k == 3);

  TBox box2(3, UNIVERSE);
  box2.add_constraint(i == 1);
  box2.add_constraint(k >= 0);
  box2.add_constraint(k <= 3);

  TBox known_result(3, UNIVERSE);
  known_result.add_constraint(k >= 3);
  known_result.add_constraint(j + 1 == 0);

  box1.simplify_using_context_assign(box2);

  bool ok = (box1 == known_result);

  print_constraints(box1.minimized_constraints(),
                    "=== box1.simplify_using_context_assign(box2) ===");
  print_constraints(known_result.minimized_constraints(),
                    "=== known_result ===");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box1(3, UNIVERSE);
  box1.add_constraint(A == 0);
  box1.add_constraint(B >= 2);
  box1.add_constraint(C >= 2);
  print_constraints(box1, "\n=== box1 ===");

  TBox box2(3, UNIVERSE);
  box2.add_constraint(A == 0);
  box2.add_constraint(C >= 2);
  print_constraints(box2, "\n=== box2 ===");

  box1.simplify_using_context_assign(box2);

  TBox known_result(3, UNIVERSE);
  known_result.add_constraint(B >= 2);

  bool ok = (box1 == known_result);

  print_constraints(box1, "\n=== box1.simplify_using_context_assign(box2) ===");

  return ok;
}

bool
test07() {
  TBox box1(0, EMPTY);
  TBox box2;

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  TBox known_result = box1;

  box1.simplify_using_context_assign(box2);

  bool ok = (box1 == known_result);

  print_constraints(box1,
                    "*** box1.simplify_using_context_assign(box2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);

  TBox box1(1);
  TBox box2(1);

  box2.add_constraint(A == 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  TBox known_result = box1;

  box1.simplify_using_context_assign(box2);

  bool ok = (box1 == known_result);

  print_constraints(box1,
                    "*** box1.simplify_using_context_assign(box2) ***");

  return ok;
}

bool
test09() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TBox box1(3, UNIVERSE);
  box1.add_constraint(i >= 1);
  box1.add_constraint(i <= 10);
  box1.add_constraint(j >= 1);
  box1.add_constraint(j <= 10);
  box1.add_constraint(k == 0);

  TBox box2(3, UNIVERSE);
  box2.add_constraint(i <= 25);
  box2.add_constraint(j <= 25);
  box2.add_constraint(k == 0);

  TBox known_result(3, UNIVERSE);
  known_result.add_constraint(i >= 1);
  known_result.add_constraint(i <= 10);
  known_result.add_constraint(j >= 1);
  known_result.add_constraint(j <= 10);

  box1.simplify_using_context_assign(box2);

  bool ok = (box1 == known_result);

  print_constraints(box1);

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F(test01);
  DO_TEST_F(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
