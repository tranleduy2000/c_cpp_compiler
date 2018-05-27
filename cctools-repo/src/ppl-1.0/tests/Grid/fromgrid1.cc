/* Test copy construction and assignment for grids.
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

// Assignment for universe and empty grids of the first ten dimensions.

bool
test01() {
  bool ok = true;
  for (unsigned int dim = 0; dim < 10; ++dim) {

    nout << "dimension: " << dim << endl;
    // Universe.
    Grid gr(dim);
    print_congruences(gr, "*** gr ***");

    Grid gr_copy(0);
    gr_copy = gr;

    Grid known_gr(dim);
    ok &= (gr_copy == known_gr);
    print_congruences(gr_copy, "*** gr_copy ***");

    if (ok) {
      // Empty.
      gr = Grid(dim, EMPTY);
      print_generators(gr, "*** gr ***");

      Grid gr_copy(0);
      gr_copy = gr;

      Grid known_gr(dim, EMPTY);
      ok &= (gr_copy == known_gr);
      print_generators(gr_copy, "*** gr_copy ***");
    }
  };

  return ok;
}

// Copy construct for universe and empty grids of the first ten dimensions.
bool
test02() {
  bool ok = true;
  for (unsigned int dim = 0; dim < 10; ++dim) {

    nout << "dimension: " << dim << endl;
    // Universe.
    Grid gr(dim);
    print_congruences(gr, "*** gr ***");

    Grid gr_copy(gr);

    Grid known_gr(dim);
    ok &= (gr_copy == known_gr);
    print_congruences(gr_copy, "*** gr_copy ***");

    if (ok) {
      // Empty.
      gr = Grid(dim, EMPTY);
      print_generators(gr, "*** gr ***");

      Grid gr_copy(gr);

      Grid known_gr(dim, EMPTY);
      ok &= (gr_copy == known_gr);
      print_generators(gr_copy, "*** gr_copy ***");
    }
  };

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
