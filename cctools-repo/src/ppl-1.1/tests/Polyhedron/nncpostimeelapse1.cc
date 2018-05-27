/* Test Polyhedron::positive_time_elapse_assign().
   Copyright (C)      2013 Marco Faella <marfaella@gmail.com>
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

// Positive time elapse between: the origin and an open half-plane.
bool
test01() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x == 0);
  ph1.add_constraint(y == 0);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(x>0);

  bool ok = (ph1 == known_result);

  return ok;
}

// Positive time elapse between: a square without a side and a half-strip.
bool
test02() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 1);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 1);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y < 0);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(x>0);
  known_result.add_constraint(y<1);

  bool ok = (ph1 == known_result);

  return ok;
}

// Positive time elapse between: a closed square and an unbounded C-polyhedron.
bool
test03() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x <= 1);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 1);

  C_Polyhedron ph2(2);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y <= 0);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(y <= 1);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  return ok;
}

// The same as test03, but with two NNC polyhedra.
bool
test04() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x <= 1);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 1);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y <= 0);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(y <= 1);

  bool ok = (ph1 == known_result);

  return ok;
}

// Positive time elapse between the universe and a point.
bool
test05() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x == 1);
  ph2.add_constraint(y == 1);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);

  bool ok = (ph1 == known_result);

  return ok;
}

// Positive time elapse between a point and the universe.
bool
test06() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x == 1);
  ph1.add_constraint(y == 1);

  NNC_Polyhedron ph2(2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);

  bool ok = (ph1 == known_result);
  return ok;
}

// Positive time elapse between two rational points with different
// denominators.
bool
test07() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(x+y,2));

  NNC_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point(x+y,3));

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.positive_time_elapse_assign(ph2);

  print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(2*x > 1);
  known_result.add_constraint(x == y);

  bool ok = (ph1 == known_result);
  return ok;
}


// Positive time elapse between Polyhedra with two different dimensions.
bool
test08() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(1);
  ph1.add_constraint(x > 0);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 0);
  ph2.add_constraint(y > 1);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  bool known_result = false;

  try
  {
    ph1.positive_time_elapse_assign(ph2);
    print_generators(ph1, "*** ph1_positive_time_elapse_assign(ph2) ***");
  }
  catch (std::invalid_argument& e)
  {
    known_result=true;
  }

  bool ok = (known_result == true);
  return ok;
}


// Positive time elapse between two zero-dimensional polyhedra where
// the second one is empty.
bool
test09() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(0);

  NNC_Polyhedron ph2(0, EMPTY);

  ph1.positive_time_elapse_assign(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(0, EMPTY);

  bool ok = (ph1 == known_result);
  return ok;
}

// Positive time elapse between a polyhedron and the empty polyhedron.
bool
test10() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(y > 1);

  NNC_Polyhedron ph2(2, EMPTY);

  ph1.positive_time_elapse_assign(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);
  return ok;
}

// Positive time elapse between the empty polyhedron and another polyhedron.
bool
test11() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, EMPTY);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 0);
  ph2.add_constraint(y > 1);

  ph1.positive_time_elapse_assign(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);
  return ok;
}

// Positive time elapse between an empty polyhedron with non-processed
// pending constraints and a normal polyhedron.
bool
test12() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x >= 255);
  ph1.add_constraint(x <= 257);
  ph1.add_constraint(y >= 255);
  ph1.add_constraint(y <= 257);

  Variables_Set vars(x, y);

  // Defines constraints of an empty polyhedron.
  Constraint_System cs;
  cs.insert(x > y);
  cs.insert(x < y);

  // Adding pending constraints.
  ph1.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs, 16, false);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 0);
  ph2.add_constraint(y > 1);

  ph1.positive_time_elapse_assign(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);
  return ok;
}

// Positive time elapse between a normal polyhedron and an empty
// polyhedron with non-processed pending constraints.
bool
test13() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(y > 1);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x >= 255);
  ph2.add_constraint(x <= 257);
  ph2.add_constraint(y >= 255);
  ph2.add_constraint(y <= 257);

  Variables_Set vars(x, y);

  // Defines constraints of an EMPTY Polyhedron
  Constraint_System cs;
  cs.insert(x > y);
  cs.insert(x < y);

  // Adding pending constraints
  ph2.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs, 16, true);

  ph1.positive_time_elapse_assign(ph2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);
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
