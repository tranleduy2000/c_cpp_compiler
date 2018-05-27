/* Test Polyhedron::minimized_constraints().
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
#include <algorithm>

namespace {

bool
test01() {
  NNC_Polyhedron ph1;

  Constraint_System cs = ph1.minimized_constraints();

  NNC_Polyhedron ph2(cs);

  const Constraint_System& min_cs = ph2.minimized_constraints();

  bool ok = (ph1 == ph2
	     && min_cs.begin() == min_cs.end());

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x < 1);
  cs.insert(y > 0);

  NNC_Polyhedron ph(cs);
  const Constraint_System& min_cs = ph.minimized_constraints();
  return std::distance(min_cs.begin(), min_cs.end()) == 3;
}

} // namespace


BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN

