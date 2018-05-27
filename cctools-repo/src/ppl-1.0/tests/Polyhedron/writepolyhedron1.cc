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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(  A - B       >= 3);
  ph.add_constraint(    - B +   C >= 3);
  ph.add_constraint(  A - B       <= 1);
  ph.add_constraint(  A - B + 3*C >= 3);
  ph.add_constraint(3*A     + 2*C >= 3);

  std::stringstream s;
  s << ph;

  nout << "*** s << ph ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "false";
}

bool
test02() {
  NNC_Polyhedron ph1;

  C_Polyhedron ph(ph1.constraints());

  std::stringstream s;
  s << ph;

  nout << "*** s << ph ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "true";
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
