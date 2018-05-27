/* Test Box::propagate_constraints().
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

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B >= A);
  cs.insert(11*A < 127);
  cs.insert(7*A - 15*B > 8);
  cs.insert(3*B > 2*A);
  cs.insert(A == B);

  print_constraints(cs, "*** cs ***");

  TBox box(2);
  box.propagate_constraints(cs);

  bool ok = box.is_empty();

  print_constraints(box, "*** box.propagate_constraints(cs) ***");

  return ok;
}

bool
test02() {
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

  Constraint_System cs;
  cs.insert(A + C - Q == 130);
  cs.insert(B + D - R == 190);
  cs.insert(-450*A1 - 45*B1 >= -4);
  cs.insert(-90*C1 - 9*D1 >= -8000);
  cs.insert(-30*E1 - 3*F1 >= -3500);
  cs.insert(-30*G1 - 3*H1 >= -3500);
  cs.insert(-E - G >= -1000);
  cs.insert(-F - H >= -1000);
  cs.insert(E + I - 6*A1 == 0);
  cs.insert(G + K - 6*E1 == 0);
  cs.insert(F + J - 6*C1 == 0);
  cs.insert(H + L - 6*G1 == 0);
  cs.insert(A + W - A1 == 5);
  cs.insert(B - W + X - C1 == 0);
  cs.insert(C + Y - E1 == 2);
  cs.insert(D - Y + Z - G1 == 0);
  cs.insert(M + S + A1 - B1 == 6);
  cs.insert(N - O - S + T + C1 - D1 == 0);
  cs.insert(O + U + E1 - F1 == 4);
  cs.insert(M - P + U - V - G1 + H1 == 0);
  cs.insert(A >= 0);
  cs.insert(B >= 0);
  cs.insert(C >= 0);
  cs.insert(D >= 0);
  cs.insert(E >= 0);
  cs.insert(F >= 0);
  cs.insert(G >= 0);
  cs.insert(H >= 0);
  cs.insert(I >= 0);
  cs.insert(J >= 0);
  cs.insert(K >= 0);
  cs.insert(L >= 0);
  cs.insert(M >= 0);
  cs.insert(N >= 0);
  cs.insert(O >= 0);
  cs.insert(P >= 0);
  cs.insert(Q >= 0);
  cs.insert(R >= 0);
  cs.insert(S >= 0);
  cs.insert(T >= 0);
  cs.insert(U >= 0);
  cs.insert(V >= 0);
  cs.insert(W >= 0);
  cs.insert(X >= 0);
  cs.insert(Y >= 0);
  cs.insert(Z >= 0);
  cs.insert(A1 >= 0);
  cs.insert(B1 >= 0);
  cs.insert(C1 >= 0);
  cs.insert(D1 >= 0);
  cs.insert(E1 >= 0);
  cs.insert(F1 >= 0);
  cs.insert(G1 >= 0);
  cs.insert(H1 >= 0);
  print_constraints(cs, "*** cs ***");

  TBox box(cs.space_dimension());
  box.propagate_constraints(cs);

  bool ok = box.is_empty();

  print_constraints(box, "*** box.propagate_constraints(cs) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);
  Variable H(7);

  Constraint_System cs;
  cs.insert(-A >= -20);
  cs.insert(-B - C >= -20);
  cs.insert(-D >= -20);
  cs.insert(A + B - E - F == 0);
  cs.insert(C + D - G - H == 0);
  cs.insert(E >= 10);
  cs.insert(F + G >= 20);
  cs.insert(H >= 30);
  cs.insert(A >= 0);
  cs.insert(-A >= -30);
  cs.insert(B >= 0);
  cs.insert(-B >= -20);
  cs.insert(C >= 0);
  cs.insert(-C >= -10);
  cs.insert(D >= 0);
  cs.insert(-D >= -10);
  cs.insert(E >= 0);
  cs.insert(-E >= -10);
  cs.insert(F >= 0);
  cs.insert(-F >= -2);
  cs.insert(G >= 0);
  cs.insert(-G >= -20);
  cs.insert(H >= 0);
  cs.insert(-H >= -30);
  print_constraints(cs, "*** cs ***");

  TBox box(cs.space_dimension());
  box.propagate_constraints(cs);

  bool ok = box.is_empty();

  print_constraints(box, "*** box.propagate_constraints(cs) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
END_MAIN
