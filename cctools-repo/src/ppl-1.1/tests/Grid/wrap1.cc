/* Test Grid::wrap_assign().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
  Grid gr(2);
  gr.add_congruence((x + 24 %= 8*y) / 2);
  gr.add_congruence((y %= 1) / 3);

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  Grid known_result(2);
  known_result.add_congruence((x + 24 %= 8*y) / 2);
  known_result.add_congruence((y %= 0) / 1);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr(3);
  gr.add_congruence(x + 24 %= 8*y);
  gr.add_congruence((y %= 1) / 2);

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);

  Grid known_result(3);
  known_result.add_congruence(x %= 0);
  known_result.add_congruence(y %= 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Grid gr(2);
  gr.add_congruence((x + 24 %= 8*y) / 255);
  gr.add_congruence(x %= 0);

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);

  Grid known_result(2);
  known_result.add_congruence(x %= 0);
  known_result.add_congruence((32*x + 3 %= y) / 255);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(4);
  gr.add_congruence((x %= 255) / 0);

  Variables_Set vars(x, w);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  Grid known_result(4);
  known_result.add_congruence((x %= 255) / 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr1(4);
  gr1.add_congruence((x %= 10) / 255);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_IMPOSSIBLE);

  Grid known_result(4);
  known_result.add_congruence((x %= 10) / 0);

  bool ok = (gr1 == known_result && gr2 == known_result);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr1(4);
  gr1.add_congruence((x %= 245) / 255);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_IMPOSSIBLE);

  Grid known_result1(4);
  known_result1.add_congruence((x %= 245) / 0);
  Grid known_result2(4);
  known_result2.add_congruence((x %= -10) / 0);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr(3);
  gr.add_congruence(x + 24 %= 8*y);
  gr.add_congruence((y %= 1) / 0);

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);

  Grid known_result(3);
  known_result.add_congruence(x %= 0);
  known_result.add_congruence((y %= 1) / 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

// Expression with a greater space dimension than the grid.
bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(3);
  gr.add_congruence((z == -2) / 0);
  gr.add_congruence((x ==  0) / 0);

  Variables_Set vars(x, w);

  try {
    gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Constraint with a greater space dimension than the grid.
bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(3);
  gr.add_congruence((z == -2) / 0);
  gr.add_congruence((x ==  0) / 0);

  Constraint_System pcs;
  pcs.insert(x+y+z+w == 2);

  Variables_Set vars(x, y);

  try {
    gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED, &pcs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test10() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(4);
  gr.add_congruence((x %= 1) / 2);

  Constraint_System pcs;
  pcs.insert(x+y+z+w == 2);

  Variables_Set vars(x, w);

  // The constraint system pcs will be ignored.
  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &pcs);

  Grid known_result(4);
  known_result.add_congruence((x %= 1) / 2);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);
  Grid gr(2);
  gr.add_congruence((x %= 256) / 0);
  gr.add_congruence(y %= 0);

  Variables_Set vars(x);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);

  Grid known_result(2);
  known_result.add_congruence((x %= 0) / 1);
  known_result.add_congruence(y %= 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);
  Grid gr(2);
  gr.add_congruence((x %= 256) / 0);
  gr.add_congruence(y %= 0);

  Variables_Set vars(x);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  Grid known_result(2);
  known_result.add_congruence((x %= 0) / 0);
  known_result.add_congruence(y %= 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);
  Grid gr(2);
  gr.add_congruence((x %= 25) / 0);
  gr.add_congruence(y %= 0);

  Variables_Set vars(x);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);

  Grid known_result(2);
  known_result.add_congruence((x %= 25) / 0);
  known_result.add_congruence(y %= 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test14() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 245) / 255);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_WRAPS);

  Grid known_result1(2);
  known_result1.add_congruence((x %= 0) / 1);
  Grid known_result2(2);
  known_result2.add_congruence((x %= 0) / 1);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test15() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((x %= 245) / 256);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_WRAPS);

  Grid known_result1(2);
  known_result1.add_congruence((x %= 245) / 0);
  Grid known_result2(2);
  known_result2.add_congruence((x %= -11) / 0);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}


bool
test16() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((2*x %= 245) / 0);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_WRAPS);

  Grid known_result1(2, EMPTY);
  Grid known_result2(2, EMPTY);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test17() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((4*x %= 3) / 2);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_WRAPS);

  Grid known_result1(2, EMPTY);
  Grid known_result2(2, EMPTY);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test18() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((4*x %= 2) / 1);
  Grid gr2(gr1);

  Variables_Set vars(x);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_WRAPS);

  Grid known_result1(2);
  known_result1.add_congruence((x %= 0) / 1);
  Grid known_result2(2);
  known_result2.add_congruence((x %= 0) / 1);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test19() {
  Variable x(0);
  Variable y(1);

  Grid gr1(2);
  gr1.add_congruence((2*x %= 245) / 0);
  Grid gr2(gr1);

  Variables_Set vars(x, y);

  gr1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);
  gr2.wrap_assign(vars, BITS_8, SIGNED_2_COMPLEMENT, OVERFLOW_UNDEFINED);

  Grid known_result1(2, EMPTY);
  Grid known_result2(2, EMPTY);

  bool ok = (gr1 == known_result1 && gr2 == known_result2);

  print_congruences(gr1, "*** gr1.wrap_assign(...) ***");
  print_congruences(gr2, "*** gr2.wrap_assign(...) ***");

  return ok;
}

bool
test20() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(4);
  gr.add_congruence((x %= 1) / 2);

  Constraint_System pcs;
  pcs.insert(x+y == 2);

  Variables_Set vars(x, w);

  // The constraint system pcs will be ignored.
  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &pcs);

  Grid known_result(4);
  known_result.add_congruence((x %= 1) / 2);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
  DO_TEST_F8(test03);
  DO_TEST_F8(test04);
  DO_TEST_F8(test05);
  DO_TEST_F8(test06);
  DO_TEST_F8(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST_F8(test10);
  DO_TEST_F8(test11);
  DO_TEST_F8(test12);
  DO_TEST_F8(test13);
  DO_TEST_F16(test14);
  DO_TEST_F8(test15);
  DO_TEST_F8(test16);
  DO_TEST_F8(test17);
  DO_TEST_F8(test18);
  DO_TEST_F8(test19);
  DO_TEST_F8(test20);
END_MAIN
