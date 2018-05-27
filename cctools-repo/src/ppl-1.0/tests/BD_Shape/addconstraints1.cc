/* Test BD_Shape::add_constraints().
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
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B == 5);

  TBD_Shape bds(2);
  bds.add_constraints(cs);

  print_constraints(bds, "*** bds.add_constraints(cs) ***");

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B == 5);
  known_result.add_constraint(B - A <= 5);

  bool ok = check_result(bds, known_result);

  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);

  try {
    // This is an invalid use of method
    // BD_Shape::add_constraint: it is illegal
    // to add a strict inequality.
    bds.add_constraint(x <= 0);
    bds.add_constraint(y < 0);
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
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(2);

  try {
    // This is an invalid use of method
    // BD_Shape::add_constraint: it is illegal
    // to add a constraint with bigger dimension.
    bds.add_constraint(x <= 0);
    bds.add_constraint(y - x + z >= 0);
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
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(1);

  try {
    // This is an invalid use of the method
    // BD_Shape::add_constraints(cs): it is illegal to
    // add a system of constraints that is not dimensional incompatible
    // with the polyhedron.
    Constraint_System cs;
    cs.insert(x - y >= 0);
    bds.add_constraints(cs);
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
test05() {
  Variable y(1);

  TBD_Shape bds(1);

  try {
    // This is an invalid use of the method
    // BD_Shape::add_constraint(c): it is illegal to insert a
    // constraints that contains a variable that is not in the space
    // of the polyhedron.
    bds.add_constraint(y >= 0);
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
test06() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(1);

  try {
    // This is an invalid use of the method
    // BD_Shape::add_constraints(cs): it is illegal to add a system
    // of constraints that is dimensional incompatible with the
    // polyhedron.
    Constraint_System cs;
    cs.insert(x - y == 0);
    bds.add_constraints(cs);
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
