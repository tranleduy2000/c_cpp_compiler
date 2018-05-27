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
  Variable B(1);
  Constraint_System cs(A == 3);
  TBox box(2);
  box.add_constraints(cs);

  Poly_Con_Relation rel = box.relation_with(A + B %= 3);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(A + B %= 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A <= 3);
  cs.insert(A >= 0);
  cs.insert(B <= 3);
  cs.insert(B >= 0);

  TBox box(cs);

  Poly_Con_Relation rel = box.relation_with((A + B %= 2) / 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 2) / 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A <= 3);
  cs.insert(A >= 0);
  cs.insert(B <= 3);
  cs.insert(B >= 0);

  TBox box(cs);

  Poly_Con_Relation rel = box.relation_with((A + B %= -7) / 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 7 / 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A < 3);
  cs.insert(A >= 0);
  cs.insert(B <= 3);
  cs.insert(B >= 0);

  TBox box(cs);

  Poly_Con_Relation rel = box.relation_with((A + B %= 6) / 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 6 / 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A <= 3);
  cs.insert(A >= 3);
  cs.insert(B <= 3);
  cs.insert(B >= 3);

  TBox box(cs);

  Poly_Con_Relation rel = box.relation_with((A + B %= 0) / 6);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 0) / 6) == " << rel << endl;

  Poly_Con_Relation known_result
    = Poly_Con_Relation::is_included() && Poly_Con_Relation::saturates();

  return rel == known_result;
}

bool
test06() {

  TBox box(0);

  Poly_Con_Relation rel = box.relation_with((Linear_Expression(1) %= 0) / 2);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((Linear_Expression(1) %= 0) / 2) == "
       << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBox box(2, EMPTY);

  Poly_Con_Relation rel = box.relation_with(A - 2*B %= 1);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(A + 2*B %= 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A > 0);
  cs.insert(3*A < 1);
  cs.insert(B < 1);
  cs.insert(2*B > 1);

  TBox box(cs);

  Poly_Con_Relation rel = box.relation_with(2*B - 3*A  %= 1);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(2*B - 3*A  %= 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test09() {
  // The zero-dim universe box.
  TBox box(0);

  Congruence c(Linear_Expression(0) == 0);
  Poly_Con_Relation rel = box.relation_with(c);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(0 %= 0) == " << rel << endl;

  Poly_Con_Relation known_result
    = Poly_Con_Relation::is_included() && Poly_Con_Relation::saturates();

  return rel == known_result;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(B == 0);

  Poly_Con_Relation rel = box.relation_with((A + B %= 0) / 2);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A %= 2) / 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A >= 1);
  box.add_constraint(A <= 2);
  box.add_constraint(B >= 1);
  box.add_constraint(B <= 2);

  Poly_Con_Relation rel = box.relation_with((A + B %= 0) / 5);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A %= 2) / 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A >= 4);
  box.add_constraint(A <= 7);
  box.add_constraint(B == 0);

  Poly_Con_Relation rel = box.relation_with((A + B %= 2) / 5);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 2) / 5) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 6);
  box.add_constraint(B == 0);

  Poly_Con_Relation rel = box.relation_with((A + B %= 1) / 5);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 1) / 5) == " << rel << endl;

  Poly_Con_Relation known_result
    = Poly_Con_Relation::is_included() && Poly_Con_Relation::saturates();

  return rel == known_result;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 6);
  box.add_constraint(B >= 0);
  box.add_constraint(B <= 3);

  Poly_Con_Relation rel = box.relation_with((A - 2*B %= -9) / 8);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 1) / 5) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 6);
  box.add_constraint(B >= 0);
  box.add_constraint(B <= 3);

  Poly_Con_Relation rel = box.relation_with((A + B %= 4) / 5);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 1) / 5) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 6);
  box.add_constraint(B >= 0);
  box.add_constraint(B <= 3);

  Poly_Con_Relation rel = box.relation_with((A + B %= 24) / 5);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 1) / 5) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(3*A >= 1);
  box.add_constraint(7*A <= 20);
  box.add_constraint(11*B >= 5);
  box.add_constraint(B <= 3);

  Poly_Con_Relation rel = box.relation_with((A + B %= 1) / 9);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 1) / 9) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}


bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(3*A >= 1);
  box.add_constraint(7*A <= 20);
  box.add_constraint(11*B >= 5);
  box.add_constraint(B <= 3);

  Poly_Con_Relation rel = box.relation_with((A + B %= 0) / 10);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with((A + B %= 1) / 5) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  TBox box(1);
  box.add_constraint(A >= 1);

  try {
    // This tests the space dimension exception..
   Poly_Con_Relation rel = box.relation_with((A + B %= 1) / 9);
   (void) rel;
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test20() {
  // The zero-dim empty box.
  TBox box(0, EMPTY);
  Poly_Con_Relation rel = box.relation_with((Linear_Expression(0) %= 1) / 0);

  print_constraints(box, "*** box ***");
  using namespace IO_Operators;
  nout << "box.relation_with(0 %= 1) == " << rel << endl;

  Poly_Con_Relation known_result
    =  Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

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
