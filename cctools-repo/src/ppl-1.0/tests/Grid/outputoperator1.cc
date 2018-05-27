/* Test IO_Operators::operator<<(s, gr).
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
#include <sstream>

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

// One dimension universe and empty.
bool
test01() {
  Grid gr1(1, EMPTY);

  std::stringstream ss1;
  ss1 << gr1;

  Grid gr2(1);

  std::stringstream ss2;
  ss2 << gr2;

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Many dimensioned universe and empty.
bool
test02() {
  Grid gr1(3, EMPTY);

  std::stringstream ss1;
  ss1 << gr1;

  Grid gr2(3);

  std::stringstream ss2;
  ss2 << gr2;

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Universe and empty, mixed dimensions.
bool
test03() {
  Grid gr1(4, EMPTY);

  std::stringstream ss1;
  ss1 << gr1;

  Grid gr2(3);

  std::stringstream ss2;
  ss2 << gr2;

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Grids of same dimensions.
bool
test04() {
  Variable A(0);
  Variable C(2);

  Grid gr1(4, EMPTY);
  gr1.add_grid_generator(grid_point(3*A + C));
  gr1.add_grid_generator(parameter(3*A));

  std::stringstream ss1;
  ss1 << gr1;

  Grid gr2(4);
  gr2.add_constraint(3*A == 0);

  std::stringstream ss2;
  ss2 << gr2;

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Grids of mixed dimensions.
bool
test05() {
  Variable A(0);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*A + C));
  gr1.add_grid_generator(parameter(3*A));

  std::stringstream ss1;
  ss1 << gr1;

  Grid gr2(4);
  gr2.add_constraint(3*A == 0);

  std::stringstream ss2;
  ss2 << gr2;

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
