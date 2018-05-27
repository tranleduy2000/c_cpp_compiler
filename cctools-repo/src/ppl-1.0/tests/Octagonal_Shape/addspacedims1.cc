/* Test Octagonal_Shape::add_space_dimensions_and_embed() and
   Octagonal_Shape::add_space_dimensions_and_project().
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
//   Variable y(1);
//   Variable z(2);
  Variable w(3);

  TOctagonal_Shape oc(2);
  oc.add_constraint(x <= 2);

  print_constraints(oc, "*** oc ***");

  oc.add_space_dimensions_and_embed(2);
  oc.add_constraint(w <= 2);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(w <= 2);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc,
                    "*** oc.add_space_dimensions_and_embed(2) "
                    "and oc.add_constraint(w <= 2) ***");

  return ok;
}

bool
test02() {
  TOctagonal_Shape oc1(0, EMPTY);
  TOctagonal_Shape oc2(1, EMPTY);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  oc1.add_space_dimensions_and_embed(2);
  oc2.add_space_dimensions_and_embed(1);

  bool ok = (oc1 == oc2);

  print_constraints(oc1, "*** oc1.add_space_dimensions_and_embed(2) ***");
  print_constraints(oc2, "*** oc2.add_space_dimensions_and_embed(1) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  //  Variable y(1);
  Variable z(2);
  Variable w(3);

  TOctagonal_Shape oc(2);
  oc.add_constraint(x <= 2);

  print_constraints(oc, "*** oc ***");

  oc.add_space_dimensions_and_project(2);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(w == 0);
  known_result.add_constraint(z == 0);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.add_space_dimensions_and_project(2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Constraint_System cs;
  cs.insert(x >= 2);
  cs.insert(y >= 2);
  cs.insert(x <= 6);
  cs.insert(y <= 6);

  TOctagonal_Shape oc(cs);

  print_constraints(oc, "*** oc ***");

  oc.add_space_dimensions_and_project(2);

  Octagonal_Shape<mpq_class> known_result(4);
  known_result.add_constraint(z == 0);
  known_result.add_constraint(w == 0);
  known_result.add_constraint(x >= 2);
  known_result.add_constraint(y >= 2);
  known_result.add_constraint(x <= 6);
  known_result.add_constraint(y <= 6);

  bool ok = (Octagonal_Shape<mpq_class>(oc) == known_result);

  print_constraints(oc, "*** oc.add_space_dimensions_and_project(2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(A >= 0);
  oc1.add_constraint(A <= 2);

  TOctagonal_Shape oc2(oc1);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  oc1.add_space_dimensions_and_embed(0);
  oc2.add_space_dimensions_and_project(0);

  bool ok = (oc1 == oc2);

  print_constraints(oc1, "*** oc1.add_space_dimensions_and_embed(0) ***");
  print_constraints(oc2, "*** oc2.add_space_dimensions_and_project(0) ***");

  return ok;
}

bool
test06() {
  TOctagonal_Shape oc1(0, EMPTY);
  TOctagonal_Shape oc2(1, EMPTY);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  oc1.add_space_dimensions_and_project(2);
  oc2.add_space_dimensions_and_project(1);

  bool ok = (oc1 == oc2);

  print_constraints(oc1, "*** oc1.add_space_dimensions_and_project(2) ***");
  print_constraints(oc2, "*** oc2.add_space_dimensions_and_project(1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);

  TOctagonal_Shape oc1(0);
  TOctagonal_Shape oc2(1);
  oc2.add_constraint(A == 0);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  oc1.add_space_dimensions_and_project(2);
  oc2.add_space_dimensions_and_project(1);

  bool ok = (oc1 == oc2);

  print_constraints(oc1, "*** oc1.add_space_dimensions_and_project(2) ***");
  print_constraints(oc2, "*** oc2.add_space_dimensions_and_project(1) ***");

  return ok;
}

bool
test08() {
  //  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc1(1);
  TOctagonal_Shape oc2(2);
  oc2.add_constraint(B == 0);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  oc1.add_space_dimensions_and_project(2);
  oc2.add_space_dimensions_and_project(1);

  bool ok = (oc1 == oc2);

  print_constraints(oc1, "*** oc1.add_space_dimensions_and_project(2) ***");
  print_constraints(oc2, "*** oc2.add_space_dimensions_and_project(1) ***");

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
