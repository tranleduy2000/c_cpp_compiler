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

const C_Polyhedron&
aux1_test01(unsigned n) {
  Variable x(0);
  Variable y(1);

  static std::vector<C_Polyhedron> p;
  if (p.size() < 5) {
    p.resize(5, C_Polyhedron(2));
    p[2].add_constraint(0 <= x);
    p[2].add_constraint(x <= 4);
    p[2].add_constraint(0 <= y);
    p[2].add_constraint(y <= 4);
    p[1] = p[2];
    p[1].add_constraint(x-y <= 3);
    p[0] = p[1];
    p[0].add_constraint(x+y >= 1);

    p[3].add_constraint(0 <= x);
    p[3].add_constraint(x <= 8);
    p[3].add_constraint(0 <= y);
    p[3].add_constraint(y <= 8);
    p[3].add_constraint(x+y <= 14);
    p[3].add_constraint(x-y >= -6);
    p[4] = p[3];
    p[3].add_constraint(5*x-y >= -2);
    p[3].add_constraint(x+3*y >= 3);
    p[4].add_constraint(4*x-y >= -3);
    p[4].add_constraint(x+2*y >= 2);
  }

  if (n >= p.size()) {
    unsigned new_size = p.size();
    while (n >= new_size)
      new_size *= 2;
    p.resize(p.size()*2);
  }

  if (p[n].is_universe()) {
    p[n] = aux1_test01(n-4);
    p[n].affine_image(x, 2*x);
    p[n].affine_image(y, 8 - 2*y);
  }

  return p[n];
}

Pointset_Powerset<C_Polyhedron>
aux2_test01(unsigned n) {
  Pointset_Powerset<C_Polyhedron> s(2, EMPTY);
  if (n == 0) {

    nout << "S0 = { P0 }" << endl;

    s.add_disjunct(aux1_test01(0));
    return s;
  }

  const int p_base = (n-1)/3*4;

  switch (n % 3) {
  case 1:

    nout << "S" << n << " = { "
	 << "P" << p_base + 1 << ", "
	 << "P" << p_base + 3 << " }" << endl;

    s.add_disjunct(aux1_test01(p_base + 1));
    s.add_disjunct(aux1_test01(p_base + 3));
    break;
  case 2:

    nout << "S" << n << " = { "
	 << "P" << p_base + 2 << ", "
	 << "P" << p_base + 3 << " }" << endl;

    s.add_disjunct(aux1_test01(p_base + 2));
    s.add_disjunct(aux1_test01(p_base + 3));
    break;
  case 0:

    nout << "S" << n << " = { "
	 << "P" << p_base + 2 << ", "
	 << "P" << p_base + 4 << " }" << endl;

    s.add_disjunct(aux1_test01(p_base + 2));
    s.add_disjunct(aux1_test01(p_base + 4));
    break;
  }
  return s;
}

void
aux3_test01(std::ostream& s, const Variable v) {
  s << char('x' + v.id());
}

bool
test01() {
  // Install the alternate output function.
  Variable::set_output_function(aux3_test01);

  Pointset_Powerset<C_Polyhedron> T = aux2_test01(0);

  using namespace Parma_Polyhedra_Library::IO_Operators;

  nout << "T0 = " << T << endl;

  bool converged = false;
  for (unsigned n = 1; !converged && n <= 20; ++n) {
    Pointset_Powerset<C_Polyhedron> Sn = aux2_test01(n);

    nout << "S" << n << " = " << Sn << endl;

    Sn.BGP99_extrapolation_assign
      (T, widen_fun_ref(&Polyhedron::H79_widening_assign), 2);

    nout << "T" << n << " = " << Sn << endl;

    if (Sn.definitely_entails(T))
      converged = true;
    else
      swap(Sn, T);
  }

  return !converged;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F16(test01);
END_MAIN
