/* Test refine_with_congruence() and refine_with_congruences()..
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
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {

// Refine with an equality congruence.
bool
test01() {
  Variable A(0);

  Product prp(1);
  prp.refine_with_congruence((A == 0) / 0);

  DOMAIN2 gr(1);
  gr.refine_with_congruence((A == 0) / 0);
  Product known_result(gr);

  bool ok = (prp == known_result);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok && prp.OK();
}

// Refine with a proper congruence.
bool
test02() {
  Variable A(0);

  Product prp(1);
  prp.refine_with_congruence((A %= 0) / 4);

  DOMAIN2 gr(1);
  gr.refine_with_congruence((A %= 0) / 4);
  Product known_result(gr);

  bool ok = (prp == known_result);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok && prp.OK();
}

// Refine with congruences.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((2*A %= 1) / 4);
  cgs.insert((A + B %= 1) / 0);

  Product prp(2);
  prp.refine_with_congruences(cgs);

  DOMAIN2 gr(2);
  gr.refine_with_congruences(cgs);
  Product known_result(gr);

  bool ok = (prp == known_result);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok && prp.OK();
}

// Universe product with 0 dimensions.
bool
test04() {

  Product prp(0);
  prp.refine_with_congruence((Linear_Expression(0) == 1) / 2);

  Product known_result(0, EMPTY);

  bool ok = (prp == known_result);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok && prp.OK();
}

// Empty product with 0 dimensions.
bool
test05() {

  Product prp(0, EMPTY);
  prp.refine_with_congruence((Linear_Expression(0) == 0) / 0);

  Product known_result(0, EMPTY);

  bool ok = (prp == known_result);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok && prp.OK();
}

// refine_with_congruences
bool
test06() {

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);
  cgs.insert((B == 0) / 2);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.refine_with_congruences(cgs);

  Product known_dp(2);
  known_dp.refine_with_congruence((A %= 0) / 2);
  known_dp.refine_with_congruence((B == 0) / 2);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// refine_with_congruences
bool
test07() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);
  cgs.insert(A + B == 0);

  Product prp(2);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  prp.refine_with_congruences(cgs);
  bool ok = !prp.is_empty();

  Product known_prp(2);
  known_prp.refine_with_congruence((A %= 0) / 2);
  known_prp.refine_with_constraint(A + B == 0);

  ok = ok && (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// refine_with_congruences
bool
test08() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);

  Product prp(2);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  prp.refine_with_congruences(cgs);

  Product known_prp(2);
  known_prp.refine_with_congruence((A + B %= 0) / 2);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// refine_with_congruences
bool
test09() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((B %= 0) / 2);
  cgs.insert(A - B == 0);

  Product prp(2);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  prp.refine_with_congruences(cgs);
  bool ok = !prp.is_empty();

  Product known_prp(2);
  known_prp.refine_with_congruence((B %= 0) / 2);
  known_prp.refine_with_constraint(A - B == 0);

  ok = ok &&(prp == known_prp);

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
  DO_TEST(test09);
END_MAIN
