/* Test Grid_Generator_System IO operators.
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

// Single point.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs1(grid_point(A + 7*B));

  std::stringstream ss1;
  ss1 << gs1;

  print_generators(gs1, "*** gs1 ***");

  Grid_Generator_System gs2;
  gs2.insert(grid_point(A + 7*B));

  std::stringstream ss2;
  ss2 << gs2;

  print_generators(gs2, "*** gs2 ***");

  bool ok = (!ss2.str().compare(ss1.str()));

  return ok;
}

// Many generators.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs1(grid_point(A + 7*B));
  gs1.insert(parameter(2*C));

  std::stringstream ss1;
  ss1 << gs1;

  print_generators(gs1, "*** gs1 ***");

  Grid_Generator_System gs2;
  gs2.insert(grid_point(A + 7*B));
  gs2.insert(parameter(2*C));

  std::stringstream ss2;
  ss2 << gs2;

  print_generators(gs2, "*** gs2 ***");

  bool ok = (!ss2.str().compare(ss1.str()));

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
