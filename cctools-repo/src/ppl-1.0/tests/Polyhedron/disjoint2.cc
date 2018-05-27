/* Test Polyhedron::is_disjoint_from(const Polyhedron& y).
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

NNC_Polyhedron
half_strip(const Generator& p,
	   const Linear_Expression& e,
	   bool closed = true) {
  assert((p.is_point() && closed) || (p.is_closure_point() && ! closed));

  Linear_Expression e1(p);
  e1 += 3*Variable(0);

  Generator_System gs;
  gs.insert(p);
  gs.insert(ray(e));
  if (closed)
    gs.insert(point(e1, p.divisor()));
  else {
    gs.insert(closure_point(e1, p.divisor()));
    e1 -= Variable(0);
    e1 += e.coefficient(Variable(1)) * p.divisor() * Variable(1);
    gs.insert(point(e1));
  }
  NNC_Polyhedron ph(gs);
  return ph;
}

bool
test01() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1 = half_strip(point(A + B), B);

  NNC_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point(3*A + B));
  ph2.add_generator(closure_point(2*A + B));
  ph2.add_generator(closure_point(4*A + 3*B));
  ph2.add_generator(ray(A - B));

  bool disjoint = ph1.is_disjoint_from(ph2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  return !disjoint;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1 = half_strip(point(A + B), B);
  NNC_Polyhedron ph2 = half_strip(closure_point(4*A + B), B, false);

  bool disjoint = ph1.is_disjoint_from(ph2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  return disjoint;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1 = half_strip(point(A + B), B);
  NNC_Polyhedron ph2 = half_strip(closure_point(A + B), -B, false);

  bool disjoint = ph1.is_disjoint_from(ph2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  return disjoint;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1 = half_strip(point(), B);

  NNC_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point(-2*A - 2*B));
  ph2.add_generator(closure_point(2*A - 2*B));
  ph2.add_generator(closure_point(-2*A + 2*B));
  ph2.add_generator(ray(-A - B));

  bool disjoint = ph1.is_disjoint_from(ph2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  return disjoint;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
