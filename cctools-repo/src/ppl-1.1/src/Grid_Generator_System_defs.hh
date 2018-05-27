/* Grid_Generator_System class declaration.
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

#ifndef PPL_Grid_Generator_System_defs_hh
#define PPL_Grid_Generator_System_defs_hh 1

#include "Grid_Generator_System_types.hh"

#include "Linear_System_defs.hh"
#include "Grid_Generator_defs.hh"
#include "Variables_Set_types.hh"
#include "Polyhedron_types.hh"
#include <iosfwd>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*!
  \relates Parma_Polyhedra_Library::Grid_Generator_System
  Writes <CODE>false</CODE> if \p gs is empty.  Otherwise, writes on
  \p s the generators of \p gs, all in one row and separated by ", ".
*/
std::ostream& operator<<(std::ostream& s, const Grid_Generator_System& gs);

} // namespace IO_Operators

//! Swaps \p x with \p y.
/*! \relates Grid_Generator_System */
void swap(Grid_Generator_System& x, Grid_Generator_System& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are identical.
/*! \relates Grid_Generator_System */
bool operator==(const Grid_Generator_System& x,
                const Grid_Generator_System& y);

} // namespace Parma_Polyhedra_Library

//! A system of grid generators.
/*! \ingroup PPL_CXX_interface
    An object of the class Grid_Generator_System is a system of
    grid generators, i.e., a multiset of objects of the class
    Grid_Generator (lines, parameters and points).
    When inserting generators in a system, space dimensions are
    automatically adjusted so that all the generators in the system
    are defined on the same vector space.
    A system of grid generators which is meant to define a non-empty
    grid must include at least one point: the reason is that
    lines and parameters need a supporting point
    (lines only specify directions while parameters only
    specify direction and distance.

    \par
     In all the examples it is assumed that variables
    <CODE>x</CODE> and <CODE>y</CODE> are defined as follows:
    \code
  Variable x(0);
  Variable y(1);
    \endcode

    \par Example 1
    The following code defines the line having the same direction
    as the \f$x\f$ axis (i.e., the first Cartesian axis)
    in \f$\Rset^2\f$:
    \code
  Grid_Generator_System gs;
  gs.insert(grid_line(x + 0*y));
    \endcode
    As said above, this system of generators corresponds to
    an empty grid, because the line has no supporting point.
    To define a system of generators that does correspond to
    the \f$x\f$ axis, we can add the following code which
    inserts the origin of the space as a point:
    \code
  gs.insert(grid_point(0*x + 0*y));
    \endcode
    Since space dimensions are automatically adjusted, the following
    code obtains the same effect:
    \code
  gs.insert(grid_point(0*x));
    \endcode
    In contrast, if we had added the following code, we would have
    defined a line parallel to the \f$x\f$ axis through
    the point \f$(0, 1)^\transpose \in \Rset^2\f$.
    \code
  gs.insert(grid_point(0*x + 1*y));
    \endcode

    \par Example 2
    The following code builds a system of generators corresponding
    to the grid consisting of all the integral points on the \f$x\f$ axes;
    that is, all points satisfying the congruence relation
    \f[
      \bigl\{\,
        (x, 0)^\transpose \in \Rset^2
      \bigm|
        x \pmod{1}\ 0
      \,\bigr\},
    \f]
    \code
  Grid_Generator_System gs;
  gs.insert(parameter(x + 0*y));
  gs.insert(grid_point(0*x + 0*y));
    \endcode

    \par Example 3
    The following code builds a system of generators having three points
    corresponding to a non-relational grid consisting of all points
    whose coordinates are integer multiple of 3.
    \code
  Grid_Generator_System gs;
  gs.insert(grid_point(0*x + 0*y));
  gs.insert(grid_point(0*x + 3*y));
  gs.insert(grid_point(3*x + 0*y));
    \endcode

    \par Example 4
    By using parameters instead of two of the points we
    can define the same grid as that defined in the previous example.
    Note that there has to be at least one point and, for this purpose,
    any point in the grid could be considered.
    Thus the following code builds two identical grids from the
    grid generator systems \p gs and \p gs1.
    \code
  Grid_Generator_System gs;
  gs.insert(grid_point(0*x + 0*y));
  gs.insert(parameter(0*x + 3*y));
  gs.insert(parameter(3*x + 0*y));
  Grid_Generator_System gs1;
  gs1.insert(grid_point(3*x + 3*y));
  gs1.insert(parameter(0*x + 3*y));
  gs1.insert(parameter(3*x + 0*y));
    \endcode

    \par Example 5
    The following code builds a system of generators having one point and
    a parameter corresponding to all the integral points that
    lie on \f$x + y = 2\f$ in \f$\Rset^2\f$
    \code
  Grid_Generator_System gs;
  gs.insert(grid_point(1*x + 1*y));
  gs.insert(parameter(1*x - 1*y));
    \endcode

    \note
    After inserting a multiset of generators in a grid generator system,
    there are no guarantees that an <EM>exact</EM> copy of them
    can be retrieved:
    in general, only an <EM>equivalent</EM> grid generator system
    will be available, where original generators may have been
    reordered, removed (if they are duplicate or redundant), etc.
*/
class Parma_Polyhedra_Library::Grid_Generator_System {
public:
  typedef Grid_Generator row_type;

  static const Representation default_representation = SPARSE;

  //! Default constructor: builds an empty system of generators.
  explicit Grid_Generator_System(Representation r = default_representation);

  //! Builds the singleton system containing only generator \p g.
  explicit Grid_Generator_System(const Grid_Generator& g,
                                 Representation r = default_representation);

  //! Builds an empty system of generators of dimension \p dim.
  explicit Grid_Generator_System(dimension_type dim,
                                 Representation r = default_representation);

  //! Ordinary copy constructor.
  //! The new Grid_Generator_System will have the same representation as `gs'.
  Grid_Generator_System(const Grid_Generator_System& gs);

  //! Copy constructor with specified representation.
  Grid_Generator_System(const Grid_Generator_System& gs, Representation r);

  //! Destructor.
  ~Grid_Generator_System();

  //! Assignment operator.
  Grid_Generator_System& operator=(const Grid_Generator_System& y);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Grid_Generator_System can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  /*! \brief
    Removes all the generators from the generator system and sets its
    space dimension to 0.
  */
  void clear();

  /*! \brief
    Inserts into \p *this a copy of the generator \p g, increasing the
    number of space dimensions if needed.

    If \p g is an all-zero parameter then the only action is to ensure
    that the space dimension of \p *this is at least the space
    dimension of \p g.
  */
  void insert(const Grid_Generator& g);

  /*! \brief
    Inserts into \p *this the generator \p g, increasing the number of
    space dimensions if needed.
  */
  void insert(Grid_Generator& g, Recycle_Input);

  /*! \brief
    Inserts into \p *this the generators in \p gs, increasing the
    number of space dimensions if needed.
  */
  void insert(Grid_Generator_System& gs, Recycle_Input);

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  /*! \brief
    Returns the singleton system containing only
    Grid_Generator::zero_dim_point().
  */
  static const Grid_Generator_System& zero_dim_univ();

  //! An iterator over a system of grid generators
  /*! \ingroup PPL_CXX_interface
    A const_iterator is used to provide read-only access
    to each generator contained in an object of Grid_Generator_System.

    \par Example
    The following code prints the system of generators
    of the grid <CODE>gr</CODE>:
    \code
  const Grid_Generator_System& ggs = gr.generators();
  for (Grid_Generator_System::const_iterator i = ggs.begin(),
        ggs_end = ggs.end(); i != ggs_end; ++i)
    cout << *i << endl;
    \endcode
    The same effect can be obtained more concisely by using
    more features of the STL:
    \code
  const Grid_Generator_System& ggs = gr.generators();
  copy(ggs.begin(), ggs.end(), ostream_iterator<Grid_Generator>(cout, "\n"));
    \endcode
  */
  class const_iterator
    : public std::iterator<std::forward_iterator_tag,
                           Grid_Generator,
                           ptrdiff_t,
                           const Grid_Generator*,
                           const Grid_Generator&> {
  public:
    //! Default constructor.
    const_iterator();

    //! Ordinary copy constructor.
    const_iterator(const const_iterator& y);

    //! Destructor.
    ~const_iterator();

    //! Assignment operator.
    const_iterator& operator=(const const_iterator& y);

    //! Dereference operator.
    const Grid_Generator& operator*() const;

    //! Indirect member selector.
    const Grid_Generator* operator->() const;

    //! Prefix increment operator.
    const_iterator& operator++();

    //! Postfix increment operator.
    const_iterator operator++(int);

    /*! \brief
      Returns <CODE>true</CODE> if and only if \p *this and \p y are
      identical.
    */
    bool operator==(const const_iterator& y) const;

    /*! \brief
      Returns <CODE>true</CODE> if and only if \p *this and \p y are
      different.
    */
    bool operator!=(const const_iterator& y) const;

  private:
    friend class Grid_Generator_System;

    Linear_System<Grid_Generator>::const_iterator i;

    //! Copy constructor from Linear_System< Grid_Generator>::const_iterator.
    const_iterator(const Linear_System<Grid_Generator>::const_iterator& y);
  };

  //! Returns <CODE>true</CODE> if and only if \p *this has no generators.
  bool empty() const;

  /*! \brief
    Returns the const_iterator pointing to the first generator, if \p
    *this is not empty; otherwise, returns the past-the-end
    const_iterator.
  */
  const_iterator begin() const;

  //! Returns the past-the-end const_iterator.
  const_iterator end() const;

  //! Returns the number of rows (generators) in the system.
  dimension_type num_rows() const;

  //! Returns the number of parameters in the system.
  dimension_type num_parameters() const;

  //! Returns the number of lines in the system.
  dimension_type num_lines() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this contains one or
    more points.
  */
  bool has_points() const;

  //! Returns <CODE>true</CODE> if \p *this is identical to \p y.
  bool is_equal_to(const Grid_Generator_System& y) const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.

    Resizes the matrix of generators using the numbers of rows and columns
    read from \p s, then initializes the coordinates of each generator
    and its type reading the contents from \p s.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Swaps \p *this with \p y.
  void m_swap(Grid_Generator_System& y);

private:
  //! Returns a constant reference to the \p k- th generator of the system.
  const Grid_Generator& operator[](dimension_type k) const;

  //! Assigns to a given variable an affine expression.
  /*!
    \param v
    The variable to which the affine transformation is assigned;

    \param expr
    The numerator of the affine transformation:
    \f$\sum_{i = 0}^{n - 1} a_i x_i + b\f$;

    \param denominator
    The denominator of the affine transformation;

    We allow affine transformations (see the Section \ref
    rational_grid_operations)to have rational
    coefficients. Since the coefficients of linear expressions are
    integers we also provide an integer \p denominator that will
    be used as denominator of the affine transformation.  The
    denominator is required to be a positive integer and its
    default value is 1.

    The affine transformation assigns to every variable \p v, in every
    column, the follow expression:
    \f[
      \frac{\sum_{i = 0}^{n - 1} a_i x_i + b}
           {\mathrm{denominator}}.
    \f]

    \p expr is a constant parameter and unaltered by this computation.
  */
  void affine_image(Variable v,
                    const Linear_Expression& expr,
                    Coefficient_traits::const_reference denominator);

  //! Sets the sortedness flag of the system to \p b.
  void set_sorted(bool b);

  /*! \brief
    Adds \p dims rows and \p dims columns of zeroes to the matrix,
    initializing the added rows as in the universe system.

    \param dims
    The number of rows and columns to be added: must be strictly
    positive.

    Turns the \f$r \times c\f$ matrix \f$A\f$ into the \f$(r+dims)
    \times (c+dims)\f$ matrix
    \f$\bigl(\genfrac{}{}{0pt}{}{A}{0} \genfrac{}{}{0pt}{}{0}{B}\bigr)\f$
    where \f$B\f$ is the \f$dims \times dims\f$ unit matrix of the form
    \f$\bigl(\genfrac{}{}{0pt}{}{1}{0} \genfrac{}{}{0pt}{}{0}{1}\bigr)\f$.
    The matrix is expanded avoiding reallocation whenever possible.
  */
  void add_universe_rows_and_columns(dimension_type dims);

  //! Resizes the system to the specified space dimension.
  void set_space_dimension(dimension_type space_dim);

  //! Removes all the specified dimensions from the generator system.
  /*!
    The space dimension of the variable with the highest space
    dimension in \p vars must be at most the space dimension
    of \p this.
  */
  void remove_space_dimensions(const Variables_Set& vars);

  //! Shift by \p n positions the coefficients of variables, starting from
  //! the coefficient of \p v. This increases the space dimension by \p n.
  void shift_space_dimensions(Variable v, dimension_type n);

  //! Sets the index to indicate that the system has no pending rows.
  void unset_pending_rows();

  //! Permutes the space dimensions of the matrix.
  /*
    \param cycle
    A vector representing a cycle of the permutation according to which the
    columns must be rearranged.

    The \p cycle vector represents a cycle of a permutation of space
    dimensions.
    For example, the permutation
    \f$ \{ x_1 \mapsto x_2, x_2 \mapsto x_3, x_3 \mapsto x_1 \}\f$ can be
    represented by the vector containing \f$ x_1, x_2, x_3 \f$.
  */
  void permute_space_dimensions(const std::vector<Variable>& cycle);

  bool has_no_rows() const;

  //! Makes the system shrink by removing its \p n trailing rows.
  void remove_trailing_rows(dimension_type n);

  void insert_verbatim(const Grid_Generator& g);

  //! Returns the system topology.
  Topology topology() const;

  //! Returns the index of the first pending row.
  dimension_type first_pending_row() const;

  Linear_System<Grid_Generator> sys;

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the singleton system containing only Grid_Generator::zero_dim_point().
  */
  static const Grid_Generator_System* zero_dim_univ_p;

  friend bool
  operator==(const Grid_Generator_System& x, const Grid_Generator_System& y);

  //! Sets the index of the first pending row to \p i.
  void set_index_first_pending_row(dimension_type i);

  //! Removes all the invalid lines and parameters.
  /*!
    The invalid lines and parameters are those with all
    the homogeneous terms set to zero.
  */
  void remove_invalid_lines_and_parameters();

  friend class Polyhedron;
  friend class Grid;
};

// Grid_Generator_System_inlines.hh is not included here on purpose.

#endif // !defined(PPL_Grid_Generator_System_defs_hh)
