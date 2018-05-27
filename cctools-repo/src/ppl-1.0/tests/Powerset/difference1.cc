/* Test Pointset_Powerset<PH>::difference_assign().
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

// Powerset of C polyhedra: difference_assign().
bool
test01() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<C_Polyhedron> c_ps1(2, UNIVERSE);

  Pointset_Powerset<C_Polyhedron> c_ps2(2);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(y >= 0);
  cs.insert(y <= 1);
  c_ps2.add_constraints(cs);

  using namespace IO_Operators;
  c_ps1.difference_assign(c_ps2);

  nout << c_ps1 << endl;

  return true;
}

// Powerset of C polyhedra: difference_assign().
bool
test02() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<C_Polyhedron> c_ps1(2, UNIVERSE);

  Pointset_Powerset<C_Polyhedron> c_ps2(2);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(y >= 0);
  cs.insert(y <= 1);
  c_ps2.add_constraints(cs);

  using namespace IO_Operators;
  c_ps1.difference_assign(c_ps2);

  nout << c_ps1 << endl;

  return true;
}

// Creating a rectangle.
C_Polyhedron
aux_test03(int lx, int ly, int dx, int dy) {
  Variable x(0);
  Variable y(1);
  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point((lx+0*dx)*x + (ly+0*dy)*y));
  ph.add_generator(point((lx+1*dx)*x + (ly+0*dy)*y));
  ph.add_generator(point((lx+1*dx)*x + (ly+1*dy)*y));
  ph.add_generator(point((lx+0*dx)*x + (ly+1*dy)*y));
  return ph;
}

// Powerset of C polyhedra: difference_assign(), meet_assign,
// upper_bound_assign() and pairwise_reduce().
bool
test03() {
  Pointset_Powerset<C_Polyhedron> cross(2, EMPTY);
  cross.add_disjunct(aux_test03(0, 3, 9, 3));
  cross.add_disjunct(aux_test03(3, 0, 3, 9));

  using namespace IO_Operators;
  nout << "cross = " << cross << endl;

  Pointset_Powerset<C_Polyhedron> squares(2, EMPTY);
  squares.add_disjunct(aux_test03(1, 4, 1, 1));
  squares.add_disjunct(aux_test03(4, 4, 1, 1));
  squares.add_disjunct(aux_test03(7, 4, 1, 1));
  squares.add_disjunct(aux_test03(4, 1, 1, 1));
  squares.add_disjunct(aux_test03(4, 7, 1, 1));

  nout << "squares = " << squares << endl;

  Pointset_Powerset<C_Polyhedron> difference = cross;
  difference.difference_assign(squares);

  nout << "cross - squares = " << difference << endl;

  Pointset_Powerset<C_Polyhedron> intersection = difference;
  intersection.meet_assign(squares);

  nout << "(cross - squares) inters squares = " << intersection << endl;

  // When using Pointset_Powerset<NNC_Polyhedron>, intersection will be
  // empty.  When using Pointset_Powerset<C_Polyhedron>,
  // intersection will consist of objects of affine dimension at most 1.
  bool ok1 = true;
  for (Pointset_Powerset<C_Polyhedron>::const_iterator
	 i = intersection.begin(), in_end = intersection.end();
       i != in_end; ++i)
    if (i->pointset().affine_dimension() > 1) {
      nout << "intersection contains " << i->pointset() << "," << endl
	   << "which is of affine dimension greater than 1" << endl;
      ok1 = false;
    }

  Pointset_Powerset<C_Polyhedron> re_union = difference;
  re_union.upper_bound_assign(squares);

  nout << "(cross - squares) union squares = " << re_union << endl;
  re_union.pairwise_reduce();
  nout << "<Above union pairwise reduced>  = " << re_union << endl;

  bool ok2 = re_union.geometrically_equals(cross);

  if (!ok2)
    nout << "Union does not give back the original!" << endl;

  return ok1 && ok2;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
