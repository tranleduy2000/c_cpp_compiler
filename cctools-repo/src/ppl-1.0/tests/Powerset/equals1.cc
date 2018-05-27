/* Test Pointset_Powerset<PH>::equals(),
        Pointset_Powerset<PH>::geometrically_equals().
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

// geometrically_equals().
bool
test01() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1);
  Constraint_System cs;
  cs.insert(x >= 5);
  cs.insert(x <= 3);
  pps_box.add_constraints(cs);

  Pointset_Powerset<TBox> pps_box1(1, EMPTY);

  // pps_box.ascii_dump();
  // pps_box1.ascii_dump();

  bool ok = pps_box.geometrically_equals(pps_box1);
  bool ok1 = pps_box.geometrically_equals(pps_box1);

  return ok && ok1;
}

// geometrically_equals().
bool
test02() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1);
  Constraint_System cs;
  cs.insert(x >= 5);
  cs.insert(x >= 8);
  pps_box.add_constraints(cs);

  Pointset_Powerset<TBox> pps_box1(1);
  cs.clear();
  cs.insert(x >= 8);
  pps_box1.add_constraints(cs);

  // pps_box.ascii_dump();
  // pps_box1.ascii_dump();

  bool ok = pps_box.geometrically_equals(pps_box1);
  bool ok1 = pps_box.geometrically_equals(pps_box1);

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
