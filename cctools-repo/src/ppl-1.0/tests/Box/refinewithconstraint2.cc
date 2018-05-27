/* Test Box::refine_with_constraint(const Constraint&).
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

struct Unsigned_Integer_Closed_Interval_Info_Policy {
  const_bool_nodef(store_special, true);
  const_bool_nodef(store_open, false);
  const_bool_nodef(cache_empty, true);
  const_bool_nodef(cache_singleton, true);
  const_bool_nodef(cache_normalized, false);
  const_int_nodef(next_bit, 0);
  const_bool_nodef(may_be_empty, true);
  const_bool_nodef(may_contain_infinity, false);
  const_bool_nodef(check_empty_result, false);
  const_bool_nodef(check_inexact, false);
};

typedef Interval_Info_Bitset<unsigned int,
                             Unsigned_Integer_Closed_Interval_Info_Policy> Unsigned_Integer_Closed_Interval_Info;

typedef Interval<unsigned long long, Unsigned_Integer_Closed_Interval_Info>
Unsigned_Integer_Closed_Interval;

typedef Box<Unsigned_Integer_Closed_Interval> UILL_Box;

bool
test01() {
  Variable A(0);
  Variable B(1);

  UILL_Box box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(B >= 1);

  print_constraints(box, "*** box ***");

  box.refine_with_constraint(A == B);

  print_constraints(box, "*** box.refine_with_constraint(A == B) ***");

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 1);

  bool ok = check_result(box, known_result);

  print_constraints(known_result, "*** known_result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
