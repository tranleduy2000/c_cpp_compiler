/* Test operator<<(std::ostream&, const Congruence_System&).
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

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;

  bool ok = (cgs.OK());

  std::stringstream ss;
  ss << cgs;
  ok &= (!ss.str().compare("true"));

  cgs.insert(A - 2*B %= 2);
  cgs.insert(2*A %= 4);

#define OUTPUT "A - 2*B = 0 (mod 1), 2*A = 0 (mod 1)"

  ss.str("");
  ss << cgs;
  ok &= (!ss.str().compare(OUTPUT));

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN

