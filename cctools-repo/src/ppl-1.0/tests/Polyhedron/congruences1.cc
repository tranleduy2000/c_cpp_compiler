/* Test C_Polyhedron(Congruence_System&).
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

// Empty polyhedron.
bool
test01() {
  C_Polyhedron ph1(7, EMPTY);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.congruences();

  C_Polyhedron ph2(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Universe polyhedron.
bool
test02() {
  C_Polyhedron ph1(7);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.congruences();

  C_Polyhedron ph2(7);
  ph2.add_congruences(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Zero dimension empty polyhedron.
bool
test03() {
  C_Polyhedron ph1(0, EMPTY);

  Congruence_System cgs = ph1.congruences();

  C_Polyhedron known_ph(Congruence_System::zero_dim_empty());

  bool ok = (ph1 == known_ph);

  print_constraints(ph1, "*** ph1 ***");

  return ok;
}

// Zero dimension universe polyhedron.
bool
test04() {
  C_Polyhedron ph1(0);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.congruences();

  C_Polyhedron ph2(0);
  ph2.add_congruences(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Polyhedron in 3D.
bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(3);
  ph1.add_constraint(A + B >= 3);
  ph1.add_constraint(A == 0);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.congruences();

  C_Polyhedron ph2(3);
  ph2.add_congruences(cgs);
  ph2.add_constraint(A + B >= 3);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Polyhedron in 3D with implied equality.
bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(3);
  ph1.add_constraint(A + B >= 3);
  ph1.add_constraint(A + B <= 3);
  ph1.add_constraint(A == 0);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.congruences();

  C_Polyhedron ph2(3);
  ph2.add_congruences(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Empty polyhedron.
bool
test07() {
  C_Polyhedron ph1(7, EMPTY);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.minimized_congruences();

  C_Polyhedron ph2(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Universe polyhedron.
bool
test08() {
  C_Polyhedron ph1(7);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.minimized_congruences();

  C_Polyhedron ph2(7);
  ph2.add_congruences(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Zero dimension empty polyhedron.
bool
test09() {
  C_Polyhedron ph1(0, EMPTY);

  Congruence_System cgs = ph1.minimized_congruences();

  C_Polyhedron known_ph(Congruence_System::zero_dim_empty());

  bool ok = (ph1 == known_ph);

  print_constraints(ph1, "*** ph1 ***");

  return ok;
}

// Zero dimension universe polyhedron.
bool
test10() {
  C_Polyhedron ph1(0);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.minimized_congruences();

  C_Polyhedron ph2(0);
  ph2.add_congruences(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Polyhedron in 3D.
bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(3);
  ph1.add_constraint(A + B >= 3);
  ph1.add_constraint(A == 0);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.minimized_congruences();

  C_Polyhedron ph2(3);
  ph2.add_congruences(cgs);
  ph2.add_constraint(A + B >= 3);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

// Polyhedron in 3D with implied equality.
bool
test12() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(3);
  ph1.add_constraint(A + B >= 3);
  ph1.add_constraint(A + B <= 3);
  ph1.add_constraint(A == 0);

  C_Polyhedron known_ph = ph1;

  Congruence_System cgs = ph1.minimized_congruences();

  C_Polyhedron ph2(3);
  ph2.add_congruences(cgs);

  bool ok = (ph2 == known_ph);

  print_congruences(cgs, "*** cgs ***");
  print_constraints(ph2, "*** ph2(cgs) ***");

  return ok;
}

bool test13() {
  Variable x(0);
  Variable y(1);

  Congruence_System cgs;
  cgs.insert(0*x %= 3);
  cgs.insert(y == 3);

  C_Polyhedron ph(cgs);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(2);
  known_result.add_constraint(y == 3);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after add_congruences ***");

  return ok;
}

bool test14() {
  Variable x(0);
  Variable y(1);

  Congruence_System cgs;
  cgs.insert((2*y %= 2) / 3);

  Constraint_System cs(cgs);
  print_constraints(cs, "*** cs ***");

  cs.insert(0*x > -1);
  print_constraints(cs, "*** cs.insert(0*x > -1) ***");

  C_Polyhedron ph(cs);

  bool ok = ph.is_universe();

  print_constraints(ph, "*** ph(cs) ***");

  return ok;
}

// A similar test to the previous test but with 0 dimensions.
bool test15() {
  Variable x(0);

  Congruence_System cgs;

  Constraint_System cs(cgs);
  print_constraints(cs, "*** cs ***");

  cs.insert(0*x > -1);
  print_constraints(cs, "*** cs.insert(0*x > -1) ***");

  C_Polyhedron ph(cs);

  bool ok = ph.is_universe();

  print_constraints(ph, "*** ph(cs) ***");

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
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
END_MAIN
