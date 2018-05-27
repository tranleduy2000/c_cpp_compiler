/* Linear_Form class declaration.
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

#ifndef PPL_Linear_Form_defs_hh
#define PPL_Linear_Form_defs_hh 1

#include "Linear_Form.types.hh"
#include "Linear_Expression.types.hh"
#include "Variable.defs.hh"
#include "Box.types.hh"
#include "Float.defs.hh"
#include <vector>

namespace Parma_Polyhedra_Library {

//! Swaps \p x with \p y.
/*! \relates Linear_Form */
template <typename C>
void swap(Linear_Form<C>& x, Linear_Form<C>& y);

// Put them in the namespace here to declare them friend later.

//! Returns the linear form \p f1 + \p f2.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator+(const Linear_Form<C>& f1, const Linear_Form<C>& f2);

//! Returns the linear form \p v + \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator+(Variable v, const Linear_Form<C>& f);

//! Returns the linear form \p f + \p v.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator+(const Linear_Form<C>& f, Variable v);

//! Returns the linear form \p n + \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator+(const C& n, const Linear_Form<C>& f);

//! Returns the linear form \p f + \p n.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator+(const Linear_Form<C>& f, const C& n);

//! Returns the linear form \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator+(const Linear_Form<C>& f);

//! Returns the linear form - \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator-(const Linear_Form<C>& f);

//! Returns the linear form \p f1 - \p f2.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator-(const Linear_Form<C>& f1, const Linear_Form<C>& f2);

//! Returns the linear form \p v - \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator-(Variable v, const Linear_Form<C>& f);

//! Returns the linear form \p f - \p v.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator-(const Linear_Form<C>& f, Variable v);

//! Returns the linear form \p n - \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator-(const C& n, const Linear_Form<C>& f);

//! Returns the linear form \p f - \p n.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator-(const Linear_Form<C>& f, const C& n);

//! Returns the linear form \p n * \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator*(const C& n, const Linear_Form<C>& f);

//! Returns the linear form \p f * \p n.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>
operator*(const Linear_Form<C>& f, const C& n);

//! Returns the linear form \p f1 + \p f2 and assigns it to \p e1.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>&
operator+=(Linear_Form<C>& f1, const Linear_Form<C>& f2);

//! Returns the linear form \p f + \p v and assigns it to \p f.
/*! \relates Linear_Form
  \exception std::length_error
  Thrown if the space dimension of \p v exceeds
  <CODE>Linear_Form::max_space_dimension()</CODE>.
 */
template <typename C>
Linear_Form<C>&
operator+=(Linear_Form<C>& f, Variable v);

//! Returns the linear form \p f + \p n and assigns it to \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>&
operator+=(Linear_Form<C>& f, const C& n);

//! Returns the linear form \p f1 - \p f2 and assigns it to \p f1.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>&
operator-=(Linear_Form<C>& f1, const Linear_Form<C>& f2);

//! Returns the linear form \p f - \p v and assigns it to \p f.
/*! \relates Linear_Form
  \exception std::length_error
  Thrown if the space dimension of \p v exceeds
  <CODE>Linear_Form::max_space_dimension()</CODE>.
 */
template <typename C>
Linear_Form<C>&
operator-=(Linear_Form<C>& f, Variable v);

//! Returns the linear form \p f - \p n and assigns it to \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>&
operator-=(Linear_Form<C>& f, const C& n);

//! Returns the linear form \p n * \p f and assigns it to \p f.
/*! \relates Linear_Form */
template <typename C>
Linear_Form<C>&
operator*=(Linear_Form<C>& f, const C& n);

//! Returns the linear form \p f / \p n and assigns it to \p f.
/*!
   \relates Linear_Form
   Performs the division of a linear form by a scalar. It is up to the user to
   ensure that division by 0 is not performed.
*/
template <typename C>
Linear_Form<C>&
operator/=(Linear_Form<C>& f, const C& n);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are equal.
/*! \relates Linear_Form */
template <typename C>
bool
operator==(const Linear_Form<C>& x, const Linear_Form<C>& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates Linear_Form */
template <typename C>
bool
operator!=(const Linear_Form<C>& x, const Linear_Form<C>& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Linear_Form */
template <typename C>
std::ostream& operator<<(std::ostream& s, const Linear_Form<C>& f);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

//! A linear form with interval coefficients.
/*! \ingroup PPL_CXX_interface
  An object of the class Linear_Form represents the interval linear form
  \f[
    \sum_{i=0}^{n-1} a_i x_i + b
  \f]
  where \f$n\f$ is the dimension of the vector space,
  each \f$a_i\f$ is the coefficient
  of the \f$i\f$-th variable \f$x_i\f$
  and \f$b\f$ is the inhomogeneous term.
  The coefficients and the inhomogeneous term of the linear form
  have the template parameter \p C as their type. \p C must be the
  type of an Interval.

  \par How to build a linear form.
  A full set of functions is defined in order to provide a convenient
  interface for building complex linear forms starting from simpler ones
  and from objects of the classes Variable and \p C. Available operators
  include binary addition and subtraction, as well as multiplication and
  division by a coefficient.
  The space dimension of a linear form is defined as
  the highest variable dimension among variables that have a nonzero
  coefficient in the linear form, or zero if no such variable exists.
  The space dimension for each variable \f$x_i\f$ is given by \f$i + 1\f$.

  \par Example
  Given the type \p T of an Interval with floating point coefficients (though
  any integral type may also be used), the following code builds the interval
  linear form \f$lf = x_5 - x_2 + 1\f$ with space dimension 6:
  \code
  Variable x5(5);
  Variable x2(2);
  T x5_coefficient;
  x5_coefficient.lower() = 2.0;
  x5_coefficient.upper() = 3.0;
  T inhomogeneous_term;
  inhomogeneous_term.lower() = 4.0;
  inhomogeneous_term.upper() = 8.0;
  Linear_Form<T> lf(x2);
  lf = -lf;
  lf += Linear_Form<T>(x2);
  Linear_Form<T> lf_x5(x5);
  lf_x5 *= x5_coefficient;
  lf += lf_x5;
  \endcode
  Note that \c lf_x5 is created with space dimension 6, while \c lf is
  created with space dimension 0 and then extended first to space
  dimension 2 when \c x2 is subtracted and finally to space dimension
  6 when \c lf_x5 is added.
*/
template <typename C>
class Parma_Polyhedra_Library::Linear_Form {
public:
  //! Default constructor: returns a copy of Linear_Form::zero().
  Linear_Form();

  //! Ordinary copy constructor.
  Linear_Form(const Linear_Form& f);

  //! Destructor.
  ~Linear_Form();

  //! Builds the linear form corresponding to the inhomogeneous term \p n.
  explicit Linear_Form(const C& n);

  //! Builds the linear form corresponding to the variable \p v.
  /*!
    \exception std::length_error
    Thrown if the space dimension of \p v exceeds
    <CODE>Linear_Form::max_space_dimension()</CODE>.
  */
  Linear_Form(Variable v);

  //! Builds a linear form approximating the linear expression \p e.
  Linear_Form(const Linear_Expression& e);

  //! Returns the maximum space dimension a Linear_Form can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Returns the coefficient of \p v in \p *this.
  const C& coefficient(Variable v) const;

  //! Returns the inhomogeneous term of \p *this.
  const C& inhomogeneous_term() const;

  //! Negates all the coefficients of \p *this.
  void negate();

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  //! Swaps \p *this with \p y.
  void m_swap(Linear_Form& y);

  // Floating point analysis related methods.

  /*! \brief
    Verifies if the linear form overflows.

    \return
    Returns <CODE>false</CODE> if all coefficients in \p lf are bounded,
    <CODE>true</CODE> otherwise.

    \p T must be the type of possibly unbounded quantities.
  */
  bool overflows() const;

  /*! \brief
    Computes the relative error associated to floating point computations
    that operate on a quantity that is overapproximated by \p *this.

    \param analyzed_format The floating point format used by the analyzed
    program.
    \param result Becomes the linear form corresponding to the relative
    error committed.

    This method makes <CODE>result</CODE> become a linear form
    obtained by evaluating the function \f$\varepsilon_{\mathbf{f}}(l)\f$
    on the linear form. This function is defined as:
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

    The result is undefined if \p T is not the type of an interval with
    floating point boundaries.
  */
  void relative_error(Floating_Point_Format analyzed_format,
                      Linear_Form& result) const;

  /*! \brief
    Makes \p result become an interval that overapproximates all the
    possible values of \p *this.

    \param oracle The FP_Oracle to be queried.
    \param result The linear form that will store the result.

    \return <CODE>true</CODE> if the operation was successful,
    <CODE>false</CODE> otherwise (the possibility of failure
    depends on the oracle's implementation).

    \par Template type parameters

    - The class template parameter \p Target specifies the implementation
    of Concrete_Expression to be used.

    This method makes <CODE>result</CODE> become
    \f$\iota(lf)\rho^{\#}\f$, that is an interval defined as:
    \f[
    \iota\left(i + \sum_{v \in \cV}i_{v}v\right)\rho^{\#}
    \defeq
    i \asifp \left(\bigoplus_{v \in \cV}{}^{\#}i_{v} \amifp
    \rho^{\#}(v)\right)
    \f]
    where \f$\rho^{\#}(v)\f$ is an interval (provided by the oracle)
    that correctly approximates the value of \f$v\f$.

    The result is undefined if \p C is not the type of an interval with
    floating point boundaries.
  */
  template <typename Target>
  bool intervalize(const FP_Oracle<Target,C>& oracle, C& result) const;

private:
  //! The generic coefficient equal to the singleton zero.
  static C zero;

  //! Type of the container vector.
  typedef std::vector<C> vec_type;

  //! The container vector.
  vec_type vec;

  //! Implementation sizing constructor.
  /*!
    The bool parameter is just to avoid problems with
    the constructor Linear_Form(const C& n).
  */
  Linear_Form(dimension_type sz, bool);

  /*! \brief
    Builds the linear form corresponding to the difference of
    \p v and \p w.

    \exception std::length_error
    Thrown if the space dimension of \p v or the one of \p w exceed
    <CODE>Linear_Form::max_space_dimension()</CODE>.
  */
  Linear_Form(Variable v, Variable w);

  //! Gives the number of generic coefficients currently in use.
  dimension_type size() const;

  //! Extends the vector of \p *this to size \p sz.
  void extend(dimension_type sz);

  //! Returns a reference to \p vec[i].
  C& operator[](dimension_type i);

  //! Returns a const reference to \p vec[i].
  const C& operator[](dimension_type i) const;

  friend Linear_Form<C>
  operator+<C>(const Linear_Form<C>& f1, const Linear_Form<C>& f2);
  friend Linear_Form<C>
  operator+<C>(const C& n, const Linear_Form<C>& f);
  friend Linear_Form<C>
  operator+<C>(const Linear_Form<C>& f, const C& n);
  friend Linear_Form<C>
  operator+<C>(Variable v, const Linear_Form<C>& f);

  friend Linear_Form<C>
  operator-<C>(const Linear_Form<C>& f);

  friend Linear_Form<C>
  operator-<C>(const Linear_Form<C>& f1, const Linear_Form<C>& f2);
  friend Linear_Form<C>
  operator-<C>(const C& n, const Linear_Form<C>& f);
  friend Linear_Form<C>
  operator-<C>(const Linear_Form<C>& f, const C& n);
  friend Linear_Form<C>
  operator-<C>(Variable v, const Linear_Form<C>& f);
  friend Linear_Form<C>
  operator-<C>(const Linear_Form<C>& f, Variable v);

  friend Linear_Form<C>
  operator*<C>(const C& n, const Linear_Form<C>& f);
  friend Linear_Form<C>
  operator*<C>(const Linear_Form<C>& f, const C& n);

  friend Linear_Form<C>&
  operator+=<C>(Linear_Form<C>& f1, const Linear_Form<C>& f2);
  friend Linear_Form<C>&
  operator+=<C>(Linear_Form<C>& f, Variable v);
  friend Linear_Form<C>&
  operator+=<C>(Linear_Form<C>& f, const C& n);

  friend Linear_Form<C>&
  operator-=<C>(Linear_Form<C>& f1, const Linear_Form<C>& f2);
  friend Linear_Form<C>&
  operator-=<C>(Linear_Form<C>& f, Variable v);
  friend Linear_Form<C>&
  operator-=<C>(Linear_Form<C>& f, const C& n);

  friend Linear_Form<C>&
  operator*=<C>(Linear_Form<C>& f, const C& n);

  friend Linear_Form<C>&
  operator/=<C>(Linear_Form<C>& f, const C& n);

  friend bool
  operator==<C>(const Linear_Form<C>& x, const Linear_Form<C>& y);

  friend std::ostream&
  Parma_Polyhedra_Library::IO_Operators
  ::operator<<<C>(std::ostream& s, const Linear_Form<C>& f);
};

#include "Linear_Form.inlines.hh"
// Linear_Form.templates.hh is not included here on purpose.

#endif // !defined(PPL_Linear_Form_defs_hh)
