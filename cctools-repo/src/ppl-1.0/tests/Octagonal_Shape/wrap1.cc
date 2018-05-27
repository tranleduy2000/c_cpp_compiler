/* Test Octagonal_Shape::wrap_assign().
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
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x >= 224);
  oct.refine_with_constraint(x-y <= 160);
  oct.refine_with_constraint(x+y <= 416);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  TOctagonal_Shape known_result(2);
  typedef TOctagonal_Shape::coefficient_type T;
  if (!std::numeric_limits<T>::is_bounded
      || (std::numeric_limits<T>::max() >= 510
          && (std::numeric_limits<T>::is_iec559
              || std::numeric_limits<T>::min() <= -510))) {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(x <= 255);
    known_result.refine_with_constraint(y >= 64);
    known_result.refine_with_constraint(y <= 192);
    known_result.refine_with_constraint(+x-y <= 160);
    known_result.refine_with_constraint(-x+y <= 160);
    known_result.refine_with_constraint(+x+y >= 96);
    known_result.refine_with_constraint(+x+y <= 416);
  }
  else {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(y >= 0);
  }

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x >= 224);
  oct.refine_with_constraint(x-y <= 160);
  oct.refine_with_constraint(x+y <= 416);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs);

  TOctagonal_Shape known_result(2);
  typedef TOctagonal_Shape::coefficient_type_base T;
  if (!std::numeric_limits<T>::is_bounded
      || (std::numeric_limits<T>::max() >= 510
          && (std::numeric_limits<T>::is_iec559
              || std::numeric_limits<T>::min() <= -510))) {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(y >= 96);
    known_result.refine_with_constraint(-x+y >= 96);
    known_result.refine_with_constraint(+x+y <= 160);
  }
  else {
    known_result.refine_with_constraint(x >= 0);
    known_result.refine_with_constraint(y >= 0);
    known_result.refine_with_constraint(+x-y <= 0);
  }

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

#if 0
bool
test03() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);
  oct.refine_with_constraint(-64 <= x);
  oct.refine_with_constraint(x <= 448);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(y >= 120);
  known_result.refine_with_constraint(y <= 184);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);
  oct.refine_with_constraint(-64 <= x);
  oct.refine_with_constraint(x <= 448);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED, &cs);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(y >= 120);
  known_result.refine_with_constraint(y <= 184);
  known_result.refine_with_constraint(x <= y);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);
  oct.refine_with_constraint(-64 <= x);
  oct.refine_with_constraint(x <= 448);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(x + 1024 == 8*y);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);
  oct.refine_with_constraint(-64 <= x);
  oct.refine_with_constraint(x <= 448);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE, &cs);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(7*x <= 1024);
  known_result.refine_with_constraint(x + 1024 == 8*y);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(y >= 0);
  known_result.refine_with_constraint(y <= 255);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(y <= 255);
  known_result.refine_with_constraint(x <= y);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);
  oct.refine_with_constraint(-64 <= x);
  oct.refine_with_constraint(x <= 448);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, 0, 32, false);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(x + 24*y >= 3072);
  known_result.refine_with_constraint(193*x + 504*y <= 129792);
  known_result.refine_with_constraint(x - 8*y >= -1280);
  known_result.refine_with_constraint(x - 8*y <= -768);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);
  oct.refine_with_constraint(-64 <= x);
  oct.refine_with_constraint(x <= 448);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs, 32, false);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(x <= y);
  known_result.refine_with_constraint(x - 8*y >= -1280);
  known_result.refine_with_constraint(x - 8*y <= -1024);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, 0, 32, false);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(x <= 255);
  known_result.refine_with_constraint(y >= 0);
  known_result.refine_with_constraint(y <= 255);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);
  TOctagonal_Shape oct(2);
  oct.refine_with_constraint(x + 1024 == 8*y);

  print_constraints(oct, "*** oct ***");

  Variables_Set vars(x, y);

  Constraint_System cs;
  cs.insert(x <= y);

  oct.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS, &cs, 32, false);

  TOctagonal_Shape known_result(2);
  known_result.refine_with_constraint(x >= 0);
  known_result.refine_with_constraint(y <= 255);
  known_result.refine_with_constraint(x <= y);

  bool ok = (oct == known_result);

  print_constraints(oct, "*** oct.wrap_assign(...) ***");

  return ok;
}
#endif

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
END_MAIN
