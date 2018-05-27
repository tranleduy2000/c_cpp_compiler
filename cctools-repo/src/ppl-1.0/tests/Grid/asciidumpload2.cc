/* Test Grid::ascii_dump() and Grid::ascii_load().
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

// ascii_load simple test of loading the result of ascii_dump()
bool
test01() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr1(3);
  gr1.add_congruence((A - B %= 2) / 5);
  gr1.add_congruence(B %= 0);

  gr1.minimized_grid_generators();

  fstream f;
  open(f, my_file, ios_base::out);
  gr1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  gr2.ascii_load(f);
  close(f);

  bool ok = (gr1 == gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

bool
test02() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr1(2);
  gr1.add_congruence((A %= 2) / 5);
  gr1.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr1.ascii_dump(f);
  close(f);

  print_congruences(gr1, "*** gr1 ***");

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "space_dim");
  f.seekp(0, ios_base::cur);
  f << " A";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test03() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "-ZE");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test04() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "con_sys");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test05() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "(up-to-date)");
  f.seekp(0, ios_base::cur);
  f << "A\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test06() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "gen_sys");
  f.seekp(0, ios_base::cur);
  f << "A";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test07() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "(not_up-to-date)");
  f.seekp(0, ios_base::cur);
  f << "A\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test08() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "(up-to-date)");
  f.seekp(0, ios_base::cur);
  f << "\nA";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test09() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 0);
  gr.add_congruence(B %= 0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in | ios_base::out);
  string str;
  do
    f >> str;
  while (str != "(not_up-to-date)");
  f.seekp(0, ios_base::cur);
  f << "\nA";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test10() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test11() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test12() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  +CM";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

bool
test13() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  +CM +GM";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  bool ok = !gr2.ascii_load(f);
  close(f);

  return ok;
}

// A grid with a virtual generator.
bool
test14() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3);
  gr1.add_congruence((A - B %= 2) / 5);
  gr1.add_congruence(B %= 0);
  gr1.add_congruence((C == 4) / 0);

  gr1.minimized_grid_generators();

  fstream f;
  open(f, my_file, ios_base::out);
  gr1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  gr2.ascii_load(f);
  close(f);

  bool ok = (gr1 == gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Zero dimension universe.
bool
test15() {
  const char* my_file = "ascii_dump_load2.dat";

  Grid gr1(0);

  fstream f;
  open(f, my_file, ios_base::out);
  gr1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  gr2.ascii_load(f);
  close(f);

  bool ok = (gr1 == gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

  return ok;
}

// Zero dimension empty.
bool
test16() {
  const char* my_file = "ascii_dump_load2.dat";

  Grid gr1(0, EMPTY);

  fstream f;
  open(f, my_file, ios_base::out);
  gr1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr2;
  gr2.ascii_load(f);
  close(f);

  bool ok = (gr1 == gr2);

  print_congruences(gr1, "*** gr1 ***");
  print_congruences(gr2, "*** gr2 ***");

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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
END_MAIN
