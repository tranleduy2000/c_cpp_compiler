/* Test Box::contains_integer_point().
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

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  TBox box(3);
  box.add_constraints(cs);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

bool
test02() {
  Variable x(0);

  Constraint_System cs;
  cs.insert(x > 0);
  cs.insert(x < 1);

  TBox box(3);
  box.add_constraints(cs);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(8*z >= 7);

  TBox box(3);
  box.add_constraints(cs);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

bool
test04() {
  Variable x(0);

  Constraint_System cs;
  cs.insert(x > 0);
  cs.insert(x < 1);

  TBox box(1);
  box.add_constraints(cs);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;
  return !contains;
}

bool
test05() {
  Variable x(0);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(2*x < 1);

  TBox box(1);
  box.add_constraints(cs);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

bool
test06() {
  Variable x(0);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= -7);

  TBox box(1);
  box.add_constraints(cs);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

bool
test07() {
  TBox box(1);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

bool
test08() {
  Variable x(0);

  TBox box(1);
  box.add_constraint(x >= 6);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

bool
test09() {
  Variable x(0);

  TBox box(1);
  box.add_constraint(x > 6);

  print_constraints(box, "*** box ***");

  bool contains = box.contains_integer_point();

  nout << "box.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
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
