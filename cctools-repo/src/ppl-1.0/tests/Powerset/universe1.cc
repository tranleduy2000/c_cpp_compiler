/* Test Pointset_Powerset<PH>::is_universe().
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

// Powerset of C polyhedra: is_universe().
bool
test01() {
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);
  bool b = !ps.is_universe();

  ps.add_disjunct(C_Polyhedron(0));
  bool b1 = ps.is_universe();
  return b && b1;
}

// Powerset of NNC polyhedra: is_universe().
bool
test02() {
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

  bool b = !ps.is_universe();
  return b;
}

// Powerset of C polyhedra: is_universe().
bool
test03() {
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  bool b = !ps.is_universe();

  ps.add_disjunct(C_Polyhedron(1));

  bool b1 = ps.is_universe();
  return b && b1;
}

// Powerset of grids: is_universe().
bool
test04() {
  Variable A(0);
  Variable B(1);
  Pointset_Powerset<Grid> ps(2, EMPTY);
  bool b0 = !ps.is_universe();

  Grid g(2);
  g.add_congruence((A %= 0) / 2);
  ps.add_disjunct(g);
  bool b1 = !ps.is_universe();

  g = Grid(2);
  g.add_congruence((A + B %= 0) / 2);
  ps.add_disjunct(g);
  bool b2 = !ps.is_universe();

  g = Grid(2);
  ps.add_disjunct(g);
  bool b3 = ps.is_universe();

  return b0 && b1 && b2 && b3;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
