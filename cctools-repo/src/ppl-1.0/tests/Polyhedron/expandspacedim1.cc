/* Test Polyhedron::expand_space_dimension().
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

// Test with a universe polyhedron.
bool
test01() {
  Variable A(0);

  C_Polyhedron ph1(3);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 1);

  C_Polyhedron known_result(4);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.expand_space_dimension(A, 1) ***");

  return ok;
}

// Test with an empty polyhedron.
bool
test02() {
  //  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(3, EMPTY);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(B, 1);

  C_Polyhedron known_result(4, EMPTY);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.expand_space_dimension(B, 1) ***");

  return ok;
}

// Test trivial expansion.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A + B <= 2);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 0);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A + B <= 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.expand_space_dimension(A, 0) ***");

  return ok;
}

// Test with given generators.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(A));
  ph1.add_generator(point(A + B));
  ph1.add_generator(point(B));

  print_generators(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 1);

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point(A + C));
  known_result.add_generator(point(A + B));
  known_result.add_generator(point(A + B + C));
  known_result.add_generator(point(B));
  known_result.add_generator(point(B + C));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.expand_space_dimension(A, 1) ***");

  return ok;
}

// Test with given constraints.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A + B <= 2);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 1);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A + B <= 2);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(C + B <= 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.expand_space_dimension(A, 1) ***");

  return ok;
}

// Test using constraints expanding 2 dimensions.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A + B <= 2);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 2);

  C_Polyhedron known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A + B <= 2);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(C + B <= 2);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(D + B <= 2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.expand_space_dimension(A, 2) ***");

  return ok;
}

// Test using constraints with equality constraint.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  C_Polyhedron ph1(3);
  ph1.add_constraint(A <= 1);
  ph1.add_constraint(C == 1);
  ph1.add_constraint(A + B >= 1);
  ph1.add_constraint(B <= 1);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 1);
  ph1.expand_space_dimension(C, 1);

  C_Polyhedron known_result(5);
  known_result.add_constraint(A <= 1);
  known_result.add_constraint(A + B >= 1);
  known_result.add_constraint(C == 1);
  known_result.add_constraint(E == 1);
  known_result.add_constraint(B <= 1);
  known_result.add_constraint(D <= 1);
  known_result.add_constraint(D + B >= 1);

  bool ok = (ph1 == known_result);

  print_constraints(ph1,
		    "*** after ph1.expand_space_dimension(A, 1);"
		    " ph1.expand_space_dimension(C, 1) ***");

  return ok;
}

// Test as given in [GopanDMDRS04] on page 519.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(A + 2*B));
  ph1.add_generator(point(A + 3*B));
  ph1.add_generator(point(A + 4*B));

  print_generators(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(B, 1);

  C_Polyhedron known_result(3, EMPTY);
  known_result.add_generator(point(A + 2*B + 2*C));
  known_result.add_generator(point(A + 2*B + 3*C));
  known_result.add_generator(point(A + 2*B + 4*C));
  known_result.add_generator(point(A + 3*B + 2*C));
  known_result.add_generator(point(A + 3*B + 3*C));
  known_result.add_generator(point(A + 3*B + 4*C));
  known_result.add_generator(point(A + 4*B + 2*C));
  known_result.add_generator(point(A + 4*B + 3*C));
  known_result.add_generator(point(A + 4*B + 4*C));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.expand_space_dimension(A, 2) ***");

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
