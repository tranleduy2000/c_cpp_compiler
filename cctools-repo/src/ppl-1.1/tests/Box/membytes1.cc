/* Test the total_memory_in_bytes() and external_memory_in_bytes() methods.
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
#include <cmath>

namespace {

bool
test01() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box(3);
  box.refine_with_constraint(x >= 0);
  box.refine_with_constraint(x < 7);
  box.refine_with_constraint(z < 3);
  box.refine_with_constraint(x - y - 1 <= 0);
  box.refine_with_constraint(2*x - 2*y >= 0);
  box.refine_with_constraint(4*x - 2*y - z + 2 >= 0);

  using namespace IO_Operators;
  nout << "box = " << box << endl;

  const memory_size_type box_total_size = box.total_memory_in_bytes();
  const memory_size_type box_external_size = box.external_memory_in_bytes();

  nout << "box.total_memory_in_bytes() = " << box_total_size
       << endl
       << "box.external_memory_in_bytes() = " << box_external_size
       << endl
       << endl;

  return true;
}

#if PPL_SUPPORTED_DOUBLE

bool test02() {
  Variable x(0);
  Variable y(1);

  Pointset_Powerset<TBox> pbox(2, EMPTY);
  TBox box(2);

  typedef TBox::interval_type interval_type;
  typedef interval_type::boundary_type boundary_type;

  interval_type ix;
  interval_type iy;

  for (double d = -M_PI; d <= M_PI; d += 0.01) {
    ix.assign(EMPTY);
    ix.join_assign(boundary_type(d-0.015));
    ix.join_assign(boundary_type(d+0.015));

    // Many libm implementations only work with round-to-nearest.
    // See, e.g, http://sources.redhat.com/bugzilla/show_bug.cgi?id=3976
    restore_pre_PPL_rounding();
    boundary_type sdm = boundary_type(sin(d-0.015));
    boundary_type sdp = boundary_type(sin(d-0.015));
    set_rounding_for_PPL();

    iy.assign(EMPTY);
    iy.join_assign(sdm);
    iy.join_assign(sdp);

    box.set_interval(x, ix);
    box.set_interval(y, iy);

    pbox.add_disjunct(box);
  }

  using namespace IO_Operators;
  nout << "pbox = " << pbox << endl;

  nout << "pbox.total_memory_in_bytes() = "
       << pbox.total_memory_in_bytes()
       << endl
       << "pbox.external_memory_in_bytes() = "
       << pbox.external_memory_in_bytes()
       << endl;

  pbox.refine_with_constraint(x < y);

  using namespace IO_Operators;
  nout << "pbox = " << pbox << endl;

  nout << "pbox.total_memory_in_bytes() = "
       << pbox.total_memory_in_bytes()
       << endl
       << "pbox.external_memory_in_bytes() = "
       << pbox.external_memory_in_bytes()
       << endl;

  return true;
}

#endif // PPL_SUPPORTED_DOUBLE

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
#if PPL_SUPPORTED_DOUBLE
  DO_TEST(test02);
#endif
END_MAIN
