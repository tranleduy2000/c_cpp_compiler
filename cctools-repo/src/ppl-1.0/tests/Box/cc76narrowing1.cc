/* Test Box::CC76_narrowing_assign().
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

  TBox box1(3);
  box1.add_constraint(x <= 1);
  box1.add_constraint(y <= 7);
  box1.add_constraint(-y <= 6);

  TBox box2(3);
  box2.add_constraint(-x <= 3);
  box2.add_constraint(y <= 2);
  box2.add_constraint(-y <= 5);
  box2.add_constraint(x <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.CC76_narrowing_assign(box1);

  Rational_Box known_result(3);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(-x <= 3);
  known_result.add_constraint(-y <= 6);
  known_result.add_constraint(y <= 7);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.CC76_narrowing_assign(box1) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(4);
  box1.add_constraint(z <= 1);

  TBox box2(4);
  box2.add_constraint(-y <= 3);
  box2.add_constraint(-x <= 2);
  box2.add_constraint(x <= 3);
  box2.add_constraint(z <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.CC76_narrowing_assign(box1);

  Rational_Box known_result(4);
  known_result.add_constraint(z <= 1);
  known_result.add_constraint(-y <= 3);
  known_result.add_constraint(-x <= 2);
  known_result.add_constraint(x <= 3);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.CC76_narrowing_assign(box1) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(3);
  TBox box2(3);

  box1.add_constraint(z <= 1);
  Rational_Box known_result(3, EMPTY);

  box2.add_constraint(-y <= 3);
  box2.add_constraint(x >= 4);
  box2.add_constraint(x <= 3);
  box2.add_constraint(z <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.CC76_narrowing_assign(box1);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.CC76_narrowing_assign(box1) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(3);
  box1.add_constraint(z <= 1);
  box1.add_constraint(z >= 3);

  TBox box2(3);
  box2.add_constraint(-y <= 3);
  box2.add_constraint(x >= 4);
  box2.add_constraint(x <= 3);
  box2.add_constraint(z <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.CC76_narrowing_assign(box1);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.CC76_narrowing_assign(box1) ***");

  return ok;
}

bool
test05() {
  TBox box1;
  TBox box2(0, EMPTY);
  Rational_Box known_result(box2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box2.CC76_narrowing_assign(box1);

  bool ok = check_result(box2, known_result);

  print_constraints(box2, "*** box2.CC76_narrowing_assign(box1) ***");

  return ok;
}

bool
test06() {
  Variable y(1);

  TBox box1(1);
  TBox box2(2);

  Constraint_System cs;
  cs.insert(y >= 6);

  try {
    // This is an invalid use of the method
    // Box::CC76_narrowing_assign(box2, cs): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    box2.CC76_narrowing_assign(box1);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
