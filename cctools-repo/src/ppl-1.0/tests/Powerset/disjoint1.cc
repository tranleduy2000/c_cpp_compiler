/* Test Pointset_Powerset<PH>::is_disjoint_from().
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

// Powerset of C polyhedra: is_disjoint_from().
bool
test01() {
  Pointset_Powerset<C_Polyhedron> ps1(0, EMPTY);

  Pointset_Powerset<C_Polyhedron> ps2(0, EMPTY);
  bool b = ps1.is_disjoint_from(ps2);
  bool c = ps2.is_disjoint_from(ps1);

  ps1.add_disjunct(C_Polyhedron(0));
  bool b1 = ps1.is_disjoint_from(ps2);
  bool c1 = ps2.is_disjoint_from(ps1);

  ps2.add_disjunct(C_Polyhedron(0));
  bool b2 = !ps1.is_disjoint_from(ps2);
  bool c2 = !ps2.is_disjoint_from(ps1);

  return b && c && b1 && c1 && b2 && c2;
}

// Powerset of NNC polyhedra: is_disjoint_from().
bool
test02() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<NNC_Polyhedron> ps1(1, EMPTY);

  cs.clear();
  cs.insert(x > 0);
  cs.insert(x <= 1);
  ps1.add_disjunct(NNC_Polyhedron(cs));

  cs.clear();
  cs.insert(x == 2);
  ps1.add_disjunct(NNC_Polyhedron(cs));

  Pointset_Powerset<NNC_Polyhedron> ps2(1, EMPTY);

  cs.clear();
  cs.insert(x > 2);
  cs.insert(x <= 6);
  ps2.add_disjunct(NNC_Polyhedron(cs));

  bool b = ps1.is_disjoint_from(ps2);
  bool c = ps2.is_disjoint_from(ps1);

  cs.clear();
  cs.insert(x >= 2);
  ps2.add_disjunct(NNC_Polyhedron(cs));

  bool b1 = !ps1.is_disjoint_from(ps2);
  bool c1 = !ps2.is_disjoint_from(ps1);

  return b && c && b1 && c1;
}

// Powerset of C polyhedra: is_disjoint_from().
bool
test03() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<C_Polyhedron> ps1(1, EMPTY);

  Pointset_Powerset<C_Polyhedron> ps2(1, EMPTY);
  bool b = ps1.is_disjoint_from(ps2);
  bool c = ps2.is_disjoint_from(ps1);

  ps1.add_disjunct(C_Polyhedron(1));

  bool b1 = ps1.is_disjoint_from(ps2);
  bool c1 = ps2.is_disjoint_from(ps1);

  cs.clear();
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  ps2.add_disjunct(C_Polyhedron(cs));

  bool b2 = !ps1.is_disjoint_from(ps2);
  bool c2 = !ps2.is_disjoint_from(ps1);

  return b && c && b1 && c1 && b2 && c2;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
