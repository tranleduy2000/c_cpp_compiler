/* Test BD_Shape::simplify_using_context_assign().
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

  TBD_Shape bds1(2);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(x <= 2);
  bds1.add_constraint(y >= 0);
  bds1.add_constraint(y <= 2);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(bds1);
  bds2.affine_image(x, x + 6);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result(2);
  known_result.add_constraint(x <= 2);

  bool ok = !bds1.simplify_using_context_assign(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1,
                    "*** bds1.simplify_using_context_assign(bds2) ***");
  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(x <= 2);
  bds1.add_constraint(y >= 0);
  bds1.add_constraint(y <= 2);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(bds1);
  bds2.affine_image(x, x + 1);
  bds2.affine_image(y, y + 6);

  print_constraints(bds2, "*** bds2 ***");

  // NOTE: this is the result computed when using the current heuristics.
  // It turns out that the current heuristics is not smart enough to see
  // that constraint y <= 2 is not really needed.
  TBD_Shape known_result(2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y <= 2);

  bool ok = !bds1.simplify_using_context_assign(bds2);
  ok &= (bds1 == known_result);

  print_constraints(bds1,
                    "*** bds1.simplify_using_context_assign(bds2) ***");
  return ok;
}

bool
test03() {
  Variable x(0);

  TBD_Shape bds1(1);
  bds1.add_constraint(x >= 0);
  bds1.add_constraint(x <= 6);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(1);
  bds2.add_constraint(x >= 0);
  bds2.add_constraint(x <= 5);

  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape computed_result = bds1;

  computed_result.simplify_using_context_assign(bds2);

  TBD_Shape known_result(1);

  bool ok = (computed_result == known_result);

  print_constraints(computed_result,
                    "*** bds1.simplify_using_context_assign ***");

  return ok;
}

bool
test04() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TBD_Shape bds1 = TBD_Shape(3, UNIVERSE);
  bds1.add_constraint(i >= 1);
  bds1.add_constraint(i <= 10);
  bds1.add_constraint(j >= 1);
  bds1.add_constraint(j <= 10);
  bds1.add_constraint(k == 0);

  TBD_Shape bds2 = TBD_Shape(3, UNIVERSE);
  bds2.add_constraint(i >= 0);
  bds2.add_constraint(i <= 2);
  bds2.add_constraint(j >= 2);
  bds2.add_constraint(j <= 9);
  bds2.add_constraint(k == 0);

  print_constraints(bds1, "=== bds1 ===");
  print_constraints(bds2, "=== bds2 ===");

  TBD_Shape known_result = TBD_Shape(3, UNIVERSE);
  known_result.add_constraint(i >= 1);

  bds1.simplify_using_context_assign(bds2);

  bool ok = (bds1 == known_result);

  print_constraints(bds1, "=== bds1.simplify_using_context_assign(bds2) ===");

  return ok;
}

bool
test05() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TBD_Shape bds1(3, UNIVERSE);
  bds1.add_constraint(i == 1);
  bds1.add_constraint(j + 1 == 0);
  bds1.add_constraint(k == 3);

  TBD_Shape bds2(3, UNIVERSE);
  bds2.add_constraint(i == 1);
  bds2.add_constraint(k >= 0);
  bds2.add_constraint(k <= 3);

  TBD_Shape known_result(3, UNIVERSE);
  known_result.add_constraint(k >= 3);
  known_result.add_constraint(j + 1 == 0);

  bds1.simplify_using_context_assign(bds2);

  bool ok = (bds1 == known_result);

  print_constraints(bds1.minimized_constraints(),
                    "=== bds1.simplify_using_context_assign(bds2) ===");
  print_constraints(known_result.minimized_constraints(),
                    "=== known_result ===");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3, UNIVERSE);
  bds1.add_constraint(A == 0);
  bds1.add_constraint(B == C);
  bds1.add_constraint(B >= 2);
  print_constraints(bds1, "\n=== bds1 ===");

  TBD_Shape bds2(3, UNIVERSE);
  bds2.add_constraint(A == 0);
  bds2.add_constraint(C >= 2);
  print_constraints(bds2, "\n=== bds2 ===");

  bds1.simplify_using_context_assign(bds2);

  TBD_Shape known_result(3, UNIVERSE);
  known_result.add_constraint(B == C);

  bool ok = (bds1 == known_result);

  print_constraints(bds1, "\n=== bds1.simplify_using_context_assign(bds2) ===");

  return ok;
}

bool
test07() {
  TBD_Shape bds1(0, EMPTY);
  TBD_Shape bds2;

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result = bds1;

  bds1.simplify_using_context_assign(bds2);

  bool ok = (bds1 == known_result);

  print_constraints(bds1,
                    "*** bds1.simplify_using_context_assign(bds2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);

  TBD_Shape bds1(1);
  TBD_Shape bds2(1);

  bds2.add_constraint(A == 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  TBD_Shape known_result = bds1;

  bds1.simplify_using_context_assign(bds2);

  bool ok = (bds1 == known_result);

  print_constraints(bds1,
                    "*** bds1.simplify_using_context_assign(bds2) ***");

  return ok;
}

bool
test09() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  TBD_Shape bds1(3, UNIVERSE);
  bds1.add_constraint(i >= 1);
  bds1.add_constraint(i <= 10);
  bds1.add_constraint(j >= 1);
  bds1.add_constraint(j <= 10);
  bds1.add_constraint(k == 0);

  TBD_Shape bds2(3, UNIVERSE);
  bds2.add_constraint(i <= 25);
  bds2.add_constraint(j <= 25);
  bds2.add_constraint(k == 0);

  TBD_Shape known_result(3, UNIVERSE);
  known_result.add_constraint(i >= 1);
  known_result.add_constraint(i <= 10);
  known_result.add_constraint(j >= 1);
  known_result.add_constraint(j <= 10);

  bds1.simplify_using_context_assign(bds2);

  bool ok = (bds1 == known_result);

  print_constraints(bds1);

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
END_MAIN
