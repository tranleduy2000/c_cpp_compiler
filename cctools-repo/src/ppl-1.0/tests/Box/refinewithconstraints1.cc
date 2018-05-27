/* Test Box::refine_with_constraints().
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

#if defined(__sun) || defined(__sun__)

// Some versions of Solaris define R1 in /usr/include/ia32/sys/reg.h.
// This conflicts with the use or R1 being made in this proram.
# undef R1

#endif // defined(__sun) || defined(__sun__)

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B == 5);

  TBox box(2);
  box.refine_with_constraints(cs);

  print_constraints(box, "*** box.refine_with_constraints(cs) ***");

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B == 5);
  known_result.refine_with_constraint(B - A <= 5);

  bool ok = check_result(box, known_result);

  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(2);

  try {
    // This is an invalid use of method
    // Box::refine_with_constraint: it is illegal
    // to add a constraint with bigger dimension.
    box.refine_with_constraint(x <= 0);
    box.refine_with_constraint(y - x + z >= 0);
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
test03() {
  Variable x(0);
  Variable y(1);

  TBox box(1);

  try {
    // This is an invalid use of the method
    // Box::refine_with_constraints(cs): it is illegal to
    // add a system of constraints that is not dimensional incompatible
    // with the polyhedron.
    Constraint_System cs;
    cs.insert(x - y >= 0);
    box.refine_with_constraints(cs);
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
  Variable y(1);

  TBox box(1);

  try {
    // This is an invalid use of the method
    // Box::refine_with_constraint(c): it is illegal to insert a
    // constraints that contains a variable that is not in the space
    // of the polyhedron.
    box.refine_with_constraint(y >= 0);
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
test05() {
  Variable x(0);
  Variable y(1);

  TBox box(1);

  try {
    // This is an invalid use of the method
    // Box::refine_with_constraints(cs): it is illegal to add a system
    // of constraints that is dimensional incompatible with the
    // polyhedron.
    Constraint_System cs;
    cs.insert(x - y == 0);
    box.refine_with_constraints(cs);
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
test06() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B >= 5);
  cs.insert(B <= 5);

  TBox box(2);
  box.refine_with_constraints(cs);

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B == 5);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box.add_constraints(cs) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);
  Variable H(7);
  Variable I(8);
  Variable J(9);
  Variable K(10);
  Variable L(11);
  Variable M(12);
  Variable N(13);
  Variable O(14);
  Variable P(15);
  Variable Q(16);
  Variable R(17);
  Variable S(18);
  Variable T(19);
  Variable U(20);
  Variable V(21);
  Variable W(22);
  Variable X(23);
  Variable Y(24);
  Variable Z(25);
  Variable A1(26);
  Variable B1(27);
  Variable C1(28);
  Variable D1(29);
  Variable E1(30);
  Variable F1(31);
  Variable G1(32);
  Variable H1(33);
  Variable I1(34);
  Variable J1(35);
  Variable K1(36);
  Variable L1(37);
  Variable M1(38);
  Variable N1(39);
  Variable O1(40);
  Variable P1(41);
  Variable Q1(42);
  Variable R1(43);
  Variable S1(44);
  Variable T1(45);
  Variable U1(46);
  Variable V1(47);
  Variable W1(48);
  Variable X1(49);
  Variable Y1(50);
  Variable Z1(51);
  Variable A2(52);
  Variable B2(53);
  Variable C2(54);
  Variable D2(55);
  Variable E2(56);
  Variable F2(57);
  Variable G2(58);
  Variable H2(59);
  Variable I2(60);
  Variable J2(61);
  Variable K2(62);
  Variable L2(63);
  Variable M2(64);
  Variable N2(65);
  Variable O2(66);
  Variable P2(67);
  Variable Q2(68);
  Variable R2(69);
  Variable S2(70);
  Variable T2(71);
  Variable U2(72);
  Variable V2(73);
  Variable W2(74);
  Variable X2(75);
  Variable Y2(76);
  Variable Z2(77);
  Variable A3(78);
  Variable B3(79);
  Variable C3(80);
  Variable D3(81);
  Variable E3(82);
  Variable F3(83);
  Variable G3(84);
  Variable H3(85);
  Variable I3(86);
  Variable J3(87);
  Variable K3(88);

  Constraint_System cs;
  cs.insert(A + F - M1 - O1 - Q1 - S1 == 0);
  cs.insert(S - U1 - D2 - P2 - V2 == 0);
  cs.insert(B + G - U1 - X1 - A2 == 0);
  cs.insert(T - M1 - H2 - R2 - Z2 == 0);
  cs.insert(C + H - D2 - H2 - L2 == 0);
  cs.insert(U - O1 - X1 - T2 - D3 == 0);
  cs.insert(D - P2 - R2 - T2 == 0);
  cs.insert(V - Q1 - A2 - L2 - H3 == 0);
  cs.insert(E - V2 - Z2 - D3 - H3 == 0);
  cs.insert(W - S1 == 0);
  cs.insert(X - V1 - E2 - W2 == 0);
  cs.insert(I - V1 - Y1 - B2 == 0);
  cs.insert(Y - I2 - A3 == 0);
  cs.insert(J - E2 - I2 - M2 == 0);
  cs.insert(Z - Y1 - E3 == 0);
  cs.insert(A1 - B2 - M2 - I3 == 0);
  cs.insert(K - W2 - A3 - E3 - I3 == 0);
  cs.insert(B1 == 0);
  cs.insert(L - N1 - P1 - R1 - T1 == 0);
  cs.insert(C1 - W1 - F2 - Q2 - X2 == 0);
  cs.insert(M - W1 - Z1 - C2 == 0);
  cs.insert(D1 - N1 - J2 - S2 - B3 == 0);
  cs.insert(N - F2 - J2 - N2 == 0);
  cs.insert(E1 - P1 - Z1 - U2 - F3 == 0);
  cs.insert(O - Q2 - S2 - U2 == 0);
  cs.insert(F1 - R1 - C2 - N2 - J3 == 0);
  cs.insert(P - X2 - B3 - F3 - J3 == 0);
  cs.insert(G1 - T1 == 0);
  cs.insert(H1 - G2 - Y2 == 0);
  cs.insert(I1 - K2 - C3 == 0);
  cs.insert(Q - G2 - K2 - O2 == 0);
  cs.insert(J1 - G3 == 0);
  cs.insert(K1 - O2 - K3 == 0);
  cs.insert(R - Y2 - C3 - G3 - K3 == 0);
  cs.insert(L1 == 0);
  cs.insert(A >= 0);
  cs.insert(-A >= -100);
  cs.insert(B >= 0);
  cs.insert(-B >= -100);
  cs.insert(C >= 0);
  cs.insert(-C >= -90);
  cs.insert(D >= 0);
  cs.insert(-D >= -50);
  cs.insert(E >= 0);
  cs.insert(-E >= -10);
  cs.insert(F >= 0);
  cs.insert(-F >= -200);
  cs.insert(G >= 0);
  cs.insert(-G >= -100);
  cs.insert(H >= 0);
  cs.insert(I >= 0);
  cs.insert(-I >= -25);
  cs.insert(J >= 0);
  cs.insert(-J >= -10);
  cs.insert(K >= 0);
  cs.insert(L >= 0);
  cs.insert(-L >= -50);
  cs.insert(M >= 0);
  cs.insert(-M >= -40);
  cs.insert(N >= 0);
  cs.insert(-N >= -20);
  cs.insert(O >= 0);
  cs.insert(-O >= -5);
  cs.insert(P >= 0);
  cs.insert(Q >= 0);
  cs.insert(-Q >= -30);
  cs.insert(R >= 0);
  cs.insert(S >= 100);
  cs.insert(T >= 100);
  cs.insert(U >= 90);
  cs.insert(V >= 50);
  cs.insert(W >= 10);
  cs.insert(X >= 20);
  cs.insert(Y >= 25);
  cs.insert(Z >= 10);
  cs.insert(A1 >= 15);
  cs.insert(B1 >= 5);
  cs.insert(C1 >= 50);
  cs.insert(D1 >= 40);
  cs.insert(E1 >= 20);
  cs.insert(F1 >= 5);
  cs.insert(G1 >= 15);
  cs.insert(H1 >= 20);
  cs.insert(I1 >= 25);
  cs.insert(J1 >= 30);
  cs.insert(K1 >= 20);
  cs.insert(L1 >= 10);
  cs.insert(M1 >= 0);
  cs.insert(N1 >= 0);
  cs.insert(O1 >= 0);
  cs.insert(P1 >= 0);
  cs.insert(Q1 >= 0);
  cs.insert(R1 >= 0);
  cs.insert(S1 >= 0);
  cs.insert(T1 >= 0);
  cs.insert(U1 >= 0);
  cs.insert(V1 >= 0);
  cs.insert(W1 >= 0);
  cs.insert(X1 >= 0);
  cs.insert(Y1 >= 0);
  cs.insert(Z1 >= 0);
  cs.insert(A2 >= 0);
  cs.insert(B2 >= 0);
  cs.insert(C2 >= 0);
  cs.insert(D2 >= 0);
  cs.insert(E2 >= 0);
  cs.insert(F2 >= 0);
  cs.insert(G2 >= 0);
  cs.insert(H2 >= 0);
  cs.insert(I2 >= 0);
  cs.insert(J2 >= 0);
  cs.insert(K2 >= 0);
  cs.insert(L2 >= 0);
  cs.insert(M2 >= 0);
  cs.insert(N2 >= 0);
  cs.insert(O2 >= 0);
  cs.insert(P2 >= 0);
  cs.insert(Q2 >= 0);
  cs.insert(R2 >= 0);
  cs.insert(S2 >= 0);
  cs.insert(T2 >= 0);
  cs.insert(U2 >= 0);
  cs.insert(V2 >= 0);
  cs.insert(W2 >= 0);
  cs.insert(X2 >= 0);
  cs.insert(Y2 >= 0);
  cs.insert(Z2 >= 0);
  cs.insert(A3 >= 0);
  cs.insert(B3 >= 0);
  cs.insert(C3 >= 0);
  cs.insert(D3 >= 0);
  cs.insert(E3 >= 0);
  cs.insert(F3 >= 0);
  cs.insert(G3 >= 0);
  cs.insert(H3 >= 0);
  cs.insert(I3 >= 0);
  cs.insert(J3 >= 0);
  cs.insert(K3 >= 0);
  print_constraints(cs, "*** cs ***");

  TBox box(cs.space_dimension());
  box.refine_with_constraints(cs);

  bool ok = box.is_empty();

  print_constraints(box, "*** box.refine_with_constraints(cs) ***");

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
  DO_TEST_F8(test07);
END_MAIN
