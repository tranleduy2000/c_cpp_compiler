/* Congruence_System class implementation: inline functions.
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

#ifndef PPL_Congruence_System_inlines_hh
#define PPL_Congruence_System_inlines_hh 1

#include "Congruence_defs.hh"

namespace Parma_Polyhedra_Library {

inline const Congruence&
Congruence_System::operator[](const dimension_type k) const {
  return rows[k];
}

inline dimension_type
Congruence_System::num_rows() const {
  return rows.size();
}

inline bool
Congruence_System::has_no_rows() const {
  return num_rows() == 0;
}

inline void
Congruence_System::remove_trailing_rows(dimension_type n) {
  PPL_ASSERT(num_rows() >= n);
  rows.resize(num_rows() - n);
}

inline void
Congruence_System::insert(const Congruence& cg) {
  Congruence tmp = cg;
  insert(tmp, Recycle_Input());
}

inline void
Congruence_System::insert(Congruence& cg, Recycle_Input) {
  PPL_ASSERT(cg.OK());
  cg.strong_normalize();
  PPL_ASSERT(cg.OK());
  insert_verbatim(cg, Recycle_Input());
  PPL_ASSERT(OK());
}

inline
Congruence_System::Congruence_System(Representation r)
  : rows(),
    space_dimension_(0),
    representation_(r) {
}

inline
Congruence_System::Congruence_System(const Congruence& cg, Representation r)
  : rows(),
    space_dimension_(0),
    representation_(r) {
  insert(cg);
}

inline
Congruence_System::Congruence_System(const Constraint& c, Representation r)
  : rows(),
    space_dimension_(0),
    representation_(r) {
  insert(c);
}

inline
Congruence_System::Congruence_System(const Congruence_System& cgs)
  : rows(cgs.rows),
    space_dimension_(cgs.space_dimension_),
    representation_(cgs.representation_) {
}

inline
Congruence_System::Congruence_System(const Congruence_System& cgs,
                                     Representation r)
  : rows(cgs.rows),
    space_dimension_(cgs.space_dimension_),
    representation_(r) {
  if (cgs.representation() != r) {
    for (dimension_type i = 0; i < num_rows(); ++i)
      rows[i].set_representation(representation());
  }
}

inline
Congruence_System::Congruence_System(const dimension_type d, Representation r)
  : rows(),
    space_dimension_(d),
    representation_(r) {
}

inline
Congruence_System::~Congruence_System() {
}

inline Congruence_System&
Congruence_System::operator=(const Congruence_System& y) {
  Congruence_System tmp = y;
  swap(*this, tmp);
  return *this;
}

inline Representation
Congruence_System::representation() const {
  return representation_;
}

inline void
Congruence_System::set_representation(Representation r) {
  if (representation_ == r)
    return;
  representation_ = r;
  for (dimension_type i = 0; i < num_rows(); ++i)
    rows[i].set_representation(r);
  PPL_ASSERT(OK());
}

inline dimension_type
Congruence_System::max_space_dimension() {
  return Congruence::max_space_dimension();
}

inline dimension_type
Congruence_System::space_dimension() const {
  return space_dimension_;
}

inline void
Congruence_System::clear() {
  rows.clear();
  space_dimension_ = 0;
}

inline const Congruence_System&
Congruence_System::zero_dim_empty() {
  PPL_ASSERT(zero_dim_empty_p != 0);
  return *zero_dim_empty_p;
}

inline
Congruence_System::const_iterator::const_iterator()
  : i(), csp(0) {
}

inline
Congruence_System::const_iterator::const_iterator(const const_iterator& y)
  : i(y.i), csp(y.csp) {
}

inline
Congruence_System::const_iterator::~const_iterator() {
}

inline Congruence_System::const_iterator&
Congruence_System::const_iterator::operator=(const const_iterator& y) {
  i = y.i;
  csp = y.csp;
  return *this;
}

inline const Congruence&
Congruence_System::const_iterator::operator*() const {
  return *i;
}

inline const Congruence*
Congruence_System::const_iterator::operator->() const {
  return i.operator->();
}

inline Congruence_System::const_iterator&
Congruence_System::const_iterator::operator++() {
  ++i;
  skip_forward();
  return *this;
}

inline Congruence_System::const_iterator
Congruence_System::const_iterator::operator++(int) {
  const const_iterator tmp = *this;
  operator++();
  return tmp;
}

inline bool
Congruence_System::const_iterator::operator==(const const_iterator& y) const {
  return i == y.i;
}

inline bool
Congruence_System::const_iterator::operator!=(const const_iterator& y) const {
  return i != y.i;
}

inline
Congruence_System::const_iterator::
const_iterator(const Swapping_Vector<Congruence>::const_iterator& iter,
               const Congruence_System& cgs)
  : i(iter), csp(&cgs.rows) {
}

inline Congruence_System::const_iterator
Congruence_System::begin() const {
  const_iterator i(rows.begin(), *this);
  i.skip_forward();
  return i;
}

inline Congruence_System::const_iterator
Congruence_System::end() const {
  const const_iterator i(rows.end(), *this);
  return i;
}

inline bool
Congruence_System::empty() const {
  return begin() == end();
}

inline void
Congruence_System::m_swap(Congruence_System& y) {
  using std::swap;
  swap(rows, y.rows);
  swap(space_dimension_, y.space_dimension_);
  swap(representation_, y.representation_);
  PPL_ASSERT(OK());
  PPL_ASSERT(y.OK());
}

inline memory_size_type
Congruence_System::external_memory_in_bytes() const {
  return rows.external_memory_in_bytes();
}

inline memory_size_type
Congruence_System::total_memory_in_bytes() const {
  return rows.external_memory_in_bytes() + sizeof(*this);
}

/*! \relates Congruence_System */
inline void
swap(Congruence_System& x, Congruence_System& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Congruence_System_inlines_hh)
