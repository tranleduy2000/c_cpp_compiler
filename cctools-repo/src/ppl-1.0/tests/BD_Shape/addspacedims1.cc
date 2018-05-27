/* Test BD_Shape::add_space_dimensions_and_embed():
   we add two variables to a BD_Shape.
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

  TBD_Shape bds(2);
  bds.add_constraint(x <= 2);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_embed(2);
  bds.add_constraint(z <= 2);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(z <= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.add_space_dimensions_and_embed(2) "
                    "and bds.add_constraint(z <= 2) ***");

  return ok;
}

bool
test02() {
  TBD_Shape bds1(0, EMPTY);
  TBD_Shape bds2(1, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.add_space_dimensions_and_embed(2);
  bds2.add_space_dimensions_and_embed(1);

  bool ok = (bds1 == bds2);

  print_constraints(bds1, "*** bds1.add_space_dimensions_and_embed(2) ***");
  print_constraints(bds2, "*** bds2.add_space_dimensions_and_embed(1) ***");

  return ok;
}

bool
test03() {
  TBD_Shape bds(0, UNIVERSE);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_embed(3);

  BD_Shape<mpq_class> known_result(3, UNIVERSE);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.add_space_dimension_and_embed(3) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(0);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_project(4);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C == 0);
  known_result.add_constraint(D == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.add_space_dimensions_and_project(4) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A == 1);
  bds.add_constraint(C - B >= 9);

  BD_Shape<mpq_class> known_result(bds);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_project(0);

  print_constraints(bds, "*** bds.add_space_dimensions_and_project(0) ***");

  bool ok = check_result(bds, known_result);

  return ok;
}

bool
test06() {
  //Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(1);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_project(3);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C == 0);
  known_result.add_constraint(D == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.add_space_dimensions_and_project(3) ***");

  return ok;
}

bool
test07() {
  TBD_Shape bds(10, UNIVERSE);

  bds.remove_higher_space_dimensions(5);
  bds.add_space_dimensions_and_embed(6);

  return bds == TBD_Shape(11, UNIVERSE);
}

bool
test08() {
  Variable x(0);
  //Variable y(1);
  Variable z(2);
  Variable w(3);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 2);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_project(2);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(w == 0);
  known_result.add_constraint(z == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.add_space_dimensions_and_project(2) ***");

  return ok;
}

bool
test09() {
  TBD_Shape bds1(0, EMPTY);
  TBD_Shape bds2(1, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.add_space_dimensions_and_project(2);
  bds2.add_space_dimensions_and_project(1);

  bool ok = (bds1 == bds2);

  print_constraints(bds1, "*** bds1.add_space_dimensions_and_project(2) ***");
  print_constraints(bds2, "*** bds2.add_space_dimensions_and_project(1) ***");

  return ok;
}

bool
test10() {
  TBD_Shape bds(0, EMPTY);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_project(3);

  BD_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.add_space_dimension_and_project(3) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(0);

  print_constraints(bds, "*** bds ***");

  bds.add_space_dimensions_and_project(4);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A == 0);
  known_result.add_constraint(B == 0);
  known_result.add_constraint(C == 0);
  known_result.add_constraint(D == 0);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.add_space_dimensions_and_project(4) ***");

  return ok;
}

bool
test12() {
  Variable A(0);

  TBD_Shape bds1(2);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(A <= 2);

  TBD_Shape bds2(bds1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.add_space_dimensions_and_project(0);
  bds1.add_space_dimensions_and_embed(0);

  bool ok = (bds1 == bds2);

  print_constraints(bds1, "*** bds1.add_space_dimensions_and_project(0) ***");
  print_constraints(bds2, "*** bds2.add_space_dimensions_and_embed(0) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  //Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x <= 2);

  TBD_Shape bds(cs);

  print_constraints(bds, "*** bds ***");

  bds.minimized_constraints();
  bds.add_space_dimensions_and_embed(3);
  bds.add_constraint(z <= 2);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(z <= 2);

  bool ok = check_result(bds, known_result) ;

  print_constraints(bds,
                    "*** bds.add_space_dimensions_and_embed(2) "
                    "and bds.add_constraint(z <= 2) ***");

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
