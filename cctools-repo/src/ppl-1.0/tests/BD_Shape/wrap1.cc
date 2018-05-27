/* Test BD_Shape::wrap_assign().
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
  bd.refine_with_constraint(x >= 224);
  bd.refine_with_constraint(x <= 260);
  bd.refine_with_constraint(x-y >= 164);
  bd.refine_with_constraint(x-y <= 168);

  print_constraints(bd, "*** bd ***");

  Variables_Set vars(x, y);

  bd.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  TBD_Shape known_result(2);
  typedef TBD_Shape::coefficient_type T;
  if (!std::numeric_limits<T>::is_bounded
      || (std::numeric_limits<T>::max() >= 260
          && (std::numeric_limits<T>::is_iec559
              || std::numeric_limits<T>::min() <= -260))) {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(x <= 255);
    known_result.refine_with_constraint(y >= 56);
    known_result.refine_with_constraint(y <= 96);
    known_result.refine_with_constraint(x-y <= 168);
    known_result.refine_with_constraint(y-x <= 92);
  }
  else {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(y >= 0);
  }

  bool ok = (bd == known_result);

  print_constraints(bd, "*** bd.wrap_assign(...) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  TBD_Shape bd(2);
  bd.refine_with_constraint(x >= 224);
  bd.refine_with_constraint(x <= 260);
  bd.refine_with_constraint(x-y >= 164);
  bd.refine_with_constraint(x-y <= 168);

  print_constraints(bd, "*** bd ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  bd.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs);

  TBD_Shape known_result(2);
  typedef TBD_Shape::coefficient_type_base T;
  if (!std::numeric_limits<T>::is_bounded
      || (std::numeric_limits<T>::max() >= 260
          && (std::numeric_limits<T>::is_iec559
              || std::numeric_limits<T>::min() <= -260))) {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(x <= 4);
    known_result.refine_with_constraint(y >= 88);
    known_result.refine_with_constraint(y-x >= 88);
    known_result.refine_with_constraint(y-x <= 92);
  }
  else {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(y >= 0);
    known_result.refine_with_constraint(+x-y <= 0);
  }

  bool ok = (bd == known_result);

  print_constraints(bd, "*** bd.wrap_assign(...) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
END_MAIN
