/* Test Product<NNC_Polyhedron, Grid>::contains() and
   Product<NNC_Polyhedron, Grid>::strictly_contains().
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

// contains()
bool
test01() {
  Variable A(0);

  Product prp1(1);
  prp1.refine_with_constraint(A <= 3);
  prp1.refine_with_congruence((A %= 3) / 2);

  Product prp2(1);
  prp2.refine_with_constraint(A <= 3);
  prp2.refine_with_congruence(A %= 3);

  bool ok1 = !prp1.contains(prp2);

  prp2.refine_with_congruence((A %= 1) / 4);

  bool ok2 = prp1.contains(prp2);

  prp1.refine_with_congruence((A == 1) / 0);
  prp2.refine_with_constraint(A <= 2);
  prp2.refine_with_constraint(A >= -1);
  prp2.refine_with_congruence((A %= 1) / 3);

  bool ok3 = !prp1.contains(prp2);

  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp1, "*** prp1 congruences ***");

  return ok1 && ok2 && ok3;
}

// strictly_contains()
bool
test02() {
  Variable A(0);

  Product prp1(1);
  prp1.refine_with_constraint(A <= 2);
  prp1.refine_with_congruence(A %= 0);

  Product prp2(1);
  prp2.refine_with_constraint(A <= 1);
  prp2.refine_with_congruence(A %= 0);

  bool ok1 = prp1.strictly_contains(prp2);

  prp1.refine_with_constraint(A <= 1);
  prp2.refine_with_congruence((A %= 1) /2);

  bool ok2 = prp1.strictly_contains(prp2);

  prp1.refine_with_congruence((A %= 1) /2);

  bool ok3 = !prp1.strictly_contains(prp2);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok1 && ok2 && ok3;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
