/* Test Pointset_Powerset<PH>::size().
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

// Powerset of C polyhedra: size().
bool
test01() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  c_ps.add_disjunct(C_Polyhedron(cs));

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);
  c_ps.add_disjunct(C_Polyhedron(cs));

  bool ok = (c_ps.size() == 2);

  return ok;
}

// Powerset of boxes: size().
bool
test02() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  pps_box.add_disjunct(TBox(cs));

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);
  pps_box.add_disjunct(TBox(cs));

  bool ok = (pps_box.size() == 2);

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
