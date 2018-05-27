/* Test Octagonal_Shape::is_empty() .
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

  TOctagonal_Shape oc1(2, EMPTY);

  bool empty1 = oc1.is_empty();

  nout << "*** oc1.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false ") << endl;

  TOctagonal_Shape oc2(2, UNIVERSE);

  bool empty2 = oc2.is_empty();

  nout << "*** oc2.is_empty() ***" << endl;
  nout << (empty2 ? "true" : "false") << endl;

  return empty1 && !empty2;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(3);

  oc.add_constraint(A == 0);
  oc.add_constraint(C >= 0);
  oc.add_constraint(B - C >= 1);

  print_constraints(oc, "*** oc ***");

  bool empty = oc.is_empty();

  nout << "*** oc.is_empty() ***" << endl;
  nout << (empty ? "true" : "false") << endl;

  return !empty;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oc1(5);
  oc1.add_constraint(A <= 3);
  oc1.add_constraint(D <= 3);
  oc1.add_constraint(B - A <= 0);
  oc1.add_constraint(C - A <= -2);
  oc1.add_constraint(E - A <= 2);
  oc1.add_constraint(-B <= 0);
  oc1.add_constraint(C - B <= 5);
  oc1.add_constraint(D - C <= -6);
  oc1.add_constraint(A - D <= 5);
  oc1.add_constraint(E - D <= 2);
  oc1.add_constraint(-E <= -5);
  oc1.add_constraint(C - E <= 7);

  print_constraints(oc1, "*** oc1 ***");

  bool empty = oc1.is_empty();

  nout << "*** oc1.is_empty() ***" << endl;
  nout << (empty ? "true" : "false") << endl;

  TOctagonal_Shape oc2(5);
  oc2.add_constraint(A <= 3);
  oc2.add_constraint(D <= 3);
  oc2.add_constraint(B - A <= 0);
  oc2.add_constraint(C - A <= 2);
  oc2.add_constraint(E - A <= 2);
  oc2.add_constraint(-B <= 0);
  oc2.add_constraint(C - B <= 5);
  oc2.add_constraint(D - C <= 6);
  oc2.add_constraint(A - D <= 5);
  oc2.add_constraint(E - D <= 2);
  oc2.add_constraint(-E <= 5);
  oc2.add_constraint(C - E <= 7);

  print_constraints(oc2, "*** oc2 ***");

  bool empty1 = oc2.is_empty();

  nout << "*** oc2.is_empty() ***" << endl;
  nout << (empty1 ? "true" : "false") << endl;

  return empty && !empty1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
