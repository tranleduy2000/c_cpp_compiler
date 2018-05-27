/* Test BD_Shape::relation_with().
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

bool
test01() {
  // The zero-dim universe BDS.
  TBD_Shape bds(0);
  Poly_Con_Relation rel = bds.relation_with(Linear_Expression(0) > 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(0 > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test02() {
  // The zero-dim universe BDS.
  TBD_Shape bds(0);
  Poly_Con_Relation rel = bds.relation_with(Linear_Expression(0) > 1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(0 > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test03() {
  // The zero-dim universe BDS.
  TBD_Shape bds(0);
  Poly_Con_Relation rel = bds.relation_with(Linear_Expression(1) > 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(1 > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test04() {
  // An empty BDS.
  TBD_Shape bds(1);
  bds.add_constraint(Linear_Expression(0) >= 1);

  Variable A(0);

  Poly_Con_Relation rel = bds.relation_with(A > 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A > 0) = " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs(A - B == 3);
  TBD_Shape bds(cs);

  Poly_Con_Relation rel = bds.relation_with(A - B > 3);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A - B > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs(A - B <= 3);
  TBD_Shape bds(cs);

  Poly_Con_Relation rel = bds.relation_with(A - B > 3);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A - B > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test07() {
  Variable A(0);

  Constraint_System cs;
  cs.insert(A <= 1);

  TBD_Shape bds(cs);

  Poly_Con_Relation rel = bds.relation_with(A > 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(B >= 0);
  cs.insert(A - B <= 3);

  TBD_Shape bds(cs);

  Poly_Con_Relation rel = bds.relation_with(A - B > 1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A - B > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(B >= 0);
  cs.insert(A - B <= 3);

  TBD_Shape bds(cs);

  Poly_Con_Relation rel = bds.relation_with(A > 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A == 0);
  cs.insert(B <= -1);
  cs.insert(A - B <= 2);

  TBD_Shape bds(cs);

  Poly_Con_Relation rel = bds.relation_with(B - A > 1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(B - A > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test11() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A >= 0);

  Poly_Con_Relation rel = bds.relation_with(Linear_Expression(1) >= 1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(1 >= 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A == 1);
  bds.add_constraint(B >= 2);

  Poly_Con_Relation rel = bds.relation_with(Linear_Expression(1) > 1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(1 > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A == 1);
  bds.add_constraint(B >= 2);
  bds.add_constraint(C <= 1);

  Poly_Con_Relation rel = bds.relation_with(Linear_Expression(1) == 1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(1 == 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B >= 0);
  bds.add_constraint(B >= 0);

  Poly_Gen_Relation rel1 = bds.relation_with(point(B));
  Poly_Gen_Relation rel2 = bds.relation_with(point(-B));

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(point(B)) == " << rel1 << endl;
  nout << "bds.relation_with(point(-B)) == " << rel2 << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel1 == known_result && rel2 == known_result;
}

bool
test15() {
  Variable A(0);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);

  Poly_Gen_Relation rel = bds.relation_with(ray(-A));

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(ray(-A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test16() {
  Variable A(0);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);

  Poly_Gen_Relation rel = bds.relation_with(line(A));

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(line(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test17() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A == 0);
  bds.add_constraint(B == 0);

  Poly_Gen_Relation rel = bds.relation_with(closure_point(A));

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(closure_point(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 2);
  bds.add_constraint(B == 0);

  Poly_Gen_Relation rel = bds.relation_with(ray(A + B));

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(ray(A + B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test19() {
  // The system of constraints of the BDS contains only
  // an equality and the generator `g' is a point.
  Variable A(0);

  TBD_Shape bds(2);
  bds.add_constraint(A == 0);

  Poly_Gen_Relation rel = bds.relation_with(point(2*A));

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(point(2*A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test20() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 1);

  Poly_Con_Relation rel = bds.relation_with(A - C - B <= 2);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A - C - B <= 2) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

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
