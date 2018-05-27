/* Test Pointset_Powerset<Grid>.
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

#if PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Partitions the grid \p qq according to the congruence \p c.
/*! \relates Parma_Polyhedra_Library::Pointset_Powerset
  On exit, the intersection of \p qq and congruence \p c is stored
  in \p qq, whereas the intersection of \p qq with the negation of \p c
  is added, as a set of new disjuncts, to the powerset \p r.
*/
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
void
partition_aux(const Congruence& c,
	      Grid& qq,
	      Pointset_Powerset<Grid>& r) {
  const Coefficient& c_modulus = c.modulus();
  const Coefficient& c_inhomogeneous_term = c.inhomogeneous_term();
  Linear_Expression le(c);
  le -= c_inhomogeneous_term;
  PPL_DIRTY_TEMP_COEFFICIENT(n);
  rem_assign(n, c_inhomogeneous_term, c_modulus);
  PPL_DIRTY_TEMP_COEFFICIENT(i);
  for (i = c_modulus; i-- > 0; )
    if (i != n) {
      Grid qqq(qq);
      qqq.add_congruence((le+i %= 0) / c_modulus);
      if (qqq.is_empty())
	r.add_disjunct(qqq);
    }
  qq.add_congruence(c);
}

/*! \relates Pointset_Powerset */
std::pair<Grid, Pointset_Powerset<Grid> >
partition(const Grid& p, const Grid& q) {
  Pointset_Powerset<Grid> r(p.space_dimension(), EMPTY);
  Grid qq = q;
  const Congruence_System& pcs = p.congruences();
  for (Congruence_System::const_iterator i = pcs.begin(),
	 pcs_end = pcs.end(); i != pcs_end; ++i)
    partition_aux(*i, qq, r);
  return std::pair<Grid, Pointset_Powerset<Grid> >(qq, r);
}

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

  std::pair<Grid, Pointset_Powerset<Grid> >
    result = partition(p, q);

  nout << "*** q partition ***" << endl;
  nout << "  +++ p inters q +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  result = partition(q, p);

  nout << "*** p partition ***" << endl;
  nout << "  +++ q inters p +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  return true;
}

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
