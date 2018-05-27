/* Test Polyhedron::is_disjoint_from(const Octagonal_Shape& y).
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

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(B == 0);
  oct1.add_constraint(A >= 1);
  oct1.add_constraint(A <= 2);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A == 0);
  oct2.add_constraint(B >= 1);
  oct2.add_constraint(B <= 2);

  bool disjoint = oct1.is_disjoint_from(oct2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return disjoint;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A + B == 1);
  oct1.add_constraint(A <= 1);
  oct1.add_constraint(B <= 1);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A + B <= 2);
  oct2.add_constraint(A >= 0);
  oct2.add_constraint(B >= 3);

  bool disjoint = oct1.is_disjoint_from(oct2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return disjoint;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2, EMPTY);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A - B <= 5);

  bool disjoint = oct1.is_disjoint_from(oct2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return disjoint;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point(A + B));
  gs1.insert(ray(B));

  TOctagonal_Shape oct1(gs1);

  Generator_System gs2;
  gs2.insert(point(A + B));
  gs2.insert(ray(-B));

  TOctagonal_Shape oct2(gs2);

  bool disjoint = oct1.is_disjoint_from(oct2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return !disjoint;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A >= 0);
  oct1.add_constraint(B >= -4);

  TOctagonal_Shape oct2(2, EMPTY);

  bool disjoint = oct1.is_disjoint_from(oct2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  return disjoint;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc1(2);
  oc1.add_constraint(x >= y);

  TOctagonal_Shape oc2(3);

  try {
    // This is an invalid use of method
    // Octagonal_Shape::is_disjoint_from(oc2): it is illegal
    // to apply this method to two polyhedra of different dimensions.
    oc1.is_disjoint_from(oc2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test07() {
  Variable A(0);

  TOctagonal_Shape oct1(2);
  oct1.add_constraint(A == 0);

  TOctagonal_Shape oct2(2);
  oct2.add_constraint(A == 1);

  bool ok = oct1.is_disjoint_from(oct2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

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
END_MAIN
