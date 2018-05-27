/* iterator_to_const and const_iterator_to_const class implementations:
   inline functions.
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

#ifndef PPL_iterator_to_const_inlines_hh
#define PPL_iterator_to_const_inlines_hh 1

namespace Parma_Polyhedra_Library {

template <typename Container>
inline
iterator_to_const<Container>::iterator_to_const()
  : base() {
}

template <typename Container>
inline
iterator_to_const<Container>::iterator_to_const(const iterator_to_const& y)
  : base(y.base) {
}

template <typename Container>
inline
iterator_to_const<Container>::iterator_to_const(const Base& b)
  : base(b) {
}

template <typename Container>
inline typename iterator_to_const<Container>::reference
iterator_to_const<Container>::operator*() const {
  return *base;
}

template <typename Container>
inline typename iterator_to_const<Container>::pointer
iterator_to_const<Container>::operator->() const {
  return &*base;
}

template <typename Container>
inline iterator_to_const<Container>&
iterator_to_const<Container>::operator++() {
  ++base;
  return *this;
}

template <typename Container>
inline iterator_to_const<Container>
iterator_to_const<Container>::operator++(int) {
  iterator_to_const tmp = *this;
  operator++();
  return tmp;
}

template <typename Container>
inline iterator_to_const<Container>&
iterator_to_const<Container>::operator--() {
  --base;
  return *this;
}

template <typename Container>
inline iterator_to_const<Container>
iterator_to_const<Container>::operator--(int) {
  iterator_to_const tmp = *this;
  operator--();
  return tmp;
}

template <typename Container>
inline bool
iterator_to_const<Container>::operator==(const iterator_to_const& y) const {
  return base == y.base;
}

template <typename Container>
inline bool
iterator_to_const<Container>::operator!=(const iterator_to_const& y) const {
  return !operator==(y);
}

template <typename Container>
inline
const_iterator_to_const<Container>::const_iterator_to_const()
  : base() {
}

template <typename Container>
inline
const_iterator_to_const<Container>
::const_iterator_to_const(const const_iterator_to_const& y)
  : base(y.base) {
}

template <typename Container>
inline
const_iterator_to_const<Container>::const_iterator_to_const(const Base& b)
  : base(b) {
}

template <typename Container>
inline typename const_iterator_to_const<Container>::reference
const_iterator_to_const<Container>::operator*() const {
  return *base;
}

template <typename Container>
inline typename const_iterator_to_const<Container>::pointer
const_iterator_to_const<Container>::operator->() const {
  return &*base;
}

template <typename Container>
inline const_iterator_to_const<Container>&
const_iterator_to_const<Container>::operator++() {
  ++base;
  return *this;
}

template <typename Container>
inline const_iterator_to_const<Container>
const_iterator_to_const<Container>::operator++(int) {
  const_iterator_to_const tmp = *this;
  operator++();
  return tmp;
}

template <typename Container>
inline const_iterator_to_const<Container>&
const_iterator_to_const<Container>::operator--() {
  --base;
  return *this;
}

template <typename Container>
inline const_iterator_to_const<Container>
const_iterator_to_const<Container>::operator--(int) {
  const_iterator_to_const tmp = *this;
  operator--();
  return tmp;
}

template <typename Container>
inline bool
const_iterator_to_const<Container>
::operator==(const const_iterator_to_const& y) const {
  return base == y.base;
}

template <typename Container>
inline bool
const_iterator_to_const<Container>
::operator!=(const const_iterator_to_const& y) const {
  return !operator==(y);
}

template <typename Container>
inline
const_iterator_to_const<Container>
::const_iterator_to_const(const iterator_to_const<Container>& y)
  : base(y.base) {
}

/*! \relates const_iterator_to_const */
template <typename Container>
inline bool
operator==(const iterator_to_const<Container>& x,
	   const const_iterator_to_const<Container>& y) {
  return const_iterator_to_const<Container>(x).operator==(y);
}

/*! \relates const_iterator_to_const */
template <typename Container>
inline bool
operator!=(const iterator_to_const<Container>& x,
	   const const_iterator_to_const<Container>& y) {
  return !(x == y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_iterator_to_const_inlines_hh)
