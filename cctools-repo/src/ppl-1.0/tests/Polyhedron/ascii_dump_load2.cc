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
  const char* my_file = "ascii_dump_load2.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  +CM +GM  +CS";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test02() {
  const char* my_file = "ascii_dump_load2.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  +CM +GM  +CS  +SC";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test03() {
  const char* my_file = "ascii_dump_load2.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  +CM +GM  +CS  +GS";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test04() {
  const char* my_file = "ascii_dump_load2.dat";
  fstream f;
  open(f, my_file, ios_base::out);
  f << "space_dim 2\n"
    << "-ZE -EM  +CM +GM  +CS  +GS  -SC";
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  bool ok = !ph2.ascii_load(f);
  close(f);

  return ok;
}

bool
test05() {
  const char* my_file = "ascii_dump_load2.dat";
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A + B));
  C_Polyhedron ph1(gs);
  ph1.constraints();

  fstream f;
  open(f, my_file, ios_base::out);
  ph1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  C_Polyhedron ph2;
  ph2.ascii_load(f);
  close(f);

  bool ok = (ph1 == ph2);

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
