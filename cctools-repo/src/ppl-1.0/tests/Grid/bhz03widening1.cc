/* Test Pointset_Powerset<Grid>::BHZ03_widening_assign().
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

typedef Pointset_Powerset<Grid> GSet;

namespace {

// Joins of grids in powersets shows that series is stabilizing.
bool
test01() {
  Variable A(0);
  Variable B(1);

  GSet grs1(2, EMPTY);
  Grid gr1(2);
  gr1.add_congruence(A %= 0);
  gr1.add_congruence(B %= 0);
  grs1.add_disjunct(gr1);

  print_congruences<Grid>(grs1, "*** grs1 ***");

  GSet grs2(2, EMPTY);
  Grid gr2(2);
  gr2.add_congruence(A %= 0);
  grs2.add_disjunct(gr2);

  print_congruences<Grid>(grs2, "*** grs2 ***");

  GSet known_grs = grs2;

  grs2.BHZ03_widening_assign<Grid_Certificate>
    (grs1, widen_fun_ref(&Grid::widening_assign));

  bool ok = (grs2 == known_grs);

  print_congruences<Grid>(grs1, "*** grs2.BHZ03_widening_assign<Grid_Certificate>(grs1, widen_fun_ref(&Grid::widening_assign) ***");

  return ok;
}

// Widening falls back to a singleton join of the grids in the larger
// grid set.
bool
test02() {
  Variable A(0);
  Variable B(1);

  GSet grs1(2, EMPTY);
  Grid gr1(2);
  gr1.add_congruence(A - B %= 0);
  Grid gr2(2);
  gr2.add_congruence((A %= 0) / 2);
  grs1.add_disjunct(gr1);
  grs1.add_disjunct(gr2);

  print_congruences<Grid>(grs1, "*** grs1 ***");

  GSet grs2(2, EMPTY);
  Grid gr3(2);
  gr3.add_congruence(A - B %= 0);
  Grid gr4(2);
  gr4.add_congruence((A %= 0) / 2);
  grs2.add_disjunct(gr3);
  grs2.add_disjunct(gr4);

  print_congruences<Grid>(grs2, "*** grs2 ***");

  Grid known_gr = gr3;
  known_gr.upper_bound_assign(gr4);

  grs2.BHZ03_widening_assign<Grid_Certificate>
    (grs1, widen_fun_ref(&Grid::widening_assign));

  GSet known_grs(2, EMPTY);
  known_grs.add_disjunct(known_gr);

  bool ok = (grs2 == known_grs);

  print_congruences<Grid>(grs1, "*** grs2.BHZ03_widening_assign<Grid_Certificate>(grs1, widen_fun_ref(&Grid::widening_assign) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
