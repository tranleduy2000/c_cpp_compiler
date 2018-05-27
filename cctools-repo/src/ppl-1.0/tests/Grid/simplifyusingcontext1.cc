/* Test Grid::simplify_using_context_assign().
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

  Grid gr1(2);
  gr1.add_congruence((x %= 0) / 2);
  gr1.add_congruence((y %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(gr1);
  gr2.affine_image(x, x + 6);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2, UNIVERSE);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 0) / 2);
  gr1.add_congruence((y %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(gr1);
  gr2.affine_image(x, x + 1);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2, UNIVERSE);
  known_result.add_congruence((x %= 0) / 2);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 0) / 2);
  gr1.add_congruence((y %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, UNIVERSE);
  gr2.add_congruence((x %= 0) / 4);
  gr2.add_congruence((y %= 0) / 1);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2, UNIVERSE);
  known_result.add_congruence((y %= 0) / 2);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 0) / 0);
  gr1.add_congruence((y %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, UNIVERSE);
  gr2.add_congruence((x %= 0) / 4);
  gr2.add_congruence((y %= 0) / 0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2, UNIVERSE);
  known_result.add_congruence((x %= 0) / 0);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x + y %= 0) / 0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, UNIVERSE);
  gr2.add_congruence((x %= 0) / 0);
  gr2.add_congruence((y %= 0) / 0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2, UNIVERSE);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 0) / 0);
  gr1.add_congruence((y %= 0) / 0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((x + y %= 0) / 0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result1(2);
  known_result1.add_congruence((x %= 0) / 0);
  Grid known_result2(2);
  known_result2.add_congruence((y %= 0) / 0);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result1 || gr1 == known_result2);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 0) / 1);
  gr1.add_congruence((y %= 0) / 1);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((x + y %= 0) / 1);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result1(2);
  known_result1.add_congruence((x %= 0) / 1);
  Grid known_result2(2);
  known_result2.add_congruence((y %= 0) / 1);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result1 || gr1 == known_result2);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((x + y %= 0) / 0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2);
  known_result.add_congruence((x + y %= 1) / 0);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((x + y %= 0) / 1);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2);
  known_result.add_congruence((2*x + 2*y %= 1) / 0);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((x + y %= 0) / 2);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2);
  known_result.add_congruence((x + y %= 1) / 0);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x + y %= 0) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2, EMPTY);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(2);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr1(3);
  gr1.add_congruence((x + 2*z %= 2) / 4);
  gr1.add_congruence((y + z %= 2) / 4);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_congruence((z %= 1) / 4);
  gr2.add_congruence((y %= 1) / 4);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(3);
  known_result.add_congruence((2*y - x %= 2) / 4);

  Grid gr3(gr1);
  gr3.intersection_assign(gr2);
  print_congruences(gr3,
                    "*** gr3.intersection_assign(gr2) ***");
  Grid gr4(gr1);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");

  gr4.intersection_assign(gr2);
  print_congruences(gr4,
                    "*** gr4.intersection_assign(gr2) ***");
  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr1(3);
  gr1.add_congruence((x - 2*y %= 2) / 4);
  gr1.add_congruence((x + 2*z %= 1) / 2);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(3);
  gr2.add_congruence((2*z %= 1) / 2);
  gr2.add_congruence((2*y %= 1) / 2);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(gr1);

  Grid gr3(gr1);
  gr3.intersection_assign(gr2);
  print_congruences(gr3,
                    "*** gr3.intersection_assign(gr2) ***");
  Grid gr4(gr1);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");

  gr4.intersection_assign(gr2);
  print_congruences(gr4,
                    "*** gr4.intersection_assign(gr2) ***");
  return ok;
}

bool
test14() {

  Grid gr1(0, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0, EMPTY);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(0, UNIVERSE);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                   "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test15() {

  Grid gr1(0, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(0, EMPTY);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test16() {

  Grid gr1(0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0, EMPTY);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(0);

  bool ok = !gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
  return ok;
}

bool
test17() {

  Grid gr1(0);

  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(0);

  print_congruences(gr2, "*** gr2 ***");

  Grid known_result(0);

  bool ok = gr1.simplify_using_context_assign(gr2);
  ok &= (gr1 == known_result);

  print_congruences(gr1,
                    "*** gr1.simplify_using_context_assign(gr2) ***");
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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
END_MAIN
