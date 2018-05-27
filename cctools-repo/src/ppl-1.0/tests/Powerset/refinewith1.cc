/* Test Pointset_Powerset<PH>::refine_with_constraint().
        Pointset_Powerset<PH>::refine_with_constraints().
        Pointset_Powerset<PH>::refine_with_congruence().
        Pointset_Powerset<PH>::refine_with_congruences().
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

// Powerset of C polyhedra: refine_with_constraint(), refine_with_congruence().
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  C_Polyhedron ph1(4);
  ph1.add_constraint(A == 1);
  ph1.add_constraint(C >= 0);
  ph1.add_constraint(B >= 0);

  C_Polyhedron ph2(4);
  ph2.add_constraint(A <= 2);
  ph2.add_constraint(B >= 2);
  Pointset_Powerset<C_Polyhedron> pps(4, EMPTY);

  pps.add_disjunct(ph1);
  pps.add_disjunct(ph2);

  Pointset_Powerset<C_Polyhedron> known_pps(pps);

  pps.refine_with_constraint(C == 0);
  pps.refine_with_constraint(D < 0);
  pps.refine_with_congruence(B + D %= 2);
  pps.refine_with_congruence((B - D %= 2) / 0);


  known_pps.add_constraint(C == 0);
  known_pps.add_constraint(A <= 2);
  known_pps.add_constraint(B <= 2);
  known_pps.add_constraint(B - D == 2);


  bool ok = (pps == known_pps);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = pps.begin();
  C_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");
  i++;
  C_Polyhedron phi1 = i->pointset();
  print_constraints(phi1, "*** phi1 ***");

  return ok;
}

// Powerset of C polyhedra: refine_with_constraints(),
// refine_with_congruences().
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  C_Polyhedron ph1(4);
  ph1.add_constraint(A == 1);
  ph1.add_constraint(C >= 0);

  C_Polyhedron ph2(4);
  ph2.add_constraint(A <= 2);
  Pointset_Powerset<C_Polyhedron> pps(4, EMPTY);

  pps.add_disjunct(ph1);
  pps.add_disjunct(ph2);

  Pointset_Powerset<C_Polyhedron> known_pps(pps);

  Constraint_System cs;
  cs.insert(A + 2*B >= 0);
  cs.insert(B - C < 3);
  cs.insert(D == 1);

  Congruence_System cgs;
  cgs.insert(A + B %= 0);
  cgs.insert((C %= 2) / 0);

  pps.refine_with_constraints(cs);
  pps.refine_with_congruences(cgs);

  known_pps.add_constraint(C == 2);
  known_pps.add_constraint(D == 1);
  known_pps.add_constraint(A + 2*B >= 0);
  known_pps.add_constraint(B - C <= 3);

  bool ok = (pps == known_pps);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = pps.begin();
  C_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Powerset of C polyhedra: refine_with_constraints(),
// refine_with_congruences() make the powerset empty.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  C_Polyhedron ph1(4);
  ph1.add_constraint(A == 1);
  ph1.add_constraint(C >= 0);

  C_Polyhedron ph2(4);
  ph2.add_constraint(A <= 2);
  Pointset_Powerset<C_Polyhedron> pps(4, EMPTY);

  pps.add_disjunct(ph1);
  pps.add_disjunct(ph2);

  Constraint_System cs;
  cs.insert(A + 2*B >= 0);
  cs.insert(B - C < 3);
  cs.insert(A > 1);

  Congruence_System cgs;
  cgs.insert(A + B %= 0);
  cgs.insert((B - C %= 4) / 0);

  pps.refine_with_constraints(cs);
  pps.refine_with_congruences(cgs);

  Pointset_Powerset<C_Polyhedron> known_pps(4, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Powerset of NNC polyhedra: refine_with_constraints(),
// refine_with_congruences().
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  NNC_Polyhedron ph1(4);
  ph1.add_constraint(A == 1);
  ph1.add_constraint(C >= 0);
  ph1.add_constraint(B >= 0);

  NNC_Polyhedron ph2(4);
  ph2.add_constraint(A <= 2);
  ph2.add_constraint(B >= 2);
  Pointset_Powerset<NNC_Polyhedron> pps(4, EMPTY);

  pps.add_disjunct(ph1);
  pps.add_disjunct(ph2);

  Pointset_Powerset<NNC_Polyhedron> known_pps(pps);

  Constraint_System cs;
  cs.insert(A + 2*B >= 0);
  cs.insert(B - C < 3);
  cs.insert(D == 1);

  Congruence_System cgs;
  cgs.insert(A + B %= 0);
  cgs.insert((C %= 2) / 0);

  pps.refine_with_constraints(cs);
  pps.refine_with_congruences(cgs);

  known_pps.add_constraint(C == 2);
  known_pps.add_constraint(D == 1);
  known_pps.add_constraint(A + 2*B >= 0);
  known_pps.add_constraint(B - C < 3);

  bool ok = (pps == known_pps);

  Pointset_Powerset<NNC_Polyhedron>::const_iterator i = pps.begin();
  NNC_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");
  i++;
  NNC_Polyhedron phi1 = i->pointset();
  print_constraints(phi1, "*** phi1 ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
