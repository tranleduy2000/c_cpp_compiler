/* Grid_Certificate class declaration.
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

#ifndef PPL_Grid_Certificate_defs_hh
#define PPL_Grid_Certificate_defs_hh 1

#include "Grid_Certificate.types.hh"

#include "Grid.types.hh"
#include "globals.defs.hh"
#include "assert.hh"
#include <vector>

//! The convergence certificate for the Grid widening operator.
/*! \ingroup PPL_CXX_interface
  Convergence certificates are used to instantiate the BHZ03 framework
  so as to define widening operators for the finite powerset domain.

  \note
  Each convergence certificate has to be used together with a
  compatible widening operator. In particular, Grid_Certificate can
  certify the Grid widening.
*/
class Parma_Polyhedra_Library::Grid_Certificate {
public:
  //! Default constructor.
  Grid_Certificate();

  //! Constructor: computes the certificate for \p gr.
  Grid_Certificate(const Grid& gr);

  //! Copy constructor.
  Grid_Certificate(const Grid_Certificate& y);

  //! Destructor.
  ~Grid_Certificate();

  //! The comparison function for certificates.
  /*!
    \return
    \f$-1\f$, \f$0\f$ or \f$1\f$ depending on whether \p *this
    is smaller than, equal to, or greater than \p y, respectively.
  */
  int compare(const Grid_Certificate& y) const;

  //! Compares \p *this with the certificate for grid \p gr.
  int compare(const Grid& gr) const;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Returns <CODE>true</CODE> if and only if the certificate for grid
    \p gr is strictly smaller than \p *this.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool is_stabilizing(const Grid& gr) const;

  //! A total ordering on Grid certificates.
  /*!
    This binary predicate defines a total ordering on Grid certificates
    which is used when storing information about sets of grids.
  */
  struct Compare {
    //! Returns <CODE>true</CODE> if and only if \p x comes before \p y.
    bool operator()(const Grid_Certificate& x,
		    const Grid_Certificate& y) const;
  };

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! Check if gathered information is meaningful.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool OK() const;

private:
  //! Number of a equalities in a minimized congruence system for the
  //! grid.
  dimension_type num_equalities;
  //! Number of a proper congruences in a minimized congruence system
  //! for the grid.
  dimension_type num_proper_congruences;
};

#include "Grid_Certificate.inlines.hh"

#endif // !defined(PPL_Grid_Certificate_defs_hh)
