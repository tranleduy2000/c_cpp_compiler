/* Different ways of creating an empty BD_Shape.
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
  Variable z(2);

  TBD_Shape bds1(4);
  TBD_Shape bds2(4);

  bds1.add_constraint(-x <= 4);
  bds1.add_constraint(y - x <= 0);
  bds1.add_constraint(x - y <= -5);

  bool empty = bds1.is_empty();

  nout << "*** bds1.is_empty() ***" << endl;
  nout << (empty ? "true" : "false ") << endl;

  bds2.add_constraint(-x <= 4);
  bds2.add_constraint(y - x <= 0);
  bds2.add_constraint(x - y <= 5);
  bds2.add_constraint(z - x <= 1);

  bool empty1 = bds2.is_empty();

  nout << "*** bds2.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false") << endl;

  return !empty1 && empty;

}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds1(4);
  TBD_Shape bds2(4);

  bds1.add_constraint(-x <= 2);
  bds1.add_constraint(y - x <= -9);
  bds1.add_constraint(x - y <= -7);

  bool empty = bds1.is_empty();

  print_constraints(bds1, "*** bds1 ***");
  nout << "*** bds1.is_empty() ***" << endl;
  nout << (empty ? "true" : "false") << endl;

  bds2.add_constraint(-x <= 7);
  bds2.add_constraint(y - x <= 1);
  bds2.add_constraint(-y <= 2);
  bds2.add_constraint(z - x <= 1);

  bool empty1 = bds2.is_empty();

  print_constraints(bds2, "*** bds2 ***");
  nout << "*** bds2.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false") << endl;

  return !empty1 && empty;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  // Variable x6(5);

  TBD_Shape bds1(6);
  TBD_Shape bds2(6);

  bds1.add_constraint(x1 <= 3);
  bds1.add_constraint(x4 <= 3);
  bds1.add_constraint(x2 - x1 <= 0);
  bds1.add_constraint(x3 - x1 <= -2);
  bds1.add_constraint(x5 - x1 <= 2);
  bds1.add_constraint(-x2 <= 0);
  bds1.add_constraint(x3 - x2 <= 5);
  bds1.add_constraint(x4 - x3 <= -6);
  bds1.add_constraint(x1 - x4 <= 5);
  bds1.add_constraint(x5 - x4 <= 2);
  bds1.add_constraint(-x5 <= -5);
  bds1.add_constraint(x3 - x5 <= 7);

  bool empty = bds1.is_empty();

  nout << "*** bds1.is_empty() ***" << endl;
  nout << (empty ? "true" : "false") << endl;

  bds2.add_constraint(x1 <= 3);
  bds2.add_constraint(x4 <= 3);
  bds2.add_constraint(x2 - x1 <= 0);
  bds2.add_constraint(x3 - x1 <= 2);
  bds2.add_constraint(x5 - x1 <= 2);
  bds2.add_constraint(-x2 <= 0);
  bds2.add_constraint(x3 - x2 <= 5);
  bds2.add_constraint(x4 - x3 <= 6);
  bds2.add_constraint(x1 - x4 <= 5);
  bds2.add_constraint(x5 - x4 <= 2);
  bds2.add_constraint(-x5 <= 5);
  bds2.add_constraint(x3 - x5 <= 7);

  bool empty1 = bds2.is_empty();

  nout << "*** bds2.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false") << endl;

  return !empty1 && empty;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);

  bds.add_constraint(A == 0);
  bds.add_constraint(C >= 0);
  bds.add_constraint(B - C >= 1);

  bool empty = bds.is_empty();

  print_constraints(bds, "*** bds ***");
  nout << "*** bds.is_empty() ***"
       << endl
       << (empty ? "true" : "false") << endl;

  return !empty;
}

bool
test05() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);

  TBD_Shape bds(5);

  Coefficient a;
  if (std::numeric_limits<Coefficient>::is_bounded)
    a = -(std::numeric_limits<Coefficient>::min()/2) + 1;
  else
    a = 1300000000;

  bds.add_constraint(x1 - x2 <= -a);
  bds.add_constraint(x2 - x3 <= -a);
  bds.add_constraint(x3 - x4 <= a);
  bds.add_constraint(x4 - x5 <= a);
  bds.add_constraint(x5 - x1 <= a);

  print_constraints(bds, "*** bds ***");

  bool empty = bds.is_empty();

  nout << "*** bds.is_empty() ***" << endl;
  nout << (empty ? "true" : "false") << endl;

  return !empty;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN

