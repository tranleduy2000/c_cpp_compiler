/* Test BD_Shape::BD_Shape(const C_Polyhedron&).
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

  Constraint_System cs;
  cs.insert(A - B == 5);
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  TOctagonal_Shape os(cs);

  Grid gr(os);

  Grid known_result(2);

  known_result.refine_with_constraints(cs);

  bool ok = gr == known_result;

  print_congruences(gr, "*** gr ***");

  return ok;
}

bool
test02() {
  Variable A(0);

  Constraint_System cs;
  cs.insert(A >= 5);
  cs.insert(A <= -7);
  TOctagonal_Shape bd(cs);

  Grid gr(bd);

  Grid known_result(1, EMPTY);

  bool ok = gr == known_result;

  print_congruences(gr, "*** gr ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
