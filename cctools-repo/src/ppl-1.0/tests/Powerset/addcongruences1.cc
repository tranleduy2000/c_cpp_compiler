/* Test Pointset_Powerset<PH>::add_congruence().
        Pointset_Powerset<PH>::add_congruences().
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

// Powerset of C polyhedra: add_congruence().
bool
test01() {
  Variable x(0);
  Congruence cg = (Linear_Expression(0) %= 0);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  ps.add_disjunct(C_Polyhedron(1));
  ps.add_congruence(cg);
  Congruence cg1 = ((Linear_Expression(25) %= 1) / 2);
  ps.add_congruence(cg1);
  bool ok = !ps.is_empty() && ps.OK();
  return ok;
}

// Powerset of C polyhedra: add_congruences().
bool
test02() {
  Variable x(0);
  Congruence_System cgs;
  cgs.insert((x %= 0) / 0);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  ps.add_disjunct(C_Polyhedron(1));
  ps.add_congruences(cgs);
  cgs.insert((x %= 0) / 0);
  ps.add_congruences(cgs);
  bool ok = !ps.is_empty();
  cgs.insert((x %= 0) / 0);
  cgs.insert((x %= 1) / 0);
  ps.add_congruences(cgs);
  ok &= ps.is_empty() && ps.OK();

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
