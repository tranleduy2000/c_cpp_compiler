/* Test Pointset_Powerset<PH>::relation_with().
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

// Powerset of C polyhedra: relation_with().
bool
test01() {
  Variable x(0);
  Constraint c = (x == 0);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  ps.add_disjunct(C_Polyhedron(1));
  ps.add_constraint(c);
  Congruence cg((x %= 0) / 0);
  Poly_Con_Relation rel = ps.relation_with(cg);

  Poly_Con_Relation known_rel = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  bool ok = (rel == known_rel);;

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test02() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x == 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Congruence cg((x %= 1) / 0);
  Poly_Con_Relation rel = ps.relation_with(cg);

  Poly_Con_Relation known_rel = Poly_Con_Relation::saturates();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test03() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x == 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Congruence cg((x %= 3) / 4);
  Poly_Con_Relation rel = ps.relation_with(cg);

  Poly_Con_Relation known_rel = Poly_Con_Relation::is_disjoint();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test04() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x >= 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x <= 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Congruence cg((x %= 3) / 4);
  Poly_Con_Relation rel = ps.relation_with(cg);

  Poly_Con_Relation known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test05() {
  Variable x(0);
  Constraint c = (x == 0);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  ps.add_disjunct(C_Polyhedron(1));
  ps.add_constraint(c);
  Poly_Con_Relation rel = ps.relation_with(c);

  Poly_Con_Relation known_rel = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  bool ok = (rel == known_rel);;

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test06() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x == 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Constraint c(x == 1);
  Poly_Con_Relation rel = ps.relation_with(c);

  Poly_Con_Relation known_rel = Poly_Con_Relation::saturates();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test07() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x == 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Constraint c(x >= 3);
  Poly_Con_Relation rel = ps.relation_with(c);

  Poly_Con_Relation known_rel = Poly_Con_Relation::is_disjoint();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test08() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x >= 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x <= 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Constraint c(x >= 3);
  Poly_Con_Relation rel = ps.relation_with(c);

  Poly_Con_Relation known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test09() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x == 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Generator g(point(x));
  Poly_Gen_Relation rel = ps.relation_with(g);

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::subsumes();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
}

// Powerset of C polyhedra: relation_with().
bool
test10() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x <= 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Generator g(line(3*x));
  Poly_Gen_Relation rel = ps.relation_with(g);

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::nothing();

  bool ok = (rel == known_rel);

  return ok && ps.OK();
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
END_MAIN
