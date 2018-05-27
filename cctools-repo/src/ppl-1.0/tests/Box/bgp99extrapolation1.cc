/* Test Pointset_Powerset<PH>::BGP99_extrapolation_assign().
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
  typedef Pointset_Powerset<TBox> Box_Set;

  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A >= 0);
  TBox box2(2);
  box2.add_constraint(A >= 1);
  TBox box3(2);
  box3.add_constraint(A >= 2);
  TBox box4(2);
  box4.add_constraint(A >= 3);

  Box_Set box_set1(2, EMPTY);
  box_set1.add_disjunct(box1);
  box_set1.add_disjunct(box2);
  box_set1.add_disjunct(box3);

  Box_Set box_set2(box_set1);
  box_set1.add_disjunct(box4);

  using namespace IO_Operators;
  nout << "*** box_set1 ***" << endl
       << box_set1 << endl;
  nout << "*** box_set2 ***" << endl
       << box_set2 << endl;

  TBox box5(2);
  box5.add_constraint(A >= 2);
  box5.add_constraint(B >= 6);

  Box_Set known_result(2, EMPTY);
  known_result.add_disjunct(box1);
  known_result.add_disjunct(box2);
  known_result.add_disjunct(box5);

  typedef void (TBox::*Widening_Member)(const TBox&, unsigned*);
  Widening_Member wm = &TBox::CC76_widening_assign;
  box_set1.BGP99_extrapolation_assign(box_set2, widen_fun_ref(wm), 3);

  bool ok = box_set1.geometrically_equals(known_result);

  nout
    << "*** box_set1.BGP99_extrapolation_assign"
    << "(box_set2, widen_fun_ref(&CC76_widening_assign), 3) ***"
    << endl
    << box_set1 << endl;

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
