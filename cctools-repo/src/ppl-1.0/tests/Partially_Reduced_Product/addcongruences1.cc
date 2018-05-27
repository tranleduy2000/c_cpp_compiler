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
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {

// add_recycled_congruences
bool
test01() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);
  cgs.insert((A %= 0) / 0);

  Product prp(2);

  Congruence_System cgs_copy = cgs;
  Congruence_System cgs_copy2 = cgs;

  prp.add_recycled_congruences(cgs_copy);

  Grid gr(cgs_copy2);

  Product known_prp(gr);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

// add_recycled_congruences
bool
test02() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((B %= 0) / 2);
  cgs.insert((A %= 0) / 2);
  cgs.insert((A %= 0) / 1);
  cgs.insert(A - B == 0);

  Product prp(2);

  Congruence_System cgs_copy = cgs;
  Congruence_System cgs_copy2 = cgs;

  prp.add_recycled_congruences(cgs_copy);

  Grid gr(cgs_copy2);

  Product known_prp(gr);

  bool ok = (prp == known_prp);

  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp, "*** prp congruences ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
