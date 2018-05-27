/* Test Polyhedron::add_generator().
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

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A + 2*B));
  gs.insert(point(A + B));
  gs.insert(point(2*A + 2*B));
  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.add_generator(ray(A));

  Generator_System known_gs;
  known_gs.insert(point());
  known_gs.insert(point(A + 2*B));
  known_gs.insert(ray(A));
  C_Polyhedron known_result(known_gs);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ph.add_generator(ray(A)) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph_empty(2, EMPTY);
  Generator_System gs1 = ph_empty.generators();
  assert(gs1.space_dimension() == ph_empty.space_dimension());

  gs1.insert(point());
  gs1.insert(ray(A));
  C_Polyhedron ph1(gs1);

  print_generators(ph1, "*** ph1 ***");

  C_Polyhedron known_result1(2);
  known_result1.add_constraint(A >= 0);
  known_result1.add_constraint(B == 0);

  Generator_System gs2 = ph_empty.generators();
  assert(gs2.space_dimension() == ph_empty.space_dimension());

  gs2.insert(point(C));
  gs2.insert(line(C));
  gs2.insert(ray(A));
  C_Polyhedron ph2(gs2);

  print_generators(ph2, "*** ph2 ***");

  C_Polyhedron known_result2(3);
  known_result2.add_constraint(A >= 0);
  known_result2.add_constraint(B == 0);

  bool ok = (ph1 == known_result1 && ph2 == known_result2);

  return ok;
}

bool
test03() {
  C_Polyhedron ph(0, EMPTY);
  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(0, UNIVERSE);
  print_constraints(known_result, "*** known_result ***");

  ph.add_generator(point());

  bool ok = (ph == known_result);
  print_constraints(ph, "*** ph.add_generator(point()) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
