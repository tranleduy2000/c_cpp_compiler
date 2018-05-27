/* Test Box::Box(const Direct_Product<D1, D2>&, Complexity_Class).
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

namespace {

typedef Domain_Product<C_Polyhedron, Grid>::Direct_Product DProduct;
typedef Domain_Product<C_Polyhedron, Grid>::Smash_Product SProduct;
typedef Domain_Product<C_Polyhedron, Grid>::Constraints_Product CProduct;

bool
test01() {
  Variable A(0);

  DProduct dp(1);
  dp.refine_with_constraint(A <= 4);
  dp.refine_with_constraint(A >= 2);
  dp.refine_with_congruence((A %= 1) / 5);

  print_congruences(dp, "*** dp.congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  Rational_Box box(dp);

  Rational_Box known_box(1);
  known_box.refine_with_constraint(A >= 2);
  known_box.refine_with_constraint(A <= 4);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

bool
test02() {
  Variable A(0);

  DProduct dp(1);
  dp.refine_with_constraint(A <= 4);
  dp.refine_with_constraint(A >= 2);
  dp.refine_with_congruence((A %= 0) / 3);

  Rational_Box box(dp);
  box.refine_with_constraint(3*A >= 2);
  box.refine_with_constraint(A <= 6);

  print_congruences(dp, "*** dp.congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  Rational_Box known_box(1);
  known_box.refine_with_constraint(A >= 2);
  known_box.refine_with_constraint(A <= 4);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

bool
test03() {
  Variable A(0);

  SProduct sp(1);
  sp.refine_with_constraint(A >= 4);
  sp.refine_with_constraint(A <= 2);
  sp.refine_with_congruence((A %= 0) / 3);

  Rational_Box box(sp);

  print_congruences(sp, "*** sp.congruences ***");
  print_constraints(sp, "*** sp constraints ***");

  bool ok = box.is_empty();

  print_constraints(box, "*** box ***");

  return ok;
}

bool
test04() {
  Variable A(0);

  CProduct cp(1);
  cp.refine_with_constraint(A <= 4);
  cp.refine_with_constraint(A >= 4);
  cp.refine_with_congruence((A %= 0) / 3);

  Rational_Box box(cp);

  print_congruences(cp, "*** cp.congruences ***");
  print_constraints(cp, "*** cp constraints ***");

  Rational_Box known_box(1, EMPTY);

  bool ok = box.is_empty();

  print_constraints(box, "*** box ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
