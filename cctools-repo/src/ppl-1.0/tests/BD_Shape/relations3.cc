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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // BD_Shape::relation_with(c):
    // it is illegal to use a constraint that is
    // dimensional incompatible with the BDS.
    Poly_Con_Relation rel = bds.relation_with(C - B <= 2);
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
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A >= 1);

  Poly_Con_Relation rel = bds.relation_with(A - 2*B <= 2);
  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();
  print_constraints(bds, "*** bds ***");

  return rel == known_result;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // BD_Shape::relation_with(c):
    // it is illegal to use a generator that is
    // dimensional incompatible with the BDS.
    Poly_Gen_Relation rel = bds.relation_with(ray(C));
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
test04() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A == -1);

  Poly_Con_Relation rel = bds.relation_with(A == 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A == 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test05() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A == -1);

  Poly_Con_Relation rel = bds.relation_with(A >= 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A >= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test06() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A == -1);

  Poly_Con_Relation rel = bds.relation_with(A <= -2);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A <= -2) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test07() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A == -1);

  Poly_Con_Relation rel = bds.relation_with(A == -1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A == -1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included() &&
    Poly_Con_Relation::saturates();

  return rel == known_result;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B == -1);

  Poly_Con_Relation rel = bds.relation_with(A - B == 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A == 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B == -1);

  Poly_Con_Relation rel = bds.relation_with(A - B >= 0);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A >= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B == -1);

  Poly_Con_Relation rel = bds.relation_with(A - B <= -2);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A <= -2) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B == -1);

  Poly_Con_Relation rel = bds.relation_with(A - B == -1);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A == -1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_included() &&
    Poly_Con_Relation::saturates();

  return rel == known_result;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A - B == -1);

  Poly_Con_Relation rel = bds.relation_with(A - B == -2);

  print_constraints(bds, "*** bds ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(A == -1) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test13() {
  // A 1D empty BDS that is not in minimal form and the point is the origin.
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A <= 0);
  bds.add_constraint(A >= 1);

  Generator g = point();
  Poly_Gen_Relation rel = bds.relation_with(g);

  print_constraints(bds, "*** bds ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test14() {
  // A single point does not subsume another (different) point.
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A == 1);

  Generator g = point();
  Poly_Gen_Relation rel = bds.relation_with(g);

  print_constraints(bds, "*** bds ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test15() {
  // A single point does not subsume another (different) point.
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A == 1);

  Generator g = point(3*A, 2);
  Poly_Gen_Relation rel = bds.relation_with(g);

  print_constraints(bds, "*** bds ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test16() {
  // A single point does not subsume another (different) point.
  Variable A(0);

  BD_Shape<mpz_class> bds(1);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 1);

  Constraint c(2*A == 1);
  Poly_Con_Relation rel = bds.relation_with(c);

  print_constraints(bds, "*** bds ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test17() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 1);

  Constraint c(Linear_Expression(1) == 0);
  Poly_Con_Relation rel = bds.relation_with(c);

  print_constraints(bds, "*** bds ***");
  print_constraint(c, "*** c ***");
  using namespace IO_Operators;
  nout << "bds.relation_with(c) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

  return rel == known_result;
}

bool
test18() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 1);

  Congruence cg((A %= 0) / 0);
  Poly_Con_Relation rel = bds.relation_with(cg);

  print_constraints(bds, "--- bds ---");
  print_congruence(cg, "--- cg ---");
  using namespace IO_Operators;
  nout << "bds.relation_with(A == 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 2);
  bds.add_constraint(A - B <= 1);

  Congruence cg((A + 3*B %= 1) / 10);
  Poly_Con_Relation rel = bds.relation_with(cg);

  print_constraints(bds, "--- bds ---");
  print_congruence(cg, "--- cg ---");
  using namespace IO_Operators;
  nout << "bds.relation_with((A + 3*B %= 1)/10) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();

  return rel == known_result;
}

bool
test20() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 2);
  bds.add_constraint(A - B <= 1);

  Congruence cg((A + 3*B %= 10) / 11);
  Poly_Con_Relation rel = bds.relation_with(cg);

  print_constraints(bds, "--- bds ---");
  print_congruence(cg, "--- cg ---");
  using namespace IO_Operators;
  nout << "bds.relation_with((A + 3*B %= 1)/10) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::is_disjoint();

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
