/* Generator_System class declaration.
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

#ifndef PPL_Generator_System_defs_hh
#define PPL_Generator_System_defs_hh 1

#include "Generator_System_types.hh"

#include "Linear_Expression_types.hh"
#include "Linear_System_defs.hh"
#include "Generator_defs.hh"
#include "Constraint_types.hh"
#include "Poly_Con_Relation_defs.hh"
#include "Polyhedron_types.hh"
#include <iosfwd>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*!
  \relates Parma_Polyhedra_Library::Generator_System
  Writes <CODE>false</CODE> if \p gs is empty.  Otherwise, writes on
  \p s the generators of \p gs, all in one row and separated by ", ".
*/
std::ostream& operator<<(std::ostream& s, const Generator_System& gs);

} // namespace IO_Operators

// TODO: Consider removing this.
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are identical.
/*! \relates Generator_System */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool operator==(const Generator_System& x, const Generator_System& y);

// TODO: Consider removing this.
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates Generator_System */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool operator!=(const Generator_System& x, const Generator_System& y);

/*! \relates Generator_System */
void
swap(Generator_System& x, Generator_System& y);

} // namespace Parma_Polyhedra_Library

//! A system of generators.
/*! \ingroup PPL_CXX_interface
    An object of the class Generator_System is a system of generators,
    i.e., a multiset of objects of the class Generator
    (lines, rays, points and closure points).
    When inserting generators in a system, space dimensions are automatically
    adjusted so that all the generators in the system are defined
    on the same vector space.
    A system of generators which is meant to define a non-empty
    polyhedron must include at least one point: the reason is that
    lines, rays and closure points need a supporting point
    (lines and rays only specify directions while closure points only
    specify points in the topological closure of the NNC polyhedron).

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
  Generator_System gs;
  gs.insert(line(x + 0*y));
    \endcode
    As said above, this system of generators corresponds to
    an empty polyhedron, because the line has no supporting point.
    To define a system of generators that does correspond to
    the \f$x\f$ axis, we can add the following code which
    inserts the origin of the space as a point:
    \code
  gs.insert(point(0*x + 0*y));
    \endcode
    Since space dimensions are automatically adjusted, the following
    code obtains the same effect:
    \code
  gs.insert(point(0*x));
    \endcode
    In contrast, if we had added the following code, we would have
    defined a line parallel to the \f$x\f$ axis through
    the point \f$(0, 1)^\transpose \in \Rset^2\f$.
    \code
  gs.insert(point(0*x + 1*y));
    \endcode

    \par Example 2
    The following code builds a ray having the same direction as
    the positive part of the \f$x\f$ axis in \f$\Rset^2\f$:
    \code
  Generator_System gs;
  gs.insert(ray(x + 0*y));
    \endcode
    To define a system of generators indeed corresponding to the set
    \f[
      \bigl\{\,
        (x, 0)^\transpose \in \Rset^2
      \bigm|
        x \geq 0
      \,\bigr\},
    \f]
    one just has to add the origin:
    \code
  gs.insert(point(0*x + 0*y));
    \endcode

    \par Example 3
    The following code builds a system of generators having four points
    and corresponding to a square in \f$\Rset^2\f$
    (the same as Example 1 for the system of constraints):
    \code
  Generator_System gs;
  gs.insert(point(0*x + 0*y));
  gs.insert(point(0*x + 3*y));
  gs.insert(point(3*x + 0*y));
  gs.insert(point(3*x + 3*y));
    \endcode

    \par Example 4
    By using closure points, we can define the \e kernel
    (i.e., the largest open set included in a given set)
    of the square defined in the previous example.
    Note that a supporting point is needed and, for that purpose,
    any inner point could be considered.
    \code
  Generator_System gs;
  gs.insert(point(x + y));
  gs.insert(closure_point(0*x + 0*y));
  gs.insert(closure_point(0*x + 3*y));
  gs.insert(closure_point(3*x + 0*y));
  gs.insert(closure_point(3*x + 3*y));
    \endcode

    \par Example 5
    The following code builds a system of generators having two points
    and a ray, corresponding to a half-strip in \f$\Rset^2\f$
    (the same as Example 2 for the system of constraints):
    \code
  Generator_System gs;
  gs.insert(point(0*x + 0*y));
  gs.insert(point(0*x + 1*y));
  gs.insert(ray(x - y));
    \endcode

    \note
    After inserting a multiset of generators in a generator system,
    there are no guarantees that an <EM>exact</EM> copy of them
    can be retrieved:
    in general, only an <EM>equivalent</EM> generator system
    will be available, where original generators may have been
    reordered, removed (if they are duplicate or redundant), etc.
*/
class Parma_Polyhedra_Library::Generator_System {
public:
  typedef Generator row_type;

  static const Representation default_representation = SPARSE;

  //! Default constructor: builds an empty system of generators.
  Generator_System(Representation r = default_representation);

  //! Builds the singleton system containing only generator \p g.
  explicit Generator_System(const Generator& g,
                            Representation r = default_representation);

  //! Ordinary copy constructor.
  //! The new Generator_System will have the same representation as `gs'.
  Generator_System(const Generator_System& gs);

  //! Copy constructor with specified representation.
  Generator_System(const Generator_System& gs, Representation r);

  //! Destructor.
  ~Generator_System();

  //! Assignment operator.
  Generator_System& operator=(const Generator_System& y);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Generator_System can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Sets the space dimension of the rows in the system to \p space_dim .
  void set_space_dimension(dimension_type space_dim);

  /*! \brief
    Removes all the generators from the generator system
    and sets its space dimension to 0.
  */
  void clear();

  /*! \brief
    Inserts in \p *this a copy of the generator \p g,
    increasing the number of space dimensions if needed.
  */
  void insert(const Generator& g);

  /*! \brief
    Inserts in \p *this the generator \p g, stealing its contents and
    increasing the number of space dimensions if needed.
  */
  void insert(Generator& g, Recycle_Input);

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  /*! \brief
    Returns the singleton system containing only Generator::zero_dim_point().
  */
  static const Generator_System& zero_dim_univ();

  typedef Generator_System_const_iterator const_iterator;

  //! Returns <CODE>true</CODE> if and only if \p *this has no generators.
  bool empty() const;

  /*! \brief
    Returns the const_iterator pointing to the first generator,
    if \p *this is not empty;
    otherwise, returns the past-the-end const_iterator.
  */
  const_iterator begin() const;

  //! Returns the past-the-end const_iterator.
  const_iterator end() const;

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
  void m_swap(Generator_System& y);

private:

  bool has_no_rows() const;

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

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  void swap_space_dimensions(Variable v1, Variable v2);

  dimension_type num_rows() const;

  //! Adds \p n rows and space dimensions to the system.
  /*!
    \param n
    The number of rows and space dimensions to be added: must be strictly
    positive.

    Turns the system \f$M \in \Rset^r \times \Rset^c\f$ into
    the system \f$N \in \Rset^{r+n} \times \Rset^{c+n}\f$
    such that
    \f$N = \bigl(\genfrac{}{}{0pt}{}{0}{M}\genfrac{}{}{0pt}{}{J}{o}\bigr)\f$,
    where \f$J\f$ is the specular image
    of the \f$n \times n\f$ identity matrix.
  */
  void add_universe_rows_and_space_dimensions(dimension_type n);

  Topology topology() const;

  //! Returns the index of the first pending row.
  dimension_type first_pending_row() const;

  //! Sets the index to indicate that the system has no pending rows.
  void unset_pending_rows();

  //! Sets the sortedness flag of the system to \p b.
  void set_sorted(bool b);

  //! Returns the value of the sortedness flag.
  bool is_sorted() const;

  //! Sets the index of the first pending row to \p i.
  void set_index_first_pending_row(dimension_type i);

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    the system topology is <CODE>NECESSARILY_CLOSED</CODE>.
  */
  bool is_necessarily_closed() const;

  //! Full assignment operator: pending rows are copied as pending.
  void assign_with_pending(const Generator_System& y);

  //! Returns the number of rows that are in the pending part of the system.
  dimension_type num_pending_rows() const;

  /*! \brief
    Sorts the pending rows and eliminates those that also occur
    in the non-pending part of the system.
  */
  void sort_pending_and_remove_duplicates();

  /*! \brief
    Sorts the system, removing duplicates, keeping the saturation
    matrix consistent.

    \param sat
    Bit matrix with rows corresponding to the rows of \p *this.
  */
  void sort_and_remove_with_sat(Bit_Matrix& sat);

  /*! \brief
    Sorts the non-pending rows (in growing order) and eliminates
    duplicated ones.
  */
  void sort_rows();

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is sorted,
    without checking for duplicates.
  */
  bool check_sorted() const;

  /*! \brief
    Returns the number of rows in the system
    that represent either lines or equalities.
  */
  dimension_type num_lines_or_equalities() const;

  //! Makes the system shrink by removing its i-th row.
  /*!
    When \p keep_sorted is \p true and the system is sorted, sortedness will
    be preserved, but this method costs O(n).

    Otherwise, this method just swaps the i-th row with the last and then
    removes it, so it costs O(1).
  */
  void remove_row(dimension_type i, bool keep_sorted = false);

  //! Makes the system shrink by removing the rows in [first,last).
  /*!
    When \p keep_sorted is \p true and the system is sorted, sortedness will
    be preserved, but this method costs O(num_rows()).

    Otherwise, this method just swaps the rows with the last ones and then
    removes them, so it costs O(last - first).
  */
  void remove_rows(dimension_type first, dimension_type last,
                   bool keep_sorted = false);

  //! Removes the specified rows. The row ordering of remaining rows is
  //! preserved.
  /*!
    \param indexes specifies a list of row indexes.
                   It must be sorted.
  */
  void remove_rows(const std::vector<dimension_type>& indexes);

  //! Makes the system shrink by removing its \p n trailing rows.
  void remove_trailing_rows(dimension_type n);

  //! Minimizes the subsystem of equations contained in \p *this.
  /*!
    This method works only on the equalities of the system:
    the system is required to be partially sorted, so that
    all the equalities are grouped at its top; it is assumed that
    the number of equalities is exactly \p n_lines_or_equalities.
    The method finds a minimal system for the equalities and
    returns its rank, i.e., the number of linearly independent equalities.
    The result is an upper triangular subsystem of equalities:
    for each equality, the pivot is chosen starting from
    the right-most columns.
  */
  dimension_type gauss(dimension_type n_lines_or_equalities);

  /*! \brief
    Back-substitutes the coefficients to reduce
    the complexity of the system.

    Takes an upper triangular system having \p n_lines_or_equalities rows.
    For each row, starting from the one having the minimum number of
    coefficients different from zero, computes the expression of an element
    as a function of the remaining ones and then substitutes this expression
    in all the other rows.
  */
  void back_substitute(dimension_type n_lines_or_equalities);

  //! Strongly normalizes the system.
  void strong_normalize();

  /*! \brief
    Assigns to \p *this the result of merging its rows with
    those of \p y, obtaining a sorted system.

    Duplicated rows will occur only once in the result.
    On entry, both systems are assumed to be sorted and have
    no pending rows.
  */
  void merge_rows_assign(const Generator_System& y);

  //! Adds to \p *this a copy of  the rows of \p y.
  /*!
    It is assumed that \p *this has no pending rows.
  */
  void insert(const Generator_System& y);

  //! Adds a copy of the rows of `y' to the pending part of `*this'.
  void insert_pending(const Generator_System& r);

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the singleton system containing only Generator::zero_dim_point().
  */
  static const Generator_System* zero_dim_univ_p;

  friend class Generator_System_const_iterator;

  //! Builds an empty system of generators having the specified topology.
  explicit Generator_System(Topology topol,
                            Representation r = default_representation);

  /*! \brief
    Builds a system of rays/points on a \p space_dim dimensional space. If
    \p topol is <CODE>NOT_NECESSARILY_CLOSED</CODE> the \f$\epsilon\f$
    dimension is added.
  */
  Generator_System(Topology topol, dimension_type space_dim,
                   Representation r = default_representation);

  /*! \brief
    Adjusts \p *this so that it matches the \p new_topology and
    \p new_space_dim (adding or removing columns if needed).
    Returns <CODE>false</CODE> if and only if \p topol is
    equal to <CODE>NECESSARILY_CLOSED</CODE> and \p *this
    contains closure points.
  */
  bool adjust_topology_and_space_dimension(Topology new_topology,
                                           dimension_type new_space_dim);

  /*! \brief
    For each unmatched closure point in \p *this, adds the
    corresponding point.

    It is assumed that the topology of \p *this
    is <CODE>NOT_NECESSARILY_CLOSED</CODE>.
  */
  void add_corresponding_points();

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    contains one or more points.
  */
  bool has_points() const;

  /*! \brief
    For each unmatched point in \p *this, adds the corresponding
    closure point.

    It is assumed that the topology of \p *this
    is <CODE>NOT_NECESSARILY_CLOSED</CODE>.
  */
  void add_corresponding_closure_points();

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    contains one or more closure points.

    Note: the check for the presence of closure points is
    done under the point of view of the user. Namely, we scan
    the generator system using high-level iterators, so that
    closure points that are matching the corresponding points
    will be disregarded.
  */
  bool has_closure_points() const;

  //! Converts this generator system into a non-necessarily closed generator
  //! system.
  void convert_into_non_necessarily_closed();

  //! Returns a constant reference to the \p k- th generator of the system.
  const Generator& operator[](dimension_type k) const;

  /*! \brief
    Returns the relations holding between the generator system
    and the constraint \p c.
  */
  Parma_Polyhedra_Library::Poly_Con_Relation
  relation_with(const Constraint& c) const;

  //! Returns <CODE>true</CODE> if all the generators satisfy \p c.
  bool satisfied_by_all_generators(const Constraint& c) const;

  //! Returns <CODE>true</CODE> if all the generators satisfy \p c.
  /*!
    It is assumed that <CODE>c.is_necessarily_closed()</CODE> holds.
  */
  bool satisfied_by_all_generators_C(const Constraint& c) const;

  //! Returns <CODE>true</CODE> if all the generators satisfy \p c.
  /*!
    It is assumed that <CODE>c.is_necessarily_closed()</CODE> does not hold.
  */
  bool satisfied_by_all_generators_NNC(const Constraint& c) const;

  //! Assigns to a given variable an affine expression.
  /*!
    \param v
    The variable to which the affine transformation is assigned;

    \param expr
    The numerator of the affine transformation:
    \f$\sum_{i = 0}^{n - 1} a_i x_i + b\f$;

    \param denominator
    The denominator of the affine transformation.

    We want to allow affine transformations (see the Introduction) having
    any rational coefficients. Since the coefficients of the
    constraints are integers we must also provide an integer \p denominator
    that will be used as denominator of the affine transformation.
    The denominator is required to be a positive integer.

    The affine transformation assigns to each element of the column containing
    the coefficients of v the follow expression:
    \f[
      \frac{\sum_{i = 0}^{n - 1} a_i x_i + b}
           {\mathrm{denominator}}.
    \f]

    \p expr is a constant parameter and unaltered by this computation.
  */
  void affine_image(Variable v,
                    const Linear_Expression& expr,
                    Coefficient_traits::const_reference denominator);

  //! Returns the number of lines of the system.
  dimension_type num_lines() const;

  //! Returns the number of rays of the system.
  dimension_type num_rays() const;

  //! Removes all the invalid lines and rays.
  /*!
    The invalid lines and rays are those with all
    the homogeneous terms set to zero.
  */
  void remove_invalid_lines_and_rays();

  /*! \brief
    Applies Gaussian elimination and back-substitution so as
    to provide a partial simplification of the system of generators.

    It is assumed that the system has no pending generators.
  */
  void simplify();

  /*! \brief
    Inserts in \p *this a copy of the generator \p g,
    increasing the number of space dimensions if needed.
    It is a pending generator.
  */
  void insert_pending(const Generator& g);

  /*! \brief
    Inserts in \p *this the generator \p g, stealing its contents and
    increasing the number of space dimensions if needed.
    It is a pending generator.
  */
  void insert_pending(Generator& g, Recycle_Input);

  Linear_System<Generator> sys;

  friend bool
  operator==(const Generator_System& x, const Generator_System& y);

  friend class Polyhedron;
};

//! An iterator over a system of generators
/*! \ingroup PPL_CXX_interface
    A const_iterator is used to provide read-only access
    to each generator contained in an object of Generator_System.

    \par Example
    The following code prints the system of generators
    of the polyhedron <CODE>ph</CODE>:
    \code
const Generator_System& gs = ph.generators();
for (Generator_System::const_iterator i = gs.begin(),
        gs_end = gs.end(); i != gs_end; ++i)
  cout << *i << endl;
    \endcode
    The same effect can be obtained more concisely by using
    more features of the STL:
    \code
const Generator_System& gs = ph.generators();
copy(gs.begin(), gs.end(), ostream_iterator<Generator>(cout, "\n"));
    \endcode
*/
class Parma_Polyhedra_Library::Generator_System_const_iterator
  : public std::iterator<std::forward_iterator_tag,
        Generator,
        ptrdiff_t,
        const Generator*,
        const Generator&> {
public:
  //! Default constructor.
  Generator_System_const_iterator();

  //! Ordinary copy constructor.
  Generator_System_const_iterator(const Generator_System_const_iterator& y);

  //! Destructor.
  ~Generator_System_const_iterator();

  //! Assignment operator.
  Generator_System_const_iterator& operator=(const Generator_System_const_iterator& y);

  //! Dereference operator.
  const Generator& operator*() const;

  //! Indirect member selector.
  const Generator* operator->() const;

  //! Prefix increment operator.
  Generator_System_const_iterator& operator++();

  //! Postfix increment operator.
  Generator_System_const_iterator operator++(int);

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this and \p y are identical.
  */
  bool operator==(const Generator_System_const_iterator& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this and \p y are different.
  */
  bool operator!=(const Generator_System_const_iterator& y) const;

private:
  friend class Generator_System;

  //! The const iterator over the Linear_System.
  Linear_System<Generator>::const_iterator i;

  //! A const pointer to the Linear_System.
  const Linear_System<Generator>* gsp;

  //! Constructor.
  Generator_System_const_iterator(const Linear_System<Generator>::const_iterator& iter,
      const Generator_System& gsys);

  /*! \brief
    \p *this skips to the next generator, skipping those
    closure points that are immediately followed by a matching point.
  */
  void skip_forward();
};

// Generator_System_inlines.hh is not included here on purpose.

#endif // !defined(PPL_Generator_System_defs_hh)
