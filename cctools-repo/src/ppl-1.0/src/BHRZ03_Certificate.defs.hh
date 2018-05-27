/* BHRZ03_Certificate class declaration.
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

#ifndef PPL_BHRZ03_Certificate_defs_hh
#define PPL_BHRZ03_Certificate_defs_hh 1

#include "BHRZ03_Certificate.types.hh"
#include "Polyhedron.types.hh"
#include "globals.defs.hh"
#include "assert.hh"
#include <vector>

//! The convergence certificate for the BHRZ03 widening operator.
/*! \ingroup PPL_CXX_interface
  Convergence certificates are used to instantiate the BHZ03 framework
  so as to define widening operators for the finite powerset domain.

  \note
  Each convergence certificate has to be used together with a
  compatible widening operator. In particular, BHRZ03_Certificate
  can certify the convergence of both the BHRZ03 and the H79 widenings.
*/
class Parma_Polyhedra_Library::BHRZ03_Certificate {
public:
  //! Default constructor.
  BHRZ03_Certificate();

  //! Constructor: computes the certificate for \p ph.
  BHRZ03_Certificate(const Polyhedron& ph);

  //! Copy constructor.
  BHRZ03_Certificate(const BHRZ03_Certificate& y);

  //! Destructor.
  ~BHRZ03_Certificate();

  //! The comparison function for certificates.
  /*!
    \return
    \f$-1\f$, \f$0\f$ or \f$1\f$ depending on whether \p *this
    is smaller than, equal to, or greater than \p y, respectively.

    Compares \p *this with \p y, using a total ordering which is a
    refinement of the limited growth ordering relation for the
    BHRZ03 widening.
  */
  int compare(const BHRZ03_Certificate& y) const;

  //! Compares \p *this with the certificate for polyhedron \p ph.
  int compare(const Polyhedron& ph) const;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Returns <CODE>true</CODE> if and only if the certificate for
    polyhedron \p ph is strictly smaller than \p *this.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool is_stabilizing(const Polyhedron& ph) const;

  //! A total ordering on BHRZ03 certificates.
  /*! \ingroup PPL_CXX_interface
    This binary predicate defines a total ordering on BHRZ03 certificates
    which is used when storing information about sets of polyhedra.
  */
  struct Compare {
    //! Returns <CODE>true</CODE> if and only if \p x comes before \p y.
    bool operator()(const BHRZ03_Certificate& x,
		    const BHRZ03_Certificate& y) const;
  };

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! Check if gathered information is meaningful.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool OK() const;

private:
  //! Affine dimension of the polyhedron.
  dimension_type affine_dim;
  //! Dimension of the lineality space of the polyhedron.
  dimension_type lin_space_dim;
  //! Cardinality of a non-redundant constraint system for the polyhedron.
  dimension_type num_constraints;
  /*! \brief
    Number of non-redundant points in a generator system
    for the polyhedron.
  */
  dimension_type num_points;
  /*! \brief
    A vector containing, for each index `0 <= i < space_dim',
    the number of non-redundant rays in a generator system of the
    polyhedron having exactly `i' null coordinates.
  */
  std::vector<dimension_type> num_rays_null_coord;
};

#include "BHRZ03_Certificate.inlines.hh"

#endif // !defined(PPL_BHRZ03_Certificate_defs_hh)
