/* Test Octagonal_Shape::is_universe().
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

  TOctagonal_Shape oc(4);
  oc.add_constraint(-x <= 4);
  oc.add_constraint(y - x <= 0);
  oc.add_constraint(x - y <= -5);

  bool universe = oc.is_universe();

  nout << "*** oc.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return !universe;
}

bool
test02() {
  TOctagonal_Shape oc(4);

  bool universe = oc.is_universe();

  nout << "*** oc.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return universe;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);
  oc.add_constraint(x >= 1);
  oc.add_constraint(y >= 1);
  oc.add_constraint(x <= 4);
  oc.add_constraint(y <= 4);

  bool universe = oc.is_universe();

  nout << "*** oc.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return !universe;
}

bool
test04() {
  TOctagonal_Shape oc(0);

  bool universe = oc.is_universe();

  nout << "*** oc.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return universe;
}

bool
test05() {
  //  Variable x(0);

  TOctagonal_Shape oc(1, EMPTY);

  bool universe = oc.is_universe();

  nout << "*** oc.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return !universe;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
