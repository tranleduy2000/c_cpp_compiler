/* Test Polyhedron::add_constrains().
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
  // For const-correctness, making a copy is required.
  Constraint_System cs1 = Constraint_System::zero_dim_empty();
  C_Polyhedron ph1;
  ph1.add_constraints(cs1);

  Constraint_System cs2;
  cs2.insert(Linear_Expression::zero() >= 7);
  C_Polyhedron ph2;
  ph2.add_constraints(cs2);

  Constraint_System cs3;
  cs3.insert(Linear_Expression::zero() >= -3);
  C_Polyhedron ph3;
  ph3.add_constraints(cs3);

  C_Polyhedron empty_result(0, EMPTY);
  C_Polyhedron univ_result;

  bool ok = (ph1 == empty_result
	     && ph2 == empty_result
	     && ph3 == univ_result);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(ph3, "*** ph3 ***");
  print_constraints(empty_result, "*** empty_result ***");
  print_constraints(univ_result, "*** univ_result ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  Constraint_System cs;

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  C_Polyhedron known_result(ph);

  ph.add_constraints(cs);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x + y >= 0);
  C_Polyhedron ph(cs1);

  print_constraints(ph, "*** ph ***");

  Linear_Expression e(1);
  Constraint_System cs2;
  cs2.insert(e == 0);
  ph.add_constraints(cs2);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.add_constraints(cs2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);

  C_Polyhedron ph(3);
  ph.add_constraint(x >= 1);

  print_constraints(ph, "*** ph ***");

  Constraint_System cs;

  C_Polyhedron computed_result(ph);

  computed_result.add_constraints(cs);

  bool ok = (computed_result == ph);

  print_constraints(computed_result, "*** after add_constraints ***");

  return ok;
}

bool
test05() {
  Variable x(0);

  C_Polyhedron ph(3, EMPTY);

  Constraint_System cs;
  cs.insert(x >= 4);

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.add_constraints(cs);

  C_Polyhedron computed_result(3, EMPTY);

  bool ok = (ph == computed_result);

  print_constraints(ph, "*** after add_constraints(cs) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);

  Constraint_System cs;
  cs.insert(x >= 4);
  cs.insert(x - y >= 0);

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.add_constraints(cs);

  C_Polyhedron known_result(3);
  known_result.add_constraint(x >= 4);
  known_result.add_constraint(x - y >= 0);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after add_constraints(cs) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);
  ph.add_constraint(y >= 1);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(y <= 0);

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.add_constraints(cs);

  C_Polyhedron known_result(3, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after add_constraints(cs) ***");

  return ok;
}

bool
test08() {
  C_Polyhedron ph;
  ph.add_constraint(Linear_Expression(-2) >= 0);

  Constraint_System cs;
  cs.insert(Linear_Expression(-1) >= 0);

  print_constraints(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.add_constraints(cs);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** after add_constraints ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(x));
  gs.insert(ray(x + y));

  C_Polyhedron ph(gs);

  Constraint_System cs;
  cs.insert(x <= 3);

  print_generators(ph, "*** ph ***");
  print_constraints(cs, "*** cs ***");

  ph.add_constraints(cs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(x - y >= 0);
  known_result.add_constraint(x <= 3);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** after add_constraints ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  Constraint_System cs;
  cs.insert(A - B >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(cs, "*** cs ***");

  ph1.add_constraints(cs);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.add_constraints(cs) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 0);
  C_Polyhedron copy_ph(ph);

  Constraint_System cs1;
  cs1.insert(A == 0);
  cs1.insert(B >= 0);
  Constraint_System cs2(cs1);

  ph.add_constraints(cs1);
  copy_ph.add_constraints(cs2);

  bool ok = (ph == copy_ph);

  print_constraints(ph, "*** after ph.add_constraints(cs1) ***");
  print_constraints(ph, "*** after copy_ph.add_constraints(cs2) ***");
  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point());
  ph1.constraints();
  ph1.add_generator(line(A + B));
  C_Polyhedron copy_ph1 = ph1;

  C_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point());
  ph2.constraints();
  ph2.add_generator(ray(A));
  ph2.add_generator(ray(B));

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  Constraint_System cs1 = ph2.constraints();
  Constraint_System cs2 = ph2.constraints();

  ph1.add_constraints(cs1);
  copy_ph1.add_constraints(cs2);

  bool ok = (ph1 == copy_ph1);

  print_constraints(ph1, "*** after add_constraints_assign ***");
  print_constraints(copy_ph1, "*** after add_constraints ***");

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
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
END_MAIN
