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

// Test using constraints for NNC polyhedron.
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A - B > 2);
  ph1.add_constraint(A + 2*B < 6);
  ph1.add_constraint(B < 6);

  print_constraints(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(B, 2);

  NNC_Polyhedron known_result(4);
  known_result.add_constraint(A - B > 2);
  known_result.add_constraint(A + 2*B < 6);
  known_result.add_constraint(B < 6);
  known_result.add_constraint(A - C > 2);
  known_result.add_constraint(A + 2*C < 6);
  known_result.add_constraint(C < 6);
  known_result.add_constraint(A - D > 2);
  known_result.add_constraint(A + 2*D < 6);
  known_result.add_constraint(D < 6);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.expand_space_dimension(B, 2) ***");

  return ok;
}

// Test using generators for NNC polyhedron.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  NNC_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(A));
  ph1.add_generator(closure_point(A + B));
  ph1.add_generator(ray(A - B));

  print_generators(ph1, "*** ph1 ***");

  ph1.expand_space_dimension(A, 2);

  NNC_Polyhedron known_result(4, EMPTY);
  known_result.add_generator(point(A + C + D));
  known_result.add_generator(ray(A -B + C + D));
  known_result.add_generator(closure_point(A + C + 2*D));
  known_result.add_generator(closure_point(A + 2*C + D));
  known_result.add_generator(closure_point(A + 2*C + 2*D));
  known_result.add_generator(closure_point(A + B + C + D));
  known_result.add_generator(closure_point(2*A + C + D));
  known_result.add_generator(closure_point(2*A + C + 2*D));
  known_result.add_generator(closure_point(2*A + 2*C + D));

  bool ok = (ph1 == known_result);

  print_generators(ph1, "*** after ph1.expand_space_dimension(A, 2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
