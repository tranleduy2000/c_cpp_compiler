/* Test Octagonal_Shape::ascii_dump() and Octagonal_Shape::ascii_load().
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

namespace {

const char* my_file = "ascii_dump_load1.dat";

bool
test01() {

  nout << "test01()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A - B >= 2);
  oc1.add_constraint(B >= 0);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc1.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  oc2.ascii_load(f);
  close(f);

  print_constraints(oc1, "*** oc1 ***");
  print_constraints(oc2, "*** oc2 ***");

  bool ok = (oc1 == oc2);

  return ok;
}

bool
test02() {

  nout << "test02()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc1(3);
  oc1.add_constraint(A - B >= 0);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc1.ascii_dump(f);
  f.seekp(0);
  f << "A";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
  close(f);

  return ok;
}

bool
test03() {

  nout << "test03()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 0);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in | std::ios_base::out);
  std::string str;
  do
    f >> str;
  while (str != "-EM");
  f.seekp(0, std::ios_base::cur);
  f << " A";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
  close(f);

  return ok;
}

bool
test04() {

  nout << "test04()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 1);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in | std::ios_base::out);
  std::string str;
  do
    f >> str;
  while(str != "+ZE");
  f.seekp(0, std::ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
  close(f);

  return ok;
}

bool
test05() {

 nout << "test05()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 2);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in | std::ios_base::out);
  std::string str;
  do
    f >> str;
  while(str != "-SC");
  f.seekp(0, std::ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
  close(f);

  return ok;
}

bool
test06() {

  nout << "test06()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 3);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in | std::ios_base::out);
  std::string str;
  do
    f >> str;
  while(str != "+inf");
  f.seekp(0, std::ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
  close(f);

  return ok;
}

bool
test07() {

  nout << "test07()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 3);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in | std::ios_base::out);
  std::string str;
  do
    f >> str;
  while(str != "+inf");
  do
    f >> str;
  while(str != "+inf");
  f.seekp(0, std::ios_base::cur);
  f << " 3 ";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
  close(f);

  return ok;
}

bool
test08() {

  nout << "test08()" << endl;

  TOctagonal_Shape oc1(0, EMPTY);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc1.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  oc2.ascii_load(f);
  close(f);

  bool ok = (oc1 == oc2);

  return ok;
}

bool
test09() {

  nout << "test09()" << endl;

  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 0);
  oc.add_constraint(B >= 0);

  std::fstream f;
  open(f, my_file, std::ios_base::out);
  oc.ascii_dump(f);
  close(f);

  open(f, my_file, std::ios_base::in | std::ios_base::out);
  std::string str;
  do
    f >> str;
  while (str != "space_dim");
  f.seekp(0, std::ios_base::cur);
  f << " A";
  close(f);

  open(f, my_file, std::ios_base::in);
  TOctagonal_Shape oc2;
  bool ok = !oc2.ascii_load(f);
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
END_MAIN
