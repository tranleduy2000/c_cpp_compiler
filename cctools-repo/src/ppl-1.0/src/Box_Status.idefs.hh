/* Box<ITV>::Status class declaration.
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

#ifndef PPL_Box_Status_idefs_hh
#define PPL_Box_Status_idefs_hh 1

#ifndef PPL_IN_Box_CLASS
#error "Do not include Box_Status.idefs.hh directly; use Box.defs.hh instead"
#endif

//! A conjunctive assertion about a Box<ITV> object.
/*! \ingroup PPL_CXX_interface
  The assertions supported are:
  - <EM>empty up-to-date</EM>: the empty flag is meaningful;
  - <EM>empty</EM>: the box is the empty set.
  - <EM>universe</EM>: the box is universe \f$n\f$-dimensional vector space
     \f$\Rset^n\f$.

  Not all the conjunctions of these elementary assertions constitute
  a legal Status.  In fact:
  - <EM>empty up-to-date</EM> and <EM>empty</EM> excludes <EM>universe</EM>.
*/
class Status;

class Status {
public:
  //! By default Status is the empty set of assertion.
  Status();

  //! Ordinary copy constructor.
  Status(const Status& y);

  //! Copy constructor from a box of different type.
  template <typename Other_ITV>
  Status(const typename Box<Other_ITV>::Status& y);

  //! \name Test, remove or add an individual assertion from the conjunction.
  //@{
  bool test_empty_up_to_date() const;
  void reset_empty_up_to_date();
  void set_empty_up_to_date();

  bool test_empty() const;
  void reset_empty();
  void set_empty();

  bool test_universe() const;
  void reset_universe();
  void set_universe();
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
  static const flags_t NONE             = 0U;
  static const flags_t EMPTY_UP_TO_DATE = 1U << 0;
  static const flags_t EMPTY            = 1U << 1;
  static const flags_t UNIVERSE         = 1U << 2;
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

#endif // !defined(PPL_Box_Status_idefs_hh)
