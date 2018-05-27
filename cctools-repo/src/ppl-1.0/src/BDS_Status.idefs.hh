/* BD_Shape<T>::Status class declaration.
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

#ifndef PPL_BDS_Status_idefs_hh
#define PPL_BDS_Status_idefs_hh 1

#ifndef PPL_IN_BD_Shape_CLASS
#error "Do not include BDS_Status.idefs.hh directly; use BD_Shape.defs.hh instead"
#endif

//! A conjunctive assertion about a BD_Shape<T> object.
/*! \ingroup PPL_CXX_interface
  The assertions supported are:
  - <EM>zero-dim universe</EM>: the BDS is the zero-dimensional
    vector space \f$\Rset^0 = \{\cdot\}\f$;
  - <EM>empty</EM>: the BDS is the empty set;
  - <EM>shortest-path closed</EM>: the BDS is represented by a shortest-path
    closed system of bounded differences, so that all the constraints are
    as tight as possible;
  - <EM>shortest-path reduced</EM>: the BDS is represented by a shortest-path
    closed system of bounded differences and each constraint in such a system
    is marked as being either redundant or non-redundant.

  Not all the conjunctions of these elementary assertions constitute
  a legal Status.  In fact:
  - <EM>zero-dim universe</EM> excludes any other assertion;
  - <EM>empty</EM>: excludes any other assertion;
  - <EM>shortest-path reduced</EM> implies <EM>shortest-path closed</EM>.
*/
class Status {
public:
  //! By default Status is the <EM>zero-dim universe</EM> assertion.
  Status();

  //! \name Test, remove or add an individual assertion from the conjunction.
  //@{
  bool test_zero_dim_univ() const;
  void reset_zero_dim_univ();
  void set_zero_dim_univ();

  bool test_empty() const;
  void reset_empty();
  void set_empty();

  bool test_shortest_path_closed() const;
  void reset_shortest_path_closed();
  void set_shortest_path_closed();

  bool test_shortest_path_reduced() const;
  void reset_shortest_path_reduced();
  void set_shortest_path_reduced();
  //@}

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

private:
  //! Status is implemented by means of a finite bitset.
  typedef unsigned int flags_t;

  //! \name Bit-masks for the individual assertions.
  //@{
  static const flags_t ZERO_DIM_UNIV         = 0U;
  static const flags_t EMPTY                 = 1U << 0;
  static const flags_t SHORTEST_PATH_CLOSED  = 1U << 1;
  static const flags_t SHORTEST_PATH_REDUCED = 1U << 2;
  //@}

  //! This holds the current bitset.
  flags_t flags;

  //! Construct from a bit-mask.
  Status(flags_t mask);

  //! Check whether <EM>all</EM> bits in \p mask are set.
  bool test_all(flags_t mask) const;

  //! Check whether <EM>at least one</EM> bit in \p mask is set.
  bool test_any(flags_t mask) const;

  //! Set the bits in \p mask.
  void set(flags_t mask);

  //! Reset the bits in \p mask.
  void reset(flags_t mask);
};

#endif // !defined(PPL_BDS_Status_idefs_hh)
