/* Grid class declaration.
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

#ifndef PPL_Grid_defs_hh
#define PPL_Grid_defs_hh 1

#include "Grid_types.hh"
#include "globals_defs.hh"
#include "Variable_defs.hh"
#include "Variables_Set_types.hh"
#include "Linear_Expression_defs.hh"
#include "Constraint_types.hh"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "Congruence_System_defs.hh"
#include "Congruence_System_inlines.hh"
#include "Grid_Generator_System_defs.hh"
#include "Grid_Generator_System_inlines.hh"
#include "Grid_Generator_types.hh"
#include "Poly_Con_Relation_defs.hh"
#include "Poly_Gen_Relation_defs.hh"
#include "Grid_Certificate_types.hh"
#include "Box_types.hh"
#include "Polyhedron_types.hh"
#include "BD_Shape_types.hh"
#include "Octagonal_Shape_types.hh"
#include <vector>
#include <iosfwd>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*!
  \relates Parma_Polyhedra_Library::Grid
  Writes a textual representation of \p gr on \p s: <CODE>false</CODE>
  is written if \p gr is an empty grid; <CODE>true</CODE> is written
  if \p gr is a universe grid; a minimized system of congruences
  defining \p gr is written otherwise, all congruences in one row
  separated by ", "s.
*/
std::ostream&
operator<<(std::ostream& s, const Grid& gr);

} // namespace IO_Operators

//! Swaps \p x with \p y.
/*! \relates Grid */
void swap(Grid& x, Grid& y);

/*! \brief
  Returns <CODE>true</CODE> if and only if \p x and \p y are the same
  grid.

  \relates Grid
  Note that \p x and \p y may be dimension-incompatible grids: in
  those cases, the value <CODE>false</CODE> is returned.
*/
bool operator==(const Grid& x, const Grid& y);

/*! \brief
  Returns <CODE>true</CODE> if and only if \p x and \p y are different
  grids.

  \relates Grid
  Note that \p x and \p y may be dimension-incompatible grids: in
  those cases, the value <CODE>true</CODE> is returned.
*/
bool operator!=(const Grid& x, const Grid& y);

} // namespace Parma_Polyhedra_Library


//! A grid.
/*! \ingroup PPL_CXX_interface
  An object of the class Grid represents a rational grid.

  The domain of grids <EM>optimally supports</EM>:
    - all (proper and non-proper) congruences;
    - tautological and inconsistent constraints;
    - linear equality constraints (i.e., non-proper congruences).

  Depending on the method, using a constraint that is not optimally
  supported by the domain will either raise an exception or
  result in a (possibly non-optimal) upward approximation.

  The domain of grids support a concept of double description similar
  to the one developed for polyhedra: hence, a grid can be specified
  as either a finite system of congruences or a finite system of
  generators (see Section \ref sect_rational_grids) and it is always
  possible to obtain either representation.
  That is, if we know the system of congruences, we can obtain
  from this a system of generators that define the same grid
  and vice versa.
  These systems can contain redundant members, or they can be in the
  minimal form.

  A key attribute of any grid is its space dimension (the dimension
  \f$n \in \Nset\f$ of the enclosing vector space):

  - all grids, the empty ones included, are endowed with a space
    dimension;
  - most operations working on a grid and another object (another
    grid, a congruence, a generator, a set of variables, etc.) will
    throw an exception if the grid and the object are not
    dimension-compatible (see Section \ref Grid_Space_Dimensions);
  - the only ways in which the space dimension of a grid can be
    changed are with <EM>explicit</EM> calls to operators provided for
    that purpose, and with standard copy, assignment and swap
    operators.

  Note that two different grids can be defined on the zero-dimension
  space: the empty grid and the universe grid \f$R^0\f$.

  \par
  In all the examples it is assumed that variables
  <CODE>x</CODE> and <CODE>y</CODE> are defined (where they are
  used) as follows:
  \code
  Variable x(0);
  Variable y(1);
  \endcode

  \par Example 1
  The following code builds a grid corresponding to the even integer
  pairs in \f$\Rset^2\f$, given as a system of congruences:
  \code
  Congruence_System cgs;
  cgs.insert((x %= 0) / 2);
  cgs.insert((y %= 0) / 2);
  Grid gr(cgs);
  \endcode
  The following code builds the same grid as above, but starting
  from a system of generators specifying three of the points:
  \code
  Grid_Generator_System gs;
  gs.insert(grid_point(0*x + 0*y));
  gs.insert(grid_point(0*x + 2*y));
  gs.insert(grid_point(2*x + 0*y));
  Grid gr(gs);
  \endcode

  \par Example 2
  The following code builds a grid corresponding to a line in
  \f$\Rset^2\f$ by adding a single congruence to the universe grid:
  \code
  Congruence_System cgs;
  cgs.insert(x - y == 0);
  Grid gr(cgs);
  \endcode
  The following code builds the same grid as above, but starting
  from a system of generators specifying a point and a line:
  \code
  Grid_Generator_System gs;
  gs.insert(grid_point(0*x + 0*y));
  gs.insert(grid_line(x + y));
  Grid gr(gs);
  \endcode

  \par Example 3
  The following code builds a grid corresponding to the integral
  points on the line \f$x = y\f$ in \f$\Rset^2\f$ constructed
  by adding an equality and congruence to the universe grid:
  \code
  Congruence_System cgs;
  cgs.insert(x - y == 0);
  cgs.insert(x %= 0);
  Grid gr(cgs);
  \endcode
  The following code builds the same grid as above, but starting
  from a system of generators specifying a point and a parameter:
  \code
  Grid_Generator_System gs;
  gs.insert(grid_point(0*x + 0*y));
  gs.insert(parameter(x + y));
  Grid gr(gs);
  \endcode

  \par Example 4
  The following code builds the grid corresponding to a plane by
  creating the universe grid in \f$\Rset^2\f$:
  \code
  Grid gr(2);
  \endcode
  The following code builds the same grid as above, but starting
  from the empty grid in \f$\Rset^2\f$ and inserting the appropriate
  generators (a point, and two lines).
  \code
  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(0*x + 0*y));
  gr.add_grid_generator(grid_line(x));
  gr.add_grid_generator(grid_line(y));
  \endcode
  Note that a generator system must contain a point when describing
  a grid.  To ensure that this is always the case it is required
  that the first generator inserted in an empty grid is a point
  (otherwise, an exception is thrown).

  \par Example 5
  The following code shows the use of the function
  <CODE>add_space_dimensions_and_embed</CODE>:
  \code
  Grid gr(1);
  gr.add_congruence(x == 2);
  gr.add_space_dimensions_and_embed(1);
  \endcode
  We build the universe grid in the 1-dimension space \f$\Rset\f$.
  Then we add a single equality congruence,
  thus obtaining the grid corresponding to the singleton set
  \f$\{ 2 \} \sseq \Rset\f$.
  After the last line of code, the resulting grid is
  \f[
  \bigl\{\,
  (2, y)^\transpose \in \Rset^2
  \bigm|
  y \in \Rset
  \,\bigr\}.
  \f]

  \par Example 6
  The following code shows the use of the function
  <CODE>add_space_dimensions_and_project</CODE>:
  \code
  Grid gr(1);
  gr.add_congruence(x == 2);
  gr.add_space_dimensions_and_project(1);
  \endcode
  The first two lines of code are the same as in Example 4 for
  <CODE>add_space_dimensions_and_embed</CODE>.
  After the last line of code, the resulting grid is
  the singleton set
  \f$\bigl\{ (2, 0)^\transpose \bigr\} \sseq \Rset^2\f$.

  \par Example 7
  The following code shows the use of the function
  <CODE>affine_image</CODE>:
  \code
  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(0*x + 0*y));
  gr.add_grid_generator(grid_point(4*x + 0*y));
  gr.add_grid_generator(grid_point(0*x + 2*y));
  Linear_Expression expr = x + 3;
  gr.affine_image(x, expr);
  \endcode
  In this example the starting grid is all the pairs of \f$x\f$ and
  \f$y\f$ in \f$\Rset^2\f$ where \f$x\f$ is an integer multiple of 4
  and \f$y\f$ is an integer multiple of 2.  The considered variable
  is \f$x\f$ and the affine expression is \f$x+3\f$.  The resulting
  grid is the given grid translated 3 integers to the right (all the
  pairs \f$(x, y)\f$ where \f$x\f$ is -1 plus an integer multiple of 4
  and \f$y\f$ is an integer multiple of 2).
  Moreover, if the affine transformation for the same variable \p x
  is instead \f$x+y\f$:
  \code
  Linear_Expression expr = x + y;
  \endcode
  the resulting grid is every second integral point along the \f$x=y\f$
  line, with this line of points repeated at every fourth integral value
  along the \f$x\f$ axis.
  Instead, if we do not use an invertible transformation for the
  same variable; for example, the affine expression \f$y\f$:
  \code
  Linear_Expression expr = y;
  \endcode
  the resulting grid is every second point along the \f$x=y\f$ line.

  \par Example 8
  The following code shows the use of the function
  <CODE>affine_preimage</CODE>:
  \code
  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(0*x + 0*y));
  gr.add_grid_generator(grid_point(4*x + 0*y));
  gr.add_grid_generator(grid_point(0*x + 2*y));
  Linear_Expression expr = x + 3;
  gr.affine_preimage(x, expr);
  \endcode
  In this example the starting grid, \p var and the affine
  expression and the denominator are the same as in Example 6, while
  the resulting grid is similar but translated 3 integers to the
  left (all the pairs \f$(x, y)\f$
  where \f$x\f$ is -3 plus an integer multiple of 4 and
  \f$y\f$ is an integer multiple of 2)..
  Moreover, if the affine transformation for \p x is \f$x+y\f$
  \code
  Linear_Expression expr = x + y;
  \endcode
  the resulting grid is a similar grid to the result in Example 6,
  only the grid is slanted along \f$x=-y\f$.
  Instead, if we do not use an invertible transformation for the same
  variable \p x, for example, the affine expression \f$y\f$:
  \code
  Linear_Expression expr = y;
  \endcode
  the resulting grid is every fourth line parallel to the \f$x\f$
  axis.

  \par Example 9
  For this example we also use the variables:
  \code
  Variable z(2);
  Variable w(3);
  \endcode
  The following code shows the use of the function
  <CODE>remove_space_dimensions</CODE>:
  \code
  Grid_Generator_System gs;
  gs.insert(grid_point(3*x + y +0*z + 2*w));
  Grid gr(gs);
  Variables_Set vars;
  vars.insert(y);
  vars.insert(z);
  gr.remove_space_dimensions(vars);
  \endcode
  The starting grid is the singleton set
  \f$\bigl\{ (3, 1, 0, 2)^\transpose \bigr\} \sseq \Rset^4\f$, while
  the resulting grid is
  \f$\bigl\{ (3, 2)^\transpose \bigr\} \sseq \Rset^2\f$.
  Be careful when removing space dimensions <EM>incrementally</EM>:
  since dimensions are automatically renamed after each application
  of the <CODE>remove_space_dimensions</CODE> operator, unexpected
  results can be obtained.
  For instance, by using the following code we would obtain
  a different result:
  \code
  set<Variable> vars1;
  vars1.insert(y);
  gr.remove_space_dimensions(vars1);
  set<Variable> vars2;
  vars2.insert(z);
  gr.remove_space_dimensions(vars2);
  \endcode
  In this case, the result is the grid
  \f$\bigl\{(3, 0)^\transpose \bigr\} \sseq \Rset^2\f$:
  when removing the set of dimensions \p vars2
  we are actually removing variable \f$w\f$ of the original grid.
  For the same reason, the operator \p remove_space_dimensions
  is not idempotent: removing twice the same non-empty set of dimensions
  is never the same as removing them just once.
*/

class Parma_Polyhedra_Library::Grid {
public:
  //! The numeric type of coefficients.
  typedef Coefficient coefficient_type;

  //! Returns the maximum space dimension all kinds of Grid can handle.
  static dimension_type max_space_dimension();

  /*! \brief
    Returns true indicating that this domain has methods that
    can recycle congruences.
  */
  static bool can_recycle_congruence_systems();

  /*! \brief
    Returns true indicating that this domain has methods that
    can recycle constraints.
  */
  static bool can_recycle_constraint_systems();

  //! Builds a grid having the specified properties.
  /*!
    \param num_dimensions
    The number of dimensions of the vector space enclosing the grid;

    \param kind
    Specifies whether the universe or the empty grid has to be built.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  explicit Grid(dimension_type num_dimensions = 0,
                Degenerate_Element kind = UNIVERSE);

  //! Builds a grid, copying a system of congruences.
  /*!
    The grid inherits the space dimension of the congruence system.

    \param cgs
    The system of congruences defining the grid.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  explicit Grid(const Congruence_System& cgs);

  //! Builds a grid, recycling a system of congruences.
  /*!
    The grid inherits the space dimension of the congruence system.

    \param cgs
    The system of congruences defining the grid.  Its data-structures
    may be recycled to build the grid.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  Grid(Congruence_System& cgs, Recycle_Input dummy);

  //! Builds a grid, copying a system of constraints.
  /*!
    The grid inherits the space dimension of the constraint system.

    \param cs
    The system of constraints defining the grid.

    \exception std::invalid_argument
    Thrown if the constraint system \p cs contains inequality constraints.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  explicit Grid(const Constraint_System& cs);

  //! Builds a grid, recycling a system of constraints.
  /*!
    The grid inherits the space dimension of the constraint system.

    \param cs
    The system of constraints defining the grid.  Its data-structures
    may be recycled to build the grid.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.

    \exception std::invalid_argument
    Thrown if the constraint system \p cs contains inequality constraints.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  Grid(Constraint_System& cs, Recycle_Input dummy);

  //! Builds a grid, copying a system of grid generators.
  /*!
    The grid inherits the space dimension of the generator system.

    \param ggs
    The system of generators defining the grid.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  explicit Grid(const Grid_Generator_System& ggs);

  //! Builds a grid, recycling a system of grid generators.
  /*!
    The grid inherits the space dimension of the generator system.

    \param ggs
    The system of generators defining the grid.  Its data-structures
    may be recycled to build the grid.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space dimension.
  */
  Grid(Grid_Generator_System& ggs, Recycle_Input dummy);

  //! Builds a grid out of a box.
  /*!
    The grid inherits the space dimension of the box.
    The built grid is the most precise grid that includes the box.

    \param box
    The box representing the grid to be built.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.

    \exception std::length_error
    Thrown if the space dimension of \p box exceeds the maximum
    allowed space dimension.
  */
  template <typename Interval>
  explicit Grid(const Box<Interval>& box,
                Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a grid out of a bounded-difference shape.
  /*!
    The grid inherits the space dimension of the BDS.
    The built grid is the most precise grid that includes the BDS.

    \param bd
    The BDS representing the grid to be built.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.

    \exception std::length_error
    Thrown if the space dimension of \p bd exceeds the maximum
    allowed space dimension.
  */
  template <typename U>
  explicit Grid(const BD_Shape<U>& bd,
                Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a grid out of an octagonal shape.
  /*!
    The grid inherits the space dimension of the octagonal shape.
    The built grid is the most precise grid that includes the octagonal shape.

    \param os
    The octagonal shape representing the grid to be built.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.

    \exception std::length_error
    Thrown if the space dimension of \p os exceeds the maximum
    allowed space dimension.
  */
  template <typename U>
  explicit Grid(const Octagonal_Shape<U>& os,
                Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    Builds a grid from a polyhedron using algorithms whose complexity
    does not exceed the one specified by \p complexity.
    If \p complexity is \p ANY_COMPLEXITY, then the grid built is the
    smallest one containing \p ph.

    The grid inherits the space dimension of polyhedron.

    \param ph
    The polyhedron.

    \param complexity
    The complexity class.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space
    dimension.
  */
  explicit Grid(const Polyhedron& ph,
                Complexity_Class complexity = ANY_COMPLEXITY);

  //! Ordinary copy constructor.
  /*!
    The complexity argument is ignored.
  */
  Grid(const Grid& y,
       Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    The assignment operator.  (\p *this and \p y can be
    dimension-incompatible.)
  */
  Grid& operator=(const Grid& y);

  //! \name Member Functions that Do Not Modify the Grid
  //@{

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  /*! \brief
    Returns \f$0\f$, if \p *this is empty; otherwise, returns
    the \ref Grid_Affine_Dimension "affine dimension" of \p *this.
  */
  dimension_type affine_dimension() const;

  /*! \brief
    Returns a system of equality constraints satisfied by \p *this
    with the same affine dimension as \p *this.
  */
  Constraint_System constraints() const;

  /*! \brief
    Returns a minimal system of equality constraints satisfied by
    \p *this with the same affine dimension as \p *this.
  */
  Constraint_System minimized_constraints() const;

  //! Returns the system of congruences.
  const Congruence_System& congruences() const;

  //! Returns the system of congruences in minimal form.
  const Congruence_System& minimized_congruences() const;

  //! Returns the system of generators.
  const Grid_Generator_System& grid_generators() const;

  //! Returns the minimized system of generators.
  const Grid_Generator_System& minimized_grid_generators() const;

  //! Returns the relations holding between \p *this and \p cg.
  /*
    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are dimension-incompatible.
  */
  // FIXME: Poly_Con_Relation seems to encode exactly what we want
  // here.  We must find a new name for that class.  Temporarily,
  // we keep using it without changing the name.
  Poly_Con_Relation relation_with(const Congruence& cg) const;

  //! Returns the relations holding between \p *this and \p g.
  /*
    \exception std::invalid_argument
    Thrown if \p *this and generator \p g are dimension-incompatible.
  */
  // FIXME: see the comment for Poly_Con_Relation above.
  Poly_Gen_Relation
  relation_with(const Grid_Generator& g) const;

  //! Returns the relations holding between \p *this and \p g.
  /*
    \exception std::invalid_argument
    Thrown if \p *this and generator \p g are dimension-incompatible.
  */
  // FIXME: see the comment for Poly_Con_Relation above.
  Poly_Gen_Relation
  relation_with(const Generator& g) const;

  //! Returns the relations holding between \p *this and \p c.
  /*
    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible.
  */
  // FIXME: Poly_Con_Relation seems to encode exactly what we want
  // here.  We must find a new name for that class.  Temporarily,
  // we keep using it without changing the name.
  Poly_Con_Relation relation_with(const Constraint& c) const;

  //! Returns \c true if and only if \p *this is an empty grid.
  bool is_empty() const;

  //! Returns \c true if and only if \p *this is a universe grid.
  bool is_universe() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is a
    topologically closed subset of the vector space.

    A grid is always topologically closed.
  */
  bool is_topologically_closed() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this and \p y are
    disjoint.

    \exception std::invalid_argument
    Thrown if \p x and \p y are dimension-incompatible.
  */
  bool is_disjoint_from(const Grid& y) const;

  //! Returns <CODE>true</CODE> if and only if \p *this is discrete.
  /*!
    A grid is discrete if it can be defined by a generator system which
    contains only points and parameters.  This includes the empty grid
    and any grid in dimension zero.
  */
  bool is_discrete() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is bounded.
  bool is_bounded() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    contains at least one integer point.
  */
  bool contains_integer_point() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p var is constrained in
    \p *this.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.
  */
  bool constrains(Variable var) const;

  //! Returns <CODE>true</CODE> if and only if \p expr is bounded in \p *this.
  /*!
    This method is the same as bounds_from_below.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.
  */
  bool bounds_from_above(const Linear_Expression& expr) const;

  //! Returns <CODE>true</CODE> if and only if \p expr is bounded in \p *this.
  /*!
    This method is the same as bounds_from_above.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.
  */
  bool bounds_from_below(const Linear_Expression& expr) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty and
    \p expr is bounded from above in \p *this, in which case the
    supremum value is computed.

    \param expr
    The linear expression to be maximized subject to \p *this;

    \param sup_n
    The numerator of the supremum value;

    \param sup_d
    The denominator of the supremum value;

    \param maximum
    <CODE>true</CODE> if the supremum value can be reached in \p this.
    Always <CODE>true</CODE> when \p this bounds \p expr.  Present for
    interface compatibility with class Polyhedron, where closure
    points can result in a value of false.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded by \p *this,
    <CODE>false</CODE> is returned and \p sup_n, \p sup_d and \p
    maximum are left untouched.
  */
  bool maximize(const Linear_Expression& expr,
                Coefficient& sup_n, Coefficient& sup_d, bool& maximum) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty and
    \p expr is bounded from above in \p *this, in which case the
    supremum value and a point where \p expr reaches it are computed.

    \param expr
    The linear expression to be maximized subject to \p *this;

    \param sup_n
    The numerator of the supremum value;

    \param sup_d
    The denominator of the supremum value;

    \param maximum
    <CODE>true</CODE> if the supremum value can be reached in \p this.
    Always <CODE>true</CODE> when \p this bounds \p expr.  Present for
    interface compatibility with class Polyhedron, where closure
    points can result in a value of false;

    \param point
    When maximization succeeds, will be assigned a point where \p expr
    reaches its supremum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded by \p *this,
    <CODE>false</CODE> is returned and \p sup_n, \p sup_d, \p maximum
    and \p point are left untouched.
  */
  bool maximize(const Linear_Expression& expr,
                Coefficient& sup_n, Coefficient& sup_d, bool& maximum,
                Generator& point) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty and
    \p expr is bounded from below in \p *this, in which case the
    infimum value is computed.

    \param expr
    The linear expression to be minimized subject to \p *this;

    \param inf_n
    The numerator of the infimum value;

    \param inf_d
    The denominator of the infimum value;

    \param minimum
    <CODE>true</CODE> if the is the infimum value can be reached in \p
    this.  Always <CODE>true</CODE> when \p this bounds \p expr.
    Present for interface compatibility with class Polyhedron, where
    closure points can result in a value of false.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded from below,
    <CODE>false</CODE> is returned and \p inf_n, \p inf_d
    and \p minimum are left untouched.
  */
  bool minimize(const Linear_Expression& expr,
                Coefficient& inf_n, Coefficient& inf_d, bool& minimum) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty and
    \p expr is bounded from below in \p *this, in which case the
    infimum value and a point where \p expr reaches it are computed.

    \param expr
    The linear expression to be minimized subject to \p *this;

    \param inf_n
    The numerator of the infimum value;

    \param inf_d
    The denominator of the infimum value;

    \param minimum
    <CODE>true</CODE> if the is the infimum value can be reached in \p
    this.  Always <CODE>true</CODE> when \p this bounds \p expr.
    Present for interface compatibility with class Polyhedron, where
    closure points can result in a value of false;

    \param point
    When minimization succeeds, will be assigned a point where \p expr
    reaches its infimum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded from below,
    <CODE>false</CODE> is returned and \p inf_n, \p inf_d, \p minimum
    and \p point are left untouched.
  */
  bool minimize(const Linear_Expression& expr,
                Coefficient& inf_n, Coefficient& inf_d, bool& minimum,
                Generator& point) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty and
    \ref Grid_Frequency "frequency" for \p *this with respect to \p expr
    is defined, in which case the frequency and the value for \p expr
    that is closest to zero are computed.

    \param expr
    The linear expression for which the frequency is needed;

    \param freq_n
    The numerator of the maximum frequency of \p expr;

    \param freq_d
    The denominator of the maximum frequency of \p expr;

    \param val_n
    The numerator of them value of \p expr at a point in the grid
    that is closest to zero;

    \param val_d
    The denominator of a value of \p expr at a point in the grid
    that is closest to zero;

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or frequency is undefined with respect to \p expr,
    then <CODE>false</CODE> is returned and \p freq_n, \p freq_d,
    \p val_n and \p val_d are left untouched.
  */
  bool frequency(const Linear_Expression& expr,
                 Coefficient& freq_n, Coefficient& freq_d,
                 Coefficient& val_n, Coefficient& val_d) const;

  //! Returns <CODE>true</CODE> if and only if \p *this contains \p y.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool contains(const Grid& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this strictly
    contains \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool strictly_contains(const Grid& y) const;

  //! Checks if all the invariants are satisfied.
  /*!
    \return
    <CODE>true</CODE> if and only if \p *this satisfies all the
    invariants and either \p check_not_empty is <CODE>false</CODE> or
    \p *this is not empty.

    \param check_not_empty
    <CODE>true</CODE> if and only if, in addition to checking the
    invariants, \p *this must be checked to be not empty.

    The check is performed so as to intrude as little as possible.  If
    the library has been compiled with run-time assertions enabled,
    error messages are written on <CODE>std::cerr</CODE> in case
    invariants are violated. This is useful for the purpose of
    debugging the library.
  */
  bool OK(bool check_not_empty = false) const;

  //@} // Member Functions that Do Not Modify the Grid

  //! \name Space Dimension Preserving Member Functions that May Modify the Grid
  //@{

  //! Adds a copy of congruence \p cg to \p *this.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are
    dimension-incompatible.
  */
  void add_congruence(const Congruence& cg);

  /*! \brief
    Adds a copy of grid generator \p g to the system of generators of
    \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and generator \p g are dimension-incompatible,
    or if \p *this is an empty grid and \p g is not a point.
  */
  void add_grid_generator(const Grid_Generator& g);

  //! Adds a copy of each congruence in \p cgs to \p *this.
  /*!
    \param cgs
    Contains the congruences that will be added to the system of
    congruences of \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible.
  */
  void add_congruences(const Congruence_System& cgs);

  //! Adds the congruences in \p cgs to *this.
  /*!
    \param cgs
    The congruence system to be added to \p *this.  The congruences in
    \p cgs may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible.

    \warning
    The only assumption that can be made about \p cgs upon successful
    or exceptional return is that it can be safely destroyed.
  */
  void add_recycled_congruences(Congruence_System& cgs);

  /*! \brief
    Adds to \p *this a congruence equivalent to constraint \p c.

    \param c
    The constraint to be added.

    \exception std::invalid_argument
    Thrown if \p *this and \p c are dimension-incompatible
    or if constraint \p c is not optimally supported by the grid domain.
  */
  void add_constraint(const Constraint& c);

  /*! \brief
    Adds to \p *this congruences equivalent to the constraints in \p cs.

    \param cs
    The constraints to be added.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible
    or if \p cs contains a constraint which is not optimally supported
    by the grid domain.
  */
  void add_constraints(const Constraint_System& cs);

  /*! \brief
    Adds to \p *this congruences equivalent to the constraints in \p cs.

    \param cs
    The constraints to be added. They may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible
    or if \p cs contains a constraint which is not optimally supported
    by the grid domain.

    \warning
    The only assumption that can be made about \p cs upon successful
    or exceptional return is that it can be safely destroyed.
  */
  void add_recycled_constraints(Constraint_System& cs);

  //! Uses a copy of the congruence \p cg to refine \p *this.
  /*!
    \param cg
    The congruence used.

    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are dimension-incompatible.
  */
  void refine_with_congruence(const Congruence& cg);

 //! Uses a copy of the congruences in \p cgs to refine \p *this.
  /*!
    \param cgs
    The congruences used.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible.
  */
  void refine_with_congruences(const Congruence_System& cgs);

  //! Uses a copy of the constraint \p c to refine \p *this.
  /*!

    \param c
    The constraint used. If it is not an equality, it will be ignored

    \exception std::invalid_argument
    Thrown if \p *this and \p c are dimension-incompatible.
  */
  void refine_with_constraint(const Constraint& c);

  //! Uses a copy of the constraints in \p cs to refine \p *this.
  /*!
    \param cs
    The constraints used. Constraints that are not equalities are ignored.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible.
  */
  void refine_with_constraints(const Constraint_System& cs);

  /*! \brief
    Adds a copy of the generators in \p gs to the system of generators
    of \p *this.

    \param gs
    Contains the generators that will be added to the system of
    generators of \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p gs are dimension-incompatible, or if
    \p *this is empty and the system of generators \p gs is not empty,
    but has no points.
  */
  void add_grid_generators(const Grid_Generator_System& gs);

  /*! \brief
    Adds the generators in \p gs to the system of generators of \p
    *this.

    \param gs
    The generator system to be added to \p *this.  The generators in
    \p gs may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p gs are dimension-incompatible.

    \warning
    The only assumption that can be made about \p gs upon successful
    or exceptional return is that it can be safely destroyed.
  */
  void add_recycled_grid_generators(Grid_Generator_System& gs);

  /*! \brief
    Computes the \ref Cylindrification "cylindrification" of \p *this with
    respect to space dimension \p var, assigning the result to \p *this.

    \param var
    The space dimension that will be unconstrained.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.
  */
  void unconstrain(Variable var);

  /*! \brief
    Computes the \ref Cylindrification "cylindrification" of \p *this with
    respect to the set of space dimensions \p vars,
    assigning the result to \p *this.

    \param vars
    The set of space dimension that will be unconstrained.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the
    Variable objects contained in \p vars.
  */
  void unconstrain(const Variables_Set& vars);

  /*! \brief
    Assigns to \p *this the intersection of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void intersection_assign(const Grid& y);

  /*! \brief
    Assigns to \p *this the least upper bound of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void upper_bound_assign(const Grid& y);

  /*! \brief
    If the upper bound of \p *this and \p y is exact it is assigned to \p
    *this and <CODE>true</CODE> is returned, otherwise
    <CODE>false</CODE> is returned.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool upper_bound_assign_if_exact(const Grid& y);

  /*! \brief
    Assigns to \p *this the \ref Convex_Polyhedral_Difference "grid-difference"
    of \p *this and \p y.

    The grid difference between grids x and y is the smallest grid
    containing all the points from x and y that are only in x.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void difference_assign(const Grid& y);

  /*! \brief
    Assigns to \p *this a \ref Meet_Preserving_Simplification
    "meet-preserving simplification" of \p *this with respect to \p y.
    If \c false is returned, then the intersection is empty.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool simplify_using_context_assign(const Grid& y);

  /*! \brief
    Assigns to \p *this the \ref Grid_Affine_Transformation
    "affine image" of \p
    *this under the function mapping variable \p var to the affine
    expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is assigned;

    \param expr
    The numerator of the affine expression;

    \param denominator
    The denominator of the affine expression (optional argument with
    default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of
    \p *this.

    \if Include_Implementation_Details

    When considering the generators of a grid, the
    affine transformation
    \f[
      \frac{\sum_{i=0}^{n-1} a_i x_i + b}{\mathrm{denominator}}
    \f]
    is assigned to \p var where \p expr is
    \f$\sum_{i=0}^{n-1} a_i x_i + b\f$
    (\f$b\f$ is the inhomogeneous term).

    If congruences are up-to-date, it uses the specialized function
    affine_preimage() (for the system of congruences)
    and inverse transformation to reach the same result.
    To obtain the inverse transformation we use the following observation.

    Observation:
    -# The affine transformation is invertible if the coefficient
       of \p var in this transformation (i.e., \f$a_\mathrm{var}\f$)
       is different from zero.
    -# If the transformation is invertible, then we can write
       \f[
         \mathrm{denominator} * {x'}_\mathrm{var}
           = \sum_{i = 0}^{n - 1} a_i x_i + b
           = a_\mathrm{var} x_\mathrm{var}
             + \sum_{i \neq var} a_i x_i + b,
       \f]
       so that the inverse transformation is
       \f[
         a_\mathrm{var} x_\mathrm{var}
           = \mathrm{denominator} * {x'}_\mathrm{var}
             - \sum_{i \neq j} a_i x_i - b.
       \f]

    Then, if the transformation is invertible, all the entities that
    were up-to-date remain up-to-date. Otherwise only generators remain
    up-to-date.

    \endif
  */
  void affine_image(Variable var,
                    const Linear_Expression& expr,
                    Coefficient_traits::const_reference denominator
                    = Coefficient_one());

  /*! \brief
    Assigns to \p *this the \ref Grid_Affine_Transformation
    "affine preimage" of
    \p *this under the function mapping variable \p var to the affine
    expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is substituted;

    \param expr
    The numerator of the affine expression;

    \param denominator
    The denominator of the affine expression (optional argument with
    default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of \p *this.

    \if Include_Implementation_Details

    When considering congruences of a grid, the affine transformation
    \f[
      \frac{\sum_{i=0}^{n-1} a_i x_i + b}{denominator},
    \f]
    is assigned to \p var where \p expr is
    \f$\sum_{i=0}^{n-1} a_i x_i + b\f$
    (\f$b\f$ is the inhomogeneous term).

    If generators are up-to-date, then the specialized function
    affine_image() is used (for the system of generators)
    and inverse transformation to reach the same result.
    To obtain the inverse transformation, we use the following observation.

    Observation:
    -# The affine transformation is invertible if the coefficient
       of \p var in this transformation (i.e. \f$a_\mathrm{var}\f$)
       is different from zero.
    -# If the transformation is invertible, then we can write
       \f[
         \mathrm{denominator} * {x'}_\mathrm{var}
           = \sum_{i = 0}^{n - 1} a_i x_i + b
           = a_\mathrm{var} x_\mathrm{var}
               + \sum_{i \neq \mathrm{var}} a_i x_i + b,
       \f],
       the inverse transformation is
       \f[
         a_\mathrm{var} x_\mathrm{var}
           = \mathrm{denominator} * {x'}_\mathrm{var}
               - \sum_{i \neq j} a_i x_i - b.
       \f].

    Then, if the transformation is invertible, all the entities that
    were up-to-date remain up-to-date. Otherwise only congruences remain
    up-to-date.

    \endif
  */
  void affine_preimage(Variable var,
                       const Linear_Expression& expr,
                       Coefficient_traits::const_reference denominator
                         = Coefficient_one());

  /*! \brief
    Assigns to \p *this the image of \p *this with respect to
    the \ref Grid_Generalized_Image "generalized affine relation"
    \f$\mathrm{var}' = \frac{\mathrm{expr}}{\mathrm{denominator}}
    \pmod{\mathrm{modulus}}\f$.

    \param var
    The left hand side variable of the generalized affine relation;

    \param relsym
    The relation symbol where EQUAL is the symbol for a congruence
    relation;

    \param expr
    The numerator of the right hand side affine expression;

    \param denominator
    The denominator of the right hand side affine expression.
    Optional argument with an automatic value of one;

    \param modulus
    The modulus of the congruence lhs %= rhs.  A modulus of zero
    indicates lhs == rhs.  Optional argument with an automatic value
    of zero.

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of \p
    *this.
  */
  void
  generalized_affine_image(Variable var,
                           Relation_Symbol relsym,
                           const Linear_Expression& expr,
                           Coefficient_traits::const_reference denominator
                           = Coefficient_one(),
                           Coefficient_traits::const_reference modulus
                           = Coefficient_zero());

  /*! \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Grid_Generalized_Image "generalized affine relation"
    \f$\mathrm{var}' = \frac{\mathrm{expr}}{\mathrm{denominator}}
    \pmod{\mathrm{modulus}}\f$.

    \param var
    The left hand side variable of the generalized affine relation;

    \param relsym
    The relation symbol where EQUAL is the symbol for a congruence
    relation;

    \param expr
    The numerator of the right hand side affine expression;

    \param denominator
    The denominator of the right hand side affine expression.
    Optional argument with an automatic value of one;

    \param modulus
    The modulus of the congruence lhs %= rhs.  A modulus of zero
    indicates lhs == rhs.  Optional argument with an automatic value
    of zero.

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of \p
    *this.
  */
  void
  generalized_affine_preimage(Variable var,
                              Relation_Symbol relsym,
                              const Linear_Expression& expr,
                              Coefficient_traits::const_reference denominator
                              = Coefficient_one(),
                              Coefficient_traits::const_reference modulus
                              = Coefficient_zero());

  /*! \brief
    Assigns to \p *this the image of \p *this with respect to
    the \ref Grid_Generalized_Image "generalized affine relation"
    \f$\mathrm{lhs}' = \mathrm{rhs} \pmod{\mathrm{modulus}}\f$.

    \param lhs
    The left hand side affine expression.

    \param relsym
    The relation symbol where EQUAL is the symbol for a congruence
    relation;

    \param rhs
    The right hand side affine expression.

    \param modulus
    The modulus of the congruence lhs %= rhs.  A modulus of zero
    indicates lhs == rhs.  Optional argument with an automatic value
    of zero.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p lhs or \p
    rhs.
  */
  void
  generalized_affine_image(const Linear_Expression& lhs,
                           Relation_Symbol relsym,
                           const Linear_Expression& rhs,
                           Coefficient_traits::const_reference modulus
                           = Coefficient_zero());

  /*! \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Grid_Generalized_Image "generalized affine relation"
    \f$\mathrm{lhs}' = \mathrm{rhs} \pmod{\mathrm{modulus}}\f$.

    \param lhs
    The left hand side affine expression;

    \param relsym
    The relation symbol where EQUAL is the symbol for a congruence
    relation;

    \param rhs
    The right hand side affine expression;

    \param modulus
    The modulus of the congruence lhs %= rhs.  A modulus of zero
    indicates lhs == rhs.  Optional argument with an automatic value
    of zero.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p lhs or \p
    rhs.
  */
  void
  generalized_affine_preimage(const Linear_Expression& lhs,
                              Relation_Symbol relsym,
                              const Linear_Expression& rhs,
                              Coefficient_traits::const_reference modulus
                              = Coefficient_zero());

  /*!
    \brief
    Assigns to \p *this the image of \p *this with respect to the
    \ref Single_Update_Bounded_Affine_Relations "bounded affine relation"
    \f$\frac{\mathrm{lb\_expr}}{\mathrm{denominator}}
         \leq \mathrm{var}'
           \leq \frac{\mathrm{ub\_expr}}{\mathrm{denominator}}\f$.

    \param var
    The variable updated by the affine relation;

    \param lb_expr
    The numerator of the lower bounding affine expression;

    \param ub_expr
    The numerator of the upper bounding affine expression;

    \param denominator
    The (common) denominator for the lower and upper bounding
    affine expressions (optional argument with default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p lb_expr (resp., \p ub_expr)
    and \p *this are dimension-incompatible or if \p var is not a space
    dimension of \p *this.
  */
  void bounded_affine_image(Variable var,
                            const Linear_Expression& lb_expr,
                            const Linear_Expression& ub_expr,
                            Coefficient_traits::const_reference denominator
                            = Coefficient_one());

  /*!
    \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Single_Update_Bounded_Affine_Relations "bounded affine relation"
    \f$\frac{\mathrm{lb\_expr}}{\mathrm{denominator}}
         \leq \mathrm{var}'
           \leq \frac{\mathrm{ub\_expr}}{\mathrm{denominator}}\f$.

    \param var
    The variable updated by the affine relation;

    \param lb_expr
    The numerator of the lower bounding affine expression;

    \param ub_expr
    The numerator of the upper bounding affine expression;

    \param denominator
    The (common) denominator for the lower and upper bounding
    affine expressions (optional argument with default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p lb_expr (resp., \p ub_expr)
    and \p *this are dimension-incompatible or if \p var is not a space
    dimension of \p *this.
  */
  void bounded_affine_preimage(Variable var,
                               const Linear_Expression& lb_expr,
                               const Linear_Expression& ub_expr,
                               Coefficient_traits::const_reference denominator
                               = Coefficient_one());

  /*! \brief
    Assigns to \p *this the result of computing the \ref Grid_Time_Elapse
    "time-elapse" between \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void time_elapse_assign(const Grid& y);

  /*! \brief
    \ref Wrapping_Operator "Wraps" the specified dimensions of the
    vector space.

    \param vars
    The set of Variable objects corresponding to the space dimensions
    to be wrapped.

    \param w
    The width of the bounded integer type corresponding to
    all the dimensions to be wrapped.

    \param r
    The representation of the bounded integer type corresponding to
    all the dimensions to be wrapped.

    \param o
    The overflow behavior of the bounded integer type corresponding to
    all the dimensions to be wrapped.

    \param cs_p
    Possibly null pointer to a constraint system.
    This argument is for compatibility with wrap_assign()
    for the other domains and only checked for dimension-compatibility.

    \param complexity_threshold
    A precision parameter of the \ref Wrapping_Operator "wrapping operator".
    This argument is for compatibility with wrap_assign()
    for the other domains and is ignored.

    \param wrap_individually
    <CODE>true</CODE> if the dimensions should be wrapped individually.
    As wrapping dimensions collectively does not improve the precision,
    this argument is ignored.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the
    Variable objects contained in \p vars or with <CODE>*cs_p</CODE>.

    \warning
    It is assumed that variables in \p Vars represent integers.  Thus,
    where the extra cost is negligible, the integrality of these
    variables is enforced; possibly causing a non-integral grid to
    become empty.
  */
  void wrap_assign(const Variables_Set& vars,
                   Bounded_Integer_Type_Width w,
                   Bounded_Integer_Type_Representation r,
                   Bounded_Integer_Type_Overflow o,
                   const Constraint_System* cs_p = 0,
                   unsigned complexity_threshold = 16,
                   bool wrap_individually = true);

  /*! \brief
    Possibly tightens \p *this by dropping all points with non-integer
    coordinates.

    \param complexity
    This argument is ignored as the algorithm used has polynomial
    complexity.
  */
  void drop_some_non_integer_points(Complexity_Class complexity
                                    = ANY_COMPLEXITY);

  /*! \brief
    Possibly tightens \p *this by dropping all points with non-integer
    coordinates for the space dimensions corresponding to \p vars.

    \param vars
    Points with non-integer coordinates for these variables/space-dimensions
    can be discarded.

    \param complexity
    This argument is ignored as the algorithm used has polynomial
    complexity.
  */
  void drop_some_non_integer_points(const Variables_Set& vars,
                                    Complexity_Class complexity
                                    = ANY_COMPLEXITY);

  //! Assigns to \p *this its topological closure.
  void topological_closure_assign();

  /*! \brief
    Assigns to \p *this the result of computing the \ref Grid_Widening
    "Grid widening" between \p *this and \p y using congruence systems.

    \param y
    A grid that <EM>must</EM> be contained in \p *this;

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Grid_Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void congruence_widening_assign(const Grid& y, unsigned* tp = NULL);

  /*! \brief
    Assigns to \p *this the result of computing the \ref Grid_Widening
    "Grid widening" between \p *this and \p y using generator systems.

    \param y
    A grid that <EM>must</EM> be contained in \p *this;

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Grid_Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void generator_widening_assign(const Grid& y, unsigned* tp = NULL);

  /*! \brief
    Assigns to \p *this the result of computing the \ref Grid_Widening
    "Grid widening" between \p *this and \p y.

    This widening uses either the congruence or generator systems
    depending on which of the systems describing x and y
    are up to date and minimized.

    \param y
    A grid that <EM>must</EM> be contained in \p *this;

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Grid_Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void widening_assign(const Grid& y, unsigned* tp = NULL);

  /*! \brief
    Improves the result of the congruence variant of
    \ref Grid_Widening "Grid widening" computation by also enforcing
    those congruences in \p cgs that are satisfied by all the points
    of \p *this.

    \param y
    A grid that <EM>must</EM> be contained in \p *this;

    \param cgs
    The system of congruences used to improve the widened grid;

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Grid_Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cgs are dimension-incompatible.
  */
  void limited_congruence_extrapolation_assign(const Grid& y,
                                               const Congruence_System& cgs,
                                               unsigned* tp = NULL);

  /*! \brief
    Improves the result of the generator variant of the
    \ref Grid_Widening "Grid widening"
    computation by also enforcing those congruences in \p cgs that are
    satisfied by all the points of \p *this.

    \param y
    A grid that <EM>must</EM> be contained in \p *this;

    \param cgs
    The system of congruences used to improve the widened grid;

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Grid_Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cgs are dimension-incompatible.
  */
  void limited_generator_extrapolation_assign(const Grid& y,
                                              const Congruence_System& cgs,
                                              unsigned* tp = NULL);

  /*! \brief
    Improves the result of the \ref Grid_Widening "Grid widening"
    computation by also enforcing those congruences in \p cgs that are
    satisfied by all the points of \p *this.

    \param y
    A grid that <EM>must</EM> be contained in \p *this;

    \param cgs
    The system of congruences used to improve the widened grid;

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Grid_Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cgs are dimension-incompatible.
  */
  void limited_extrapolation_assign(const Grid& y,
                                    const Congruence_System& cgs,
                                    unsigned* tp = NULL);

  //@} // Space Dimension Preserving Member Functions that May Modify [...]

  //! \name Member Functions that May Modify the Dimension of the Vector Space
  //@{

  /*! \brief
    \ref Adding_New_Dimensions_to_the_Vector_Space "Adds"
    \p m new space dimensions and embeds the old grid in the new
    vector space.

    \param m
    The number of dimensions to add.

    \exception std::length_error
    Thrown if adding \p m new space dimensions would cause the vector
    space to exceed dimension <CODE>max_space_dimension()</CODE>.

    The new space dimensions will be those having the highest indexes
    in the new grid, which is characterized by a system of congruences
    in which the variables which are the new dimensions can have any
    value.  For instance, when starting from the grid \f$\cL \sseq
    \Rset^2\f$ and adding a third space dimension, the result will be
    the grid
    \f[
      \bigl\{\,
        (x, y, z)^\transpose \in \Rset^3
      \bigm|
        (x, y)^\transpose \in \cL
      \,\bigr\}.
    \f]
  */
  void add_space_dimensions_and_embed(dimension_type m);

  /*! \brief
    \ref Adding_New_Dimensions_to_the_Vector_Space "Adds"
    \p m new space dimensions to the grid and does not embed it
    in the new vector space.

    \param m
    The number of space dimensions to add.

    \exception std::length_error
    Thrown if adding \p m new space dimensions would cause the
    vector space to exceed dimension <CODE>max_space_dimension()</CODE>.

    The new space dimensions will be those having the highest indexes
    in the new grid, which is characterized by a system of congruences
    in which the variables running through the new dimensions are all
    constrained to be equal to 0.  For instance, when starting from
    the grid \f$\cL \sseq \Rset^2\f$ and adding a third space
    dimension, the result will be the grid
    \f[
      \bigl\{\,
        (x, y, 0)^\transpose \in \Rset^3
      \bigm|
        (x, y)^\transpose \in \cL
      \,\bigr\}.
    \f]
  */
  void add_space_dimensions_and_project(dimension_type m);

  /*! \brief
    Assigns to \p *this the \ref Concatenating_Polyhedra "concatenation" of
    \p *this and \p y, taken in this order.

    \exception std::length_error
    Thrown if the concatenation would cause the vector space
    to exceed dimension <CODE>max_space_dimension()</CODE>.
  */
  void concatenate_assign(const Grid& y);

  //! Removes all the specified dimensions from the vector space.
  /*!
    \param vars
    The set of Variable objects corresponding to the space dimensions
    to be removed.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the
    Variable objects contained in \p vars.
  */
  void remove_space_dimensions(const Variables_Set& vars);

  /*! \brief
    Removes the higher dimensions of the vector space so that the
    resulting space will have \ref Removing_Dimensions_from_the_Vector_Space
    "dimension \p new_dimension."

    \exception std::invalid_argument
    Thrown if \p new_dimensions is greater than the space dimension of
    \p *this.
  */
  void remove_higher_space_dimensions(dimension_type new_dimension);

  /*! \brief
    Remaps the dimensions of the vector space according to
    a \ref Mapping_the_Dimensions_of_the_Vector_Space "partial function".

    If \p pfunc maps only some of the dimensions of \p *this then the
    rest will be projected away.

    If the highest dimension mapped to by \p pfunc is higher than the
    highest dimension in \p *this then the number of dimensions in \p
    *this will be increased to the highest dimension mapped to by \p
    pfunc.

    \param pfunc
    The partial function specifying the destiny of each space
    dimension.

    The template type parameter Partial_Function must provide
    the following methods.
    \code
      bool has_empty_codomain() const
    \endcode
    returns <CODE>true</CODE> if and only if the represented partial
    function has an empty codomain (i.e., it is always undefined).
    The <CODE>has_empty_codomain()</CODE> method will always be called
    before the methods below.  However, if
    <CODE>has_empty_codomain()</CODE> returns <CODE>true</CODE>, none
    of the functions below will be called.
    \code
      dimension_type max_in_codomain() const
    \endcode
    returns the maximum value that belongs to the codomain of the
    partial function.
    The <CODE>max_in_codomain()</CODE> method is called at most once.
    \code
      bool maps(dimension_type i, dimension_type& j) const
    \endcode
    Let \f$f\f$ be the represented function and \f$k\f$ be the value
    of \p i.  If \f$f\f$ is defined in \f$k\f$, then \f$f(k)\f$ is
    assigned to \p j and <CODE>true</CODE> is returned.  If \f$f\f$ is
    undefined in \f$k\f$, then <CODE>false</CODE> is returned.
    This method is called at most \f$n\f$ times, where \f$n\f$ is the
    dimension of the vector space enclosing the grid.

    The result is undefined if \p pfunc does not encode a partial
    function with the properties described in the
    \ref Mapping_the_Dimensions_of_the_Vector_Space "specification of the mapping operator".
  */
  template <typename Partial_Function>
  void map_space_dimensions(const Partial_Function& pfunc);

  //! Creates \p m copies of the space dimension corresponding to \p var.
  /*!
    \param var
    The variable corresponding to the space dimension to be replicated;

    \param m
    The number of replicas to be created.

    \exception std::invalid_argument
    Thrown if \p var does not correspond to a dimension of the vector
    space.

    \exception std::length_error
    Thrown if adding \p m new space dimensions would cause the vector
    space to exceed dimension <CODE>max_space_dimension()</CODE>.

    If \p *this has space dimension \f$n\f$, with \f$n > 0\f$,
    and <CODE>var</CODE> has space dimension \f$k \leq n\f$,
    then the \f$k\f$-th space dimension is
    \ref Expanding_One_Dimension_of_the_Vector_Space_to_Multiple_Dimensions
    "expanded" to \p m new space dimensions
    \f$n\f$, \f$n+1\f$, \f$\dots\f$, \f$n+m-1\f$.
  */
  void expand_space_dimension(Variable var, dimension_type m);

  //! Folds the space dimensions in \p vars into \p dest.
  /*!
    \param vars
    The set of Variable objects corresponding to the space dimensions
    to be folded;

    \param dest
    The variable corresponding to the space dimension that is the
    destination of the folding operation.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p dest or with
    one of the Variable objects contained in \p vars.  Also
    thrown if \p dest is contained in \p vars.

    If \p *this has space dimension \f$n\f$, with \f$n > 0\f$,
    <CODE>dest</CODE> has space dimension \f$k \leq n\f$,
    \p vars is a set of variables whose maximum space dimension
    is also less than or equal to \f$n\f$, and \p dest is not a member
    of \p vars, then the space dimensions corresponding to
    variables in \p vars are
    \ref Folding_Multiple_Dimensions_of_the_Vector_Space_into_One_Dimension "folded"
    into the \f$k\f$-th space dimension.
  */
  void fold_space_dimensions(const Variables_Set& vars, Variable dest);

  //@} // Member Functions that May Modify the Dimension of the Vector Space

  friend bool operator==(const Grid& x, const Grid& y);

  friend class Parma_Polyhedra_Library::Grid_Certificate;

  template <typename Interval> friend class Parma_Polyhedra_Library::Box;

  //! \name Miscellaneous Member Functions
  //@{

  //! Destructor.
  ~Grid();

  /*! \brief
    Swaps \p *this with grid \p y.  (\p *this and \p y can be
    dimension-incompatible.)
  */
  void m_swap(Grid& y);

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  /*! \brief
    Returns a 32-bit hash code for \p *this.

    If \p x and \p y are such that <CODE>x == y</CODE>,
    then <CODE>x.hash_code() == y.hash_code()</CODE>.
  */
  int32_t hash_code() const;

  //@} // Miscellaneous Member Functions

private:

  //! The system of congruences.
  Congruence_System con_sys;

  //! The system of generators.
  Grid_Generator_System gen_sys;

#define PPL_IN_Grid_CLASS
#include "Grid_Status_idefs.hh"
#undef PPL_IN_Grid_CLASS

  //! The status flags to keep track of the grid's internal state.
  Status status;

  //! The number of dimensions of the enclosing vector space.
  dimension_type space_dim;

  enum Dimension_Kind {
    PARAMETER = 0,
    LINE = 1,
    GEN_VIRTUAL = 2,
    PROPER_CONGRUENCE = PARAMETER,
    CON_VIRTUAL = LINE,
    EQUALITY = GEN_VIRTUAL
  };

  typedef std::vector<Dimension_Kind> Dimension_Kinds;

  // The type of row associated with each dimension.  If the virtual
  // rows existed then the reduced systems would be square and upper
  // or lower triangular, and the rows in each would have the types
  // given in this vector.  As the congruence system is reduced to an
  // upside-down lower triangular form the ordering of the congruence
  // types is last to first.
  Dimension_Kinds dim_kinds;

  //! Builds a grid universe or empty grid.
  /*!
    \param num_dimensions
    The number of dimensions of the vector space enclosing the grid;

    \param kind
    specifies whether the universe or the empty grid has to be built.
  */
  void construct(dimension_type num_dimensions, Degenerate_Element kind);

  //! Builds a grid from a system of congruences.
  /*!
    The grid inherits the space dimension of the congruence system.

    \param cgs
    The system of congruences defining the grid. Its data-structures
    may be recycled to build the grid.
  */
  void construct(Congruence_System& cgs);

  //! Builds a grid from a system of grid generators.
  /*!
    The grid inherits the space dimension of the generator system.

    \param ggs
    The system of grid generators defining the grid.  Its data-structures
    may be recycled to build the grid.
  */
  void construct(Grid_Generator_System& ggs);

  //! \name Private Verifiers: Verify if Individual Flags are Set
  //@{

  //! Returns <CODE>true</CODE> if the grid is known to be empty.
  /*!
    The return value <CODE>false</CODE> does not necessarily
    implies that \p *this is non-empty.
  */
  bool marked_empty() const;

  //! Returns <CODE>true</CODE> if the system of congruences is up-to-date.
  bool congruences_are_up_to_date() const;

  //! Returns <CODE>true</CODE> if the system of generators is up-to-date.
  bool generators_are_up_to_date() const;

  //! Returns <CODE>true</CODE> if the system of congruences is minimized.
  bool congruences_are_minimized() const;

  //! Returns <CODE>true</CODE> if the system of generators is minimized.
  bool generators_are_minimized() const;

  //@} // Private Verifiers: Verify if Individual Flags are Set

  //! \name State Flag Setters: Set Only the Specified Flags
  //@{

  /*! \brief
    Sets \p status to express that the grid is the universe
    0-dimension vector space, clearing all corresponding matrices.
  */
  void set_zero_dim_univ();

  /*! \brief
    Sets \p status to express that the grid is empty, clearing all
    corresponding matrices.
  */
  void set_empty();

  //! Sets \p status to express that congruences are up-to-date.
  void set_congruences_up_to_date();

  //! Sets \p status to express that generators are up-to-date.
  void set_generators_up_to_date();

  //! Sets \p status to express that congruences are minimized.
  void set_congruences_minimized();

  //! Sets \p status to express that generators are minimized.
  void set_generators_minimized();

  //@} // State Flag Setters: Set Only the Specified Flags

  //! \name State Flag Cleaners: Clear Only the Specified Flag
  //@{

  //! Clears the \p status flag indicating that the grid is empty.
  void clear_empty();

  //! Sets \p status to express that congruences are out of date.
  void clear_congruences_up_to_date();

  //! Sets \p status to express that generators are out of date.
  void clear_generators_up_to_date();

  //! Sets \p status to express that congruences are no longer minimized.
  void clear_congruences_minimized();

  //! Sets \p status to express that generators are no longer minimized.
  void clear_generators_minimized();

  //@} // State Flag Cleaners: Clear Only the Specified Flag

  //! \name Updating Matrices
  //@{

  //! Updates and minimizes the congruences from the generators.
  void update_congruences() const;

  //! Updates and minimizes the generators from the congruences.
  /*!
    \return
    <CODE>false</CODE> if and only if \p *this turns out to be an
    empty grid.

    It is illegal to call this method when the Status field already
    declares the grid to be empty.
  */
  bool update_generators() const;

  //@} // Updating Matrices

  //! \name Minimization of Descriptions
  //@{

  //! Minimizes both the congruences and the generators.
  /*!
    \return
    <CODE>false</CODE> if and only if \p *this turns out to be an
    empty grid.

    Minimization is performed on each system only if the minimized
    Status field is clear.
  */
  bool minimize() const;

  //@} // Minimization of Descriptions

  enum Three_Valued_Boolean {
    TVB_TRUE,
    TVB_FALSE,
    TVB_DONT_KNOW
  };

  //! Polynomial but incomplete equivalence test between grids.
  Three_Valued_Boolean quick_equivalence_test(const Grid& y) const;

  //! Returns <CODE>true</CODE> if and only if \p *this is included in \p y.
  bool is_included_in(const Grid& y) const;

  //! Checks if and how \p expr is bounded in \p *this.
  /*!
    Returns <CODE>true</CODE> if and only if \p from_above is
    <CODE>true</CODE> and \p expr is bounded from above in \p *this,
    or \p from_above is <CODE>false</CODE> and \p expr is bounded
    from below in \p *this.

    \param expr
    The linear expression to test;

    \param method_call
    The call description of the public parent method, for example
    "bounded_from_above(e)".  Passed to throw_dimension_incompatible,
    as the first argument.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.
  */
  bool bounds(const Linear_Expression& expr, const char* method_call) const;

  //! Maximizes or minimizes \p expr subject to \p *this.
  /*!
    \param expr
    The linear expression to be maximized or minimized subject to \p
    *this;

    \param method_call
    The call description of the public parent method, for example
    "maximize(e)".  Passed to throw_dimension_incompatible, as the
    first argument;

    \param ext_n
    The numerator of the extremum value;

    \param ext_d
    The denominator of the extremum value;

    \param included
    <CODE>true</CODE> if and only if the extremum of \p expr in \p
    *this can actually be reached (which is always the case);

    \param point
    When maximization or minimization succeeds, will be assigned the
    point where \p expr reaches the extremum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded in the appropriate
    direction, <CODE>false</CODE> is returned and \p ext_n, \p ext_d,
    \p included and \p point are left untouched.
  */
  bool max_min(const Linear_Expression& expr,
               const char* method_call,
               Coefficient& ext_n, Coefficient& ext_d, bool& included,
               Generator* point = NULL) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty and
    \ref Grid_Frequency "frequency" for \p *this with respect to \p expr
    is defined, in which case the frequency and the value for \p expr
    that is closest to zero are computed.

    \param expr
    The linear expression for which the frequency is needed;

    \param freq_n
    The numerator of the maximum frequency of \p expr;

    \param freq_d
    The denominator of the maximum frequency of \p expr;

    \param val_n
    The numerator of a value of \p expr at a point in the grid
    that is closest to zero;

    \param val_d
    The denominator of a value of \p expr at a point in the grid
    that is closest to zero;

    If \p *this is empty or frequency is undefined with respect to \p expr,
    then <CODE>false</CODE> is returned and \p freq_n, \p freq_d,
    \p val_n and \p val_d are left untouched.

    \warning
    If \p expr and \p *this are dimension-incompatible,
    the grid generator system is not minimized or \p *this is
    empty, then the behavior is undefined.
  */
  bool frequency_no_check(const Linear_Expression& expr,
                Coefficient& freq_n, Coefficient& freq_d,
                Coefficient& val_n, Coefficient& val_d) const;

  //! Checks if and how \p expr is bounded in \p *this.
  /*!
    Returns <CODE>true</CODE> if and only if \p from_above is
    <CODE>true</CODE> and \p expr is bounded from above in \p *this,
    or \p from_above is <CODE>false</CODE> and \p expr is bounded
    from below in \p *this.

    \param expr
    The linear expression to test;
  */
  bool bounds_no_check(const Linear_Expression& expr) const;

  /*! \brief
    Adds the congruence \p cg to \p *this.

    \warning
    If \p cg and \p *this are dimension-incompatible,
    the grid generator system is not minimized or \p *this is
    empty, then the behavior is undefined.
  */
  void add_congruence_no_check(const Congruence& cg);

  /*! \brief
    Uses the constraint \p c to refine \p *this.

    \param c
    The constraint to be added.

    \exception std::invalid_argument
    Thrown if c is a non-trivial inequality constraint.

    \warning
    If \p c and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void add_constraint_no_check(const Constraint& c);

  /*! \brief
    Uses the constraint \p c to refine \p *this.

    \param c
    The constraint to be added.
    Non-trivial inequalities are ignored.

    \warning
    If \p c and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Constraint& c);

  //! \name Widening- and Extrapolation-Related Functions
  //@{

  //! Copies a widened selection of congruences from \p y to \p selected_cgs.
  void select_wider_congruences(const Grid& y,
                                Congruence_System& selected_cgs) const;

  //! Copies widened generators from \p y to \p widened_ggs.
  void select_wider_generators(const Grid& y,
                               Grid_Generator_System& widened_ggs) const;

  //@} // Widening- and Extrapolation-Related Functions

  //! Adds new space dimensions to the given systems.
  /*!
    \param cgs
    A congruence system, to which columns are added;

    \param gs
    A generator system, to which rows and columns are added;

    \param dims
    The number of space dimensions to add.

    This method is invoked only by
    <CODE>add_space_dimensions_and_embed()</CODE>.
  */
  void add_space_dimensions(Congruence_System& cgs,
                            Grid_Generator_System& gs,
                            dimension_type dims);

  //! Adds new space dimensions to the given systems.
  /*!
    \param gs
    A generator system, to which columns are added;

    \param cgs
    A congruence system, to which rows and columns are added;

    \param dims
    The number of space dimensions to add.

    This method is invoked only by
    <CODE>add_space_dimensions_and_project()</CODE>.
  */
  void add_space_dimensions(Grid_Generator_System& gs,
                            Congruence_System& cgs,
                            dimension_type dims);

  //! \name Minimization-related Static Member Functions
  //@{

  //! Normalizes the divisors in \p sys.
  /*!
    Converts \p sys to an equivalent system in which the divisors are
    of equal value.

    \param sys
    The generator system to be normalized.  It must have at least one
    row.

    \param divisor
    A reference to the initial value of the divisor.  The resulting
    value of this object is the new system divisor.

    \param first_point
    If \p first_point has a value other than NULL then it is taken as
    the first point in \p sys, and it is assumed that any following
    points have the same divisor as \p first_point.
  */
  static void
  normalize_divisors(Grid_Generator_System& sys,
                     Coefficient& divisor,
                     const Grid_Generator* first_point = NULL);

  //! Normalizes the divisors in \p sys.
  /*!
    Converts \p sys to an equivalent system in which the divisors are
    of equal value.

    \param sys
    The generator system to be normalized.  It must have at least one
    row.
  */
  static void
  normalize_divisors(Grid_Generator_System& sys);

  //! Normalize all the divisors in \p sys and \p gen_sys.
  /*!
    Modify \p sys and \p gen_sys to use the same single divisor value
    for all generators, leaving each system representing the grid it
    represented originally.

    \param sys
    The first of the generator systems to be normalized.

    \param gen_sys
    The second of the generator systems to be normalized.  This system
    must have at least one row and the divisors of the generators in
    this system must be equal.

    \exception std::runtime_error
    Thrown if all rows in \p gen_sys are lines and/or parameters.
  */
  static void normalize_divisors(Grid_Generator_System& sys,
                                 Grid_Generator_System& gen_sys);

  /*! \brief
    Converts generator system \p dest to be equivalent to congruence
    system \p source.
  */
  static void conversion(Congruence_System& source,
                         Grid_Generator_System& dest,
                         Dimension_Kinds& dim_kinds);

  /*! \brief
    Converts congruence system \p dest to be equivalent to generator
    system \p source.
  */
  static void conversion(Grid_Generator_System& source,
                         Congruence_System& dest,
                         Dimension_Kinds& dim_kinds);

  //! Converts \p cgs to upper triangular (i.e. minimized) form.
  /*!
    Returns <CODE>true</CODE> if \p cgs represents the empty set,
    otherwise returns <CODE>false</CODE>.
  */
  static bool simplify(Congruence_System& cgs,
                       Dimension_Kinds& dim_kinds);

  //! Converts \p gs to lower triangular (i.e. minimized) form.
  /*!
    Expects \p gs to contain at least one point.
  */
  static void simplify(Grid_Generator_System& ggs,
                       Dimension_Kinds& dim_kinds);

  //! Reduces the line \p row using the line \p pivot.
  /*!
    Uses the line \p pivot to change the representation of the line
    \p row so that the element at index \p column of \p row is zero.
  */
  // A member of Grid for access to Matrix<Dense_Row>::rows.
  static void reduce_line_with_line(Grid_Generator& row,
                                    Grid_Generator& pivot,
                                    dimension_type column);

  //! Reduces the equality \p row using the equality \p pivot.
  /*!
    Uses the equality \p pivot to change the representation of the
    equality \p row so that the element at index \p column of \p row
    is zero.
  */
  // A member of Grid for access to Matrix<Dense_Row>::rows.
  static void reduce_equality_with_equality(Congruence& row,
                                            const Congruence& pivot,
                                            dimension_type column);

  //! Reduces \p row using \p pivot.
  /*!
    Uses the point, parameter or proper congruence at \p pivot to
    change the representation of the point, parameter or proper
    congruence at \p row so that the element at index \p column of \p row
    is zero.  Only elements from index \p start to index \p end are
    modified (i.e. it is assumed that all other elements are zero).
    This means that \p col must be in [start,end).

    NOTE: This may invalidate the rows, since it messes with the divisors.
    Client code has to fix that (if needed) and assert OK().
  */
  // Part of Grid for access to Matrix<Dense_Row>::rows.
  template <typename R>
  static void reduce_pc_with_pc(R& row,
                                R& pivot,
                                dimension_type column,
                                dimension_type start,
                                dimension_type end);

  //! Reduce \p row using \p pivot.
  /*!
    Use the line \p pivot to change the representation of the
    parameter \p row such that the element at index \p column of \p row
    is zero.
  */
  // This takes a parameter with type Swapping_Vector<Grid_Generator> (instead
  // of Grid_Generator_System) to simplify the implementation of `simplify()'.
  // NOTE: This may invalidate `row' and the rows in `sys'. Client code must
  // fix/check this.
  static void reduce_parameter_with_line(Grid_Generator& row,
                                         const Grid_Generator& pivot,
                                         dimension_type column,
                                         Swapping_Vector<Grid_Generator>& sys,
                                         dimension_type num_columns);

  //! Reduce \p row using \p pivot.
  /*!
    Use the equality \p pivot to change the representation of the
    congruence \p row such that element at index \p column of \p row
    is zero.
  */
  // A member of Grid for access to Matrix<Dense_Row>::rows.
  // This takes a parameter with type Swapping_Vector<Congruence> (instead of
  // Congruence_System) to simplify the implementation of `conversion()'.
  static void reduce_congruence_with_equality(Congruence& row,
                                              const Congruence& pivot,
                                              dimension_type column,
                                              Swapping_Vector<Congruence>& sys);

  //! Reduce column \p dim in rows preceding \p pivot_index in \p sys.
  /*!
    Required when converting (or simplifying) a congruence or generator
    system to "strong minimal form"; informally, strong minimal form means
    that, not only is the system in minimal form (ie a triangular matrix),
    but also the absolute values of the coefficients of the proper congruences
    and parameters are minimal. As a simple example, the set of congruences
    \f$\{3x \equiv_3 0, 4x + y \equiv_3 1\}\f$,
    (which is in minimal form) is equivalent to the set
    \f$\{3x \equiv_3 0, x + y \equiv_3 1\}\f$
    (which is in strong minimal form).

    \param sys
    The generator or congruence system to be reduced to strong minimal form.

    \param dim
    Column to be reduced.

    \param pivot_index
    Index of last row to be reduced.

    \param start
    Index of first column to be changed.

    \param end
    Index of last column to be changed.

    \param sys_dim_kinds
    Dimension kinds of the elements of \p sys.

    \param generators
    Flag indicating whether \p sys is a congruence or generator system
  */
  template <typename M>
  // This takes a parameter with type `Swapping_Vector<M::row_type>'
  // instead of `M' to simplify the implementation of simplify().
  // NOTE: This may invalidate the rows in `sys'. Client code must
  // fix/check this.
  static void reduce_reduced(Swapping_Vector<typename M::row_type>& sys,
                             dimension_type dim,
                             dimension_type pivot_index,
                             dimension_type start, dimension_type end,
                             const Dimension_Kinds& sys_dim_kinds,
                             bool generators = true);

  //! Multiply the elements of \p dest by \p multiplier.
  // A member of Grid for access to Matrix<Dense_Row>::rows and cgs::operator[].
  // The type of `dest' is Swapping_Vector<Congruence> instead of
  // Congruence_System to simplify the implementation of conversion().
  static void multiply_grid(const Coefficient& multiplier,
                            Congruence& cg,
                            Swapping_Vector<Congruence>& dest,
                            dimension_type num_rows);

  //! Multiply the elements of \p dest by \p multiplier.
  // A member of Grid for access to Grid_Generator::operator[].
  // The type of `dest' is Swapping_Vector<Grid_Generator> instead of
  // Grid_Generator_System to simplify the implementation of conversion().
  // NOTE: This does not check whether the rows are OK(). Client code
  // should do that.
  static void multiply_grid(const Coefficient& multiplier,
                            Grid_Generator& gen,
                            Swapping_Vector<Grid_Generator>& dest,
                            dimension_type num_rows);

  /*! \brief
    If \p sys is lower triangular return <CODE>true</CODE>, else
    return <CODE>false</CODE>.
  */
  static bool lower_triangular(const Congruence_System& sys,
                               const Dimension_Kinds& dim_kinds);

  /*! \brief
    If \p sys is upper triangular return <CODE>true</CODE>, else
    return <CODE>false</CODE>.
  */
  static bool upper_triangular(const Grid_Generator_System& sys,
                               const Dimension_Kinds& dim_kinds);

#ifndef NDEBUG
  //! Checks that trailing rows contain only zero terms.
  /*!
    If all columns contain zero in the rows of \p system from row
    index \p first to row index \p last then return <code>true</code>,
    else return <code>false</code>.  \p row_size gives the number of
    columns in each row.

    This method is only used in assertions in the simplify methods.
  */
  template <typename M, typename R>
  static bool rows_are_zero(M& system,
                            dimension_type first,
                            dimension_type last,
                            dimension_type row_size);
#endif

  //@} // Minimization-Related Static Member Functions

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \name Exception Throwers
  //@{
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
protected:
  void throw_dimension_incompatible(const char* method,
                                    const char* other_name,
                                    dimension_type other_dim) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* gr_name,
                                    const Grid& gr) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* le_name,
                                    const Linear_Expression& le) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* cg_name,
                                    const Congruence& cg) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* c_name,
                                    const Constraint& c) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* g_name,
                                    const Grid_Generator& g) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* g_name,
                                    const Generator& g) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* cgs_name,
                                    const Congruence_System& cgs) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* cs_name,
                                    const Constraint_System& cs) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* gs_name,
                                    const Grid_Generator_System& gs) const;
  void throw_dimension_incompatible(const char* method,
                                    const char* var_name,
                                    Variable var) const;
  void throw_dimension_incompatible(const char* method,
                                    dimension_type required_space_dim) const;

  static void throw_invalid_argument(const char* method,
                                     const char* reason);
  static void throw_invalid_constraint(const char* method,
                                       const char* c_name);
  static void throw_invalid_constraints(const char* method,
                                        const char* cs_name);
  static void throw_invalid_generator(const char* method,
                                      const char* g_name);
  static void throw_invalid_generators(const char* method,
                                       const char* gs_name);
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //@} // Exception Throwers
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)

};

#include "Grid_Status_inlines.hh"
#include "Grid_inlines.hh"
#include "Grid_templates.hh"

#endif // !defined(PPL_Grid_defs_hh)
