/* Ptr_Iterator class implementation: inline functions.
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

#ifndef PPL_Ptr_Iterator_inlines_hh
#define PPL_Ptr_Iterator_inlines_hh 1

#include <algorithm>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

namespace Implementation {

template <typename P>
inline const P&
Ptr_Iterator<P>::base() const {
  return p;
}

template <typename P>
inline
Ptr_Iterator<P>::Ptr_Iterator()
  : p(P()) {
}

template <typename P>
inline
Ptr_Iterator<P>::Ptr_Iterator(const P& q)
  : p(q) {
}

template <typename P>
template <typename Q>
inline
Ptr_Iterator<P>::Ptr_Iterator(const Ptr_Iterator<Q>& q)
  : p(q.base()) {
}

template <typename P>
inline typename Ptr_Iterator<P>::reference
Ptr_Iterator<P>::operator*() const {
  return *p;
}

template <typename P>
inline typename Ptr_Iterator<P>::pointer
Ptr_Iterator<P>::operator->() const {
  return p;
}

template <typename P>
inline typename Ptr_Iterator<P>::reference
Ptr_Iterator<P>::operator[](const difference_type m) const {
  return p[m];
}

template <typename P>
inline Ptr_Iterator<P>&
Ptr_Iterator<P>::operator++() {
  ++p;
  return *this;
}

template <typename P>
inline Ptr_Iterator<P>
Ptr_Iterator<P>::operator++(int) {
  return Ptr_Iterator(p++);
}

template <typename P>
inline Ptr_Iterator<P>&
Ptr_Iterator<P>::operator--() {
  --p;
  return *this;
}

template <typename P>
inline Ptr_Iterator<P>
Ptr_Iterator<P>::operator--(int) {
  return Ptr_Iterator(p--);
}


template <typename P>
inline Ptr_Iterator<P>&
Ptr_Iterator<P>::operator+=(const difference_type m) {
  p += m;
  return *this;
}

template <typename P>
inline Ptr_Iterator<P>&
Ptr_Iterator<P>::operator-=(const difference_type m) {
  p -= m;
  return *this;
}

template <typename P>
inline typename Ptr_Iterator<P>::difference_type
Ptr_Iterator<P>::operator-(const Ptr_Iterator& y) const {
  return p - y.p;
}

template <typename P>
inline Ptr_Iterator<P>
Ptr_Iterator<P>::operator+(const difference_type m) const {
  return Ptr_Iterator(p + m);
}

template <typename P>
inline Ptr_Iterator<P>
Ptr_Iterator<P>::operator-(const difference_type m) const {
  return Ptr_Iterator(p - m);
}

template<typename P, typename Q>
inline bool
operator==(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() == y.base();
}

template<typename P, typename Q>
inline bool
operator!=(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() != y.base();
}

template<typename P, typename Q>
inline bool
operator<(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() < y.base();
}

template<typename P, typename Q>
inline bool
operator<=(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() <= y.base();
}

template<typename P, typename Q>
inline bool
operator>(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() > y.base();
}

template<typename P, typename Q>
inline bool
operator>=(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() >= y.base();
}

template<typename P, typename Q>
inline typename Ptr_Iterator<P>::difference_type
operator-(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y) {
  return x.base() - y.base();
}

template<typename P>
inline Ptr_Iterator<P>
operator+(typename Ptr_Iterator<P>::difference_type m,
	  const Ptr_Iterator<P>& y) {
  return Ptr_Iterator<P>(m + y.base());
}

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Ptr_Iterator_inlines_hh)
