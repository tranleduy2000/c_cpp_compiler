/* Test Product<NNC_Polyhedron, Grid>::generalized_affine_image() and
   Product<NNC_Polyhedron, Grid>::generalized_affine_preimage().
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

// generalized_affine_image(v, EQUAL, e)
bool
test01() {
  Variable A(0);
  Variable B(1);

  Product prp(3);
  prp.refine_with_congruence(A %= 0);
  prp.refine_with_congruence((A + B %= 0) / 2);
  prp.refine_with_constraint(B >= 0);
  prp.refine_with_constraint(A - B >= 0);

  Linear_Expression le(A+2);

  prp.generalized_affine_image(A, EQUAL, le);

  Product known_prp(3);
  known_prp.refine_with_congruence(A %= 0);
  known_prp.refine_with_congruence((A + B %= 0) / 2);
  known_prp.refine_with_constraint(B >= 0);
  known_prp.refine_with_constraint(A - B >= 2);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_image(v, EQUAL, e, d)
bool
test02() {
  Variable A(0);
  Variable B(1);

  Product prp(3);
  prp.refine_with_congruence(A %= 0);
  prp.refine_with_congruence((A + B %= 0) / 2);
  prp.refine_with_constraint(A >= 3);

  prp.generalized_affine_image(B, EQUAL, A + 1, 2);

  Product known_prp(3);
  known_prp.refine_with_constraint(A - 2*B == -1);
  known_prp.refine_with_congruence(A %= 0);
  known_prp.refine_with_constraint(A >= 3);
  known_prp.refine_with_constraint(B >= 2);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_preimage(v, GREATER_OR_EQUAL, e)
bool
test03() {
  Variable A(0);
  Variable B(1);

  Product prp(3);
  prp.refine_with_constraint(A >= 0);
  prp.refine_with_constraint(A <= 4);
  prp.refine_with_constraint(B <= 5);
  prp.refine_with_constraint(A <= B);
  prp.refine_with_congruence(A %= B);

  prp.generalized_affine_preimage(B, GREATER_OR_EQUAL, A+2);

  Product known_prp(3);
  known_prp.refine_with_constraint(0 <= A);
  known_prp.refine_with_constraint(A <= 3);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_preimage(v, EQUAL, e, d),
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_congruence(A %= 0);
  prp.refine_with_congruence((B %= 0) / 2);

  prp.generalized_affine_preimage(B, EQUAL, A + B, 1);

  Product known_prp(3);
  known_prp.refine_with_congruence((A + B %= 0) / 2);
  known_prp.refine_with_congruence(A %= 0);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_image(lhs, relsym, rhs)
bool
test05() {
  Variable A(0);
  Variable B(1);

  Product prp(3);
  prp.refine_with_congruence(A %= 0);
  prp.refine_with_constraint(B >= 0);
  prp.refine_with_constraint(A - B >= 1);

  prp.generalized_affine_image(Linear_Expression(2), LESS_OR_EQUAL, A + B);

  Product known_prp(3);
  known_prp.refine_with_congruence(A %= 0);
  known_prp.refine_with_constraint(B >= 0);
  known_prp.refine_with_constraint(A - B >= 1);
  known_prp.refine_with_constraint(2 <= A + B);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_image(lhs, EQUAL, rhs),
bool
test06() {
  Variable A(0);
  Variable B(1);

  Congruence_System cs;
  cs.insert((A %= 0) / 1);
  cs.insert((B %= 0) / 2);

  Product prp(2);
  prp.refine_with_congruences(cs);
  prp.refine_with_constraint(A <= 3);

  prp.generalized_affine_image(A + 2*B, EQUAL, A - B);

  Product known_prp(2);
  known_prp.refine_with_congruence((A + 2*B %= 0) / 1);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_preimage(lhs, relsym, rhs).
bool
test07() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A <= 4);
  cs.insert(B <= 5);
  cs.insert(A <= B);

  Product prp(3);
  prp.refine_with_constraints(cs);
  prp.refine_with_congruence(A %= B);

  prp.generalized_affine_preimage(1*B, GREATER_OR_EQUAL, A+2);

  Product known_prp(3);
  known_prp.refine_with_constraint(0 <= A);
  known_prp.refine_with_constraint(A <= 3);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// generalized_affine_preimage(lhs, EQUAL, rhs)
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_constraint(A - B == 1);

  prp.generalized_affine_preimage(A - B, EQUAL, 2*A - 2*B);

  Product known_prp(3);
  known_prp.refine_with_congruence((2*A - 2*B %= 1) / 0);

  bool ok = (prp == known_prp);


  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

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
END_MAIN
