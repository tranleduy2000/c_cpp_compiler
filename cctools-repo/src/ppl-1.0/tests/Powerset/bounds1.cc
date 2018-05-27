/* Test Pointset_Powerset<PH>::bounds_from_above(),
        Pointset_Powerset<PH>::bounds_from_below().
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

// Powerset of C polyhedra: bounds_from_above(), bounds_from_below().
bool
test01() {
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);
  Linear_Expression LE;
  bool ok1 = ps.bounds_from_above(LE);
  bool ok2 = ps.bounds_from_below(LE);

  if (!ok1 || !ok2)
    return false;

  ps.add_disjunct(C_Polyhedron(0));
  ok1 = ps.bounds_from_above(LE);
  ok2 = ps.bounds_from_below(LE);

  return ok1 && ok2;
}

// Powerset of NNC polyhedra: bounds_from_above(), bounds_from_below().
bool
test02() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<NNC_Polyhedron> ps(1, EMPTY);
  Linear_Expression LE = x;

  cs.clear();
  cs.insert(x > 0);
  cs.insert(x <= 1);
  ps.add_disjunct(NNC_Polyhedron(cs));

  cs.clear();
  cs.insert(x > 1);
  ps.add_disjunct(NNC_Polyhedron(cs));

  bool ok1 = !ps.bounds_from_above(LE);
  bool ok2 = ps.bounds_from_below(LE);

  return ok1 && ok2;
}

// Powerset of C polyhedra: bounds_from_above(), bounds_from_below().
bool
test03() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  Linear_Expression LE = x;

  bool ok1 = ps.bounds_from_above(LE);
  bool ok2 = ps.bounds_from_below(LE);

  if (!ok1 || !ok2)
    return false;

  ps.add_disjunct(C_Polyhedron(1));

  ok1 = !ps.bounds_from_above(LE);
  ok2 = !ps.bounds_from_below(LE);

  return ok1 && ok2;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
