/* Test BD_Shape::is_discrete().
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

  TBD_Shape bds(3);

  bds.add_constraint(x <= 2);
  bds.add_constraint(x - y == 3);
  bds.add_constraint(y <= 2);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return !ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);

  bds.add_constraint(A <= 5);
  bds.add_constraint(A - B == 3);
  bds.add_constraint(C <= 2);
  bds.add_constraint(E - D == 2);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return !ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);

  bds.add_constraint(A == 5);
  bds.add_constraint(A - B == 3);
  bds.add_constraint(C <= 2);
  bds.add_constraint(E - D == 2);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return !ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A <= 3);
  bds.add_constraint(B - A <= -5);
  bds.add_constraint(-B <= 2);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return ok;
}

bool
test05() {
  TBD_Shape bds(2, EMPTY);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return ok;
}

bool
test06() {
  TBD_Shape bds(2);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return !ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable D(3);

  TBD_Shape bds(4);
  bds.add_constraint(A <= 1);
  bds.add_constraint(A - D == 8);
  bds.add_constraint(B <= 7);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return !ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);
  bds.add_constraint(A == 1);
  bds.add_constraint(E == 1);
  bds.add_constraint(A - D == 8);
  bds.add_constraint(B <= 7);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return !ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A == 0);
  bds.add_constraint(B == 2);

  print_constraints(bds, "*** bds ***");

  bool ok = bds.is_discrete();

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(7);
  bds.add_constraint(A <= 1);
  bds.add_constraint(B == 2);
  bds.add_constraint(B - A <= -6);

  print_constraints(bds, "*** bds ***");

   bool ok = bds.is_discrete();

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
END_MAIN


