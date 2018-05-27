/* Congruence_System class declaration.
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

#ifndef PPL_Congruence_System_defs_hh
#define PPL_Congruence_System_defs_hh 1

#include "Congruence_System_types.hh"
#include "Linear_Expression_types.hh"
#include "Constraint_types.hh"
#include "Congruence_types.hh"
#include "Grid_Generator_types.hh"
#include "Swapping_Vector_defs.hh"
#include "Congruence_defs.hh"
#include "Constraint_System_types.hh"
#include <iosfwd>

namespace Parma_Polyhedra_Library {

/*! \relates Congruence_System */
bool
operator==(const Congruence_System& x, const Congruence_System& y);

}

//! A system of congruences.
/*! \ingroup PPL_CXX_interface
    An object of the class Congruence_System is a system of congruences,
    i.e., a multiset of objects of the class Congruence.
    When inserting congruences in a system, space dimensions are
    automatically adjusted so that all the congruences in the system
    are defined on the same vector space.

    \par
    In all the examples it is assumed that variables
    <CODE>x</CODE> and <CODE>y</CODE> are defined as follows:
    \code
  Variable x(0);
  Variable y(1);
    \endcode

    \par Example 1
    The following code builds a system of congruences corresponding to
    an integer grid in \f$\Rset^2\f$:
    \code
  Congruence_System cgs;
  cgs.insert(x %= 0);
  cgs.insert(y %= 0);
    \endcode
    Note that:
    the congruence system is created with space dimension zero;
    the first and second congruence insertions increase the space
    dimension to \f$1\f$ and \f$2\f$, respectively.

    \par Example 2
    By adding to the congruence system of the previous example,
    the congruence \f$x + y = 1 \pmod{2}\f$:
    \code
  cgs.insert((x + y %= 1) / 2);
    \endcode
    we obtain the grid containing just those integral
    points where the sum of the \p x and \p y values is odd.

    \par Example 3
    The following code builds a system of congruences corresponding to
    the grid in \f$\Zset^2\f$ containing just the integral points on
    the \p x axis:
    \code
  Congruence_System cgs;
  cgs.insert(x %= 0);
  cgs.insert((y %= 0) / 0);
    \endcode

    \note
    After inserting a multiset of congruences in a congruence system,
    there are no guarantees that an <EM>exact</EM> copy of them
    can be retrieved:
    in general, only an <EM>equivalent</EM> congruence system
    will be available, where original congruences may have been
    reordered, removed (if they are trivial, duplicate or
    implied by other congruences), linearly combined, etc.
*/
class Parma_Polyhedra_Library::Congruence_System {
public:

  typedef Congruence row_type;

  static const Representation default_representation = SPARSE;

  //! Default constructor: builds an empty system of congruences.
  explicit Congruence_System(Representation r = default_representation);

  //! Builds an empty (i.e. zero rows) system of dimension \p d.
  explicit Congruence_System(dimension_type d,
                             Representation r = default_representation);

  //! Builds the singleton system containing only congruence \p cg.
  explicit Congruence_System(const Congruence& cg,
                             Representation r = default_representation);

  /*! \brief
    If \p c represents the constraint \f$ e_1 = e_2 \f$, builds the
    singleton system containing only constraint \f$ e_1 = e_2
    \pmod{0}\f$.

    \exception std::invalid_argument
    Thrown if \p c is not an equality constraint.
  */
  explicit Congruence_System(const Constraint& c,
                             Representation r = default_representation);

  //! Builds a system containing copies of any equalities in \p cs.
  explicit Congruence_System(const Constraint_System& cs,
                             Representation r = default_representation);

  //! Ordinary copy constructor.
  /*!
    \note
    The new Congruence_System will have the same Representation as `cgs'
    so that it's indistinguishable from `cgs'.
  */
  Congruence_System(const Congruence_System& cgs);

  //! Copy constructor with specified representation.
  Congruence_System(const Congruence_System& cgs, Representation r);

  //! Destructor.
  ~Congruence_System();

  //! Assignment operator.
  Congruence_System& operator=(const Congruence_System& y);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Congruence_System can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is exactly equal
    to \p y.
  */
  bool is_equal_to(const Congruence_System& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this contains one or
    more linear equalities.
  */
  bool has_linear_equalities() const;

  //! Removes all the congruences and sets the space dimension to 0.
  void clear();

  /*! \brief
    Inserts in \p *this a copy of the congruence \p cg, increasing the
    number of space dimensions if needed.

    The copy of \p cg will be strongly normalized after being
    inserted.
  */
  void insert(const Congruence& cg);

  /*! \brief
    Inserts in \p *this the congruence \p cg, stealing its contents and
    increasing the number of space dimensions if needed.

    \p cg will be strongly normalized.
  */
  void insert(Congruence& cg, Recycle_Input);

  /*! \brief
    Inserts in \p *this a copy of the equality constraint \p c, seen
    as a modulo 0 congruence, increasing the number of space
    dimensions if needed.

    The modulo 0 congruence will be strongly normalized after being
    inserted.

    \exception std::invalid_argument
    Thrown if \p c is a relational constraint.
  */
  void insert(const Constraint& c);

  // TODO: Consider adding a insert(cg, Recycle_Input).

  /*! \brief
    Inserts in \p *this a copy of the congruences in \p y,
    increasing the number of space dimensions if needed.

    The inserted copies will be strongly normalized.
  */
  void insert(const Congruence_System& y);

  /*! \brief
    Inserts into \p *this the congruences in \p cgs, increasing the
    number of space dimensions if needed.
  */
  void insert(Congruence_System& cgs, Recycle_Input);

  //! Initializes the class.
  static void initialize();

  //! Finalizes the class.
  static void finalize();

  //! Returns the system containing only Congruence::zero_dim_false().
  static const Congruence_System& zero_dim_empty();

  //! An iterator over a system of congruences.
  /*! \ingroup PPL_CXX_interface
    A const_iterator is used to provide read-only access
    to each congruence contained in an object of Congruence_System.

    \par Example
    The following code prints the system of congruences
    defining the grid <CODE>gr</CODE>:
    \code
  const Congruence_System& cgs = gr.congruences();
  for (Congruence_System::const_iterator i = cgs.begin(),
         cgs_end = cgs.end(); i != cgs_end; ++i)
    cout << *i << endl;
    \endcode
  */
  class const_iterator
    : public std::iterator<std::forward_iterator_tag,
                           Congruence,
                           ptrdiff_t,
                           const Congruence*,
                           const Congruence&> {
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
    const Congruence& operator*() const;

    //! Indirect member selector.
    const Congruence* operator->() const;

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
    friend class Congruence_System;

    //! The const iterator over the vector of congruences.
    Swapping_Vector<Congruence>::const_iterator i;

    //! A const pointer to the vector of congruences.
    const Swapping_Vector<Congruence>* csp;

    //! Constructor.
    const_iterator(const Swapping_Vector<Congruence>::const_iterator& iter,
                   const Congruence_System& cgs);

    //! \p *this skips to the next non-trivial congruence.
    void skip_forward();
  };

  //! Returns <CODE>true</CODE> if and only if \p *this has no congruences.
  bool empty() const;

  /*! \brief
    Returns the const_iterator pointing to the first congruence, if \p
    *this is not empty; otherwise, returns the past-the-end
    const_iterator.
  */
  const_iterator begin() const;

  //! Returns the past-the-end const_iterator.
  const_iterator end() const;

  //! Checks if all the invariants are satisfied.
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*!
    Returns <CODE>true</CODE> if and only if all rows have space dimension
    space_dimension_, each row in the system is a valid Congruence and the
    space dimension is consistent with the number of congruences.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool OK() const;

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

  //! Returns the number of equalities.
  dimension_type num_equalities() const;

  //! Returns the number of proper congruences.
  dimension_type num_proper_congruences() const;

  //! Swaps \p *this with \p y.
  void m_swap(Congruence_System& y);

  /*! \brief
    Adds \p dims rows and \p dims space dimensions to the matrix,
    initializing the added rows as in the unit congruence system.

    \param dims
    The number of rows and space dimensions to be added: must be strictly
    positive.

    Turns the \f$r \times c\f$ matrix \f$A\f$ into the \f$(r+dims) \times
    (c+dims)\f$ matrix
    \f$\bigl(\genfrac{}{}{0pt}{}{0}{A} \genfrac{}{}{0pt}{}{B}{A}\bigr)\f$
    where \f$B\f$ is the \f$dims \times dims\f$ unit matrix of the form
    \f$\bigl(\genfrac{}{}{0pt}{}{0}{1} \genfrac{}{}{0pt}{}{1}{0}\bigr)\f$.
    The matrix is expanded avoiding reallocation whenever possible.
  */
  void add_unit_rows_and_space_dimensions(dimension_type dims);

  //! Permutes the space dimensions of the system.
  /*!
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

  //! Swaps the columns having indexes \p i and \p j.
  void swap_space_dimensions(Variable v1, Variable v2);

  //! Sets the number of space dimensions to \p new_space_dim.
  /*!
    If \p new_space_dim is lower than the current space dimension, the
    coefficients referring to the removed space dimensions are lost.
  */
  bool set_space_dimension(dimension_type new_space_dim);

  // Note: the following method is protected to allow tests/Grid/congruences2
  // to call it using a derived class.
protected:
  //! Returns <CODE>true</CODE> if \p g satisfies all the congruences.
  bool satisfies_all_congruences(const Grid_Generator& g) const;

private:
  //! Returns the number of rows in the system.
  dimension_type num_rows() const;

  //! Returns \c true if num_rows()==0.
  bool has_no_rows() const;

  //! Returns a constant reference to the \p k- th congruence of the system.
  const Congruence& operator[](dimension_type k) const;

  //! Adjusts all expressions to have the same moduli.
  void normalize_moduli();

  /*! \brief
    Substitutes a given column of coefficients by a given affine
    expression.

    \param v
    Index of the column to which the affine transformation is
    substituted;

    \param expr
    The numerator of the affine transformation:
    \f$\sum_{i = 0}^{n - 1} a_i x_i + b\f$;

    \param denominator
    The denominator of the affine transformation.

    We allow affine transformations (see the Section \ref
    rational_grid_operations) to have rational
    coefficients. Since the coefficients of linear expressions are
    integers we also provide an integer \p denominator that will
    be used as denominator of the affine transformation.  The
    denominator is required to be a positive integer and its default value
    is 1.

    The affine transformation substitutes the matrix of congruences
    by a new matrix whose elements \f${a'}_{ij}\f$ are built from
    the old one \f$a_{ij}\f$ as follows:
    \f[
      {a'}_{ij} =
        \begin{cases}
          a_{ij} * \mathrm{denominator} + a_{iv} * \mathrm{expr}[j]
            \quad \text{for } j \neq v; \\
          \mathrm{expr}[v] * a_{iv}
            \quad \text{for } j = v.
        \end{cases}
    \f]

    \p expr is a constant parameter and unaltered by this computation.
  */
  void affine_preimage(Variable v,
                       const Linear_Expression& expr,
                       Coefficient_traits::const_reference denominator);

  // TODO: Consider making this private.
  /*! \brief
    Concatenates copies of the congruences from \p y onto \p *this.

    \param y
    The congruence system to append to \p this.  The number of rows in
    \p y must be strictly positive.

    The matrix for the new system of congruences is obtained by
    leaving the old system in the upper left-hand side and placing the
    congruences of \p y in the lower right-hand side, and padding
    with zeroes.
  */
  void concatenate(const Congruence_System& y);

  /*! \brief
    Inserts in \p *this the congruence \p cg, stealing its contents and
    increasing the number of space dimensions if needed.

    This method inserts \p cg in the given form, instead of first strong
    normalizing \p cg as \ref insert would do.
  */
  void insert_verbatim(Congruence& cg, Recycle_Input);

  //! Makes the system shrink by removing the rows in [first,last).
  /*!
    If \p keep_sorted is <CODE>true</CODE>, the ordering of the remaining rows
    will be preserved.
  */
  void remove_rows(dimension_type first, dimension_type last,
                   bool keep_sorted);

  void remove_trailing_rows(dimension_type n);

  /*! \brief
    Holds (between class initialization and finalization) a pointer to
    the singleton system containing only Congruence::zero_dim_false().
  */
  static const Congruence_System* zero_dim_empty_p;

  Swapping_Vector<Congruence> rows;

  dimension_type space_dimension_;

  Representation representation_;

  /*! \brief
    Returns <CODE>true</CODE> if and only if any of the dimensions in
    \p *this is free of constraint.

    Any equality or proper congruence affecting a dimension constrains
    that dimension.

    This method assumes the system is in minimal form.
  */
  bool has_a_free_dimension() const;

  friend class Grid;

  friend bool
  operator==(const Congruence_System& x, const Congruence_System& y);
};

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*!
  \relates Parma_Polyhedra_Library::Congruence_System
  Writes <CODE>true</CODE> if \p cgs is empty.  Otherwise, writes on
  \p s the congruences of \p cgs, all in one row and separated by ", ".
*/
std::ostream&
operator<<(std::ostream& s, const Congruence_System& cgs);

} // namespace IO_Operators

/*! \relates Congruence_System */
void
swap(Congruence_System& x, Congruence_System& y);

} // namespace Parma_Polyhedra_Library

// Congruence_System_inlines.hh is not included here on purpose.

#endif // !defined(PPL_Congruence_System_defs_hh)
