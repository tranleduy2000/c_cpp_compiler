/* Test Pointset_Powerset<PH>::contains(),
        Pointset_Powerset<PH>::strictly_contains().
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

// Powerset of C polyhedra: contains(), strictly_contains().
bool
test01() {
  Pointset_Powerset<C_Polyhedron> ps1(1, EMPTY);

  Pointset_Powerset<C_Polyhedron> ps2(1, EMPTY);
  bool b = ps1.contains(ps2);
  bool c = ps2.contains(ps1);
  bool bs = ps1.strictly_contains(ps2);
  bool cs = ps2.strictly_contains(ps1);

  ps1.add_disjunct(C_Polyhedron(1));
  bool b1 = ps1.contains(ps2);
  bool c1 = !ps2.contains(ps1);
  bool bs1 = ps1.strictly_contains(ps2);
  bool cs1 = !ps2.strictly_contains(ps1);

  ps2.add_disjunct(C_Polyhedron(1));
  bool b2 = ps1.contains(ps2);
  bool c2 = ps2.contains(ps1);
  bool bs2 = !ps1.strictly_contains(ps2);
  bool cs2 = !ps2.strictly_contains(ps1);

  bool ok = b && c && b1 && c1 && b2 && c2;
  bool oks = bs && cs && bs1 && cs1 && bs2 && cs2;

  return ok && oks;
}

// Powerset of C polyhedra: contains(), strictly_contains().
bool
test02() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  c_ps.add_disjunct(C_Polyhedron(cs));

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 4);
  c_ps.add_disjunct(C_Polyhedron(cs));

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);
  c_ps.add_disjunct(C_Polyhedron(cs));

  Pointset_Powerset<C_Polyhedron> c_ps1(1, EMPTY);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);
  c_ps1.add_disjunct(C_Polyhedron(cs));

  bool ok = c_ps.contains(c_ps1)
    && !c_ps1.contains(c_ps)
    && c_ps.strictly_contains(c_ps1)
    && !c_ps1.strictly_contains(c_ps);

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 4);
  c_ps1.add_disjunct(C_Polyhedron(cs));

  bool ok1 = c_ps.contains(c_ps1)
    && !c_ps1.contains(c_ps)
    && !c_ps.strictly_contains(c_ps1)
    && !c_ps1.strictly_contains(c_ps);

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
