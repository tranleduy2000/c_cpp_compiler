/* Test Octagonal_Shape::map_space_dimensions().
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

  TOctagonal_Shape oc1(3);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result;

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

return ok;
}

bool
test02() {
  Partial_Function function;

  TOctagonal_Shape oc1(3, EMPTY);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result(0, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

  return ok;
}

bool
test03() {
  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  TOctagonal_Shape oc1(3, EMPTY);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

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
  cs.insert(z - x <= 3);

  TOctagonal_Shape oc1(cs);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x == 1);
  known_result.add_constraint(y - x <= 3);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

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
  cs.insert(z - x <= 3);
  cs.insert(z - y <= 7);
  cs.insert(y - x <= 2);

  TOctagonal_Shape oc1(cs);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result(1);
  known_result.add_constraint(x <= 4);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

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
  cs.insert(z - x <= 1);
  cs.insert(z - y <= 7);
  cs.insert(y - x <= 1);

  TOctagonal_Shape oc1(cs);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y - x <= 7);
  known_result.add_constraint(x + y <= 4);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

  return ok;
}

bool
test07() {
  Partial_Function function;

  TOctagonal_Shape oc1;

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  oc1.map_space_dimensions(function);

  Octagonal_Shape<mpq_class> known_result;

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Partial_Function function;
  function.insert(2, 1);
  function.insert(1, 0);
  function.insert(0, 2);

  Constraint_System cs;
  cs.insert(x == 1);
  cs.insert(z - x <= 1);
  cs.insert(y - z <= -3);
  cs.insert(x - y <= 1);

  TOctagonal_Shape oc1(cs);

  print_function(function, "*** function ***");
  print_constraints(oc1, "*** oc1 ***");

  Octagonal_Shape<mpq_class> known_result(3, EMPTY);

  oc1.map_space_dimensions(function);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oc1.map_space_dimensions(function) ***");

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
END_MAIN
