/* Test Pointset_Powerset<PH>::BHZ03_widening_assign().
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

  typedef Pointset_Powerset<TBox> PSet;

  TBox p(2);
  TBox q(2);
  TBox r(2);
  TBox s(2);
  p.add_constraint(A >= 1);
  p.add_constraint(B == 0);
  q.add_constraint(A >= 2);
  q.add_constraint(A <= 7);
  q.add_constraint(B == 1);
  r.add_constraint(A >= 3);
  r.add_constraint(A <= 8);
  r.add_constraint(B == 1);
  s.add_constraint(A >= 1);
  s.add_constraint(A <= 6);
  s.add_constraint(B == 1);
  PSet P(2, EMPTY);
  P.add_disjunct(p);
  P.add_disjunct(q);
  P.add_disjunct(r);
  P.add_disjunct(s);
  PSet Q(2, EMPTY);
  Q.add_disjunct(p);
  Q.add_disjunct(q);
  Q.add_disjunct(s);

  using namespace IO_Operators;
  nout << "P = " << P << endl
       << "Q = " << Q << endl;

  PSet old_P = P;
  typedef void (TBox::*Widening_Member)(const TBox&, unsigned*);
  Widening_Member wm = &TBox::CC76_widening_assign;
  // FIXME: provide a widening certificate for CC76.
  P.BHZ03_widening_assign<H79_Certificate>(Q, widen_fun_ref(wm));

  nout << "P.BHZ03(Q, CC76)" << " = " << P << endl;

  return P.geometrically_covers(old_P) && P.geometrically_covers(Q);
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
