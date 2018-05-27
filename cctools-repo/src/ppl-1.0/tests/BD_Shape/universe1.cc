/* Test BD_Shape::is_universe().
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

  TBD_Shape bds(4);

  bds.add_constraint(-x <= 4);
  bds.add_constraint(y - x <= 0);
  bds.add_constraint(x - y <= -5);

  bool universe = bds.is_universe();

  nout << "*** bds.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return !universe;
}

bool
test02() {
  TBD_Shape bds(4);

  bool universe = bds.is_universe();

  nout << "*** bds.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return universe;
}

bool
test03() {
  TBD_Shape bds(0);

  bool universe = bds.is_universe();

  nout << "*** bds.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return universe;
}

bool
test04() {
  TBD_Shape bds(20, EMPTY);

  bool universe = bds.is_universe();

  nout << "*** bds.is_universe() ***" << endl;
  nout << (universe ? "true" : "false") << endl;

  return !universe;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
