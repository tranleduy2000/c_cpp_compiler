/* Test operator<<(std::ostream&, const Polyhedron&).
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

using namespace IO_Operators;

namespace {

bool
test01() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);
  Variable x4(3);

  NNC_Polyhedron ph(4);

  ph.add_constraint(     +x2-x3-x4 <= 0);
  ph.add_constraint(-  x1   +x3-x4 <  0);
  ph.add_constraint(+  x1   -x3-x4 <= 0);
  ph.add_constraint(-2*x1+x2+x3-x4 <  0);
  ph.add_constraint(           +x4 <= 1);
  ph.add_constraint(        +x3    <  1);
  ph.add_constraint(-  x1+x2+x3    <= 1);
  ph.add_constraint(        -x3    <  0);
  ph.add_constraint(-  x1          <= 0);
  ph.add_constraint(     -x2       <  0);
  ph.add_constraint(     +x2       <= 1);
  ph.add_constraint(+  x1          <  1);
  ph.add_constraint(+  x1-x2+x3+x4 <= 2);

  std::stringstream s;
  s << ph;

  nout << "*** s << ph ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "-A > -1, -A + C + D >= 0, -A + B - C - D >= -2, -B >= -1,"
    " -B + C + D >= 0, -C > -1, -D >= -1, 2*A - B - C + D > 0, C > 0, B > 0,"
    " A - B - C >= -1, A - C + D > 0, A >= 0";
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
