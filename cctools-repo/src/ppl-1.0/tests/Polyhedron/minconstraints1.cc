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

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(A + B >= -2);
  ph1.add_constraint(A >= -3);

  const Constraint_System cs = ph1.minimized_constraints();

  C_Polyhedron ph2(cs);
  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (known_result == ph2);

  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test02() {
  C_Polyhedron ph1(2, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron known_result;
  known_result = ph1;

  Constraint_System cs = ph1.minimized_constraints();

  C_Polyhedron ph2(cs);

  bool ok = (ph2 == known_result);

  print_constraints(cs, "*** cs ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
