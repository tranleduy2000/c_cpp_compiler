/* Test Pointset_Powerset<PH>::
          Pointset_Powerset(dimension_type, Degenerate_Element).
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
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);

  bool ok = (ps.OK() && ps.is_empty() && ps.space_dimension() == 0);

  return ok;
}

bool
test02() {
  Pointset_Powerset<C_Polyhedron> ps(0, UNIVERSE);

  bool ok = (ps.OK() && ps.is_universe() && ps.space_dimension() == 0);

  return ok;
}

bool
test03() {
  Pointset_Powerset<C_Polyhedron> ps(4, EMPTY);

  bool ok = (ps.OK() && ps.is_empty() && ps.space_dimension() == 4);

  return ok;
}

bool
test04() {
  Pointset_Powerset<C_Polyhedron> ps(4, UNIVERSE);

  bool ok = (ps.OK() && ps.is_universe() && ps.space_dimension() == 4);

  return ok;
}

bool
test05() {
  Pointset_Powerset<C_Polyhedron> ps(4);

  bool ok = (ps.OK() && ps.is_universe() && ps.space_dimension() == 4);

  return ok;
}

// Construct powerset from zero dimension empty constraint system.
bool
test06() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<TBox> ps(cs);

  print_constraints(ps, "*** ps ***");

  return ps.OK();
}

bool
test07() {
  Pointset_Powerset<TBox> pps_box(1, EMPTY);
  bool ok = (pps_box.space_dimension() == 1);
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
END_MAIN
