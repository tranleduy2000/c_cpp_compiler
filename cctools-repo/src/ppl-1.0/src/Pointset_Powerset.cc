/* Pointset_Powerset class implementation: non-inline functions.
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

#include "ppl-config.h"
#include "Pointset_Powerset.defs.hh"
#include "Grid.defs.hh"
#include <utility>

namespace PPL = Parma_Polyhedra_Library;

template <>
void
PPL::Pointset_Powerset<PPL::NNC_Polyhedron>
::difference_assign(const Pointset_Powerset& y) {
  Pointset_Powerset& x = *this;
  using std::swap;
  // Ensure omega-reduction.
  x.omega_reduce();
  y.omega_reduce();
  Sequence new_sequence = x.sequence;
  for (const_iterator yi = y.begin(), y_end = y.end(); yi != y_end; ++yi) {
    const NNC_Polyhedron& ph_yi = yi->pointset();
    Sequence tmp_sequence;
    for (Sequence_const_iterator itr = new_sequence.begin(),
	   ns_end = new_sequence.end(); itr != ns_end; ++itr) {
      std::pair<NNC_Polyhedron, Pointset_Powerset<NNC_Polyhedron> > partition
	= linear_partition(ph_yi, itr->pointset());
      const Pointset_Powerset<NNC_Polyhedron>& residues = partition.second;
      // Append the contents of `residues' to `tmp_sequence'.
      std::copy(residues.begin(), residues.end(), back_inserter(tmp_sequence));
    }
    swap(tmp_sequence, new_sequence);
  }
  swap(x.sequence, new_sequence);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <>
bool
PPL::Pointset_Powerset<PPL::NNC_Polyhedron>
::geometrically_covers(const Pointset_Powerset& y) const {
  const Pointset_Powerset& x = *this;
  for (const_iterator yi = y.begin(), y_end = y.end(); yi != y_end; ++yi)
    if (!check_containment(yi->pointset(), x))
      return false;
  return true;
}

/*! \relates Parma_Polyhedra_Library::Pointset_Powerset */
bool
PPL::check_containment(const NNC_Polyhedron& ph,
		       const Pointset_Powerset<NNC_Polyhedron>& ps) {
  if (ph.is_empty())
    return true;
  Pointset_Powerset<NNC_Polyhedron> tmp(ph.space_dimension(), EMPTY);
  tmp.add_disjunct(ph);
  for (Pointset_Powerset<NNC_Polyhedron>::const_iterator
	 i = ps.begin(), ps_end = ps.end(); i != ps_end; ++i) {
    const NNC_Polyhedron& pi = i->pointset();
    for (Pointset_Powerset<NNC_Polyhedron>::iterator
	   j = tmp.begin(); j != tmp.end(); ) {
      const NNC_Polyhedron& pj = j->pointset();
      if (pi.contains(pj))
	j = tmp.drop_disjunct(j);
      else
	++j;
    }
    if (tmp.empty())
      return true;
    else {
      Pointset_Powerset<NNC_Polyhedron> new_disjuncts(ph.space_dimension(),
						      EMPTY);
      for (Pointset_Powerset<NNC_Polyhedron>::iterator
	     j = tmp.begin(); j != tmp.end(); ) {
	const NNC_Polyhedron& pj = j->pointset();
	if (pj.is_disjoint_from(pi))
	  ++j;
	else {
	  std::pair<NNC_Polyhedron, Pointset_Powerset<NNC_Polyhedron> >
	    partition = linear_partition(pi, pj);
	  new_disjuncts.upper_bound_assign(partition.second);
	  j = tmp.drop_disjunct(j);
	}
      }
      tmp.upper_bound_assign(new_disjuncts);
    }
  }
  return false;
}


namespace {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Uses the congruence \p c to approximately partition the grid \p gr.
/*! \relates Parma_Polyhedra_Library::Pointset_Powerset
  On exit, the intersection of \p gr and congruence \p c is stored
  in \p gr, whereas a finite set of grids whose set-theoretic union
  contains the intersection of \p gr with the negation of \p c
  is added, as a set of new disjuncts, to the powerset \p r.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool
approximate_partition_aux(const PPL::Congruence& c,
			  PPL::Grid& gr,
			  PPL::Pointset_Powerset<PPL::Grid>& r) {
  using namespace PPL;
  const Coefficient& c_modulus = c.modulus();
  Grid gr_copy(gr);
  gr.add_congruence(c);
  if (gr.is_empty()) {
    r.add_disjunct(gr_copy);
    return true;
  }

  Congruence_System cgs = gr.congruences();
  Congruence_System cgs_copy = gr_copy.congruences();
  // When c is an equality, not satisfied by Grid gr
  // then add gr to the set r. There is no finite
  // partition in this case.
  if (c_modulus == 0) {
    if (cgs.num_equalities() != cgs_copy.num_equalities()) {
      r.add_disjunct(gr_copy);
      return false;
    }
    return true;
  }

  // When c is a proper congruence but, in gr, this direction has
  // no congruence, then add gr to the set r. There is no finite
  // partition in this case.
  if (cgs.num_proper_congruences() != cgs_copy.num_proper_congruences()) {
    r.add_disjunct(gr_copy);
    return false;
  }

  // When  c is a proper congruence and gr also is discrete
  // in this direction, then there is a finite partition and that
  // is added to r.
  const Coefficient& c_inhomogeneous_term = c.inhomogeneous_term();
  Linear_Expression le(c);
  le -= c_inhomogeneous_term;
  PPL_DIRTY_TEMP_COEFFICIENT(n);
  rem_assign(n, c_inhomogeneous_term, c_modulus);
  if (n < 0)
    n += c_modulus;
  PPL_DIRTY_TEMP_COEFFICIENT(i);
  for (i = c_modulus; i-- > 0; )
    if (i != n) {
      Grid gr_tmp(gr_copy);
      gr_tmp.add_congruence((le+i %= 0) / c_modulus);
      if (!gr_tmp.is_empty())
	r.add_disjunct(gr_tmp);
    }
  return true;
}

} // namespace

/*! \relates Parma_Polyhedra_Library::Pointset_Powerset */
std::pair<PPL::Grid, PPL::Pointset_Powerset<PPL::Grid> >
PPL::approximate_partition(const Grid& p, const Grid& q,
			   bool& finite_partition) {
  using namespace PPL;
  finite_partition = true;
  Pointset_Powerset<Grid> r(p.space_dimension(), EMPTY);
  // Ensure that the congruence system of q is minimized
  // before copying and calling approximate_partition_aux().
  (void) q.minimized_congruences();
  Grid gr = q;
  const Congruence_System& p_congruences = p.congruences();
  for (Congruence_System::const_iterator i = p_congruences.begin(),
	 p_congruences_end = p_congruences.end(); i != p_congruences_end; ++i)
    if (!approximate_partition_aux(*i, gr, r)) {
      finite_partition = false;
      Pointset_Powerset<Grid> s(q);
      return std::make_pair(gr, s);
    }
  return std::make_pair(gr, r);
}

/*! \relates Parma_Polyhedra_Library::Pointset_Powerset */
bool
PPL::check_containment(const Grid& ph,
		       const Pointset_Powerset<Grid>& ps) {
  if (ph.is_empty())
    return true;
  Pointset_Powerset<Grid> tmp(ph.space_dimension(), EMPTY);
  tmp.add_disjunct(ph);
  for (Pointset_Powerset<Grid>::const_iterator
	 i = ps.begin(), ps_end = ps.end(); i != ps_end; ++i) {
    const Grid& pi = i->pointset();
    for (Pointset_Powerset<Grid>::iterator
	   j = tmp.begin(); j != tmp.end(); ) {
      const Grid& pj = j->pointset();
      if (pi.contains(pj))
	j = tmp.drop_disjunct(j);
      else
	++j;
    }
    if (tmp.empty())
      return true;
    else {
      Pointset_Powerset<Grid> new_disjuncts(ph.space_dimension(),
						      EMPTY);
      for (Pointset_Powerset<Grid>::iterator
	     j = tmp.begin(); j != tmp.end(); ) {
	const Grid& pj = j->pointset();
	if (pj.is_disjoint_from(pi))
	  ++j;
	else {
          bool finite_partition;
	  std::pair<Grid, Pointset_Powerset<Grid> >
	    partition = approximate_partition(pi, pj, finite_partition);

          // If there is a finite partition, then we add the new
          // disjuncts to the temporary set of disjuncts and drop pj.
          // If there is no finite partition, then by the
          // specification of approximate_partition(), we can
          // ignore checking the remaining temporary disjuncts as they
          // will all have the same lines and equalities and therefore
          // also not have a finite partition with respect to pi.
          if (!finite_partition)
            break;
	  new_disjuncts.upper_bound_assign(partition.second);
	  j = tmp.drop_disjunct(j);
	}
      }
      tmp.upper_bound_assign(new_disjuncts);
    }
  }
  return false;
}

template <>
void
PPL::Pointset_Powerset<PPL::Grid>
::difference_assign(const Pointset_Powerset& y) {
  Pointset_Powerset& x = *this;
  using std::swap;
  // Ensure omega-reduction.
  x.omega_reduce();
  y.omega_reduce();
  Sequence new_sequence = x.sequence;
  for (const_iterator yi = y.begin(), y_end = y.end(); yi != y_end; ++yi) {
    const Grid& gr_yi = yi->pointset();
    Sequence tmp_sequence;
    for (Sequence_const_iterator itr = new_sequence.begin(),
	   ns_end = new_sequence.end(); itr != ns_end; ++itr) {
      bool finite_partition;
      std::pair<Grid, Pointset_Powerset<Grid> > partition
	= approximate_partition(gr_yi, itr->pointset(), finite_partition);
      const Pointset_Powerset<Grid>& residues = partition.second;
      // Append the contents of `residues' to `tmp_sequence'.
      std::copy(residues.begin(), residues.end(), back_inserter(tmp_sequence));
    }
    swap(tmp_sequence, new_sequence);
  }
  swap(x.sequence, new_sequence);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <>
bool
PPL::Pointset_Powerset<PPL::Grid>
::geometrically_covers(const Pointset_Powerset& y) const {
  const Pointset_Powerset& x = *this;
  for (const_iterator yi = y.begin(), y_end = y.end(); yi != y_end; ++yi)
    if (!check_containment(yi->pointset(), x))
      return false;
  return true;
}

template <>
template <>
PPL::Pointset_Powerset<PPL::NNC_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<C_Polyhedron>& y,
                    Complexity_Class)
  : Base(), space_dim(y.space_dimension()) {
  Pointset_Powerset& x = *this;
  for (Pointset_Powerset<C_Polyhedron>::const_iterator i = y.begin(),
	 y_end = y.end(); i != y_end; ++i)
    x.sequence.push_back(Determinate<NNC_Polyhedron>
			 (NNC_Polyhedron(i->pointset())));
  x.reduced = y.reduced;
  PPL_ASSERT_HEAVY(x.OK());
}

template <>
template <>
PPL::Pointset_Powerset<PPL::NNC_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<Grid>& y,
                    Complexity_Class)
  : Base(), space_dim(y.space_dimension()) {
  Pointset_Powerset& x = *this;
  for (Pointset_Powerset<Grid>::const_iterator i = y.begin(),
	 y_end = y.end(); i != y_end; ++i)
    x.sequence.push_back(Determinate<NNC_Polyhedron>
			 (NNC_Polyhedron(i->pointset())));
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <>
template <>
PPL::Pointset_Powerset<PPL::C_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<NNC_Polyhedron>& y,
                    Complexity_Class)
  : Base(), space_dim(y.space_dimension()) {
  Pointset_Powerset& x = *this;
  for (Pointset_Powerset<NNC_Polyhedron>::const_iterator i = y.begin(),
	 y_end = y.end(); i != y_end; ++i)
    x.sequence.push_back(Determinate<C_Polyhedron>
			 (C_Polyhedron(i->pointset())));

  // Note: this might be non-reduced even when `y' is known to be
  // omega-reduced, because the constructor of C_Polyhedron, by
  // enforcing topological closure, may have made different elements
  // comparable.
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}
