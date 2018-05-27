/* Test BD_Shape copy construct, construction from other
   BD shapes and assignment.
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
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(A - B <= 18);
  TBD_Shape bds1(cs);

  TBD_Shape bds(bds1);

  BD_Shape<mpq_class> known_result(cs);

  bool ok = (check_result(bds, known_result));

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(A - B <= 18);
  TBD_Shape bds1(cs);

  TBD_Shape bds = bds1;

  BD_Shape<mpq_class> known_result(cs);

  bool ok = (check_result(bds, known_result));

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(A - B <= 18);
  TBD_Shape bds1(cs);

  TBD_Shape bds(1);
  bds = bds1;

  BD_Shape<mpq_class> known_result(cs);

  bool ok = (check_result(bds, known_result));

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(A - B <= 18);
  TBD_Shape bds1(cs);

  BD_Shape<mpq_class> bds(bds1);

  BD_Shape<mpq_class> known_result(cs);

  bool ok = (check_result(bds, known_result));

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test05() {
  Variable A(0);

  BD_Shape<mpq_class> bds_q(1, UNIVERSE);
  bds_q.refine_with_constraint(3*A <= 1);
  bds_q.refine_with_constraint(3*A >= 2);

  print_constraints(bds_q, "*** bds_q ***");

  BD_Shape<mpz_class> bds_z(bds_q);

  BD_Shape<mpz_class> known_result(bds_q.space_dimension(), EMPTY);

  bool ok = (check_result(bds_z, known_result));

  print_constraints(bds_z, "*** bds_z ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
