/* Test Box::Box(dimension_type, Degenerate_Element).
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
  TBox box(0, EMPTY);

  bool ok = (box.OK() && box.is_empty() && box.space_dimension() == 0);

  print_constraints(box, "*** box ***");

  return ok;
}

bool
test02() {
  TBox box(0, UNIVERSE);

  bool ok = (box.OK() && box.is_universe() && box.space_dimension() == 0);

  print_constraints(box, "*** box ***");

  return ok;
}

bool
test03() {
  TBox box(4, EMPTY);

  bool ok = (box.OK() && box.is_empty() && box.space_dimension() == 4);

  print_constraints(box, "*** box ***");

  return ok;
}

bool
test04() {
  TBox box(4, UNIVERSE);

  bool ok = (box.OK() && box.is_universe() && box.space_dimension() == 4);

  print_constraints(box, "*** box ***");

  return ok;
}

bool
test05() {
  try {
    TBox box(not_a_dimension(), EMPTY);
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test06() {
  try {
    TBox box(not_a_dimension(), UNIVERSE);
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
