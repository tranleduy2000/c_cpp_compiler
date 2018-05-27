/* Poly_Con_Relation class declaration.
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

#ifndef PPL_Poly_Con_Relation_defs_hh
#define PPL_Poly_Con_Relation_defs_hh 1

#include "Poly_Con_Relation.types.hh"
#include "globals.defs.hh"
#include <iosfwd>

namespace Parma_Polyhedra_Library {

// Put them in the namespace here to declare them friend later.

//! True if and only if \p x and \p y are logically equivalent.
/*! \relates Poly_Con_Relation */
bool operator==(const Poly_Con_Relation& x, const Poly_Con_Relation& y);

//! True if and only if \p x and \p y are not logically equivalent.
/*! \relates Poly_Con_Relation */
bool operator!=(const Poly_Con_Relation& x, const Poly_Con_Relation& y);

//! Yields the logical conjunction of \p x and \p y.
/*! \relates Poly_Con_Relation */
Poly_Con_Relation operator&&(const Poly_Con_Relation& x,
			     const Poly_Con_Relation& y);

/*! \brief
  Yields the assertion with all the conjuncts of \p x
  that are not in \p y.

  \relates Poly_Con_Relation
*/
Poly_Con_Relation operator-(const Poly_Con_Relation& x,
			    const Poly_Con_Relation& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Poly_Con_Relation */
std::ostream& operator<<(std::ostream& s, const Poly_Con_Relation& r);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library


//! The relation between a polyhedron and a constraint.
/*! \ingroup PPL_CXX_interface
  This class implements conjunctions of assertions on the relation
  between a polyhedron and a constraint.
*/
class Parma_Polyhedra_Library::Poly_Con_Relation {
private:
  //! Poly_Con_Relation is implemented by means of a finite bitset.
  typedef unsigned int flags_t;

  //! \name Bit-masks for the individual assertions
  //@{
  static const flags_t NOTHING             = 0U;
  static const flags_t IS_DISJOINT         = 1U << 0;
  static const flags_t STRICTLY_INTERSECTS = 1U << 1;
  static const flags_t IS_INCLUDED         = 1U << 2;
  static const flags_t SATURATES           = 1U << 3;
  //@} // Bit-masks for the individual assertions

  //! All assertions together.
  static const flags_t EVERYTHING
  = IS_DISJOINT
  | STRICTLY_INTERSECTS
  | IS_INCLUDED
  | SATURATES;

  //! This holds the current bitset.
  flags_t flags;

  //! True if and only if the conjunction \p x implies the conjunction \p y.
  static bool implies(flags_t x, flags_t y);

  //! Construct from a bit-mask.
  Poly_Con_Relation(flags_t mask);

  friend bool
  operator==(const Poly_Con_Relation& x, const Poly_Con_Relation& y);
  friend bool
  operator!=(const Poly_Con_Relation& x, const Poly_Con_Relation& y);

  friend Poly_Con_Relation
  operator&&(const Poly_Con_Relation& x, const Poly_Con_Relation& y);

  friend Poly_Con_Relation
  operator-(const Poly_Con_Relation& x, const Poly_Con_Relation& y);

  friend std::ostream&
  Parma_Polyhedra_Library::
  IO_Operators::operator<<(std::ostream& s, const Poly_Con_Relation& r);

public:
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Access the internal flags: this is needed for some language
    interfaces.
  */
#endif
  flags_t get_flags() const;

public:
  //! The assertion that says nothing.
  static Poly_Con_Relation nothing();

  /*! \brief
    The polyhedron and the set of points satisfying
    the constraint are disjoint.
  */
  static Poly_Con_Relation is_disjoint();

  /*! \brief
    The polyhedron intersects the set of points satisfying
    the constraint, but it is not included in it.
  */
  static Poly_Con_Relation strictly_intersects();

  /*! \brief
    The polyhedron is included in the set of points satisfying
    the constraint.
  */
  static Poly_Con_Relation is_included();

  /*! \brief
    The polyhedron is included in the set of points saturating
    the constraint.
  */
  static Poly_Con_Relation saturates();

  PPL_OUTPUT_DECLARATIONS

  //! True if and only if \p *this implies \p y.
  bool implies(const Poly_Con_Relation& y) const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;
};

#include "Poly_Con_Relation.inlines.hh"

#endif // !defined(PPL_Poly_Con_Relation_defs_hh)
