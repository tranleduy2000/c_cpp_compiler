/* Linearization function implementation.
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

#ifndef PPL_linearize_hh
#define PPL_linearize_hh 1

#include "Concrete_Expression.defs.hh"
#include "Float.defs.hh"
#include "Linear_Form.defs.hh"
#include "Box.defs.hh"
#include <map>

namespace Parma_Polyhedra_Library {

/*! \brief \relates Parma_Polyhedra_Library::Concrete_Expression
  Helper function used by <CODE>linearize</CODE> to linearize a
  sum of floating point expressions.

  Makes \p result become the linearization of \p *this in the given
  composite abstract store.

  \tparam Target
  A type template parameter specifying the instantiation of
  Concrete_Expression to be used.

  \tparam FP_Interval_Type
  A type template parameter for the intervals used in the abstract domain.
  The interval bounds should have a floating point type.

  \return
  <CODE>true</CODE> if the linearization succeeded,
  <CODE>false</CODE> otherwise.

  \param bop_expr
  The binary operator concrete expression to linearize.
  Its binary operator type must be <CODE>ADD</CODE>.

  \param oracle
  The FP_Oracle to be queried.

  \param lf_store
  The linear form abstract store.

  \param result
  The modified linear form.

  \par Linearization of sum floating-point expressions

  Let \f$i + \sum_{v \in \cV}i_{v}v \f$ and
  \f$i' + \sum_{v \in \cV}i'_{v}v \f$
  be two linear forms and \f$\aslf\f$ a sound abstract operator on linear
  forms such that:

  \f[
  \left(i + \sum_{v \in \cV}i_{v}v \right)
  \aslf
  \left(i' + \sum_{v \in \cV}i'_{v}v \right)
  =
  \left(i \asifp i'\right)
  + \sum_{v \in \cV}\left(i_{v} \asifp i'_{v} \right)v.
  \f]

  Given an expression \f$e_{1} \oplus e_{2}\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$, we construct the interval linear form
  \f$\linexprenv{e_{1} \oplus e_{2}}{\rho^{\#}}{\rho^{\#}_l}\f$
  as follows:
  \f[
  \linexprenv{e_{1} \oplus e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  =
  \linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \aslf
  \linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  \aslf
  \varepsilon_{\mathbf{f}}\left(\linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \right)
  \aslf
  \varepsilon_{\mathbf{f}}\left(\linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  \right)
  \aslf
  mf_{\mathbf{f}}[-1, 1]
  \f]
  where \f$\varepsilon_{\mathbf{f}}(l)\f$ is the relative error
  associated to \f$l\f$ (see method <CODE>relative_error</CODE> of
  class Linear_Form) and \f$mf_{\mathbf{f}}\f$ is a rounding
  error computed by function <CODE>compute_absolute_error</CODE>.
*/
template <typename Target, typename FP_Interval_Type>
static bool
add_linearize(const Binary_Operator<Target>& bop_expr,
              const FP_Oracle<Target,FP_Interval_Type>& oracle,
              const std::map<dimension_type, Linear_Form<FP_Interval_Type> >& lf_store,
              Linear_Form<FP_Interval_Type>& result) {
  PPL_ASSERT(bop_expr.binary_operator() == Binary_Operator<Target>::ADD);

  typedef typename FP_Interval_Type::boundary_type analyzer_format;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;
  typedef std::map<dimension_type, FP_Linear_Form> FP_Linear_Form_Abstract_Store;

  if (!linearize(*(bop_expr.left_hand_side()), oracle, lf_store, result))
    return false;

  Floating_Point_Format analyzed_format =
    bop_expr.type().floating_point_format();
  FP_Linear_Form rel_error;
  result.relative_error(analyzed_format, rel_error);
  result += rel_error;
  FP_Linear_Form linearized_second_operand;
  if (!linearize(*(bop_expr.right_hand_side()), oracle, lf_store,
                 linearized_second_operand))
    return false;

  result += linearized_second_operand;
  linearized_second_operand.relative_error(analyzed_format, rel_error);
  result += rel_error;
  FP_Interval_Type absolute_error =
                   compute_absolute_error<FP_Interval_Type>(analyzed_format);
  result += absolute_error;
  return !result.overflows();
}

/*! \brief \relates Parma_Polyhedra_Library::Concrete_Expression
  Helper function used by <CODE>linearize</CODE> to linearize a
  difference of floating point expressions.

  Makes \p result become the linearization of \p *this in the given
  composite abstract store.

  \tparam Target
  A type template parameter specifying the instantiation of
  Concrete_Expression to be used.

  \tparam FP_Interval_Type
  A type template parameter for the intervals used in the abstract domain.
  The interval bounds should have a floating point type.

  \return
  <CODE>true</CODE> if the linearization succeeded,
  <CODE>false</CODE> otherwise.

  \param bop_expr
  The binary operator concrete expression to linearize.
  Its binary operator type must be <CODE>SUB</CODE>.

  \param oracle
  The FP_Oracle to be queried.

  \param lf_store
  The linear form abstract store.

  \param result
  The modified linear form.

  \par Linearization of difference floating-point expressions

  Let \f$i + \sum_{v \in \cV}i_{v}v \f$ and
  \f$i' + \sum_{v \in \cV}i'_{v}v \f$
  be two linear forms, \f$\aslf\f$ and \f$\adlf\f$ two sound abstract
  operators on linear form such that:
  \f[
  \left(i + \sum_{v \in \cV}i_{v}v\right)
  \aslf
  \left(i' + \sum_{v \in \cV}i'_{v}v\right)
  =
  \left(i \asifp i'\right)
  + \sum_{v \in \cV}\left(i_{v} \asifp i'_{v}\right)v,
  \f]
  \f[
  \left(i + \sum_{v \in \cV}i_{v}v\right)
  \adlf
  \left(i' + \sum_{v \in \cV}i'_{v}v\right)
  =
  \left(i \adifp i'\right)
  + \sum_{v \in \cV}\left(i_{v} \adifp i'_{v}\right)v.
  \f]
  Given an expression \f$e_{1} \ominus e_{2}\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$,  we construct the interval linear form
  \f$\linexprenv{e_{1} \ominus e_{2}}{\rho^{\#}}{\rho^{\#}_l}\f$
  on \f$\cV\f$ as follows:
  \f[
  \linexprenv{e_{1} \ominus e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  =
  \linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \adlf
  \linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  \aslf
  \varepsilon_{\mathbf{f}}\left(\linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \right)
  \aslf
  \varepsilon_{\mathbf{f}}\left(\linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  \right)
  \aslf
  mf_{\mathbf{f}}[-1, 1]
  \f]
  where \f$\varepsilon_{\mathbf{f}}(l)\f$ is the relative error
  associated to \f$l\f$ (see method <CODE>relative_error</CODE> of
  class Linear_Form) and \f$mf_{\mathbf{f}}\f$ is a rounding
  error computed by function <CODE>compute_absolute_error</CODE>.
*/
template <typename Target, typename FP_Interval_Type>
static bool
sub_linearize(const Binary_Operator<Target>& bop_expr,
              const FP_Oracle<Target,FP_Interval_Type>& oracle,
              const std::map<dimension_type, Linear_Form<FP_Interval_Type> >& lf_store,
              Linear_Form<FP_Interval_Type>& result) {
  PPL_ASSERT(bop_expr.binary_operator() == Binary_Operator<Target>::SUB);

  typedef typename FP_Interval_Type::boundary_type analyzer_format;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;
  typedef std::map<dimension_type, FP_Linear_Form> FP_Linear_Form_Abstract_Store;

  if (!linearize(*(bop_expr.left_hand_side()), oracle, lf_store, result))
    return false;

  Floating_Point_Format analyzed_format =
    bop_expr.type().floating_point_format();
  FP_Linear_Form rel_error;
  result.relative_error(analyzed_format, rel_error);
  result += rel_error;
  FP_Linear_Form linearized_second_operand;
  if (!linearize(*(bop_expr.right_hand_side()), oracle, lf_store,
                 linearized_second_operand))
    return false;

  result -= linearized_second_operand;
  linearized_second_operand.relative_error(analyzed_format, rel_error);
  result += rel_error;
  FP_Interval_Type absolute_error =
                   compute_absolute_error<FP_Interval_Type>(analyzed_format);
  result += absolute_error;
  return !result.overflows();
}

/*! \brief \relates Parma_Polyhedra_Library::Concrete_Expression
  Helper function used by <CODE>linearize</CODE> to linearize a
  product of floating point expressions.

  Makes \p result become the linearization of \p *this in the given
  composite abstract store.

  \tparam Target
  A type template parameter specifying the instantiation of
  Concrete_Expression to be used.

  \tparam FP_Interval_Type
  A type template parameter for the intervals used in the abstract domain.
  The interval bounds should have a floating point type.

  \return
  <CODE>true</CODE> if the linearization succeeded,
  <CODE>false</CODE> otherwise.

  \param bop_expr
  The binary operator concrete expression to linearize.
  Its binary operator type must be <CODE>MUL</CODE>.

  \param oracle
  The FP_Oracle to be queried.

  \param lf_store
  The linear form abstract store.

  \param result
  The modified linear form.

  \par Linearization of multiplication floating-point expressions

  Let \f$i + \sum_{v \in \cV}i_{v}v \f$ and
  \f$i' + \sum_{v \in \cV}i'_{v}v \f$
  be two linear forms, \f$\aslf\f$ and \f$\amlf\f$ two sound abstract
  operators on linear forms such that:
  \f[
  \left(i + \sum_{v \in \cV}i_{v}v\right)
  \aslf
  \left(i' + \sum_{v \in \cV}i'_{v}v\right)
  =
  \left(i \asifp i'\right)
  + \sum_{v \in \cV}\left(i_{v} \asifp i'_{v}\right)v,
  \f]
  \f[
  i
  \amlf
  \left(i' + \sum_{v \in \cV}i'_{v}v\right)
  =
  \left(i \amifp i'\right)
  + \sum_{v \in \cV}\left(i \amifp i'_{v}\right)v.
  \f]
  Given an expression \f$[a, b] \otimes e_{2}\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$, we construct the interval linear form
  \f$\linexprenv{[a, b] \otimes e_{2}}{\rho^{\#}}{\rho^{\#}_l}\f$
  as follows:
  \f[
  \linexprenv{[a, b] \otimes e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  =
  \left([a, b]
  \amlf
  \linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}\right)
  \aslf
  \left([a, b]
  \amlf
  \varepsilon_{\mathbf{f}}\left(\linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  \right)\right)
  \aslf
  mf_{\mathbf{f}}[-1, 1].
  \f].

  Given an expression \f$e_{1} \otimes [a, b]\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$, we construct the interval linear form
  \f$\linexprenv{e_{1} \otimes [a, b]}{\rho^{\#}}{\rho^{\#}_l}\f$
  as follows:
  \f[
  \linexprenv{e_{1} \otimes [a, b]}{\rho^{\#}}{\rho^{\#}_l}
  =
  \linexprenv{[a, b] \otimes e_{1}}{\rho^{\#}}{\rho^{\#}_l}.
  \f]

  Given an expression \f$e_{1} \otimes e_{2}\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$, we construct the interval linear form
  \f$\linexprenv{e_{1} \otimes e_{2}}{\rho^{\#}}{\rho^{\#}_l}\f$
  as follows:
  \f[
  \linexprenv{e_{1} \otimes e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  =
  \linexprenv{\iota\left(\linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \right)\rho^{\#}
  \otimes e_{2}}{\rho^{\#}}{\rho^{\#}_l},
  \f]
  where \f$\varepsilon_{\mathbf{f}}(l)\f$ is the relative error
  associated to \f$l\f$ (see method <CODE>relative_error</CODE> of
  class Linear_Form), \f$\iota(l)\rho^{\#}\f$ is the intervalization
  of \f$l\f$ (see method <CODE>intervalize</CODE> of class Linear_Form),
  and \f$mf_{\mathbf{f}}\f$ is a rounding error computed by function
  <CODE>compute_absolute_error</CODE>.

  Even though we intervalize the first operand in the above example, the
  actual implementation utilizes an heuristics for choosing which of the two
  operands must be intervalized in order to obtain the most precise result.
*/
template <typename Target, typename FP_Interval_Type>
static bool
mul_linearize(const Binary_Operator<Target>& bop_expr,
              const FP_Oracle<Target,FP_Interval_Type>& oracle,
              const std::map<dimension_type, Linear_Form<FP_Interval_Type> >& lf_store,
              Linear_Form<FP_Interval_Type>& result) {
  PPL_ASSERT(bop_expr.binary_operator() == Binary_Operator<Target>::MUL);

  typedef typename FP_Interval_Type::boundary_type analyzer_format;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;
  typedef std::map<dimension_type, FP_Linear_Form> FP_Linear_Form_Abstract_Store;

  /*
    FIXME: We currently adopt the "Interval-Size Local" strategy in order to
    decide which of the two linear forms must be intervalized, as described
    in Section 6.2.4 ("Multiplication Strategies") of Antoine Mine's Ph.D.
    thesis "Weakly Relational Numerical Abstract Domains".
    In this Section are also described other multiplication strategies, such
    as All-Cases, Relative-Size Local, Simplification-Driven Global and
    Homogeneity Global.
  */

  // Here we choose which of the two linear forms must be intervalized.

  // true if we intervalize the first form, false if we intervalize the second.
  bool intervalize_first;
  FP_Linear_Form linearized_first_operand;
  if (!linearize(*(bop_expr.left_hand_side()), oracle, lf_store,
                 linearized_first_operand))
    return false;
  FP_Interval_Type intervalized_first_operand;
  if (!linearized_first_operand.intervalize(oracle, intervalized_first_operand))
    return false;
  FP_Linear_Form linearized_second_operand;
  if (!linearize(*(bop_expr.right_hand_side()), oracle, lf_store,
                 linearized_second_operand))
    return false;
  FP_Interval_Type intervalized_second_operand;
  if (!linearized_second_operand.intervalize(oracle,
                                             intervalized_second_operand))
    return false;

  // FIXME: we are not sure that what we do here is policy-proof.
  if (intervalized_first_operand.is_bounded()) {
    if (intervalized_second_operand.is_bounded()) {
      analyzer_format first_interval_size
        = intervalized_first_operand.upper()
        - intervalized_first_operand.lower();
      analyzer_format second_interval_size
        = intervalized_second_operand.upper()
        - intervalized_second_operand.lower();
      if (first_interval_size <= second_interval_size)
        intervalize_first = true;
      else
        intervalize_first = false;
    }
    else
      intervalize_first = true;
  }
  else {
    if (intervalized_second_operand.is_bounded())
      intervalize_first = false;
    else
      return false;
  }

  // Here we do the actual computation.
  // For optimizing, we store the relative error directly into result.
  Floating_Point_Format analyzed_format =
    bop_expr.type().floating_point_format();
  if (intervalize_first) {
    linearized_second_operand.relative_error(analyzed_format, result);
    linearized_second_operand *= intervalized_first_operand;
    result *= intervalized_first_operand;
    result += linearized_second_operand;
  }
  else {
    linearized_first_operand.relative_error(analyzed_format, result);
    linearized_first_operand *= intervalized_second_operand;
    result *= intervalized_second_operand;
    result += linearized_first_operand;
  }

  FP_Interval_Type absolute_error =
                   compute_absolute_error<FP_Interval_Type>(analyzed_format);
  result += absolute_error;
  return !result.overflows();
}

/*! \brief \relates Parma_Polyhedra_Library::Concrete_Expression
  Helper function used by <CODE>linearize</CODE> to linearize a
  division of floating point expressions.

  Makes \p result become the linearization of \p *this in the given
  composite abstract store.

  \tparam Target
  A type template parameter specifying the instantiation of
  Concrete_Expression to be used.

  \tparam FP_Interval_Type
  A type template parameter for the intervals used in the abstract domain.
  The interval bounds should have a floating point type.

  \return
  <CODE>true</CODE> if the linearization succeeded,
  <CODE>false</CODE> otherwise.

  \param bop_expr
  The binary operator concrete expression to linearize.
  Its binary operator type must be <CODE>DIV</CODE>.

  \param oracle
  The FP_Oracle to be queried.

  \param lf_store
  The linear form abstract store.

  \param result
  The modified linear form.

  \par Linearization of division floating-point expressions

  Let \f$i + \sum_{v \in \cV}i_{v}v \f$ and
  \f$i' + \sum_{v \in \cV}i'_{v}v \f$
  be two linear forms, \f$\aslf\f$ and \f$\adivlf\f$ two sound abstract
  operator on linear forms such that:
  \f[
  \left(i + \sum_{v \in \cV}i_{v}v\right)
  \aslf
  \left(i' + \sum_{v \in \cV}i'_{v}v\right)
  =
  \left(i \asifp i'\right)
  + \sum_{v \in \cV}\left(i_{v} \asifp i'_{v}\right)v,
  \f]
  \f[
  \left(i + \sum_{v \in \cV}i_{v}v\right)
  \adivlf
  i'
  =
  \left(i \adivifp i'\right)
  + \sum_{v \in \cV}\left(i_{v} \adivifp i'\right)v.
  \f]
  Given an expression \f$e_{1} \oslash [a, b]\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$,
  we construct the interval linear form
  \f$
  \linexprenv{e_{1} \oslash [a, b]}{\rho^{\#}}{\rho^{\#}_l}
  \f$
  as follows:
  \f[
  \linexprenv{e_{1} \oslash [a, b]}{\rho^{\#}}{\rho^{\#}_l}
  =
  \left(\linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \adivlf
  [a, b]\right)
  \aslf
  \left(\varepsilon_{\mathbf{f}}\left(
  \linexprenv{e_{1}}{\rho^{\#}}{\rho^{\#}_l}
  \right)
  \adivlf
  [a, b]\right)
  \aslf
  mf_{\mathbf{f}}[-1, 1],
  \f]
  given an expression \f$e_{1} \oslash e_{2}\f$ and a composite
  abstract store \f$\left \llbracket \rho^{\#}, \rho^{\#}_l \right
  \rrbracket\f$, we construct the interval linear form
  \f$\linexprenv{e_{1} \oslash e_{2}}{\rho^{\#}}{\rho^{\#}_l}\f$
  as follows:
  \f[
  \linexprenv{e_{1} \oslash e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  =
  \linexprenv{e_{1} \oslash \iota\left(
  \linexprenv{e_{2}}{\rho^{\#}}{\rho^{\#}_l}
  \right)\rho^{\#}}{\rho^{\#}}{\rho^{\#}_l},
  \f]
  where \f$\varepsilon_{\mathbf{f}}(l)\f$ is the relative error
  associated to \f$l\f$ (see method <CODE>relative_error</CODE> of
  class Linear_Form), \f$\iota(l)\rho^{\#}\f$ is the intervalization
  of \f$l\f$ (see method <CODE>intervalize</CODE> of class Linear_Form),
  and \f$mf_{\mathbf{f}}\f$ is a rounding error computed by function
  <CODE>compute_absolute_error</CODE>.
*/
template <typename Target, typename FP_Interval_Type>
static bool
div_linearize(const Binary_Operator<Target>& bop_expr,
              const FP_Oracle<Target,FP_Interval_Type>& oracle,
              const std::map<dimension_type, Linear_Form<FP_Interval_Type> >& lf_store,
              Linear_Form<FP_Interval_Type>& result) {
  PPL_ASSERT(bop_expr.binary_operator() == Binary_Operator<Target>::DIV);

  typedef typename FP_Interval_Type::boundary_type analyzer_format;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;
  typedef std::map<dimension_type, FP_Linear_Form> FP_Linear_Form_Abstract_Store;

  FP_Linear_Form linearized_second_operand;
  if (!linearize(*(bop_expr.right_hand_side()), oracle, lf_store,
                 linearized_second_operand))
    return false;
  FP_Interval_Type intervalized_second_operand;
  if (!linearized_second_operand.intervalize(oracle,
                                             intervalized_second_operand))
    return false;

  // Check if we may divide by zero.
  if ((intervalized_second_operand.lower_is_boundary_infinity() ||
       intervalized_second_operand.lower() <= 0) &&
      (intervalized_second_operand.upper_is_boundary_infinity() ||
       intervalized_second_operand.upper() >= 0))
    return false;

  if (!linearize(*(bop_expr.left_hand_side()), oracle, lf_store, result))
    return false;

  Floating_Point_Format analyzed_format =
    bop_expr.type().floating_point_format();
  FP_Linear_Form rel_error;
  result.relative_error(analyzed_format, rel_error);
  result /= intervalized_second_operand;
  rel_error /= intervalized_second_operand;
  result += rel_error;
  FP_Interval_Type absolute_error =
                   compute_absolute_error<FP_Interval_Type>(analyzed_format);
  result += absolute_error;
  return !result.overflows();
}

/*! \brief \relates Parma_Polyhedra_Library::Concrete_Expression
  Helper function used by <CODE>linearize</CODE> to linearize a cast
  floating point expression.

  Makes \p result become the linearization of \p *this in the given
  composite abstract store.

  \tparam Target
  A type template parameter specifying the instantiation of
  Concrete_Expression to be used.

  \tparam FP_Interval_Type
  A type template parameter for the intervals used in the abstract domain.
  The interval bounds should have a floating point type.

  \return
  <CODE>true</CODE> if the linearization succeeded,
  <CODE>false</CODE> otherwise.

  \param cast_expr
  The cast operator concrete expression to linearize.

  \param oracle
  The FP_Oracle to be queried.

  \param lf_store
  The linear form abstract store.

  \param result
  The modified linear form.
*/
template <typename Target, typename FP_Interval_Type>
static bool
cast_linearize(const Cast_Operator<Target>& cast_expr,
               const FP_Oracle<Target,FP_Interval_Type>& oracle,
               const std::map<dimension_type, Linear_Form<FP_Interval_Type> >& lf_store,
               Linear_Form<FP_Interval_Type>& result) {
  typedef typename FP_Interval_Type::boundary_type analyzer_format;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;
  typedef std::map<dimension_type, FP_Linear_Form> FP_Linear_Form_Abstract_Store;

  Floating_Point_Format analyzed_format =
    cast_expr.type().floating_point_format();
  const Concrete_Expression<Target>* cast_arg = cast_expr.argument();
  if (cast_arg->type().is_floating_point()) {
    if (!linearize(*cast_arg, oracle, lf_store, result))
      return false;
    if (!is_less_precise_than(analyzed_format,
                              cast_arg->type().floating_point_format()) ||
        result == FP_Linear_Form(FP_Interval_Type(0)) ||
        result == FP_Linear_Form(FP_Interval_Type(1)))
      /*
        FIXME: find a general way to check if the casted constant
        is exactly representable in the less precise format.
      */
      /*
        We are casting to a more precise format or casting
        a definitely safe value: do not add errors.
      */
      return true;
  }
  else {
    FP_Interval_Type expr_value;
    if (!oracle.get_integer_expr_value(*cast_arg, expr_value))
      return false;
    result = FP_Linear_Form(expr_value);
    if (is_less_precise_than(Float<analyzer_format>::Binary::floating_point_format, analyzed_format) ||
        result == FP_Linear_Form(FP_Interval_Type(0)) ||
        result == FP_Linear_Form(FP_Interval_Type(1)))
      /*
        FIXME: find a general way to check if the casted constant
        is exactly representable in the less precise format.
      */
      /*
        We are casting to a more precise format or casting
        a definitely safe value: do not add errors.
      */
      return true;
  }

  FP_Linear_Form rel_error;
  result.relative_error(analyzed_format, rel_error);
  result += rel_error;
  FP_Interval_Type absolute_error =
                   compute_absolute_error<FP_Interval_Type>(analyzed_format);
  result += absolute_error;
  return !result.overflows();
}

//! Linearizes a floating point expression.
/*! \relates Parma_Polyhedra_Library::Concrete_Expression
  Makes \p result become a linear form that correctly approximates the
  value of \p expr in the given composite abstract store.

  \tparam Target
  A type template parameter specifying the instantiation of
  Concrete_Expression to be used.

  \tparam FP_Interval_Type
  A type template parameter for the intervals used in the abstract domain.
  The interval bounds should have a floating point type.

  \return
  <CODE>true</CODE> if the linearization succeeded,
  <CODE>false</CODE> otherwise.

  \param expr
  The concrete expression to linearize.

  \param oracle
  The FP_Oracle to be queried.

  \param lf_store
  The linear form abstract store.

  \param result
  Becomes the linearized expression.

  Formally, if \p expr represents the expression \f$e\f$ and
  \p lf_store represents the linear form abstract store \f$\rho^{\#}_l\f$,
  then \p result will become \f$\linexprenv{e}{\rho^{\#}}{\rho^{\#}_l}\f$
  if the linearization succeeds.
*/
template <typename Target, typename FP_Interval_Type>
bool
linearize(const Concrete_Expression<Target>& expr,
          const FP_Oracle<Target,FP_Interval_Type>& oracle,
          const std::map<dimension_type, Linear_Form<FP_Interval_Type> >& lf_store,
          Linear_Form<FP_Interval_Type>& result) {
  typedef typename FP_Interval_Type::boundary_type analyzer_format;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef Box<FP_Interval_Type> FP_Interval_Abstract_Store;
  typedef std::map<dimension_type, FP_Linear_Form> FP_Linear_Form_Abstract_Store;

  PPL_ASSERT(expr.type().is_floating_point());
  // Check that analyzer_format is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<analyzer_format>::is_exact,
      "linearize<Target, FP_Interval_Type>:"
      " FP_Interval_Type is not the type of an interval with floating point boundaries.");

  switch(expr.kind()) {
  case Integer_Constant<Target>::KIND:
    PPL_UNREACHABLE;
    break;
  case Floating_Point_Constant<Target>::KIND:
  {
    const Floating_Point_Constant<Target>* fpc_expr =
      expr.template as<Floating_Point_Constant>();
    FP_Interval_Type constant_value;
    if (!oracle.get_fp_constant_value(*fpc_expr, constant_value))
      return false;
    result = FP_Linear_Form(constant_value);
    return true;
  }
  case Unary_Operator<Target>::KIND:
  {
    const Unary_Operator<Target>* uop_expr =
      expr.template as<Unary_Operator>();
    switch (uop_expr->unary_operator()) {
    case Unary_Operator<Target>::UPLUS:
      return linearize(*(uop_expr->argument()), oracle, lf_store, result);
    case Unary_Operator<Target>::UMINUS:
      if (!linearize(*(uop_expr->argument()), oracle, lf_store, result))
        return false;

      result.negate();
      return true;
    case Unary_Operator<Target>::BNOT:
      throw std::runtime_error("PPL internal error: unimplemented");
      break;
    default:
      PPL_UNREACHABLE;
      break;
    }
    break;
  }
  case Binary_Operator<Target>::KIND:
  {
    const Binary_Operator<Target>* bop_expr =
      expr.template as<Binary_Operator>();
    switch (bop_expr->binary_operator()) {
    case Binary_Operator<Target>::ADD:
      return add_linearize(*bop_expr, oracle, lf_store, result);
    case Binary_Operator<Target>::SUB:
      return sub_linearize(*bop_expr, oracle, lf_store, result);
    case Binary_Operator<Target>::MUL:
      return mul_linearize(*bop_expr, oracle, lf_store, result);
    case Binary_Operator<Target>::DIV:
      return div_linearize(*bop_expr, oracle, lf_store, result);
    case Binary_Operator<Target>::REM:
    case Binary_Operator<Target>::BAND:
    case Binary_Operator<Target>::BOR:
    case Binary_Operator<Target>::BXOR:
    case Binary_Operator<Target>::LSHIFT:
    case Binary_Operator<Target>::RSHIFT:
      // FIXME: can we do better?
      return false;
    default:
      PPL_UNREACHABLE;
      return false;
    }
    break;
  }
  case Approximable_Reference<Target>::KIND:
  {
    const Approximable_Reference<Target>* ref_expr =
      expr.template as<Approximable_Reference>();
    std::set<dimension_type> associated_dimensions;
    if (!oracle.get_associated_dimensions(*ref_expr, associated_dimensions)
        || associated_dimensions.empty())
      /*
        We were unable to find any associated space dimension:
        linearization fails.
      */
      return false;

    if (associated_dimensions.size() == 1) {
      /* If a linear form associated to the only referenced
         space dimension exists in lf_store, return that form.
         Otherwise, return the simplest linear form. */
      dimension_type variable_index = *associated_dimensions.begin();
      PPL_ASSERT(variable_index != not_a_dimension());

      typename FP_Linear_Form_Abstract_Store::const_iterator
               variable_value = lf_store.find(variable_index);
      if (variable_value == lf_store.end()) {
        result = FP_Linear_Form(Variable(variable_index));
        return true;
      }

      result = FP_Linear_Form(variable_value->second);
      /* FIXME: do we really need to contemplate the possibility
	 that an unbounded linear form was saved into lf_store? */
      return !result.overflows();
    }

    /*
      Here associated_dimensions.size() > 1. Try to return the LUB
      of all intervals associated to each space dimension.
    */
    PPL_ASSERT(associated_dimensions.size() > 1);
    std::set<dimension_type>::const_iterator i = associated_dimensions.begin();
    std::set<dimension_type>::const_iterator i_end =
      associated_dimensions.end();
    FP_Interval_Type lub(EMPTY);
    for (; i != i_end; ++i) {
      FP_Interval_Type curr_int;
      PPL_ASSERT(*i != not_a_dimension());
      if (!oracle.get_interval(*i, curr_int))
        return false;

      lub.join_assign(curr_int);
    }

    result = FP_Linear_Form(lub);
    return !result.overflows();
  }
  case Cast_Operator<Target>::KIND:
  {
    const Cast_Operator<Target>* cast_expr =
      expr.template as<Cast_Operator>();
    return cast_linearize(*cast_expr, oracle, lf_store, result);
  }
  default:
    PPL_UNREACHABLE;
    break;
  }

  PPL_UNREACHABLE;
  return false;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_linearize_hh)
