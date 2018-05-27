/* Test Polyhedron::concatenate_assign().
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
  Variable z(2);
  Variable w(3);
  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(y >= 0);
  cs1.insert(x - y >= 0);
  C_Polyhedron ph(cs1);

  print_constraints(ph, "*** ph before ***");

  Constraint_System cs2;
  cs2.insert(x >= 1);
  cs2.insert(y >= 1);
  cs2.insert(x - y >= -1);
  C_Polyhedron qh(cs2);

  C_Polyhedron copy_ph = ph;

  ph.concatenate_assign(qh);

  copy_ph.add_space_dimensions_and_embed(2);
  copy_ph.add_constraint(z >= 1);
  copy_ph.add_constraint(w >= 1);
  copy_ph.add_constraint(z - w >= -1);

  bool ok = (ph == copy_ph);

  print_constraints(ph, "*** concatenate_assign ***");
  print_constraints(copy_ph, "*** embed + renaming + insert ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  Constraint_System cs;
  cs.insert(x >= y);
  cs.insert(x >= 2);
  C_Polyhedron qh(cs);

  print_constraints(qh, "*** qh ***");

  ph.concatenate_assign(qh);

  C_Polyhedron known_result(4, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after concatenate_assign(qh) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph;

  print_constraints(ph, "*** ph ***");

  Constraint_System cs;
  cs.insert(x - 3 >= y);
  cs.insert(y >= 0);
  C_Polyhedron qh(cs);

  print_constraints(qh, "*** qh ***");

  ph.concatenate_assign(qh);

  bool ok = (ph == qh);

  print_constraints(ph, "*** after concatenate_assign(qh) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(2*A));
  gs1.insert(point(2*B));
  gs1.insert(point(2*A + 2*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point(2*A));
  gs2.insert(point(2*A + 3*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.concatenate_assign(ph2);

  C_Polyhedron known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B <= 2);
  known_result.add_constraint(C == 2);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(D <= 3);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after concatenate_assign(ph2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(A - B >= 0);

  C_Polyhedron ph2;

  C_Polyhedron known_result(ph1);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.concatenate_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.concatenate_assign(ph2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(1);
  ph1.generators();
  ph1.add_constraint(A >= 0);

  C_Polyhedron ph2(1);
  ph2.generators();
  ph2.add_constraint(A == 2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.concatenate_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B == 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.concatenate_assign(ph2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(1, EMPTY);
  ph1.add_generator(point());
  ph1.constraints();
  ph1.add_generator(ray(A));

  C_Polyhedron ph2(1, EMPTY);
  ph2.add_generator(point(2*A));
  ph2.constraints();

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.concatenate_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B == 2);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.concatenate_assign(ph2) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(1);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(-A >= -2);
  ph1.generators();
  ph1.add_generator(point(0*A));
  ph1.add_generator(point(2*A));

  C_Polyhedron ph2(1, EMPTY);
  ph2.add_generator(point(10*A));

  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.concatenate_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(-A >= -2);
  known_result.add_constraint(B == 10);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.concatenate_assign(ph2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
END_MAIN
