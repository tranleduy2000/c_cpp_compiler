/* Variables_Set class declaration.
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

#ifndef PPL_Variables_Set_defs_hh
#define PPL_Variables_Set_defs_hh 1

#include "Variables_Set.types.hh"
#include "Variable.defs.hh"
#include "globals.types.hh"
#include <iosfwd>
#include <set>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Variables_Set */
std::ostream&
operator<<(std::ostream& s, const Variables_Set& vs);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

//! An std::set of variables' indexes.
class Parma_Polyhedra_Library::Variables_Set
  : public std::set<dimension_type> {
private:
  typedef std::set<dimension_type> Base;

public:
  //! Builds the empty set of variable indexes.
  Variables_Set();

  //! Builds the singleton set of indexes containing <CODE>v.id()</CODE>;
  explicit Variables_Set(const Variable v);

  /*! \brief
    Builds the set of variables's indexes in the range from
    <CODE>v.id()</CODE> to <CODE>w.id()</CODE>.

    If <CODE>v.id() <= w.id()</CODE>, this constructor builds the
    set of variables' indexes
    <CODE>v.id()</CODE>, <CODE>v.id()+1</CODE>, ..., <CODE>w.id()</CODE>.
    The empty set is built otherwise.
  */
  Variables_Set(const Variable v, const Variable w);

  //! Returns the maximum space dimension a Variables_Set can handle.
  static dimension_type max_space_dimension();

  /*! \brief
    Returns the dimension of the smallest vector space enclosing all
    the variables whose indexes are in the set.
  */
  dimension_type space_dimension() const;

  //! Inserts the index of variable \p v into the set.
  void insert(Variable v);

  // The `insert' method above overloads (instead of hiding) the
  // other `insert' method of std::set.
  using Base::insert;

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  PPL_OUTPUT_DECLARATIONS
};

#include "Variables_Set.inlines.hh"

#endif // !defined(PPL_Variables_Set_defs_hh)
