/* Test Product<>::is_disjoint().
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
#include "partially_reduced_product_test.hh"

typedef NNC_Polyhedron DOMAIN1;
typedef Grid DOMAIN2;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {


// is_disjoint_from(prp), due to the Polyhedra.
bool
test01() {
  Variable B(1);

  Product prp1(12);
  Product prp2(12);
  prp1.refine_with_constraint(B < 2);
  prp2.refine_with_constraint(B > 3);
  bool ok = prp1.is_disjoint_from(prp2);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// is_disjoint_from(prp), due to the Grids.
bool
test02() {
  Variable A(0);

  Product prp1(3);
  prp1.refine_with_congruence((A %= 0) / 7);

  Product prp2(3);
  prp2.refine_with_congruence((A %= 1) / 7);

  bool ok = prp1.is_disjoint_from(prp2);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// is_disjoint_from(prp), due to either.
bool
test03() {
  Variable A(0);

  Product prp1(3);
  prp1.refine_with_congruence((A %= 0) / 7);
  Product prp2(3);
  prp2.refine_with_congruence((A %= 1) / 7);
  prp1.refine_with_constraint(A < 3);
  prp2.refine_with_constraint(A > 3);

  bool ok = prp1.is_disjoint_from(prp2);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// is_disjoint_from(prp), due to both.
bool
test04() {
  Variable A(0);

  Product prp1(3);
  prp1.refine_with_congruence((A %= 1) / 7);
  Product prp2(3);
  prp2.refine_with_congruence((A %= 1) / 14);
  prp1.refine_with_constraint(A < 6);
  prp2.refine_with_constraint(A > 3);

  bool ok = !prp1.is_disjoint_from(prp2);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// is_disjoint_from(prp), due to the intersection of the entire direct
// products (i.e. the prp1 and prp2 polyhedron components intersect, as
// do the grid components).
bool
test05() {
  Variable A(0);
  Variable B(1);

  Product prp1(2);
  prp1.refine_with_constraint(A <= 4);
  prp1.refine_with_constraint(A >= 0);
  prp1.refine_with_constraint(A - B <= 0);
  prp1.refine_with_constraint(A - B >= 2);
  prp1.refine_with_congruence((A %= 0) / 2);
  prp1.refine_with_congruence((A %= 0) / 4);

  Product prp2(2);
  prp2.refine_with_constraint(A <= 4);
  prp2.refine_with_constraint(A <= 0);
  prp2.refine_with_constraint(A + B >= 4);
  prp2.refine_with_constraint(A + B <= 6);
  // Same grid as prp1.
  prp2.refine_with_congruence((A %= 0) / 2);
  prp2.refine_with_congruence((A %= 0) / 4);

  bool ok = prp1.is_disjoint_from(prp2);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
