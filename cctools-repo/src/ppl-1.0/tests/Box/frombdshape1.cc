/* Test Box::Box(const BD_Shape&, Complexity_Class).
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

  TBD_Shape bd(2);
  bd.add_constraint(x <= 1);
  bd.add_constraint(x - y <= 2);
  bd.add_constraint(y - x <= 7);

  print_constraints(bd, "*** bd ***");

  TBox box(bd, SIMPLEX_COMPLEXITY);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y <= 8);

  bool ok = check_result(box, known_result);

  print_constraints(known_result, "*** known_result ***");
  print_constraints(box, "*** box ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bd(2);
  bd.add_constraint(x - y <= 2);
  bd.add_constraint(-x <= 3);
  bd.add_constraint(x <= 0);
  bd.add_constraint(y - x <= 2);

  print_constraints(bd, "*** bd ***");

  TBox box(bd, SIMPLEX_COMPLEXITY);

  Rational_Box known_result(2);
  known_result.add_constraint(x >= -3);
  known_result.add_constraint(x <= 0);
  known_result.add_constraint(y >= -5);
  known_result.add_constraint(y <= 2);

  bool ok = check_result(box, known_result);

  print_constraints(known_result, "*** known_result ***");
  print_constraints(box, "*** box ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
