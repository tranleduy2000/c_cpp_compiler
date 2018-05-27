/* Test operator<<(ostream&, const Box&).
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

  TBox box1(3);
  TBox box2(3);

  box1.add_constraint(x <= 3);

  box2.add_constraint(-y <= -2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  // FIXME!!!
  return true;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBox box1(0, EMPTY);
  TBox box2(3);
  TBox box3(3);

  box2.add_constraint(-y <= -2);

  box3.add_constraint(x <= 0);
  box3.add_constraint(-x <= -1);
  box3.add_constraint(y <= 3);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");
  print_constraints(box3, "*** box3 ***");

  // FIXME!!!
  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN

