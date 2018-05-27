/* Test time_elapse_assign().
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

  NNC_Polyhedron ph1(2);

  ph1.add_constraint(x >= 0);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(x + y - 2 <= 0);

  NNC_Polyhedron ph2(2);

  ph2.add_constraint(x > 2);
  ph2.add_constraint(x < 4);
  ph2.add_constraint(y == 3);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  Generator_System known_gs;
  known_gs.insert(point());
  known_gs.insert(point(2*x));
  known_gs.insert(point(2*y));
  known_gs.insert(ray(2*x + 3*y));
  known_gs.insert(ray(4*x + 3*y));

  NNC_Polyhedron known_result(known_gs);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** ph1_time_elapse_assign(ph2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x > 3);
  cs1.insert(y > 3);
  NNC_Polyhedron ph(cs1);

  NNC_Polyhedron ph1(ph);

  Generator_System gs;
  gs.insert(point(x + y));
  NNC_Polyhedron ph2(gs);

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  bool ok = (ph1 == ph);

  print_generators(ph1, "*** ph1_time_elapse_assign(ph2) ***");

 return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x == 3);
  ph1.add_constraint(y > 2);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 3);
  ph2.add_constraint(y > 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.time_elapse_assign(ph2);

  Generator_System gs;
  gs.insert(closure_point(3*x + 2*y));
  gs.insert(point(3*x + 3*y));
  gs.insert(ray(y));
  gs.insert(ray(x));

  NNC_Polyhedron known_result(gs);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** ph1_time_elapse_assign(ph2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
