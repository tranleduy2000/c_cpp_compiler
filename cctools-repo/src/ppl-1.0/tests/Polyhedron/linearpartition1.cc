/* Test linear_partition().
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
aux_test01(const C_Polyhedron& p,
	   const C_Polyhedron& q,
	   const std::pair<C_Polyhedron,
	   Pointset_Powerset<NNC_Polyhedron> >& partition) {
  const C_Polyhedron& r = partition.first;
  // `r' must be a subset of or equal to `q'.
  if (!q.contains(r))
    return false;
  const Pointset_Powerset<NNC_Polyhedron>& s = partition.second;
  NNC_Polyhedron the_union(r);
  // These are the NNC versions of `p' and `q'.
  NNC_Polyhedron nnc_p(p);
  NNC_Polyhedron nnc_q(q);
  typedef Pointset_Powerset<NNC_Polyhedron>::const_iterator iter;
  for (iter i = s.begin(), s_end = s.end(); i != s_end; ++i) {
    const NNC_Polyhedron& a = i->pointset();
    // All elements of `s' must be disjoint from `p'.
    if (!a.is_disjoint_from(nnc_p))
      return false;
    iter j = i;
    for (++j; j != s_end; ++j) {
      const NNC_Polyhedron& b = j->pointset();
      // All elements of `s' must be pairwise disjoint.
      if (!a.is_disjoint_from(b))
	return false;
    }
    the_union.upper_bound_assign(a);
  }
  // The union of all the elements in `partition' must be exactly `q'.
  return the_union == nnc_q;
}

bool
test01() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2);
  p.add_constraint(x == 0);
  p.add_constraint(y >= 0);
  p.add_constraint(y <= 2);

  using namespace IO_Operators;
  nout << "p = " << p << endl;

  C_Polyhedron q(2);
  q.add_constraint(x >= -1);
  q.add_constraint(x <=  1);
  q.add_constraint(y >=  1);
  q.add_constraint(y <=  3);

  nout << "q = " << q << endl;

  std::pair<C_Polyhedron, Pointset_Powerset<NNC_Polyhedron> >
    result = linear_partition(p, q);

  nout << "*** q partition ***" << endl;
  nout << "  +++ p inters q +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  if (!aux_test01(p, q, result))
    return false;

  result = linear_partition(q, p);

  nout << "*** p partition ***" << endl;
  nout << "  +++ q inters p +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  return aux_test01(q, p, result);
}

bool
aux_test02(const C_Polyhedron& p,
	   const C_Polyhedron& q,
	   const std::pair<C_Polyhedron,
	   Pointset_Powerset<NNC_Polyhedron> >& partition) {
  const C_Polyhedron& r = partition.first;
  // `r' must be a subset of or equal to `q'.
  if (!q.contains(r))
    return false;
  const Pointset_Powerset<NNC_Polyhedron>& s = partition.second;
  NNC_Polyhedron the_union(r);
  // These are the NNC versions of `p' and `q'.
  NNC_Polyhedron nnc_p(p);
  NNC_Polyhedron nnc_q(q);
  typedef Pointset_Powerset<NNC_Polyhedron>::const_iterator iter;
  for (iter i = s.begin(), s_end = s.end(); i != s_end; ++i) {
    const NNC_Polyhedron& a = i->pointset();
    // All elements of `s' must be disjoint from `p'.
    if (!a.is_disjoint_from(nnc_p))
      return false;
    iter j = i;
    for (++j; j != s_end; ++j) {
      const NNC_Polyhedron& b = j->pointset();
      // All elements of `s' must be pairwise disjoint.
      if (!a.is_disjoint_from(b))
	return false;
    }
    the_union.upper_bound_assign(a);
  }
  // The union of all the elements in `partition' must be exactly `q'.
  return the_union == nnc_q;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2);
  p.add_constraint(x == 5);
  p.add_constraint(y >= 0);
  p.add_constraint(y <= 2);

  using namespace IO_Operators;
  nout << "p = " << p << endl;

  C_Polyhedron q(2);
  q.add_constraint(x >= -1);
  q.add_constraint(x <=  1);
  q.add_constraint(y >=  1);
  q.add_constraint(y <=  3);

  nout << "q = " << q << endl;

  std::pair<C_Polyhedron, Pointset_Powerset<NNC_Polyhedron> >
    result = linear_partition(p, q);

  nout << "*** q partition ***" << endl;
  nout << "  +++ p inters q +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  if (!aux_test01(p, q, result))
    return false;

  result = linear_partition(q, p);

  nout << "*** p partition ***" << endl;
  nout << "  +++ q inters p +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  return aux_test02(q, p, result);
}

bool
aux_test03(const C_Polyhedron& p,
	   const C_Polyhedron& q,
	   const std::pair<C_Polyhedron,
	   Pointset_Powerset<NNC_Polyhedron> >& partition) {
  const C_Polyhedron& r = partition.first;
  // `r' must be a subset of or equal to `q'.
  if (!q.contains(r))
    return false;
  const Pointset_Powerset<NNC_Polyhedron>& s = partition.second;
  NNC_Polyhedron the_union(r);
  // These are the NNC versions of `p' and `q'.
  NNC_Polyhedron nnc_p(p);
  NNC_Polyhedron nnc_q(q);
  typedef Pointset_Powerset<NNC_Polyhedron>::const_iterator iter;
  for (iter i = s.begin(), s_end = s.end(); i != s_end; ++i) {
    const NNC_Polyhedron& a = i->pointset();
    // All elements of `s' must be disjoint from `p'.
    if (!a.is_disjoint_from(nnc_p))
      return false;
    iter j = i;
    for (++j; j != s_end; ++j) {
      const NNC_Polyhedron& b = j->pointset();
      // All elements of `s' must be pairwise disjoint.
      if (!a.is_disjoint_from(b))
	return false;
    }
    the_union.upper_bound_assign(a);
  }
  // The union of all the elements in `partition' must be exactly `q'.
  return the_union == nnc_q;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2);
  p.add_constraint(x >= -2);
  p.add_constraint(x <=  2);
  p.add_constraint(y >=  0);
  p.add_constraint(y <=  4);

  using namespace IO_Operators;
  nout << "p = " << p << endl;

  C_Polyhedron q(2);
  q.add_constraint(x >= -1);
  q.add_constraint(x <=  1);
  q.add_constraint(y >=  1);
  q.add_constraint(y <=  3);

  nout << "q = " << q << endl;

  std::pair<C_Polyhedron, Pointset_Powerset<NNC_Polyhedron> >
    result = linear_partition(p, q);

  nout << "*** q partition ***" << endl;
  nout << "  +++ p inters q +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  if (!aux_test03(p, q, result))
    return false;

  result = linear_partition(q, p);

  nout << "*** p partition ***" << endl;
  nout << "  +++ q inters p +++" << endl << "  " << result.first << endl;
  nout << "  +++    rest    +++" << endl << "  " << result.second << endl;

  return aux_test03(q, p, result);
}

// Compute the complement.
template <typename PH>
Pointset_Powerset<NNC_Polyhedron>
aux_test04(const PH& ph) {
  std::pair<PH, Pointset_Powerset<NNC_Polyhedron> > partition
    = linear_partition(ph, PH(ph.space_dimension(), UNIVERSE));
  return partition.second;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2, EMPTY);
  p.add_generator(point(x));
  p.add_generator(point(y));
  p.add_generator(point(-x));
  p.add_generator(point(-y));

  using namespace IO_Operators;

  nout << "p = " << p << endl;

  Pointset_Powerset<NNC_Polyhedron> p_c = aux_test04(p);

  nout << "complement(p) = " << p_c << endl;

  Pointset_Powerset<NNC_Polyhedron> p_p(p.space_dimension(), EMPTY);
  p_p.add_disjunct(NNC_Polyhedron(p));
  p_p.intersection_assign(p_c);

  nout << "p intersected with complement(p) = " << p_p << endl;

  if (!p_p.empty())
    return false;

  p_c.add_disjunct(NNC_Polyhedron(p));
  p_c.pairwise_reduce();

  nout << "p added to complement(p), pairwise reduced = " << p_c << endl;

  if (!p_c.is_top())
    return false;

  C_Polyhedron q(2);
  q.add_constraint(x >= -1);
  q.add_constraint(x <=  1);
  q.add_constraint(y >=  1);
  q.add_constraint(y <=  3);

  nout << "q = " << q << endl;

  Pointset_Powerset<NNC_Polyhedron> q_c = aux_test04(q);

  nout << "complement(q) = " << q_c << endl;

  Pointset_Powerset<NNC_Polyhedron> q_p(q.space_dimension(), EMPTY);
  q_p.add_disjunct(NNC_Polyhedron(q));
  q_p.intersection_assign(q_c);

  nout << "q intersected with complement(q) = " << q_p << endl;

  if (!q_p.empty())
    return false;

  q_c.add_disjunct(NNC_Polyhedron(q));
  q_c.pairwise_reduce();

  nout << "q added to complement(q), pairwise reduced = " << q_c << endl;

  if (!q_c.is_top())
    return false;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
