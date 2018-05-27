/* Test Pointset_Powerset<PH>::BHZ03_widening_assign().
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
#include <vector>

namespace {

using namespace Parma_Polyhedra_Library::IO_Operators;

const C_Polyhedron&
aux1_test01(unsigned n) {
  Variable x(0);
  Variable y(1);

  static std::vector<C_Polyhedron> p;
  if (p.size() < 5) {
    p.resize(5, C_Polyhedron(2));
    p[2].add_constraint(0 <= x);
    p[2].add_constraint(x <= 4);
    p[2].add_constraint(0 <= y);
    p[2].add_constraint(y <= 4);
    p[1] = p[2];
    p[1].add_constraint(x-y <= 3);
    p[0] = p[1];
    p[0].add_constraint(x+y >= 1);

    p[3].add_constraint(0 <= x);
    p[3].add_constraint(x <= 8);
    p[3].add_constraint(0 <= y);
    p[3].add_constraint(y <= 8);
    p[3].add_constraint(x+y <= 14);
    p[3].add_constraint(x-y >= -6);
    p[4] = p[3];
    p[3].add_constraint(5*x-y >= -2);
    p[3].add_constraint(x+3*y >= 3);
    p[4].add_constraint(4*x-y >= -3);
    p[4].add_constraint(x+2*y >= 2);
  }

  if (n >= p.size()) {
    unsigned new_size = p.size();
    while (n >= new_size)
      new_size *= 2;
    p.resize(p.size()*2);
  }

  if (p[n].is_universe()) {
    p[n] = aux1_test01(n-4);
    p[n].affine_image(x, 2*x);
    p[n].affine_image(y, 8 - 2*y);
  }

  return p[n];
}

Pointset_Powerset<C_Polyhedron>
aux2_test01(unsigned n) {
  Pointset_Powerset<C_Polyhedron> s(2, EMPTY);
  if (n == 0) {

    nout << "S0 = { P0 }" << endl;

    s.add_disjunct(aux1_test01(0));
    return s;
  }

  const int p_base = (n-1)/3*4;

  switch (n % 3) {
  case 1:

    nout << "S" << n << " = { "
	 << "P" << p_base + 1 << ", "
	 << "P" << p_base + 3 << " }" << endl;

    s.add_disjunct(aux1_test01(p_base + 1));
    s.add_disjunct(aux1_test01(p_base + 3));
    break;
  case 2:

    nout << "S" << n << " = { "
	 << "P" << p_base + 2 << ", "
	 << "P" << p_base + 3 << " }" << endl;

    s.add_disjunct(aux1_test01(p_base + 2));
    s.add_disjunct(aux1_test01(p_base + 3));
    break;
  case 0:

    nout << "S" << n << " = { "
	 << "P" << p_base + 2 << ", "
	 << "P" << p_base + 4 << " }" << endl;

    s.add_disjunct(aux1_test01(p_base + 2));
    s.add_disjunct(aux1_test01(p_base + 4));
    break;
  }
  return s;
}

void
aux3_test01(std::ostream& s, const Variable v) {
  s << char('x' + v.id());
}

bool
test01() {
  // Install the alternate output function.
  Variable::set_output_function(aux3_test01);

  Pointset_Powerset<C_Polyhedron> T = aux2_test01(0);

  nout << "T0 = " << T << endl;

  bool converged = false;
  for (unsigned n = 1; !converged && n <= 20; ++n) {
    Pointset_Powerset<C_Polyhedron> Sn = aux2_test01(n);

    nout << "S" << n << " = " << Sn << endl;

    Sn.upper_bound_assign(T);
    Sn.BHZ03_widening_assign<BHRZ03_Certificate>
      (T, widen_fun_ref(&Polyhedron::H79_widening_assign));

    nout << "T" << n << " = " << Sn << endl;

    if (Sn.definitely_entails(T))
      converged = true;
    else
      swap(Sn, T);
  }

  return converged;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron p(2);
  C_Polyhedron q(2);
  C_Polyhedron r(2);
  C_Polyhedron s(2);
  p.add_constraint(A >= 1);
  p.add_constraint(B == 0);
  q.add_constraint(A >= 2);
  q.add_constraint(A <= 7);
  q.add_constraint(B == 1);
  r.add_constraint(A >= 3);
  r.add_constraint(A <= 8);
  r.add_constraint(B == 1);
  s.add_constraint(A >= 1);
  s.add_constraint(A <= 6);
  s.add_constraint(B == 1);
  Pointset_Powerset<C_Polyhedron> P(2, EMPTY);
  P.add_disjunct(p);
  P.add_disjunct(q);
  P.add_disjunct(r);
  P.add_disjunct(s);
  Pointset_Powerset<C_Polyhedron> Q(2, EMPTY);
  Q.add_disjunct(p);
  Q.add_disjunct(q);
  Q.add_disjunct(s);

  nout << "P = " << P << endl
       << "Q = " << Q << endl;

  Pointset_Powerset<C_Polyhedron> old_P = P;
  P.BHZ03_widening_assign<BHRZ03_Certificate>
    (Q, widen_fun_ref(&Polyhedron::H79_widening_assign));

  nout << "P.BHZ03(Q, H79)" << " = " << P << endl;

  return P.geometrically_covers(old_P) && P.geometrically_covers(Q);
}

bool
test03() {
  Variable X(0);
  Variable Y(1);

  C_Polyhedron p1(2);
  p1.add_constraint(X >= 0);
  p1.add_constraint(Y >= 0);
  p1.add_constraint(X <= 2);
  p1.add_constraint(Y <= 1);

  C_Polyhedron p2(2);
  p2.add_constraint(X >= 0);
  p2.add_constraint(Y >= 2);
  p2.add_constraint(X <= 1);
  p2.add_constraint(Y <= 3);

  C_Polyhedron p3(2);
  p3.add_constraint(X >= 3);
  p3.add_constraint(Y >= 1);
  p3.add_constraint(X <= 4);
  p3.add_constraint(Y <= 3);

  Pointset_Powerset<C_Polyhedron> T1(2, EMPTY);
  T1.add_disjunct(p1);
  T1.add_disjunct(p2);
  T1.add_disjunct(p3);

  C_Polyhedron p4(2);
  p4.add_constraint(X >= 0);
  p4.add_constraint(Y >= 4);
  p4.add_constraint(X <= 2);
  p4.add_constraint(Y <= 5);

  Pointset_Powerset<C_Polyhedron> T2(2, EMPTY);
  T2.add_disjunct(p1);
  T2.add_disjunct(p2);
  T2.add_disjunct(p3);
  T2.add_disjunct(p4);

  nout << "T1 = " << T1 << endl
       << "T2 = " << T2 << endl;

  Pointset_Powerset<C_Polyhedron> old_T2 = T2;
  T2.BHZ03_widening_assign<BHRZ03_Certificate>
    (T1, widen_fun_ref(&Polyhedron::H79_widening_assign));

  C_Polyhedron pd(2);
  pd.add_constraint(X >= 0);
  pd.add_constraint(X <= 4);
  pd.add_constraint(X + 2*Y >= 10);

  Pointset_Powerset<C_Polyhedron> known_result = old_T2;
  known_result.add_disjunct(pd);

  nout << "T2.BHZ03(T1, H79)" << " = " << T2 << endl;
  nout << "known result" << " = " << known_result << endl;

  return T2 == known_result
    && T2.geometrically_covers(old_T2)
    && T2.geometrically_covers(T1);
}

const C_Polyhedron&
aux1_test04(unsigned n) {
  Variable x(0);
  Variable y(1);
  static std::vector<C_Polyhedron> p;
  if (p.size() < 5) {
    p.resize(5, C_Polyhedron(2));
    p[2].add_constraint(0 <= x);
    p[2].add_constraint(x <= 4);
    p[2].add_constraint(0 <= y);
    p[2].add_constraint(y <= 4);
    p[1] = p[2];
    p[1].add_constraint(x-y <= 3);
    p[0] = p[1];
    p[0].add_constraint(x+y >= 1);

    p[3].add_constraint(0 <= x);
    p[3].add_constraint(x <= 8);
    p[3].add_constraint(0 <= y);
    p[3].add_constraint(y <= 8);
    p[3].add_constraint(x+y <= 14);
    p[3].add_constraint(x-y >= -6);
    p[4] = p[3];
    p[3].add_constraint(5*x-y >= -2);
    p[3].add_constraint(x+3*y >= 3);
    p[4].add_constraint(4*x-y >= -3);
    p[4].add_constraint(x+2*y >= 2);
  }

  if (n >= p.size()) {
    unsigned new_size = p.size();
    while (n >= new_size)
      new_size *= 2;
    p.resize(p.size()*2);
  }

  if (p[n].is_universe()) {
    p[n] = aux1_test04(n-4);
    p[n].affine_image(x, 2*x);
    p[n].affine_image(y, 8 - 2*y);
  }

  return p[n];
}

Pointset_Powerset<C_Polyhedron>
aux2_test04(unsigned n) {
  Pointset_Powerset<C_Polyhedron> s(2, EMPTY);
  if (n == 0) {

    nout << "S0 = { P0 }" << endl;

    s.add_disjunct(aux1_test04(0));
    return s;
  }

  const int p_base = (n-1)/3*4;

  switch (n % 3) {
  case 1:

    nout << "S" << n << " = { "
	 << "P" << p_base + 1 << ", "
	 << "P" << p_base + 3 << " }" << endl;

    s.add_disjunct(aux1_test04(p_base + 1));
    s.add_disjunct(aux1_test04(p_base + 3));
    break;
  case 2:

    nout << "S" << n << " = { "
	 << "P" << p_base + 2 << ", "
	 << "P" << p_base + 3 << " }" << endl;

    s.add_disjunct(aux1_test04(p_base + 2));
    s.add_disjunct(aux1_test04(p_base + 3));
    break;
  case 0:

    nout << "S" << n << " = { "
	 << "P" << p_base + 2 << ", "
	 << "P" << p_base + 4 << " }" << endl;

    s.add_disjunct(aux1_test04(p_base + 2));
    s.add_disjunct(aux1_test04(p_base + 4));
    break;
  }
  return s;
}

void
aux3_test04(std::ostream& s, const Variable v) {
  s << char('x' + v.id());
}

bool
test04() {
  // Install the alternate output function.
  Variable::set_output_function(aux3_test04);

  Pointset_Powerset<C_Polyhedron> T = aux2_test04(0);

  nout << "T0 = " << T << endl;

  bool converged = false;
  for (unsigned n = 1; !converged && n <= 20; ++n) {
    Pointset_Powerset<C_Polyhedron> Sn = aux2_test04(n);

    nout << "S" << n << " = " << Sn << endl;

    Sn.upper_bound_assign(T);
    Sn.BHZ03_widening_assign<H79_Certificate>
      (T, widen_fun_ref(&Polyhedron::H79_widening_assign));

    nout << "T" << n << " = " << Sn << endl;

    if (Sn.definitely_entails(T))
      converged = true;
    else
      swap(Sn, T);
  }

  return converged;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron p(2);
  C_Polyhedron q(2);
  C_Polyhedron r(2);
  C_Polyhedron s(2);
  p.add_constraint(A >= 1);
  p.add_constraint(B == 0);
  q.add_constraint(A >= 2);
  q.add_constraint(A <= 7);
  q.add_constraint(B == 1);
  r.add_constraint(A >= 3);
  r.add_constraint(A <= 8);
  r.add_constraint(B == 1);
  s.add_constraint(A >= 1);
  s.add_constraint(A <= 6);
  s.add_constraint(B == 1);
  Pointset_Powerset<C_Polyhedron> P(2, EMPTY);
  P.add_disjunct(p);
  P.add_disjunct(q);
  P.add_disjunct(r);
  P.add_disjunct(s);
  Pointset_Powerset<C_Polyhedron> Q(2, EMPTY);
  Q.add_disjunct(p);
  Q.add_disjunct(q);
  Q.add_disjunct(s);

  nout << "P = " << P << endl
       << "Q = " << Q << endl;

  Pointset_Powerset<C_Polyhedron> old_P = P;
  P.BHZ03_widening_assign<H79_Certificate>
    (Q, widen_fun_ref(&Polyhedron::H79_widening_assign));

  nout << "P.BHZ03(Q, H79)" << " = " << P << endl;

  return P.geometrically_covers(old_P) && P.geometrically_covers(Q);
}

bool
test06() {
  Variable X(0);
  Variable Y(1);

  C_Polyhedron p1(2);
  p1.add_constraint(X >= 0);
  p1.add_constraint(Y >= 0);
  p1.add_constraint(X <= 2);
  p1.add_constraint(Y <= 1);

  C_Polyhedron p3(2);
  p3.add_constraint(X >= 3);
  p3.add_constraint(Y >= 1);
  p3.add_constraint(X <= 4);
  p3.add_constraint(Y <= 3);

  C_Polyhedron p4(2);
  p4.add_constraint(X >= 0);
  p4.add_constraint(Y >= 4);
  p4.add_constraint(X <= 2);
  p4.add_constraint(Y <= 5);

  Pointset_Powerset<C_Polyhedron> T1(2, EMPTY);
  T1.add_disjunct(p1);
  T1.add_disjunct(p3);
  T1.add_disjunct(p4);

  C_Polyhedron p2(2);
  p2.add_constraint(X >= 0);
  p2.add_constraint(Y >= 2);
  p2.add_constraint(X <= 1);
  p2.add_constraint(Y <= 3);

  Pointset_Powerset<C_Polyhedron> T2(2, EMPTY);
  T2.add_disjunct(p1);
  T2.add_disjunct(p2);
  T2.add_disjunct(p3);
  T2.add_disjunct(p4);

  nout << "T1 = " << T1 << endl
       << "T2 = " << T2 << endl;

  Pointset_Powerset<C_Polyhedron> old_T2 = T2;
  T2.BHZ03_widening_assign<BHRZ03_Certificate>
    (T1, widen_fun_ref(&Polyhedron::H79_widening_assign));

  C_Polyhedron phull_T2(2);
  phull_T2.add_constraint(X >= 0);
  phull_T2.add_constraint(X <= 4);
  phull_T2.add_constraint(Y >= 0);
  phull_T2.add_constraint(Y <= 5);
  phull_T2.add_constraint(X - 2*Y <= 2);
  phull_T2.add_constraint(X + Y <= 7);

  Pointset_Powerset<C_Polyhedron> known_result(2, EMPTY);
  known_result.add_disjunct(phull_T2);

  nout << "T2.BHZ03(T1, H79)" << " = " << T2 << endl;
  nout << "known result" << " = " << known_result << endl;

  return T2 == known_result
    && T2.geometrically_covers(old_T2)
    && T2.geometrically_covers(T1);
}

// This tests the first case of the widening definition when the widening
// does nothing as the lgo for the poly-hull is decreasing.
bool
test07() {
  Variable X(0);
  Variable Y(1);

  C_Polyhedron p1(2);
  p1.add_constraint(Y >= 2);
  p1.add_constraint(Y - X <= 2);
  p1.add_constraint(X + Y <= 4);

  C_Polyhedron p2(2);
  p2.add_constraint(X >= 0);
  p2.add_constraint(Y >= 0);
  p2.add_constraint(X <= 1);
  p2.add_constraint(Y <= 1);

  C_Polyhedron p3(2);
  p3.add_constraint(X >= 2);
  p3.add_constraint(Y >= 0);
  p3.add_constraint(X <= 4);
  p3.add_constraint(Y <= 1);

  C_Polyhedron p4(2);
  p4.add_constraint(X >= 3);
  p4.add_constraint(Y >= 2);
  p4.add_constraint(X <= 4);
  p4.add_constraint(Y <= 3);

  Pointset_Powerset<C_Polyhedron> T1(2, EMPTY);
  T1.add_disjunct(p1);
  T1.add_disjunct(p2);
  T1.add_disjunct(p3);
  T1.add_disjunct(p4);

  C_Polyhedron q1(2);
  q1.add_constraint(X >= 0);
  q1.add_constraint(Y >= 0);
  q1.add_constraint(X <= 4);
  q1.add_constraint(Y <= 4);

  C_Polyhedron q2(2);
  q2.add_constraint(X >= 5);
  q2.add_constraint(Y >= 3);
  q2.add_constraint(X <= 6);
  q2.add_constraint(Y <= 4);

  C_Polyhedron q3(2);
  q3.add_constraint(X >= 5);
  q3.add_constraint(Y >= 0);
  q3.add_constraint(X <= 6);
  q3.add_constraint(Y <= 2);

  Pointset_Powerset<C_Polyhedron> T2(2, EMPTY);
  T2.add_disjunct(q1);
  T2.add_disjunct(q2);
  T2.add_disjunct(q3);

  nout << "T1 = " << T1 << endl
       << "T2 = " << T2 << endl;

  Pointset_Powerset<C_Polyhedron> old_T2 = T2;
  T2.BHZ03_widening_assign<BHRZ03_Certificate>
    (T1, widen_fun_ref(&Polyhedron::H79_widening_assign));

  nout << "T2.BHZ03(T1, H79)" << " = " << T2 << endl;

  return T2 == old_T2
    && T2.geometrically_covers(old_T2)
    && T2.geometrically_covers(T1);
}

// This tests the first case of the widening definition when the widening
// does nothing; the poly-hull is stable with respect to the certificate
// and the multiset ordering for this certificate is decreasing.
bool
test08() {
  Variable X(0);
  Variable Y(1);

  C_Polyhedron p1(2);
  p1.add_constraint(X >= 1);
  p1.add_constraint(Y >= 4);
  p1.add_constraint(X <= 7);
  p1.add_constraint(Y <= 7);
  p1.add_constraint(X - Y <= 2);
  p1.add_constraint(X + Y >= 6);

  C_Polyhedron p2(2);
  p2.add_constraint(X >= 1);
  p2.add_constraint(Y >= 1);
  p2.add_constraint(X <= 3);
  p2.add_constraint(Y <= 3);

  C_Polyhedron p3(2);
  p3.add_constraint(X >= 5);
  p3.add_constraint(Y >= 1);
  p3.add_constraint(X <= 7);
  p3.add_constraint(Y <= 3);

  Pointset_Powerset<C_Polyhedron> T1(2, EMPTY);
  T1.add_disjunct(p1);
  T1.add_disjunct(p2);
  T1.add_disjunct(p3);

  C_Polyhedron q1(2);
  q1.add_constraint(X >= 0);
  q1.add_constraint(Y >= 0);
  q1.add_constraint(X <= 8);
  q1.add_constraint(Y <= 8);

  C_Polyhedron q2(2);
  q2.add_constraint(X >= 10);
  q2.add_constraint(Y >= 6);
  q2.add_constraint(X <= 12);
  q2.add_constraint(Y <= 8);

  C_Polyhedron q3(2);
  q3.add_constraint(X >= 10);
  q3.add_constraint(Y >= 0);
  q3.add_constraint(X <= 12);
  q3.add_constraint(Y <= 4);

  Pointset_Powerset<C_Polyhedron> T2(2, EMPTY);
  T2.add_disjunct(q1);
  T2.add_disjunct(q2);
  T2.add_disjunct(q3);

  nout << "T1 = " << T1 << endl
       << "T2 = " << T2 << endl;

  Pointset_Powerset<C_Polyhedron> old_T2 = T2;
  T2.BHZ03_widening_assign<BHRZ03_Certificate>
    (T1, widen_fun_ref(&Polyhedron::H79_widening_assign));

  nout << "T2.BHZ03(T1, H79)" << " = " << T2 << endl;

  return T2 == old_T2
    && T2.geometrically_covers(old_T2)
    && T2.geometrically_covers(T1);
}

// This tests the first case of the widening definition when the widening
// of the elements of the set reduces the multiset ordering.
bool
test09() {
  Variable X(0);
  Variable Y(1);

  C_Polyhedron p1(2);
  p1.add_constraint(Y >= 2);
  p1.add_constraint(Y <= 3);
  p1.add_constraint(Y - X <= 2);
  p1.add_constraint(X + Y <= 8);

  C_Polyhedron p2(2);
  p2.add_constraint(X >= 0);
  p2.add_constraint(Y >= 0);
  p2.add_constraint(X <= 1);
  p2.add_constraint(Y <= 1);

  C_Polyhedron p3(2);
  p3.add_constraint(X >= 5);
  p3.add_constraint(Y >= 0);
  p3.add_constraint(X <= 8);
  p3.add_constraint(Y <= 1);

  C_Polyhedron p4(2);
  p4.add_constraint(X >= 7);
  p4.add_constraint(Y >= 4);
  p4.add_constraint(X <= 8);
  p4.add_constraint(Y <= 5);

  Pointset_Powerset<C_Polyhedron> T1(2, EMPTY);
  T1.add_disjunct(p1);
  T1.add_disjunct(p2);
  T1.add_disjunct(p3);
  T1.add_disjunct(p4);

  C_Polyhedron q1(2);
  q1.add_constraint(Y >= 2);
  q1.add_constraint(Y <= 4);
  q1.add_constraint(Y - X <= 2);
  q1.add_constraint(X + Y <= 8);

  Pointset_Powerset<C_Polyhedron> T2(2, EMPTY);
  T2.add_disjunct(q1);
  T2.add_disjunct(p2);
  T2.add_disjunct(p3);
  T2.add_disjunct(p4);

  nout << "T1 = " << T1 << endl
       << "T2 = " << T2 << endl;

  Pointset_Powerset<C_Polyhedron> old_T2 = T2;
  T2.BHZ03_widening_assign<BHRZ03_Certificate>
    (T1, widen_fun_ref(&Polyhedron::H79_widening_assign));

  C_Polyhedron r1(2);
  r1.add_constraint(Y >= 2);
  r1.add_constraint(Y - X <= 2);
  r1.add_constraint(X + Y <= 8);

  Pointset_Powerset<C_Polyhedron> known_result(2, EMPTY);
  known_result.add_disjunct(r1);
  known_result.add_disjunct(p2);
  known_result.add_disjunct(p3);
  known_result.add_disjunct(p4);

  nout << "T2.BHZ03(T1, H79)" << " = " << T2 << endl;

  return T2 == known_result
    && T2.geometrically_covers(old_T2)
    && T2.geometrically_covers(T1);
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST_F8(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST_F8(test09);
END_MAIN
