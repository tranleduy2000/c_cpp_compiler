/* Congruence class declaration.
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

#ifndef PPL_Congruence_defs_hh
#define PPL_Congruence_defs_hh 1

#include "Congruence_types.hh"

#include "Coefficient_defs.hh"
#include "Variable_defs.hh"

#include "Constraint_types.hh"
#include "Grid_types.hh"
#include "Scalar_Products_types.hh"
#include "Linear_Expression_defs.hh"
#include "Expression_Adapter_defs.hh"

#include <iosfwd>
#include <vector>

// These are declared here because they are friend of Congruence.
namespace Parma_Polyhedra_Library {

//! Returns <CODE>true</CODE> if and only if \p x and \p y are equivalent.
/*! \relates Congruence */
bool
operator==(const Congruence& x, const Congruence& y);

//! Returns <CODE>false</CODE> if and only if \p x and \p y are equivalent.
/*! \relates Congruence */
bool
operator!=(const Congruence& x, const Congruence& y);

} // namespace Parma_Polyhedra_Library


//! A linear congruence.
/*! \ingroup PPL_CXX_interface
  An object of the class Congruence is a congruence:
  - \f$\cg = \sum_{i=0}^{n-1} a_i x_i + b = 0 \pmod m\f$

  where \f$n\f$ is the dimension of the space,
  \f$a_i\f$ is the integer coefficient of variable \f$x_i\f$,
  \f$b\f$ is the integer inhomogeneous term and \f$m\f$ is the integer modulus;
  if \f$m = 0\f$, then \f$\cg\f$ represents the equality congruence
  \f$\sum_{i=0}^{n-1} a_i x_i + b = 0\f$
  and, if \f$m \neq 0\f$, then the congruence \f$\cg\f$ is
  said to be a proper congruence.

  \par How to build a congruence
  Congruences \f$\pmod{1}\f$ are typically built by
  applying the congruence symbol `<CODE>\%=</CODE>'
  to a pair of linear expressions.
  Congruences with modulus \p m
  are typically constructed by building a congruence \f$\pmod{1}\f$
  using the given pair of linear expressions
  and then adding the modulus \p m
  using the modulus symbol is `<CODE>/</CODE>'.

  The space dimension of a congruence is defined as the maximum
  space dimension of the arguments of its constructor.

  \par
  In the following examples it is assumed that variables
  <CODE>x</CODE>, <CODE>y</CODE> and <CODE>z</CODE>
  are defined as follows:
  \code
  Variable x(0);
  Variable y(1);
  Variable z(2);
  \endcode

  \par Example 1
  The following code builds the equality congruence
  \f$3x + 5y - z = 0\f$, having space dimension \f$3\f$:
  \code
  Congruence eq_cg((3*x + 5*y - z %= 0) / 0);
  \endcode
  The following code builds the congruence
  \f$4x = 2y - 13 \pmod{1}\f$, having space dimension \f$2\f$:
  \code
  Congruence mod1_cg(4*x %= 2*y - 13);
  \endcode
  The following code builds the congruence
  \f$4x = 2y - 13 \pmod{2}\f$, having space dimension \f$2\f$:
  \code
  Congruence mod2_cg((4*x %= 2*y - 13) / 2);
  \endcode
  An unsatisfiable congruence on the zero-dimension space \f$\Rset^0\f$
  can be specified as follows:
  \code
  Congruence false_cg = Congruence::zero_dim_false();
  \endcode
  Equivalent, but more involved ways are the following:
  \code
  Congruence false_cg1((Linear_Expression::zero() %= 1) / 0);
  Congruence false_cg2((Linear_Expression::zero() %= 1) / 2);
  \endcode
  In contrast, the following code defines an unsatisfiable congruence
  having space dimension \f$3\f$:
  \code
  Congruence false_cg3((0*z %= 1) / 0);
  \endcode

  \par How to inspect a congruence
  Several methods are provided to examine a congruence and extract
  all the encoded information: its space dimension, its modulus
  and the value of its integer coefficients.

  \par Example 2
  The following code shows how it is possible to access the modulus
  as well as each of the coefficients.
  Given a congruence with linear expression \p e and modulus \p m
  (in this case \f$x - 5y + 3z = 4 \pmod{5}\f$), we construct a new
  congruence with the same modulus \p m but where the linear
  expression is \f$2 e\f$ (\f$2x - 10y + 6z = 8 \pmod{5}\f$).
  \code
  Congruence cg1((x - 5*y + 3*z %= 4) / 5);
  cout << "Congruence cg1: " << cg1 << endl;
  const Coefficient& m = cg1.modulus();
  if (m == 0)
    cout << "Congruence cg1 is an equality." << endl;
  else {
    Linear_Expression e;
    for (dimension_type i = cg1.space_dimension(); i-- > 0; )
      e += 2 * cg1.coefficient(Variable(i)) * Variable(i);
      e += 2 * cg1.inhomogeneous_term();
    Congruence cg2((e %= 0) / m);
    cout << "Congruence cg2: " << cg2 << endl;
  }
  \endcode
  The actual output could be the following:
  \code
  Congruence cg1: A - 5*B + 3*C %= 4 / 5
  Congruence cg2: 2*A - 10*B + 6*C %= 8 / 5
  \endcode
  Note that, in general, the particular output obtained can be
  syntactically different from the (semantically equivalent)
  congruence considered.
*/
class Parma_Polyhedra_Library::Congruence {
public:

  //! The representation used for new Congruences.
  /*!
    \note The copy constructor and the copy constructor with specified size
          use the representation of the original object, so that it is
          indistinguishable from the original object.
  */
  static const Representation default_representation = SPARSE;

  //! Constructs the 0 = 0 congruence with space dimension \p 0 .
  explicit Congruence(Representation r = default_representation);

  //! Ordinary copy constructor.
  /*!
    \note The new Congruence will have the same representation as `cg',
          not default_representation, so that they are indistinguishable.
  */
  Congruence(const Congruence& cg);

  //! Copy constructor with specified representation.
  Congruence(const Congruence& cg, Representation r);

  //! Copy-constructs (modulo 0) from equality constraint \p c.
  /*!
    \exception std::invalid_argument
    Thrown if \p c is an inequality.
  */
  explicit Congruence(const Constraint& c,
                      Representation r = default_representation);

  //! Destructor.
  ~Congruence();

  //! Assignment operator.
  Congruence& operator=(const Congruence& y);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Congruence can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  void permute_space_dimensions(const std::vector<Variable>& cycles);

  //! The type of the (adapted) internal expression.
  typedef Expression_Adapter_Transparent<Linear_Expression> expr_type;
  //! Partial read access to the (adapted) internal expression.
  expr_type expression() const;

  //! Returns the coefficient of \p v in \p *this.
  /*!
    \exception std::invalid_argument thrown if the index of \p v
    is greater than or equal to the space dimension of \p *this.
  */
  Coefficient_traits::const_reference coefficient(Variable v) const;

  //! Returns the inhomogeneous term of \p *this.
  Coefficient_traits::const_reference inhomogeneous_term() const;

  //! Returns a const reference to the modulus of \p *this.
  Coefficient_traits::const_reference modulus() const;

  //! Sets the modulus of \p *this to \p m .
  //! If \p m is 0, the congruence becomes an equality.
  void set_modulus(Coefficient_traits::const_reference m);

  //! Multiplies all the coefficients, including the modulus, by \p factor .
  void scale(Coefficient_traits::const_reference factor);

  // TODO: Document this.
  void affine_preimage(Variable v,
                       const Linear_Expression& expr,
                       Coefficient_traits::const_reference denominator);

  //! Multiplies \p k into the modulus of \p *this.
  /*!
    If called with \p *this representing the congruence \f$ e_1 = e_2
    \pmod{m}\f$, then it returns with *this representing
    the congruence \f$ e_1 = e_2 \pmod{mk}\f$.
  */
  Congruence&
  operator/=(Coefficient_traits::const_reference k);

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is a tautology
    (i.e., an always true congruence).

    A tautological congruence has one the following two forms:
    - an equality: \f$\sum_{i=0}^{n-1} 0 x_i + 0 == 0\f$; or
    - a proper congruence: \f$\sum_{i=0}^{n-1} 0 x_i + b \%= 0 / m\f$,
      where \f$b = 0 \pmod{m}\f$.
  */
  bool is_tautological() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this is inconsistent (i.e., an always false congruence).

    An inconsistent congruence has one of the following two forms:
    - an equality: \f$\sum_{i=0}^{n-1} 0 x_i + b == 0\f$
      where \f$b \neq 0\f$; or
    - a proper congruence: \f$\sum_{i=0}^{n-1} 0 x_i + b \%= 0 / m\f$,
      where \f$b \neq 0 \pmod{m}\f$.
  */
  bool is_inconsistent() const;

  //! Returns <CODE>true</CODE> if the modulus is greater than zero.
  /*!
    A congruence with a modulus of 0 is a linear equality.
  */
  bool is_proper_congruence() const;

  //! Returns <CODE>true</CODE> if \p *this is an equality.
  /*!
    A modulus of zero denotes a linear equality.
  */
  bool is_equality() const;

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  /*! \brief
    Returns a reference to the true (zero-dimension space) congruence
    \f$0 = 1 \pmod{1}\f$, also known as the <EM>integrality
    congruence</EM>.
  */
  static const Congruence& zero_dim_integrality();

  /*! \brief
    Returns a reference to the false (zero-dimension space) congruence
    \f$0 = 1 \pmod{0}\f$.
  */
  static const Congruence& zero_dim_false();

  //! Returns the congruence \f$e1 = e2 \pmod{1}\f$.
  static Congruence
  create(const Linear_Expression& e1, const Linear_Expression& e2,
         Representation r = default_representation);

  //! Returns the congruence \f$e = n \pmod{1}\f$.
  static Congruence
  create(const Linear_Expression& e, Coefficient_traits::const_reference n,
         Representation r = default_representation);

  //! Returns the congruence \f$n = e \pmod{1}\f$.
  static Congruence
  create(Coefficient_traits::const_reference n, const Linear_Expression& e,
         Representation r = default_representation);

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
    Loads from \p s an ASCII representation of the internal
    representation of \p *this.
  */
  bool ascii_load(std::istream& s);

  //! Swaps \p *this with \p y.
  void m_swap(Congruence& y);

  //! Copy-constructs with the specified space dimension.
  /*!
    \note The new Congruence will have the same representation as `cg',
          not default_representation, for consistency with the copy
          constructor.
  */
  Congruence(const Congruence& cg, dimension_type new_space_dimension);

  //! Copy-constructs with the specified space dimension and representation.
  Congruence(const Congruence& cg, dimension_type new_space_dimension,
             Representation r);

  //! Copy-constructs from a constraint, with the specified space dimension
  //! and (optional) representation.
  Congruence(const Constraint& cg, dimension_type new_space_dimension,
             Representation r = default_representation);

  //! Constructs from Linear_Expression \p le, using modulus \p m.
  /*!
    Builds a congruence with modulus \p m, stealing the coefficients
    from \p le.

    \note The new Congruence will have the same representation as `le'.

    \param le
    The Linear_Expression holding the coefficients.

    \param m
    The modulus for the congruence, which must be zero or greater.
  */
  Congruence(Linear_Expression& le,
             Coefficient_traits::const_reference m, Recycle_Input);

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  void swap_space_dimensions(Variable v1, Variable v2);

  //! Sets the space dimension by \p n , adding or removing coefficients as
  //! needed.
  void set_space_dimension(dimension_type n);

  //! Shift by \p n positions the coefficients of variables, starting from
  //! the coefficient of \p v. This increases the space dimension by \p n.
  void shift_space_dimensions(Variable v, dimension_type n);

  //! Normalizes the signs.
  /*!
    The signs of the coefficients and the inhomogeneous term are
    normalized, leaving the first non-zero homogeneous coefficient
    positive.
  */
  void sign_normalize();

  //! Normalizes signs and the inhomogeneous term.
  /*!
    Applies sign_normalize, then reduces the inhomogeneous term to the
    smallest possible positive number.
  */
  void normalize();

  //! Calls normalize, then divides out common factors.
  /*!
    Strongly normalized Congruences have equivalent semantics if and
    only if they have the same syntax (as output by operator<<).
  */
  void strong_normalize();

private:
  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the false (zero-dimension space) congruence \f$0 = 1 \pmod{0}\f$.
  */
  static const Congruence* zero_dim_false_p;

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the true (zero-dimension space) congruence \f$0 = 1 \pmod{1}\f$,
    also known as the <EM>integrality congruence</EM>.
  */
  static const Congruence* zero_dim_integrality_p;

  Linear_Expression expr;

  Coefficient modulus_;

  /*! \brief
    Returns <CODE>true</CODE> if \p *this is equal to \p cg in
    dimension \p v.
  */
  bool is_equal_at_dimension(Variable v,
                             const Congruence& cg) const;

  /*! \brief
    Throws a <CODE>std::invalid_argument</CODE> exception containing
    error message \p message.
  */
  void
  throw_invalid_argument(const char* method, const char* message) const;

  /*! \brief
    Throws a <CODE>std::invalid_argument</CODE> exception containing
    the appropriate error message.
  */
  void
  throw_dimension_incompatible(const char* method,
                               const char* v_name,
                               Variable v) const;

  friend bool
  operator==(const Congruence& x, const Congruence& y);

  friend bool
  operator!=(const Congruence& x, const Congruence& y);

  friend class Scalar_Products;
  friend class Grid;
};

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operators.

/*! \relates Parma_Polyhedra_Library::Congruence */
std::ostream&
operator<<(std::ostream& s, const Congruence& c);

} // namespace IO_Operators

//! Returns the congruence \f$e1 = e2 \pmod{1}\f$.
/*! \relates Congruence */
Congruence
operator%=(const Linear_Expression& e1, const Linear_Expression& e2);

//! Returns the congruence \f$e = n \pmod{1}\f$.
/*! \relates Congruence */
Congruence
operator%=(const Linear_Expression& e, Coefficient_traits::const_reference n);

//! Returns a copy of \p cg, multiplying \p k into the copy's modulus.
/*!
    If \p cg represents the congruence \f$ e_1 = e_2
    \pmod{m}\f$, then the result represents the
    congruence \f$ e_1 = e_2 \pmod{mk}\f$.
  \relates Congruence
*/
Congruence
operator/(const Congruence& cg, Coefficient_traits::const_reference k);

//! Creates a congruence from \p c, with \p m as the modulus.
/*! \relates Congruence */
Congruence
operator/(const Constraint& c, Coefficient_traits::const_reference m);

/*! \relates Congruence */
void
swap(Congruence& x, Congruence& y);

} // namespace Parma_Polyhedra_Library

#include "Congruence_inlines.hh"

#endif // !defined(PPL_Congruence_defs_hh)
