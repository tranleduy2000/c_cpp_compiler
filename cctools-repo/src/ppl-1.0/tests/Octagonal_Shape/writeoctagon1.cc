/* Test operator<<(ostream&, const Octagonal_Shape&).
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
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);
  oc.add_constraint(x + y == 3);
  oc.add_constraint(x - y == 4);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "A - B = 4, A + B = 3";
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);
  oc.add_constraint(x - y == 5);
  oc.add_constraint(x + y == -1);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "A - B = 5, A + B = -1";
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);
  oc.add_constraint(-x - y <= 3);
  oc.add_constraint(-x + y <= 4);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "B - A <= 4, A + B >= -3";
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);
  oc.add_constraint(x - y <= 3);
  oc.add_constraint(x + y <= 4);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "A - B <= 3, A + B <= 4";
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);

  oc.add_constraint(x - y >= 4);
  oc.add_constraint(x - y <= 4);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "A - B = 4";
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oc(3);

  oc.add_constraint(x + y <= 3);
  oc.add_constraint(x + y >= 3);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "A + B = 3";
}

bool
test07() {
  TOctagonal_Shape oc(32, UNIVERSE);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "true";
}

bool
test08() {
  TOctagonal_Shape oc(32, EMPTY);

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "false";
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oc(2);

  oc.add_constraint(x + y == 2);
  oc.add_constraint(x - y == 1);

  (void) oc.is_empty();

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "2*A = 3, 2*B = 1, A - B = 1, A + B = 2";
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpq_class> oc(2);

  oc.add_constraint(x + y == 2);
  oc.add_constraint(x - y == 1);

  (void) oc.is_empty();

  std::stringstream s;
  s << oc;

  nout << "*** s << oc ***" << endl << "`" << s.str() << "'" << endl;

  return s.str() == "A = 3/2, B = 1/2, A - B = 1, A + B = 2";
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
END_MAIN
