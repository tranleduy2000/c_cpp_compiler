/* Test Pointset_Powerset::simplify_using_context_assign().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(3, EMPTY);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k == 0);

  ps1.add_disjunct(ph);
  ps2.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k >= 1);
  ph.add_constraint(k <= 10);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(3, EMPTY);
  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(k == 0);
  known_result.add_disjunct(ph);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  return ok;
}

bool
test02() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(3, EMPTY);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k >= 1);
  ph.add_constraint(k <= 10);

  ps1.add_disjunct(ph);
  ps2.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k == 0);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(3, EMPTY);
  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(k >= 1);
  known_result.add_disjunct(ph);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  return ok;
}

bool
test03() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(3, EMPTY);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k == 0);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i <= 25);
  ph.add_constraint(j <= 25);
  ph.add_constraint(i + j >= 25);
  ph.add_constraint(k == 0);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(3, EMPTY);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator it = ps1.begin(),
         ps1_end = ps1.end(); it != ps1_end; ++it)
    print_constraints(it->pointset());

  return ok;
}

bool
test04() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(3, EMPTY);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k == 0);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i <= 25);
  ph.add_constraint(j <= 25);
  ph.add_constraint(i + j >= 25);
  ph.add_constraint(k == 0);

  ps2.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 0);
  ph.add_constraint(i <= 2);
  ph.add_constraint(j >= 2);
  ph.add_constraint(j <= 9);
  ph.add_constraint(k == 0);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(3, EMPTY);
  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j <= 10);
  known_result.add_disjunct(ph);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator it = ps1.begin(),
         ps1_end = ps1.end(); it != ps1_end; ++it)
    print_constraints(it->pointset());

  return ok;
}

bool
test05() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(3, EMPTY);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 1);
  ph.add_constraint(i <= 10);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k == 0);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 28);
  ph.add_constraint(i <= 31);
  ph.add_constraint(j >= 1);
  ph.add_constraint(j <= 10);
  ph.add_constraint(k == 0);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i <= 25);
  ph.add_constraint(j <= 25);
  ph.add_constraint(i + j >= 25);
  ph.add_constraint(k == 0);

  ps2.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 30);
  ph.add_constraint(i <= 32);
  ph.add_constraint(j >= 2);
  ph.add_constraint(j <= 9);
  ph.add_constraint(k == 0);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(3, EMPTY);
  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i >= 28);
  ph.add_constraint(i <= 31);
  known_result.add_disjunct(ph);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator it = ps1.begin(),
         ps1_end = ps1.end(); it != ps1_end; ++it)
    print_constraints(it->pointset());

  return ok;
}

bool
test06() {
  Variable i(0);
  Variable j(1);
  Variable k(2);

  C_Polyhedron ph(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(3, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(3, EMPTY);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i == 1);
  ph.add_constraint(j + 1 == 0);
  ph.add_constraint(k == 3);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(i == 1);
  ph.add_constraint(j + k == 2);
  ph.add_constraint(k >= 0);
  ph.add_constraint(k <= 3);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(3, EMPTY);
  ph = C_Polyhedron(3, UNIVERSE);
  ph.add_constraint(k == 3);
  // ph.add_constraint(j + 1 == 0);
  known_result.add_disjunct(ph);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator it = ps1.begin(),
         ps1_end = ps1.end(); it != ps1_end; ++it)
    print_constraints(it->pointset());

  return ok;
}

bool
test07() {
  Variable i(0);
  Variable j(1);

  C_Polyhedron ph(2, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(2, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(2, EMPTY);

  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(i <= j);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(i == j);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(2, UNIVERSE);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator it = ps1.begin(),
         ps1_end = ps1.end(); it != ps1_end; ++it)
    print_constraints(it->pointset());

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(2, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(2, EMPTY);

  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(A >= 0);

  ps1.add_disjunct(ph);

  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(A <= 0);

  ps2.add_disjunct(ph);

  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(B >= 0);

  ps2.add_disjunct(ph);

  Pointset_Powerset<C_Polyhedron> known_result(2, EMPTY);
  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(A >= 0);
  known_result.add_disjunct(ph);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator i = ps1.begin(),
         iend = ps1.end(); i != iend; ++i)
    print_constraints(i->pointset());

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps1(2, EMPTY);
  Pointset_Powerset<C_Polyhedron> ps2(2, EMPTY);

  // Populate ps1 with a single square.
  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(A >= 10);
  ph.add_constraint(A <= 40);
  ph.add_constraint(B >= 10);
  ph.add_constraint(B <= 40);

  ps1.add_disjunct(ph);

  nout << "Pointset_Powerset to be simplified:\n";
  for (Pointset_Powerset<C_Polyhedron>::const_iterator i = ps1.begin(),
         iend = ps1.end(); i != iend; ++i) {
    print_constraints(i->pointset());
    nout << "\n";
  }

  // Populate ps2 with four squares intersecting the single square above.
  ph = C_Polyhedron(2, UNIVERSE);
  ph.add_constraint(A >= 0);
  ph.add_constraint(A <= 20);
  ph.add_constraint(B >= 0);
  ph.add_constraint(B <= 20);

  ps2.add_disjunct(ph);

  ph.affine_image(B, B + 30);
  ps2.add_disjunct(ph);

  ph.affine_image(A, A + 30);
  ps2.add_disjunct(ph);

  ph.affine_image(B, B - 30);
  ps2.add_disjunct(ph);

  nout << "\nPointset_Powerset to be used as context:\n";
  for (Pointset_Powerset<C_Polyhedron>::const_iterator i = ps2.begin(),
         iend = ps2.end(); i != iend; ++i) {
    print_constraints(i->pointset());
    nout << "\n";
  }

  Pointset_Powerset<C_Polyhedron> known_result(ps1);

  ps1.simplify_using_context_assign(ps2);

  bool ok = (ps1 == known_result);

  for (Pointset_Powerset<C_Polyhedron>::const_iterator i = ps1.begin(),
         iend = ps1.end(); i != iend; ++i) {
    print_constraints(i->pointset());
    nout << "\n";
  }

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST_F8A(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
