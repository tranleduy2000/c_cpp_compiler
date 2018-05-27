/* Test Box::operator==().
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
  // Variable z(2);

  TBox box1(3);
  TBox box2(2);

  box1.add_constraint(x == 3);

  box2.add_constraint(x == 3);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  bool known_result = false;
  bool ok = ((box1 == box2) == known_result);

  return ok;
}

bool
test02() {
  TBox box1(0, EMPTY);
  TBox box2(0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  bool known_result = false;
  bool ok = ((box1 == box2) == known_result);

  return ok;
}

bool
test03() {
  TBox box1(0);
  TBox box2(0, EMPTY);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  bool known_result = false;
  bool ok = ((box1 == box2) == known_result);

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A >= 7);
  box1.add_constraint(B <= -2);
  box1.add_constraint(A >= 9);

  TBox box2(2);
  box2.add_constraint(A >= 9);
  box2.add_constraint(B <= -2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  bool known_result = true;
  bool ok = ((box1 == box2) == known_result);

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A <= 1);
  box1.add_constraint(A >= 7);

  TBox box2(2);
  box2.add_constraint(B <= 5);
  box2.add_constraint(B >= 10);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  bool known_result = true;
  bool ok = ((box1 == box2) == known_result);

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
