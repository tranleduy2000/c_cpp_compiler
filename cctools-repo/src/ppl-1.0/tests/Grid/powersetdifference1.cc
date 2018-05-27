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
  p.add_congruence(y %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_congruence((x %= 1) / 3);
  q.add_congruence((y %= 1) / 2);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);

  Grid known_gr1(2);
  known_gr1.add_congruence((x %= 0) / 1);
  known_gr1.add_congruence((y %= 0) / 2);

  Grid known_gr2(2);
  known_gr2.add_congruence((x %= 2) / 3);
  known_gr2.add_congruence((y %= 1) / 2);

  Grid known_gr3(2);
  known_gr3.add_congruence((x %= 0) / 3);
  known_gr3.add_congruence((y %= 1) / 2);

  Pointset_Powerset<Grid> known_qs(2, EMPTY);
  known_qs.add_disjunct(known_gr1);
  known_qs.add_disjunct(known_gr2);
  known_qs.add_disjunct(known_gr3);

  bool ok = (ps == known_qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  return ok;
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

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  Grid known_gr(2);
  known_gr.add_congruence((x %= 0) / 1);

  Pointset_Powerset<Grid>::iterator i = ps.begin();
  return i->pointset() == known_gr;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_constraint(x == 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_congruence(x %= 1);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  return ps.begin() == ps.end();
}

bool
test04() {
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

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  Grid known_gr(2);
  known_gr.add_congruence((x %= 0) / 1);

  Pointset_Powerset<Grid>::iterator i = ps.begin();
  return i->pointset() == known_gr;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  Pointset_Powerset<Grid> ps(2, EMPTY);

  Grid q(2);
  q.add_congruence(y %= 0);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  return ps.begin() == ps.end();
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(y %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Pointset_Powerset<Grid> qs(2, EMPTY);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  Grid known_gr(p);

  Pointset_Powerset<Grid>::iterator i = ps.begin();
  return i->pointset() == known_gr;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(y %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Pointset_Powerset<Grid> qs(2, EMPTY);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  Grid known_gr(p);

  Pointset_Powerset<Grid>::iterator i = ps.begin();
  return i->pointset() == known_gr;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(x %= 0);
  p.add_congruence(y %= 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_congruence((x %= 1) / 3);
  q.add_congruence((y %= 1) / 2);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);

  Grid known_gr1(2);
  known_gr1.add_congruence((x %= 0) / 1);
  known_gr1.add_congruence((y %= 0) / 2);

  Grid known_gr2(2);
  known_gr2.add_congruence((x %= 2) / 3);
  known_gr2.add_congruence((y %= 1) / 2);

  Grid known_gr3(2);
  known_gr3.add_congruence((x %= 0) / 3);
  known_gr3.add_congruence((y %= 1) / 2);

  Pointset_Powerset<Grid> known_qs(2, EMPTY);
  known_qs.add_disjunct(known_gr1);
  known_qs.add_disjunct(known_gr2);
  known_qs.add_disjunct(known_gr3);

  bool ok = (ps == known_qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_constraint(x == 0);

  Pointset_Powerset<Grid> ps(2, EMPTY);
  ps.add_disjunct(p);

  Grid q(2);
  q.add_congruence((x %= 0) / 0);

  Pointset_Powerset<Grid> qs(2, EMPTY);
  qs.add_disjunct(q);

  print_congruences(ps, "*** ps ***");
  print_congruences(qs, "*** qs ***");

  ps.difference_assign(qs);
  print_congruences(ps, "*** ps.difference(qs) ***");

  return ps.is_empty();
}

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
