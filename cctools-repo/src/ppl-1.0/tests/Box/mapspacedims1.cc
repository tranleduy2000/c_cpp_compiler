/* Test Box::map_space_dimensions().
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
  Partial_Function function;

  TBox box(3);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  box.map_space_dimensions(function);

  Rational_Box known_result;

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test02() {
  Partial_Function function;

  TBox box(3, EMPTY);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(0, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test03() {
  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  TBox box(3, EMPTY);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(2, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Partial_Function function;
  function.insert(0, 0);
  function.insert(2, 1);

  Constraint_System cs;
  cs.insert(x == 1);

  TBox box(3);
  box.add_constraints(cs);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  cs = box.minimized_constraints();
  box.map_space_dimensions(function);

  Rational_Box known_result(2);
  known_result.add_constraint(x == 1);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Partial_Function function;
  function.insert(2, 0);

  Constraint_System cs;
  cs.insert(x == 1);

  TBox box(3);
  box.add_constraints(cs);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(1);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Partial_Function function;
  function.insert(2, 1);
  function.insert(1, 0);

  Constraint_System cs;
  cs.insert(x == 1);

  TBox box(3);
  box.add_constraints(cs);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(2);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  TBox box(2);
  box.add_constraint(x <= 1);
  box.add_constraint(y <= -1);

  print_constraints(box, "*** box ***");
  print_function(function, "*** function ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= -1);
  known_result.add_constraint(y <= 1);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimension(function) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B >= 0);
  cs.insert(C >= 0);
  cs.insert(D == 0);
  TBox box(cs);

  Partial_Function function;
  function.insert(0, 2);
  function.insert(1, 1);
  function.insert(3, 0);

  print_function(function, "*** function ***");
  print_constraints(box, "*** box ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(3);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimensions(function) ***");

  return ok;
}

bool
test09() {
  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  TBox box(0);

  print_constraints(box, "*** box ***");
  print_function(function, "*** function ***");

  box.map_space_dimensions(function);

  Rational_Box known_result(0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.map_space_dimension(function) ***");

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
