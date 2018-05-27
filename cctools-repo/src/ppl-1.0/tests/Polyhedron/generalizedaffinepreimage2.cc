/* Test Polyhedron::generalized_affine_preimage().
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

  NNC_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(A + B));
  ph.add_generator(closure_point(2*A));
  ph.add_generator(closure_point(2*A + 2*B));
  ph.add_generator(closure_point(3*A + B));

  print_constraints(ph, "*** ph ***");

  // A longer way of computing the generalized affine preimage below.
  NNC_Polyhedron known_result(ph);
  known_result.add_space_dimensions_and_embed(1);
  Variable C(2);
  known_result.add_constraint(B < C+2);
  Variables_Set vset;
  vset.insert(B);
  known_result.remove_space_dimensions(vset);

  ph.generalized_affine_preimage(B, LESS_THAN, B+2);

  bool ok = (ph == known_result);

  print_generators(ph, "*** ph after "
		   "ph.generalized_affine_preimage(B, LESS_THAN, B+2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
