/* Test Box::constraints().
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
  TBox box1(0, EMPTY);

  Rational_Box known_result(box1);

  Constraint_System cs = box1.constraints();
  TBox box2(cs);

  bool ok = check_result(box2, known_result);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test02() {
  TBox box1(0, UNIVERSE);

  Rational_Box known_result(box1);

  Constraint_System cs = box1.constraints();
  TBox box2(cs);

  bool ok = check_result(box2, known_result);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box1(3);
  box1.add_constraint(A >= 0);
  box1.add_constraint(B >= 0);

  Rational_Box known_result(box1);

  box1.contains(box1);

  Constraint_System cs = box1.constraints();
  TBox box2(cs);

  bool ok = check_result(box2, known_result);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box1(3);
  box1.add_constraint(A >= 0);
  box1.add_constraint(B >= 0);

  Constraint_System cs = box1.constraints();
  TBox box2(cs);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(cs, "*** cs ***");

  Rational_Box known_result(box1);

  bool ok = check_result(box2, known_result);

  return ok;
}

bool
test05() {
  Variable A(0);

  Constraint_System cs(Linear_Expression(0) == -1);

  print_constraints(cs, "*** cs ***");

  TBox box(cs);

  Rational_Box known_result(0, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

bool
test06() {
  Variable A(0);

  Constraint_System cs(Linear_Expression(0) == -1);
  cs.insert(A >= 0);

  print_constraints(cs, "*** cs ***");

  TBox box(cs);

  Rational_Box known_result(1, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

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
END_MAIN
