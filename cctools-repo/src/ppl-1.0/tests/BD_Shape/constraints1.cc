/* Test BD_Shape::constraints().
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
  TBD_Shape bds1(0, EMPTY);

  BD_Shape<mpq_class> known_result(bds1);

  Constraint_System cs = bds1.constraints();
  TBD_Shape bds2(cs);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test02() {
  TBD_Shape bds1(0, UNIVERSE);

  BD_Shape<mpq_class> known_result(bds1);

  Constraint_System cs = bds1.constraints();
  TBD_Shape bds2(cs);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(B - C >= 1);
  bds1.add_constraint(C - A <= 9);

  BD_Shape<mpq_class> known_result(bds1);

  bds1.contains(bds1);

  Constraint_System cs = bds1.constraints();
  TBD_Shape bds2(cs);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(B - C == 1);
  bds1.add_constraint(C - A <= 9);

  Constraint_System cs = bds1.constraints();
  TBD_Shape bds2(cs);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  BD_Shape<mpq_class> known_result(bds1);

  bool ok = check_result(bds2, known_result);

  return ok;
}

bool
test05() {

  TBD_Shape bds1(0);
  bds1.add_constraint(Linear_Expression(1) == 0);

  TBD_Shape bds2(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result(bds2);

  bool ok = check_result(bds1, known_result);

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds(4);
  bds.refine_with_constraint(A > 0);
  bds.refine_with_constraint(B < 0);
  bds.refine_with_constraint(2*B - 2*C < 1);
  bds.refine_with_constraint(A - C > 2);
  bds.refine_with_constraint(A + 2*D >= 5);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 0);
  known_result.add_constraint(2*B - 2*C <= 1);
  known_result.add_constraint(A - C >= 2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs(A > 0);
  cs.insert(2*B - 2*C <= 1);
  cs.insert(A - 5*C > 4);

  TBD_Shape bds(3);
  bds.refine_with_constraints(cs);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(2*B - 2*C <= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds ***");

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
END_MAIN
