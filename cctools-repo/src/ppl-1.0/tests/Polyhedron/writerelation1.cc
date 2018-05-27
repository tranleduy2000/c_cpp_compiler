/* Test operator<<(std::ostream& s, const Poly_Con_Relation& r)
   and operator<<(std::ostream& s, const Poly_Gen_Relation& r).
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
  Poly_Con_Relation rel = Poly_Con_Relation::nothing();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "NOTHING";
}

bool
test02() {
  Poly_Con_Relation rel = Poly_Con_Relation::is_disjoint();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "IS_DISJOINT";
}

bool
test03() {
  Poly_Con_Relation rel = Poly_Con_Relation::strictly_intersects();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "STRICTLY_INTERSECTS";
}

bool
test04() {
  Poly_Con_Relation rel = Poly_Con_Relation::is_included();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "IS_INCLUDED";
}

bool
test05() {
  Poly_Con_Relation rel = Poly_Con_Relation::saturates();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "SATURATES";
}

bool
test06() {
  Poly_Gen_Relation rel = Poly_Gen_Relation::nothing();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "NOTHING";
}

bool
test07() {
  Poly_Gen_Relation rel = Poly_Gen_Relation::subsumes();
  std::stringstream s;
  using namespace IO_Operators;
  s << rel;
  nout << s.str() << endl;
  return s.str() == "SUBSUMES";
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
END_MAIN
