/* Polyhedron::Status class declaration.
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

#ifndef PPL_Status_idefs_hh
#define PPL_Status_idefs_hh 1

#ifndef PPL_IN_Polyhedron_CLASS
#error "Do not include Ph_Status.idefs.hh directly; use Polyhedron.defs.hh instead"
#endif

//! A conjunctive assertion about a polyhedron.
/*! \ingroup PPL_CXX_interface
  The assertions supported are:
  - <EM>zero-dim universe</EM>: the polyhedron is the zero-dimension
    vector space \f$\Rset^0 = \{\cdot\}\f$;
  - <EM>empty</EM>: the polyhedron is the empty set;
  - <EM>constraints pending</EM>: the polyhedron is correctly
    characterized by the attached system of constraints, which is
    split in two non-empty subsets: the already processed constraints,
    which are in minimal form, and the pending constraints, which
    still have to be processed and may thus be inconsistent or
    contain redundancies;
  - <EM>generators pending</EM>: the polyhedron is correctly
    characterized by the attached system of generators, which is
    split in two non-empty subsets: the already processed generators,
    which are in minimal form, and the pending generators, which still
    have to be processed and may thus contain redundancies;
  - <EM>constraints up-to-date</EM>: the polyhedron is correctly
    characterized by the attached system of constraints, modulo the
    processing of pending generators;
  - <EM>generators up-to-date</EM>: the polyhedron is correctly
    characterized by the attached system of generators, modulo the
    processing of pending constraints;
  - <EM>constraints minimized</EM>: the non-pending part of the system
    of constraints attached to the polyhedron is in minimal form;
  - <EM>generators minimized</EM>: the non-pending part of the system
    of generators attached to the polyhedron is in minimal form;
  - <EM>constraints' saturation matrix up-to-date</EM>: the attached
    saturation matrix having rows indexed by non-pending generators and
    columns indexed by non-pending constraints correctly expresses
    the saturation relation between the attached non-pending constraints
    and generators;
  - <EM>generators' saturation matrix up-to-date</EM>: the attached
    saturation matrix having rows indexed by non-pending constraints and
    columns indexed by non-pending generators correctly expresses
    the saturation relation between the attached non-pending constraints
    and generators;

  Not all the conjunctions of these elementary assertions constitute
  a legal Status.  In fact:
  - <EM>zero-dim universe</EM> excludes any other assertion;
  - <EM>empty</EM>: excludes any other assertion;
  - <EM>constraints pending</EM> and <EM>generators pending</EM>
    are mutually exclusive;
  - <EM>constraints pending</EM> implies both <EM>constraints minimized</EM>
    and <EM>generators minimized</EM>;
  - <EM>generators pending</EM> implies both <EM>constraints minimized</EM>
    and <EM>generators minimized</EM>;
  - <EM>constraints minimized</EM> implies <EM>constraints up-to-date</EM>;
  - <EM>generators minimized</EM> implies <EM>generators up-to-date</EM>;
  - <EM>constraints' saturation matrix up-to-date</EM> implies both
    <EM>constraints up-to-date</EM> and <EM>generators up-to-date</EM>;
  - <EM>generators' saturation matrix up-to-date</EM> implies both
    <EM>constraints up-to-date</EM> and <EM>generators up-to-date</EM>.
*/
class Status {
public:
  //! By default Status is the <EM>zero-dim universe</EM> assertion.
  Status();

  //! \name Test, remove or add an individual assertion from the conjunction
  //@{
  bool test_zero_dim_univ() const;
  void reset_zero_dim_univ();
  void set_zero_dim_univ();

  bool test_empty() const;
  void reset_empty();
  void set_empty();

  bool test_c_up_to_date() const;
  void reset_c_up_to_date();
  void set_c_up_to_date();

  bool test_g_up_to_date() const;
  void reset_g_up_to_date();
  void set_g_up_to_date();

  bool test_c_minimized() const;
  void reset_c_minimized();
  void set_c_minimized();

  bool test_g_minimized() const;
  void reset_g_minimized();
  void set_g_minimized();

  bool test_sat_c_up_to_date() const;
  void reset_sat_c_up_to_date();
  void set_sat_c_up_to_date();

  bool test_sat_g_up_to_date() const;
  void reset_sat_g_up_to_date();
  void set_sat_g_up_to_date();

  bool test_c_pending() const;
  void reset_c_pending();
  void set_c_pending();

  bool test_g_pending() const;
  void reset_g_pending();
  void set_g_pending();
  //@} // Test, remove or add an individual assertion from the conjunction

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

  //! \name Bit-masks for the individual assertions
  //@{
  static const flags_t ZERO_DIM_UNIV    = 0U;
  static const flags_t EMPTY            = 1U << 0;
  static const flags_t C_UP_TO_DATE     = 1U << 1;
  static const flags_t G_UP_TO_DATE     = 1U << 2;
  static const flags_t C_MINIMIZED      = 1U << 3;
  static const flags_t G_MINIMIZED      = 1U << 4;
  static const flags_t SAT_C_UP_TO_DATE = 1U << 5;
  static const flags_t SAT_G_UP_TO_DATE = 1U << 6;
  static const flags_t CS_PENDING       = 1U << 7;
  static const flags_t GS_PENDING       = 1U << 8;
  //@} // Bit-masks for the individual assertions

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

#endif // !defined(PPL_Status_idefs_hh)
