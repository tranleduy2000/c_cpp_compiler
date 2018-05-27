/* Test Box::wrap_assign().
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
  TBox box(2);
  box.add_constraint(100 <= x);
  box.add_constraint(x <= 300);
  box.add_constraint(-50 <= y);
  box.add_constraint(y <= 50);

  print_constraints(box, "*** box ***");

  Variables_Set vars(x, y);

  box.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  // FIXME.
  TBox known_result(2);
  known_result.refine_with_constraint(0 <= x);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(0 <= y);
  known_result.refine_with_constraint(y <= 255);

  bool ok = (box == known_result);

  print_constraints(box, "*** box.wrap_assign(...) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  TBox box(2);
  box.add_constraint(100 <= x);
  box.add_constraint(x <= 300);
  box.add_constraint(-50 <= y);
  box.add_constraint(y <= 50);

  print_constraints(box, "*** box ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x >= y);
  cs.insert(y <= 75);

  box.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs);

  // FIXME.
  TBox known_result(2);
  known_result.refine_with_constraint(0 <= x);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(0 <= y);
  known_result.refine_with_constraint(y <= 50);

  bool ok = (box == known_result);

  print_constraints(box, "*** box.wrap_assign(...) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  TBox box(2);
  box.add_constraint(100 <= x);
  box.add_constraint(x <= 300);
  box.add_constraint(-50 <= y);
  box.add_constraint(y <= 50);

  print_constraints(box, "*** box ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x + y >= 50);
  cs.insert(x >= y);
  cs.insert(y <= 75);

  box.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs);

  TBox known_result(2);
  known_result.refine_with_constraint(0 <= x);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(0 <= y);
  known_result.refine_with_constraint(y <= 50);

  bool ok = (box == known_result);

  print_constraints(box, "*** box.wrap_assign(...) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  TBox box1(1);
  box1.add_constraint(2*x == 511);

  print_constraints(box1, "*** box ***");

  Variables_Set vars(x);

  // Making copies before affecting box1.
  TBox box2(box1);
  TBox box3(box1);

  TBox good_enough_result(box1);
  TBox precise_result(1, EMPTY);

  box1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  box2.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);
  box3.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);

  // FIXME: Implementation can be more precise than expected.
  bool ok = box1.contains(precise_result) && good_enough_result.contains(box1)
    && box2.contains(precise_result) && good_enough_result.contains(box2)
    && box3.contains(precise_result) && good_enough_result.contains(box3);

  print_constraints(box1, "*** box.wrap_assign(..., OVERFLOW_WRAPS) ***");
  print_constraints(box2, "*** box.wrap_assign(..., OVERFLOW_UNDEFINED) ***");
  print_constraints(box3, "*** box.wrap_assign(..., OVERFLOW_IMPOSSIBLE) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  TBox box1(1);
  box1.add_constraint(2*x == 18*256 + 511);

  print_constraints(box1, "*** box ***");

  Variables_Set vars(x);

  // Making copies before affecting box1.
  TBox box2(box1);
  TBox box3(box1);

  box1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);
  box2.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);
  box3.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);

  TBox known_result1(1, EMPTY);
  TBox known_result2(1);
  known_result2.add_constraint(x >= 0);
  known_result2.add_constraint(x <= 255);
  TBox known_result3(1, EMPTY);

  bool ok = (box1 == known_result1)
    && (box2 == known_result2)
    && (box3 == known_result3);

  print_constraints(box1, "*** box.wrap_assign(..., OVERFLOW_WRAPS) ***");
  print_constraints(box2, "*** box.wrap_assign(..., OVERFLOW_UNDEFINED) ***");
  print_constraints(box3, "*** box.wrap_assign(..., OVERFLOW_IMPOSSIBLE) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
  DO_TEST_F8(test03);
  DO_TEST_F8(test04);
  DO_TEST_F8(test05);
END_MAIN
