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
#include <sstream>

using std::string;
using std::fstream;
using std::ios_base;
using std::stringstream;

namespace {

// One dimension universe and empty.
bool
test01() {
  Grid gr1(1, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  stringstream ss1;
  gr1.ascii_dump(ss1);

  gr1.ascii_dump(vnout);

  Grid gr2(1);

  print_congruences(gr2, "*** gr2 ***");

  stringstream ss2;
  gr2.ascii_dump(ss2);

  gr2.ascii_dump(vnout);

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Many dimensioned universe and empty.
bool
test02() {
  Grid gr1(3, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  stringstream ss1;
  gr1.ascii_dump(ss1);

  gr1.ascii_dump(vnout);

  Grid gr2(3);

  print_congruences(gr2, "*** gr2 ***");

  stringstream ss2;
  gr2.ascii_dump(ss2);

  gr2.ascii_dump(vnout);

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Universe and empty, mixed dimensions.
bool
test03() {
  Grid gr1(4, EMPTY);

  print_congruences(gr1, "*** gr1 ***");

  stringstream ss1;
  gr1.ascii_dump(ss1);

  gr1.ascii_dump(vnout);

  Grid gr2(3);

  print_congruences(gr2, "*** gr2 ***");

  stringstream ss2;
  gr2.ascii_dump(ss2);

  gr2.ascii_dump(vnout);

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Grids of same dimensions.
bool
test04() {
  Variable A(0);
  Variable C(2);

  Grid gr1(4, EMPTY);
  gr1.add_grid_generator(grid_point(3*A + C));
  gr1.add_grid_generator(parameter(3*A));

  print_generators(gr1, "*** gr1 ***");

  stringstream ss1;
  gr1.ascii_dump(ss1);

  gr1.ascii_dump(vnout);

  Grid gr2(4);
  gr2.add_congruence((3*A == 0) / 0);

  print_congruences(gr2, "*** gr2 ***");

  stringstream ss2;
  gr2.ascii_dump(ss2);

  gr2.ascii_dump(vnout);

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// Grids of mixed dimensions.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(3*A + C));
  gr1.add_grid_generator(parameter(3*A));

  print_generators(gr1, "*** gr1 ***");

  stringstream ss1;
  gr1.ascii_dump(ss1);

  gr1.ascii_dump(vnout);

  Grid gr2(4);
  gr2.add_congruence((3*A == 0) / 0);

  print_congruences(gr2, "*** gr2 ***");

  stringstream ss2;
  gr2.ascii_dump(ss2);

  gr2.ascii_dump(vnout);

  bool ok = (ss2.str().compare(ss1.str()));

  return ok;
}

// ascii_load failure.
bool
test06() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_err 2\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test07() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test08() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM +err +CM +GM  +CS +GS  -CP -GP  -SC -SG\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test09() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "err_sys (up-to-date)";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test10() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-err)";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test11() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test12() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "err\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test13() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "0 1 m 1\n"
    << "1 0 m 1\n"
    << "err_sys (up-to-date)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test14() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys (up-to-err)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test15() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test16() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys (up-to-date)\n"
    << "err\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test17() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys (up-to-date)\n"
    << "2 x err\n"
    << "size 3 1 0 0 P\n"
    << "size 3 0 1 1 Q\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test18() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 1 0 0 P\n"
    << "size 3 0 1 1 Q\n"
    << "dimerrnsion_kinds 0 0\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test19() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 1 0 0 P\n"
    << "size 3 0 1 1 Q\n"
    << "dimension_kinds 0\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test20() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 0 1 m 1\n"
    << "size 3 1 0 m 1\n"
    << "gen_sys (up-to-date)\n"
    << "2 x 3\n"
    << "size 3 1 0 0 P\n"
    << "size 3 0 1 1 Q\n"
    << "dimension_kinds 0 700\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// Grid_Status::ascii_load failure.
bool
test21() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS err  -CP -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// Grid_Status::ascii_load failure.
bool
test22() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  err -GP  -SC -SG\n"
    << "con_sys (up-to-date)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// Grid_Status::ascii_load failure.
bool
test23() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP err  -SC -SG\n"
    << "con_sys (up-to-date)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// Grid_Status::ascii_load failure.
bool
test24() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  err -SG\n"
    << "con_sys (up-to-date)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
  close(f);

  return ok;
}

// Grid_Status::ascii_load failure.
bool
test25() {
  const char* my_file = "ascii_dump_load1.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 1\n"
    << "-ZE -EM  +CM +GM  +CS +GS  -CP -GP  -SC err\n"
    << "con_sys (up-to-date)\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid gr;
  bool ok = !gr.ascii_load(f);
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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
  DO_TEST(test21);
  DO_TEST(test22);
  DO_TEST(test23);
  DO_TEST(test24);
  DO_TEST(test25);
END_MAIN
