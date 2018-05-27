/* Test Partially_Reduced_Product<>:: Shape_Preserving_Reduction()
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

typedef Domain_Product<Poly, Grid>::Shape_Preserving_Product SHPProduct;
namespace {

// Shape_Preserving_Reduction with non-strict constraints and
// equality found. Positive coefficients.
bool
test01() {
  Variable A(0);
  Variable B(1);

  SHPProduct shpp(2);
  Constraint_System cs;
  cs.insert(4*A >= 6);
  cs.insert(4*A <= 17);
  shpp.refine_with_constraints(cs);
  shpp.refine_with_congruence((A %= 0)/ 2);

  SHPProduct known_shpp(2);

  known_shpp.refine_with_constraint(A >= 2);
  known_shpp.refine_with_constraint(A <= 4);
  known_shpp.refine_with_congruence((A %= 0)/ 2);

  bool ok = shpp.OK() && shpp == known_shpp;

  print_congruences(shpp, "*** after ok check: shpp congruences ***");
  print_constraints(shpp, "*** after ok check: shpp constraints ***");

  return ok;
}

// Shape_Preserving_Reduction with non-strict constraints and
// constraints tightened. Negative coefficients.
bool
test02() {
  Variable A(0);
  Variable B(1);

  SHPProduct shpp(2);
  Constraint_System cs;
  cs.insert(4*A >= -5);
  cs.insert(4*A <= -1);
  shpp.refine_with_constraints(cs);
  shpp.refine_with_congruence((2*A %= 0)/ 1);

  SHPProduct known_shpp(2);

  known_shpp.refine_with_constraint(A >= -1);
  known_shpp.refine_with_constraint(2*A <= -1);
  known_shpp.refine_with_congruence((2*A %= 0)/ 1);

  bool ok = shpp.OK() && shpp == known_shpp;

  print_congruences(shpp, "*** after ok check: shpp congruences ***");
  print_constraints(shpp, "*** after ok check: shpp constraints ***");

  return ok;
}

// Shape_Preserving_Reduction with strict bounds.
bool
test03() {
  Variable A(0);
  Variable B(1);

  SHPProduct shpp(2);
  Constraint_System cs;
  cs.insert(A > 1);
  cs.insert(A < 5);
  shpp.refine_with_constraints(cs);
  shpp.refine_with_congruence((A %= 0)/ 2);

  SHPProduct known_shpp(2);

  known_shpp.refine_with_constraint(A >= 2);
  known_shpp.refine_with_constraint(A <= 4);
  known_shpp.refine_with_congruence((A %= 0)/ 2);

  bool ok = shpp.OK() && shpp == known_shpp;

  print_congruences(shpp, "*** after ok check: shpp congruences ***");
  print_constraints(shpp, "*** after ok check: shpp constraints ***");

  return ok;
}

// Shape_Preserving_Reduction where an equality is found, not found
// by the congruences reduction.
bool
test04() {
  Variable A(0);
  Variable B(1);

  SHPProduct shpp(2);
  Constraint_System cs;
  cs.insert(A + B >= 1);
  cs.insert(A + B <= 3);
  shpp.refine_with_constraints(cs);
  shpp.refine_with_congruence((A %= 0)/ 2);
  shpp.refine_with_congruence((B %= 0)/ 2);

  SHPProduct known_shpp(2);

  known_shpp.refine_with_constraint(A + B == 2);
  known_shpp.refine_with_congruence((A %= 0)/ 2);
  known_shpp.refine_with_congruence((B %= 0)/ 2);

  bool ok = shpp.OK() && shpp == known_shpp;

  print_congruences(shpp, "*** after ok check: shpp congruences ***");
  print_constraints(shpp, "*** after ok check: shpp constraints ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
