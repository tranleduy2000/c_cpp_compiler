/* Test approximate_partition().
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

bool
test01() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(x %= 0);

  using namespace IO_Operators;
  nout << "p = " << p << endl;

  Grid q(2);
  q.add_congruence((x %= 4) / 9);

  nout << "q = " << q << endl;

  bool finite_partition;

  std::pair<Grid, Pointset_Powerset<Grid> >
    result = approximate_partition(p, q, finite_partition);

  nout << "*** q partition ***" << endl;
  nout << "  === p inters q === " << endl << "  " << result.first << endl;
  nout << "  ===    rest    === " << endl << "  " << result.second << endl;

  if (!finite_partition)
    return false;

  result = approximate_partition(q, p, finite_partition);

  nout << "*** p partition ***" << endl;
  nout << "  === q inters p === " << endl << "  " << result.first << endl;
  nout << "  ===    rest    === " << endl << "  " << result.second << endl;

  return finite_partition;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Grid p(2);
  p.add_congruence(x %= 0);

  print_congruences(p, "*** p ***");

  Grid q(2);
  q.add_congruence((x %= 0) / 2);
  q.add_congruence((y %= 0) / 1);

  print_congruences(q, "*** q ***");

  bool finite_partition;

  std::pair<Grid, Pointset_Powerset<Grid> >
    result = approximate_partition(q, p, finite_partition);

  print_congruences(result.first,
		    "*** result.first ***");

  print_congruences(result.second,
		    "*** result.second ***");

  if (finite_partition)
    return false;

  Grid known_gr(p);

  Pointset_Powerset<Grid>::iterator i = (result.second).begin();

return i->pointset() == known_gr;
}

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
