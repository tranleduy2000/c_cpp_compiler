/* Test OctagonalShape::bounds_from_below() and
   Octagonal_Shape::bounds_from_above().
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

  TOctagonal_Shape oct1;
  TOctagonal_Shape oct2(2, EMPTY);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  bool ok = oct1.bounds_from_above(Linear_Expression(3))
    && oct2.bounds_from_below(A);

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 1);
  oct.add_constraint(B >= 1);

  print_constraints(oct, "*** oct ***");

  bool ok = !oct.bounds_from_above(A + B) &&
    oct.bounds_from_below(A + B);

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B >= 0);
  oct.add_constraint(A - B <= 0);

  print_constraints(oct, "*** oct ***");

  bool ok = oct.bounds_from_above(A - B) &&
    !oct.bounds_from_below(A - B);

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B - C <= 0);

  print_constraints(oct, "*** oct ***");

  bool ok = oct.bounds_from_above(A + B - C)
    && !oct.bounds_from_below(A + B - C);

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct(5);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B - C <= 0);
  oct.add_constraint(E - D <= 0);

  print_constraints(oct, "*** oct ***");

  bool ok = oct.bounds_from_above(A + B - C + 2*E - 2*D)
    && !oct.bounds_from_below(A + B - C + 2*E - 2*D);

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct(5);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B - C >= 0);
  oct.add_constraint(E - D >= 0);

  print_constraints(oct, "*** oct ***");

  bool ok = !oct.bounds_from_above(A + B - C + 2*E - 2*D)
    && oct.bounds_from_below(A + B - C + 2*E - 2*D);

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct(5);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B - C >= 0);
  oct.add_constraint(B - C <= -1);
  oct.add_constraint(E - D >= 0);

  print_constraints(oct, "*** oct ***");

  bool ok = oct.bounds_from_above(A + B - C + 2*E - 2*D)
    && oct.bounds_from_below(A + B - C + 2*E - 2*D);

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct(5);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B - C >= 0);
  oct.add_constraint(E - D >= 0);

  print_constraints(oct, "*** oct ***");

  bool ok = oct.bounds_from_above(Linear_Expression(3))
    && oct.bounds_from_below(Linear_Expression(3));

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::bounds_from_below(expr): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the OS.
    oct.bounds_from_below(z);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test10() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::bounds_from_below(expr): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the OS.
    oct.bounds_from_above(z);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);

  print_constraints(oct, "*** oct ***");

  bool ok = !oct.bounds_from_above(A - B) &&
    !oct.bounds_from_below(A - B);

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);

  print_constraints(oct, "*** oct ***");

  bool ok = !oct.bounds_from_above(A + B) &&
    !oct.bounds_from_below(A + B);

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);

  print_constraints(oct, "*** oct ***");

  bool ok = !oct.bounds_from_above(A + B + C) &&
    !oct.bounds_from_below(A + B + C);

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);

  print_constraints(oct, "*** oct ***");

  bool ok = !oct.bounds_from_above(-A - B - C) &&
    !oct.bounds_from_below(-A - B - C);

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
END_MAIN
