/* Test Polyhedron::add_congruence().
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

  C_Polyhedron ph(2);
  ph.add_congruence(Linear_Expression(0) %= 0);
  ph.add_congruence((B == 5) / 0);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B == 5);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);

  ph.add_congruence((0*(A + B) %= 0) / 4);
  ph.add_congruence((A == -1) / 0);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A == -1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  ph.add_congruence((A - B == 0) / 0);
  ph.add_congruence((Linear_Expression(0) %= 1) / 2);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph after adding congruences ***");

  return ok;
}

bool
test04() {
  C_Polyhedron ph(0);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(0);

  ph.add_congruence((Linear_Expression::zero() %= 0) / 2);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph after adding congruence ***");

  return ok;
}

bool
test05() {
  C_Polyhedron ph(0);

  print_constraints(ph, "*** ph ***");

  ph.add_congruence((Linear_Expression::zero() %= 1) / 0);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph after adding congruence ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_congruence(0*A %= 0);
  ph.add_congruence((B == 5) / 0);
  bool b = !ph.is_empty();

  C_Polyhedron known_result(2);
  known_result.add_constraint(B == 5);

  bool ok = b && (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);

  ph.add_congruence((Linear_Expression(0) %= 8) / 4);
  ph.add_congruence((A == -1) / 0);
  bool b = !ph.is_empty();

  C_Polyhedron known_result(2);
  known_result.add_constraint(A == -1);

  bool ok = b && (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  ph.add_congruence((A - B == 0) / 0);
  ph.add_congruence((Linear_Expression(0) %= 1) / 2);
  bool b = !ph.is_empty();

  bool ok = !b && (ph == known_result);

  print_constraints(ph, "*** ph after adding congruences ***");

  return ok;
}

bool
test09() {
  C_Polyhedron ph(0);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(0);

  ph.add_congruence((Linear_Expression::zero() %= 0) / 2);
  bool b = !ph.is_empty();

  bool ok = b && (ph == known_result);

  print_constraints(ph, "*** ph after adding congruence ***");

  return ok;
}

bool
test10() {
  C_Polyhedron ph(0);

  print_constraints(ph, "*** ph ***");

  ph.add_congruence((Linear_Expression::zero() %= 1) / 0);
  bool b = !ph.is_empty();

  C_Polyhedron known_result(0, EMPTY);

  bool ok = !b && (ph == known_result);

  print_constraints(ph, "*** ph after adding congruence ***");

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
END_MAIN
