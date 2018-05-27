/* Test Polyhedron::add_congruences().
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

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(x));
  gs.insert(ray(x + y));

  Congruence_System cgs;
  cgs.insert(0*x %= 0);
  cgs.insert(y == 3);

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");
  print_congruences(cgs, "*** cgs ***");

  ph.add_congruences(cgs);

  C_Polyhedron known_result(2);
  known_result.add_constraint(y == 3);
  known_result.add_constraint(x - y >= 0);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** after add_congruences ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2, EMPTY);

  Congruence_System cgs;
  cgs.insert(Linear_Expression(0) %= 0);
  cgs.insert(B == 7);

  print_constraints(ph1, "*** ph1 ***");
  print_congruences(cgs, "*** cgs ***");

  ph1.add_congruences(cgs);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.add_congruences(cgs) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  Congruence_System cgs;

  print_constraints(ph, "*** ph ***");
  print_congruences(cgs, "*** cgs ***");

  C_Polyhedron known_result(ph);

  ph.add_congruences(cgs);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x + y >= 0);
  C_Polyhedron ph(cs);

  print_constraints(ph, "*** ph ***");

  Linear_Expression e(1);
  Congruence_System cgs;
  cgs.insert(e == 0);

  ph.add_congruences(cgs);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.add_congruences(cgs) ***");

  return ok;
}

bool test05() {
  C_Polyhedron ph;
  ph.add_constraint(Linear_Expression(-2) >= 0);

  print_constraints(ph, "*** ph ***");

  Congruence_System cgs;
  cgs.insert(Linear_Expression(-1) %= 0);

  print_congruences(cgs, "*** cgs ***");

  ph.add_congruences(cgs);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** after add_constraints ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);

  Congruence_System cgs;
  cgs.insert((Linear_Expression(26) %= 0) / 13);
  cgs.insert(B == 7);

  print_constraints(ph1, "*** ph1 ***");
  print_congruences(cgs, "*** cgs ***");

  ph1.add_congruences(cgs);

  C_Polyhedron known_result(cgs);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.add_congruences(cgs) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);

  Congruence_System cgs;
  cgs.insert(Linear_Expression(0) %= 0);
  cgs.insert(B == 7);

  Congruence_System cgs_copy = cgs;

  print_constraints(ph1, "*** ph1 ***");
  print_congruences(cgs, "*** cgs ***");

  ph1.add_recycled_congruences(cgs);

  C_Polyhedron known_result(cgs_copy);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.add_congruences(cgs) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);

  Congruence_System cgs;
  cgs.insert((Linear_Expression(18) %= 3) / 5);
  cgs.insert(A == 0);
  cgs.insert(A + B == 7);
  cgs.insert(B == 7);

  Congruence_System cgs_copy = cgs;

  print_constraints(ph1, "*** ph1 ***");
  print_congruences(cgs, "*** cgs ***");

  ph1.add_recycled_congruences(cgs);

  C_Polyhedron known_result(cgs_copy);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.add_congruences(cgs) ***");

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

