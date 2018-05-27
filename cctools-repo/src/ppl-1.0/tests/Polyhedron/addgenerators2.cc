/* Test Polyhedron::add_generators().
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

  Generator_System gs1;
  gs1.insert(closure_point());
  gs1.insert(closure_point(4*x));
  gs1.insert(closure_point(4*y));
  gs1.insert(closure_point(4*x + 4*y));
  gs1.insert(point(2*x));
  gs1.insert(point(4*x + y));
  gs1.insert(point(x + 4*y));
  gs1.insert(point(3*y));

  NNC_Polyhedron ph(gs1);

  print_generators(ph, "*** ph ***");

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(point(4*x));
  gs2.insert(point(4*y));
  gs2.insert(point(4*x + 4*y));

  ph.add_generators(gs2);

  Generator_System gs3;
  gs3.insert(point());
  gs3.insert(point(4*x));
  gs3.insert(point(4*y));
  gs3.insert(point(4*x + 4*y));

  NNC_Polyhedron known_result(gs3);

  C_Polyhedron closed_ph(ph);
  C_Polyhedron closed_known_result(known_result);

  bool ok = (ph == known_result
	     && closed_ph == closed_known_result);

  print_generators(ph, "*** after add_generators(gs) ***");
  print_generators(closed_ph, "*** closed_ph ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  NNC_Polyhedron ph1(3, EMPTY);

  ph1.add_generator(point(x));

  Generator_System gs;
  gs.insert(point(1*x + 0*y + 0*z));
  NNC_Polyhedron known_result(gs);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** ph1 ***");
  print_generators(known_result, "*** known_result ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point(A + B));
  gs1.insert(closure_point());
  gs1.insert(ray(A));
  gs1.insert(ray(B));
  NNC_Polyhedron ph1(gs1);

  print_generators(ph1, "*** ph1 ***");

  ph1.topological_closure_assign();
  Generator_System gs2 = ph1.minimized_generators();

  print_generators(gs2, "*** gs2 ***");

  C_Polyhedron ph2(2, EMPTY);
  ph2.add_generators(gs2);

  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(known_result, "*** known_result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
