/* Declarations for the Opposite_Floating_Point_Expression class and
   its constituents.
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

#ifndef PPL_Opposite_Floating_Point_Expression_defs_hh
#define PPL_Opposite_Floating_Point_Expression_defs_hh 1

#include "Floating_Point_Expression.defs.hh"
#include "globals.defs.hh"
#include "Opposite_Floating_Point_Expression.types.hh"
#include <map>

namespace Parma_Polyhedra_Library {

//! Swaps \p x with \p y.
/*! \relates Opposite_Floating_Point_Expression */
template<typename FP_Interval_Type, typename FP_Format>
void swap(Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>& x,
          Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>& y);

/*! \brief
  A generic Opposite Floating Point Expression.

  \ingroup PPL_CXX_interface

  \par Template type parameters

  - The class template type parameter \p FP_Interval_Type represents the type
  of the intervals used in the abstract domain.
  - The class template type parameter \p FP_Format represents the floating
  point format used in the concrete domain.

  \par Linearization of opposite floating-point expressions

  Let \f$i + \sum_{v \in \cV}i_{v}v \f$ be an interval linear form and
  let \f$\adlf\f$ be a sound unary operator on linear forms such that:

  \f[
  \adlf
  \left(i + \sum_{v \in \cV}i_{v}v\right)
  =
  \left(\adifp i\right)
  + \sum_{v \in \cV}\left(\adifp i_{v} \right)v,
  \f]

  Given a floating point expression \f$\ominus e\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$, we construct the interval linear form
  \f$\linexprenv{\ominus e}{\rho^{\#}}{\rho^{\#}_l}\f$
  as follows:
  \f[
  \linexprenv{\ominus e}{\rho^{\#}}{\rho^{\#}_l}
  =
  \adlf
  \left(
  \linexprenv{e}{\rho^{\#}}{\rho^{\#}_l}
  \right).
  \f]
*/
template <typename FP_Interval_Type, typename FP_Format>
class Opposite_Floating_Point_Expression
  : public Floating_Point_Expression<FP_Interval_Type, FP_Format> {

public:

  /* \brief
     Alias for the Linear_Form<FP_Interval_Type> from
     Floating_Point_Expression
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::
  FP_Linear_Form FP_Linear_Form;

  /* \brief
     Alias for the std::map<dimension_type, FP_Interval_Type> from
     Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::
  FP_Interval_Abstract_Store FP_Interval_Abstract_Store;

  /* \brief
     Alias for the std::map<dimension_type, FP_Linear_Form> from
     Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::
  FP_Linear_Form_Abstract_Store FP_Linear_Form_Abstract_Store;

  /* \brief
     Alias for the FP_Interval_Type::boundary_type from
     Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::boundary_type
  boundary_type;

  /* \brief
     Alias for the FP_Interval_Type::info_type from Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::info_type info_type;

  //! \name Constructors and Destructor
  //@{
  /*! \brief
    Constructor with one parameter: builds the opposite floating point
    expression \f$\ominus\f$ \p op.
  */
  explicit Opposite_Floating_Point_Expression(
           Floating_Point_Expression<FP_Interval_Type, FP_Format>* const op);

  //! Destructor.
  ~Opposite_Floating_Point_Expression();

  //@} // Constructors and Destructor

  /*! \brief
    Linearizes the expression in a given astract store.

    Makes \p result become the linearization of \p *this in the given
    composite abstract store.

    \param int_store The interval abstract store.
    \param lf_store The linear form abstract store.
    \param result The modified linear form.

    \return <CODE>true</CODE> if the linearization succeeded,
    <CODE>false</CODE> otherwise.

    Note that all variables occuring in the expression represented
    by \p operand MUST have an associated value in \p int_store.
    If this precondition is not met, calling the method
    causes an undefined behavior.

    See the class description for a detailed explanation of how \p result
    is computed.
  */
  bool linearize(const FP_Interval_Abstract_Store& int_store,
                 const FP_Linear_Form_Abstract_Store& lf_store,
                 FP_Linear_Form& result) const;

  //! Swaps \p *this with \p y.
  void m_swap(Opposite_Floating_Point_Expression& y);

private:

  #ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Inhibited copy constructor.
  */
  #endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  Opposite_Floating_Point_Expression(
			  const Opposite_Floating_Point_Expression& y);

  #ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Inhibited assignment operator.
  */
  #endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  Opposite_Floating_Point_Expression& operator=(
			  const Opposite_Floating_Point_Expression& y);

  //! Pointer to the operand.
  Floating_Point_Expression<FP_Interval_Type, FP_Format>* operand;

}; // class Opposite_Floating_Point_Expression

} // namespace Parma_Polyhedra_Library

#include "Opposite_Floating_Point_Expression.inlines.hh"

#endif // !defined(PPL_Opposite_Floating_Point_Expression_defs_hh)
