/* Test Box::max_space_dimension().
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
  nout << Box<fl_r_oc>::max_space_dimension() << " (fl_r_oc)"
       << endl
       << Box<db_r_oc>::max_space_dimension() << " (db_r_oc)"
       << endl
       << Box<ld_r_oc>::max_space_dimension() << " (ld_r_oc)"
       << endl
       << Box<rt_r_oc>::max_space_dimension() << " (rt_r_oc)"
       << endl;

  if (Box<fl_r_oc>::max_space_dimension()
      < Box<db_r_oc>::max_space_dimension())
    return false;

  if (Box<db_r_oc>::max_space_dimension()
      < Box<ld_r_oc>::max_space_dimension())
    return false;

  // CHECKME: is this a valid assertion?
  if (Box<ld_r_oc>::max_space_dimension()
      < Box<rt_r_oc>::max_space_dimension())
    return false;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
