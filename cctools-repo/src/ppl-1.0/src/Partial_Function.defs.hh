/* Partial_Function class declaration.
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

#ifndef PPL_Partial_Function_defs_hh
#define PPL_Partial_Function_defs_hh 1

#include "Partial_Function.types.hh"
#include "globals.defs.hh"
#include <vector>
#ifndef NDEBUG
#include <set>
#endif
#include <iosfwd>

namespace Parma_Polyhedra_Library {

class Partial_Function {
public:
  /*! \brief
    Default constructor: builds a function with empty codomain
    (i.e., always undefined).
  */
  Partial_Function();

  /*! \brief
    Returns \c true if and only if the represented partial function
    has an empty codomain (i.e., it is always undefined).
  */
  bool has_empty_codomain() const;

  /*! \brief
    If the codomain is \e not empty, returns the maximum value in it.

    \exception std::runtime_error
    Thrown if called when \p *this has an empty codomain.
  */
  dimension_type max_in_codomain() const;

  /*! \brief
    If \p *this maps \p i to a value \c k, assigns \c k to \p j and
    returns \c true; otherwise, \p j is unchanged and \c false is returned.
  */
  bool maps(dimension_type i, dimension_type& j) const;

  void print(std::ostream& s) const;

  /*! \brief
    Modifies \p *this so that \p i is mapped to \p j.

    \exception std::runtime_error
    Thrown if \p *this is already mapping \p j.
  */
  void insert(dimension_type i, dimension_type j);

private:
  std::vector<dimension_type> vec;
  dimension_type max;
#ifndef NDEBUG
  std::set<dimension_type> codomain;
#endif
}; // class Partial_Function

} // namespace Parma_Polyhedra_Library

#include "Partial_Function.inlines.hh"

#endif // !defined(PPL_Partial_Function_defs_hh)
