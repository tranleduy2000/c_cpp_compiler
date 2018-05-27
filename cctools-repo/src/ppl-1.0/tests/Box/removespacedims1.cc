/* Remove some variables from the space.
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
  // Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  Variable x6(5);

  TBox box(6);
  box.add_constraint(x3 <= 2);
  box.add_constraint(x5 <= 3);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(x3);
  to_be_removed.insert(x5);

  box.remove_space_dimensions(to_be_removed);

  Rational_Box known_result(4);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_space_dimensions({x3, x5}) ***");

  return ok;
}

bool
test02() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  Variable x6(5);
  Variable x7(6);
  Variable x8(7);

  TBox box(8);
  box.add_constraint(x1 <= 2);
  box.add_constraint(x5 <= 7);
  box.add_constraint(x2 <= 10);

  print_constraints(box, "*** box ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(x1);
  to_be_removed.insert(x2);
  to_be_removed.insert(x3);
  to_be_removed.insert(x4);
  to_be_removed.insert(x5);
  to_be_removed.insert(x6);
  to_be_removed.insert(x7);
  to_be_removed.insert(x8);

  box.remove_space_dimensions(to_be_removed);

  Rational_Box known_result(0);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.remove_space_dimensions"
                    "({x1, x2, x3, x4, x5, x6, x7, x8}) ***");

  return ok;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);

  TBox box(4);
  box.add_constraint(x2 >= 5);
  box.add_constraint(x4 >= 3);
  box.add_constraint(x4 <= 0);

  print_constraints(box, "*** box ***");

  Variables_Set to_be_removed;
  to_be_removed.insert(x1);
  to_be_removed.insert(x3);
  to_be_removed.insert(x4);

  box.remove_space_dimensions(to_be_removed);

  Rational_Box known_result(1, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box,
                    "*** box.remove_space_dimensions({x1, x3, x4}) ***");

  return ok;
}

bool
test04() {
  Variable x1(0);
  Variable x2(1);

  TBox box(2);
  box.add_constraint(x1 <= 2);
  box.add_constraint(x2 <= 10);

  print_constraints(box, "*** box ***");

  box.remove_higher_space_dimensions(0);

  Rational_Box known_result(0, UNIVERSE);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_higher_space_dimensions(0) ***");

  return ok;
}

bool
test05() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);

  TBox box(4);
  box.add_constraint(x2 >= 5);
  box.add_constraint(x2 <= 0);
  box.add_constraint(x4 >= 3);

  print_constraints(box, "*** box ***");

  box.remove_higher_space_dimensions(1);

  Rational_Box known_result(1, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_higher_space_dimensions(1) ***");

  return ok;
}

bool
test06() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);

  TBox box(5);
  box.add_constraint(x2 >= 5);
  box.add_constraint(x4 >= 3);

  print_constraints(box, "*** box ***");

  box.remove_higher_space_dimensions(3);

  Rational_Box known_result(3);
  known_result.add_constraint(x2 >= 5);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_higher_space_dimensions(3) ***");

  return ok;
}

bool
test07() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TBox box(3);
  box.add_constraint(x2 >= 5);

  print_constraints(box, "*** box ***");

  Rational_Box known_result(box);

  box.remove_higher_space_dimensions(3);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_higher_space_dimensions(3) ***");

  return ok;
}

bool
test08() {
  TBox box(5);

  try {
    // This is an invalid use of the method
    // Box::remove_higher_dimensions(n): it is illegal to erase
    // a variable that is not in the space of the polyhedron.
    box.remove_higher_space_dimensions(7);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x <= 3);
  cs.insert(y - 0*z <= 2);
  TBox box(cs);

  Variables_Set to_be_removed;
  to_be_removed.insert(z);

  box.remove_space_dimensions(to_be_removed);

  try {
    to_be_removed.insert(x);
    // This is an incorrect use use of method
    // Box::remove_dimensions(to_be_remove).
    // Here the set `to_be_removed' still contains variable `z'.
    // This variable is now beyond the space dimension,
    // so that a dimension-incompatibility exception is obtained.
    box.remove_space_dimensions(to_be_removed);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test10() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);

  Constraint_System cs;
  cs.insert(x2 >= 6);
  cs.insert(x4 >= 4);
  cs.insert(x5 == 3);
  TBox box(cs);

  print_constraints(box, "*** box ***");

  cs = box.minimized_constraints();
  box.remove_higher_space_dimensions(3);

  Rational_Box known_result(3);
  known_result.add_constraint(x2 >= 6);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_higher_space_dimensions(3) ***");

  return ok;
}

bool
test11() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);

  Constraint_System cs;
  cs.insert(x2 >= 6);
  cs.insert(x4 >= 4);
  cs.insert(x5 == 3);
  TBox box(cs);

  print_constraints(box, "*** box ***");

  Variables_Set to_be_removed;
  to_be_removed.insert(x4);
  to_be_removed.insert(x5);

  cs = box.minimized_constraints();
  box.remove_space_dimensions(to_be_removed);

  Rational_Box known_result(3);
  known_result.add_constraint(x2 >= 6);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.remove_space_dimensions({x4, x5}) ***");

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
END_MAIN
