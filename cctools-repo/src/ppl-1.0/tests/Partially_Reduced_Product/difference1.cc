/* Test Product<NNC_Polyhedron, Grid>::difference_assign() and.
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

// difference_assign()
bool
test01() {
  Variable A(0);
  Variable B(1);

  Product prp1(3, UNIVERSE);
  prp1.refine_with_constraint(A >= 0);
  prp1.refine_with_congruence((A - B %= 0) / 2);

  Product prp2(3);
  prp2.refine_with_constraint(A >= 3);
  prp2.refine_with_congruence((A - B %= 0) / 4);

  prp1.difference_assign(prp2);
  Product known_prp(3);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_constraint(A < 3);
  known_prp.refine_with_congruence((A - B %= 2) / 4);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(prp2,  "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return ok;
}

// difference_assign()
bool
test02() {
  Variable A(0);
  Variable B(1);

  Product prp(1);
  prp.refine_with_constraint(A <= 18);
  prp.refine_with_constraint(A >= 18);

  Product prp1(prp);

  Product prp2(1);
  prp2.refine_with_congruence((A %= 18) / 20);

  prp1.difference_assign(prp2);

  bool cons_ok = prp1.is_empty();

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  if (!cons_ok)
    return false;

  prp2.difference_assign(prp1);

  Product known_prp(1);
  known_prp.refine_with_congruence((A %= 18) / 20);

  cons_ok = (prp2 == known_prp);

  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return cons_ok;
}

// difference_assign() where difference is not reduced.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Product prp(1);
  prp.refine_with_constraint(A >= 1);
  prp.refine_with_congruence((A %= 0) / 2);

  Product prp1(prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  Product prp2(1);
  prp2.refine_with_constraint(A > 1);
  prp2.refine_with_congruence((A %= 1) / 2);

  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  prp1.difference_assign(prp2);

  bool cons_ok = prp1.is_empty();

  print_congruences(prp1,
                    "*** prp1.difference_assign(prp2) congruences ***");
  print_constraints(prp1,
                    "*** prp1.difference_assign(prp2) constraints ***");

  return cons_ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
