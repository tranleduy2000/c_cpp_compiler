/* Test BD_Shape::limited_H79_extrapolation_assign().
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

  Constraint_System cs1;
  cs1.insert(x <= 1);
  cs1.insert(y >= 4);

  TBD_Shape bds1(cs1);

  print_constraints(bds1, "*** bds1 ***");

  Constraint_System cs2;
  cs2.insert(x == 0);
  cs2.insert(y >= 5);

  TBD_Shape bds2(cs2);

  print_constraints(bds2, "*** bds2 ***");

  Constraint_System cs;
  cs.insert(x >= 20);
  cs.insert(y >= 3);

  print_constraints(cs, "*** cs ***");

  bds1.limited_H79_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(y >= 3);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.limited_H79_extrapolation_assign(bds2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
