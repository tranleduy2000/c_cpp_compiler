/* Test Polyhedron::ascii_dump() and Polyhedron::ascii_load().
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
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A == 2);
  ph.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test02() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A == 2);
  ph.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "=");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test03() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A == 2);
  ph1.add_constraint(B >= 0);

  Constraint_System cs = ph1.constraints();

  C_Polyhedron ph(6);
  ph.add_constraints(cs);

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "=");
  f.seekp(-6, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test04() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "NECESSARILY_CLOSED");
  f.seekp(-2, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test05() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "x");
  f.seekp(1, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test06() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "(not_sorted)");
  f.seekp(-2, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test07() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  ph.generators();

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "sat_c");
  do
    f >> str;
  while (str != "x");
  f.seekp(0, ios_base::cur);
  f << " A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test08() {
  const char* my_file = "ascii_dump_load3.dat";
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);
  ph.add_constraint(B >= 0);

  ph.generators();

  fstream f;
  open(f, my_file, ios_base::out);
  ph.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "sat_c");
  f.seekp(8, ios_base::cur);
  f << " A";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test09() {
  const char* my_file = "ascii_dump_load3.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  -CM -GM  +CS -GS  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "topology NECESSARILY_CLOSED\n"
    << "3 x 3 (not_sorted)\n"
    << "1 0 0";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test10() {
  const char* my_file = "ascii_dump_load3.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  -CM -GM  +CS -GS  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "topology NECESSARILY_CLOSED\n"
    << "3 x 3 (not_sorted)\n"
    << "1 0 0   >=\n"
    << "0 1 0   >=\n"
    << "0 0 1   >=\n\n"
    << "gen_sys (not_up-to-date)\n"
    << "topology NECESSARILY_CLOSED\n"
    <<" 3 x 3 (not_sorted)\n"
    << "1 0 0";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test11() {
  const char* my_file = "ascii_dump_load3.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  -CM -GM  +CS -GS  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "topology NECESSARILY_CLOSED\n"
    << "3";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test12() {
  const char* my_file = "ascii_dump_load3.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  -CM -GM  +CS -GS  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "topology";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test13() {
  const char* my_file = "ascii_dump_load3.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 3\n"
    << "-ZE -EM  +CM +GM  +CS +GS  +SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "topology NOT_NECESSARILY_CLOSED\n"
    << "4 x 5 (sorted)\n"
    << "1 0 0 0 -1   >\n"
    << "0 0 0 0 1   >=\n"
    << "0 0 1 0 0   >=\n"
    << "-2 1 -1 0 0   >=\n"
    << "\n"
    << "gen_sys (up-to-date)\n"
    << "topology NOT_NECESSARILY_CLOSED\n"
    << "5 x 5 (sorted)\n"
    << "0 0 0 1 0   L\n"
    << "0 1 0 0 0   R\n"
    << "0 1 1 0 0   R\n"
    << "1 2 0 0 0   C\n"
    << "1 2 0 0 1   P\n"
    << "\n"
    << "sat_c\n"
    << "5";
    close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

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
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
END_MAIN
