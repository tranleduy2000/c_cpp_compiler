/* Test Product<NNC_Polyhedron, Grid>::upper_bound_assign()
   Product<NNC_Polyhedron, Grid>::upper_bound_assign_if_exact().
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
#include "partially_reduced_product_test.hh"

typedef NNC_Polyhedron DOMAIN1;
typedef Grid DOMAIN2;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {

// upper_bound_assign(prp2)
bool
test01() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs(A == 9);

  Product prp1(cs);

  Product prp2(1);
  prp2.refine_with_constraint(A == 19);

  prp1.upper_bound_assign(prp2);

  Product known_prp(1);
  known_prp.refine_with_constraint(A >= 9);
  known_prp.refine_with_constraint(A <= 19);
  known_prp.refine_with_congruence((A %= 9) / 10);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// upper_bound_assign(cp2)
// The first product is empty and the second a single point in 1D
bool
test02() {
  Variable A(0);

  Product prp1(1);
  Constraint_System cs1;
  cs1.insert(A >= 1);
  cs1.insert(A <= 0);
  prp1.refine_with_constraints(cs1);

  Product prp2(1);
  Constraint_System cs2;
  cs2.insert(A == 1);
  prp2.refine_with_constraints(cs2);

  Product prp1_copy(prp1);

  prp1.upper_bound_assign(prp2);


  bool ok = (prp1 == prp2 && prp1.OK());

  print_congruences(prp1, "*** after OK() check: prp1 congruences ***");
  print_constraints(prp1, "*** after OK() check: prp1 constraints ***");

  return ok;
}

// upper_bound_assign_if_exact()
bool
test03() {
  Variable A(0);
  Variable B(1);

  Product prp1(3);
  prp1.refine_with_constraint(B == 0);

  Product prp2(3);
  prp2.refine_with_constraint(B == 0);
  prp2.refine_with_constraint(A == 8);

  prp1.upper_bound_assign_if_exact(prp2);

  Product known_prp(3);
  known_prp.refine_with_constraint(B == 0);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// upper_bound_assign_if_exact()
bool
test04() {
  Variable A(0);
  Variable B(1);

  Product prp(3);
  prp.refine_with_congruence((A %= 0) / 2);
  prp.refine_with_congruence((B %= 0) / 2);

  Product prp1(prp);

  Product prp2(3);
  prp2.refine_with_congruence((A %= 1) / 3);
  prp2.refine_with_congruence((B %= 1) / 3);

  bool cons_ok = (!prp1.upper_bound_assign_if_exact(prp2)
		   && !prp2.upper_bound_assign_if_exact(prp1));

  if (!cons_ok) {
    print_congruences(prp1, "*** prp1 congruences ub exact ***");
    print_constraints(prp1, "*** prp1 constraints ub exact ***");
    print_congruences(prp2, "*** prp2 congruences ub exact ***");
    print_constraints(prp2, "*** prp2 constraints ub exact ***");
    return false;
  }

  cons_ok = (prp == prp1);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  if (!cons_ok)
    return false;

  prp1.refine_with_congruence((B %= 1) / 2);

  prp2.refine_with_constraint(B >= 1);
  prp2.refine_with_constraint(B <= 1);

  cons_ok = prp1.upper_bound_assign_if_exact(prp2)
   && prp2.upper_bound_assign_if_exact(prp1);

  if (!cons_ok)
    return false;

  Product known_prp(3);
  known_prp.refine_with_congruence((B %= 1) / 0);
  known_prp.refine_with_congruence((A %= 1) / 3);

  cons_ok = (prp1 == known_prp) && (prp2 == known_prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return cons_ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
