/* Grid_Generator_System class implementation: inline functions.
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

#ifndef PPL_Grid_Generator_System_inlines_hh
#define PPL_Grid_Generator_System_inlines_hh 1

#include "Grid_Generator_defs.hh"

namespace Parma_Polyhedra_Library {

inline void
Grid_Generator_System::set_sorted(bool b) {
  sys.set_sorted(b);
}

inline void
Grid_Generator_System::unset_pending_rows() {
  sys.unset_pending_rows();
}

inline void
Grid_Generator_System::set_index_first_pending_row(const dimension_type i) {
  sys.set_index_first_pending_row(i);
}

inline void
Grid_Generator_System
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  return sys.permute_space_dimensions(cycle);
}

inline bool
Grid_Generator_System::is_equal_to(const Grid_Generator_System& y) const {
  return (sys == y.sys);
}

inline
Grid_Generator_System::Grid_Generator_System(Representation r)
  : sys(NECESSARILY_CLOSED, r) {
  sys.set_sorted(false);
  PPL_ASSERT(space_dimension() == 0);
}

inline
Grid_Generator_System::Grid_Generator_System(const Grid_Generator_System& gs)
  : sys(gs.sys) {
}

inline
Grid_Generator_System::Grid_Generator_System(const Grid_Generator_System& gs,
                                             Representation r)
  : sys(gs.sys, r) {
}

inline
Grid_Generator_System::Grid_Generator_System(dimension_type dim,
                                             Representation r)
  : sys(NECESSARILY_CLOSED, r) {
  sys.set_space_dimension(dim);
  sys.set_sorted(false);
  PPL_ASSERT(space_dimension() == dim);
}

inline
Grid_Generator_System::Grid_Generator_System(const Grid_Generator& g,
                                             Representation r)
  : sys(NECESSARILY_CLOSED, r) {
  sys.insert(g);
  sys.set_sorted(false);
}

inline
Grid_Generator_System::~Grid_Generator_System() {
}

inline Grid_Generator_System&
Grid_Generator_System::operator=(const Grid_Generator_System& y) {
  Grid_Generator_System tmp = y;
  swap(*this, tmp);
  return *this;
}

inline Representation
Grid_Generator_System::representation() const {
  return sys.representation();
}

inline void
Grid_Generator_System::set_representation(Representation r) {
  sys.set_representation(r);
}

inline dimension_type
Grid_Generator_System::max_space_dimension() {
  // Grid generators use an extra column for the parameter divisor.
  return Linear_System<Grid_Generator>::max_space_dimension() - 1;
}

inline dimension_type
Grid_Generator_System::space_dimension() const {
  return sys.space_dimension();
}

inline const Grid_Generator_System&
Grid_Generator_System::zero_dim_univ() {
  PPL_ASSERT(zero_dim_univ_p != 0);
  return *zero_dim_univ_p;
}

inline void
Grid_Generator_System::clear() {
  sys.clear();
  sys.set_sorted(false);
  sys.unset_pending_rows();
  PPL_ASSERT(space_dimension() == 0);
}

inline void
Grid_Generator_System::m_swap(Grid_Generator_System& y) {
  swap(sys, y.sys);
}

inline memory_size_type
Grid_Generator_System::external_memory_in_bytes() const {
  return sys.external_memory_in_bytes();
}

inline memory_size_type
Grid_Generator_System::total_memory_in_bytes() const {
  return external_memory_in_bytes() + sizeof(*this);
}

inline dimension_type
Grid_Generator_System::num_rows() const {
  return sys.num_rows();
}

inline
Grid_Generator_System::const_iterator::const_iterator()
  : i() {
}

inline
Grid_Generator_System::const_iterator::const_iterator(const const_iterator& y)
  : i(y.i) {
}

inline
Grid_Generator_System::const_iterator::~const_iterator() {
}

inline Grid_Generator_System::const_iterator&
Grid_Generator_System::const_iterator::operator=(const const_iterator& y) {
  i = y.i;
  return *this;
}

inline const Grid_Generator&
Grid_Generator_System::const_iterator::operator*() const {
  return *i;
}

inline const Grid_Generator*
Grid_Generator_System::const_iterator::operator->() const {
  return i.operator->();
}

inline Grid_Generator_System::const_iterator&
Grid_Generator_System::const_iterator::operator++() {
  ++i;
  return *this;
}

inline Grid_Generator_System::const_iterator
Grid_Generator_System::const_iterator::operator++(int) {
  const const_iterator tmp = *this;
  operator++();
  return tmp;
}

inline bool
Grid_Generator_System
::const_iterator::operator==(const const_iterator& y) const {
  return i == y.i;
}

inline bool
Grid_Generator_System
::const_iterator::operator!=(const const_iterator& y) const {
  return i != y.i;
}

inline bool
Grid_Generator_System::empty() const {
  return sys.has_no_rows();
}

inline
Grid_Generator_System::const_iterator
::const_iterator(const Linear_System<Grid_Generator>::const_iterator& y)
  : i(y) {
}

inline Grid_Generator_System::const_iterator
Grid_Generator_System::begin() const {
  return static_cast<Grid_Generator_System::const_iterator>(sys.begin());
}

inline Grid_Generator_System::const_iterator
Grid_Generator_System::end() const {
  return static_cast<Grid_Generator_System::const_iterator>(sys.end());
}

inline const Grid_Generator&
Grid_Generator_System::operator[](const dimension_type k) const {
  return sys[k];
}

inline bool
Grid_Generator_System::has_no_rows() const {
  return sys.has_no_rows();
}

inline void
Grid_Generator_System::remove_trailing_rows(dimension_type n) {
  sys.remove_trailing_rows(n);
}

inline void
Grid_Generator_System::insert_verbatim(const Grid_Generator& g) {
  sys.insert(g);
}

inline Topology
Grid_Generator_System::topology() const {
  return sys.topology();
}

inline dimension_type
Grid_Generator_System::first_pending_row() const {
  return sys.first_pending_row();
}

/*! \relates Grid_Generator_System */
inline bool
operator==(const Grid_Generator_System& x,
           const Grid_Generator_System& y) {
  return x.is_equal_to(y);
}

/*! \relates Grid_Generator_System */
inline void
swap(Grid_Generator_System& x, Grid_Generator_System& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Grid_Generator_System_inlines_hh)
