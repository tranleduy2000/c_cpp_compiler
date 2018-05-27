/* Test Pointset_Powerset<Grid>.
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


bool
test01() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(x %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_congruence((x %= 1) / 3);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  return ps.geometrically_covers(qs) && !qs.geometrically_covers(ps);
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(x %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_constraint(x == 1);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  return ps.geometrically_covers(qs) && !qs.geometrically_covers(ps);
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(x %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_congruence(y %= 0);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  return !ps.geometrically_covers(qs) && !qs.geometrically_covers(ps);
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Pointset_Powerset<Grid> ps(2, EMPTY);

  Grid q(2);
  q.add_congruence(y %= 0);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  return !ps.geometrically_covers(qs) && qs.geometrically_covers(ps);
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  Grid p1(2);
  p1.add_congruence((x %= 0) / 3);
  Grid p2(2);
  p2.add_congruence((x %= 1) / 3);
  Grid p3(2);
  p3.add_congruence((x %= 2) / 3);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p1);
  ps.add_disjunct(p2);
  ps.add_disjunct(p3);

  Grid q(2);
  q.add_congruence(x %= 0);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  return ps.geometrically_covers(qs);
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  Grid p1(2);
  p1.add_congruence((x %= 0) / 3);
  Grid p2(2);
  p2.add_congruence((x %= 1) / 3);
  Grid p3(2);
  p3.add_congruence((x %= 2) / 3);
  p3.add_congruence((y %= 2) / 3);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p1);
  ps.add_disjunct(p2);
  ps.add_disjunct(p3);

  Grid q(2);
  q.add_congruence(x %= 0);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  return !ps.geometrically_covers(qs);
}

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
