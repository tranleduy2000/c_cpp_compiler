/* Grid_Generator class declaration.
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

#ifndef PPL_Grid_Generator_defs_hh
#define PPL_Grid_Generator_defs_hh 1

#include "Grid_Generator_types.hh"
#include "Grid_types.hh"

#include "Variables_Set_types.hh"
#include "Grid_Generator_System_types.hh"
#include "Linear_System_types.hh"

#include "Coefficient_defs.hh"
#include "Linear_Expression_defs.hh"
#include "Topology_types.hh"
#include "Expression_Hide_Inhomo_defs.hh"
#include "Expression_Hide_Last_defs.hh"

#include "Grid_types.hh"
#include <iosfwd>

namespace Parma_Polyhedra_Library {

// Put these in the namespace here to declare them friend later.

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The basic comparison function.
/*! \relates Grid_Generator
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
int compare(const Grid_Generator& x, const Grid_Generator& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Grid_Generator */
std::ostream& operator<<(std::ostream& s, const Grid_Generator& g);

} // namespace IO_Operators

//! Swaps \p x with \p y.
/*! \relates Grid_Generator */
void swap(Grid_Generator& x, Grid_Generator& y);

} // namespace Parma_Polyhedra_Library

//! A grid line, parameter or grid point.
/*! \ingroup PPL_CXX_interface
  An object of the class Grid_Generator is one of the following:

  - a grid_line \f$\vect{l} = (a_0, \ldots, a_{n-1})^\transpose\f$;

  - a parameter
    \f$\vect{q} = (\frac{a_0}{d}, \ldots, \frac{a_{n-1}}{d})^\transpose\f$;

  - a grid_point
    \f$\vect{p} = (\frac{a_0}{d}, \ldots, \frac{a_{n-1}}{d})^\transpose\f$;

  where \f$n\f$ is the dimension of the space
  and, for grid_points and parameters, \f$d > 0\f$ is the divisor.

  \par How to build a grid generator.
  Each type of generator is built by applying the corresponding
  function (<CODE>grid_line</CODE>, <CODE>parameter</CODE>
  or <CODE>grid_point</CODE>) to a linear expression;
  the space dimension of the generator is defined as the space dimension
  of the corresponding linear expression.
  Linear expressions used to define a generator should be homogeneous
  (any constant term will be simply ignored).
  When defining grid points and parameters, an optional Coefficient argument
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
  The following code builds a grid line with direction \f$x-y-z\f$
  and having space dimension \f$3\f$:
  \code
  Grid_Generator l = grid_line(x - y - z);
  \endcode
  By definition, the origin of the space is not a line, so that
  the following code throws an exception:
  \code
  Grid_Generator l = grid_line(0*x);
  \endcode

  \par Example 2
  The following code builds the parameter as the vector
  \f$\vect{p} = (1, -1, -1)^\transpose \in \Rset^3\f$
  which has the same direction as the line in Example 1:
  \code
  Grid_Generator q = parameter(x - y - z);
  \endcode
  Note that, unlike lines, for parameters, the length as well
  as the direction of the vector represented by the code is significant.
  Thus \p q is \e not the same as the parameter \p q1 defined by
  \code
  Grid_Generator q1 = parameter(2x - 2y - 2z);
  \endcode
  By definition, the origin of the space is not a parameter, so that
  the following code throws an exception:
  \code
  Grid_Generator q = parameter(0*x);
  \endcode

  \par Example 3
  The following code builds the grid point
  \f$\vect{p} = (1, 0, 2)^\transpose \in \Rset^3\f$:
  \code
  Grid_Generator p = grid_point(1*x + 0*y + 2*z);
  \endcode
  The same effect can be obtained by using the following code:
  \code
  Grid_Generator p = grid_point(x + 2*z);
  \endcode
  Similarly, the origin \f$\vect{0} \in \Rset^3\f$ can be defined
  using either one of the following lines of code:
  \code
  Grid_Generator origin3 = grid_point(0*x + 0*y + 0*z);
  Grid_Generator origin3_alt = grid_point(0*z);
  \endcode
  Note however that the following code would have defined
  a different point, namely \f$\vect{0} \in \Rset^2\f$:
  \code
  Grid_Generator origin2 = grid_point(0*y);
  \endcode
  The following two lines of code both define the only grid point
  having space dimension zero, namely \f$\vect{0} \in \Rset^0\f$.
  In the second case we exploit the fact that the first argument
  of the function <CODE>point</CODE> is optional.
  \code
  Grid_Generator origin0 = Generator::zero_dim_point();
  Grid_Generator origin0_alt = grid_point();
  \endcode

  \par Example 4
  The grid point \f$\vect{p}\f$ specified in Example 3 above
  can also be obtained with the following code,
  where we provide a non-default value for the second argument
  of the function <CODE>grid_point</CODE> (the divisor):
  \code
  Grid_Generator p = grid_point(2*x + 0*y + 4*z, 2);
  \endcode
  Obviously, the divisor can be used to specify
  points having some non-integer (but rational) coordinates.
  For instance, the grid point
  \f$\vect{p1} = (-1.5, 3.2, 2.1)^\transpose \in \Rset^3\f$
  can be specified by the following code:
  \code
  Grid_Generator p1 = grid_point(-15*x + 32*y + 21*z, 10);
  \endcode
  If a zero divisor is provided, an exception is thrown.

  \par Example 5
  Parameters, like grid points can have a divisor.
  For instance, the parameter
  \f$\vect{q} = (1, 0, 2)^\transpose \in \Rset^3\f$ can be defined:
  \code
  Grid_Generator q = parameter(2*x + 0*y + 4*z, 2);
  \endcode
  Also, the divisor can be used to specify
  parameters having some non-integer (but rational) coordinates.
  For instance, the parameter
  \f$\vect{q} = (-1.5, 3.2, 2.1)^\transpose \in \Rset^3\f$
  can be defined:
  \code
  Grid_Generator q = parameter(-15*x + 32*y + 21*z, 10);
  \endcode
  If a zero divisor is provided, an exception is thrown.

  \par How to inspect a grid generator
  Several methods are provided to examine a grid generator and extract
  all the encoded information: its space dimension, its type and
  the value of its integer coefficients and the value of the denominator.

  \par Example 6
  The following code shows how it is possible to access each single
  coefficient of a grid generator.
  If <CODE>g1</CODE> is a grid point having coordinates
  \f$(a_0, \ldots, a_{n-1})^\transpose\f$,
  we construct the parameter <CODE>g2</CODE> having coordinates
  \f$(a_0, 2 a_1, \ldots, (i+1)a_i, \ldots, n a_{n-1})^\transpose\f$.
  \code
  if (g1.is_point()) {
    cout << "Grid point g1: " << g1 << endl;
    Linear_Expression e;
    for (dimension_type i = g1.space_dimension(); i-- > 0; )
      e += (i + 1) * g1.coefficient(Variable(i)) * Variable(i);
    Grid_Generator g2 = parameter(e, g1.divisor());
    cout << "Parameter g2: " << g2 << endl;
  }
  else
    cout << "Grid generator g1 is not a grid point." << endl;
  \endcode
  Therefore, for the grid point
  \code
  Grid_Generator g1 = grid_point(2*x - y + 3*z, 2);
  \endcode
  we would obtain the following output:
  \code
  Grid point g1: p((2*A - B + 3*C)/2)
  Parameter g2: parameter((2*A - 2*B + 9*C)/2)
  \endcode
  When working with grid points and parameters, be careful not to confuse
  the notion of <EM>coefficient</EM> with the notion of <EM>coordinate</EM>:
  these are equivalent only when the divisor is 1.
*/
class Parma_Polyhedra_Library::Grid_Generator {
public:

  //! The possible kinds of Grid_Generator objects.
  enum Kind {
    LINE_OR_EQUALITY = 0,
    RAY_OR_POINT_OR_INEQUALITY = 1
  };

  //! The representation used for new Grid_Generators.
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
  static Grid_Generator grid_line(const Linear_Expression& e,
                                  Representation r = default_representation);

  //! Returns the parameter of direction \p e and size \p e/d, with the same
  //! representation as e.
  /*!
    Both \p e and \p d are optional arguments, with default values
    Linear_Expression::zero() and Coefficient_one(), respectively.

    \exception std::invalid_argument
    Thrown if \p d is zero.
  */
  static Grid_Generator parameter(const Linear_Expression& e
                                  = Linear_Expression::zero(),
                                  Coefficient_traits::const_reference d
                                  = Coefficient_one(),
                                  Representation r = default_representation);

  // TODO: Improve the documentation of this method.
  //! Returns the parameter of direction and size \p Linear_Expression::zero() .
  static Grid_Generator parameter(Representation r);

  //! Returns the parameter of direction and size \p e .
  static Grid_Generator parameter(const Linear_Expression& e,
                                  Representation r);

  //! Returns the point at \p e / \p d.
  /*!
    Both \p e and \p d are optional arguments, with default values
    Linear_Expression::zero() and Coefficient_one(), respectively.

    \exception std::invalid_argument
    Thrown if \p d is zero.
  */
  static Grid_Generator grid_point(const Linear_Expression& e
                                   = Linear_Expression::zero(),
                                   Coefficient_traits::const_reference d
                                   = Coefficient_one(),
                                   Representation r = default_representation);

  //! Returns the point at \p e .
  static Grid_Generator grid_point(Representation r);

  //! Returns the point at \p e .
  static Grid_Generator grid_point(const Linear_Expression& e,
                                   Representation r);

  //! Returns the origin of the zero-dimensional space \f$\Rset^0\f$.
  explicit Grid_Generator(Representation r = default_representation);

  //! Ordinary copy constructor.
  //! The new Grid_Generator will have the same representation as g.
  Grid_Generator(const Grid_Generator& g);

  //! Copy constructor with specified representation.
  Grid_Generator(const Grid_Generator& g, Representation r);

  //! Copy constructor with specified space dimension.
  //! The new Grid_Generator will have the same representation as g.
  Grid_Generator(const Grid_Generator& g, dimension_type space_dim);

  //! Copy constructor with specified space dimension and representation.
  Grid_Generator(const Grid_Generator& g, dimension_type space_dim,
                 Representation r);

  //! Destructor.
  ~Grid_Generator();

  //! Assignment operator.
  Grid_Generator& operator=(const Grid_Generator& g);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Grid_Generator can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Sets the dimension of the vector space enclosing \p *this to
  //! \p space_dim .
  void set_space_dimension(dimension_type space_dim);

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  void swap_space_dimensions(Variable v1, Variable v2);

  //! Removes all the specified dimensions from the grid generator.
  /*!
    The space dimension of the variable with the highest space
    dimension in \p vars must be at most the space dimension
    of \p this.

    Always returns \p true. The return value is needed for compatibility with
    the Generator class.
  */
  bool remove_space_dimensions(const Variables_Set& vars);

  //! Permutes the space dimensions of the grid generator.
  /*
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
    /*! The generator is a grid line. */
    LINE,
    /*! The generator is a parameter. */
    PARAMETER,
    /*! The generator is a grid point. */
    POINT
  };

  //! Returns the generator type of \p *this.
  Type type() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a line.
  bool is_line() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a parameter.
  bool is_parameter() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is a line or
    a parameter.
  */
  bool is_line_or_parameter() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a point.
  bool is_point() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this row represents a
    parameter or a point.
  */
  bool is_parameter_or_point() const;

  //! Returns the coefficient of \p v in \p *this.
  /*!
    \exception std::invalid_argument
    Thrown if the index of \p v is greater than or equal to the
    space dimension of \p *this.
  */
  Coefficient_traits::const_reference coefficient(Variable v) const;

  //! Returns the divisor of \p *this.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this is a line.
  */
  Coefficient_traits::const_reference divisor() const;

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  //! Returns the origin of the zero-dimensional space \f$\Rset^0\f$.
  static const Grid_Generator& zero_dim_point();

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this and \p y are
    equivalent generators.

    Generators having different space dimensions are not equivalent.
  */
  bool is_equivalent_to(const Grid_Generator& y) const;

  //! Returns <CODE>true</CODE> if \p *this is identical to \p y.
  /*!
    This is faster than is_equivalent_to(), but it may return `false' even
    for equivalent generators.
  */
  bool is_equal_to(const Grid_Generator& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if all the homogeneous terms
    of \p *this are \f$0\f$.
  */
  bool all_homogeneous_terms_are_zero() const;

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
  void m_swap(Grid_Generator& y);

  /*! \brief
    Scales \p *this to be represented with a divisor of \p d (if
    \*this is a parameter or point). Does nothing at all on lines.

    It is assumed that \p d is a multiple of the current divisor
    and different from zero. The behavior is undefined if the assumption
    does not hold.
  */
  void scale_to_divisor(Coefficient_traits::const_reference d);

  //! Sets the divisor of \p *this to \p d.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this is a line.
  */
  void set_divisor(Coefficient_traits::const_reference d);

  //! The type of the (adapted) internal expression.
  typedef Expression_Hide_Last<Expression_Hide_Inhomo<Linear_Expression> >
  expr_type;
  //! Partial read access to the (adapted) internal expression.
  expr_type expression() const;

private:
  Linear_Expression expr;

  Kind kind_;

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the origin of the zero-dimensional space \f$\Rset^0\f$.
  */
  static const Grid_Generator* zero_dim_point_p;

  //! Constructs a Grid_Generator with the specified space dimension, kind
  //! and topology.
  Grid_Generator(dimension_type space_dim, Kind kind, Topology topology,
                 Representation r = default_representation);

  // TODO: Avoid reducing the space dimension.
  /*! \brief
    Constructs a grid generator of type \p t from linear expression \p e,
    stealing the underlying data structures from \p e.

    The last column in \p e becomes the parameter divisor column of
    the new Grid_Generator.

    \note The new Grid_Generator will have the same representation as `e'.
  */
  Grid_Generator(Linear_Expression& e, Type t);

  //! Sets the dimension of the vector space enclosing \p *this to
  //! \p space_dim .
  //! Sets the space dimension of the rows in the system to \p space_dim .
  /*!
    This method is for internal use, it does *not* assert OK() at the end,
    so it can be used for invalid objects.
  */
  void set_space_dimension_no_ok(dimension_type space_dim);

  /*! \brief
    Returns <CODE>true</CODE> if \p *this is equal to \p gg in
    dimension \p dim.
  */
  bool is_equal_at_dimension(dimension_type dim,
                             const Grid_Generator& gg) const;

  /*! \brief
    A print function, with fancy, more human-friendly output.

    This is used by operator<<().
  */
  void fancy_print(std::ostream& s) const;

  //! Converts the Grid_Generator into a parameter.
  void set_is_parameter();

  //! Sets the Grid_Generator kind to <CODE>LINE_OR_EQUALITY</CODE>.
  void set_is_line();

  //! Sets the Grid_Generator kind to <CODE>RAY_OR_POINT_OR_INEQUALITY</CODE>.
  void set_is_parameter_or_point();

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
  //@} // Flags inspection methods

  //! \name Flags coercion methods
  //@{

  //! Sets to \p x the topological kind of \p *this row.
  void set_topology(Topology x);

  //! Sets to \p NECESSARILY_CLOSED the topological kind of \p *this row.
  void set_necessarily_closed();

  //! Sets to \p NOT_NECESSARILY_CLOSED the topological kind of \p *this row.
  void set_not_necessarily_closed();

  //! Sets to \p LINE_OR_EQUALITY the kind of \p *this row.
  void set_is_line_or_equality();

  //! Sets to \p RAY_OR_POINT_OR_INEQUALITY the kind of \p *this row.
  void set_is_ray_or_point_or_inequality();
  //@} // Flags coercion methods

  /*! \brief
    Normalizes the sign of the coefficients so that the first non-zero
    (homogeneous) coefficient of a line-or-equality is positive.
  */
  void sign_normalize();

  /*! \brief
    Strong normalization: ensures that different Grid_Generator objects
    represent different hyperplanes or hyperspaces.

    Applies both Grid_Generator::normalize() and Grid_Generator::sign_normalize().
  */
  void strong_normalize();

  /*! \brief
    Returns <CODE>true</CODE> if and only if the coefficients are
    strongly normalized.
  */
  bool check_strong_normalized() const;

  //! Linearly combines \p *this with \p y so that i-th coefficient is 0.
  /*!
    \param y
    The Grid_Generator that will be combined with \p *this object;

    \param i
    The index of the coefficient that has to become \f$0\f$.

    Computes a linear combination of \p *this and \p y having
    the i-th coefficient equal to \f$0\f$. Then it assigns
    the resulting Grid_Generator to \p *this and normalizes it.
  */
  void linear_combine(const Grid_Generator& y, dimension_type i);

  /*! \brief
    Throw a <CODE>std::invalid_argument</CODE> exception containing
    the appropriate error message.
  */
  void
  throw_dimension_incompatible(const char* method,
                               const char* name_var,
                               const Variable v) const;

  /*! \brief
    Throw a <CODE>std::invalid_argument</CODE> exception containing
    the appropriate error message.
  */
  void
  throw_invalid_argument(const char* method, const char* reason) const;

  friend std::ostream&
  IO_Operators::operator<<(std::ostream& s, const Grid_Generator& g);

  friend int
  compare(const Grid_Generator& x, const Grid_Generator& y);

  friend class Expression_Adapter<Grid_Generator>;
  friend class Grid_Generator_System;
  friend class Grid;
  friend class Linear_System<Grid_Generator>;
  friend class Scalar_Products;
  friend class Topology_Adjusted_Scalar_Product_Sign;
};


namespace Parma_Polyhedra_Library {

/*! \brief
  Shorthand for
  Grid_Generator::grid_line(const Linear_Expression& e, Representation r).

  \relates Grid_Generator
*/
Grid_Generator
grid_line(const Linear_Expression& e,
          Representation r = Grid_Generator::default_representation);

/*! \brief
  Shorthand for
  Grid_Generator::parameter(const Linear_Expression& e, Coefficient_traits::const_reference d, Representation r).

  \relates Grid_Generator
*/
Grid_Generator
parameter(const Linear_Expression& e = Linear_Expression::zero(),
          Coefficient_traits::const_reference d = Coefficient_one(),
          Representation r = Grid_Generator::default_representation);

//! Shorthand for Grid_Generator::parameter(Representation r).
/*! \relates Grid_Generator */
Grid_Generator
parameter(Representation r);

/*! \brief
  Shorthand for
  Grid_Generator::parameter(const Linear_Expression& e, Representation r).

  \relates Grid_Generator
*/
Grid_Generator
parameter(const Linear_Expression& e, Representation r);

/*! \brief
  Shorthand for
  Grid_Generator::grid_point(const Linear_Expression& e, Coefficient_traits::const_reference d, Representation r).

  \relates Grid_Generator
*/
Grid_Generator
grid_point(const Linear_Expression& e = Linear_Expression::zero(),
           Coefficient_traits::const_reference d = Coefficient_one(),
           Representation r = Grid_Generator::default_representation);

//! Shorthand for Grid_Generator::grid_point(Representation r).
/*! \relates Grid_Generator */
Grid_Generator
grid_point(Representation r);

/*! \brief
  Shorthand for
  Grid_Generator::grid_point(const Linear_Expression& e, Representation r).

  \relates Grid_Generator
*/
Grid_Generator
grid_point(const Linear_Expression& e, Representation r);

//! Returns <CODE>true</CODE> if and only if \p x is equivalent to \p y.
/*! \relates Grid_Generator */
bool operator==(const Grid_Generator& x, const Grid_Generator& y);

//! Returns <CODE>true</CODE> if and only if \p x is not equivalent to \p y.
/*! \relates Grid_Generator */
bool operator!=(const Grid_Generator& x, const Grid_Generator& y);


namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Grid_Generator */
std::ostream& operator<<(std::ostream& s, const Grid_Generator::Type& t);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

#include "Grid_Generator_inlines.hh"

#endif // !defined(PPL_Grid_Generator_defs_hh)
