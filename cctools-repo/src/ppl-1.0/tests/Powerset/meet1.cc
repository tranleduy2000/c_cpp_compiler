/* Test Pointset_Powerset<PH>::meet_assign().
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

// Powerset of C polyhedra: meet_assign().
bool
test01() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  c_ps.add_disjunct(C_Polyhedron(cs));

  Pointset_Powerset<C_Polyhedron> c_ps1(1, EMPTY);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);

  c_ps.meet_assign(c_ps1);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 2);
  Pointset_Powerset<C_Polyhedron> c_ps_expected(1, EMPTY);
  c_ps_expected.add_disjunct(C_Polyhedron(cs));

  bool ok = c_ps.definitely_entails(c_ps_expected);
  bool ok1 = !c_ps_expected.definitely_entails(c_ps);

  return ok && ok1;
}

// Powerset of boxes: meet_assign().
bool
test02() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  pps_box.add_disjunct(TBox(cs));

  Pointset_Powerset<TBox> pps_box1(1, EMPTY);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);

  pps_box.meet_assign(pps_box1);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 2);
  Pointset_Powerset<TBox> pps_box_expected(1, EMPTY);
  pps_box_expected.add_disjunct(TBox(cs));

  bool ok = pps_box.definitely_entails(pps_box_expected);
  bool ok1 = !pps_box_expected.definitely_entails(pps_box);

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
