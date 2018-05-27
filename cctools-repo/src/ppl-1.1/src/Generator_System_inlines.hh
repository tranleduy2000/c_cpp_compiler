/* Generator_System class implementation: inline functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Generator_System_inlines_hh
#define PPL_Generator_System_inlines_hh 1

#include "Generator_defs.hh"

namespace Parma_Polyhedra_Library {

inline
Generator_System::Generator_System(Representation r)
  : sys(NECESSARILY_CLOSED, r) {
}

inline
Generator_System::Generator_System(const Generator& g, Representation r)
  : sys(g.topology(), r) {
  sys.insert(g);
}

inline
Generator_System::Generator_System(const Generator_System& gs)
  : sys(gs.sys) {
}

inline
Generator_System::Generator_System(const Generator_System& gs,
                                   Representation r)
  : sys(gs.sys, r) {
}

inline
Generator_System::Generator_System(const Topology topol, Representation r)
  : sys(topol, r) {
}

inline
Generator_System::Generator_System(const Topology topol,
                                   const dimension_type space_dim,
                                   Representation r)
  : sys(topol, space_dim, r) {
}

inline
Generator_System::~Generator_System() {
}

inline Generator_System&
Generator_System::operator=(const Generator_System& y) {
  Generator_System tmp = y;
  swap(*this, tmp);
  return *this;
}

inline Representation
Generator_System::representation() const {
  return sys.representation();
}

inline void
Generator_System::set_representation(Representation r) {
  sys.set_representation(r);
}

inline dimension_type
Generator_System::max_space_dimension() {
  return Linear_System<Generator>::max_space_dimension();
}

inline dimension_type
Generator_System::space_dimension() const {
  return sys.space_dimension();
}

inline void
Generator_System::set_space_dimension(dimension_type space_dim) {
  const dimension_type old_space_dim = space_dimension();
  sys.set_space_dimension_no_ok(space_dim);

  if (space_dim < old_space_dim)
    // We may have invalid lines and rays now.
    remove_invalid_lines_and_rays();

#ifndef NDEBUG
  for (dimension_type i = 0; i < sys.num_rows(); ++i)
    PPL_ASSERT(sys[i].OK());
#endif
  PPL_ASSERT(sys.OK());
  PPL_ASSERT(OK());
}

inline void
Generator_System::clear() {
  sys.clear();
}

inline const Generator&
Generator_System::operator[](const dimension_type k) const {
  return sys[k];
}

inline void
Generator_System
::remove_space_dimensions(const Variables_Set& vars) {
  sys.remove_space_dimensions(vars);
}

inline void
Generator_System
::shift_space_dimensions(Variable v, dimension_type n) {
  sys.shift_space_dimensions(v, n);
}

inline void
Generator_System
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  sys.permute_space_dimensions(cycle);
}

inline void
Generator_System
::swap_space_dimensions(Variable v1, Variable v2) {
  sys.swap_space_dimensions(v1, v2);
}

inline dimension_type
Generator_System::num_rows() const {
  return sys.num_rows();
}

inline void
Generator_System::add_universe_rows_and_space_dimensions(dimension_type n) {
  sys.add_universe_rows_and_space_dimensions(n);
}

inline Topology
Generator_System::topology() const {
  return sys.topology();
}

inline dimension_type
Generator_System::first_pending_row() const {
  return sys.first_pending_row();
}

inline void
Generator_System::unset_pending_rows() {
  sys.unset_pending_rows();
}

inline void
Generator_System::set_sorted(bool b) {
  sys.set_sorted(b);
}

inline bool
Generator_System::is_sorted() const {
  return sys.is_sorted();
}

inline void
Generator_System::set_index_first_pending_row(dimension_type i) {
  sys.set_index_first_pending_row(i);
}

inline bool
Generator_System::is_necessarily_closed() const {
  return sys.is_necessarily_closed();
}

inline void
Generator_System::assign_with_pending(const Generator_System& y) {
  sys.assign_with_pending(y.sys);
}

inline dimension_type
Generator_System::num_pending_rows() const {
  return sys.num_pending_rows();
}

inline void
Generator_System::sort_pending_and_remove_duplicates() {
  return sys.sort_pending_and_remove_duplicates();
}

inline void
Generator_System::sort_and_remove_with_sat(Bit_Matrix& sat) {
  sys.sort_and_remove_with_sat(sat);
}

inline void
Generator_System::sort_rows() {
  sys.sort_rows();
}

inline bool
Generator_System::check_sorted() const {
  return sys.check_sorted();
}

inline dimension_type
Generator_System::num_lines_or_equalities() const {
  return sys.num_lines_or_equalities();
}

inline void
Generator_System::remove_row(dimension_type i, bool keep_sorted) {
  sys.remove_row(i, keep_sorted);
}

inline void
Generator_System::remove_rows(dimension_type first, dimension_type last,
                              bool keep_sorted) {
  sys.remove_rows(first, last, keep_sorted);
}

inline void
Generator_System::remove_rows(const std::vector<dimension_type>& indexes) {
  sys.remove_rows(indexes);
}

inline void
Generator_System::remove_trailing_rows(dimension_type n) {
  sys.remove_trailing_rows(n);
}

inline dimension_type
Generator_System::gauss(dimension_type n_lines_or_equalities) {
  return sys.gauss(n_lines_or_equalities);
}

inline void
Generator_System::back_substitute(dimension_type n_lines_or_equalities) {
  sys.back_substitute(n_lines_or_equalities);
}

inline void
Generator_System::strong_normalize() {
  sys.strong_normalize();
}

inline void
Generator_System::merge_rows_assign(const Generator_System& y) {
  sys.merge_rows_assign(y.sys);
}

inline void
Generator_System::insert(const Generator_System& y) {
  sys.insert(y.sys);
}

inline void
Generator_System::insert_pending(const Generator_System& r) {
  sys.insert_pending(r.sys);
}

inline bool
operator==(const Generator_System& x, const Generator_System& y) {
  return x.sys == y.sys;
}

inline bool
operator!=(const Generator_System& x, const Generator_System& y) {
  return !(x == y);
}

inline
Generator_System_const_iterator::Generator_System_const_iterator()
  : i(), gsp(0) {
}

inline
Generator_System_const_iterator::Generator_System_const_iterator(const Generator_System_const_iterator& y)
  : i(y.i), gsp(y.gsp) {
}

inline
Generator_System_const_iterator::~Generator_System_const_iterator() {
}

inline
Generator_System_const_iterator&
Generator_System_const_iterator::operator=(const Generator_System_const_iterator& y) {
  i = y.i;
  gsp = y.gsp;
  return *this;
}

inline const Generator&
Generator_System_const_iterator::operator*() const {
  return *i;
}

inline const Generator*
Generator_System_const_iterator::operator->() const {
  return i.operator->();
}

inline Generator_System_const_iterator&
Generator_System_const_iterator::operator++() {
  ++i;
  if (!gsp->is_necessarily_closed())
    skip_forward();
  return *this;
}

inline Generator_System_const_iterator
Generator_System_const_iterator::operator++(int) {
  const Generator_System_const_iterator tmp = *this;
  operator++();
  return tmp;
}

inline bool
Generator_System_const_iterator::operator==(const Generator_System_const_iterator& y) const {
  return i == y.i;
}

inline bool
Generator_System_const_iterator::operator!=(const Generator_System_const_iterator& y) const {
  return i != y.i;
}

inline
Generator_System_const_iterator::
Generator_System_const_iterator(const Linear_System<Generator>::const_iterator& iter,
                                const Generator_System& gs)
  : i(iter), gsp(&gs.sys) {
}

inline bool
Generator_System::empty() const {
  return sys.has_no_rows();
}

inline bool
Generator_System::has_no_rows() const {
  return sys.has_no_rows();
}

inline Generator_System::const_iterator
Generator_System::begin() const {
  const_iterator i(sys.begin(), *this);
  if (!sys.is_necessarily_closed())
    i.skip_forward();
  return i;
}

inline Generator_System::const_iterator
Generator_System::end() const {
  const const_iterator i(sys.end(), *this);
  return i;
}

inline const Generator_System&
Generator_System::zero_dim_univ() {
  PPL_ASSERT(zero_dim_univ_p != 0);
  return *zero_dim_univ_p;
}

inline void
Generator_System::m_swap(Generator_System& y) {
  swap(sys, y.sys);
}

inline memory_size_type
Generator_System::external_memory_in_bytes() const {
  return sys.external_memory_in_bytes();
}

inline memory_size_type
Generator_System::total_memory_in_bytes() const {
  return external_memory_in_bytes() + sizeof(*this);
}

inline void
Generator_System::simplify() {
  sys.simplify();
  remove_invalid_lines_and_rays();
}

/*! \relates Generator_System */
inline void
swap(Generator_System& x, Generator_System& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Generator_System_inlines_hh)
