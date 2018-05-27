/* Test class Congruence.
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

class Test_Congruence : public Congruence {
public:
  Test_Congruence(Congruence cg) : Congruence(cg) {}
  Test_Congruence(Constraint c) : Congruence(c) {}
  void strong_normalize() { Congruence::strong_normalize(); }
  void normalize() { Congruence::normalize(); }
};

// Negative inhomogeneous term.
static bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((A + 2*B + 3*C %= 5) / 7);
  a.strong_normalize();

  Test_Congruence b((A %= 5 - 3*C - 2*B) / 7);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Positive inhomogeneous term.
static bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((A + 2*B + 3*C %= -5) / 7);
  a.strong_normalize();

  Test_Congruence b((A %= -5 - 3*C - 2*B) / 7);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Common factors and reducible positive inhomogeneous term.
static bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((16*A + 2*B + 8*C + 64 %= 0) / 4);
  a.strong_normalize();

  Test_Congruence b((16*A + 2*B %= - 64 - 8*C) / 4);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Negative first coefficient.
static bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((- A + 2*B + 3*C %= 5) / 7);
  a.strong_normalize();

  Test_Congruence b((- A %= - 2*B + 5 - 3*C) / 7);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Constructed with only the %= operator.
static bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence c(A + 4*B + 3*C %= 5);
  Test_Congruence a(c);
  //Test_Congruence a = (A + 4*B + 3*C %= 5);
  //Test_Congruence a(A + 4*B + 3*C %= 5);
  a.strong_normalize();

  Test_Congruence b(A + 4*B %= 5 - 3*C);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Equality congruence (a modulus of 0).
static bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((3*A + 24*B + 3*C %= -19) / 0);
  a.strong_normalize();

  Test_Congruence b((3*A + 24*B %= -19 - 3*C) / 0);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Constructed from a Constraint with the `/' operator.
static bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((A + 4*B + 3*C == 17) / 3);
  a.strong_normalize();

  Test_Congruence b((A + 4*B == 17 - 3*C) / 3);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Constructed from a Constraint.
static bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a(A + 4*B + 3*C == 17);
  a.strong_normalize();

  Test_Congruence b(A + 4*B == 17 - 3*C);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Set modulus with `/='.
static bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a(A + 4*B + 3*C == 17);
  a /= 3;
  a.strong_normalize();

  Test_Congruence b(A + 4*B == 17 - 3*C);
  b /= 3;
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// is_tautological and is_inconsistent.
static bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a(0*A + 0*B + 0*C %= 17);

  bool ok = (a.is_tautological()) && (!a.is_inconsistent());

  print_congruence(a,
    "*** a(0*A + 0*B + 0*C %= 17) ***");

  a = Test_Congruence((0*A + 0*B + 0*C %= 0) / 3);
  ok &= a.is_tautological()
    && !a.is_inconsistent();

  a = Test_Congruence((0*A + 0*B + 8 %= 0) / 4);
  ok &= a.is_tautological()
    && !a.is_inconsistent();

  print_congruence(a,
    "*** a = Test_Congruence((0*A + 0*B + 8 %= 0) / 4) ***");

  a = Test_Congruence(0*A + 0*B %= 17);
  a /= 0;
  ok &= !a.is_tautological()
    && a.is_inconsistent();

  print_congruence(a,
    "*** a = Test_Congruence(0*A + 0*B %= 17) ***");

  a = Test_Congruence((0*A + 0*B + 3 %= 0) / 0);
  a.strong_normalize();
  ok &= !a.is_tautological()
    && a.is_inconsistent();

  print_congruence(a,
    "*** a = Test_Congruence((0*A + 0*B + 3 %= 0) / 0) ***");

  a = Test_Congruence((0*A + 0*B + 4 %= 0) / 3);
  a.strong_normalize();
  ok &= !a.is_tautological()
    && a.is_inconsistent();

  print_congruence(a,
    "*** a = Test_Congruence((0*A + 0*B + 4 %= 0) / 3) ***");

  a = Test_Congruence((0*A + 1*B %= 1) / 3);
  ok &= !a.is_tautological()
    && !a.is_inconsistent();

  print_congruence(a,
    "*** a = Test_Congruence((0*A + 1*B %= 1) / 3) ***");

  return ok;
}

// Negative moduli.
static bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((A + 4*B + 3*C %= -4) / -3);
  a.strong_normalize();

  Test_Congruence b((A + 4*B %= -1 - 3*C) / -3);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Negative modulus and negative first coefficient.
static bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Test_Congruence a((-A + 4*B + 3*C + 17*D + 2*E %= -4) / -3);
  a.strong_normalize();

  Test_Congruence b((-A + 4*B %= - 3*C - 17*D - 2*E - 4) / -3);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Create from empty linear expression.
static bool
test13() {
  Linear_Expression le;
  Test_Congruence a(le %= le);
  a.strong_normalize();

  Test_Congruence b(le %= 0);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Linear expressions on both sides.
static bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((A - 5 %= 2*B + 3*C) / 7);
  a.strong_normalize();

  Test_Congruence b((-A %= -5 - 3*C - 2*B) / 7);
  b.strong_normalize();

  bool ok = (a == b);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");

  return ok;
}

// Try construct congruence from inequality constraint.
static bool
test15() {
  Variable A(0);
  Variable C(2);

  try {
    Congruence cg(A + C > 0);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Try access the coefficient of a space dimension higher than that of
// a congruence.
static bool
test16() {
  Variable A(0);
  Variable C(2);

  Congruence cg(A + C %= 0);

  try {
    cg.coefficient(Variable(3));
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Check if the congruences are equivalent.
static bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence a((A + 2*B + 3*C %= 5) / 7);

  Test_Congruence b((A + 2*B + 3*C %= 12) / 7);

  Test_Congruence c((2*A + 4*B + 6*C %= 10) / 14);

  bool ok = (a == b);
  ok &= (a == c);

  Test_Congruence d((2*A + 4*B + 6*C %= 10) / 7);

  ok &= (a != d);

  print_congruence(a, "*** a ***");
  print_congruence(b, "*** b ***");
  print_congruence(c, "*** c ***");
  print_congruence(d, "*** d ***");

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
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
END_MAIN
