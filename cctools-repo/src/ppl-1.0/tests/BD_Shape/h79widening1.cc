/* Test BD_Shape::H79_widening_assign().
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

bool
test01() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(x <= 1);
  bds1.add_constraint(x - y <= 2);
  bds1.add_constraint(y - x <= 7);

  TBD_Shape bds2(2);
  bds2.add_constraint(x - y <= 2);
  bds2.add_constraint(-x <= 3);
  bds2.add_constraint(x <= 0);
  bds2.add_constraint(y - x <= 2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.H79_widening_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x - y <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
