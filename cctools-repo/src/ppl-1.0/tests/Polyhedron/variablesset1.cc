/* Test the Variables_Set class.
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
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);

  Variables_Set v_set(A, F);
  v_set.insert(C);
  v_set.insert(B);
  v_set.insert(G);

  Variables_Set known_result(A, G);

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** v_set ***" << endl << v_set << endl;
  nout << "*** known_result ***" << endl << known_result << endl;

  return v_set == known_result;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  Variables_Set v_set(F, A);

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** v_set ***" << endl << v_set << endl;

  return v_set.empty();
}

bool
test03() {
  Variable first(Variable::max_space_dimension()-1);
  Variable last(0);

  Variables_Set v_set(first, last);

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** v_set ***" << endl << v_set << endl;

  return v_set.empty();
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);
  Variable H(7);
  Variable I(8);

  Variables_Set vs1(A, C);
  Variables_Set vs2(C, I);

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** vs1 ***" << endl << vs1 << endl;
  nout << "*** vs2 ***" << endl << vs2 << endl;

  Variables_Set vs_union;
  Variables_Set vs_difference;
  Variables_Set vs_intersection;

  std::set_union(vs1.begin(), vs1.end(),
		 vs2.begin(), vs2.end(),
		 std::inserter(vs_union, vs_union.begin()));
  nout << "*** vs_union ***" << endl << vs_union << endl;
  if (vs_union != Variables_Set(A, I))
    return false;

  std::set_difference(vs1.begin(), vs1.end(),
		      vs2.begin(), vs2.end(),
		      std::inserter(vs_difference, vs_difference.begin()));
  nout << "*** vs_difference ***" << endl << vs_difference << endl;
  if (vs_difference != Variables_Set(A, B))
    return false;

  std::set_intersection(vs1.begin(), vs1.end(),
			vs2.begin(), vs2.end(),
			std::inserter(vs_intersection,
				      vs_intersection.begin()));
  nout << "*** vs_intersection ***" << endl << vs_intersection << endl;
  if (vs_intersection != Variables_Set(C))
    return false;

  return true;
}


bool
test05() {
  Variable A(0);

  Variables_Set v_set;

  if (!v_set.empty())
    return false;

  v_set.insert(Variable(A));
  if (v_set.space_dimension() != 1)
    return false;

  v_set.insert(Variable(Variable::max_space_dimension()-1));

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** v_set ***" << endl << v_set << endl;

  return v_set.space_dimension() == Variable::max_space_dimension();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
