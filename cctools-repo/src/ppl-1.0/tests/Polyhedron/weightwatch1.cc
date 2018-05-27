/* Test the weightwatch (i.e., deterministic timeout) facility of the library.
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
#include <stdexcept>

namespace {

typedef
Parma_Polyhedra_Library::Threshold_Watcher<Weightwatch_Traits> Weightwatch;

class Deterministic_Timeout
  : virtual public std::exception,
    public Parma_Polyhedra_Library::Throwable {
public:
  const char* what() const throw() {
    return "deterministic timeout in weightwatch1.cc";
  }

  void throw_me() const {
    throw *this;
  }

  int priority() const {
    return 0;
  }

  ~Deterministic_Timeout() throw() {
  }
};

void too_fat() {
  throw Deterministic_Timeout();
}

bool test01() {
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

  Constraint_System cs;
  cs.insert(B + 8192*D - R == 0);
  cs.insert(Q == 1);
  cs.insert(B + 8192*D - P == 0);
  cs.insert(O == 1);
  cs.insert(B + 8192*D - I - 8192*N == 0);
  cs.insert(I - M == 0);
  cs.insert(L == 0);
  cs.insert(B + 8192*D - I - 8192*K == 0);
  cs.insert(J == 0);
  cs.insert(H == 0);
  cs.insert(D - G == 0);
  cs.insert(B - F == 0);
  cs.insert(E == 0);
  cs.insert(C == 0);
  cs.insert(A == 0);
  // Blind relaxation of strict constraint B - I > 0.
  cs.insert(B - I >= 0);
  cs.insert(-B - 8192*D + I >= -67100672);
  cs.insert(-B >= -8191);
  cs.insert(I >= 0);
  cs.insert(D >= 0);

  C_Polyhedron ph(cs);
  print_constraints(ph, "*** ph ***");

  try {
    Weightwatch ww(5000000, too_fat);
    // Thanks to the blind relaxation of the strict inequality constraint,
    // polyhedron ph is easily seen to contain an integer point.
    const bool contains = ph.contains_integer_point();
    nout << endl << "ph "
         << (contains ? "contains" : "does not contain")
         << " an integer point" << endl;
    return contains;
  }
  // Note: other exceptions are just propagated.
  catch (const Deterministic_Timeout& e) {
    // Unexpected timeout exception.
    nout << endl << e.what() << endl;
    return false;
  }
  // Should never get here.
  return false;
}

bool test02() {
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

  Constraint_System cs;
  cs.insert(B + 8192*D - R == 0);
  cs.insert(Q == 1);
  cs.insert(B + 8192*D - P == 0);
  cs.insert(O == 1);
  cs.insert(B + 8192*D - I - 8192*N == 0);
  cs.insert(I - M == 0);
  cs.insert(L == 0);
  cs.insert(B + 8192*D - I - 8192*K == 0);
  cs.insert(J == 0);
  cs.insert(H == 0);
  cs.insert(D - G == 0);
  cs.insert(B - F == 0);
  cs.insert(E == 0);
  cs.insert(C == 0);
  cs.insert(A == 0);
  // Rewriting the strict constraint B - I > 0.
  cs.insert(B - I >= 1);
  cs.insert(-B - 8192*D + I >= -67100672);
  cs.insert(-B >= -8191);
  cs.insert(I >= 0);
  cs.insert(D >= 0);

  C_Polyhedron ph(cs);
  print_constraints(ph, "*** ph ***");

  try {
    Weightwatch ww(2500000, too_fat);
    // The branch-and-bound heuristics of the MIP solver behaves badly
    // on this particular example, causing timeout to expire.
    const bool contains = ph.contains_integer_point();
    nout << endl
         << "ph " << (contains ? "contains" : "does not contain")
         << " an integer point" << endl;
    return false;
  }
  // Note: other exceptions are just propagated.
  catch (const Deterministic_Timeout& e) {
    // Expected exception.
    nout << endl << e.what() << endl;
    return true;
  }
  // Should never get here.
  return false;
}

bool test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(8192*A + B - 8192*C - D == 0);
  cs.insert(D >= 0);
  cs.insert(-B >= -8191);
  cs.insert(B -D >= 1);
  cs.insert(-B + 8192*C + D >= 0);
  cs.insert(-C >= -8191);

  C_Polyhedron ph(cs);
  print_constraints(ph, "*** ph ***");

  try {
    Weightwatch ww(1000000000, too_fat);
    // Polyhedron ph is the projection of the polyehdron of test01
    // onto a lower dimensional space: the performance issue of previous
    // test does not depend on high dimension vector space.
    const bool contains = ph.contains_integer_point();
    nout << endl
         << "ph " << (contains ? "contains" : "does not contain")
         << " an integer point" << endl;
    return false;
  }
  // Note: other exceptions are just propagated.
  catch (const Deterministic_Timeout& e) {
    // Expected exception.
    nout << endl << e.what() << endl;
    return true;
  }
  // Should never get here.
  return false;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F32(test01);
  DO_TEST_F32(test02);
  DO_TEST_F32(test03);
END_MAIN
