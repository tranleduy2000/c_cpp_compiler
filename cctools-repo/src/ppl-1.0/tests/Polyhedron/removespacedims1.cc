/* Removing space dimensions from a polyhedron.
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
  Generator_System gs;
  gs.insert(point(0*x + y +0*z + 2*w));
  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(y);
  to_be_removed.insert(z);
  ph.remove_space_dimensions(to_be_removed);

  Generator_System known_result_gs;
  known_result_gs.insert(point(0*x +2*y));
  C_Polyhedron known_result(known_result_gs);

  bool ok = (known_result == ph);

  print_generators(ph, "*** ph ***");
  print_generators(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Generator_System gs;

  // Creating 10 points.
  for (int i = 0; i < 10; i++) {
    Linear_Expression e;
    for (int j = 0; j < 10; j++)
      e += (10*i + j) * Variable(j);
    gs.insert(point(e));
  }

  C_Polyhedron ph(gs);

  print_generators(ph, "*** before ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(Variable(0));
  to_be_removed.insert(Variable(5));
  to_be_removed.insert(Variable(3));
  to_be_removed.insert(Variable(4));
  to_be_removed.insert(Variable(8));

  ph.remove_space_dimensions(to_be_removed);

  // Useless, but much clearer.
  gs.clear();

  Variable a(0);
  Variable b(1);
  Variable c(2);
  Variable d(3);
  Variable e(4);

  Linear_Expression expr01 = (1*a + 2*b + 6*c + 7*d + 9*e);
  Linear_Expression expr10 = 10 * (a + b + c + d + e);

  for (int i = 0; i < 10; i++) {
    Linear_Expression expr = i * expr10 + expr01;
    gs.insert(point(expr));
  }

  C_Polyhedron known_result(gs);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after ***");
  print_generators(known_result, "*** known_result ***");

  return ok;
}

bool
test03() {
  Variable y(1);
  Variable z(2);
  Variable w(6);

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(y);
  to_be_removed.insert(z);
  to_be_removed.insert(w);

  // A 10-dim space, empty polyhedron.
  C_Polyhedron ph(10, EMPTY);
  ph.remove_space_dimensions(to_be_removed);

  // A 7-dim space, empty polyhedron.
  C_Polyhedron known_result(7, EMPTY);

  bool ok = (known_result == ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Generator_System gs;
  gs.insert(point(x + y + 2*z - w));

  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  ph.remove_higher_space_dimensions(2);

  Generator_System gs_known_result;
  gs_known_result.insert(point(x + y));
  C_Polyhedron known_result(gs_known_result);

  bool ok = (ph == known_result);

  print_generators(ph, "*** after remove_higher_space_dimensions(2) ***");

  return ok;
}

bool
test05() {
  C_Polyhedron ph(4, EMPTY);

  print_constraints(ph, "*** ph ***");

  ph.remove_higher_space_dimensions(0);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph after remove_higher_space_dimensions(0) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  C_Polyhedron ph1(3);
  ph1.add_constraint(x >= 3);
  ph1.add_constraint(x - y >= 0);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2 = ph1;

  print_constraints(ph2, "*** ph2 ***");

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;
  to_be_removed.insert(y);
  to_be_removed.insert(z);
  to_be_removed.insert(x);

  ph1.remove_space_dimensions(to_be_removed);
  ph2.remove_higher_space_dimensions(0);

  bool ok = (ph1 == ph2);

  print_generators(ph1, "*** ph1 after remove_space_dimensions ***");
  print_generators(ph2, "*** ph2 after remove_higher_space_dimensions ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  C_Polyhedron ph(gs);
  ph.add_constraint(A >= 2);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;

  ph.remove_space_dimensions(to_be_removed);

  bool ok = (ph == known_result);

  print_constraints(ph,
		    "*** after ph.remove_space_dimensions(to_be_removed) ***");

  return ok;
}

bool
test08() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 3);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  ph.remove_higher_space_dimensions(2);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.remove_higher_space_dimensions(2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(2*A + B, 4));

  print_constraints(ph, "*** ph ***");

  ph.remove_higher_space_dimensions(1);

  C_Polyhedron known_result(1, EMPTY);
  known_result.add_generator(point(A, 2));

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.remove_higher_space_dimensions(1) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
#ifdef DERIVED_TEST
  DO_TEST_F8A(test02);
#else
  DO_TEST_F8(test02);
#endif // !defined(DERIVED_TEST)
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
