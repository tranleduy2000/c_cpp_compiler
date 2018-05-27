/* Test Polyhedron::affine_image().
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

void
aux_test01(std::ostream& s, const Variable v) {
  s << char('i' + v.id());
}

bool
test01() {
  Variable i(0);
  Variable j(1);

  // Install the alternate output function.
  Variable::set_output_function(aux_test01);

  NNC_Polyhedron p1(2);
  p1.add_constraint(j == 0);
  p1.add_constraint(i >= 0);

  NNC_Polyhedron p2(2);
  p2.add_constraint(j == 0);
  p2.add_constraint(-i > 0);

  print_constraints(p1, "*** p1 ***");
  print_constraints(p2, "*** p2 ***");

  p1.affine_image(j, i+2);
  p2.affine_image(j, i);

  print_constraints(p1, "*** p1.affine_image(j, i+2) ***");
  print_constraints(p2, "*** p2.affine_image(j, i) ***");

  p1.upper_bound_assign(p2);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(i - j >= -2);
  known_result.add_constraint(-i + j >= 0);

  bool ok = (p1 == known_result);

  print_constraints(p1, "*** p1.upper_bound_assign(p2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
