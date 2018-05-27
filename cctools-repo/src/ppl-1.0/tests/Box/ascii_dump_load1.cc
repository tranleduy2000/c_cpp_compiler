/* Test Box::ascii_dump() and Box::ascii_load().
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

const char* my_file = "ascii_dump_load1.dat";

bool
test01() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  box.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "-EUP");
  f.seekp(0, ios_base::cur);
  f << " A";
  close(f);

  open(f, my_file, ios_base::in);
  TBox box2;
  bool ok = !box2.ascii_load(f);
  close(f);

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(B >= 1);

  fstream f;
  open(f, my_file, ios_base::out);
  box.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "-EM");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  TBox box2;
  bool ok = !box2.ascii_load(f);
  close(f);

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);
  box.add_constraint(B >= 2);

  fstream f;
  open(f, my_file, ios_base::out);
  box.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "-UN");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  TBox box2;
  bool ok = !box2.ascii_load(f);
  close(f);

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= -10);
  box.add_constraint(A <= 10);
  box.add_constraint(B >= 3);
  box.add_constraint(B <= 6);

  fstream f;
  open(f, my_file, ios_base::out);
  box.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "lower");
  f.seekp(0, ios_base::cur);
  f << "Z(";
  close(f);

  open(f, my_file, ios_base::in);
  TBox box2;
  bool ok = !box2.ascii_load(f);
  close(f);

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBox box1(3);
  box1.add_constraint(A >= 2);
  box1.add_constraint(B >= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  box1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  TBox box2;
  bool ok = box2.ascii_load(f);
  close(f);

  if (!ok) {
    nout << "ascii_load() failed" << endl;
    return false;
  }

  ok = (box1 == box2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(3*C == 5);
  TBox box1(cs);

  print_constraints(box1, "*** box1(cs) ***");

  TBox box1_copy(box1);

  box1.difference_assign(box1_copy);

  print_constraints(box1, "*** box1.difference_assign(box1_copy) ***");

  box1.concatenate_assign(box1_copy);

  print_constraints(box1, "*** box1.concatenate_assign(box1_copy) ***");

  nout << "box1.space_dimension() = " << box1.space_dimension() << endl;

  fstream f;
  open(f, my_file, ios_base::out);
  box1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  TBox box2;
  bool ok = box2.ascii_load(f);
  close(f);

  if (!ok) {
    nout << "ascii_load() failed" << endl;
    return false;
  }

  ok = (box1 == box2);

  nout << "box2.space_dimension() = " << box2.space_dimension() << endl;

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

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
END_MAIN
