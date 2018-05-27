/* Test Pointset_Powerset<PH>::is_empty(),
        Pointset_Powerset<PH>::empty().
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

// Powerset of C polyhedra: empty().
bool
test01() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  c_ps.add_disjunct(C_Polyhedron(cs));

  Pointset_Powerset<C_Polyhedron> c_ps1;
  c_ps1 = c_ps;

  bool ok = !c_ps.empty();
  return ok;
}

// Powerset of C polyhedra: empty().
bool
test02() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  c_ps.add_disjunct(C_Polyhedron(cs));

  Pointset_Powerset<C_Polyhedron> c_ps1(1, EMPTY);
  swap(c_ps, c_ps1);

  bool ok = (c_ps.empty() && !c_ps1.empty());
  return ok;
}

// Powerset of C polyhedra: is_empty().
bool
test03() {
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);
  bool b = ps.is_empty();

  ps.add_disjunct(C_Polyhedron(0));
  bool b1 = !ps.is_empty();
  return b && b1;
}

// Powerset of NNC polyhedra: is_empty().
bool
test04() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<NNC_Polyhedron> ps(1, EMPTY);

  cs.clear();
  cs.insert(x > 0);
  cs.insert(x <= 1);
  ps.add_disjunct(NNC_Polyhedron(cs));

  cs.clear();
  cs.insert(x >= 0);
  cs.insert(x < 1);
  ps.add_disjunct(NNC_Polyhedron(cs));

  bool b = !ps.is_empty();
  return b;
}

// Powerset of boxes: !empty().
bool
test05() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  pps_box.add_disjunct(TBox(cs));

  Pointset_Powerset<TBox> pps_box1;
  pps_box1 = pps_box;

  bool ok = !pps_box.empty();
  return ok;
}

// Powerset of boxes: empty().
bool
test06() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  pps_box.add_disjunct(TBox(cs));

  Pointset_Powerset<TBox> pps_box1(1, EMPTY);
  swap(pps_box, pps_box1);

  bool ok = (pps_box.empty() && !pps_box1.empty());
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
