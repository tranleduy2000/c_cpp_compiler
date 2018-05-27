/* Test operator<<(ostream&, const BD_Shape&).
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
#include <sstream>

namespace {

bool
test01() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  std::stringstream s;
  using namespace IO_Operators;

  TBD_Shape bds(3, UNIVERSE);

  s << bds;
  if (s.str() != "true")
    return false;
  print_constraints(bds, "*** bds ***");
  s.str("");

  bds.add_constraint(x <= 3);

  s << bds;
  if (s.str() != "A <= 3")
    return false;
  print_constraints(bds, "*** bds ***");
  s.str("");

  bds.add_constraint(x - y <= 4);

  s << bds;
  if (s.str() != "A <= 3, A - B <= 4")
    return false;
  print_constraints(bds, "*** bds ***");
  s.str("");

  bds.add_constraint(-y <= -2);

  s << bds;
  if (s.str() != "A <= 3, B >= 2, A - B <= 4")
    return false;
  print_constraints(bds, "*** bds ***");
  s.str("");

  bds.add_constraint(x-z <= 0);

  s << bds;
  if (s.str() != "A <= 3, B >= 2, A - B <= 4, A - C <= 0")
    return false;
  print_constraints(bds, "*** bds ***");
  s.str("");

  return true;
}

bool
test02() {
  std::stringstream s;
  using namespace IO_Operators;

  TBD_Shape bds1(0, EMPTY);

  s << bds1;
  if (s.str() != "false")
    return false;
  print_constraints(bds1, "*** bds1 ***");
  s.str("");

  TBD_Shape bds2(0, UNIVERSE);

  s << bds2;
  if (s.str() != "true")
    return false;
  print_constraints(bds2, "*** bds2 ***");
  s.str("");

  TBD_Shape bds3(1, EMPTY);

  s << bds3;
  if (s.str() != "false")
    return false;
  print_constraints(bds3, "*** bds3 ***");
  s.str("");

  TBD_Shape bds4(1, UNIVERSE);

  s << bds4;
  if (s.str() != "true")
    return false;
  print_constraints(bds4, "*** bds4 ***");
  s.str("");

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN

