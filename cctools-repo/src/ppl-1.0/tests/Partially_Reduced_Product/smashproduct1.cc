/* Test Smash_Product.
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
#define REVERSED_TEST
#include "partially_reduced_product_test.hh"

typedef NNC_Polyhedron DOMAIN1;
typedef Grid DOMAIN2;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product CProduct;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Smash_Product SProduct;

namespace {

// Product(dims, type); == and !=
bool
test01() {
  Variable A(0);

  SProduct sp1(3);
  SProduct sp2(3, EMPTY);

  bool ok = (sp1 != sp2);

  if (!ok)
    return false;

  sp1.refine_with_congruence((A %= 0) / 4);
  sp1.refine_with_congruence((A %= 1) / 4);

  ok = (sp1 == sp2);

  ok = ok && sp1.OK() && sp2.OK();

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");

  return ok;
}

// operator=
bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs(A + B <= 9);

  SProduct sp1(2);
  sp1.refine_with_congruence((A %= 9) / 19);
  sp1.refine_with_congruence((A %= 8) / 19);
  SProduct sp2 = sp1;

  bool ok =  (sp1 == sp2);

  ok = ok && sp1.OK() && sp2.OK();

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");

  return ok;
}

// ok(), not reduced.
bool
test03() {
  Variable A(0);
  Variable B(1);

  bool ok;

  SProduct sp(2);
  sp.refine_with_constraint(A >= 1);
  sp.refine_with_constraint(A <= 0);
  sp.refine_with_congruence((A %= 1) / 3);

  ok = sp.is_empty() && sp.OK();

  print_constraints(sp, "*** sp constraints ***");
  print_congruences(sp, "*** sp congruences ***");

  return ok;
}

// ok(), is reduced.
bool
test04() {
  Variable A(0);
  Variable B(1);

  bool ok;

  SProduct sp(2);
  sp.refine_with_constraint(A >= 1);
  sp.refine_with_constraint(A <= 0);
  sp.refine_with_congruence((A %= 1) / 3);

  // reduce the product
  Constraint_System sp_cs = sp.constraints();

  ok = sp.OK();

  print_constraints(sp_cs, "*** sp.constraints(); ***");

  Grid sp_gr(sp_cs);
  ok = sp_gr.is_empty();

  return ok;
}

// Building from inequality constraints()
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A - C <= 8);
  cs.insert(A - C >= 9);

  try {
    SProduct sp(cs);
   }
  catch (const std::invalid_argument& e) {
    nout << "cs contains an inequality constraint: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Building from equality congruences()
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs1;
  cgs1.insert((A - C %= 8) / 0);
  Congruence_System cgs2;
  cgs2.insert((A - C %= 9) / 0);
  cgs2.insert((B %= 21) / 0);

  SProduct sp(cgs1);

  bool ok = sp.OK();

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  return ok;
}

// refine_with_congruences
bool
test07() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);
  cgs.insert((A + B %= 0) / 2);
  cgs.insert((B %= 0) / 2);
  cgs.insert(A + B == 0);

  SProduct sp(2);

  sp.refine_with_congruences(cgs);

  Grid gr(cgs);

  SProduct known_sp(gr);

  bool ok = (sp == known_sp);

  print_constraints(sp, "*** sp constraints ***");
  print_congruences(sp, "*** sp congruences ***");

  return ok;
}

// is_bounded().
bool
test08() {
  Variable A(0);
  Variable B(1);

  bool ok;

  SProduct sp(2);
  sp.refine_with_constraint(A >= 1);
  sp.refine_with_constraint(A <= 0);
  sp.refine_with_congruence((A %= 1) / 3);

  ok = sp.is_bounded();
  ok = ok && sp.OK();

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  return ok;
}

// space_dimension()
bool
test09() {
  Variable A(0);
  Variable E(4);

  Constraint_System cs(A + E < 9);

  SProduct sp(5);
  sp.refine_with_constraints(cs);

  bool smash_ok = (sp.space_dimension() == 5);

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  return smash_ok;
}

// affine_dimension()
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  SProduct sp(3);
  sp.refine_with_constraint(A - C <= 9);
  sp.refine_with_constraint(A - C >= 9);
  sp.refine_with_constraint(B >= 2);

  bool smash_ok = sp.space_dimension() == 3;
  smash_ok = smash_ok && sp.affine_dimension() == 2;

  if (smash_ok) {
    sp.refine_with_constraint(A - C >= 4);
    sp.refine_with_constraint(A - C <= 4);

    smash_ok = smash_ok && sp.affine_dimension() == 0;
  }

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  return smash_ok;
}

// contains()
bool
test11() {
  Variable A(0);

  SProduct sp1(1);
  sp1.refine_with_constraint(A <= 3);
  sp1.refine_with_congruence((A %= 1) / 2);

  SProduct sp2(1);
  sp2.refine_with_constraint(A <= 3);
  sp2.refine_with_constraint(A >= 4);
  sp2.refine_with_congruence((A %= 0) / 2);

  bool smash_ok1 = sp1.contains(sp2) && sp1.strictly_contains(sp2);

  print_constraints(sp1, "*** sp1 constraints ***");
  print_congruences(sp1, "*** sp1 congruences ***");

  if (!smash_ok1)
    return smash_ok1;

  SProduct sp3(1);
  sp3.refine_with_constraint(A <= 3);
  sp3.refine_with_congruence((A %= 1) / 2);

  SProduct sp4(1);
  sp4.refine_with_constraint(A <= 4);
  sp4.refine_with_congruence((A %= 0) / 2);
  sp4.refine_with_congruence((A %= 1) / 2);

  bool smash_ok2 = sp3.contains(sp4) && sp3.strictly_contains(sp4);

  print_constraints(sp3, "*** sp3 constraints ***");
  print_congruences(sp3, "*** sp3 congruences ***");

  if (!smash_ok2)
    return smash_ok2;

  SProduct sp5(1);
  sp5.refine_with_constraint(A <= 3);
  sp5.refine_with_constraint(A >= 4);
  sp5.refine_with_congruence((A %= 1) / 2);
  sp5.refine_with_congruence((A %= 0) / 2);

  SProduct sp6(1, EMPTY);

  bool smash_ok3 = sp5.contains(sp6) && !sp5.strictly_contains(sp6);

  print_constraints(sp5, "*** sp5 constraints ***");
  print_congruences(sp5, "*** sp5 congruences ***");

  return smash_ok3;
}

// intersection_assign()
bool
test12() {
  Variable A(0);
  Variable B(1);

  SProduct sp1(3);
  sp1.refine_with_constraint(A >= 0);
  sp1.refine_with_congruence((A %= 0) / 2);

  SProduct sp2(3);
  sp2.refine_with_constraint(A <= 0);
  sp2.refine_with_congruence((A %= 0) / 1);

  sp1.intersection_assign(sp2);

  SProduct known_sp(3);
  known_sp.refine_with_constraint(A == 0);

  bool smash_ok1 = !(sp1 == known_sp);
  if (!smash_ok1) {
    print_congruences(sp1, "*** sp1 congruences ***");
    print_constraints(sp1, "*** sp1 constraints ***");
    print_congruences(sp2, "*** sp2 congruences ***");
    print_constraints(sp2, "*** sp2 constraints ***");
    return smash_ok1;
  }

  sp2.refine_with_constraint(A >= 1);
  sp1.intersection_assign(sp2);

  known_sp.refine_with_constraint(A == 1);

  bool smash_ok2 = (sp1 == known_sp);

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");
  print_congruences(sp2, "*** sp2 congruences ***");
  print_constraints(sp2, "*** sp2 constraints ***");

  return smash_ok2;
}

// upper_bound_assign(sp2)
bool
test13() {
  Variable A(0);
  Variable B(1);

  SProduct sp(1);
  sp.refine_with_constraint(A <= 18);
  sp.refine_with_constraint(A >= 18);

  SProduct sp1(sp);

  SProduct sp2(1);
  sp2.refine_with_congruence((A %= 19) / 20);
  sp2.refine_with_congruence((A %= 18) / 20);

  sp1.upper_bound_assign(sp2);

  bool smash_ok = (sp1 == sp);

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");

  if (!smash_ok)
    return false;

  sp2.upper_bound_assign(sp1);

  smash_ok = (sp2 == sp);

  print_congruences(sp2, "*** sp2 congruences ***");
  print_constraints(sp2, "*** sp2 constraints ***");

  return smash_ok;
}

// upper_bound_assign_if_exact()
bool
test14() {
  Variable A(0);
  Variable B(1);

  SProduct sp(3);
  sp.refine_with_congruence((A %= 0) / 2);
  sp.refine_with_congruence((B %= 0) / 2);

  SProduct sp1(sp);

  SProduct sp2(3);
  sp2.refine_with_congruence((A %= 1) / 3);
  sp2.refine_with_congruence((B %= 1) / 3);

  bool smash_ok = (!sp1.upper_bound_assign_if_exact(sp2)
		   && !sp2.upper_bound_assign_if_exact(sp1));

  if (!smash_ok) {
    print_congruences(sp1, "*** sp1 congruences ub exact ***");
    print_constraints(sp1, "*** sp1 constraints ub exact ***");
    print_congruences(sp2, "*** sp2 congruences ub exact ***");
    print_constraints(sp2, "*** sp2 constraints ub exact ***");
    return false;
  }

  smash_ok = (sp == sp1);

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");
  print_congruences(sp2, "*** sp2 congruences ***");
  print_constraints(sp2, "*** sp2 constraints ***");

  if (!smash_ok)
    return false;

  // The grid component is empty so that the smash product
  // reduces the polyhedron component to empty.
  sp1.refine_with_congruence((B %= 1) / 2);
  sp1.refine_with_congruence((B %= 0) / 2);

  // The polyhedron component is empty so that the smash product
  // reduces the grid component to empty.
  sp2.refine_with_constraint(B >= 1);
  sp2.refine_with_constraint(B <= 0);

  smash_ok = sp1.upper_bound_assign_if_exact(sp2)
   && sp2.upper_bound_assign_if_exact(sp1);

  if (!smash_ok)
    return false;

  SProduct known_sp(3, EMPTY);

  smash_ok = (sp1 == known_sp) && (sp2 == known_sp);

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");
  print_congruences(sp2, "*** sp2 congruences ***");
  print_constraints(sp2, "*** sp2 constraints ***");

  return smash_ok;
}

// difference_assign(sp2)
bool
test15() {
  Variable A(0);
  Variable B(1);

  SProduct sp(1);
  sp.refine_with_constraint(A <= 18);
  sp.refine_with_constraint(A >= 18);

  SProduct sp1(sp);

  SProduct sp2(1);
  sp2.refine_with_congruence((A %= 19) / 20);
  sp2.refine_with_congruence((A %= 18) / 20);

  sp1.difference_assign(sp2);

  bool smash_ok = (sp1 == sp);

  print_congruences(sp1, "*** sp1 congruences ***");
  print_constraints(sp1, "*** sp1 constraints ***");

  if (!smash_ok)
    return false;

  sp2.difference_assign(sp1);

  SProduct known_sp(1, EMPTY);

  smash_ok = (sp2 == known_sp);

  print_congruences(sp2, "*** sp2 congruences ***");
  print_constraints(sp2, "*** sp2 constraints ***");

  return smash_ok;
}

// time_elapse_assign(y)
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  SProduct sp1(3);
  sp1.refine_with_constraint(A >= 0);
  sp1.refine_with_constraint(B >= 0);
  sp1.refine_with_constraint(A + B >= 3);
  sp1.refine_with_constraint(2*A - B == 0);
  sp1.refine_with_constraint(3*A + C == 0);
  sp1.refine_with_congruence(3*A %= 0);

  SProduct sp2(3);
  sp2.refine_with_constraint(7*C == 4);
  sp2.refine_with_constraint(7*B == -1);
  sp2.refine_with_constraint(7*A == 3);

  sp1.time_elapse_assign(sp2);

  SProduct known_sp(3);
  known_sp.refine_with_constraint(5*A - 13*B - 7*C == 0);
  known_sp.refine_with_constraint(3*A + C >= 0);
  known_sp.refine_with_constraint(A + B >= 3);
  known_sp.refine_with_constraint(4*A - 3*C >= 13);
  known_sp.refine_with_congruence((65*A - B %= 0) / 7);
  known_sp.refine_with_congruence(21*A %= 0);

  bool smash_ok = (sp1 == known_sp);

  print_congruences(sp1, "*** sp1.time_elapse_assign(sp2) congruences ***");
  print_constraints(sp1, "*** sp1.time_elapse_assign(sp2) constraints ***");
  print_congruences(sp2, "*** sp2.time_elapse_assign(sp2) congruences ***");
  print_constraints(sp2, "*** sp2.time_elapse_assign(sp2) constraints ***");

  return smash_ok;
}

// topological_closure_assign
bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  SProduct sp(3);
  sp.refine_with_constraint(B >= 0);
  sp.refine_with_constraint(3*A + C == 0);
  sp.refine_with_constraint(2*A - B == 0);
  sp.refine_with_congruence(3*A %= 0);
#ifdef PH_IS_NNC
  sp.refine_with_constraint(A > 0);
#else
  sp.refine_with_constraint(A >= 0);
#endif

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  sp.topological_closure_assign();

  SProduct known_sp(3);
  known_sp.refine_with_constraint(B >= 0);
  known_sp.refine_with_constraint(3*A + C == 0);
  known_sp.refine_with_constraint(2*A - B == 0);
  known_sp.refine_with_congruence(3*A %= 0);
  known_sp.refine_with_constraint(A >= 0);

  bool smash_ok = (sp == known_sp);

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  return smash_ok;
}

// widening_assign
bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  SProduct sp_prev(3);
  sp_prev.refine_with_constraint(C == 0);
  sp_prev.refine_with_constraint(A - B >= 1);
  sp_prev.refine_with_constraint(A <= 2);
  sp_prev.refine_with_constraint(B >= 0);
  sp_prev.refine_with_congruence((B %= 0) / 2);
  sp_prev.refine_with_congruence(3*A %= 0);

  print_congruences(sp_prev, "*** sp_prev congruences ***");
  print_constraints(sp_prev, "*** sp_prev constraints ***");

  SProduct sp(3);
  sp.refine_with_constraint(C == 0);
  sp.refine_with_constraint(A <= 2);
  sp.refine_with_constraint(B >= 0);
  sp.refine_with_constraint(2*A - B >= 2);
  sp.refine_with_constraint(B >= 0);
  sp.refine_with_congruence(6*A %= 0);
  sp.refine_with_congruence((B %= 0) / 2);

  sp.upper_bound_assign(sp_prev);

  print_congruences(sp, "*** sp congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  sp.widening_assign(sp_prev);

  SProduct known_sp(3);
  known_sp.refine_with_constraint(C == 0);
  known_sp.refine_with_constraint(A <= 2);
  known_sp.refine_with_constraint(B >= 0);
  known_sp.refine_with_congruence((B %= 0) / 2);

  bool smash_ok = (sp == known_sp);

  print_congruences(sp, "*** sp.widening_assign(sp_prev) congruences ***");
  print_constraints(sp, "*** sp.widening_assign(sp_prev) constraints ***");

  return smash_ok;
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
  DO_TEST_F8(test16);
  DO_TEST(test17);
  DO_TEST(test18);
END_MAIN
