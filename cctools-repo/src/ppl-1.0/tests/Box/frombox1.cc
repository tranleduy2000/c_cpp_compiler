/* Test Box<Interval>::Box(const Box<Other_Interval>&).
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

namespace {

bool
test01() {
  TBox src(0, EMPTY);
  TBox dst(src);

  bool ok = (dst == src);

  print_constraints(src, "*** src ***");
  print_constraints(dst, "*** dst ***");

  return ok;
}

bool
test02() {
  TBox src(0, UNIVERSE);
  TBox dst(src);

  bool ok = (dst == src);

  print_constraints(src, "*** src ***");
  print_constraints(dst, "*** dst ***");

  return ok;
}

bool
test03() {
  TBox src(24, EMPTY);
  TBox dst(src);

  bool ok = (dst == src);

  print_constraints(src, "*** src ***");
  print_constraints(dst, "*** dst ***");

  return ok;
}

bool
test04() {
  TBox src(24, UNIVERSE);
  TBox dst(src);

  bool ok = (dst == src);

  print_constraints(src, "*** src ***");
  print_constraints(dst, "*** dst ***");

  return ok;
}

bool
test05() {
  const int DIM = 10;
  typedef Checked_Number<mpq_class, Checked_Number_Transparent_Policy<mpq_class> > Q;
  Rational_Box src(DIM, UNIVERSE);
  Q k;
  Q a;
  k = 2;
  sqrt_assign(a, k);
  Q y = a;
  k = 1;
  y -= k;
  // Now y ~= sqrt(2) - 1.
  k = -4;
  a *= k;
  k = 6;
  a += k;
  // Now a ~= 6 - 4*sqrt(2).
  Q s;
  Q m;
  Q d;
  for (int n = 0; n < DIM; ++n) {
    s = y;
    s *= s;
    s *= s;
    neg_assign(s, s);
    // Now s ~= -y^4.
    k = 1;
    s += k;
    sqrt_assign(s, s);
    sqrt_assign(s, s);
    // Now s ~= sqrt(sqrt(1-y^4)).
    m = k;
    m -= s;
    // Now m ~= 1 - sqrt(sqrt(1-y^4)).
    d = k;
    d += s;
    // Now d ~= 1 + sqrt(sqrt(1-y^4)).
    y = m/d;
    // Now y ~= (1 - sqrt(sqrt(1-y^4)))/(1 + sqrt(sqrt(1-y^4))).
    s = y;
    s += k;
    s *= s;
    s *= s;
    // Now s ~= (1 + y)^4.
    a *= s;
    s = y;
    s *= s;
    s += y;
    s += k;
    s *= y;
    // Now s ~= y*(1 + y + y^2).
    k = 1ULL << (2*n+3);
    s *= k;
    a -= s;
    m = 1;
    m /= a;
    Coefficient num;
    Coefficient den;
    numer_denom(m, num, den);
    src.add_constraint(den*Variable(n) == num);
  }

  TBox dst(src);

  bool ok = check_result(dst, src, "2.39e-6", "5.51e-7", "1.51e-7");

  print_constraints(src, "*** src ***");
  print_constraints(dst, "*** dst ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST_F64(test05);
END_MAIN
