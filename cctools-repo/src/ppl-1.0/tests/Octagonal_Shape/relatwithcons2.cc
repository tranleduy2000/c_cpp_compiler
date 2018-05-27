/* Test Octagonal_Shape::relation_with(c).
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
  // The zero-dim universe octagon.
  TOctagonal_Shape oct(0);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(0) > 0);

  using namespace IO_Operators;
  nout << "oct.relation_with(0 > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test02() {
  // The zero-dim universe octagon.
  TOctagonal_Shape oct(0);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(0) > 1);

  using namespace IO_Operators;
  nout << "oct.relation_with(0 > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test03() {
  // The zero-dim universe octagon.
  TOctagonal_Shape oct(0);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(1) > 0);

  using namespace IO_Operators;
  nout << "oct.relation_with(1 > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test04() {
  Variable A(0);

  // An empty octagon.
  TOctagonal_Shape oct(1, EMPTY);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(A > 0);

  using namespace IO_Operators;
  nout << "oct.relation_with(A > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs(A + B == 3);
  TOctagonal_Shape oct(cs);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(A + B > 3);

  using namespace IO_Operators;
  nout << "oct.relation_with(A + B > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs(A + B <= 3);
  TOctagonal_Shape oct(cs);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(A + B > 3);

  using namespace IO_Operators;
  nout << "oct.relation_with(A + B > 3) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(B >= 0);
  cs.insert(A + B <= 3);
  TOctagonal_Shape oct(cs);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(A + B < 10);

  using namespace IO_Operators;
  nout << "oct.relation_with(A + B < 10) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(B >= 0);
  cs.insert(A + B <= 3);
  TOctagonal_Shape oct(cs);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(A + B > 1);

  using namespace IO_Operators;
  nout << "oct.relation_with(A + B > 1) == " << rel << endl;

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
  cs.insert(A + B <= 3);
  TOctagonal_Shape oct(cs);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(B - A > 1);

  using namespace IO_Operators;
  nout << "oct.relation_with(B - A > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test10() {
  Variable A(0);

  TOctagonal_Shape oct(1);
  oct.add_constraint(A >= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(1) >= 1);

  using namespace IO_Operators;
  nout << "oct.relation_with(1 >= 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A == 1);
  oct.add_constraint(B >= 2);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(1) > 1);

  using namespace IO_Operators;
  nout << "oct.relation_with(1 > 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A == 1);
  oct.add_constraint(B >= 2);
  oct.add_constraint(C <= 1);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(1) == 1);

  using namespace IO_Operators;
  nout << "oct.relation_with(1 == 1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test13() {
  Variable A(0);

  TOctagonal_Shape oct(1);
  oct.add_constraint(A >= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Con_Relation rel = oct.relation_with(Linear_Expression(0) >= -1);

  using namespace IO_Operators;
  nout << "oct.relation_with(0 >= -1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // Octagon::relation_with(c):
    // it is illegal to use a constraint that is not dimension-compatible
    // with the octagon.
    Poly_Con_Relation rel = oc.relation_with(-C - B <= 2);
    (void) rel;
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A + B == 3);
  oct.add_constraint(A <= 4);
  oct.add_constraint(B >= 2);

  print_constraints(oct, "*** oct ***");

  Constraint c(A + B == 3);

  print_constraint(c, "*** c ***");

  Poly_Con_Relation rel = oct.relation_with(c);

  using namespace IO_Operators;
  nout << "oct.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  return rel == known_result;
}

bool
test16() {
  Variable A(0);

  TOctagonal_Shape oct(1);
  oct.add_constraint(A == -1);

  Poly_Con_Relation rel = oct.relation_with(A == 0);

  print_constraints(oct, "*** oct ***");
  using namespace IO_Operators;
  nout << "oct.relation_with(A == 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test17() {
  // A single point does not subsume another (different) point.
  Variable A(0);

  Octagonal_Shape<mpz_class> oc(1);
  oc.add_constraint(A >= 0);
  oc.add_constraint(A <= 1);

  Constraint c(2*A == 1);
  Poly_Con_Relation rel = oc.relation_with(c);

  print_constraints(oc, "--- oc ---");
  print_constraint(c, "--- c ---");
  using namespace IO_Operators;
  nout << "oc.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test18() {
  Variable A(0);

  TOctagonal_Shape oc(1);
  oc.add_constraint(A >= 0);
  oc.add_constraint(A <= 1);

  Constraint c(Linear_Expression(1) == 0);
  Poly_Con_Relation rel = oc.relation_with(c);

  print_constraints(oc, "--- oc ---");
  print_constraint(c, "--- c ---");
  using namespace IO_Operators;
  nout << "oc.relation_with(1 == 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test19() {
  Variable A(0);

  TOctagonal_Shape oc(1);
  oc.add_constraint(A >= 0);
  oc.add_constraint(A <= 1);

  Congruence cg((A %= 0) / 0);
  Poly_Con_Relation rel = oc.relation_with(cg);

  print_constraints(oc, "--- oc ---");
  print_congruence(cg, "--- cg ---");
  using namespace IO_Operators;
  nout << "oc.relation_with(A == 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test20() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(A - B <= 1);

  Congruence cg((A + 3*B %= 0) / 1);
  Poly_Con_Relation rel = oc.relation_with(cg);

  print_constraints(oc, "--- oc ---");
  print_congruence(cg, "--- cg ---");
  using namespace IO_Operators;
  nout << "oc.relation_with((A + 3*B %= 0)/1) == " << rel << endl;

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

