/* Test Polyhedron::refine_with_congruence().
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
  Variable C(2);

  NNC_Polyhedron ph1(3);
  ph1.refine_with_congruence((A %= 0) / 2);
  ph1.refine_with_congruence((B %= 5) / 0);
  ph1.refine_with_congruence(A + B %= 6);

  print_constraints(ph1, "*** ph1 ***");

  C_Polyhedron ph2(ph1);
  ph2.refine_with_congruence((A %= 1) / 0);
  ph2.refine_with_congruence((B + C %= 4) / 5);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A == 1);
  known_result.add_constraint(B == 5);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  ph.refine_with_congruence((A %= B) / 0);

  bool ok = (ph == known_result);

  print_constraints(ph,
                    "*** after ph.refine_with_congruence((A %= B) / 0) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.refine_with_congruence((A %= 0) / 4);
  ph.refine_with_congruence((A + B %= 2) / 5);

  print_constraints(ph, "*** ph ***");

  ph.refine_with_congruence((B %= 1) / 0);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B == 1);

  bool ok = (ph == known_result);

  print_constraints(ph,
		    "*** after ph.refine_with_congruence(B >= 1) ***");
  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
