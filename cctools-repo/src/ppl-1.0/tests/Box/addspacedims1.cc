/* Test Box::add_space_dimensions_and_embed():
   we add two variables to a Box.
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
  //Variable y(1);
  Variable z(2);

  TBox box(2);
  box.add_constraint(x <= 2);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_embed(2);
  box.add_constraint(z <= 2);

  Rational_Box known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(z <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.add_space_dimensions_and_embed(2) "
                    "and box.add_constraint(z <= 2) ***");

  return ok;
}

bool
test02() {
  TBox box1(0, EMPTY);
  TBox box2(1, EMPTY);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.add_space_dimensions_and_embed(2);
  box2.add_space_dimensions_and_embed(1);

  bool ok = (box1 == box2);

  print_constraints(box1, "*** box1.add_space_dimensions_and_embed(2) ***");
  print_constraints(box2, "*** box2.add_space_dimensions_and_embed(1) ***");

  return ok;
}

bool
test03() {
  TBox box(0, UNIVERSE);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_embed(3);

  Rational_Box known_result(3, UNIVERSE);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_space_dimension_and_embed(3) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(0);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_project(4);

  Rational_Box known_result(4);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C == 0);
  known_result.add_constraint(D == 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_space_dimensions_and_project(4) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);

  Rational_Box known_result(box);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_project(0);

  print_constraints(box, "*** box.add_space_dimensions_and_project(0) ***");

  bool ok = check_result(box, known_result);

  return ok;
}

bool
test06() {
  //Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(1);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_project(3);

  Rational_Box known_result(4);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C == 0);
  known_result.add_constraint(D == 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_space_dimensions_and_project(3) ***");

  return ok;
}

bool
test07() {
  TBox box(10, UNIVERSE);

  box.remove_higher_space_dimensions(5);
  box.add_space_dimensions_and_embed(6);

  return box == TBox(11, UNIVERSE);
}

bool
test08() {
  Variable x(0);
  //Variable y(1);
  Variable z(2);
  Variable w(3);

  TBox box(2);
  box.add_constraint(x <= 2);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_project(2);

  Rational_Box known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(w == 0);
  known_result.add_constraint(z == 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_space_dimensions_and_project(2) ***");

  return ok;
}

bool
test09() {
  TBox box1(0, EMPTY);
  TBox box2(1, EMPTY);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.add_space_dimensions_and_project(2);
  box2.add_space_dimensions_and_project(1);

  bool ok = (box1 == box2);

  print_constraints(box1, "*** box1.add_space_dimensions_and_project(2) ***");
  print_constraints(box2, "*** box2.add_space_dimensions_and_project(1) ***");

  return ok;
}

bool
test10() {
  TBox box(0, EMPTY);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_project(3);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_space_dimension_and_project(3) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(0);

  print_constraints(box, "*** box ***");

  box.add_space_dimensions_and_project(4);

  Rational_Box known_result(4);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C == 0);
  known_result.add_constraint(D == 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_space_dimensions_and_project(4) ***");

  return ok;
}

bool
test12() {
  Variable A(0);

  TBox box1(2);
  box1.add_constraint(A >= 0);
  box1.add_constraint(A <= 2);

  TBox box2(box1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.add_space_dimensions_and_project(0);
  box1.add_space_dimensions_and_embed(0);

  bool ok = (box1 == box2);

  print_constraints(box1, "*** box1.add_space_dimensions_and_project(0) ***");
  print_constraints(box2, "*** box2.add_space_dimensions_and_embed(0) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  //Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x <= 2);

  TBox box(cs);

  print_constraints(box, "*** box ***");

  box.minimized_constraints();
  box.add_space_dimensions_and_embed(3);
  box.add_constraint(z <= 2);

  Rational_Box known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(z <= 2);

  bool ok = check_result(box, known_result) ;

  print_constraints(box,
                    "*** box.add_space_dimensions_and_embed(2) "
                    "and box.add_constraint(z <= 2) ***");

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
END_MAIN
