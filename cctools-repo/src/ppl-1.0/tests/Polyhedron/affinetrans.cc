/* Use of the functions affine_image and affine_preimage.
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
  gs.insert(point(0*x + 0*y));
  gs.insert(point(0*x + 3*y));
  gs.insert(point(3*x + 0*y));
  gs.insert(point(3*x + 3*y));
  C_Polyhedron ph(gs);

  Linear_Expression expr = x + 4;

  C_Polyhedron p1(ph);

  print_generators(p1, "*** p1 ***");

  p1.affine_image(x, expr);

  C_Polyhedron p2(ph);

  print_generators(p2, "*** p2 ***");

  p2.affine_preimage(x, expr);

  Generator_System gs1_known_result;
  gs1_known_result.insert(point(4*x + 0*y));
  gs1_known_result.insert(point(4*x + 3*y));
  gs1_known_result.insert(point(7*x + 0*y));
  gs1_known_result.insert(point(7*x + 3*y));
  C_Polyhedron p1_known_result(gs1_known_result);

  Generator_System gs2_known_result;
  gs2_known_result.insert(point(-4*x + 0*y));
  gs2_known_result.insert(point(-4*x + 3*y));
  gs2_known_result.insert(point(-1*x + 0*y));
  gs2_known_result.insert(point(-1*x + 3*y));
  C_Polyhedron p2_known_result(gs2_known_result);

  bool ok = (p1 == p1_known_result && p2 == p2_known_result);

  print_generators(p1 ,"*** p1 ***");
  print_generators(p1_known_result, "*** p1_known_result ***");
  print_generators(p2 ,"*** p2 ***");
  print_generators(p2_known_result, "*** p2_known_result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
