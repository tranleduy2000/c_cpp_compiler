/* Poly_Gen_Relation class implementation: inline functions.
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

#ifndef PPL_Poly_Gen_Relation_inlines_hh
#define PPL_Poly_Gen_Relation_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
Poly_Gen_Relation::Poly_Gen_Relation(flags_t mask)
  : flags(mask) {
}

inline Poly_Gen_Relation::flags_t
Poly_Gen_Relation::get_flags() const {
  return flags;
}

inline Poly_Gen_Relation
Poly_Gen_Relation::nothing() {
  return Poly_Gen_Relation(NOTHING);
}

inline Poly_Gen_Relation
Poly_Gen_Relation::subsumes() {
  return Poly_Gen_Relation(SUBSUMES);
}

inline bool
Poly_Gen_Relation::implies(flags_t x, flags_t y) {
  return (x & y) == y;
}

inline bool
Poly_Gen_Relation::implies(const Poly_Gen_Relation& y) const {
  return implies(flags, y.flags);
}

/*! \relates Poly_Gen_Relation */
inline bool
operator==(const Poly_Gen_Relation& x, const Poly_Gen_Relation& y) {
  return x.flags == y.flags;
}

/*! \relates Poly_Gen_Relation */
inline bool
operator!=(const Poly_Gen_Relation& x, const Poly_Gen_Relation& y) {
  return x.flags != y.flags;
}

/*! \relates Poly_Gen_Relation */
inline Poly_Gen_Relation
operator&&(const Poly_Gen_Relation& x, const Poly_Gen_Relation& y) {
  return Poly_Gen_Relation(x.flags | y.flags);
}

/*! \relates Poly_Gen_Relation */
inline Poly_Gen_Relation
operator-(const Poly_Gen_Relation& x, const Poly_Gen_Relation& y) {
  return Poly_Gen_Relation(x.flags & ~y.flags);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Poly_Gen_Relation_inlines_hh)
