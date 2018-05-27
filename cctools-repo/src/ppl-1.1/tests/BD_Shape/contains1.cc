/* Test BD_Shape::contains().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
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

bool
test01() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(x - y <= 1);

  // The BD_Shape is empty, because it has got a negative cycle.
  TBD_Shape bds2(3);
  bds2.add_constraint(x - y <= 2);
  bds2.add_constraint(y - z <= 2);
  bds2.add_constraint(z - x <= -5);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bool ok = bds1.contains(bds2);

  nout << "*** bds1.contains(bds2) ***"
       << endl
       << (ok ? "true" : "false")
       << endl;

  return ok;
}

bool
test02() {
  TBD_Shape bds1;
  TBD_Shape bds2(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bool ok = bds1.contains(bds2);

  nout << "*** bds1.contains(bds2) ***"
       << endl
       << (ok ? "true" : "false")
       << endl;

  return ok;
}

bool
test03() {
  TBD_Shape bds1(0, EMPTY);
  TBD_Shape bds2(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bool ok = bds1.contains(bds2);

  nout << "*** bds1.contains(bds2) ***"
       << endl
       << (ok ? "true" : "false")
       << endl;

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(3);
  bds1.add_constraint(x - y >= 0);

  TBD_Shape bds2(2);
  bds2.add_constraint(x - y == 0);

  try {
    // This is an invalid use of Polyhedron::contains(): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    bds1.contains(bds2);
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
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(D >= 0);
  cs.insert(C >= 0);
  cs.insert(B <= 0);
  cs.insert(A >= 0);

  TBD_Shape bds1(cs);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(cs);
  bds2.add_constraint(A - B >= 0);

  print_constraints(bds2, "*** bds2 ***");

  bool contained = bds1.contains(bds2);

  nout << "*** bds1.contains(bds2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return contained;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(C <= 0);
  bds1.add_constraint(B - C <= 1);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(3);
  bds2.add_constraint(A == 0);
  bds2.add_constraint(C <= 0);
  bds2.add_constraint(B - C <= 2);

  print_constraints(bds2, "*** bds2 ***");

  bool contained = bds1.contains(bds2);

  nout << "*** bds1.contains(bds2) ***" << endl;
  nout << (!contained ? "true" : "false") << endl;

  return !contained;
}

bool
test07() {
  TBD_Shape bds1(3, EMPTY);

  print_constraints(bds1, "*** bds1 ***");

  TBD_Shape bds2(3, UNIVERSE);

  print_constraints(bds2, "*** bds2 ***");

  bool contained = bds1.contains(bds2);

  nout << "*** bds1.contains(bds2) ***" << endl;
  nout << (contained ? "true" : "false") << endl;

  return !contained;
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
