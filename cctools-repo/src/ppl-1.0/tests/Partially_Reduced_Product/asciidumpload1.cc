/* Test Direct_Product<nnc_ph, gr> ascii_dump() and ascii_load().
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

typedef Domain_Product<Grid, NNC_Polyhedron>::Direct_Product
No_Reduction_Product;

namespace {

// Universe.
bool
test01() {
  const char* my_file = "ascii_dump_load1.dat";

  No_Reduction_Product dp1(3);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Congruence.
bool
test02() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);

  No_Reduction_Product dp1(3);
  dp1.refine_with_congruence((2*A %= 3) / 3);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Constraint.
bool
test03() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);

  No_Reduction_Product dp1(3);
  dp1.refine_with_constraint(3*A > 2);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Congruence and constraint.
bool
test04() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);

  No_Reduction_Product dp1(3);
  dp1.refine_with_constraint(3*B > 2);
  dp1.refine_with_congruence((A %= 0) / 2);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Many dimensions.
bool
test05() {
  const char* my_file = "ascii_dump_load1.dat";
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  No_Reduction_Product dp1(4);
  dp1.refine_with_constraint(3*A + D > 2);
  dp1.refine_with_congruence((A - 3*C %= 0) / 2);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(4);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Empty.
bool
test06() {
  const char* my_file = "ascii_dump_load1.dat";

  No_Reduction_Product dp1(7, EMPTY);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Zero-dimension empty.
bool
test07() {
  const char* my_file = "ascii_dump_load1.dat";

  No_Reduction_Product dp1(0, EMPTY);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

  return ok;
}

// Zero-dimension universe.
bool
test08() {
  const char* my_file = "ascii_dump_load1.dat";

  No_Reduction_Product dp1(0);

  fstream f;
  open(f, my_file, ios_base::out);
  dp1.ascii_dump(f);
  close(f);

  open(f, my_file, ios_base::in);
  No_Reduction_Product dp2(3);
  dp2.ascii_load(f);
  close(f);

  bool ok = (dp1 == dp2);

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
END_MAIN
