/* Test Octagonal_Shape::Octagonal_Shape(dimension_type, Degenerate_Element).
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
  TOctagonal_Shape oc(0, EMPTY);

  bool ok = (oc.OK() && oc.is_empty() && oc.space_dimension() == 0);

  print_constraints(oc, "*** oc ***");

  return ok;
}

bool
test02() {
  TOctagonal_Shape oc(0, UNIVERSE);

  bool ok = (oc.OK() && oc.is_universe() && oc.space_dimension() == 0);

  print_constraints(oc, "*** oc ***");

  return ok;
}

bool
test03() {
  TOctagonal_Shape oc(4, EMPTY);

  bool ok = (oc.OK() && oc.is_empty() && oc.space_dimension() == 4);

  print_constraints(oc, "*** oc ***");

  return ok;
}

bool
test04() {
  TOctagonal_Shape oc(4, UNIVERSE);

  bool ok = (oc.OK() && oc.is_universe() && oc.space_dimension() == 4);

  print_constraints(oc, "*** oc ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(4);
  oc.add_constraint(A <= -1);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B <= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C <= 2);
  oc.add_constraint(C >= 1);

  bool ok = (oc.OK() && oc.is_empty() && oc.space_dimension() == 4);

  print_constraints(oc, "*** oc ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);
  oc.add_constraint(A <= 1);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B <= 1);
  oc.add_constraint(B >= 0);
  oc.add_constraint(C <= 2);
  oc.add_constraint(C >= 1);

  bool ok = (oc.OK() && oc.space_dimension() == 3);

  print_constraints(oc, "*** oc ***");

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
END_MAIN
