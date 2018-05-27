/* Linear_Expression class declaration.
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

#ifndef PPL_Linear_Expression_defs_hh
#define PPL_Linear_Expression_defs_hh 1

#include "Linear_Expression_types.hh"

#include "Constraint_types.hh"
#include "Generator_types.hh"
#include "Congruence_types.hh"
#include "Grid_Generator_types.hh"
#include "Linear_System_types.hh"
#include "Constraint_System_types.hh"
#include "Congruence_System_types.hh"
#include "Coefficient_types.hh"
#include "Polyhedron_types.hh"
#include "Grid_types.hh"
#include "PIP_Problem_types.hh"
#include "BHRZ03_Certificate_types.hh"
#include "Scalar_Products_types.hh"
#include "MIP_Problem_types.hh"
#include "Box_types.hh"
#include "BD_Shape_types.hh"
#include "Octagonal_Shape_types.hh"
#include "termination_types.hh"

#include "Expression_Adapter_defs.hh"
#include "Expression_Hide_Inhomo_types.hh"
#include "Expression_Hide_Last_types.hh"

#include "Linear_Expression_Interface_defs.hh"
#include "Variable_defs.hh"

namespace Parma_Polyhedra_Library {

// Put them in the namespace here to declare them friend later.

//! Returns the linear expression \p e1 + \p e2.
/*! \relates Linear_Expression */
Linear_Expression
operator+(const Linear_Expression& e1, const Linear_Expression& e2);

//! Returns the linear expression \p v + \p w.
/*! \relates Linear_Expression */
Linear_Expression
operator+(Variable v, Variable w);

//! Returns the linear expression \p v + \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator+(Variable v, const Linear_Expression& e);

//! Returns the linear expression \p e + \p v.
/*! \relates Linear_Expression */
Linear_Expression
operator+(const Linear_Expression& e, Variable v);

//! Returns the linear expression \p n + \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator+(Coefficient_traits::const_reference n, const Linear_Expression& e);

//! Returns the linear expression \p e + \p n.
/*! \relates Linear_Expression */
Linear_Expression
operator+(const Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns the linear expression \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator+(const Linear_Expression& e);

//! Returns the linear expression - \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator-(const Linear_Expression& e);

//! Returns the linear expression \p e1 - \p e2.
/*! \relates Linear_Expression */
Linear_Expression
operator-(const Linear_Expression& e1, const Linear_Expression& e2);

//! Returns the linear expression \p v - \p w.
/*! \relates Linear_Expression */
Linear_Expression
operator-(Variable v, Variable w);

//! Returns the linear expression \p v - \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator-(Variable v, const Linear_Expression& e);

//! Returns the linear expression \p e - \p v.
/*! \relates Linear_Expression */
Linear_Expression
operator-(const Linear_Expression& e, Variable v);

//! Returns the linear expression \p n - \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator-(Coefficient_traits::const_reference n, const Linear_Expression& e);

//! Returns the linear expression \p e - \p n.
/*! \relates Linear_Expression */
Linear_Expression
operator-(const Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns the linear expression \p n * \p e.
/*! \relates Linear_Expression */
Linear_Expression
operator*(Coefficient_traits::const_reference n, const Linear_Expression& e);

//! Returns the linear expression \p e * \p n.
/*! \relates Linear_Expression */
Linear_Expression
operator*(const Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns the linear expression \p e1 + \p e2 and assigns it to \p e1.
/*! \relates Linear_Expression */
Linear_Expression&
operator+=(Linear_Expression& e1, const Linear_Expression& e2);

//! Returns the linear expression \p e + \p v and assigns it to \p e.
/*! \relates Linear_Expression
  \exception std::length_error
  Thrown if the space dimension of \p v exceeds
  <CODE>Linear_Expression::max_space_dimension()</CODE>.
 */
Linear_Expression&
operator+=(Linear_Expression& e, Variable v);

//! Returns the linear expression \p e + \p n and assigns it to \p e.
/*! \relates Linear_Expression */
Linear_Expression&
operator+=(Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns the linear expression \p e1 - \p e2 and assigns it to \p e1.
/*! \relates Linear_Expression */
Linear_Expression&
operator-=(Linear_Expression& e1, const Linear_Expression& e2);

//! Returns the linear expression \p e - \p v and assigns it to \p e.
/*! \relates Linear_Expression
  \exception std::length_error
  Thrown if the space dimension of \p v exceeds
  <CODE>Linear_Expression::max_space_dimension()</CODE>.
 */
Linear_Expression&
operator-=(Linear_Expression& e, Variable v);

//! Returns the linear expression \p e - \p n and assigns it to \p e.
/*! \relates Linear_Expression */
Linear_Expression&
operator-=(Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns the linear expression \p n * \p e and assigns it to \p e.
/*! \relates Linear_Expression */
Linear_Expression&
operator*=(Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns the linear expression \p n / \p e and assigns it to \p e.
/*! \relates Linear_Expression */
Linear_Expression&
operator/=(Linear_Expression& e, Coefficient_traits::const_reference n);

//! Assigns to \p e its own negation.
/*! \relates Linear_Expression */
void
neg_assign(Linear_Expression& e);

//! Returns the linear expression \p e + \p n * \p v and assigns it to \p e.
/*! \relates Linear_Expression */
Linear_Expression&
add_mul_assign(Linear_Expression& e,
               Coefficient_traits::const_reference n, Variable v);

//! Sums \p e2 multiplied by \p factor into \p e1.
/*! \relates Linear_Expression */
void add_mul_assign(Linear_Expression& e1,
                    Coefficient_traits::const_reference factor,
                    const Linear_Expression& e2);

//! Subtracts \p e2 multiplied by \p factor from \p e1.
/*! \relates Linear_Expression */
void sub_mul_assign(Linear_Expression& e1,
                    Coefficient_traits::const_reference factor,
                    const Linear_Expression& e2);

//! Returns the linear expression \p e - \p n * \p v and assigns it to \p e.
/*! \relates Linear_Expression */
Linear_Expression&
sub_mul_assign(Linear_Expression& e,
               Coefficient_traits::const_reference n, Variable v);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The basic comparison function.
/*! \relates Linear_Expression

  \returns -1 or -2 if x is less than y, 0 if they are equal and 1 or 2 is y
           is greater. The absolute value of the result is 1 if the difference
           is only in the inhomogeneous terms, 2 otherwise

  The order is a lexicographic. It starts comparing the variables' coefficient,
  starting from Variable(0), and at the end it compares the inhomogeneous
  terms.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
int compare(const Linear_Expression& x, const Linear_Expression& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Linear_Expression */
std::ostream& operator<<(std::ostream& s, const Linear_Expression& e);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

//! A linear expression.
/*! \ingroup PPL_CXX_interface
  An object of the class Linear_Expression represents the linear expression
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
  Linear_Expression e = 4*x - 2*y - z + 14;
  \endcode
  Another way to build the same linear expression is:
  \code
  Linear_Expression e1 = 4*x;
  Linear_Expression e2 = 2*y;
  Linear_Expression e3 = z;
  Linear_Expression e = Linear_Expression(14);
  e += e1 - e2 - e3;
  \endcode
  Note that \p e1, \p e2 and \p e3 have space dimension 1, 2 and 3,
  respectively; also, in the fourth line of code, \p e is created
  with space dimension zero and then extended to space dimension 3
  in the fifth line.
*/
class Parma_Polyhedra_Library::Linear_Expression {
public:
  static const Representation default_representation = SPARSE;

  //! Default constructor: returns a copy of Linear_Expression::zero().
  explicit Linear_Expression(Representation r = default_representation);

  /*! \brief Ordinary copy constructor.
    \note
    The new expression will have the same representation as \p e
    (not necessarily the default_representation).
  */
  Linear_Expression(const Linear_Expression& e);

  //! Copy constructor that takes also a Representation.
  Linear_Expression(const Linear_Expression& e, Representation r);

  // Queried by expression adapters.
  typedef const Linear_Expression& const_reference;
  typedef Linear_Expression raw_type;

  /*! \brief Copy constructor from a linear expression adapter.
    \note
    The new expression will have the same representation as \p e
    (not necessarily the default_representation).
  */
  template <typename LE_Adapter>
  explicit
  Linear_Expression(const LE_Adapter& e,
                    typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type = 0);

  /*! \brief Copy constructor from a linear expression adapter that takes a
    Representation.
  */
  template <typename LE_Adapter>
  Linear_Expression(const LE_Adapter& e, Representation r,
                    typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type = 0);

  /*! \brief
    Copy constructor from a linear expression adapter that takes a
    space dimension.
    \note
    The new expression will have the same representation as \p e
    (not necessarily default_representation).
  */
  template <typename LE_Adapter>
  explicit
  Linear_Expression(const LE_Adapter& e, dimension_type space_dim,
                    typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type = 0);

  /*! \brief
    Copy constructor from a linear expression adapter that takes a
    space dimension and a Representation.
  */
  template <typename LE_Adapter>
  Linear_Expression(const LE_Adapter& e,
                    dimension_type space_dim, Representation r,
                    typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type = 0);

  //! Assignment operator.
  Linear_Expression& operator=(const Linear_Expression& e);

  //! Destructor.
  ~Linear_Expression();

  /*! \brief
    Builds the linear expression corresponding
    to the inhomogeneous term \p n.
  */
  explicit Linear_Expression(Coefficient_traits::const_reference n,
                             Representation r = default_representation);

  //! Builds the linear expression corresponding to the variable \p v.
  /*!
    \exception std::length_error
    Thrown if the space dimension of \p v exceeds
    <CODE>Linear_Expression::max_space_dimension()</CODE>.
  */
  Linear_Expression(Variable v, Representation r = default_representation);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! A const %iterator on the expression (homogeneous) coefficient that are
  //! nonzero.
  /*!
    These iterators are invalidated by operations that modify the expression.
  */
  class const_iterator {
  private:
  public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef const Coefficient value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef Coefficient_traits::const_reference reference;

    //! Constructs an invalid const_iterator.
    /*!
      This constructor takes \f$O(1)\f$ time.
    */
    explicit const_iterator();

    //! The copy constructor.
    /*!
      \param itr
      The %iterator that will be copied.

      This constructor takes \f$O(1)\f$ time.
    */
    const_iterator(const const_iterator& itr);

    ~const_iterator();

    //! Swaps itr with *this.
    /*!
      \param itr
      The %iterator that will be swapped with *this.

      This method takes \f$O(1)\f$ time.
    */
    void m_swap(const_iterator& itr);

    //! Assigns \p itr to *this .
    /*!
      \param itr
      The %iterator that will be assigned into *this.

      This method takes \f$O(1)\f$ time.
    */
    const_iterator& operator=(const const_iterator& itr);

    //! Navigates to the next nonzero coefficient.
    /*!
      This method takes \f$O(n)\f$ time for dense expressions, and
      \f$O(1)\f$ time for sparse expressions.
    */
    const_iterator& operator++();

    //! Navigates to the previous nonzero coefficient.
    /*!
      This method takes \f$O(n)\f$ time for dense expressions, and
      \f$O(1)\f$ time for sparse expressions.
    */
    const_iterator& operator--();

    //! Returns the current element.
    reference operator*() const;

    //! Returns the variable of the coefficient pointed to by \c *this.
    /*!
      \returns the variable of the coefficient pointed to by \c *this.
    */
    Variable variable() const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    bool operator==(const const_iterator& x) const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    bool operator!=(const const_iterator& x) const;

  private:
    //! Constructor from a const_iterator_interface*.
    //! The new object takes ownership of the dynamic object.
    const_iterator(Linear_Expression_Interface::const_iterator_interface* itr);

    Linear_Expression_Interface::const_iterator_interface* itr;

    friend class Linear_Expression;
  };

  //! Returns an iterator that points to the first nonzero coefficient in the
  //! expression.
  const_iterator begin() const;

  //! Returns an iterator that points to the last nonzero coefficient in the
  //! expression.
  const_iterator end() const;

  //! Returns an iterator that points to the first nonzero coefficient of a
  //! variable bigger than or equal to v.
  const_iterator lower_bound(Variable v) const;

  //! Returns the maximum space dimension a Linear_Expression can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Sets the dimension of the vector space enclosing \p *this to \p n .
  void set_space_dimension(dimension_type n);

  //! Returns the coefficient of \p v in \p *this.
  Coefficient_traits::const_reference coefficient(Variable v) const;

  //! Sets the coefficient of \p v in \p *this to \p n.
  void set_coefficient(Variable v,
                       Coefficient_traits::const_reference n);

  //! Returns the inhomogeneous term of \p *this.
  Coefficient_traits::const_reference inhomogeneous_term() const;

  //! Sets the inhomogeneous term of \p *this to \p n.
  void set_inhomogeneous_term(Coefficient_traits::const_reference n);

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
  void linear_combine(const Linear_Expression& y, Variable v);

  //! Equivalent to <CODE>*this = *this * c1 + y * c2</CODE>, but assumes that
  //! c1 and c2 are not 0.
  void linear_combine(const Linear_Expression& y,
                      Coefficient_traits::const_reference c1,
                      Coefficient_traits::const_reference c2);

  //! Equivalent to <CODE>*this = *this * c1 + y * c2</CODE>.
  //! c1 and c2 may be 0.
  void linear_combine_lax(const Linear_Expression& y,
                          Coefficient_traits::const_reference c1,
                          Coefficient_traits::const_reference c2);

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  void swap_space_dimensions(Variable v1, Variable v2);

  //! Removes all the specified dimensions from the expression.
  /*!
    The space dimension of the variable with the highest space
    dimension in \p vars must be at most the space dimension
    of \p this.
  */
  void remove_space_dimensions(const Variables_Set& vars);

  //! Shift by \p n positions the coefficients of variables, starting from
  //! the coefficient of \p v. This increases the space dimension by \p n.
  void shift_space_dimensions(Variable v, dimension_type n);

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
  void permute_space_dimensions(const std::vector<Variable>& cycle);

  //! Returns <CODE>true</CODE> if and only if \p *this is \f$0\f$.
  bool is_zero() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if all the homogeneous
    terms of \p *this are \f$0\f$.
  */
  bool all_homogeneous_terms_are_zero() const;

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  //! Returns the (zero-dimension space) constant 0.
  static const Linear_Expression& zero();

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Swaps \p *this with \p y.
  void m_swap(Linear_Expression& y);

  //! Copy constructor with a specified space dimension.
  Linear_Expression(const Linear_Expression& e, dimension_type space_dim);

  //! Copy constructor with a specified space dimension and representation.
  Linear_Expression(const Linear_Expression& e, dimension_type space_dim,
                    Representation r);

  //! Returns \p true if *this is equal to \p x.
  //! Note that (*this == x) has a completely different meaning.
  bool is_equal_to(const Linear_Expression& x) const;

  //! Normalizes the modulo of the coefficients and of the inhomogeneous term
  //! so that they are mutually prime.
  /*!
    Computes the Greatest Common Divisor (GCD) among the coefficients
    and the inhomogeneous term and normalizes them by the GCD itself.
  */
  void normalize();

  //! Ensures that the first nonzero homogeneous coefficient is positive,
  //! by negating the row if necessary.
  void sign_normalize();

  /*! \brief
    Returns <CODE>true</CODE> if the coefficient of each variable in
    \p vars[i] is \f$0\f$.
  */
  bool all_zeroes(const Variables_Set& vars) const;

private:
  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the (zero-dimension space) constant 0.
  */
  static const Linear_Expression* zero_p;

  Linear_Expression_Interface* impl;

  //! Implementation sizing constructor.
  /*!
    The bool parameter is just to avoid problems with
    the constructor Linear_Expression(Coefficient_traits::const_reference n).
  */
  Linear_Expression(dimension_type space_dim, bool,
                    Representation r = default_representation);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns the i-th coefficient.
  Coefficient_traits::const_reference get(dimension_type i) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Sets the i-th coefficient to n.
  void set(dimension_type i, Coefficient_traits::const_reference n);

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Returns the coefficient of v.
  Coefficient_traits::const_reference get(Variable v) const;

  // NOTE: This method is public, but it's not exposed in Linear_Expression,
  // so that it can be used internally in the PPL, by friends of
  // Linear_Expression.
  //! Sets the coefficient of v to n.
  void set(Variable v, Coefficient_traits::const_reference n);

  /*! \brief
    Returns <CODE>true</CODE> if (*this)[i] is \f$0\f$, for each i in
    [start, end).
  */
  bool all_zeroes(dimension_type start, dimension_type end) const;

  /*! \brief
    Returns the number of zero coefficient in [start, end).
  */
  dimension_type num_zeroes(dimension_type start, dimension_type end) const;

  /*! \brief
    Returns the gcd of the nonzero coefficients in [start,end). If all the
    coefficients in this range are 0 returns 0.
  */
  Coefficient gcd(dimension_type start, dimension_type end) const;

  void exact_div_assign(Coefficient_traits::const_reference c,
                        dimension_type start, dimension_type end);

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
  void linear_combine(const Linear_Expression& y, dimension_type i);

  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end). It assumes that c1 and c2 are nonzero.
  void linear_combine(const Linear_Expression& y,
                      Coefficient_traits::const_reference c1,
                      Coefficient_traits::const_reference c2,
                      dimension_type start, dimension_type end);

  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end). c1 and c2 may be zero.
  void linear_combine_lax(const Linear_Expression& y,
                          Coefficient_traits::const_reference c1,
                          Coefficient_traits::const_reference c2,
                          dimension_type start, dimension_type end);

  //! Equivalent to <CODE>(*this)[i] *= n</CODE>, for each i in [start, end).
  void mul_assign(Coefficient_traits::const_reference n,
                  dimension_type start, dimension_type end);

  //! Returns the index of the last nonzero element, or 0 if there are no
  //! nonzero elements.
  dimension_type last_nonzero() const;

  //! Returns the index of the last nonzero element in [first,last), or last
  //! if there are no nonzero elements.
  dimension_type last_nonzero(dimension_type first, dimension_type last) const;

  //! Returns the index of the first nonzero element, or \p last if there are no
  //! nonzero elements, considering only elements in [first,last).
  dimension_type first_nonzero(dimension_type first, dimension_type last) const;

  /*! \brief
    Returns <CODE>true</CODE> if all coefficients in [start,end),
    except those corresponding to variables in \p vars, are zero.
  */
  bool all_zeroes_except(const Variables_Set& vars,
                         dimension_type start, dimension_type end) const;

  //! Sets results to the sum of (*this)[i]*y[i], for each i.
  void scalar_product_assign(Coefficient& result,
                             const Linear_Expression& y) const;

  //! Sets results to the sum of (*this)[i]*y[i], for each i in [start,end).
  void scalar_product_assign(Coefficient& result, const Linear_Expression& y,
                             dimension_type start, dimension_type end) const;

  //! Computes the sign of the sum of (*this)[i]*y[i], for each i.
  int scalar_product_sign(const Linear_Expression& y) const;

  //! Computes the sign of the sum of (*this)[i]*y[i],
  //! for each i in [start,end).
  int scalar_product_sign(const Linear_Expression& y,
                          dimension_type start, dimension_type end) const;

  //! Removes from the set x all the indexes of nonzero elements of *this.
  void has_a_free_dimension_helper(std::set<dimension_type>& x) const;

  //! Returns \p true if (*this)[i] is equal to x[i], for each i in [start,end).
  bool is_equal_to(const Linear_Expression& x,
                   dimension_type start, dimension_type end) const;

  //! Returns \p true if (*this)[i]*c1 is equal to x[i]*c2, for each i in
  //! [start,end).
  bool is_equal_to(const Linear_Expression& x,
                   Coefficient_traits::const_reference c1,
                   Coefficient_traits::const_reference c2,
                   dimension_type start, dimension_type end) const;

  //! Sets `row' to a copy of the row that implements *this.
  void get_row(Dense_Row& row) const;

  //! Sets `row' to a copy of the row that implements *this.
  void get_row(Sparse_Row& row) const;

  //! Returns true if there is a variable in [first,last) whose coefficient
  //! is nonzero in both *this and x.
  bool have_a_common_variable(const Linear_Expression& x,
                              Variable first, Variable last) const;

  /*! \brief
    Negates the elements from index \p first (included)
    to index \p last (excluded).
  */
  void negate(dimension_type first, dimension_type last);

  template <typename Row>
  friend class Linear_Expression_Impl;

  // NOTE: The following classes are friends of Linear_Expression in order
  // to access its private methods.
  // Since they are *not* friend of Linear_Expression_Impl, they can only
  // access its public methods so they cannot break the class invariant of
  // Linear_Expression_Impl.
  friend class Grid;
  friend class Congruence;
  friend class Polyhedron;
  friend class PIP_Tree_Node;
  friend class Grid_Generator;
  friend class Generator;
  friend class Constraint;
  friend class Constraint_System;
  friend class PIP_Problem;
  friend class BHRZ03_Certificate;
  friend class Scalar_Products;
  friend class MIP_Problem;
  friend class Box_Helpers;
  friend class Congruence_System;
  friend class BD_Shape_Helpers;
  friend class Octagonal_Shape_Helper;
  friend class Termination_Helpers;
  template <typename T>
  friend class BD_Shape;
  template <typename T>
  friend class Octagonal_Shape;
  template <typename T>
  friend class Linear_System;
  template <typename T>
  friend class Box;
  template <typename T>
  friend class Expression_Adapter;
  template <typename T>
  friend class Expression_Hide_Inhomo;
  template <typename T>
  friend class Expression_Hide_Last;

  friend Linear_Expression
  operator+(const Linear_Expression& e1, const Linear_Expression& e2);
  friend Linear_Expression
  operator+(Coefficient_traits::const_reference n, const Linear_Expression& e);
  friend Linear_Expression
  operator+(const Linear_Expression& e, Coefficient_traits::const_reference n);
  friend Linear_Expression
  operator+(Variable v, const Linear_Expression& e);
  friend Linear_Expression
  operator+(Variable v, Variable w);

  friend Linear_Expression
  operator-(const Linear_Expression& e);

  friend Linear_Expression
  operator-(const Linear_Expression& e1, const Linear_Expression& e2);
  friend Linear_Expression
  operator-(Variable v, Variable w);
  friend Linear_Expression
  operator-(Coefficient_traits::const_reference n, const Linear_Expression& e);
  friend Linear_Expression
  operator-(const Linear_Expression& e, Coefficient_traits::const_reference n);
  friend Linear_Expression
  operator-(Variable v, const Linear_Expression& e);
  friend Linear_Expression
  operator-(const Linear_Expression& e, Variable v);

  friend Linear_Expression
  operator*(Coefficient_traits::const_reference n, const Linear_Expression& e);
  friend Linear_Expression
  operator*(const Linear_Expression& e, Coefficient_traits::const_reference n);

  friend Linear_Expression&
  operator+=(Linear_Expression& e1, const Linear_Expression& e2);
  friend Linear_Expression&
  operator+=(Linear_Expression& e, Variable v);
  friend Linear_Expression&
  operator+=(Linear_Expression& e, Coefficient_traits::const_reference n);

  friend Linear_Expression&
  operator-=(Linear_Expression& e1, const Linear_Expression& e2);
  friend Linear_Expression&
  operator-=(Linear_Expression& e, Variable v);
  friend Linear_Expression&
  operator-=(Linear_Expression& e, Coefficient_traits::const_reference n);

  friend Linear_Expression&
  operator*=(Linear_Expression& e, Coefficient_traits::const_reference n);
  friend Linear_Expression&
  operator/=(Linear_Expression& e, Coefficient_traits::const_reference n);

  friend void
  neg_assign(Linear_Expression& e);

  friend Linear_Expression&
  add_mul_assign(Linear_Expression& e,
                 Coefficient_traits::const_reference n, Variable v);
  friend Linear_Expression&
  sub_mul_assign(Linear_Expression& e,
                 Coefficient_traits::const_reference n, Variable v);

  friend void
  add_mul_assign(Linear_Expression& e1,
                 Coefficient_traits::const_reference factor,
                 const Linear_Expression& e2);
  friend void
  sub_mul_assign(Linear_Expression& e1,
                 Coefficient_traits::const_reference factor,
                 const Linear_Expression& e2);

  friend int
  compare(const Linear_Expression& x, const Linear_Expression& y);

  friend std::ostream&
  Parma_Polyhedra_Library::IO_Operators
  ::operator<<(std::ostream& s, const Linear_Expression& e);
};

namespace Parma_Polyhedra_Library {

//! Swaps \p x with \p y.
/*! \relates Linear_Expression */
void swap(Linear_Expression& x, Linear_Expression& y);

//! Swaps \p x with \p y.
/*! \relates Linear_Expression::const_iterator */
void swap(Linear_Expression::const_iterator& x,
          Linear_Expression::const_iterator& y);

} // namespace Parma_Polyhedra_Library

#include "Linear_Expression_inlines.hh"

#endif // !defined(PPL_Linear_Expression_defs_hh)
