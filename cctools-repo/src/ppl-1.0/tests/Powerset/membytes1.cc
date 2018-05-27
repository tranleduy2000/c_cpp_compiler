/* Test Pointset_Powerset<PH>::total_memory_in_bytes().
        Pointset_Powerset<PH>::external_memory_in_bytes().
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

// Powerset of C polyhedra: total_memory_in_bytes() and
// external_memory_in_bytes().
bool
test01() {
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);

  bool ok = c_ps.is_bottom();

  c_ps.add_disjunct(C_Polyhedron(1, UNIVERSE));

  bool ok1 = c_ps.is_top();

  c_ps.total_memory_in_bytes();
  c_ps.external_memory_in_bytes();

  return ok && ok1;
}

// Powerset of boxes: total_memory_in_bytes() and external_memory_in_bytes().
bool
test02() {
  Pointset_Powerset<TBox> pps_box(1, EMPTY);

  bool ok = pps_box.is_bottom();

  pps_box.add_disjunct(TBox(1, UNIVERSE));

  bool ok1 = pps_box.is_top();

  pps_box.total_memory_in_bytes();
  pps_box.external_memory_in_bytes();

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
