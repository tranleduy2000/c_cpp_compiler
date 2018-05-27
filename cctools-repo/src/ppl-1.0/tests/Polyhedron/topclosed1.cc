/* Test Polyhedron::is_topologically_closed().
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
  Variable A(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(A >= 0);

  NNC_Polyhedron ph2(2, EMPTY);

  NNC_Polyhedron ph3;

  bool ok = ph1.is_topologically_closed()
    && ph2.is_topologically_closed()
    && ph3.is_topologically_closed();

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(ph3, "*** ph3 ***");

  return ok;
}

bool
test02() {
  Variable A(0);

  NNC_Polyhedron ph1(1, EMPTY);

  Generator_System gs1;
  gs1.insert(point(A));
  gs1.insert(closure_point());
  gs1.insert(closure_point(A));
  ph1.add_generators(gs1);

  bool ok = !ph1.is_topologically_closed();

  print_generators(ph1, "*** ph1 ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(0*B));
  gs.insert(closure_point(-A));
  gs.insert(closure_point(A));
  gs.insert(line(A));

  NNC_Polyhedron ph(gs);

  bool ok = ph.is_topologically_closed();

  print_constraints(ph, "*** ph ***");
  print_generators(ph, "*** ph ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
