/* Generator class declaration.
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

#ifndef PPL_Generator_defs_hh
#define PPL_Generator_defs_hh 1

#include "Generator_types.hh"
#include "Scalar_Products_types.hh"
#include "Variables_Set_types.hh"
#include "Constraint_System_types.hh"
#include "Generator_System_types.hh"
#include "Congruence_System_types.hh"
#include "Polyhedron_types.hh"
#include "Grid_Generator_types.hh"
#include "Grid_Generator_System_types.hh"
#include "MIP_Problem_types.hh"
#include "Grid_types.hh"

#include "Variable_defs.hh"
#include "Linear_Expression_defs.hh"
#include "Checked_Number_defs.hh"
#include "distances_defs.hh"
#include "Topology_types.hh"
#include "Expression_Hide_Last_defs.hh"
#include "Expression_Hide_Inhomo_defs.hh"

#include <iosfwd>

namespace Parma_Polyhedra_Library {

// Put them in the namespace here to declare them friend later.

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The basic comparison function.
/*! \relates Generator
  \return
  The returned absolute value can be \f$0\f$, \f$1\f$ or \f$2\f$.

  \param x
  A row of coefficients;

  \param y
  Another row.

  Compares \p x and \p y, where \p x and \p y may be of different size,
  in which case the "missing" coefficients are assumed to be zero.
  The comparison is such that:
  -# equalities are smaller than inequalities;
  -# lines are smaller than points and rays;
  -# the ordering is lexicographic;
  -# the positions compared are, in decreasing order of significance,
     1, 2, ..., \p size(), 0;
  -# the result is negative, zero, or positive if x is smaller than,
     equal to, or greater than y, respectively;
  -# when \p x and \p y are different, the absolute value of the
     result is 1 if the difference is due to the coefficient in
     position 0; it is 2 otherwise.

  When \p x and \p y represent the hyper-planes associated
  to two equality or inequality constraints, the coefficient
  at 0 is the known term.
  In this case, the return value can be characterized as follows:
  - -2, if \p x is smaller than \p y and they are \e not parallel;
  - -1, if \p x is smaller than \p y and they \e are parallel;
  -  0, if \p x and y are equal;
  - +1, if \p y is smaller than \p x and they \e are parallel;
  - +2, if \p y is smaller than \p x and they are \e not parallel.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
int compare(const Generator& x, const Generator& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Generator */
std::ostream& operator<<(std::ostream& s, const Generator& g);

} // namespace IO_Operators

//! Swaps \p x with \p y.
/*! \relates Generator */
void swap(Generator& x, Generator& y);

} // namespace Parma_Polyhedra_Library


//! A line, ray, point or closure point.
/*! \ingroup PPL_CXX_interface
  An object of the class Generator is one of the following:

  - a line \f$\vect{l} = (a_0, \ldots, a_{n-1})^\transpose\f$;

  - a ray \f$\vect{r} = (a_0, \ldots, a_{n-1})^\transpose\f$;

  - a point
    \f$\vect{p} = (\frac{a_0}{d}, \ldots, \frac{a_{n-1}}{d})^\transpose\f$;

  - a closure point
    \f$\vect{c} = (\frac{a_0}{d}, \ldots, \frac{a_{n-1}}{d})^\transpose\f$;

  where \f$n\f$ is the dimension of the space
  and, for points and closure points, \f$d > 0\f$ is the divisor.

  \par A note on terminology.
  As observed in Section \ref representation, there are cases when,
  in order to represent a polyhedron \f$\cP\f$ using the generator system
  \f$\cG = (L, R, P, C)\f$, we need to include in the finite set
  \f$P\f$ even points of \f$\cP\f$ that are <EM>not</EM> vertices
  of \f$\cP\f$.
  This situation is even more frequent when working with NNC polyhedra
  and it is the reason why we prefer to use the word `point'
  where other libraries use the word `vertex'.

  \par How to build a generator.
  Each type of generator is built by applying the corresponding
  function (<CODE>line</CODE>, <CODE>ray</CODE>, <CODE>point</CODE>
  or <CODE>closure_point</CODE>) to a linear expression,
  representing a direction in the space;
  the space dimension of the generator is defined as the space dimension
  of the corresponding linear expression.
  Linear expressions used to define a generator should be homogeneous
  (any constant term will be simply ignored).
  When defining points and closure points, an optional Coefficient argument
  can be used as a common <EM>divisor</EM> for all the coefficients
  occurring in the provided linear expression;
  the default value for this argument is 1.

  \par
  In all the following examples it is assumed that variables
  <CODE>x</CODE>, <CODE>y</CODE> and <CODE>z</CODE>
  are defined as follows:
  \code
  Variable x(0);
  Variable y(1);
  Variable z(2);
  \endcode

  \par Example 1
  The following code builds a line with direction \f$x-y-z\f$
  and having space dimension \f$3\f$:
  \code
  Generator l = line(x - y - z);
  \endcode
  As mentioned above, the constant term of the linear expression
  is not relevant. Thus, the following code has the same effect:
  \code
  Generator l = line(x - y - z + 15);
  \endcode
  By definition, the origin of the space is not a line, so that
  the following code throws an exception:
  \code
  Generator l = line(0*x);
  \endcode

  \par Example 2
  The following code builds a ray with the same direction as the
  line in Example 1:
  \code
  Generator r = ray(x - y - z);
  \endcode
  As is the case for lines, when specifying a ray the constant term
  of the linear expression is not relevant; also, an exception is thrown
  when trying to build a ray from the origin of the space.

  \par Example 3
  The following code builds the point
  \f$\vect{p} = (1, 0, 2)^\transpose \in \Rset^3\f$:
  \code
  Generator p = point(1*x + 0*y + 2*z);
  \endcode
  The same effect can be obtained by using the following code:
  \code
  Generator p = point(x + 2*z);
  \endcode
  Similarly, the origin \f$\vect{0} \in \Rset^3\f$ can be defined
  using either one of the following lines of code:
  \code
  Generator origin3 = point(0*x + 0*y + 0*z);
  Generator origin3_alt = point(0*z);
  \endcode
  Note however that the following code would have defined
  a different point, namely \f$\vect{0} \in \Rset^2\f$:
  \code
  Generator origin2 = point(0*y);
  \endcode
  The following two lines of code both define the only point
  having space dimension zero, namely \f$\vect{0} \in \Rset^0\f$.
  In the second case we exploit the fact that the first argument
  of the function <CODE>point</CODE> is optional.
  \code
  Generator origin0 = Generator::zero_dim_point();
  Generator origin0_alt = point();
  \endcode

  \par Example 4
  The point \f$\vect{p}\f$ specified in Example 3 above
  can also be obtained with the following code,
  where we provide a non-default value for the second argument
  of the function <CODE>point</CODE> (the divisor):
  \code
  Generator p = point(2*x + 0*y + 4*z, 2);
  \endcode
  Obviously, the divisor can be usefully exploited to specify
  points having some non-integer (but rational) coordinates.
  For instance, the point
  \f$\vect{q} = (-1.5, 3.2, 2.1)^\transpose \in \Rset^3\f$
  can be specified by the following code:
  \code
  Generator q = point(-15*x + 32*y + 21*z, 10);
  \endcode
  If a zero divisor is provided, an exception is thrown.

  \par Example 5
  Closure points are specified in the same way we defined points,
  but invoking their specific constructor function.
  For instance, the closure point
  \f$\vect{c} = (1, 0, 2)^\transpose \in \Rset^3\f$ is defined by
  \code
  Generator c = closure_point(1*x + 0*y + 2*z);
  \endcode
  For the particular case of the (only) closure point
  having space dimension zero, we can use any of the following:
  \code
  Generator closure_origin0 = Generator::zero_dim_closure_point();
  Generator closure_origin0_alt = closure_point();
  \endcode

  \par How to inspect a generator
  Several methods are provided to examine a generator and extract
  all the encoded information: its space dimension, its type and
  the value of its integer coefficients.

  \par Example 6
  The following code shows how it is possible to access each single
  coefficient of a generator.
  If <CODE>g1</CODE> is a point having coordinates
  \f$(a_0, \ldots, a_{n-1})^\transpose\f$,
  we construct the closure point <CODE>g2</CODE> having coordinates
  \f$(a_0, 2 a_1, \ldots, (i+1)a_i, \ldots, n a_{n-1})^\transpose\f$.
  \code
  if (g1.is_point()) {
    cout << "Point g1: " << g1 << endl;
    Linear_Expression e;
    for (dimension_type i = g1.space_dimension(); i-- > 0; )
      e += (i + 1) * g1.coefficient(Variable(i)) * Variable(i);
    Generator g2 = closure_point(e, g1.divisor());
    cout << "Closure point g2: " << g2 << endl;
  }
  else
    cout << "Generator g1 is not a point." << endl;
  \endcode
  Therefore, for the point
  \code
  Generator g1 = point(2*x - y + 3*z, 2);
  \endcode
  we would obtain the following output:
  \code
  Point g1: p((2*A - B + 3*C)/2)
  Closure point g2: cp((2*A - 2*B + 9*C)/2)
  \endcode
  When working with (closure) points, be careful not to confuse
  the notion of <EM>coefficient</EM> with the notion of <EM>coordinate</EM>:
  these are equivalent only when the divisor of the (closure) point is 1.
*/
class Parma_Polyhedra_Library::Generator {
public:

  //! The representation used for new Generators.
  /*!
    \note The copy constructor and the copy constructor with specified size
          use the representation of the original object, so that it is
          indistinguishable from the original object.
  */
  static const Representation default_representation = SPARSE;

  //! Returns the line of direction \p e.
  /*!
    \exception std::invalid_argument
    Thrown if the homogeneous part of \p e represents the origin of
    the vector space.
  */
  static Generator line(const Linear_Expression& e,
                        Representation r = default_representation);

  //! Returns the ray of direction \p e.
  /*!
    \exception std::invalid_argument
    Thrown if the homogeneous part of \p e represents the origin of
    the vector space.
  */
  static Generator ray(const Linear_Expression& e,
                       Representation r = default_representation);

  //! Returns the point at \p e / \p d.
  /*!
    Both \p e and \p d are optional arguments, with default values
    Linear_Expression::zero() and Coefficient_one(), respectively.

    \exception std::invalid_argument
    Thrown if \p d is zero.
  */
  static Generator point(const Linear_Expression& e
                         = Linear_Expression::zero(),
                         Coefficient_traits::const_reference d
                         = Coefficient_one(),
                         Representation r = default_representation);

  //! Returns the origin.
  static Generator point(Representation r);

  //! Returns the point at \p e.
  static Generator point(const Linear_Expression& e,
                         Representation r);

  //! Constructs the point at the origin.
  explicit Generator(Representation r = default_representation);

  //! Returns the closure point at \p e / \p d.
  /*!
    Both \p e and \p d are optional arguments, with default values
    Linear_Expression::zero() and Coefficient_one(), respectively.

    \exception std::invalid_argument
    Thrown if \p d is zero.
  */
  static Generator
  closure_point(const Linear_Expression& e = Linear_Expression::zero(),
                Coefficient_traits::const_reference d = Coefficient_one(),
                Representation r = default_representation);

  //! Returns the closure point at the origin.
  static Generator
  closure_point(Representation r);

  //! Returns the closure point at \p e.
  static Generator
  closure_point(const Linear_Expression& e, Representation r);

  //! Ordinary copy constructor.
  //! The representation of the new Generator will be the same as g.
  Generator(const Generator& g);

  //! Copy constructor with given representation.
  Generator(const Generator& g, Representation r);

  //! Copy constructor with given space dimension.
  //! The representation of the new Generator will be the same as g.
  Generator(const Generator& g, dimension_type space_dim);

  //! Copy constructor with given representation and space dimension.
  Generator(const Generator& g, dimension_type space_dim, Representation r);

  //! Destructor.
  ~Generator();

  //! Assignment operator.
  Generator& operator=(const Generator& g);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Generator can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Sets the dimension of the vector space enclosing \p *this to
  //! \p space_dim .
  void set_space_dimension(dimension_type space_dim);

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  void swap_space_dimensions(Variable v1, Variable v2);

  //! Removes all the specified dimensions from the generator.
  /*!
    The space dimension of the variable with the highest space
    dimension in \p vars must be at most the space dimension
    of \p this.

    If all dimensions with nonzero coefficients are removed from a ray or a
    line, it is changed into a point and this method returns \p false .
    Otherwise, it returns \p true .
  */
  bool remove_space_dimensions(const Variables_Set& vars);

  //! Permutes the space dimensions of the generator.
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

  //! Shift by \p n positions the coefficients of variables, starting from
  //! the coefficient of \p v. This increases the space dimension by \p n.
  void shift_space_dimensions(Variable v, dimension_type n);

  //! The generator type.
  enum Type {
    /*! The generator is a line. */
    LINE,
    /*! The generator is a ray. */
    RAY,
    /*! The generator is a point. */
    POINT,
    /*! The generator is a closure point. */
    CLOSURE_POINT
  };

  //! Returns the generator type of \p *this.
  Type type() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a line.
  bool is_line() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a ray.
  bool is_ray() const;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! Returns <CODE>true</CODE> if and only if \p *this is a line or a ray.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool is_line_or_ray() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a point.
  bool is_point() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a closure point.
  bool is_closure_point() const;

  //! Returns the coefficient of \p v in \p *this.
  /*!
    \exception std::invalid_argument
    Thrown if the index of \p v is greater than or equal to the
    space dimension of \p *this.
  */
  Coefficient_traits::const_reference coefficient(Variable v) const;

  //! If \p *this is either a point or a closure point, returns its divisor.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this is neither a point nor a closure point.
  */
  Coefficient_traits::const_reference divisor() const;

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  //! Returns the origin of the zero-dimensional space \f$\Rset^0\f$.
  static const Generator& zero_dim_point();

  /*! \brief
    Returns, as a closure point,
    the origin of the zero-dimensional space \f$\Rset^0\f$.
  */
  static const Generator& zero_dim_closure_point();

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this and \p y
    are equivalent generators.

    Generators having different space dimensions are not equivalent.
  */
  bool is_equivalent_to(const Generator& y) const;

  //! Returns <CODE>true</CODE> if \p *this is identical to \p y.
  /*!
    This is faster than is_equivalent_to(), but it may return `false' even
    for equivalent generators.
  */
  bool is_equal_to(const Generator& y) const;

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
  void m_swap(Generator& y);

  //! The type of the (adapted) internal expression.
  typedef Expression_Hide_Last<Expression_Hide_Inhomo<Linear_Expression> >
  expr_type;
  //! Partial read access to the (adapted) internal expression.
  expr_type expression() const;

private:
  //! The possible kinds of Generator objects.
  enum Kind {
    LINE_OR_EQUALITY = 0,
    RAY_OR_POINT_OR_INEQUALITY = 1
  };

  //! The linear expression encoding \p *this.
  Linear_Expression expr;

  //! The kind of \p *this.
  Kind kind_;

  //! The topology of \p *this.
  Topology topology_;

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the origin of the zero-dimensional space \f$\Rset^0\f$.
  */
  static const Generator* zero_dim_point_p;

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the origin of the zero-dimensional space \f$\Rset^0\f$, as a closure point.
  */
  static const Generator* zero_dim_closure_point_p;

  /*! \brief
    Builds a generator of type \p type and topology \p topology,
    stealing the coefficients from \p e.

    If the topology is NNC, the last dimension of \p e is used as the epsilon
    coefficient.
  */
  Generator(Linear_Expression& e, Type type, Topology topology);

  Generator(Linear_Expression& e, Kind kind, Topology topology);

  Generator(dimension_type space_dim, Kind kind, Topology topology,
            Representation r = default_representation);

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this row
    represents a line or an equality.
  */
  bool is_line_or_equality() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this row
    represents a ray, a point or an inequality.
  */
  bool is_ray_or_point_or_inequality() const;

  //! Sets to \p LINE_OR_EQUALITY the kind of \p *this row.
  void set_is_line_or_equality();

  //! Sets to \p RAY_OR_POINT_OR_INEQUALITY the kind of \p *this row.
  void set_is_ray_or_point_or_inequality();

  //! \name Flags inspection methods
  //@{
  //! Returns the topological kind of \p *this.
  Topology topology() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if the topology
    of \p *this row is not necessarily closed.
  */
  bool is_not_necessarily_closed() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if the topology
    of \p *this row is necessarily closed.
  */
  bool is_necessarily_closed() const;
  //@} // Flags inspection methods

  //! \name Flags coercion methods
  //@{

  //! Sets to \p x the topological kind of \p *this row.
  void set_topology(Topology x);

  //! Sets to \p NECESSARILY_CLOSED the topological kind of \p *this row.
  void set_necessarily_closed();

  //! Sets to \p NOT_NECESSARILY_CLOSED the topological kind of \p *this row.
  void set_not_necessarily_closed();
  //@} // Flags coercion methods

  //! Marks the epsilon dimension as a standard dimension.
  /*!
    The row topology is changed to <CODE>NECESSARILY_CLOSED</CODE>, and
    the number of space dimensions is increased by 1.
  */
  void mark_as_necessarily_closed();

  //! Marks the last dimension as the epsilon dimension.
  /*!
    The row topology is changed to <CODE>NOT_NECESSARILY_CLOSED</CODE>, and
    the number of space dimensions is decreased by 1.
  */
  void mark_as_not_necessarily_closed();

  //! Linearly combines \p *this with \p y so that i-th coefficient is 0.
  /*!
    \param y
    The Generator that will be combined with \p *this object;

    \param i
    The index of the coefficient that has to become \f$0\f$.

    Computes a linear combination of \p *this and \p y having
    the i-th coefficient equal to \f$0\f$. Then it assigns
    the resulting Generator to \p *this and normalizes it.
  */
  void linear_combine(const Generator& y, dimension_type i);

  //! Sets the dimension of the vector space enclosing \p *this to
  //! \p space_dim .
  //! Sets the space dimension of the rows in the system to \p space_dim .
  /*!
    This method is for internal use, it does *not* assert OK() at the end,
    so it can be used for invalid objects.
  */
  void set_space_dimension_no_ok(dimension_type space_dim);

  /*! \brief
    Throw a <CODE>std::invalid_argument</CODE> exception
    containing the appropriate error message.
  */
  void
  throw_dimension_incompatible(const char* method,
                               const char* v_name,
                               Variable v) const;

  /*! \brief
    Throw a <CODE>std::invalid_argument</CODE> exception
    containing the appropriate error message.
  */
  void
  throw_invalid_argument(const char* method, const char* reason) const;

  //! Returns <CODE>true</CODE> if and only if \p *this is not a line.
  bool is_ray_or_point() const;

  //! Sets the Generator kind to <CODE>LINE_OR_EQUALITY</CODE>.
  void set_is_line();

  //! Sets the Generator kind to <CODE>RAY_OR_POINT_OR_INEQUALITY</CODE>.
  void set_is_ray_or_point();

  /*! \brief
    Returns <CODE>true</CODE> if and only if the closure point
    \p *this has the same \e coordinates of the point \p p.

    It is \e assumed that \p *this is a closure point, \p p is a point
    and both topologies and space dimensions agree.
  */
  bool is_matching_closure_point(const Generator& p) const;

  //! Returns the epsilon coefficient. The generator must be NNC.
  Coefficient_traits::const_reference epsilon_coefficient() const;

  //! Sets the epsilon coefficient to \p n. The generator must be NNC.
  void set_epsilon_coefficient(Coefficient_traits::const_reference n);

  /*! \brief
    Normalizes the sign of the coefficients so that the first non-zero
    (homogeneous) coefficient of a line-or-equality is positive.
  */
  void sign_normalize();

  /*! \brief
    Strong normalization: ensures that different Generator objects
    represent different hyperplanes or hyperspaces.

    Applies both Generator::normalize() and Generator::sign_normalize().
  */
  void strong_normalize();

  /*! \brief
    Returns <CODE>true</CODE> if and only if the coefficients are
    strongly normalized.
  */
  bool check_strong_normalized() const;

  /*! \brief
    A print function, with fancy, more human-friendly output.

    This is used by operator<<().
  */
  void fancy_print(std::ostream& s) const;

  friend class Expression_Adapter<Generator>;
  friend class Linear_System<Generator>;
  friend class Parma_Polyhedra_Library::Scalar_Products;
  friend class Parma_Polyhedra_Library::Topology_Adjusted_Scalar_Product_Sign;
  friend class Parma_Polyhedra_Library::Topology_Adjusted_Scalar_Product_Assign;
  friend class Parma_Polyhedra_Library::Generator_System;
  friend class Parma_Polyhedra_Library::Generator_System_const_iterator;
  // FIXME: the following friend declaration should be avoided.
  friend class Parma_Polyhedra_Library::Polyhedron;
  // This is for access to Linear_Expression in `insert'.
  friend class Parma_Polyhedra_Library::Grid_Generator_System;
  friend class Parma_Polyhedra_Library::MIP_Problem;
  friend class Parma_Polyhedra_Library::Grid;

  friend std::ostream&
  Parma_Polyhedra_Library::IO_Operators::operator<<(std::ostream& s,
                                                    const Generator& g);

  friend int
  compare(const Generator& x, const Generator& y);
};


namespace Parma_Polyhedra_Library {

//! Shorthand for Generator::line(const Linear_Expression& e, Representation r).
/*! \relates Generator */
Generator line(const Linear_Expression& e,
               Representation r = Generator::default_representation);

//! Shorthand for Generator::ray(const Linear_Expression& e, Representation r).
/*! \relates Generator */
Generator ray(const Linear_Expression& e,
              Representation r = Generator::default_representation);

/*! \brief
  Shorthand for
  Generator::point(const Linear_Expression& e, Coefficient_traits::const_reference d, Representation r).

  \relates Generator
*/
Generator
point(const Linear_Expression& e = Linear_Expression::zero(),
      Coefficient_traits::const_reference d = Coefficient_one(),
      Representation r = Generator::default_representation);

//! Shorthand for Generator::point(Representation r).
/*! \relates Generator */
Generator
point(Representation r);

/*! \brief
  Shorthand for
  Generator::point(const Linear_Expression& e, Representation r).

  \relates Generator
*/
Generator
point(const Linear_Expression& e, Representation r);

/*! \brief
  Shorthand for
  Generator::closure_point(const Linear_Expression& e, Coefficient_traits::const_reference d, Representation r).

  \relates Generator
*/
Generator
closure_point(const Linear_Expression& e = Linear_Expression::zero(),
              Coefficient_traits::const_reference d = Coefficient_one(),
              Representation r = Generator::default_representation);

//! Shorthand for Generator::closure_point(Representation r).
/*! \relates Generator */
Generator
closure_point(Representation r);

/*! \brief
  Shorthand for
  Generator::closure_point(const Linear_Expression& e, Representation r).

  \relates Generator
*/
Generator
closure_point(const Linear_Expression& e, Representation r);

//! Returns <CODE>true</CODE> if and only if \p x is equivalent to \p y.
/*! \relates Generator */
bool operator==(const Generator& x, const Generator& y);

//! Returns <CODE>true</CODE> if and only if \p x is not equivalent to \p y.
/*! \relates Generator */
bool operator!=(const Generator& x, const Generator& y);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates Generator
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename To>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const Generator& x,
                                 const Generator& y,
                                 Rounding_Dir dir);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates Generator
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename Temp, typename To>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const Generator& x,
                                 const Generator& y,
                                 Rounding_Dir dir);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates Generator
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename Temp, typename To>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const Generator& x,
                                 const Generator& y,
                                 Rounding_Dir dir,
                                 Temp& tmp0,
                                 Temp& tmp1,
                                 Temp& tmp2);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates Generator
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename To>
bool euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                               const Generator& x,
                               const Generator& y,
                               Rounding_Dir dir);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates Generator
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename Temp, typename To>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const Generator& x,
                                 const Generator& y,
                                 Rounding_Dir dir);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates Generator
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename Temp, typename To>
bool euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                               const Generator& x,
                               const Generator& y,
                               Rounding_Dir dir,
                               Temp& tmp0,
                               Temp& tmp1,
                               Temp& tmp2);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates Generator
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename To>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const Generator& x,
                                const Generator& y,
                                Rounding_Dir dir);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates Generator
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename Temp, typename To>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const Generator& x,
                                const Generator& y,
                                Rounding_Dir dir);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates Generator
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.

  \note
  Distances are \e only defined between generators that are points and/or
  closure points; for rays or lines, \c false is returned.
*/
template <typename Temp, typename To>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const Generator& x,
                                const Generator& y,
                                Rounding_Dir dir,
                                Temp& tmp0,
                                Temp& tmp1,
                                Temp& tmp2);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Generator */
std::ostream& operator<<(std::ostream& s, const Generator::Type& t);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

#include "Generator_inlines.hh"

#endif // !defined(PPL_Generator_defs_hh)
