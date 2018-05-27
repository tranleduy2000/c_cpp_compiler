/* Test Polyhedron::relation_with(g) and Polyhedron::relation_with(c).
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

using namespace IO_Operators;

namespace {

bool
test01() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(2*x - y >= 0);
  cs.insert(y >= 0);
  C_Polyhedron ph(cs);
  print_constraints(ph, "*** ph ***");

  Generator g = ray(x + y);
  print_generator(g, "*** g ***");

  Poly_Gen_Relation rel = ph.relation_with(g);

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::subsumes();
  bool ok = (rel == known_rel);

  nout << "ph.relation_with(r(A + B)) == " << rel << endl;

  return ok;
}

bool
test02() {
  Variable x(0);

  C_Polyhedron ph(2, EMPTY);
  print_constraints(ph, "*** ph ***");

  Generator g = point(x);
  print_generator(g, "*** g ***");

  Poly_Gen_Relation rel = ph.relation_with(g);

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::nothing();
  bool ok = (rel == known_rel);

  nout << "ph.relation_with(v(A)) == " << rel << endl;

  return ok;
}

bool
test03() {
  C_Polyhedron ph;
  print_constraints(ph, "*** ph ***");

  Generator g = point();
  print_generator(g, "*** g ***");

  Poly_Gen_Relation rel = ph.relation_with(g);

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::subsumes();
  bool ok = (rel == known_rel);

  nout << "ph.relation_with(v()) == " << rel << endl;

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(y));
  gs.insert(line(x));
  C_Polyhedron ph(gs);
  print_generators(ph, "*** ph ***");

  Generator g = point(x + y);
  print_generator(g, "*** g ***");

  Poly_Gen_Relation rel = ph.relation_with(g);

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::subsumes();
  bool ok = (rel == known_rel);

  nout << "ph.relation_with(v(A + B)) == " << rel << endl;

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(1*A + 1*B));
  C_Polyhedron ph(gs);
  print_generators(ph, "*** ph ***");

  Poly_Con_Relation rel = ph.relation_with(A >= 0);

  Poly_Con_Relation known_rel = Poly_Con_Relation::is_included();
  bool ok = (rel == known_rel);

  nout << "ph.relation_with(A >= 0) == " << rel << endl;

  return ok;
}

bool
test06() {
  Variable y(1);

  C_Polyhedron ph(2, EMPTY);
  print_generators(ph, "*** ph ***");

  Constraint c(y >= 0);
  print_constraint(c, "*** c ***");

  Poly_Con_Relation rel = ph.relation_with(c);

  Poly_Con_Relation known_rel = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included()
    && Poly_Con_Relation::is_disjoint();
  bool ok = (rel == known_rel);

  nout << "ph.relation_with(c) == " << rel << endl;

  return ok;
}

bool
test07() {
  Poly_Con_Relation rel = Poly_Con_Relation::nothing();
  Poly_Con_Relation known_result = Poly_Con_Relation::nothing();

  C_Polyhedron ph;
  print_generators(ph, "*** ph ***");

  // A false inequality constraint.
  Constraint c_false1(Linear_Expression(-1) >= 0);
  print_constraint(c_false1, "*** c_false1 ***");

  rel = ph.relation_with(c_false1);

  nout << "ph.relation_with(c_false1) == " << rel << endl;

  known_result = Poly_Con_Relation::is_disjoint();
  bool ok = (rel == known_result);

  // A false equality constraint.
  Constraint c_false2(Linear_Expression(5) == -2);
  print_constraint(c_false2, "*** c_false2 ***");

  rel = ph.relation_with(c_false2);

  nout << "ph.relation_with(c_false2) == " << rel << endl;

  known_result = Poly_Con_Relation::is_disjoint();
  bool ok1 = (rel == known_result);

  // A saturated inequality.
  Constraint c_saturated1(Linear_Expression(3) >= 3);
  print_constraint(c_saturated1, "*** c_saturated1 ***");

  rel = ph.relation_with(c_saturated1);

  nout << "ph.relation_with(c_saturated1) == " << rel << endl;

  known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();
  bool ok2 = (rel == known_result);

  // A saturated equality.
  Constraint c_saturated2(Linear_Expression(1) == 1);
  print_constraint(c_saturated2, "*** c_saturated2 ***");

  rel = ph.relation_with(c_saturated2);

  nout << "ph.relation_with(c_saturated2) == " << rel << endl;

  known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();
  bool ok3 = (rel == known_result);

  // A satisfied inequality which is not saturated.
  Constraint c_satisfied(Linear_Expression(7) >= 5);
  print_constraint(c_satisfied, "*** c_satisfied ***");

  rel = ph.relation_with(c_satisfied);

  nout << "ph.relation_with(c_satisfied) == " << rel << endl;

  known_result = Poly_Con_Relation::is_included();
  bool ok4 = (rel == known_result);

  return ok && ok1 && ok2 && ok3 && ok4;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x + y >= 1);
  cs.insert(y >= 5);

  C_Polyhedron ph(cs);

  print_generators(ph, "*** ph ***");

  // An equality constraint non-intersecting the polyhedron.
  Constraint c(y == -1);

  print_constraint(c, "*** c ***");

  Poly_Con_Relation rel = ph.relation_with(c);

  nout << "ph.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();
  return rel == known_result;
}

bool
test09() {
  // The zero-dim universe polyhedron.
  C_Polyhedron ph;
  Poly_Con_Relation rel = ph.relation_with(Linear_Expression(0) > 0);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(0 > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test10() {
  // The zero-dim universe polyhedron.
  C_Polyhedron ph;
  Poly_Con_Relation rel = ph.relation_with(Linear_Expression(0) > 1);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(0 > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test11() {
  // The zero-dim universe polyhedron.
  C_Polyhedron ph;
  Poly_Con_Relation rel = ph.relation_with(Linear_Expression(1) > 0);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(1 > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test12() {
  // An empty polyhedron.
  C_Polyhedron ph(1);
  ph.add_constraint(Linear_Expression(0) >= 1);
  Variable A(0);
  Poly_Con_Relation rel = ph.relation_with(A > 0);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(A > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs(A + B == 3);
  C_Polyhedron ph(cs);

  Poly_Con_Relation rel = ph.relation_with(A + B > 3);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(A + B > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs(A + B <= 3);
  C_Polyhedron ph(cs);

  Poly_Con_Relation rel = ph.relation_with(A + B > 3);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(A + B > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(B >= 0);
  cs.insert(A + B <= 3);
  C_Polyhedron ph(cs);

  Poly_Con_Relation rel = ph.relation_with(A + 2*B < 10);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(A + 2*B < 10) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(B >= 0);
  cs.insert(A + B <= 3);
  C_Polyhedron ph(cs);

  Poly_Con_Relation rel = ph.relation_with(A + B > 1);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(A + B > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test17() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.add_constraint(A == 0);

  Poly_Gen_Relation rel = ph.relation_with(point(2*A));

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(point(2*A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(A + 0*B));
  gs.insert(point(3*A));
  C_Polyhedron ph(gs);

  print_generators(ph, "*** ph ***");

  Poly_Con_Relation rel = ph.relation_with(B == 0);

  Poly_Con_Relation known_rel = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  bool ok = (rel == known_rel);

  nout << "ph.relation_with(B == 0) == " << rel << endl;

  return ok;
}

bool
test19() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);

  Poly_Gen_Relation rel = ph.relation_with(ray(-A));

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(ray(-A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();
  return rel == known_result;
}

bool
test20() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);

  Poly_Gen_Relation rel = ph.relation_with(line(A));

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(line(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();
  return rel == known_result;
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
END_MAIN
