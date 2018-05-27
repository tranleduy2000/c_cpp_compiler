/* Test Box::refine_with_congruences(const Congruence_System&).
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

// Universe Box, zero dimensions and trivial congruence.
bool
test01() {
  Congruence cg(Linear_Expression(0) %= 1);
  TBox box(0);
  box.refine_with_congruence(cg);

  Rational_Box known_result(0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// Universe Box, 4 dimensions and refine with a proper congruence
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Congruence cg(A + B %= 0);
  TBox box(4);
  box.refine_with_congruence(cg);

  Rational_Box known_result(4);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// Universe Box in 3D and refine with an equality congruence.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence cg((A %= 7) / 0);

  TBox box(3);
  box.refine_with_congruence(cg);

  Rational_Box known_result(3);
  known_result.add_constraint(A == 7);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// Box in 1D and refine with an inconsistent proper congruence.
bool
test04() {
  Variable A(0);

  Congruence cg((0*A %= 1) / 2);

  TBox box(1);
  box.refine_with_congruence(cg);

  Rational_Box known_result(1, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// refine_with_congruence()
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(A <= 5);
  cs.insert(A >= 0);
  cs.insert(B <= 5);
  cs.insert(B >= 0);
  cs.insert(C <= 5);
  cs.insert(C >= 0);
  cs.insert(D <= 5);
  cs.insert(D >= 0);
  TBox box(cs);
  box.refine_with_congruence((1*A + 2*B + 3*C + 4*D %= 0) / 0);

  Constraint_System known_cs;
  known_cs.insert(A == 0);
  known_cs.insert(B == 0);
  known_cs.insert(C == 0);
  known_cs.insert(D == 0);
  Rational_Box known_result(known_cs);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}


bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(2);

  try {
    // This is an invalid use of method
    // Box::refine_with_congruence: it is illegal
    // to refine with a congruence with bigger dimension.
    box1.refine_with_congruence(x %= 0);
    box1.refine_with_congruence(y - x + z %= 0);
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
test07() {
  Variable y(1);

  TBox box(1);

  try {
    // This is an invalid use of the method
    // Box::refine_with_congruence(c): it is illegal to refine with a
    // congruence that contains a variable that is not in the space
    // of the box.
    box.refine_with_congruence((y %= 0) / 0);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test07);
END_MAIN
