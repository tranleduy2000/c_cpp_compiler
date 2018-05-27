/* Test Box::relation_with().
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
  Variable A(0);

  TBox box(2, EMPTY);

  Poly_Con_Relation rel = box.relation_with(A >= 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(A >= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test02() {
  // Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(y <= -1);

  Constraint c(y >= 0);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test03() {
  // Variable x(0);
  Variable y(1);
  // Variable z(2);

  TBox box(3);
  box.add_constraint(y <= 1);

  Constraint c(y >= 0);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test04() {
  // Variable x(0);
  // Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(z >= 1);

  Constraint c(z >= 0);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test05() {
  Variable x(0);
  // Variable y(1);

  TBox box(2);
  box.add_constraint(x == 1);

  Constraint c(x >= 1);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(x == 1);
  box.add_constraint(y <= 0);
  box.add_constraint(z >= 2);

  Constraint c(x == 1);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(x >= 3);
  box.add_constraint(y <= 0);
  box.add_constraint(z >= 2);

  Constraint c(x == 2);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(x <= 3);
  box.add_constraint(y <= 0);
  box.add_constraint(z >= 2);

  Constraint c(x == 2);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(x <= 3);
  box.add_constraint(y <= 0);
  box.add_constraint(z >= 2);

  Constraint c(-x >= 2);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test10() {
  // The zero-dim universe box.
  TBox box(0);
  Poly_Con_Relation rel = box.relation_with(Linear_Expression(0) >= 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(0 >= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included()
    && Poly_Con_Relation::saturates();

  return rel == known_result;
}

bool
test11() {
  // The zero-dim universe box.
  TBox box(0);
  Poly_Con_Relation rel = box.relation_with(Linear_Expression(0) >= 1);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(0 >= 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test12() {
  // The zero-dim universe box.
  TBox box;
  Poly_Con_Relation rel = box.relation_with(Linear_Expression(1) >= 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(1 >= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test13() {
  Variable x(0);
  // Variable y(1);

  TBox box(2);
  box.add_constraint(x == 1);

  Constraint c(x > 1);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test14() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(x == 1);
  box.add_constraint(y <= 0);
  box.add_constraint(z >= 2);

  Constraint c(x > 1);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "box.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test15() {
  Variable x(0);
  Variable y(1);

  TBox box(2);
  box.add_constraint(x == 0);
  box.add_constraint(y >= 1);

  Poly_Con_Relation rel = box.relation_with(-y >= -1);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(-y >= -1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test16() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(y <= 3);

  Poly_Con_Relation rel = box.relation_with(y > 3);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(y > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test17() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.add_constraint(y <= 3);

  Poly_Con_Relation rel = box.relation_with(-y >= -4);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(1 >= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test18() {
  Variable A(0);

  TBox box(1);
  box.add_constraint(A <= 0);
  box.add_constraint(A >= -2);

  Poly_Con_Relation rel = box.relation_with(Linear_Expression(0) >= -1);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(0 >= -1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test19() {
  // We verify that a generator is not subsumed by an empty box.
  Variable x(0);

  TBox box(2, EMPTY);

  Generator g = point(x);
  Poly_Gen_Relation rel = box.relation_with(g);

  print_constraints(box, "*** box ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "box.relation_with(v(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test20() {
  // We verify that a zero-dimensional generator is subsumed
  // by a zero-dimensional, universal box.
  TBox box;

  Generator g = point();
  Poly_Gen_Relation rel = box.relation_with(g);

  print_constraints(box, "*** box ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "box.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::subsumes();

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
