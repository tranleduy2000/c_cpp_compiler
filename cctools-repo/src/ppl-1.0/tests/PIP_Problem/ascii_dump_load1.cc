/* Test PIP_Problem::ascii_dump() and PIP_Problem::ascii_load().
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

#include "files.hh"
#include <string>
#include <fstream>

using std::string;
using std::fstream;
using std::ios_base;

namespace {

bool
test01() {
  const char* my_file = "ascii_dump_load1.dat";

  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 1);

  PIP_Problem pip1(3, cs.begin(), cs.end(), Variables_Set());
  pip1.clear();

  fstream f;
  open(f, my_file, ios_base::out);
  pip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  PIP_Problem pip2;
  pip2.ascii_load(f);
  close(f);

  bool ok = pip1.space_dimension() == 0
    && pip2.space_dimension() == 0
    && pip1.constraints_begin() == pip1.constraints_end()
    && pip2.constraints_begin() == pip2.constraints_end();

  return ok;
}

bool
test02() {
  const char* my_file = "ascii_dump_load1.dat";

  PIP_Problem pip1(3);
  pip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  pip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  PIP_Problem pip2;
  pip2.ascii_load(f);
  close(f);

  bool ok = pip1.space_dimension() == 3
    && pip2.space_dimension() == 3
    && pip1.constraints_begin() == pip1.constraints_end()
    && pip2.constraints_begin() == pip2.constraints_end();

  return ok;
}

bool
test03() {
  const char* my_file = "ascii_dump_load1.dat";

  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A <= 0);

  PIP_Problem pip1(1, cs.begin(), cs.end(), Variables_Set());
  pip1.set_control_parameter(PIP_Problem::CUTTING_STRATEGY_DEEPEST);
  pip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  pip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  PIP_Problem pip2;
  pip2.ascii_load(f);
  close(f);

  bool ok = pip1.space_dimension() == pip2.space_dimension()
    && 2 == std::distance(pip1.constraints_begin(), pip1.constraints_end())
    && 2 == std::distance(pip2.constraints_begin(), pip2.constraints_end());

  return ok;
}

bool
test04() {
  const char* my_file = "ascii_dump_load1.dat";

  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 5);

  PIP_Problem pip1(3, cs.begin(), cs.end(), Variables_Set());

  fstream f;
  open(f, my_file, ios_base::out);
  pip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  PIP_Problem pip2;
  pip2.ascii_load(f);
  close(f);

  bool ok = pip1.space_dimension() == pip2.space_dimension()
    && 1 == std::distance(pip1.constraints_begin(), pip1.constraints_end())
    && 1 == std::distance(pip2.constraints_begin(), pip2.constraints_end());

  return ok;
}

bool
test05() {
  const char* my_file = "ascii_dump_load1.dat";

  Variable A(0);
  Variable P(1);
  Constraint_System cs;
  cs.insert(A >= 1);
  cs.insert(A <= 0);

  PIP_Problem pip1(2, cs.begin(), cs.end(), Variables_Set(P));
  pip1.set_control_parameter(PIP_Problem::CUTTING_STRATEGY_ALL);
  pip1.set_control_parameter(PIP_Problem::PIVOT_ROW_STRATEGY_MAX_COLUMN);
  pip1.set_big_parameter_dimension(1);
  pip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  pip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  PIP_Problem pip2;
  pip2.ascii_load(f);
  close(f);

  bool ok = pip1.space_dimension() == pip2.space_dimension()
    && 2 == std::distance(pip1.constraints_begin(), pip1.constraints_end())
    && 2 == std::distance(pip2.constraints_begin(), pip2.constraints_end());

  return ok;
}

bool
test06() {
  PIP_Problem pip1;
  using namespace IO_Operators;
  nout << pip1 << endl;
  return true;
}

bool
test07() {
  Variable X1(0);
  Variable X2(1);
  Variable I0(2);
  Variable J0(3);
  Variable N(4);
  Variables_Set params(I0, N);

  Constraint_System cs;
  cs.insert(-X1 + N - 1 >= 0);
  cs.insert(X1 - X2 >= 0);
  cs.insert(X1 + I0 == N);
  cs.insert(X2 + J0 - N - 1 >= 0);
  cs.insert(I0 >= 1);
  cs.insert(N >= 1);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  (void) pip.solve();

  std::stringstream ss;
  pip.ascii_dump(ss);

  PIP_Problem pip2;
  bool ok = pip2.ascii_load(ss);

  std::stringstream ss2;
  pip2.ascii_dump(ss2);

  ok &= (ss.str() == ss2.str());

  // This further check is mainly made for coverage purposes.
  memory_size_type total = pip2.total_memory_in_bytes();
  memory_size_type external = pip2.external_memory_in_bytes();
  ok &= (external < total);

  return ok;
}

bool
test08() {
  typedef PIP_Tree_Node::Artificial_Parameter Art_Param;

  Variable A(0);

  Art_Param ap1(3*A + 8, -5);
  std::stringstream ss1;
  ap1.ascii_dump(ss1);

  Art_Param ap2;
  bool ok = ap2.ascii_load(ss1);

  std::stringstream ss2;
  ap2.ascii_dump(ss2);

  ok &= (ap1 == ap2) && (ss1.str() == ss2.str());

  return ok;
}

bool
test09() {
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  // Compute the solution tree.
  (void) pip.solve();
  // Printing ensures parametric solution values are generated.
  pip.print_solution(nout);

  std::stringstream ss;
  pip.ascii_dump(ss);

  PIP_Problem pip2;
  bool ok = pip2.ascii_load(ss);

  std::stringstream ss2;
  pip2.ascii_dump(ss2);

  ok &= (ss.str() == ss2.str());

  return ok;
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
  DO_TEST_F8(test09);
END_MAIN
