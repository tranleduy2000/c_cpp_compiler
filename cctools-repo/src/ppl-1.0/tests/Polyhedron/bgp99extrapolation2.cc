/* Test Pointset_Powerset<PH>::BGP99_extrapolation_assign().
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
#include <vector>

namespace {

// Same test as test02, but using C_Polyhedron.
// This overflows when using 8-bit coefficients.
bool
test01() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ps1_1(2);
  ps1_1.add_constraint(-A + B >= 5);
  ps1_1.add_constraint(A - B >= -13);
  ps1_1.add_constraint(A >= 3);
  C_Polyhedron ps1_2(2);
  ps1_2.add_constraint(-A + B >= 6);
  ps1_2.add_constraint(A - B >= -16);
  ps1_2.add_constraint(A >= 3);
  C_Polyhedron ps1_3(2);
  ps1_3.add_constraint(-A + B >= 7);
  ps1_3.add_constraint(A - B >= -20);
  ps1_3.add_constraint(A >= 4);
  C_Polyhedron ps1_4(2);
  ps1_4.add_constraint(-A + B >= 8);
  ps1_4.add_constraint(A - B >= -24);
  ps1_4.add_constraint(A >= 5);
  C_Polyhedron ps1_5(2);
  ps1_5.add_constraint(-A + B >= 10);
  ps1_5.add_constraint(A - B >= -28);
  ps1_5.add_constraint(A >= 6);
  C_Polyhedron ps1_6(2);
  ps1_6.add_constraint(-A + B >= 12);
  ps1_6.add_constraint(A - B >= -32);
  ps1_6.add_constraint(A >= 7);
  C_Polyhedron ps1_7(2);
  ps1_7.add_constraint(-A + B >= 2);
  ps1_7.add_constraint(A - B >= -4);
  ps1_7.add_constraint(A >= 0);
  C_Polyhedron ps1_8(2);
  ps1_8.add_constraint(-A + B >= 3);
  ps1_8.add_constraint(A - B >= -8);
  ps1_8.add_constraint(A >= 1);
  C_Polyhedron ps1_9(2);
  ps1_9.add_constraint(-A + B >= 4);
  ps1_9.add_constraint(A - B >= -12);
  ps1_9.add_constraint(A >= 2);

  Pointset_Powerset<C_Polyhedron> ps1(2, EMPTY);
  ps1.add_disjunct(ps1_1);
  ps1.add_disjunct(ps1_2);
  ps1.add_disjunct(ps1_3);
  ps1.add_disjunct(ps1_4);
  ps1.add_disjunct(ps1_5);
  ps1.add_disjunct(ps1_6);
  ps1.add_disjunct(ps1_7);
  ps1.add_disjunct(ps1_8);
  ps1.add_disjunct(ps1_9);

  C_Polyhedron ps2_1(2);
  ps2_1.add_constraint(-A + B >= 2);
  ps2_1.add_constraint(A - B >= -4);
  ps2_1.add_constraint(A >= 0);
  C_Polyhedron ps2_2(2);
  ps2_2.add_constraint(-A + B >= 3);
  ps2_2.add_constraint(A - B >= -8);
  ps2_2.add_constraint(A >= 1);
  C_Polyhedron ps2_3(2);
  ps2_3.add_constraint(-A + B >= 4);
  ps2_3.add_constraint(A - B >= -12);
  ps2_3.add_constraint(A >= 2);
  C_Polyhedron ps2_4(2);
  ps2_4.add_constraint(-A + B >= 6);
  ps2_4.add_constraint(A - B >= -16);
  ps2_4.add_constraint(A >= 3);

  Pointset_Powerset<C_Polyhedron> ps2(2, EMPTY);
  ps2.add_disjunct(ps2_1);
  ps2.add_disjunct(ps2_2);
  ps2.add_disjunct(ps2_3);
  ps2.add_disjunct(ps2_4);

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** ps1 ***" << endl
       << ps1 << endl;
  nout << "*** ps2 ***" << endl
       << ps2 << endl;

  ps1.BGP99_extrapolation_assign
    (ps2, widen_fun_ref(&Polyhedron::H79_widening_assign), 7);

  Pointset_Powerset<C_Polyhedron> known_result(2, EMPTY);
  C_Polyhedron kr_1(2);
  kr_1.add_constraint(A - B >= -16);
  kr_1.add_constraint(A >= 3);
  C_Polyhedron kr_2(2);
  kr_2.add_constraint(-A + B >= 3);
  kr_2.add_constraint(A >= 1);
  C_Polyhedron kr_3(2);
  kr_3.add_constraint(A - B >= -12);
  known_result.add_disjunct(kr_1);
  known_result.add_disjunct(kr_2);
  known_result.add_disjunct(kr_3);

  bool ok = ps1.geometrically_equals(known_result);

  nout
    << "*** ps1.BGP99_extrapolation_assign"
    << "(ps2, widen_fun_ref(&H79_widening_assign), 7) ***"
    << endl
    << ps1 << endl;

  return ok;
}

// Same test as test01, but using NNC_Polyhedron.
// This only overflows when using 8-bit coefficients with assertions on.
bool
test02() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ps1_1(2);
  ps1_1.add_constraint(-A + B >= 5);
  ps1_1.add_constraint(A - B >= -13);
  ps1_1.add_constraint(A >= 3);
  NNC_Polyhedron ps1_2(2);
  ps1_2.add_constraint(-A + B >= 6);
  ps1_2.add_constraint(A - B >= -16);
  ps1_2.add_constraint(A >= 3);
  NNC_Polyhedron ps1_3(2);
  ps1_3.add_constraint(-A + B >= 7);
  ps1_3.add_constraint(A - B >= -20);
  ps1_3.add_constraint(A >= 4);
  NNC_Polyhedron ps1_4(2);
  ps1_4.add_constraint(-A + B >= 8);
  ps1_4.add_constraint(A - B >= -24);
  ps1_4.add_constraint(A >= 5);
  NNC_Polyhedron ps1_5(2);
  ps1_5.add_constraint(-A + B >= 10);
  ps1_5.add_constraint(A - B >= -28);
  ps1_5.add_constraint(A >= 6);
  NNC_Polyhedron ps1_6(2);
  ps1_6.add_constraint(-A + B >= 12);
  ps1_6.add_constraint(A - B >= -32);
  ps1_6.add_constraint(A >= 7);
  NNC_Polyhedron ps1_7(2);
  ps1_7.add_constraint(-A + B >= 2);
  ps1_7.add_constraint(A - B >= -4);
  ps1_7.add_constraint(A >= 0);
  NNC_Polyhedron ps1_8(2);
  ps1_8.add_constraint(-A + B >= 3);
  ps1_8.add_constraint(A - B >= -8);
  ps1_8.add_constraint(A >= 1);
  NNC_Polyhedron ps1_9(2);
  ps1_9.add_constraint(-A + B >= 4);
  ps1_9.add_constraint(A - B >= -12);
  ps1_9.add_constraint(A >= 2);

  Pointset_Powerset<NNC_Polyhedron> ps1(2, EMPTY);
  ps1.add_disjunct(ps1_1);
  ps1.add_disjunct(ps1_2);
  ps1.add_disjunct(ps1_3);
  ps1.add_disjunct(ps1_4);
  ps1.add_disjunct(ps1_5);
  ps1.add_disjunct(ps1_6);
  ps1.add_disjunct(ps1_7);
  ps1.add_disjunct(ps1_8);
  ps1.add_disjunct(ps1_9);

  NNC_Polyhedron ps2_1(2);
  ps2_1.add_constraint(-A + B >= 2);
  ps2_1.add_constraint(A - B >= -4);
  ps2_1.add_constraint(A >= 0);
  NNC_Polyhedron ps2_2(2);
  ps2_2.add_constraint(-A + B >= 3);
  ps2_2.add_constraint(A - B >= -8);
  ps2_2.add_constraint(A >= 1);
  NNC_Polyhedron ps2_3(2);
  ps2_3.add_constraint(-A + B >= 4);
  ps2_3.add_constraint(A - B >= -12);
  ps2_3.add_constraint(A >= 2);
  NNC_Polyhedron ps2_4(2);
  ps2_4.add_constraint(-A + B >= 6);
  ps2_4.add_constraint(A - B >= -16);
  ps2_4.add_constraint(A >= 3);

  Pointset_Powerset<NNC_Polyhedron> ps2(2, EMPTY);
  ps2.add_disjunct(ps2_1);
  ps2.add_disjunct(ps2_2);
  ps2.add_disjunct(ps2_3);
  ps2.add_disjunct(ps2_4);

  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << "*** ps1 ***" << endl
       << ps1 << endl;
  nout << "*** ps2 ***" << endl
       << ps2 << endl;

  ps1.BGP99_extrapolation_assign
    (ps2, widen_fun_ref(&Polyhedron::H79_widening_assign), 7);

  Pointset_Powerset<NNC_Polyhedron> known_result(2, EMPTY);
  NNC_Polyhedron kr_1(2);
  kr_1.add_constraint(A - B >= -16);
  kr_1.add_constraint(A >= 3);
  NNC_Polyhedron kr_2(2);
  kr_2.add_constraint(-A + B >= 3);
  kr_2.add_constraint(A >= 1);
  NNC_Polyhedron kr_3(2);
  kr_3.add_constraint(A - B >= -12);
  known_result.add_disjunct(kr_1);
  known_result.add_disjunct(kr_2);
  known_result.add_disjunct(kr_3);

  bool ok = ps1.geometrically_equals(known_result);

  nout
    << "*** ps1.BGP99_extrapolation_assign"
    << "(ps2, widen_fun_ref(&H79_widening_assign), 7) ***"
    << endl
    << ps1 << endl;

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8A(test02);
END_MAIN
