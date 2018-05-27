/* Test Pointset_Powerset<PH>::Pointset_Powerset(C_Polyhedron),
        Pointset_Powerset<PH>::Pointset_Powerset(NNC_Polyhedron).
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

// Constructs the powerset of polyhedra from an empty polyhedron.
bool
test01() {
  C_Polyhedron c_ph(0, EMPTY);
  Pointset_Powerset<C_Polyhedron> c_ps(c_ph);

  bool ok = (c_ps.OK() && c_ps.is_empty() && c_ps.space_dimension() == 0);

  NNC_Polyhedron nnc_ph(0, EMPTY);
  Pointset_Powerset<NNC_Polyhedron> nnc_ps(nnc_ph);

  ok = ok
    && (nnc_ps.OK() && nnc_ps.is_empty() && nnc_ps.space_dimension() == 0);

  return ok;
}

// Constructs the powerset of polyhedra from a closed polyhedron.
bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron c_ph(4);
  c_ph.add_constraint(x <= 2);
  c_ph.add_constraint(z == 1);
  NNC_Polyhedron nnc_ph(c_ph);

  Pointset_Powerset<C_Polyhedron> c_pps1(c_ph);
  Pointset_Powerset<C_Polyhedron> c_pps2(4, EMPTY);
  c_pps2.add_disjunct(c_ph);

  Pointset_Powerset<NNC_Polyhedron> nnc_pps1(c_ph);
  Pointset_Powerset<NNC_Polyhedron> nnc_pps2(4, EMPTY);
  nnc_pps2.add_disjunct(nnc_ph);

  bool ok = (c_pps1 == c_pps2 && nnc_pps1 == nnc_pps2);

  Pointset_Powerset<C_Polyhedron>::const_iterator c_i = c_pps1.begin();
  C_Polyhedron c_phi = c_i->pointset();
  print_constraints(c_phi, "*** c_phi ***");
  Pointset_Powerset<NNC_Polyhedron>::const_iterator nnc_i = nnc_pps1.begin();
  NNC_Polyhedron nnc_phi = nnc_i->pointset();
  print_constraints(nnc_phi, "*** nnc_phi ***");

  return ok && c_pps1.OK() && nnc_pps1.OK();
}

// Constructs the powerset of polyhedra from an nnc polyhedron.
bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  NNC_Polyhedron nnc_ph(4);
  nnc_ph.add_constraint(x <= 2);
  nnc_ph.add_constraint(z == 1);
  C_Polyhedron c_ph(nnc_ph);

  Pointset_Powerset<C_Polyhedron> c_pps1(nnc_ph);
  Pointset_Powerset<C_Polyhedron> c_pps2(4, EMPTY);
  c_pps2.add_disjunct(c_ph);

  Pointset_Powerset<NNC_Polyhedron> nnc_pps1(nnc_ph);
  Pointset_Powerset<NNC_Polyhedron> nnc_pps2(4, EMPTY);
  nnc_pps2.add_disjunct(nnc_ph);

  bool ok = (c_pps1 == c_pps2 && nnc_pps1 == nnc_pps2);

  Pointset_Powerset<C_Polyhedron>::const_iterator c_i = c_pps1.begin();
  C_Polyhedron c_phi = c_i->pointset();
  print_constraints(c_phi, "*** c_phi ***");
  Pointset_Powerset<NNC_Polyhedron>::const_iterator nnc_i = nnc_pps1.begin();
  NNC_Polyhedron nnc_phi = nnc_i->pointset();
  print_constraints(nnc_phi, "*** nnc_phi ***");

  return ok && c_pps1.OK() && nnc_pps1.OK();
}

// Constructs the powerset of nnc polyhedra from a powerset of
// closed polyhedra.
bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(2*x == 1);
  ph.add_constraint(y >= 0);

  Pointset_Powerset<C_Polyhedron> pps_c(ph);

  Pointset_Powerset<NNC_Polyhedron> pps(pps_c);

  Pointset_Powerset<NNC_Polyhedron> known_pps(2);

  known_pps.add_constraint(2*x == 1);
  known_pps.add_constraint(y >= 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<NNC_Polyhedron>::const_iterator i = pps.begin();
  NNC_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of nnc polyhedra from a powerset of
// closed polyhedra.
bool
test05() {
  Variable x(0);
  Variable y(1);

  Pointset_Powerset<C_Polyhedron> pps(2);

  pps.add_constraint(x >= 1);
  pps.add_constraint(x <= 1);
  pps.add_constraint(y >= 0);

  Pointset_Powerset<C_Polyhedron> pps1(pps, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<C_Polyhedron>::const_iterator i1 = pps.begin();
  C_Polyhedron phi1 = i1->pointset();
  print_constraints(phi1, "*** phi1 ***");

  pps.OK();

  Pointset_Powerset<C_Polyhedron>::const_iterator i = pps.begin();
  C_Polyhedron phi = i->pointset();
  phi.OK();
  print_constraints(phi, "*** phi after ok check ***");

  bool ok = true;

  return ok;
}

// Constructs the powerset of bd shapes from a polyhedron.
bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(3*x >= 2);
  ph.add_constraint(z >= 1);
  ph.add_constraint(3*x + z <= 3);
  C_Polyhedron ph1(ph);

  // With the default complexity, the implied equalities 4*x = 2 and z = 1.
  // are found
  Pointset_Powerset<TBD_Shape> pps(ph);
  Pointset_Powerset<TBD_Shape>::const_iterator i = pps.begin();
  TBD_Shape bdsi = i->pointset();

  BD_Shape<mpq_class> known_bdsi(4);
  known_bdsi.add_constraint(3*x == 2);
  known_bdsi.add_constraint(z == 1);

  bool ok = check_result(bdsi, known_bdsi, "5.97e-8", "3.15e-8", "1.99e-8");

  print_constraints(bdsi, "*** bdsi ***");
  print_constraints(known_bdsi, "*** known_bdsi ***");

  // With the polynomial complexity, implied equalities are not found.
  Pointset_Powerset<TBD_Shape> pps1(ph1, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<TBD_Shape>::const_iterator i1 = pps1.begin();
  TBD_Shape bdsi1 = i1->pointset();

  BD_Shape<mpq_class> known_bdsi1(4);
  known_bdsi1.add_constraint(3*x >= 2);
  known_bdsi1.add_constraint(z >= 1);

  ok = check_result(bdsi1, known_bdsi1, "3.98e-8", "3.98e-8", "3.98e-8") && ok;

  print_constraints(bdsi1, "*** bdsi1 ***");
  print_constraints(known_bdsi1, "*** known_bds1i ***");

  ok = ok && pps.OK() && pps1.OK();

  Pointset_Powerset<TBD_Shape>::const_iterator i_ok = pps.begin();
  TBD_Shape bdsi_ok = i_ok->pointset();
  Pointset_Powerset<TBD_Shape>::const_iterator i1_ok = pps1.begin();
  TBD_Shape bdsi1_ok = i1_ok->pointset();

  print_constraints(bdsi_ok, "*** bdsi after OK() ***");
  print_constraints(bdsi1_ok, "*** bdsi1 after OK() ***");

  return ok;
}

// Constructs the powerset of bd shapes from a polyhedron whose
// constraints are inconsistent (i.e., is empty but not marked as
// empty).
bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(3*x >= 2);
  ph.add_constraint(z >= 1);
  ph.add_constraint(3*x + z <= 2);
  C_Polyhedron ph1(ph);

  // With the default complexity, the built powerset is empty.
  Pointset_Powerset<TBD_Shape> pps(ph);
  // With the polynomial complexity, the built powerset is non-empty.
  Pointset_Powerset<TBD_Shape> pps1(ph1, POLYNOMIAL_COMPLEXITY);

  BD_Shape<mpq_class> known_bds1(4);
  known_bds1.add_constraint(3*x >= 2);
  known_bds1.add_constraint(z >= 1);

  Pointset_Powerset<TBD_Shape>::const_iterator i1 = pps1.begin();
  TBD_Shape bdsi1 = i1->pointset();

  bool ok = check_result(bdsi1, known_bds1, "1.92e-7", "8.89e-8", "7.95e-8")
    && pps.is_empty();

  print_constraints(bdsi1, "*** bdsi1 ***");

  return ok && pps.OK() && pps1.OK();
}

// Constructs the powerset of bd shapes from an empty polyhedron.
bool
test08() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph(2, EMPTY);

  Pointset_Powerset<TBD_Shape> pps(ph);

  Pointset_Powerset<TBD_Shape> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of bd shapes from a powerset of polyhedra.
bool
test09() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(2*x >= 1);
  cs.insert(2*x + y <= 1);
  cs.insert(y >= 0);
  C_Polyhedron ph1(cs);
  C_Polyhedron ph2(cs);

  Pointset_Powerset<C_Polyhedron> pps1_c(ph1, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<C_Polyhedron> pps2_c(ph2);

  Pointset_Powerset<TBD_Shape> pps1(pps1_c, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<TBD_Shape> pps2(pps2_c);

  Pointset_Powerset<TBD_Shape> known_pps1(2);
  known_pps1.add_constraint(2*x >= 1);
  known_pps1.add_constraint(y >= 0);
  Pointset_Powerset<TBD_Shape> known_pps2(2);
  known_pps2.add_constraint(2*x == 1);
  known_pps2.add_constraint(y == 0);

  bool ok = (pps1 == known_pps1 && pps2 == known_pps2);

  Pointset_Powerset<TBD_Shape>::const_iterator i1 = pps1.begin();
  TBD_Shape bdsi1 = i1->pointset();
  print_constraints(bdsi1, "*** bdsi1 ***");

  Pointset_Powerset<TBD_Shape>::const_iterator i2 = pps2.begin();
  TBD_Shape bdsi2 = i2->pointset();
  print_constraints(bdsi2, "*** bdsi2 ***");

  return ok;
}

// Constructs the powerset of octagonal_shapes from a polyhedron.
bool
test10() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(3*x >= 2);
  ph.add_constraint(z >= 1);
  ph.add_constraint(3*x + z <= 3);
  C_Polyhedron ph1(ph);

  // With the default complexity, the implied equalities 4*x = 2 and z = 1.
  // are found
  Pointset_Powerset<TOctagonal_Shape> pps(ph);
  Pointset_Powerset<TOctagonal_Shape>::const_iterator i = pps.begin();
  TOctagonal_Shape osi = i->pointset();

  Octagonal_Shape<mpq_class> known_osi(4);
  known_osi.add_constraint(3*x == 2);
  known_osi.add_constraint(z == 1);

  bool ok = check_result(osi, known_osi, "1.193e-7", "5.45e-8", "3.98e-8");

  print_constraints(osi, "*** osi ***");
  print_constraints(known_osi, "*** known_osi ***");

  // With the polynomial complexity, implied equalities are not found.
  Pointset_Powerset<TOctagonal_Shape> pps1(ph1, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<TOctagonal_Shape>::const_iterator i1 = pps1.begin();
  TOctagonal_Shape osi1 = i1->pointset();

  Octagonal_Shape<mpq_class> known_osi1(4);
  known_osi1.add_constraint(3*x >= 2);
  known_osi1.add_constraint(z >= 1);

  ok = check_result(osi1, known_osi1, "1.193e-7", "8.89e-8", "7.95e-8") && ok;

  print_constraints(osi1, "*** osi1 ***");
  print_constraints(known_osi1, "*** known_osi1 ***");

  ok = ok && pps.OK() && pps1.OK();

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i_ok = pps.begin();
  TOctagonal_Shape osi_ok = i_ok->pointset();
  Pointset_Powerset<TOctagonal_Shape>::const_iterator i1_ok = pps1.begin();
  TOctagonal_Shape osi1_ok = i1_ok->pointset();

  print_constraints(osi_ok, "*** osi after OK() ***");
  print_constraints(osi1_ok, "*** osi1 after OK() ***");

  return ok;
}

// Constructs the powerset of octagonal_shapes from a polyhedron whose
// constraints are inconsistent (i.e., is empty but not marked as
// empty).
bool
test11() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(3*x >= 2);
  ph.add_constraint(z >= 1);
  ph.add_constraint(3*x + z <= 2);
  C_Polyhedron ph1(ph);

  // With the default complexity, the built powerset is empty.
  Pointset_Powerset<TOctagonal_Shape> pps(ph);
  // With the polynomial complexity, the built powerset is non-empty.
  Pointset_Powerset<TOctagonal_Shape> pps1(ph1, POLYNOMIAL_COMPLEXITY);

  Octagonal_Shape<mpq_class> known_os1(4);
  known_os1.add_constraint(3*x >= 2);
  known_os1.add_constraint(z >= 1);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i1 = pps1.begin();
  TOctagonal_Shape osi1 = i1->pointset();

  bool ok = check_result(osi1, known_os1, "1.92e-7", "8.89e-8", "7.95e-8")
    && pps.is_empty();

  print_constraints(osi1, "*** osi1 ***");

  return ok && pps.OK() && pps1.OK();
}

// Constructs the powerset of octagonal_shapes from an empty polyhedron.
bool
test12() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph(2, EMPTY);

  Pointset_Powerset<TOctagonal_Shape> pps(ph);

  Pointset_Powerset<TOctagonal_Shape> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of octagonal_shapes from a powerset of polyhedra.
bool
test13() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(2*x >= 1);
  cs.insert(2*x + y <= 1);
  cs.insert(y >= 0);
  C_Polyhedron ph1(cs);
  C_Polyhedron ph2(cs);

  Pointset_Powerset<C_Polyhedron> pps1_c(ph1, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<C_Polyhedron> pps2_c(ph2);

  Pointset_Powerset<TOctagonal_Shape> pps1(pps1_c, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<TOctagonal_Shape> pps2(pps2_c);

  Pointset_Powerset<TOctagonal_Shape> known_pps1(2);
  known_pps1.add_constraint(2*x >= 1);
  known_pps1.add_constraint(y >= 0);
  Pointset_Powerset<TOctagonal_Shape> known_pps2(2);
  known_pps2.add_constraint(2*x == 1);
  known_pps2.add_constraint(y == 0);

  bool ok = (pps1 == known_pps1 && pps2 == known_pps2);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i1 = pps1.begin();
  TOctagonal_Shape osi1 = i1->pointset();
  print_constraints(osi1, "*** osi1 ***");

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i2 = pps2.begin();
  TOctagonal_Shape osi2 = i2->pointset();
  print_constraints(osi2, "*** osi2 ***");

  return ok;
}

// Constructs the powerset of boxes from a polyhedron.
bool
test14() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(x >= 2);
  ph.add_constraint(y >= 5);
  ph.add_constraint(z >= 1);
  ph.add_constraint(x + y + z <= 8);

  C_Polyhedron ph1(ph);

  // With both the default and polynomial complexities,
  // the implied equalities x = 2, y = 5 and z = 1 are found.
  Pointset_Powerset<TBox> pps(ph);
  Pointset_Powerset<TBox> pps1(ph1, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TBox> known_pps(4);
  known_pps.add_constraint(x == 2);
  known_pps.add_constraint(y == 5);
  known_pps.add_constraint(z == 1);

  bool ok = (pps == known_pps && pps1 == known_pps);

  if (pps.size() > 0)
    print_constraints(pps.begin()->pointset(), "*** box ***");

  if (pps1.size() > 0)
    print_constraints(pps1.begin()->pointset(), "*** box1 ***");

  return ok && pps.OK() && pps1.OK();
}

// Constructs the powerset of boxes from a polyhedron whose constraints
// are inconsistent (i.e., is empty but not marked as empty).
bool
test15() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(x >= 2);
  ph.add_constraint(y >= 5);
  ph.add_constraint(z >= 1);
  ph.add_constraint(x + y + z <= 7);

  C_Polyhedron ph1(ph);

  // With both the default and polynomial complexity,
  // the built powerset is detected to be empty.
  Pointset_Powerset<TBox> pps(ph);
  Pointset_Powerset<TBox> pps1(ph1, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TBox> known_pps(4, EMPTY);

  bool ok = (pps == known_pps && pps1 == known_pps);

  // These will print something only if test is going to fail.
  if (pps.size() > 0)
    print_constraints(pps.begin()->pointset(), "*** box ***");
  if (pps1.size() > 0)
    print_constraints(pps1.begin()->pointset(), "*** box1 ***");

  return ok && pps.OK() && pps1.OK();
}

// Constructs the powerset of boxes from an empty polyhedron.
bool
test16() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph(2, EMPTY);

  Pointset_Powerset<TBox> pps(ph);

  Pointset_Powerset<TBox> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of boxes from a powerset of polyhedra.
bool
test17() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(2*x >= 1);
  cs.insert(2*x + y <= 1);
  cs.insert(y >= 0);
  C_Polyhedron ph1(cs);
  C_Polyhedron ph2(cs);

  Pointset_Powerset<C_Polyhedron> pps1_c(ph1, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<C_Polyhedron> pps2_c(ph2);

  Pointset_Powerset<TBox> pps1(pps1_c, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<TBox> pps2(pps2_c);

  Pointset_Powerset<TBox> known_pps(2);
  known_pps.add_constraint(2*x == 1);
  known_pps.add_constraint(y == 0);

  bool ok = (pps1 == known_pps && pps2 == known_pps);

  if (pps1.size() > 0)
    print_constraints(pps1.begin()->pointset(), "*** box1 ***");
  if (pps2.size() > 0)
    print_constraints(pps2.begin()->pointset(), "*** box2 ***");

  return ok;
}

// Constructs the powerset of grids from a polyhedron.
bool
test18() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(x >= 2);
  ph.add_constraint(z >= 1);
  ph.add_constraint(x + z <= 3);
  C_Polyhedron ph1(4);
  ph1.add_constraint(x >= 2);
  ph1.add_constraint(z >= 1);
  ph1.add_constraint(x + z <= 3);

  // With the default complexity, the implied equalities x = 2 and z = 1.
  // are found
  Pointset_Powerset<Grid> pps(ph);
  // With the polynomial complexity, no implied equalities are found.
  Pointset_Powerset<Grid> pps1(ph1, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<Grid> known_pps(4);
  known_pps.add_congruence((x %= 2) / 0);
  known_pps.add_congruence((z %= 1) / 0);
  Pointset_Powerset<Grid> known_pps1(4);

  bool ok = (pps == known_pps && pps1 == known_pps1);

  Pointset_Powerset<Grid>::const_iterator i = pps.begin();
  Grid gri = i->pointset();
  print_congruences(gri, "*** gri ***");
  Pointset_Powerset<Grid>::const_iterator i1 = pps1.begin();
  Grid gri1 = i1->pointset();
  print_congruences(gri1, "*** gri1 ***");

  return ok && pps.OK() && pps1.OK();
}

// Constructs the powerset of grids from a polyhedron whose constraints
// are inconsistent (i.e., is empty but not marked as empty).
bool
test19() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph(4);
  ph.add_constraint(x >= 2);
  ph.add_constraint(z >= 1);
  ph.add_constraint(x + z <= 2);

  C_Polyhedron ph1(ph);

  // With the default complexity, the built powerset is empty.
  Pointset_Powerset<Grid> pps(ph);
  // With the polynomial complexity, the built powerset is the universe.
  Pointset_Powerset<Grid> pps1(ph1, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<Grid> known_pps(4, EMPTY);
  Pointset_Powerset<Grid> known_pps1(4);

  bool ok = (pps == known_pps && pps1 == known_pps1);

  Pointset_Powerset<Grid>::const_iterator i1 = pps1.begin();
  Grid gri1 = i1->pointset();
  print_congruences(gri1, "*** gri1 ***");

  return ok && pps.OK() && pps1.OK();
}

// Constructs the powerset of grids from an empty polyhedron.
bool
test20() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph(2, EMPTY);

  Pointset_Powerset<Grid> pps(ph);

  Pointset_Powerset<Grid> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of grids from a powerset of polyhedra.
bool
test21() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(2*x >= 1);
  cs.insert(2*x + y <= 1);
  cs.insert(y >= 0);
  C_Polyhedron ph1(cs);
  C_Polyhedron ph2(cs);

  Pointset_Powerset<C_Polyhedron> pps1_c(ph1, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<C_Polyhedron> pps2_c(ph2);

  Pointset_Powerset<Grid> pps1(pps1_c, POLYNOMIAL_COMPLEXITY);
  Pointset_Powerset<Grid> pps2(pps2_c);

  Pointset_Powerset<Grid> known_pps1(2);
  Pointset_Powerset<Grid> known_pps2(2);
  known_pps2.add_constraint(2*x == 1);
  known_pps2.add_constraint(y == 0);

  bool ok = (pps1 == known_pps1 && pps2 == known_pps2);

  Pointset_Powerset<Grid>::const_iterator i1 = pps1.begin();
  Grid gri1 = i1->pointset();
  print_congruences(gri1, "*** gri1 ***");

  Pointset_Powerset<Grid>::const_iterator i2 = pps2.begin();
  Grid gri2 = i2->pointset();
  print_congruences(gri2, "*** gri2 ***");

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
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
  DO_TEST(test21);
END_MAIN
