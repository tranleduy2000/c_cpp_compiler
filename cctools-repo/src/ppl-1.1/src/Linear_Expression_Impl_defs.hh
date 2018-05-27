/* Linear_Expression_Impl class declaration.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Linear_Expression_Impl_defs_hh
#define PPL_Linear_Expression_Impl_defs_hh 1

#include "Linear_Expression_Impl_types.hh"
#include "Coefficient_defs.hh"
#include "Variable_defs.hh"
#include "Variables_Set_defs.hh"
#include "Dense_Row_defs.hh"
#include "Sparse_Row_defs.hh"
#include <cstddef>
#include "Linear_Expression_Interface_defs.hh"

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Output operator.
/*! \relates Parma_Polyhedra_Library::Linear_Expression_Impl */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
std::ostream&
operator<<(std::ostream& s, const Linear_Expression_Impl<Row>& e);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A linear expression.
/*! \ingroup PPL_CXX_interface
  An object of the class Linear_Expression_Impl represents the linear
  expression
  \f[
    \sum_{i=0}^{n-1} a_i x_i + b
  \f]
  where \f$n\f$ is the dimension of the vector space,
  each \f$a_i\f$ is the integer coefficient
  of the \f$i\f$-th variable \f$x_i\f$
  and \f$b\f$ is the integer for the inhomogeneous term.

  \par How to build a linear expression.

  Linear expressions are the basic blocks for defining
  both constraints (i.e., linear equalities or inequalities)
  and generators (i.e., lines, rays, points and closure points).
  A full set of functions is defined to provide a convenient interface
  for building complex linear expressions starting from simpler ones
  and from objects of the classes Variable and Coefficient:
  available operators include unary negation,
  binary addition and subtraction,
  as well as multiplication by a Coefficient.
  The space dimension of a linear expression is defined as the maximum
  space dimension of the arguments used to build it:
  in particular, the space dimension of a Variable <CODE>x</CODE>
  is defined as <CODE>x.id()+1</CODE>,
  whereas all the objects of the class Coefficient have space dimension zero.

  \par Example
  The following code builds the linear expression \f$4x - 2y - z + 14\f$,
  having space dimension \f$3\f$:
  \code
  Linear_Expression_Impl e = 4*x - 2*y - z + 14;
  \endcode
  Another way to build the same linear expression is:
  \code
  Linear_Expression_Impl e1 = 4*x;
  Linear_Expression_Impl e2 = 2*y;
  Linear_Expression_Impl e3 = z;
  Linear_Expression_Impl e = Linear_Expression_Impl(14);
  e += e1 - e2 - e3;
  \endcode
  Note that \p e1, \p e2 and \p e3 have space dimension 1, 2 and 3,
  respectively; also, in the fourth line of code, \p e is created
  with space dimension zero and then extended to space dimension 3
  in the fifth line.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
class Parma_Polyhedra_Library::Linear_Expression_Impl
  : public Linear_Expression_Interface {
public:
  //! Default constructor: returns a copy of Linear_Expression_Impl::zero().
  Linear_Expression_Impl();

  //! Ordinary copy constructor.
  Linear_Expression_Impl(const Linear_Expression_Impl& e);

  //! Copy constructor for other row types.
  template <typename Row2>
  Linear_Expression_Impl(const Linear_Expression_Impl<Row2>& e);

  //! Copy constructor from any implementation of Linear_Expression_Interface.
  Linear_Expression_Impl(const Linear_Expression_Interface& e);

  //! Destructor.
  virtual ~Linear_Expression_Impl();

  //! Checks if all the invariants are satisfied.
  virtual bool OK() const;

  /*! \brief
    Builds the linear expression corresponding
    to the inhomogeneous term \p n.
  */
  explicit Linear_Expression_Impl(Coefficient_traits::const_reference n);

  //! Builds the linear expression corresponding to the variable \p v.
  /*!
    \exception std::length_error
    Thrown if the space dimension of \p v exceeds
    <CODE>Linear_Expression_Impl::max_space_dimension()</CODE>.
  */
  Linear_Expression_Impl(Variable v);

  //! Returns the current representation of this linear expression.
  virtual Representation representation() const;

  //! An interface for const iterators on the expression (homogeneous)
  //! coefficients that are nonzero.
  /*!
    These iterators are invalidated by operations that modify the expression.
  */
  class const_iterator: public const_iterator_interface {
  public:
    explicit const_iterator(const Row& row, dimension_type i);

    //! Returns a copy of *this.
    //! This returns a pointer to dynamic-allocated memory. The caller has the
    //! duty to free the memory when it's not needed anymore.
    virtual const_iterator_interface* clone() const;

    //! Navigates to the next nonzero coefficient.
    //! Note that this method does *not* return a reference, to increase
    //! efficiency since it's virtual.
    virtual void operator++();

    //! Navigates to the previous nonzero coefficient.
    //! Note that this method does *not* return a reference, to increase
    //! efficiency since it's virtual.
    virtual void operator--();

    //! Returns the current element.
    virtual reference operator*() const;

    //! Returns the variable of the coefficient pointed to by \c *this.
    /*!
      \returns the variable of the coefficient pointed to by \c *this.
    */
    virtual Variable variable() const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    virtual bool operator==(const const_iterator_interface& x) const;

  private:

    void skip_zeroes_forward();
    void skip_zeroes_backward();

    const Row* row;
    typename Row::const_iterator itr;
  };

  //! This returns a pointer to dynamic-allocated memory. The caller has the
  //! duty to free the memory when it's not needed anymore.
  virtual const_iterator_interface* begin() const;

  //! This returns a pointer to dynamic-allocated memory. The caller has the
  //! duty to free the memory when it's not needed anymore.
  virtual const_iterator_interface* end() const;

  //! This returns a pointer to dynamic-allocated memory. The caller has the
  //! duty to free the memory when it's not needed anymore.
  //! Returns (a pointer to) an iterator that points to the first nonzero
  //! coefficient of a variable greater than or equal to v, or at end if no
  //! such coefficient exists.
  virtual const_iterator_interface* lower_bound(Variable v) const;

  //! Returns the maximum space dimension a Linear_Expression_Impl can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  virtual dimension_type space_dimension() const;

  //! Sets the dimension of the vector space enclosing \p *this to \p n .
  virtual void set_space_dimension(dimension_type n);

  //! Returns the coefficient of \p v in \p *this.
  virtual Coefficient_traits::const_reference coefficient(Variable v) const;

  //! Sets the coefficient of \p v in \p *this to \p n.
  virtual void set_coefficient(Variable v,
                               Coefficient_traits::const_reference n);

  //! Returns the inhomogeneous term of \p *this.
  virtual Coefficient_traits::const_reference inhomogeneous_term() const;

  //! Sets the inhomogeneous term of \p *this to \p n.
  virtual void set_inhomogeneous_term(Coefficient_traits::const_reference n);

  //! Linearly combines \p *this with \p y so that the coefficient of \p v
  //! is 0.
  /*!
    \param y
    The expression that will be combined with \p *this object;

    \param v
    The variable whose coefficient has to become \f$0\f$.

    Computes a linear combination of \p *this and \p y having
    the coefficient of variable \p v equal to \f$0\f$. Then it assigns
    the resulting expression to \p *this.

    \p *this and \p y must have the same space dimension.
  */
  virtual void linear_combine(const Linear_Expression_Interface& y, Variable v);

  //! Equivalent to <CODE>*this = *this * c1 + y * c2</CODE>, but assumes that
  //! \p *this and \p y have the same space dimension.
  virtual void linear_combine(const Linear_Expression_Interface& y,
                              Coefficient_traits::const_reference c1,
                              Coefficient_traits::const_reference c2);

  //! Equivalent to <CODE>*this = *this * c1 + y * c2</CODE>.
  //! c1 and c2 may be 0.
  virtual void linear_combine_lax(const Linear_Expression_Interface& y,
                                  Coefficient_traits::const_reference c1,
                                  Coefficient_traits::const_reference c2);

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  virtual void swap_space_dimensions(Variable v1, Variable v2);

  //! Removes all the specified dimensions from the expression.
  /*!
    The space dimension of the variable with the highest space
    dimension in \p vars must be at most the space dimension
    of \p this.
  */
  virtual void remove_space_dimensions(const Variables_Set& vars);

  //! Shift by \p n positions the coefficients of variables, starting from
  //! the coefficient of \p v. This increases the space dimension by \p n.
  virtual void shift_space_dimensions(Variable v, dimension_type n);

  //! Permutes the space dimensions of the expression.
  /*!
    \param cycle
    A vector representing a cycle of the permutation according to which the
    space dimensions must be rearranged.

    The \p cycle vector represents a cycle of a permutation of space
    dimensions.
    For example, the permutation
    \f$ \{ x_1 \mapsto x_2, x_2 \mapsto x_3, x_3 \mapsto x_1 \}\f$ can be
    represented by the vector containing \f$ x_1, x_2, x_3 \f$.
  */
  virtual void permute_space_dimensions(const std::vector<Variable>& cycle);

  //! Returns <CODE>true</CODE> if and only if \p *this is \f$0\f$.
  virtual bool is_zero() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if all the homogeneous
    terms of \p *this are \f$0\f$.
  */
  virtual bool all_homogeneous_terms_are_zero() const;

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  virtual memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  virtual memory_size_type external_memory_in_bytes() const;

  //! Writes to \p s an ASCII representation of \p *this.
  virtual void ascii_dump(std::ostream& s) const;

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  virtual bool ascii_load(std::istream& s);

  //! Copy constructor with a specified space dimension.
  Linear_Expression_Impl(const Linear_Expression_Interface& e,
                         dimension_type space_dim);

  //! Returns \p true if *this is equal to \p x.
  //! Note that (*this == x) has a completely different meaning.
  virtual bool is_equal_to(const Linear_Expression_Interface& x) const;

  //! Normalizes the modulo of the coefficients and of the inhomogeneous term
  //! so that they are mutually prime.
  /*!
    Computes the Greatest Common Divisor (GCD) among the coefficients
    and the inhomogeneous term and normalizes them by the GCD itself.
  */
  virtual void normalize();

  //! Ensures that the first nonzero homogeneous coefficient is positive,
  //! by negating the row if necessary.
  virtual void sign_normalize();

  /*! \brief
    Negates the elements from index \p first (included)
    to index \p last (excluded).
  */
  virtual void negate(dimension_type first, dimension_type last);

  virtual Linear_Expression_Impl&
  operator+=(Coefficient_traits::const_reference n);
  virtual Linear_Expression_Impl&
  operator-=(Coefficient_traits::const_reference n);

  //! The basic comparison function.
  /*! \relates Linear_Expression_Impl

    \returns
    -1 or -2 if x is less than y, 0 if they are equal and 1 or 2 is y
    is greater. The absolute value of the result is 1 if the difference
    is only in the inhomogeneous terms, 2 otherwise.

    The order is a lexicographic. It starts comparing the variables'
    coefficient, starting from Variable(0), and at the end it compares
    the inhomogeneous terms.
  */
  virtual int compare(const Linear_Expression_Interface& y) const;

  virtual Linear_Expression_Impl&
  operator+=(const Linear_Expression_Interface& e2);
  virtual Linear_Expression_Impl& operator+=(const Variable v);
  virtual Linear_Expression_Impl&
  operator-=(const Linear_Expression_Interface& e2);
  virtual Linear_Expression_Impl& operator-=(const Variable v);
  virtual Linear_Expression_Impl&
  operator*=(Coefficient_traits::const_reference n);
  virtual Linear_Expression_Impl&
  operator/=(Coefficient_traits::const_reference n);

  virtual void negate();

  virtual Linear_Expression_Impl&
  add_mul_assign(Coefficient_traits::const_reference n, const Variable v);

  virtual Linear_Expression_Impl&
  sub_mul_assign(Coefficient_traits::const_reference n, const Variable v);

  virtual void add_mul_assign(Coefficient_traits::const_reference factor,
                              const Linear_Expression_Interface& e2);

  virtual void sub_mul_assign(Coefficient_traits::const_reference factor,
                              const Linear_Expression_Interface& e2);

  virtual void print(std::ostream& s) const;

  /*! \brief
    Returns <CODE>true</CODE> if the coefficient of each variable in
    \p vars[i] is \f$0\f$.
  */
  virtual bool all_zeroes(const Variables_Set& vars) const;

  //! Returns true if there is a variable in [first,last) whose coefficient
  //! is nonzero in both *this and x.
  virtual bool have_a_common_variable(const Linear_Expression_Interface& x,
                                      Variable first, Variable last) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns the i-th coefficient.
  virtual Coefficient_traits::const_reference get(dimension_type i) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Sets the i-th coefficient to n.
  virtual void set(dimension_type i, Coefficient_traits::const_reference n);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  /*! \brief
    Returns <CODE>true</CODE> if (*this)[i] is \f$0\f$, for each i in
    [start, end).
  */
  virtual bool all_zeroes(dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  /*! \brief
    Returns the number of zero coefficient in [start, end).
  */
  virtual dimension_type num_zeroes(dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  /*! \brief
    Returns the gcd of the nonzero coefficients in [start,end). If all the
    coefficients in this range are 0 returns 0.
  */
  virtual Coefficient gcd(dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  virtual void exact_div_assign(Coefficient_traits::const_reference c,
                                dimension_type start, dimension_type end);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Equivalent to <CODE>(*this)[i] *= n</CODE>, for each i in [start, end).
  virtual void mul_assign(Coefficient_traits::const_reference n,
                          dimension_type start, dimension_type end);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Linearly combines \p *this with \p y so that the coefficient of \p v
  //! is 0.
  /*!
    \param y
    The expression that will be combined with \p *this object;

    \param i
    The index of the coefficient that has to become \f$0\f$.

    Computes a linear combination of \p *this and \p y having
    the i-th coefficient equal to \f$0\f$. Then it assigns
    the resulting expression to \p *this.

    \p *this and \p y must have the same space dimension.
  */
  virtual void
  linear_combine(const Linear_Expression_Interface& y, dimension_type i);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end).
  virtual void linear_combine(const Linear_Expression_Interface& y,
                              Coefficient_traits::const_reference c1,
                              Coefficient_traits::const_reference c2,
                              dimension_type start, dimension_type end);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end). c1 and c2 may be zero.
  virtual void linear_combine_lax(const Linear_Expression_Interface& y,
                                  Coefficient_traits::const_reference c1,
                                  Coefficient_traits::const_reference c2,
                                  dimension_type start, dimension_type end);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns the index of the last nonzero element, or 0 if there are no
  //! nonzero elements.
  virtual dimension_type last_nonzero() const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  /*! \brief
    Returns <CODE>true</CODE> if each coefficient in [start,end) is *not* in
    \f$0\f$, disregarding coefficients of variables in \p vars.
  */
  virtual bool
  all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Sets results to the sum of (*this)[i]*y[i], for each i in [start,end).
  virtual void
  scalar_product_assign(Coefficient& result,
                        const Linear_Expression_Interface& y,
                        dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Computes the sign of the sum of (*this)[i]*y[i], for each i in [start,end).
  virtual int
  scalar_product_sign(const Linear_Expression_Interface& y,
                      dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns the index of the first nonzero element, or \p last if there are no
  //! nonzero elements, considering only elements in [first,last).
  virtual dimension_type
  first_nonzero(dimension_type first, dimension_type last) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns the index of the last nonzero element in [first,last), or last
  //! if there are no nonzero elements.
  virtual dimension_type
  last_nonzero(dimension_type first, dimension_type last) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Removes from the set x all the indexes of nonzero elements of *this.
  virtual void has_a_free_dimension_helper(std::set<dimension_type>& x) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns \p true if (*this)[i] is equal to x[i], for each i in [start,end).
  virtual bool is_equal_to(const Linear_Expression_Interface& x,
                           dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns \p true if (*this)[i]*c1 is equal to x[i]*c2, for each i in
  //! [start,end).
  virtual bool is_equal_to(const Linear_Expression_Interface& x,
                           Coefficient_traits::const_reference c1,
                           Coefficient_traits::const_reference c2,
                           dimension_type start, dimension_type end) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Sets `row' to a copy of the row that implements *this.
  virtual void get_row(Dense_Row& row) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Sets `row' to a copy of the row that implements *this.
  virtual void get_row(Sparse_Row& row) const;

  //! Implementation sizing constructor.
  /*!
    The bool parameter is just to avoid problems with the constructor
    Linear_Expression_Impl(Coefficient_traits::const_reference n).
  */
  Linear_Expression_Impl(dimension_type space_dim, bool);

  //! Linearly combines \p *this with \p y so that the coefficient of \p v
  //! is 0.
  /*!
    \param y
    The expression that will be combined with \p *this object;

    \param v
    The variable whose coefficient has to become \f$0\f$.

    Computes a linear combination of \p *this and \p y having
    the coefficient of variable \p v equal to \f$0\f$. Then it assigns
    the resulting expression to \p *this.

    \p *this and \p y must have the same space dimension.
  */
  template <typename Row2>
  void linear_combine(const Linear_Expression_Impl<Row2>& y, Variable v);

  //! Equivalent to <CODE>*this = *this * c1 + y * c2</CODE>, but assumes that
  //! \p *this and \p y have the same space dimension.
  template <typename Row2>
  void linear_combine(const Linear_Expression_Impl<Row2>& y,
                      Coefficient_traits::const_reference c1,
                      Coefficient_traits::const_reference c2);

  //! Equivalent to <CODE>*this = *this * c1 + y * c2</CODE>.
  //! c1 and c2 may be 0.
  template <typename Row2>
  void linear_combine_lax(const Linear_Expression_Impl<Row2>& y,
                          Coefficient_traits::const_reference c1,
                          Coefficient_traits::const_reference c2);

  //! Returns \p true if *this is equal to \p x.
  //! Note that (*this == x) has a completely different meaning.
  template <typename Row2>
  bool is_equal_to(const Linear_Expression_Impl<Row2>& x) const;

  template <typename Row2>
  Linear_Expression_Impl& operator+=(const Linear_Expression_Impl<Row2>& e2);
  template <typename Row2>
  Linear_Expression_Impl& operator-=(const Linear_Expression_Impl<Row2>& e2);

  template <typename Row2>
  Linear_Expression_Impl&
  sub_mul_assign(Coefficient_traits::const_reference n,
                 const Linear_Expression_Impl<Row2>& y,
                 dimension_type start, dimension_type end);

  template <typename Row2>
  void add_mul_assign(Coefficient_traits::const_reference factor,
                      const Linear_Expression_Impl<Row2>& e2);

  template <typename Row2>
  void sub_mul_assign(Coefficient_traits::const_reference factor,
                      const Linear_Expression_Impl<Row2>& e2);

  //! Linearly combines \p *this with \p y so that the coefficient of \p v
  //! is 0.
  /*!
    \param y
    The expression that will be combined with \p *this object;

    \param i
    The index of the coefficient that has to become \f$0\f$.

    Computes a linear combination of \p *this and \p y having
    the i-th coefficient equal to \f$0\f$. Then it assigns
    the resulting expression to \p *this.

    \p *this and \p y must have the same space dimension.
  */
  template <typename Row2>
  void linear_combine(const Linear_Expression_Impl<Row2>& y, dimension_type i);

  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end).
  template <typename Row2>
  void linear_combine(const Linear_Expression_Impl<Row2>& y,
                      Coefficient_traits::const_reference c1,
                      Coefficient_traits::const_reference c2,
                      dimension_type start, dimension_type end);

  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end). c1 and c2 may be zero.
  template <typename Row2>
  void linear_combine_lax(const Linear_Expression_Impl<Row2>& y,
                          Coefficient_traits::const_reference c1,
                          Coefficient_traits::const_reference c2,
                          dimension_type start, dimension_type end);

  //! The basic comparison function.
  /*! \relates Linear_Expression_Impl

    \returns
    -1 or -2 if x is less than y, 0 if they are equal and 1 or 2 is y
    is greater. The absolute value of the result is 1 if the difference
    is only in the inhomogeneous terms, 2 otherwise.

    The order is a lexicographic. It starts comparing the variables'
    coefficient, starting from Variable(0), and at the end it compares
    the inhomogeneous terms.
  */
  template <typename Row2>
  int compare(const Linear_Expression_Impl<Row2>& y) const;

  //! Sets results to the sum of (*this)[i]*y[i], for each i in [start,end).
  template <typename Row2>
  void
  scalar_product_assign(Coefficient& result,
                        const Linear_Expression_Impl<Row2>& y,
                        dimension_type start, dimension_type end) const;

  //! Computes the sign of the sum of (*this)[i]*y[i],
  //! for each i in [start,end).
  template <typename Row2>
  int scalar_product_sign(const Linear_Expression_Impl<Row2>& y,
                          dimension_type start, dimension_type end) const;

  //! Returns \p true if (*this)[i] is equal to x[i], for each i in [start,end).
  template <typename Row2>
  bool is_equal_to(const Linear_Expression_Impl<Row2>& x,
                   dimension_type start, dimension_type end) const;

  //! Returns \p true if (*this)[i]*c1 is equal to x[i]*c2, for each i in
  //! [start,end).
  template <typename Row2>
  bool is_equal_to(const Linear_Expression_Impl<Row2>& x,
                   Coefficient_traits::const_reference c1,
                   Coefficient_traits::const_reference c2,
                   dimension_type start, dimension_type end) const;

  //! Returns true if there is a variable in [first,last) whose coefficient
  //! is nonzero in both *this and x.
  template <typename Row2>
  bool have_a_common_variable(const Linear_Expression_Impl<Row2>& x,
                              Variable first, Variable last) const;

private:

  void construct(const Linear_Expression_Interface& e);
  void construct(const Linear_Expression_Interface& e,
                 dimension_type space_dim);

  template <typename Row2>
  void construct(const Linear_Expression_Impl<Row2>& e);
  template <typename Row2>
  void construct(const Linear_Expression_Impl<Row2>& e,
                 dimension_type space_dim);

  Row row;

  template <typename Row2>
  friend class Linear_Expression_Impl;

}; // class Parma_Polyhedra_Library::Linear_Expression_Impl


namespace Parma_Polyhedra_Library {

// NOTE: declaring explicit specializations.

template <>
bool
Linear_Expression_Impl<Dense_Row>::OK() const;
template <>
bool
Linear_Expression_Impl<Sparse_Row>::OK() const;

template <>
bool
Linear_Expression_Impl<Dense_Row>::all_homogeneous_terms_are_zero() const;
template <>
bool
Linear_Expression_Impl<Sparse_Row>::all_homogeneous_terms_are_zero() const;

template <>
bool
Linear_Expression_Impl<Dense_Row>::all_zeroes(dimension_type start,
                                              dimension_type end) const;
template <>
bool
Linear_Expression_Impl<Sparse_Row>::all_zeroes(dimension_type start,
                                               dimension_type end) const;

template <>
bool
Linear_Expression_Impl<Dense_Row>
::all_zeroes(const Variables_Set& vars) const;
template <>
bool
Linear_Expression_Impl<Sparse_Row>
::all_zeroes(const Variables_Set& vars) const;

template <>
bool
Linear_Expression_Impl<Dense_Row>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const;
template <>
bool
Linear_Expression_Impl<Sparse_Row>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const;

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>
::first_nonzero(dimension_type first, dimension_type last) const;
template <>
dimension_type
Linear_Expression_Impl<Sparse_Row>
::first_nonzero(dimension_type first, dimension_type last) const;

template <>
Coefficient
Linear_Expression_Impl<Dense_Row>::gcd(dimension_type start,
                                       dimension_type end) const;
template <>
Coefficient
Linear_Expression_Impl<Sparse_Row>::gcd(dimension_type start,
                                        dimension_type end) const;

template <>
void
Linear_Expression_Impl<Dense_Row>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const;
template <>
void
Linear_Expression_Impl<Sparse_Row>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const;

template <>
template <>
bool
Linear_Expression_Impl<Dense_Row>
::have_a_common_variable(const Linear_Expression_Impl<Dense_Row>& y,
                         Variable first, Variable last) const;
template <>
template <>
bool
Linear_Expression_Impl<Dense_Row>
::have_a_common_variable(const Linear_Expression_Impl<Sparse_Row>& y,
                         Variable first, Variable last) const;
template <>
template <>
bool
Linear_Expression_Impl<Sparse_Row>
::have_a_common_variable(const Linear_Expression_Impl<Dense_Row>& y,
                         Variable first, Variable last) const;
template <>
template <>
bool
Linear_Expression_Impl<Sparse_Row>
::have_a_common_variable(const Linear_Expression_Impl<Sparse_Row>& y,
                         Variable first, Variable last) const;

template <>
bool
Linear_Expression_Impl<Dense_Row>::is_zero() const;
template <>
bool
Linear_Expression_Impl<Sparse_Row>::is_zero() const;

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>::last_nonzero() const;
template <>
dimension_type
Linear_Expression_Impl<Sparse_Row>::last_nonzero() const;

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>
::last_nonzero(dimension_type first, dimension_type last) const;
template <>
dimension_type
Linear_Expression_Impl<Sparse_Row>
::last_nonzero(dimension_type first, dimension_type last) const;

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>::num_zeroes(dimension_type start,
                                              dimension_type end) const;
template <>
dimension_type
Linear_Expression_Impl<Sparse_Row>::num_zeroes(dimension_type start,
                                               dimension_type end) const;

template <>
void
Linear_Expression_Impl<Dense_Row>
::remove_space_dimensions(const Variables_Set& vars);
template <>
void
Linear_Expression_Impl<Sparse_Row>
::remove_space_dimensions(const Variables_Set& vars);

template <>
Representation
Linear_Expression_Impl<Dense_Row>::representation() const;
template <>
Representation
Linear_Expression_Impl<Sparse_Row>::representation() const;

template <>
void
Linear_Expression_Impl<Dense_Row>::const_iterator::skip_zeroes_backward();
template <>
void
Linear_Expression_Impl<Sparse_Row>::const_iterator::skip_zeroes_backward();

template <>
void
Linear_Expression_Impl<Dense_Row>::const_iterator::skip_zeroes_forward();
template <>
void
Linear_Expression_Impl<Sparse_Row>::const_iterator::skip_zeroes_forward();

} // namespace Parma_Polyhedra_Library


#include "Linear_Expression_Impl_inlines.hh"
#include "Linear_Expression_Impl_templates.hh"

#endif // !defined(PPL_Linear_Expression_Impl_defs_hh)
