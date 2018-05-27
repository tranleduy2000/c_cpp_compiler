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
add_constraint(TOctagonal_Shape& oct, const Constraint& c) {
  const memory_size_type oct_memory_before = oct.total_memory_in_bytes();
  const memory_size_type c_memory = c.total_memory_in_bytes();
  oct.add_constraint(c);
  const memory_size_type oct_memory_after = oct.total_memory_in_bytes();

  nout << oct_memory_before
       << " + " << c_memory
       << " -> " << oct_memory_after
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

  nout << "*** Adding constraints to an octagon ***" << endl;

  TOctagonal_Shape oct(3);
  add_constraint(oct, 4*x - 4*y + 14 >= 0);
  add_constraint(oct, 4*x + 4*y + 2 >= 0);
  add_constraint(oct, x + y - 1 >= 0);

  add_constraint(oct, x + 1 >= 0);
  add_constraint(oct, x + z - 1 >= 0);
  add_constraint(oct, 2*x -2*z + 7 >= 0);
  add_constraint(oct, x - y + 1 >= 0);

  add_constraint(oct, x - y + 5 <= 0);
  add_constraint(oct, 2*x - 2*z + 13 <= 0);
  add_constraint(oct, -2*x + 2*z + 1 <= 0);
  add_constraint(oct, -x + y - 1 <= 0);

  add_constraint(oct, -x + y + 7 >= 0);
  add_constraint(oct, -4*x + 4*y - 4 >= 0);
  add_constraint(oct, -2*x + 2*z - 5 <= 0);
  add_constraint(oct, -x + 1 >= 0);

  add_constraint(oct, -x - z + 5 <= 0);
  add_constraint(oct, -x - y + 5 <= 0);
  add_constraint(oct, -x - y +13 <= 0);

  const memory_size_type oct_total_size = oct.total_memory_in_bytes();
  const memory_size_type oct_external_size = oct.external_memory_in_bytes();
  const Constraint_System& cs = oct.constraints();
  const memory_size_type cs_total_size = cs.total_memory_in_bytes();
  const memory_size_type cs_external_size = cs.external_memory_in_bytes();

  nout << endl;

  nout << "*** Size of the user-visible octagons components ***"
       << endl
       << "oct.total_memory_in_bytes() = " << oct_total_size
       << endl
       << "cs.total_memory_in_bytes() = " << cs_total_size
       << endl
       << "oct.external_memory_in_bytes() = " << oct_external_size
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

  TOctagonal_Shape oct(3);
  oct.add_constraint(4*x - 4*y + 14 >= 0);
  oct.add_constraint(4*x + 4*y + 2 <= 0);
  oct.add_constraint(x + y - 1 >= 0);
  oct.add_constraint(x + y - 5 <= 0);

  const memory_size_type oct_total_size = oct.total_memory_in_bytes();
  const memory_size_type oct_external_size = oct.external_memory_in_bytes();

  Determinate<TOctagonal_Shape> doct(oct);

  const memory_size_type doct_total_size = doct.total_memory_in_bytes();
  const memory_size_type doct_external_size = doct.external_memory_in_bytes();

  nout << "oct.total_memory_in_bytes() = " << oct_total_size
       << endl
       << "oct.external_memory_in_bytes() = " << oct_external_size
       << endl
       << "doct.total_memory_in_bytes() = " << doct_total_size
       << endl
       << "doct.external_memory_in_bytes() = " << doct_external_size
       << endl;

  Pointset_Powerset<TOctagonal_Shape> poct(oct);

  TOctagonal_Shape qoct(3);
  qoct.add_constraint(x >= 0);
  qoct.add_constraint(y >= 0);
  qoct.add_constraint(z >= 0);
  qoct.add_constraint(x <= 1);
  qoct.add_constraint(y <= 1);
  qoct.add_constraint(z <= 1);
  Pointset_Powerset<TOctagonal_Shape> pqoct(qoct);

  Pointset_Powerset<TOctagonal_Shape> proct = pqoct;
  proct.difference_assign(poct);

  const memory_size_type poct_total_size = poct.total_memory_in_bytes();
  const memory_size_type poct_external_size = poct.external_memory_in_bytes();
  const memory_size_type pqoct_total_size = pqoct.total_memory_in_bytes();
  const memory_size_type pqoct_external_size = pqoct.external_memory_in_bytes();
  const memory_size_type proct_total_size = proct.total_memory_in_bytes();
  const memory_size_type proct_external_size = proct.external_memory_in_bytes();

  nout << "poct.total_memory_in_bytes() = " << poct_total_size
       << endl
       << "poct.external_memory_in_bytes() = " << poct_external_size
       << endl
       << "pqoct.total_memory_in_bytes() = " << pqoct_total_size
       << endl
       << "pqoct.external_memory_in_bytes() = " << pqoct_external_size
       << endl
       << "proct.total_memory_in_bytes() = " << proct_total_size
       << endl
       << "proct.external_memory_in_bytes() = " << proct_external_size
       << endl;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
