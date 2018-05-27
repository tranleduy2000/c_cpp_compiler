/* Test the total_memory_in_bytes() and external_memory_in_bytes() methods.
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

namespace test01_namespace {

void
add_constraint(C_Polyhedron& ph, const Constraint& c) {
  const memory_size_type ph_memory_before = ph.total_memory_in_bytes();
  const memory_size_type c_memory = c.total_memory_in_bytes();
  ph.add_constraint(c);
  const memory_size_type ph_memory_after = ph.total_memory_in_bytes();

  nout << ph_memory_before
       << " + " << c_memory
       << " -> " << ph_memory_after
       << endl;
}

void
add_generator(C_Polyhedron& ph, const Generator& g) {
  const memory_size_type ph_memory_before = ph.total_memory_in_bytes();
  const memory_size_type g_memory = g.total_memory_in_bytes();
  ph.add_generator(g);
  const memory_size_type ph_memory_after = ph.total_memory_in_bytes();

  nout << ph_memory_before
       << " + " << g_memory
       << " -> " << ph_memory_after
       << endl;
}

void
minimize(C_Polyhedron& ph) {
  const memory_size_type ph_memory_before = ph.total_memory_in_bytes();
  (void) ph.minimized_generators();
  const memory_size_type ph_memory_after = ph.total_memory_in_bytes();

  nout << ph_memory_before
       << " -m-> " << ph_memory_after
       << endl;
}

} // namespace test01_namespace

bool
test01() {
  using namespace test01_namespace;

  Variable x(0);
  Variable y(1);
  Variable z(2);

  const memory_size_type x_total_size = x.total_memory_in_bytes();
  const memory_size_type x_external_size = x.external_memory_in_bytes();

  nout << "*** Size of variables ***"
       << endl
       << "x.total_memory_in_bytes() = " << x_total_size
       << endl
       << "x.external_memory_in_bytes() = " << x_external_size
       << endl << endl;
  nout << "*** Size of linear expressions ***"
       << endl;

  Linear_Expression le(0);
  memory_size_type le_total_size = le.total_memory_in_bytes();
  memory_size_type le_external_size = le.external_memory_in_bytes();

  using namespace IO_Operators;
  nout << "(" << le << ").total_memory_in_bytes() = " << le_total_size
       << endl
       << "(" << le << ").external_memory_in_bytes() = " << le_external_size
       << endl;

  le += x;
  le_total_size = le.total_memory_in_bytes();
  le_external_size = le.external_memory_in_bytes();

  nout << "(" << le << ").total_memory_in_bytes() = " << le_total_size
       << endl
       << "(" << le << ").external_memory_in_bytes() = " << le_external_size
       << endl;

  le += 2*y;
  le_total_size = le.total_memory_in_bytes();
  le_external_size = le.external_memory_in_bytes();

  nout << "(" << le << ").total_memory_in_bytes() = " << le_total_size
       << endl
       << "(" << le << ").external_memory_in_bytes() = " << le_external_size
       << endl;

  le += 4*z;
  le_total_size = le.total_memory_in_bytes();
  le_external_size = le.external_memory_in_bytes();

  nout << "(" << le << ").total_memory_in_bytes() = " << le_total_size
       << endl
       << "(" << le << ").external_memory_in_bytes() = " << le_external_size
       << endl << endl;

  nout << "*** Adding constraints to a polyhedron ***" << endl;

  C_Polyhedron ph(3);
  add_constraint(ph, 4*x - 2*y - z + 14 >= 0);
  add_constraint(ph, 4*x + 2*y - z + 2 >= 0);
  add_constraint(ph, x + y - 1 >= 0);
  add_constraint(ph, x + y + 2*z - 5 >= 0);
  minimize(ph);
  add_constraint(ph, x + 1 >= 0);
  add_constraint(ph, x + z - 1 >= 0);
  add_constraint(ph, 2*x + y -2*z + 7 >= 0);
  add_constraint(ph, x - y + 2*z + 1 >= 0);
  minimize(ph);
  add_constraint(ph, x - y + 5 >= 0);
  add_constraint(ph, 2*x - y - 2*z + 13 >= 0);
  add_constraint(ph, -2*x - y + 2*z + 1 >= 0);
  add_constraint(ph, -x + y - 1 >= 0);
  minimize(ph);
  add_constraint(ph, -x + y -2*z + 7 >= 0);
  add_constraint(ph, -4*x + 2*y + z - 4 >= 0);
  add_constraint(ph, -2*x + y + 2*z - 5 >= 0);
  add_constraint(ph, -x + 1 >= 0);
  minimize(ph);
  add_constraint(ph, -x - z + 5 >= 0);
  add_constraint(ph, -4*x - 2*y + z + 8 >= 0);
  add_constraint(ph, -x - y + 5 >= 0);
  add_constraint(ph, -x - y -2*z +13 >= 0);
  minimize(ph);

  const memory_size_type ph_total_size = ph.total_memory_in_bytes();
  const memory_size_type ph_external_size = ph.external_memory_in_bytes();
  const Constraint_System& cs = ph.constraints();
  const memory_size_type cs_total_size = cs.total_memory_in_bytes();
  const memory_size_type cs_external_size = cs.external_memory_in_bytes();
  const Generator_System& gs = ph.generators();
  const memory_size_type gs_total_size = gs.total_memory_in_bytes();
  const memory_size_type gs_external_size = gs.external_memory_in_bytes();

  nout << endl;

  nout << "*** Size of the user-visible polyhedra components ***"
       << endl
       << "ph.total_memory_in_bytes() = " << ph_total_size
       << endl
       << "cs.total_memory_in_bytes() = " << cs_total_size
       << endl
       << "gs.total_memory_in_bytes() = " << gs_total_size
       << endl
       << "ph.external_memory_in_bytes() = " << ph_external_size
       << endl
       << "cs.external_memory_in_bytes() = " << cs_external_size
       << endl
       << "gs.external_memory_in_bytes() = " << gs_external_size
       << endl << endl;
  nout << "*** Adding generators to a polyhedron ***" << endl;

  C_Polyhedron qh(3, EMPTY);
  unsigned n = 0;
  for (Generator_System::const_iterator i = gs.begin(),
	 gs_end = gs.end(); i != gs_end; ++i) {
    add_generator(qh, *i);
    if (++n % 4 == 0)
      minimize(qh);
  }

  nout << endl;

  nout << "*** Size of a constraint system vs size of contained constraints"
       << endl
       << "cs.total_memory_in_bytes() = " << cs_total_size
       << endl;

  memory_size_type cs_elements_size = 0;
  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); i != cs_end; ++i)
    cs_elements_size += i->total_memory_in_bytes();

  nout << "Sum of sizes of contained constraints = " << cs_elements_size
       << endl << endl;

  nout << "*** Size of a generator system vs size of contained generators"
       << endl
       << "gs.total_memory_in_bytes() = " << gs_total_size
       << endl;

  memory_size_type gs_elements_size = 0;
  for (Generator_System::const_iterator i = gs.begin(),
	 gs_end = gs.end(); i != gs_end; ++i)
    gs_elements_size += i->total_memory_in_bytes();

  nout << "Sum of sizes of contained generators = " << gs_elements_size
       << endl << endl;

  return true;
}

bool test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  C_Polyhedron ph(3);
  ph.add_constraint(4*x - 2*y - z + 14 >= 0);
  ph.add_constraint(4*x + 2*y - z + 2 >= 0);
  ph.add_constraint(x + y - 1 >= 0);
  ph.add_constraint(x + y + 2*z - 5 >= 0);

  const memory_size_type ph_total_size = ph.total_memory_in_bytes();
  const memory_size_type ph_external_size = ph.external_memory_in_bytes();

  Determinate<C_Polyhedron> dph(ph);

  const memory_size_type dph_total_size = dph.total_memory_in_bytes();
  const memory_size_type dph_external_size = dph.external_memory_in_bytes();

  nout << "ph.total_memory_in_bytes() = " << ph_total_size
       << endl
       << "ph.external_memory_in_bytes() = " << ph_external_size
       << endl
       << "dph.total_memory_in_bytes() = " << dph_total_size
       << endl
       << "dph.external_memory_in_bytes() = " << dph_external_size
       << endl;

  Pointset_Powerset<C_Polyhedron> pph(ph);

  C_Polyhedron qh(3);
  qh.add_constraint(x >= 0);
  qh.add_constraint(y >= 0);
  qh.add_constraint(z >= 0);
  qh.add_constraint(x <= 1);
  qh.add_constraint(y <= 1);
  qh.add_constraint(z <= 1);
  Pointset_Powerset<C_Polyhedron> pqh(qh);

  Pointset_Powerset<C_Polyhedron> prh = pqh;
  prh.difference_assign(pph);

  const memory_size_type pph_total_size = pph.total_memory_in_bytes();
  const memory_size_type pph_external_size = pph.external_memory_in_bytes();
  const memory_size_type pqh_total_size = pqh.total_memory_in_bytes();
  const memory_size_type pqh_external_size = pqh.external_memory_in_bytes();
  const memory_size_type prh_total_size = prh.total_memory_in_bytes();
  const memory_size_type prh_external_size = prh.external_memory_in_bytes();

  nout << "pph.total_memory_in_bytes() = " << pph_total_size
       << endl
       << "pph.external_memory_in_bytes() = " << pph_external_size
       << endl
       << "pqh.total_memory_in_bytes() = " << pqh_total_size
       << endl
       << "pqh.external_memory_in_bytes() = " << pqh_external_size
       << endl
       << "prh.total_memory_in_bytes() = " << prh_total_size
       << endl
       << "prh.external_memory_in_bytes() = " << prh_external_size
       << endl;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8A(test02);
END_MAIN
