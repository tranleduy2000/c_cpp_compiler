/* Test Polyhedron::wrap_assign().
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

  C_Polyhedron ph(2);
  ph.add_constraint(x >= 255);
  ph.add_constraint(x <= 257);
  ph.add_constraint(y >= 255);
  ph.add_constraint(y <= 257);

  print_constraints(ph, "*** ph ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x + y <= 100);

  ph.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs, 16, false);

  C_Polyhedron known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 1);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph.wrap_assign(...) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(x >= 255);
  ph.add_constraint(x <= 257);
  ph.add_constraint(y >= 255);
  ph.add_constraint(y <= 257);

  print_constraints(ph, "*** ph ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x + y <= 100);

  ph.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs, 16, true);

  C_Polyhedron known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 1);
  known_result.add_constraint(x+y <= 100);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph.wrap_assign(...) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F16(test02);
END_MAIN
