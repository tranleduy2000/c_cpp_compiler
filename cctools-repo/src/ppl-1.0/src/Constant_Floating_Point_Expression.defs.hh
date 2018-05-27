/* Declarations for the Constant_Floating_Point_Expression class and
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

#ifndef PPL_Constant_Floating_Point_Expression_defs_hh
#define PPL_Constant_Floating_Point_Expression_defs_hh 1

#include "Floating_Point_Expression.defs.hh"
#include "globals.defs.hh"
#include "Constant_Floating_Point_Expression.types.hh"
#include <map>

namespace Parma_Polyhedra_Library {

//! Swaps \p x with \p y.
/*! \relates Constant_Floating_Point_Expression */
template<typename FP_Interval_Type, typename FP_Format>
void swap(Constant_Floating_Point_Expression<FP_Interval_Type, FP_Format>& x,
          Constant_Floating_Point_Expression<FP_Interval_Type, FP_Format>& y);

/*! \brief
  A generic Constant Floating Point Expression.

  \ingroup PPL_CXX_interface

  \par Template type parameters

  - The class template type parameter \p FP_Interval_Type represents the type
  of the intervals used in the abstract domain.
  - The class template type parameter \p FP_Format represents the floating
  point format used in the concrete domain.

  \par Linearization of floating-point constant expressions

  The linearization of a constant floating point expression results in a
  linear form consisting of only the inhomogeneous term
  \f$[l, u]\f$, where \f$l\f$ and \f$u\f$ are the lower
  and upper bounds of the constant value given to the class constructor.
*/
template <typename FP_Interval_Type, typename FP_Format>
class Constant_Floating_Point_Expression
  : public Floating_Point_Expression<FP_Interval_Type, FP_Format> {

public:

  /*! \brief
     Alias for the Linear_Form<FP_Interval_Type> from
     Floating_Point_Expression
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::
  FP_Linear_Form FP_Linear_Form;

  /*! \brief
     Alias for the Box<FP_Interval_Type> from
     Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::
  FP_Interval_Abstract_Store FP_Interval_Abstract_Store;

  /*! \brief
     Alias for the std::map<dimension_type, FP_Linear_Form> from
     Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::
  FP_Linear_Form_Abstract_Store FP_Linear_Form_Abstract_Store;

  /*! \brief
     Alias for the FP_Interval_Type::boundary_type from
     Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::boundary_type
  boundary_type;

  /*! \brief
     Alias for the FP_Interval_Type::info_type from Floating_Point_Expression.
  */
  typedef typename
  Floating_Point_Expression<FP_Interval_Type, FP_Format>::info_type info_type;

  //! \name Constructors and Destructor
  //@{
  /*! \brief
    Constructor with two parameters: builds the constant floating point
    expression from a \p lower_bound and an \p upper_bound of its
    value in the concrete domain.
  */
  Constant_Floating_Point_Expression(const boundary_type lower_bound,
                                     const boundary_type upper_bound);

  /*! \brief
    Builds a constant floating point expression with the value
    expressed by the string \p str_value.
  */
  Constant_Floating_Point_Expression(const char* str_value);

  //! Destructor.
  ~Constant_Floating_Point_Expression();

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

    See the class description for an explanation of how \p result is computed.
  */
  bool linearize(const FP_Interval_Abstract_Store& int_store,
                 const FP_Linear_Form_Abstract_Store& lf_store,
                 FP_Linear_Form& result) const;

  //! Swaps \p *this with \p y.
  void m_swap(Constant_Floating_Point_Expression& y);

private:

  FP_Interval_Type value;

  #ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Inhibited copy constructor.
  */
  #endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  Constant_Floating_Point_Expression(
			  const Constant_Floating_Point_Expression& y);

  #ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Inhibited assignment operator.
  */
  #endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAIL
  Constant_Floating_Point_Expression& operator=(
		          const Constant_Floating_Point_Expression& y);

}; // class Constant_Floating_Point_Expression

} // namespace Parma_Polyhedra_Library

#include "Constant_Floating_Point_Expression.inlines.hh"

#endif // !defined(PPL_Constant_Floating_Point_Expression_defs_hh)
