/* Test Partially_Reduced_Product<>:: Congruences_Reduction()
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

using namespace Parma_Polyhedra_Library::IO_Operators;

typedef NNC_Polyhedron Poly;

typedef Domain_Product<Poly, Grid>::Congruences_Product CGProduct;
// typedef Domain_Product<Poly, Grid>::Direct_Product CGProduct;
namespace {

// Congruences_Reduction with non-strict constraints and
// equality found. Positive coefficients.
bool
test01() {
  Variable A(0);
  Variable B(1);

  CGProduct cgp(2);
  Constraint_System cs;
  cs.insert(3*A >= 7);
  cs.insert(3*A <= 16);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  CGProduct known_cgp(2);

  known_cgp.refine_with_constraint(A == 4);
  known_cgp.refine_with_congruence((A %= 0)/ 2);

  bool ok = cgp.OK();

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = ok && cgp == known_cgp;
    print_congruences(cgp, "*** after known_cgp check: cgp congruences ***");
    print_constraints(cgp, "*** after known_cgp check: cgp constraints ***");
  }

  return ok;
}

// Congruences_Reduction with non-strict constraints and
// equality found. Negative coefficients.
bool
test02() {
  Variable A(0);
  Variable B(1);

  CGProduct cgp(2);
  Constraint_System cs;
  cs.insert(3*A >= -10);
  cs.insert(2*A <= -3);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  CGProduct known_cgp(2);

  known_cgp.refine_with_constraint(A == -2);
  known_cgp.refine_with_congruence((A %= 0)/ 2);

  bool ok = cgp.OK();

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = ok && cgp == known_cgp;

    print_congruences(cgp, "*** after known_cgp check: cgp congruences ***");
    print_constraints(cgp, "*** after known_cgp check: cgp constraints ***");
  }

  return ok;
}

// Congruences_Reduction with non-strict constraints and
// equality found.
bool
test03() {
  Variable A(0);
  Variable B(1);

  CGProduct cgp(2);
  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A <= 3);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  CGProduct known_cgp(2);

  known_cgp.refine_with_constraints(cs);
  known_cgp.refine_with_congruence((A %= 0)/ 2);

  bool ok = cgp.OK();

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = ok && cgp == known_cgp;

    print_congruences(cgp, "*** after known_cgp check: cgp congruences ***");
    print_constraints(cgp, "*** after known_cgp check: cgp constraints ***");
  }

  return ok;
}

// Congruences_Reduction with strict lower bound and an equality
// is found.
bool
test04() {
  Variable A(0);
  Variable B(1);

  CGProduct cgp(2);
  Constraint_System cs;
  cs.insert(A > 0);
  cs.insert(A <= 3);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  CGProduct known_cgp(2);

  known_cgp.refine_with_constraint(A == 2);

  bool ok = cgp.OK();

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = ok && cgp == known_cgp;

    print_congruences(cgp, "*** after known_cgp check: cgp congruences ***");
    print_constraints(cgp, "*** after known_cgp check: cgp constraints ***");
  }

  return ok;
}

// Congruences_Reduction with strict upper bound and an equality
// is found.
bool
test05() {
  Variable A(0);
  Variable B(1);

  CGProduct cgp(2);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A < 3);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  CGProduct known_cgp(2);

  known_cgp.refine_with_constraint(A == 2);

  bool ok = cgp.OK();

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = ok && cgp == known_cgp;

    print_congruences(cgp, "*** after known_cgp check: cgp congruences ***");
    print_constraints(cgp, "*** after known_cgp check: cgp constraints ***");
  }

  return ok;
}

// Congruences_Reduction where emptiness is found.
bool
test06() {
  Variable A(0);
  Variable B(1);

  CGProduct cgp(2);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A < 2);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  CGProduct known_cgp(2, EMPTY);

  bool ok = cgp.OK();

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = cgp == known_cgp;

    print_congruences(cgp, "*** after known_cgp check: cgp congruences ***");
    print_constraints(cgp, "*** after known_cgp check: cgp constraints ***");
  }

  return ok;
}

// Congruences_Reduction that calls constraints()
// and hence reduce().
bool
test07() {
  Variable A(0);

  CGProduct cgp(1);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A <= 2);
  cgp.refine_with_constraints(cs);
  cgp.refine_with_congruence((A %= 0)/ 2);

  bool ok = cgp.OK();

  Constraint_System cs1 = cgp.constraints();

  CGProduct known_cgp(1);
  known_cgp.refine_with_constraints(cs1);

  print_congruences(cgp, "*** after ok check: cgp congruences ***");
  print_constraints(cgp, "*** after ok check: cgp constraints ***");

  if (ok) {
    ok = (cgp == known_cgp);
    print_constraints(cgp,
                      "*** after known_cgp check: cgp constraints ***");
    print_congruences(cgp,
                      "*** after known_cgp check: cgp congruences ***");
    if (!ok) {
      print_constraints(known_cgp,
                        "*** known_cgp constraints ***");
      print_congruences(known_cgp,
                        "*** known_cgp congruences ***");
    }
  }

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
