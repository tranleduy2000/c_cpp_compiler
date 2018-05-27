/* Test Product<NNC_Polyhedron, Grid>::bounds_from_above() and
   Product<NNC_Polyhedron, Grid>::bounds_from_below()
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

// Empty.
bool
test01() {
  Product prp(7, EMPTY);

  bool ok = (prp.bounds_from_above(Linear_Expression(0))
	     && prp.bounds_from_below(Linear_Expression(0)));

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Zero dimension empty.
bool
test02() {
  Product prp(0, EMPTY);

  bool ok = (prp.bounds_from_above(Linear_Expression(3))
	     && prp.bounds_from_below(Linear_Expression(3)));

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Zero dimension universe.
bool
test03() {
  Product prp(0);

  bool ok = (prp.bounds_from_above(Linear_Expression(1))
	     && prp.bounds_from_below(Linear_Expression(1)));

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Point.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(A == 1);
  prp.refine_with_constraint(3*B == 2);

  Linear_Expression le = A + B;
  bool ok = prp.bounds_from_above(le)
    && prp.bounds_from_below(le);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// only one component is bounded.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(A - B >= 1);
  prp.refine_with_constraint(A - B <= 1);
  prp.refine_with_congruence(3*B %= 2);

  Linear_Expression le = A - B;
  bool ok = prp.bounds_from_above(le)
    && prp.bounds_from_below(le);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Line and neither component is bounded.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(B == 1);

  Linear_Expression le = 2*A - B;

  bool ok = !prp.bounds_from_above(le)
    && !prp.bounds_from_below(le);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Non-empty product. bounded_affine_image/3
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_congruence((A ==  0) / 0);
  prp.refine_with_congruence((B ==  0) / 0);
  prp.refine_with_congruence((C == -2) / 0);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  prp.bounded_affine_image(A, 7-B, B+3);

  Product known_prp(3);
  known_prp.refine_with_constraint(C == -2);
  known_prp.refine_with_constraint(B == 0);
  known_prp.refine_with_constraint(A <= 3);
  known_prp.refine_with_constraint(A - B >= 7);

  bool ok = (prp == known_prp);

  print_congruences(prp,
     "*** prp.bounded_affine_image(A, 7-B, B+3) congruences ***");
  print_constraints(prp,
     "*** prp.bounded_affine_image(A, 7-B, B+3) constraints ***");

  return ok;
}

// Empty product component. bounded_affine_image/3
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_constraint(A ==  0);
  prp.refine_with_constraint(A ==  1);
  prp.refine_with_constraint(C == -2);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  prp.bounded_affine_image(A, 7-B, B+3);

  Product known_prp(3, EMPTY);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Non-empty product. bounded_affine_preimage/3
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_congruence((A ==  0) / 0);
  prp.refine_with_congruence((B ==  0) / 0);
  prp.refine_with_congruence((C == -2) / 0);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  prp.bounded_affine_preimage(A, 7-B, B+3);

  Constraint_System cs;
  cs.insert(C == -2);
  cs.insert(B == 0);
  cs.insert(C >= 3);

  Product known_prp(3);
  known_prp.refine_with_constraints(cs);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Empty product. bounded_affine_preimage/3
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_constraint(A ==  0);
  prp.refine_with_constraint(A ==  1);
  prp.refine_with_constraint(C == -2);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  prp.bounded_affine_preimage(A, 7-B, B+3);

  Product known_prp(3, EMPTY);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

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
END_MAIN
