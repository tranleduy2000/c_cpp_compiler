/* Test Polyhedron::bounded_affine_preimage().
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
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(B >= 0);
  ph.add_constraint(B <= 4);
  ph.add_constraint(A - B <= 2);
  ph.add_constraint(A - B >= -2);

  print_constraints(ph, "*** ph ***");

  // A longer way of computing the bounded affine preimage below.
  C_Polyhedron known_result(ph);
  known_result.add_space_dimensions_and_embed(1);
  known_result.add_constraint(7-A <= B);
  known_result.add_constraint(B <= A+3);
  Variables_Set var_set;
  var_set.insert(B);
  known_result.remove_space_dimensions(var_set);

  ph.bounded_affine_preimage(B, 7-A, A+3);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph.bounded_affine_preimage(B, 7-A, A+3) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(B >= 0);
  ph.add_constraint(B <= 4);
  ph.add_constraint(A - B <= 2);
  ph.add_constraint(A - B >= -2);

  print_constraints(ph, "*** ph ***");

  // A longer way of computing the bounded affine preimage below.
  C_Polyhedron known_result(ph);
  known_result.add_space_dimensions_and_embed(1);
  Variable C(2);
  known_result.add_constraint(7 - 3*A + 2*C <= B);
  known_result.add_constraint(B <= C + 5*A - 3);
  Variables_Set var_set;
  var_set.insert(B);
  known_result.remove_space_dimensions(var_set);

  ph.bounded_affine_preimage(B, 7-3*A+2*B, B+5*A-3);

  bool ok = (ph == known_result);

  print_generators(ph, "*** "
		   "ph.bounded_affine_preimage(B, 7-3*A+2*B, B+5*A-3) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 4);
  ph.add_constraint(A == B);

  print_constraints(ph, "*** ph ***");

  ph.bounded_affine_preimage(A, B+5, A-3);

  C_Polyhedron known_result(2, EMPTY);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph.bounded_affine_preimage(A, B+5, A-3) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
END_MAIN
