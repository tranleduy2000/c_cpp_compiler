/* Different ways of creating an empty Box.
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

  TBox box1(4);
  TBox box2(4);

  box1.add_constraint(-x <= 4);
  box1.add_constraint(y <= 0);
  box1.add_constraint(z >= 0);
  box1.add_constraint(y >= 5);

  bool empty1 = box1.is_empty();

  nout << "*** box1.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false ") << endl;

  box2.add_constraint(-x <= 4);
  box2.add_constraint(y <= 0);
  box2.add_constraint(z >= 0);

  bool empty2 = box2.is_empty();

  nout << "*** box2.is_empty() ***" << endl;
  nout << (empty2 ? "true" : "false") << endl;

  return empty1 && !empty2;

}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(4);
  TBox box2(4);

  box1.add_constraint(-5*x <= 2);
  box1.add_constraint(6*y <= -7);
  box1.add_constraint(6*y >= -5);

  bool empty1 = box1.is_empty();

  print_constraints(box1, "*** box1 ***");
  nout << "*** box1.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false") << endl;

  box2.add_constraint(-5*x <= 7);
  box2.add_constraint(6*y <= 1);
  box2.add_constraint(-6*y <= 2);
  box2.add_constraint(z <= 1);

  bool empty2 = box2.is_empty();

  print_constraints(box2, "*** box2 ***");
  nout << "*** box2.is_empty() ***" << endl;
  nout << (empty2 ? "true" : "false") << endl;

  return empty1 && !empty2;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);
  Variable x5(4);
  // Variable x6(5);

  TBox box1(6);
  TBox box2(6);

  box1.add_constraint(x1 <= 3);
  box1.add_constraint(x4 <= 3);
  box1.add_constraint(x2 <= 0);
  box1.add_constraint(x3 <= -2);
  box1.add_constraint(x5 <= 2);
  box1.add_constraint(-x2 <= 0);
  box1.add_constraint(x3 <= 5);
  box1.add_constraint(x4 <= -6);
  box1.add_constraint(x1 <= 5);
  box1.add_constraint(3*x5 <= 2);
  box1.add_constraint(-x5 <= -5);
  box1.add_constraint(-x5 <= 7);

  bool empty1 = box1.is_empty();

  nout << "*** box1.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false") << endl;

  box2.add_constraint(x1 <= 3);
  box2.add_constraint(x4 <= 3);
  box2.add_constraint(x2 <= 0);
  box2.add_constraint(x3 <= 2);
  box2.add_constraint(x5 <= 2);
  box2.add_constraint(-x2 <= 0);
  box2.add_constraint(x3 <= 5);
  box2.add_constraint(x4 <= 6);
  box2.add_constraint(x1 <= 5);
  box2.add_constraint(x5 <= 2);
  box2.add_constraint(-x5 <= 5);
  box2.add_constraint(x3 <= 7);

  bool empty2 = box2.is_empty();

  nout << "*** box2.is_empty() ***" << endl;
  nout << (empty2 ? "true" : "false") << endl;

  return empty1 && !empty2;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);

  box.add_constraint(A == 0);
  box.add_constraint(C >= 0);
  box.add_constraint(B >= 1);

  bool empty = box.is_empty();

  print_constraints(box, "*** box ***");
  nout << "*** box.is_empty() ***"
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

  TBox box(5);

  Coefficient a;
  if (std::numeric_limits<Coefficient>::is_bounded)
    a = -(std::numeric_limits<Coefficient>::min()/2) + 1;
  else
    a = 1300000000;

  box.add_constraint(2*x1 >= -a);
  box.add_constraint(3*x2 >= -a);
  box.add_constraint(5*x3 <= a);
  box.add_constraint(7*x4 <= a);
  box.add_constraint(11*x5 <= a);

  print_constraints(box, "*** box ***");

  bool empty = box.is_empty();

  nout << "*** box.is_empty() ***" << endl;
  nout << (empty ? "true" : "false") << endl;

  return !empty;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(3*x >= 7);
  box.add_constraint(3*x <= 7);
  box.add_constraint(2*y > 1);
  box.add_constraint(2*y <= 1);

  bool ok = box.is_empty();

  print_constraints(box, "*** box ***");

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
END_MAIN

