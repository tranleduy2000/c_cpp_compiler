/* Test Polyhedron::is_bounded().
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

  // This is a non-bounded polyhedron.
  C_Polyhedron ph1(2);
  ph1.add_constraint(x >= 0);

  print_constraints(ph1, "*** ph1 ***");

  return !ph1.is_bounded();
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  // This is a bounded polyhedron (it is a square);
  C_Polyhedron ph2(2);
  ph2.add_constraint(x >= 2);
  ph2.add_constraint(y >= 2);
  ph2.add_constraint(x <= 4);
  ph2.add_constraint(y <= 4);

  print_constraints(ph2, "*** ph2 ***");

  return ph2.is_bounded();
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  // This is a universal, zero-dimensional polyhedron.
  C_Polyhedron ph3;

  print_constraints(ph3, "*** ph3 ***");

  return ph3.is_bounded();
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  // This is an empty, zero-dimensional polyhedron.
  C_Polyhedron ph4;
  ph4.add_constraint(Linear_Expression(-3) >= 0);

  print_constraints(ph4, "*** ph4 ***");

  return ph4.is_bounded();
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  // This is an empty polyhedron.
  C_Polyhedron ph5(4, EMPTY);

  print_constraints(ph5, "*** ph5 ***");

  return ph5.is_bounded();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
