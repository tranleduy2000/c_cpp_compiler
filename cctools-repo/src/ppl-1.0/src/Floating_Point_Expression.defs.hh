/* Declarations for the Floating_Point_Expression class and its constituents.
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

#ifndef PPL_Floating_Point_Expression_defs_hh
#define PPL_Floating_Point_Expression_defs_hh 1

#include "globals.defs.hh"
#include "Floating_Point_Expression.types.hh"
#include "Linear_Form.types.hh"
#include "Box.types.hh"
#include <cmath>
#include <map>

namespace Parma_Polyhedra_Library {

/*! \brief
  \ingroup PPL_CXX_Interface
  A floating point expression on a given format.

  This class represents a concrete <EM>floating point expression</EM>. This
  includes constants, floating point variables, binary and unary
  arithmetic operators.

  \par Template type parameters

  - The class template type parameter \p FP_Interval_Type represents the type
  of the intervals used in the abstract domain. The interval bounds
  should have a floating point type.
  - The class template type parameter \p FP_Format represents the floating
  point format used in the concrete domain.
  This parameter must be a struct similar to the ones defined in file
  Float.defs.hh, even though it is sufficient to define the three
  fields BASE, MANTISSA_BITS and EXPONENT_BIAS.
*/
template <typename FP_Interval_Type, typename FP_Format>
class Floating_Point_Expression {

public:

  //! Alias for a linear form with template argument \p FP_Interval_Type.
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;

  //! Alias for a map that associates a variable index to an interval.
  /*! \brief
    Alias for a Box storing lower and upper bounds for floating point
    variables.

    The type a linear form abstract store associating each variable with an
    interval that correctly approximates its value.
  */
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;

  //! Alias for a map that associates a variable index to a linear form.
  /*!
    The type a linear form abstract store associating each variable with a
    linear form that correctly approximates its value.
  */
  typedef std::map<dimension_type, FP_Linear_Form>
          FP_Linear_Form_Abstract_Store;

  //! The floating point format used by the analyzer.
  typedef typename FP_Interval_Type::boundary_type boundary_type;

  //! The interval policy used by \p FP_Interval_Type.
  typedef typename FP_Interval_Type::info_type info_type;

  //! Destructor.
  virtual ~Floating_Point_Expression();

  //! Linearizes a floating point expression.
  /*! \brief
    Makes \p result become a linear form that correctly approximates the
    value of the floating point expression in the given composite
    abstract store.

    \param int_store The interval abstract store.
    \param lf_store The linear form abstract store.
    \param result Becomes the linearized expression.

    \return <CODE>true</CODE> if the linearization succeeded,
    <CODE>false</CODE> otherwise.

    Formally, if \p *this represents the expression \f$e\f$,
    \p int_store represents the interval abstract store \f$\rho^{\#}\f$ and
    \p lf_store represents the linear form abstract store \f$\rho^{\#}_l\f$,
    then \p result will become
    \f$\linexprenv{e}{\rho^{\#}}{\rho^{\#}_l}\f$
    if the linearization succeeds.

    All variables occurring in the floating point expression MUST have
    an associated interval in \p int_store.
    If this precondition is not met, calling the method causes an
    undefined behavior.
  */
  virtual bool linearize(const FP_Interval_Abstract_Store& int_store,
                         const FP_Linear_Form_Abstract_Store& lf_store,
                         FP_Linear_Form& result) const = 0;

  /*! \brief
    Absolute error.

    Represents the interval \f$[-\omega, \omega]\f$ where \f$\omega\f$ is the
    smallest non-zero positive number in the less precise floating point
    format between the analyzer format and the analyzed format.

  */
  static FP_Interval_Type absolute_error;

  // FIXME: this may not be the best place for them.
  /*! \brief
    Verifies if a given linear form overflows.
    \param lf The linear form to verify.
    \return
    Returns <CODE>false</CODE> if all coefficients in \p lf are bounded,
    <CODE>true</CODE> otherwise.
  */
  static bool overflows(const FP_Linear_Form& lf);

  /*! \brief
    Computes the relative error of a given linear form.

    Static helper method that is used by <CODE>linearize</CODE>
    to account for the relative errors on \p lf.
    \param lf The linear form used to compute the relative error.
    \param result Becomes the linear form corresponding to a relative
    error committed on \p lf.

    This method makes <CODE>result</CODE> become a linear form
    obtained by evaluating the function \f$\varepsilon_{\mathbf{f}}(l)\f$
    on the linear form \p lf. This function is defined
    such as:
    \f[
    \varepsilon_{\mathbf{f}}\left([a, b]+\sum_{v \in \cV}[a_{v}, b_{v}]v\right)
    \defeq
    (\textrm{max}(|a|, |b|) \amifp [-\beta^{-\textrm{p}}, \beta^{-\textrm{p}}])
    +
    \sum_{v \in \cV}(\textrm{max}(|a_{v}|,|b_{v}|)
    \amifp
    [-\beta^{-\textrm{p}}, \beta^{-\textrm{p}}])v
    \f]
    where p is the fraction size in bits for the format \f$\mathbf{f}\f$ and
    \f$\beta\f$ the base.
  */
  static void relative_error(const FP_Linear_Form& lf,
                             FP_Linear_Form& result);

  /*! \brief
    Makes \p result become an interval that overapproximates all the
    possible values of \p lf in the interval abstract store \p store.

    \param lf The linear form to aproximate.
    \param store The abstract store.
    \param result The linear form that will be modified.

    This method makes <CODE>result</CODE> become
    \f$\iota(lf)\rho^{\#}\f$, that is an interval defined as:
    \f[
    \iota\left(i + \sum_{v \in \cV}i_{v}v\right)\rho^{\#}
    \defeq
    i \asifp \left(\bigoplus_{v \in \cV}{}^{\#}i_{v} \amifp
    \rho^{\#}(v)\right)
    \f]
  */
  static void intervalize(const FP_Linear_Form& lf,
                          const FP_Interval_Abstract_Store& store,
                          FP_Interval_Type& result);

private:

  /*! \brief
    Computes the absolute error.

    Static helper method that is used to compute the value of the public
    static field <CODE>absolute_error</CODE>.

    \return The interval \f$[-\omega, \omega]\f$ corresponding to the value
    of <CODE>absolute_error</CODE>
  */
  static FP_Interval_Type compute_absolute_error();

}; // class Floating_Point_Expression


template <typename FP_Interval_Type, typename FP_Format>
FP_Interval_Type Floating_Point_Expression<FP_Interval_Type, FP_Format>
  ::absolute_error = compute_absolute_error();

} // namespace Parma_Polyhedra_Library

#include "Floating_Point_Expression.inlines.hh"
#include "Floating_Point_Expression.templates.hh"

#endif // !defined(PPL_Floating_Point_Expression_defs_hh)
