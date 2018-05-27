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
add_constraint(TBD_Shape& bds, const Constraint& c) {
  const memory_size_type bd_memory_before = bds.total_memory_in_bytes();
  const memory_size_type c_memory = c.total_memory_in_bytes();
  bds.add_constraint(c);
  const memory_size_type bd_memory_after = bds.total_memory_in_bytes();

  nout << bd_memory_before
       << " + " << c_memory
       << " -> " << bd_memory_after
       << endl;
}

void
minimize(TBD_Shape& bds) {
  const memory_size_type bd_memory_before = bds.total_memory_in_bytes();
  (void) bds.minimized_constraints();
  const memory_size_type bd_memory_after = bds.total_memory_in_bytes();

  nout << bd_memory_before
       << " -m-> " << bd_memory_after
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

  le -= 4*y;
  le_total_size = le.total_memory_in_bytes();
  le_external_size = le.external_memory_in_bytes();

  nout << "(" << le << ").total_memory_in_bytes() = " << le_total_size
       << endl
       << "(" << le << ").external_memory_in_bytes() = " << le_external_size
       << endl;

  le += 4;
  le_total_size = le.total_memory_in_bytes();
  le_external_size = le.external_memory_in_bytes();

  nout << "(" << le << ").total_memory_in_bytes() = " << le_total_size
       << endl
       << "(" << le << ").external_memory_in_bytes() = " << le_external_size
       << endl << endl;

  nout << "*** Adding constraints to a bounded difference shape ***" << endl;

  TBD_Shape bds(3);
  add_constraint(bds, 2*x - 2*y >= 0);
  add_constraint(bds, x - y - 1 <= 0);
  add_constraint(bds, x >= 0);
  minimize(bds);
  add_constraint(bds, x + 1 >= 0);
  add_constraint(bds, x - z - 1 >= 0);
  add_constraint(bds, 2*x - 2*z + 7 >= 0);
  minimize(bds);
  add_constraint(bds, x - y + 5 >= 0);
  add_constraint(bds, 2*x - 2*z + 13 >= 0);
  add_constraint(bds, -2*x + 2*z + 1 >= 0);
  add_constraint(bds, -x + y - 1 >= 0);
  minimize(bds);
  add_constraint(bds, -x + y + 7 >= 0);
  add_constraint(bds, -4*x + 4*y - 4 >= 0);
  add_constraint(bds, -2*x + 2*z - 5 >= 0);
  add_constraint(bds, -x + 1 >= 0);
  minimize(bds);
  add_constraint(bds, -x + y + 5 >= 0);
  minimize(bds);

  const memory_size_type bd_total_size = bds.total_memory_in_bytes();
  const memory_size_type bd_external_size = bds.external_memory_in_bytes();
  const Constraint_System& cs = bds.constraints();
  const memory_size_type cs_total_size = cs.total_memory_in_bytes();
  const memory_size_type cs_external_size = cs.external_memory_in_bytes();

  nout << endl;

  nout << "*** Size of the user-visible polyhedra components ***"
       << endl
       << "bds.total_memory_in_bytes() = " << bd_total_size
       << endl
       << "cs.total_memory_in_bytes() = " << cs_total_size
       << endl
       << "bds.external_memory_in_bytes() = " << bd_external_size
       << endl
       << "cs.external_memory_in_bytes() = " << cs_external_size
       << endl << endl;

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

  return true;
}

bool test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_constraint(4*x - 4*y + 14 >= 0);
  bds.add_constraint(x - z + 2 >= 0);
  bds.add_constraint(y - z - 5 >= 0);

  const memory_size_type bd_total_size = bds.total_memory_in_bytes();
  const memory_size_type bd_external_size = bds.external_memory_in_bytes();

  Determinate<TBD_Shape> dbds(bds);

  const memory_size_type dbd_total_size = dbds.total_memory_in_bytes();
  const memory_size_type dbd_external_size = dbds.external_memory_in_bytes();

  nout << "bds.total_memory_in_bytes() = " << bd_total_size
       << endl
       << "bds.external_memory_in_bytes() = " << bd_external_size
       << endl
       << "dbds.total_memory_in_bytes() = " << dbd_total_size
       << endl
       << "dbds.external_memory_in_bytes() = " << dbd_external_size
       << endl;

  Pointset_Powerset<TBD_Shape> pbds(bds);

  TBD_Shape qbds(3);
  qbds.add_constraint(x >= 0);
  qbds.add_constraint(y >= 0);
  qbds.add_constraint(z >= 0);
  qbds.add_constraint(x <= 1);
  qbds.add_constraint(y <= 1);
  qbds.add_constraint(z <= 1);
  Pointset_Powerset<TBD_Shape> pqbds(qbds);

  Pointset_Powerset<TBD_Shape> prbds = pqbds;
  prbds.difference_assign(pbds);

  const memory_size_type pbd_total_size = pbds.total_memory_in_bytes();
  const memory_size_type pbd_external_size = pbds.external_memory_in_bytes();
  const memory_size_type pqbd_total_size = pqbds.total_memory_in_bytes();
  const memory_size_type pqbd_external_size = pqbds.external_memory_in_bytes();
  const memory_size_type prbd_total_size = prbds.total_memory_in_bytes();
  const memory_size_type prbd_external_size = prbds.external_memory_in_bytes();

  nout << "pbds.total_memory_in_bytes() = " << pbd_total_size
       << endl
       << "pbds.external_memory_in_bytes() = " << pbd_external_size
       << endl
       << "pqbds.total_memory_in_bytes() = " << pqbd_total_size
       << endl
       << "pqbds.external_memory_in_bytes() = " << pqbd_external_size
       << endl
       << "prbds.total_memory_in_bytes() = " << prbd_total_size
       << endl
       << "prbds.external_memory_in_bytes() = " << prbd_external_size
       << endl;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
