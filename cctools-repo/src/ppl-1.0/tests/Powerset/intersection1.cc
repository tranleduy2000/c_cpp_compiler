/* Test Pointset_Powerset<PH>::intersection_assign().
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

// Powerset of C polyhedra: intersection_assign().
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
  c_ps1.add_disjunct(C_Polyhedron(cs));

  c_ps.intersection_assign(c_ps1);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 2);
  Pointset_Powerset<C_Polyhedron> c_ps_expected(1, EMPTY);
  c_ps_expected.add_disjunct(C_Polyhedron(cs));

  bool ok = c_ps.definitely_entails(c_ps_expected);
  bool ok1 = c_ps_expected.definitely_entails(c_ps);

  return ok && ok1 && c_ps.OK() && c_ps1.OK();
}

// Powerset of C polyhedra: intersection_assign().
bool
test02() {
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
  c_ps1.add_disjunct(C_Polyhedron(cs));

  c_ps.intersection_assign(c_ps1);
  bool ok = !c_ps.empty();

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 2);
  Pointset_Powerset<C_Polyhedron> c_ps_expected(1, EMPTY);
  c_ps_expected.add_disjunct(C_Polyhedron(cs));

  bool ok1 = c_ps.definitely_entails(c_ps_expected);
  bool ok2 = c_ps_expected.definitely_entails(c_ps);

  Pointset_Powerset<C_Polyhedron> c_ps2(1, EMPTY);
  cs.clear();
  cs.insert(x == 4);
  c_ps2.add_disjunct(C_Polyhedron(cs));

  c_ps2.intersection_assign(c_ps1);
  bool ok3 = c_ps2.empty();

  return ok && ok1 && ok2 && ok3 && c_ps.OK() && c_ps1.OK() && c_ps2.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
