/* Test Box::CC76_widening_assign().
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

  TBox box1(2);
  box1.add_constraint(x <= 1);

  TBox box2(2);
  box2.add_constraint(-x <= 3);
  box2.add_constraint(x <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.CC76_widening_assign(box2);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 1);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.CC76_widening_assign(box2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 1);

  TBox box2(2);
  box2.add_constraint(-x <= 3);
  box2.add_constraint(x <= 0);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.widening_assign(box2);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 1);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.CC76_widening_assign(box2) ***");

  return ok;
}

bool
test03() {
  typedef TBox::interval_type::boundary_type tbt;
  // No stop point indeed.
  tbt stop_points;

  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x >= 1);
  box1.add_constraint(x <= 4);

  TBox box2(2);
  box2.add_constraint(x >= 3);
  box2.add_constraint(x <= 4);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.CC76_widening_assign(box2, &stop_points, &stop_points);

  Rational_Box known_result(2);
  known_result.add_constraint(x <= 4);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1.CC76_widening_assign(box2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
