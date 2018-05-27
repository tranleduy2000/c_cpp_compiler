/* Test operator<<(std::ostream&, const Constraint_System&).
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

  Constraint_System cs;
  cs.insert(A - 2*B > 2);
  cs.insert(Linear_Expression(0) <= -7);
  cs.insert(A == 2);

  std::stringstream s;
  using namespace IO_Operators;
  s << cs;
  nout << "*** s << cs ***" << endl << s.str() << endl;
  return s.str() == "A - 2*B > 2, 0 >= 1, A = 2";
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;

  std::stringstream s;
  using namespace IO_Operators;
  s << cs;
  nout << "*** s << cs ***" << endl << s.str() << endl;
  return s.str() == "true";
}

bool
test03() {
  Variable A(0);

  Constraint_System cs;
  cs.insert(A > 2);
  cs.insert(Linear_Expression(0) >= -1);
  cs.insert(A == 2);

  std::stringstream s;
  using namespace IO_Operators;
  s << cs;
  nout << "*** s << cs ***" << endl << s.str() << endl;
  return s.str() == "A > 2, A = 2";
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
