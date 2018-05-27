/* Different ways of creating an empty polyhedron.
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
  // This is the easy way to create
  // the empty polyhedron in the zero-dimension space.
  C_Polyhedron ph1(0, EMPTY);

  // This is a more convoluted way of doing the same thing
  // using an empty system of generators.
  Generator_System gs;
  C_Polyhedron ph2(gs);

  bool ok = (ph1 == ph2 && ph1.is_empty() && ph2.is_empty());

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
