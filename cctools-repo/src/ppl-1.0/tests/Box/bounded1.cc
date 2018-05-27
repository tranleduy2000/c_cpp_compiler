/* Test Box::is_bounded().
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

  // This is a non-bounded box.
  TBox box(2);
  box.add_constraint(x >= 0);

  print_constraints(box, "*** box ***");

  return !box.is_bounded();
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  // This is a bounded box (it is a square);
  TBox box(2);
  box.add_constraint(x >= 2);
  box.add_constraint(y >= 2);
  box.add_constraint(x <= 4);
  box.add_constraint(y <= 4);

  print_constraints(box, "*** box ***");

  return box.is_bounded();
}

bool
test03() {
  // This is a universal, zero-dimensional box.
  TBox box;

  print_constraints(box, "*** box ***");

  return box.is_bounded();
}

bool
test04() {
  // This is an empty, zero-dimensional box.
  TBox box;
  box.add_constraint(Linear_Expression(-3) >= 0);

  print_constraints(box, "*** box ***");

  return box.is_bounded();
}

bool
test05() {
  // This is an empty box.
  TBox box(4, EMPTY);

  print_constraints(box, "*** box ***");

  return box.is_bounded();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
