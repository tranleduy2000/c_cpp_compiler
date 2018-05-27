/* Test Polyhedron::map_space_dimensions().
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
  Partial_Function function;

  C_Polyhedron ph1(3);

  print_function(function, "*** function ***");
  print_constraints(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result;

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test02() {
  Partial_Function function;

  C_Polyhedron ph1(3, EMPTY);

  print_function(function, "*** function ***");
  print_constraints(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result(0, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Partial_Function function;
  function.insert(0, 2);
  function.insert(2, 0);
  function.insert(1, 1);

  Generator_System gs;
  gs.insert(point(2*C));
  gs.insert(line(A + B));
  gs.insert(ray(A + C));

  C_Polyhedron ph1(gs);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  Generator_System known_gs;
  known_gs.insert(point(2*A));
  known_gs.insert(line(C + B));
  known_gs.insert(ray(C + A));
  C_Polyhedron known_result(known_gs);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(2, 0);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A + B));
  gs.insert(ray(A - C));

  C_Polyhedron ph1(gs);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  Generator_System known_gs;
  known_gs.insert(point());
  known_gs.insert(ray(B));
  known_gs.insert(ray(B - A));
  C_Polyhedron known_result(known_gs);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(2, 0);
  function.insert(3, 2);
  function.insert(4, 1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(ray(B));

  C_Polyhedron ph1(gs);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point());

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(0, 0);
  function.insert(1, 1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A));
  gs.insert(point(B));
  gs.insert(point(A + B));

  C_Polyhedron ph1(gs);
  C_Polyhedron known_result(ph1);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);
  function.insert(2, 2);
  function.insert(3, 3);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A));
  gs.insert(point(2*B));
  gs.insert(point(A + 2*B));

  C_Polyhedron ph1(gs);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result(4, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(point(B));
  known_result.add_generator(point(2*A));
  known_result.add_generator(point(2*A + B));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Partial_Function function;
  function.insert(0, 0);
  function.insert(2, 1);
  function.insert(3, 2);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A));
  gs.insert(ray(B));
  gs.insert(ray(A + B));

  C_Polyhedron ph1(gs);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(ray(A));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test09() {
  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  C_Polyhedron ph1(3, EMPTY);

  print_function(function, "*** function ***");
  print_constraints(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.map_space_dimensions(function) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Partial_Function rotate_right;
  rotate_right.insert(0, 1);
  rotate_right.insert(1, 2);
  rotate_right.insert(2, 0);

  Partial_Function rotate_left;
  rotate_left.insert(0, 2);
  rotate_left.insert(1, 0);
  rotate_left.insert(2, 1);

  C_Polyhedron ph(3);
  ph.add_constraint(-4*x - 2*y + z >= -8);
  ph.add_constraint(-4*x + 2*y + z >= 4);
  ph.add_constraint(-2*x - y + 2*z >= -1);
  ph.add_constraint(-2*x + y + 2*z >= 5);
  ph.add_constraint(-x - y - 2*z >= -13);
  ph.add_constraint(-x - z >= -5);
  ph.add_constraint(-x >= -1);
  ph.add_constraint(-x + y - 2*z >= -7);
  ph.add_constraint(-y >= -4);
  ph.add_constraint(y >= 2);
  ph.add_constraint(x >= 0);

  print_constraints(ph, "*** ph ***");
  print_function(rotate_right, "*** rotate_right ***");
  print_function(rotate_left, "*** rotate_left ***");

  C_Polyhedron rs[4];
  rs[0] = ph;

  print_constraints(rs[0], "*** rs[0] ***");

  for (int i = 1; i <= 3; ++i) {
    rs[i] = rs[i-1];
    rs[i].map_space_dimensions(rotate_right);

    print_constraints(rs[i], "*** rs[i] ***");

  }

  C_Polyhedron ls[4];
  ls[3] = ph;

  print_constraints(ls[3], "*** ls[3] ***");

  for (int i = 2; i >= 0; --i) {
    ls[i] = ls[i+1];
    // Force generators to be up-to-date, for a change.
    (void) ls[i].generators();
    ls[i].map_space_dimensions(rotate_left);

    print_constraints(ls[i], "*** ls[i] ***");

  }

  for (int i = 0; i <= 3; ++i)
    if (rs[i] != ls[i]) {
      nout << "rs[" << i << "] != ls[" << i << "]" << endl;
      return false;
    }

  return true;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(A >= 2);
  ph.add_constraint(B >= 1);
  ph.add_constraint(C >= 0);

  Partial_Function rotate_right;
  rotate_right.insert(0, 1);
  rotate_right.insert(1, 2);
  rotate_right.insert(2, 0);

  print_constraints(ph, "*** ph ***");
  print_function(rotate_right, "*** rotate_right ***");

  ph.map_space_dimensions(rotate_right);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 2);
  known_result.add_constraint(C >= 1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.map_space_dimensions(rotate_right) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(-A + B == 0);

  Partial_Function rotate_right;
  rotate_right.insert(0, 1);
  rotate_right.insert(1, 0);

  print_constraints(ph, "*** ph ***");
  print_function(rotate_right, "*** rotate_right ***");

  ph.map_space_dimensions(rotate_right);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A == B);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** after ph.map_space_dimensions(rotate_right) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Partial_Function function;
  function.insert(0, 2);
  function.insert(1, 3);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A));
  gs.insert(point(2*B));
  gs.insert(point(A + 2*B));

  C_Polyhedron ph1(gs);

  print_function(function, "*** function ***");
  print_generators(ph1, "*** ph1 ***");

  ph1.map_space_dimensions(function);

  C_Polyhedron known_result(4, EMPTY);
  known_result.add_generator(point());
  known_result.add_generator(point(C));
  known_result.add_generator(point(2*D));
  known_result.add_generator(point(C + 2*D));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.map_space_dimensions(function) ***");

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
  // test10() only fails when using C_Polyhedron and 8 bit coefficients.
#ifdef DERIVED_TEST
  DO_TEST(test10);
#else
  DO_TEST_F8(test10);
#endif // !defined(DERIVED_TEST)
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
END_MAIN
