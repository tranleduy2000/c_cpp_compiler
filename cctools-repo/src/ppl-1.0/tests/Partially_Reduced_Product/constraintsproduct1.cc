/* Test Partially_Reduced_Product<>:: Shrink_Using_Congruences_Reduction()
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

typedef Domain_Product<Poly, Grid>::Constraints_Product PolyGrid;
typedef Domain_Product<Poly, TBox>::Constraints_Product PolyBox;
#if 0
typedef Domain_Product<Affine_Space, TBox>::Constraints_Product AffBox;
#endif
typedef Domain_Product<Grid, TBox>::Constraints_Product GridBox;

namespace {

// Constraints_Reduction with non-strict constraints and
// equality found. Positive coefficients.
bool
test01() {
  Variable A(0);
  Variable B(1);

  PolyGrid cp(2);
  Constraint_System cs;
  cs.insert(3*A >= 7);
  cs.insert(3*A <= 7);
  cp.refine_with_constraints(cs);

  PolyGrid known_cp(2);

  known_cp.refine_with_constraint(3*A == 7);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = ok && cp == known_cp;
    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

// Constraints_Reduction with non-strict constraints and
// equality found. Negative coefficients.
bool
test02() {
  Variable A(0);
  Variable B(1);

  PolyGrid cp(2);
  Constraint_System cs;
  cs.insert(2*A >= -9);
  cs.insert(2*A <= -9);
  cp.refine_with_constraints(cs);

  PolyGrid known_cp(2);

  known_cp.refine_with_constraint(2*A == -9);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = ok && cp == known_cp;

    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

// Constraints_Reduction with strict lower bound and an equality
// is found.
bool
test03() {
  Variable A(0);
  Variable B(1);

  PolyGrid cp(2);
  Constraint_System cs;
  cs.insert(A > 0);
  cs.insert(A <= 0);
  cp.refine_with_constraints(cs);

  PolyGrid known_cp(2, EMPTY);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = ok && cp == known_cp;

    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

// Constraints_Reduction with strict upper bound and an equality
// is found.
bool
test04() {
  Variable A(0);
  Variable B(1);

  PolyGrid cp(2);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A < 3);
  cp.refine_with_constraints(cs);
  cp.refine_with_congruence((A %= 1)/ 0);

  PolyGrid known_cp(2);

  known_cp.refine_with_constraint(A == 1);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = ok && cp == known_cp;

    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

// Constraints_Reduction where emptiness is found.
bool
test05() {
  Variable A(0);
  Variable B(1);

  PolyGrid cp(2);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A <= 2);
  cp.refine_with_constraints(cs);
  cp.refine_with_congruence((A %= 0)/ 0);

  PolyGrid known_cp(2, EMPTY);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = cp == known_cp;

    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

// Constraints_Reduction where emptiness is found.
bool
test06() {
  Variable A(0);
  Variable B(1);

  PolyGrid cp(2);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A <= 1);
  cp.refine_with_constraints(cs);
  cp.refine_with_congruence((A %= 0)/ 2);

  PolyGrid known_cp(2, EMPTY);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = cp == known_cp;

    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

// Constraints_Reduction that calls constraints()
// and hence reduce().
bool
test07() {
  Variable A(0);

  PolyGrid cp(1);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A <= 2);
  cp.refine_with_constraints(cs);
  cp.refine_with_congruence((A %= 0)/ 2);

  bool ok = cp.OK();

  Constraint_System cs1 = cp.constraints();

  PolyGrid known_cp(1);
  known_cp.refine_with_constraints(cs1);
  known_cp.refine_with_congruence((A %= 0)/ 2);

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = (cp == known_cp);
    print_constraints(cp,
                      "*** after known_cp check: cp constraints ***");
    print_congruences(cp,
                      "*** after known_cp check: cp congruences ***");
    if (!ok) {
      print_constraints(known_cp,
                        "*** known_cp constraints ***");
      print_congruences(known_cp,
                        "*** known_cp congruences ***");
    }
  }

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  PolyBox cp(2);

  Constraint_System cs;
  cs.insert(A + B >= 0);
  cs.insert(A + B <= 1);
  cs.insert(A - 2*B <= 10);
  cs.insert(A - 2*B >= 0);
  cp.refine_with_constraints(cs);
  cp.refine_with_constraint(A >= 4);

  PolyBox known_cp(2);
  known_cp.refine_with_constraint(A == 4);
  known_cp.refine_with_constraint(B == -3);

  bool ok = cp.OK();

  print_congruences(cp, "*** after ok check: cp congruences ***");
  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = ok && cp == known_cp;

    print_congruences(cp, "*** after known_cp check: cp congruences ***");
    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}

#if 0
bool
test09() {
  Variable A(0);
  Variable B(1);

  AffBox cp(2);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A <= 4);
  cs.insert(B <= 10);
  cs.insert(B >= 3);
  cp.refine_with_constraints(cs);
  cp.refine_with_constraint(A >= 4);
  cp.refine_with_constraint(B <= 3);

  AffBox known_cp(2);
  known_cp.refine_with_constraint(A == 4);
  known_cp.refine_with_constraint(B == 3);

  bool ok = cp.OK();

  print_constraints(cp, "*** after ok check: cp constraints ***");

  if (ok) {
    ok = ok && cp == known_cp;

    print_constraints(cp, "*** after known_cp check: cp constraints ***");
  }

  return ok;
}
#endif

// space_dimension()
bool
test10() {
  Variable A(0);
  Variable E(4);

  Constraint_System cs;
  cs.insert(A + E <= 9);
  cs.insert(A + E >= 9);

  PolyGrid cp(5);
  cp.refine_with_constraints(cs);

  bool cons_ok = (cp.space_dimension() == 5);

  print_congruences(cp, "*** cp congruences ***");
  print_constraints(cp, "*** cp constraints ***");

  return cons_ok;
}

#if 0
// Example taken from SenS07 (figure 5(a)
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(B >= -10);
  cs.insert(B <= 5);
  cs.insert(C >= 2);
  cs.insert(C <= 3);
  cs.insert(D >= 4);
  cs.insert(D <= 9);
  Congruence_System cgs;
  cgs.insert(A %= 0);
  cgs.insert((B %= 5) / 15);
  cgs.insert(C %= 0);
  cgs.insert(D %= 0);

  AffBox ab1(4);
  ab1.refine_with_constraints(cs);
  ab1.refine_with_congruences(cgs);
  ab1.affine_image(A, 2*B + D);
  TBox box1(4);
  box1.refine_with_constraints(cs);
  box1.refine_with_congruences(cgs);
  box1.affine_image(A, 2*B + D);
  Affine_Space affs1(4);
  affs1.refine_with_constraints(cs);
  affs1.refine_with_congruences(cgs);
  affs1.affine_image(A, 2*B + D);
  GridBox gb1(4);
  gb1.refine_with_constraints(cs);
  gb1.refine_with_congruences(cgs);
  gb1.affine_image(A, 2*B + D);
  print_constraints(ab1,
       "*** (Affine_Space x TBox) ab1 constraints ***");

  AffBox ab2(ab1);
  TBox box2(box1);
  Affine_Space affs2(affs1);
  GridBox gb2(gb1);

  ab1.affine_image(A, A - 4*B);
  box1.affine_image(A, A - 4*B);
  affs1.affine_image(A, A - 4*B);
  gb1.affine_image(A, A - 4*B);

  ab2.affine_image(A, A + 2*B);
  box2.affine_image(A, A + 2*B);
  affs2.affine_image(A, A + 2*B);
  gb2.affine_image(A, A + 2*B);

  ab1.upper_bound_assign(ab2);
  box1.upper_bound_assign(box2);
  affs1.upper_bound_assign(affs2);
  gb1.upper_bound_assign(gb2);

  Constraint_System known_cs;
  known_cs.insert(A >= -36);
  known_cs.insert(A <= 29);
  known_cs.insert(B >= -10);
  known_cs.insert(B <= 5);
  known_cs.insert(C >= 2);
  known_cs.insert(C <= 3);
  known_cs.insert(D >= 4);
  known_cs.insert(D <= 9);
  AffBox known_ab(4);
  known_ab.refine_with_constraints(known_cs);
  TBox known_box(4);
  known_box.refine_with_constraints(known_cs);
  known_box.unconstrain(A);
  known_box.refine_with_constraint(A >= -36);
  known_box.refine_with_constraint(A <= 59);
  Affine_Space known_affs(4);
  known_affs.refine_with_constraints(known_cs);
  GridBox known_gb(4);
  known_gb.refine_with_constraints(known_cs);
  known_gb.refine_with_congruences(cgs);
  known_gb.refine_with_congruence((A - D %= 20) / 30);
  known_gb.refine_with_congruence((B %= 5) / 15);

  bool ok = (ab1 == known_ab
             && box1 == known_box
             && affs1 == known_affs && gb1 == known_gb);

  print_constraints(ab1,
       "*** (Affine_Space x TBox) ab1 constraints ***");
  print_constraints(box1,
       "*** (TBox) box1 constraints ***");
  print_constraints(affs1, "*** (Affine_Space) affs1 constraints ***");
  print_constraints(gb1,
       "*** (Grid x TBox) gb1 constraints ***");
  print_congruences(gb1,
       "*** (Grid x TBox) gb1 congruences ***");

  return ok;
}

// Example taken from SenS07 (figure 5(b)
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  Constraint_System cs;
  cs.insert(B >= 6);
  cs.insert(B <= 8);
  cs.insert(C >= 1);
  cs.insert(C <= 9);
  Congruence_System cgs;
  cgs.insert(A %= 0);
  cgs.insert((B %= 0) / 2);
  cgs.insert(C %= 0);
  cgs.insert(D %= 0);
  cgs.insert(E %= 0);

  AffBox ab1(5);
  TBox box1(5);
  Affine_Space affs1(5);
  GridBox gb1(5);
  ab1.refine_with_constraints(cs);
  ab1.refine_with_congruences(cgs);
  box1.refine_with_constraints(cs);
  box1.refine_with_congruences(cgs);
  affs1.refine_with_constraints(cs);
  affs1.refine_with_congruences(cgs);
  gb1.refine_with_constraints(cs);
  gb1.refine_with_congruences(cgs);

  AffBox ab2(ab1);
  TBox box2(box1);
  Affine_Space affs2(affs1);
  GridBox gb2(gb1);

  ab1.affine_image(E, 2*B);
  box1.affine_image(E, 2*B);
  affs1.affine_image(E, 2*B);
  gb1.affine_image(E, 2*B);

  ab2.affine_image(E, B + C);
  box2.affine_image(E, B + C);
  affs2.affine_image(E, B + C);
  gb2.affine_image(E, B + C);

  ab1.upper_bound_assign(ab2);
  box1.upper_bound_assign(box2);
  affs1.upper_bound_assign(affs2);
  gb1.upper_bound_assign(gb2);

  Constraint_System known_cs(cs);
  known_cs.insert(E >= 7);
  known_cs.insert(E <= 17);
  AffBox known_ab(5);
  known_ab.refine_with_constraints(known_cs);
  TBox known_box(5);
  known_box.refine_with_constraints(known_cs);
  Affine_Space known_affs(5);
  known_affs.refine_with_constraints(known_cs);
  GridBox known_gb(5);
  known_gb.refine_with_constraints(known_cs);
  known_gb.refine_with_congruences(cgs);

  bool ok = (ab1 == known_ab && box1 == known_box
             && affs1 == known_affs && gb1 == known_gb);

  print_constraints(ab1, "*** (Affine_Space x TBox) ab1 constraints ***");
  print_constraints(box1, "*** (TBox) box1 constraints ***");
  print_constraints(affs1, "*** (Affine_Space) affs1 constraints ***");
  print_constraints(gb1, "*** (Grid x TBox) gb1 constraints ***");
  print_congruences(gb1, "*** (Grid x TBox) gb1 congruences ***");

  return ok;
}
#endif

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
//DO_TEST(test09);
  DO_TEST(test10);
//DO_TEST_F8(test11);
//DO_TEST(test12);
END_MAIN
