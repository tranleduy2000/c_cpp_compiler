/* Test Grid_Generator ascii_dump() and ascii_load().
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

// Load the result of a dump.
bool
test01() {
  const char* my_file = "ascii_dump_load5.dat";
  Variable A(0);
  Variable B(1);

  Grid_Generator g1 = grid_point(A - B);

  fstream f;
  open(f, my_file, ios_base::out);
  g1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  Grid_Generator g2(grid_point(0*B));
  g2.ascii_load(f);
  close(f);

  bool ok = (g1 == g2);

  print_generator(g1, "*** g1 ***");
  print_generator(g2, "*** g2 ***");

  return ok;
}

// ascii_load failure.
bool
test02() {
  const char* my_file = "ascii_dump_load5.dat";
  Variable A(0);

  fstream f;
  open(f, my_file, ios_base::out);
  f << "1\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid_Generator g(grid_point(0*A));
  bool ok = !g.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test03() {
  const char* my_file = "ascii_dump_load5.dat";
  Variable A(0);

  fstream f;
  open(f, my_file, ios_base::out);
  f << "1 0 3\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid_Generator g(grid_point(0*A));
  bool ok = !g.ascii_load(f);
  close(f);

  return ok;
}

// ascii_load failure.
bool
test04() {
  const char* my_file = "ascii_dump_load5.dat";
  Variable A(0);

  fstream f;
  open(f, my_file, ios_base::out);
  f << "1 0 1 err\n";
  close(f);

  open(f, my_file, ios_base::in);
  Grid_Generator g(grid_point(0*A));
  bool ok = !g.ascii_load(f);
  close(f);

  return ok;
}

// Load and dump a line.
bool
test05() {
  const char* my_file = "ascii_dump_load5.dat";
  Variable A(0);
  Variable B(0);

  Grid_Generator g1 = grid_line(2*A - B);

  fstream f;
  open(f, my_file, ios_base::out);
  g1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  Grid_Generator g2(grid_point(0*B));
  g2.ascii_load(f);
  close(f);

  bool ok = (g1 == g2);

  print_generator(g1, "*** g1 ***");
  print_generator(g2, "*** g2 ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
