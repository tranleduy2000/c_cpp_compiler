/* Test operator<<(std::ostream&, const Generator_System&).
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

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(A - B));
  gs.insert(point(A + C));
  gs.insert(ray(B + C));
  gs.insert(line(C));

  std::stringstream s;
  s << gs;

  nout << "*** s << gs ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "p(0), p(A - B), p(A + C), r(B + C), l(C)";
}

bool
test02() {
  C_Polyhedron ph(3, EMPTY);

  Generator_System gs = ph.generators();

  std::stringstream s;
  s << gs;

  nout << "*** s << gs ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "false";
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1 = 2*A + 4;
  e1 += B;
  Generator_System gs;
  gs.insert(ray(e1));
  gs.insert(point(3*A + B, 2));

  std::stringstream s;
  s << gs;

  nout << "*** s << gs ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "r(2*A + B), p((3*A + B)/2)";
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
