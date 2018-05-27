/* Test Octagonal_Shape::simplify_using_context_assign().
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

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(x <= 2);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(oct1);
  oct2.affine_image(x, x + 6);

  print_constraints(oct2, "*** oct2 ***");

  TOctagonal_Shape known_result(2);
  known_result.add_constraint(x <= 2);

  bool ok = !oct1.simplify_using_context_assign(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.simplify_using_context_assign(oct2) ***");
  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(x <= 2);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(oct1);
  oct2.affine_image(x, x + 1);
  oct2.affine_image(y, y + 6);

  print_constraints(oct2, "*** oct2 ***");

  // NOTE: this is the result computed when using the current heuristics.
  // It turns out that the current heuristics is not smart enough to see
  // that constraint y <= 2 is not really needed.
  TOctagonal_Shape known_result(2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y <= 2);

  bool ok = !oct1.simplify_using_context_assign(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.simplify_using_context_assign(oct2) ***");
  return ok;
}

bool
test03() {
  Variable x(0);

  TOctagonal_Shape oct1(1);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(x <= 6);

  print_constraints(oct1, "*** oct1 ***");

  TOctagonal_Shape oct2(1);
  oct2.add_constraint(x >= 0);
  oct2.add_constraint(x <= 5);

  print_constraints(oct2, "*** oct2 ***");

  TOctagonal_Shape computed_result = oct1;

  computed_result.simplify_using_context_assign(oct2);

  TOctagonal_Shape known_result(1);

  bool ok = (computed_result == known_result);

  print_constraints(computed_result,
                    "*** oct1.simplify_using_context_assign ***");

  return ok;
}

bool
test04() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TOctagonal_Shape oct1 = TOctagonal_Shape(3, UNIVERSE);
  oct1.add_constraint(i >= 1);
  oct1.add_constraint(i <= 10);
  oct1.add_constraint(j >= 1);
  oct1.add_constraint(j <= 10);
  oct1.add_constraint(k == 0);

  TOctagonal_Shape oct2 = TOctagonal_Shape(3, UNIVERSE);
  oct2.add_constraint(i >= 0);
  oct2.add_constraint(i <= 2);
  oct2.add_constraint(j >= 2);
  oct2.add_constraint(j <= 9);
  oct2.add_constraint(k == 0);

  print_constraints(oct1, "=== oct1 ===");
  print_constraints(oct2, "=== oct2 ===");

  TOctagonal_Shape known_result = TOctagonal_Shape(3, UNIVERSE);
  known_result.add_constraint(i >= 1);

  oct1.simplify_using_context_assign(oct2);

  bool ok = (oct1 == known_result);

  print_constraints(oct1, "=== oct1.simplify_using_context_assign(oct2) ===");

  return ok;
}

bool
test05() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TOctagonal_Shape oct1(3, UNIVERSE);
  oct1.add_constraint(i == 1);
  oct1.add_constraint(j + 1 == 0);
  oct1.add_constraint(k == 3);

  TOctagonal_Shape oct2(3, UNIVERSE);
  oct2.add_constraint(i == 1);
  oct2.add_constraint(j + k == 2);
  oct2.add_constraint(k >= 0);
  oct2.add_constraint(k <= 3);

  TOctagonal_Shape known_result(3, UNIVERSE);
  known_result.add_constraint(j + 1 <= 0);
  // PolyLib 5.22.3 returns { j + 1 == 0, k == 3 };
  // Using PPL::Polyhedron we get { k == 3 }.

  oct1.simplify_using_context_assign(oct2);

  bool ok = (oct1 == known_result);

  print_constraints(oct1.minimized_constraints(),
                    "=== oct1.simplify_using_context_assign(oct2) ===");
  print_constraints(known_result.minimized_constraints(),
                    "=== known_result ===");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct1(3, UNIVERSE);
  oct1.add_constraint(A == 0);
  oct1.add_constraint(B == C);
  oct1.add_constraint(B >= 2);
  print_constraints(oct1, "\n=== oct1 ===");

  TOctagonal_Shape oct2(3, UNIVERSE);
  oct2.add_constraint(A == 0);
  oct2.add_constraint(C >= 2);
  print_constraints(oct2, "\n=== oct2 ===");

  oct1.simplify_using_context_assign(oct2);

  TOctagonal_Shape known_result(3, UNIVERSE);
  known_result.add_constraint(B == C);

  bool ok = (oct1 == known_result);

  print_constraints(oct1, "\n=== oct1.simplify_using_context_assign(oct2) ===");

  return ok;
}

bool
test07() {
  TOctagonal_Shape oct1(0, EMPTY);
  TOctagonal_Shape oct2;

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  TOctagonal_Shape known_result = oct1;

  oct1.simplify_using_context_assign(oct2);

  bool ok = (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.simplify_using_context_assign(oct2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);

  TOctagonal_Shape oct1(1);
  TOctagonal_Shape oct2(1);

  oct2.add_constraint(A == 0);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  TOctagonal_Shape known_result = oct1;

  oct1.simplify_using_context_assign(oct2);

  bool ok = (oct1 == known_result);

  print_constraints(oct1.minimized_constraints(),
                    "*** oct1.simplify_using_context_assign(oct2) ***");

  return ok;
}

bool
test09() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TOctagonal_Shape oct1(3, UNIVERSE);
  oct1.add_constraint(i >= 1);
  oct1.add_constraint(i <= 10);
  oct1.add_constraint(j >= 1);
  oct1.add_constraint(j <= 10);
  oct1.add_constraint(k == 0);

  TOctagonal_Shape oct2(3, UNIVERSE);
  oct2.add_constraint(i <= 25);
  oct2.add_constraint(j <= 25);
  oct2.add_constraint(i + j >= 25);
  oct2.add_constraint(k == 0);

  TOctagonal_Shape known_result(3, UNIVERSE);
  known_result.add_constraint(i >= 1);
  known_result.add_constraint(i <= 10);
  known_result.add_constraint(j <= 10);

  oct1.simplify_using_context_assign(oct2);

  bool ok = (oct1 == known_result);

  print_constraints(oct1.minimized_constraints(),
                    "*** oct1.simplify_using_context_assign(oct2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A == 0);
  oct1.add_constraint(B == 0);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A >= 0);
  oct2.add_constraint(B >= 0);

  TOctagonal_Shape known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B <= 0);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  bool ok = oct1.simplify_using_context_assign(oct2);

  ok = ok && (oct1 == known_result);

  print_constraints(oct1.minimized_constraints(),
                    "*** oct1.simplify_using_context_assign(oct2) ***");

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
