/* Test Powerset<D>.
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

// Uses every public Powerset method.
bool
test01() {
  typedef Powerset<FCAIBVP> PSET;

  Variable A(0);

  PSET ps1;
  ps1.add_disjunct(FCAIBVP(A));

  PSET ps2 = ps1;

  if (ps2 != ps1 || !(ps2 == ps1))
    return false;

  using IO_Operators::operator<<;
  nout << "ps1:" << endl << ps1 << endl;

  FCAIBVP d(A);
  PSET ps3(d);

  if (!ps1.definitely_entails(ps3))
    return false;

  if (ps3.is_top())
    return false;

  if (ps1.is_bottom())
    return false;

  nout << "Total memory: " << ps3.total_memory_in_bytes() << endl
       << "External memory: " << ps3.external_memory_in_bytes() << endl;

  ps3.omega_reduce();

  if (ps3.size() == 0)
    return false;

  if (ps3.empty())
    return false;

  // Iterator.
  dimension_type count = 0;
  for (PSET::iterator i = ps3.begin(); i != ps3.end(); ++i)
    ++count;
  if (count != 1)
    return false;

  // Constant iterator.
  count = 0;
  for (PSET::const_iterator i = ps3.begin(); i != ps3.end(); ++i)
    ++count;
  if (count != 1)
    return false;

  // Reverse iterator.
  count = 0;
  for (PSET::reverse_iterator i = ps3.rbegin(); i != ps3.rend(); ++i)
    ++count;
  if (count != 1)
    return false;

  // Constant reverse iterator.
  count = 0;
  for (PSET::const_reverse_iterator i = ps3.rbegin(),
	 ps3_rend = ps3.rend(); i != ps3_rend; ++i)
    ++count;
  if (count != 1)
    return false;

  ps2 = ps3;
  PSET ps_empty;
  ps2.drop_disjunct(ps2.begin());
  if (ps2 != ps_empty)
    return false;

  ps2 = ps3;
  ps2.drop_disjuncts(ps2.begin(),ps2.end());
  if (ps2 != ps_empty)
    return false;

  ps2 = ps3;
  ps2.clear();
  if (ps2 != ps_empty)
    return false;

  swap(ps3, ps2);
  swap(ps3, ps2);
  if (ps3 != ps1 || ps2 != ps_empty)
    return false;

  ps2 = ps_empty;
  ps2.least_upper_bound_assign(ps3);
  if (ps2 != ps3)
    return false;

  ps2 = ps_empty;
  ps2.upper_bound_assign(ps3);
  if (ps2 != ps3)
    return false;

  Variable B(1);
  ps2 = ps1;
  ps2.meet_assign(ps3);
  if (ps2 != ps3)
    return false;

  ps3.collapse();
  if (ps3.size() != 1)
    return false;

  return true;
}

bool
test02() {
  Variable X(0);
  Variable Y(1);

  FCAIBVP XY(X);
  XY.meet_assign(FCAIBVP(Y));

  Powerset<FCAIBVP> ps;

  ps.add_disjunct(FCAIBVP(X));
  ps.add_disjunct(XY);
  return ps.OK();
}

bool
test03() {
  Variable X(0);
  Variable Y(1);

  FCAIBVP XY(X);
  XY.meet_assign(FCAIBVP(Y));

  Powerset<FCAIBVP> ps;

  ps.add_disjunct(XY);
  ps.add_disjunct(FCAIBVP(X));
  return ps.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
