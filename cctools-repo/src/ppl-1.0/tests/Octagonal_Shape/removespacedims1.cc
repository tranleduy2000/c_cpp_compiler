/* Remove the higher variables from the space.
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
  Variable x2(1);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(x2 <= 3);

  print_constraints(oct1, "*** oct1 ***");

  oct1.remove_higher_space_dimensions(2);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x2 <= 3);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.remove_higher_space_dimensions(2) ***");

  return ok;
}

bool
test02() {
  Variable x2(1);
  Variable x3(2);
  Variable x5(4);

  TOctagonal_Shape oc1(6);
  oc1.add_constraint(x2 - x3 <= 0);
  oc1.add_constraint(x3 <= 2);
  oc1.add_constraint(x5 <= 3);

  print_constraints(oc1, "*** oc1 ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(x3);
  to_be_removed.insert(x5);
  oc1.remove_space_dimensions(to_be_removed);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(x2 <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc1) == known_result);

  print_constraints(oc1, "*** oct1.remove_space_dimensions({x3, x5}) ***");

  return ok;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  Variable x6(5);

  TOctagonal_Shape oct1(6);
  oct1.add_constraint(x1 >= 1);
  oct1.add_constraint(x1 + x3 >= 2);
  oct1.add_constraint(x2 - x3 <= 4);
  oct1.add_constraint(x4 - x1  >= 0);
  oct1.add_constraint(x6 <= 7);
  oct1.add_constraint(x5 + x4 >= 1);

  print_constraints(oct1, "*** oct1 ***");

  oct1.remove_higher_space_dimensions(3);

  Octagonal_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x1 >= 1);
  known_result.add_constraint(x1 + x3 >= 2);
  known_result.add_constraint(x2 - x3 <= 4);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.remove_higher_space_dimensions(3) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(6);

  // A 10-dim space, empty polyhedron.
  TOctagonal_Shape oc(10, EMPTY);

  print_constraints(oc, "*** oc ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(y);
  to_be_removed.insert(z);
  to_be_removed.insert(w);

  oc.remove_space_dimensions(to_be_removed);

  // A 7-dim space, empty polyhedron.
  Octagonal_Shape<mpq_class> known_result(7, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.remove_space_dimensions({y, z, w}) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  // A 4-dim space, empty polyhedron.
  TOctagonal_Shape oc(4, EMPTY);

  print_constraints(oc, "*** oc ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(y);
  to_be_removed.insert(z);
  to_be_removed.insert(w);
  to_be_removed.insert(x);

  oc.remove_space_dimensions(to_be_removed);

  Octagonal_Shape<mpq_class> known_result(0, EMPTY);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.remove_space_dimensions({x, y, z, w}) ***");

  return ok;
}

bool
test06() {
  TOctagonal_Shape oc(5);

  try {
    // This is an invalid use of the method
    // Octagonal_Shape::remove_higher_dimensions(n): it is illegal to erase
    // a variable that is not in the space of the polyhedron.
    oc.remove_higher_space_dimensions(7);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x <= 3);
  cs.insert(y - z <= 2);
  TOctagonal_Shape oc(cs);

  Variables_Set to_be_removed;
  to_be_removed.insert(z);

  oc.remove_space_dimensions(to_be_removed);

  try {
    to_be_removed.insert(x);
    // This is an incorrect use use of method
    // Octagonal_Shape::remove_space_dimensions(to_be_remove).
    // Here the set `to_be_removed' still contains variable `z'.
    // This variable is now beyond the space dimension,
    // so that a dimension-incompatibility exception is obtained.
    oc.remove_space_dimensions(to_be_removed);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  TOctagonal_Shape oc(4);
  oc.add_constraint(x - y <= 0);
  oc.add_constraint(z <= 2);
  oc.add_constraint(w >= 3);

  print_constraints(oc, "*** oc ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;

  oc.remove_space_dimensions(to_be_removed);

  Octagonal_Shape<mpq_class> known_result(oc);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.remove_space_dimensions() ***");

  return ok;
}

bool
test09() {
  TOctagonal_Shape oc(0);

  print_constraints(oc, "*** oc ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;

  oc.remove_space_dimensions(to_be_removed);

  Octagonal_Shape<mpq_class> known_result(oc);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.remove_space_dimensions() ***");

  return ok;
}

bool
test10() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  Variable x6(5);

  TOctagonal_Shape oct1(6);
  oct1.add_constraint(x1 >= 1);
  oct1.add_constraint(x1 + x3 >= 2);
  oct1.add_constraint(x2 - x3 <= 4);
  oct1.add_constraint(x4 - x1  >= 0);
  oct1.add_constraint(x6 <= 7);
  oct1.add_constraint(x5 + x4 >= 1);

  print_constraints(oct1, "*** oct1 ***");

  oct1.remove_higher_space_dimensions(6);

  Octagonal_Shape<mpq_class> known_result(oct1);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.remove_higher_space_dimensions(6) ***");

  return ok;
}

bool
test11() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct1(3);
  oct1.add_constraint(x1 - x2 >= 13);
  oct1.add_constraint(x2 <= 3);
  oct1.add_constraint(x2 + x3 == 5);

  print_constraints(oct1, "*** oct1 ***");

  oct1.remove_higher_space_dimensions(0);

  Octagonal_Shape<mpq_class> known_result(0, UNIVERSE);

  bool ok = (Octagonal_Shape<mpq_class>(oct1) == known_result);

  print_constraints(oct1, "*** oct1.remove_higher_space_dimensions(0) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  TOctagonal_Shape oc(4);
  oc.add_constraint(x - y <= 0);
  oc.add_constraint(z <= 2);
  oc.add_constraint(w >= 3);

  print_constraints(oc, "*** oc ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(x);
  to_be_removed.insert(y);
  to_be_removed.insert(z);
  to_be_removed.insert(w);

  oc.remove_space_dimensions(to_be_removed);

  Octagonal_Shape<mpq_class> known_result(0, UNIVERSE);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.remove_space_dimensions({x, y, z, w}) ***");

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
END_MAIN
