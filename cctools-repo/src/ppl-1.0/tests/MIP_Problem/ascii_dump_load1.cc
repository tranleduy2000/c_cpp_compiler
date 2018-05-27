/* Test MIP_Problem::ascii_dump() and MIP_Problem::ascii_load().
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
  Variable B(1);

  Constraint_System cs;
  cs.insert(A - B >= 2);
  cs.insert(B >= 0);

  Linear_Expression cost(A + 2*B);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  mip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f);
  close(f);
  return ok;
}

bool
test02() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(10*A - B >= 11);
  cs.insert(B >= 0);

  Linear_Expression cost(17*A + 2*B);

  MIP_Problem mip1(cs.space_dimension(), cs, cost, MINIMIZATION);
  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  f.seekp(0);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = !mip2.ascii_load(f);
  close(f);
  return ok;
}

bool
test03() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(1);

  Constraint_System cs;
  cs.insert(15*A - C >= 11);
  cs.insert(B >= 0);
  cs.insert(C <= 11);

  Linear_Expression cost(17*A + 2*B -2*C);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MINIMIZATION);
  mip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "opt_mode");
  f.seekp(0, ios_base::cur);
  f << " Q";
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = !mip2.ascii_load(f);
  close(f);

  return ok;
}

bool
test04() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A >= 10);
  cs.insert(A <= 5);
  cs.insert(C <= 11);

  Linear_Expression cost(A + 2*B + 3*C);

  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  mip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f);
  close(f);
  return ok;
}

bool
test05() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A >= 123);
  cs.insert(A + 2*B + 11*C <= 5434);
  cs.insert(C <= 11);

  Linear_Expression cost(A - 11*B);

  MIP_Problem mip1(cs.space_dimension(), cs, cost, MINIMIZATION);
  mip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);
  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "base(");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = !mip2.ascii_load(f);
  close(f);
  return ok;
}

// Unfeasible problem, not solved.
bool
test06() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A - B >= 2);
  cs.insert(C >= 10);
  cs.insert(B >= 10);
  cs.insert(B + C <= 15);

  Linear_Expression cost(A + 2*B);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f) && mip1.solve() == mip2.solve();
  close(f);
  return ok;
}

// Unbounded problem, not solved.
bool
test07() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;

  cs.insert(A >= 2);
  cs.insert(C >= 10);
  cs.insert(B + A <= 15);

  Linear_Expression cost(1*C);

  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  MIP_Problem mip2;
  open(f, my_file, ios_base::in);
  bool ok = mip2.ascii_load(f) && mip1.solve() == mip2.solve();
  close(f);
  return ok;
}

// Problem with optimum, not solved.
bool
test08() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A + B >= 2);
  cs.insert(C >= 10);
  cs.insert(B + C <= 15);
  cs.insert(A <= 30);
  cs.insert(B <= 50);

  Linear_Expression cost(1*C + 2*B);

  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f) &&
    mip1.optimizing_point() == mip2.optimizing_point();
  close(f);
  return ok;
}

// Unfeasible problem, solved.
bool
test09() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A - B >= 2);
  cs.insert(C >= 10);
  cs.insert(B >= 10);
  cs.insert(B + C <= 15);

  Linear_Expression cost(A + 2*B);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  mip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f) && mip1.solve() == mip2.solve();
  close(f);
  return ok;
}

// Unbounded problem, solved.
bool
test10() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;

  cs.insert(A >= 2);
  cs.insert(C >= 10);
  cs.insert(B + A <= 15);

  Linear_Expression cost(1*C);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  mip1.solve();

  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  MIP_Problem mip2;
  open(f, my_file, ios_base::in);
  bool ok = mip2.ascii_load(f) && mip1.solve() == mip2.solve();
  close(f);
  return ok;
}

// Problem with optimum, solved.
bool
test11() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A + B >= 2);
  cs.insert(C >= 10);
  cs.insert(B + C <= 15);
  cs.insert(A <= 30);
  cs.insert(B <= 50);

  Linear_Expression cost(1*C);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  mip1.solve();
  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f) &&
    mip1.optimizing_point() == mip2.optimizing_point();
  close(f);
  return ok;
}

// Problem with optimum and integer variables, solved.
bool
test12() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A + B >= 2);
  cs.insert(C >= 10);
  cs.insert(B + C <= 15);
  cs.insert(A <= 20);
  cs.insert(B <= 40);

  Linear_Expression cost(1*C);
  MIP_Problem mip1(cs.space_dimension(), cs, cost, MAXIMIZATION);
  mip1.add_to_integer_space_dimensions(Variables_Set(A,C));
  mip1.solve();
  fstream f;
  open(f, my_file, ios_base::out);
  mip1.ascii_dump(f);
  close(f);
  open(f, my_file, ios_base::in);
  MIP_Problem mip2;
  bool ok = mip2.ascii_load(f) &&
    mip1.optimizing_point() == mip2.optimizing_point();
  close(f);
  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST_F8(test03);
  DO_TEST(test04);
  DO_TEST_F16(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
END_MAIN
