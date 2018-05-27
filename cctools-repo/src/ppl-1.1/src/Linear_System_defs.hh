/* Linear_System class declaration.
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

#ifndef PPL_Linear_System_defs_hh
#define PPL_Linear_System_defs_hh 1

#include "Linear_System_types.hh"

#include "Swapping_Vector_defs.hh"
#include "globals_defs.hh"
#include "Variable_defs.hh"
#include "Variables_Set_defs.hh"

#include "Polyhedron_types.hh"
#include "Bit_Row_types.hh"
#include "Bit_Matrix_types.hh"
#include "Generator_System_types.hh"
#include "Topology_types.hh"

// TODO: Check how much of this description is still true.
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The base class for systems of constraints and generators.
/*! \ingroup PPL_CXX_interface
  An object of this class represents either a constraint system
  or a generator system. Each Linear_System object can be viewed
  as a finite sequence of strong-normalized Row objects,
  where each Row implements a constraint or a generator.
  Linear systems are characterized by the matrix of coefficients,
  also encoding the number, size and capacity of Row objects,
  as well as a few additional information, including:
   - the topological kind of (all) the rows;
   - an indication of whether or not some of the rows in the Linear_System
     are <EM>pending</EM>, meaning that they still have to undergo
     an (unspecified) elaboration; if there are pending rows, then these
     form a proper suffix of the overall sequence of rows;
   - a Boolean flag that, when <CODE>true</CODE>, ensures that the
     non-pending prefix of the sequence of rows is sorted.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
class Parma_Polyhedra_Library::Linear_System {
public:

  // NOTE: `iterator' is actually a const_iterator.
  typedef typename Swapping_Vector<Row>::const_iterator iterator;
  typedef typename Swapping_Vector<Row>::const_iterator const_iterator;

  //! Builds an empty linear system with specified topology.
  /*!
    Rows size and capacity are initialized to \f$0\f$.
  */
  Linear_System(Topology topol, Representation r);

  //! Builds a system with specified topology and dimensions.
  /*!
    \param topol
    The topology of the system that will be created;

    \param space_dim
    The number of space dimensions of the system that will be created.

    \param r
    The representation for system's rows.

    Creates a \p n_rows \f$\times\f$ \p space_dim system whose
    coefficients are all zero and with the given topology.
  */
  Linear_System(Topology topol, dimension_type space_dim, Representation r);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! A tag class.
  /*! \ingroup PPL_CXX_interface
    Tag class to differentiate the Linear_System copy constructor that
    copies pending rows as pending from the one that transforms
    pending rows into non-pending ones.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  struct With_Pending {
  };

  //! Copy constructor: pending rows are transformed into non-pending ones.
  Linear_System(const Linear_System& y);

  //! Copy constructor with specified representation. Pending rows are
  //! transformed into non-pending ones.
  Linear_System(const Linear_System& y, Representation r);

  //! Full copy constructor: pending rows are copied as pending.
  Linear_System(const Linear_System& y, With_Pending);

  //! Full copy constructor: pending rows are copied as pending.
  Linear_System(const Linear_System& y, Representation r, With_Pending);

  //! Assignment operator: pending rows are transformed into non-pending ones.
  Linear_System& operator=(const Linear_System& y);

  //! Full assignment operator: pending rows are copied as pending.
  void assign_with_pending(const Linear_System& y);

  //! Swaps \p *this with \p y.
  void m_swap(Linear_System& y);

  //! Returns the current representation of *this.
  Representation representation() const;

  //! Converts *this to the specified representation.
  void set_representation(Representation r);

  //! Returns the maximum space dimension a Linear_System can handle.
  static dimension_type max_space_dimension();

  //! Returns the space dimension of the rows in the system.
  /*!
    The computation of the space dimension correctly ignores
    the column encoding the inhomogeneous terms of constraint
    (resp., the divisors of generators);
    if the system topology is <CODE>NOT_NECESSARILY_CLOSED</CODE>,
    also the column of the \f$\epsilon\f$-dimension coefficients
    will be ignored.
  */
  dimension_type space_dimension() const;

  //! Sets the space dimension of the rows in the system to \p space_dim .
  void set_space_dimension(dimension_type space_dim);

  //! Makes the system shrink by removing its \p n trailing rows.
  void remove_trailing_rows(dimension_type n);

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

  // TODO: Consider removing this.
  //! Removes the specified rows. The row ordering of remaining rows is
  //! preserved.
  /*!
    \param indexes specifies a list of row indexes.
                   It must be sorted.
  */
  void remove_rows(const std::vector<dimension_type>& indexes);

  // TODO: Consider making this private.
  //! Removes all the specified dimensions from the system.
  /*!
    The space dimension of the variable with the highest space
    dimension in \p vars must be at most the space dimension
    of \p this.
  */
  void remove_space_dimensions(const Variables_Set& vars);

  //! Shift by \p n positions the coefficients of variables, starting from
  //! the coefficient of \p v. This increases the space dimension by \p n.
  void shift_space_dimensions(Variable v, dimension_type n);

  // TODO: Consider making this private.
  //! Permutes the space dimensions of the matrix.
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

  //! Swaps the coefficients of the variables \p v1 and \p v2 .
  void swap_space_dimensions(Variable v1, Variable v2);

  //! \name Subscript operators
  //@{
  //! Returns a const reference to the \p k-th row of the system.
  const Row& operator[](dimension_type k) const;
  //@} // Subscript operators

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  bool has_no_rows() const;
  dimension_type num_rows() const;

  //! Strongly normalizes the system.
  void strong_normalize();

  //! Sign-normalizes the system.
  void sign_normalize();

  //! \name Accessors
  //@{
  //! Returns the system topology.
  Topology topology() const;

  //! Returns the value of the sortedness flag.
  bool is_sorted() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    the system topology is <CODE>NECESSARILY_CLOSED</CODE>.
  */
  bool is_necessarily_closed() const;

  /*! \brief
    Returns the number of rows in the system
    that represent either lines or equalities.
  */
  dimension_type num_lines_or_equalities() const;

  //! Returns the index of the first pending row.
  dimension_type first_pending_row() const;

  //! Returns the number of rows that are in the pending part of the system.
  dimension_type num_pending_rows() const;
  //@} // Accessors

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is sorted,
    without checking for duplicates.
  */
  bool check_sorted() const;

  //! Sets the system topology to \p t .
  void set_topology(Topology t);

  //! Sets the system topology to <CODE>NECESSARILY_CLOSED</CODE>.
  void set_necessarily_closed();

  //! Sets the system topology to <CODE>NOT_NECESSARILY_CLOSED</CODE>.
  void set_not_necessarily_closed();

  // TODO: Consider removing this, or making it private.
  //! Marks the epsilon dimension as a standard dimension.
  /*!
    The system topology is changed to <CODE>NOT_NECESSARILY_CLOSED</CODE>, and
    the number of space dimensions is increased by 1.
  */
  void mark_as_necessarily_closed();

  // TODO: Consider removing this, or making it private.
  //! Marks the last dimension as the epsilon dimension.
  /*!
    The system topology is changed to <CODE>NECESSARILY_CLOSED</CODE>, and
    the number of space dimensions is decreased by 1.
  */
  void mark_as_not_necessarily_closed();

  //! Sets the index to indicate that the system has no pending rows.
  void unset_pending_rows();

  //! Sets the index of the first pending row to \p i.
  void set_index_first_pending_row(dimension_type i);

  //! Sets the sortedness flag of the system to \p b.
  void set_sorted(bool b);

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

  /*! \brief
    Adds a copy of \p r to the system,
    automatically resizing the system or the row's copy, if needed.
  */
  void insert(const Row& r);

  /*! \brief
    Adds a copy of the given row to the pending part of the system,
    automatically resizing the system or the row, if needed.
  */
  void insert_pending(const Row& r);

  /*! \brief
    Adds \p r to the system, stealing its contents and
    automatically resizing the system or the row, if needed.
  */
  void insert(Row& r, Recycle_Input);

  /*! \brief
    Adds the given row to the pending part of the system, stealing its
    contents and automatically resizing the system or the row, if needed.
  */
  void insert_pending(Row& r, Recycle_Input);

  //! Adds to \p *this a copy of  the rows of \p y.
  /*!
    It is assumed that \p *this has no pending rows.
  */
  void insert(const Linear_System& y);

  //! Adds a copy of the rows of `y' to the pending part of `*this'.
  void insert_pending(const Linear_System& r);

  //! Adds to \p *this a the rows of `y', stealing them from `y'.
  /*!
    It is assumed that \p *this has no pending rows.
  */
  void insert(Linear_System& r, Recycle_Input);

  //! Adds the rows of `y' to the pending part of `*this', stealing them from
  //! `y'.
  void insert_pending(Linear_System& r, Recycle_Input);

  /*! \brief
    Sorts the non-pending rows (in growing order) and eliminates
    duplicated ones.
  */
  void sort_rows();

  /*! \brief
    Sorts the rows (in growing order) form \p first_row to
    \p last_row and eliminates duplicated ones.
  */
  void sort_rows(dimension_type first_row, dimension_type last_row);

  /*! \brief
    Assigns to \p *this the result of merging its rows with
    those of \p y, obtaining a sorted system.

    Duplicated rows will occur only once in the result.
    On entry, both systems are assumed to be sorted and have
    no pending rows.
  */
  void merge_rows_assign(const Linear_System& y);

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
    the right-most space dimensions.
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

  /*! \brief
    Applies Gaussian elimination and back-substitution so as to
    simplify the linear system.
  */
  void simplify();

  //! Clears the system deallocating all its rows.
  void clear();

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.

    Reads into a Linear_System object the information produced by the
    output of ascii_dump(std::ostream&) const.  The specialized methods
    provided by Constraint_System and Generator_System take care of
    properly reading the contents of the system.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! The vector that contains the rows.
  /*!
    \note This is public for convenience. Clients that modify if must preserve
          the class invariant.
  */
  Swapping_Vector<Row> rows;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

private:
  //! Makes the system shrink by removing its i-th row.
  /*!
    When \p keep_sorted is \p true and the system is sorted, sortedness will
    be preserved, but this method costs O(n).

    Otherwise, this method just swaps the i-th row with the last and then
    removes it, so it costs O(1).

    This method is for internal use, it does *not* assert OK() at the end,
    so it can be used for invalid systems.
  */
  void remove_row_no_ok(dimension_type i, bool keep_sorted = false);

  /*! \brief
    Adds \p r to the pending part of the system, stealing its contents and
    automatically resizing the system or the row, if needed.

    This method is for internal use, it does *not* assert OK() at the end,
    so it can be used for invalid systems.
  */
  void insert_pending_no_ok(Row& r, Recycle_Input);

  /*! \brief
    Adds \p r to the system, stealing its contents and
    automatically resizing the system or the row, if needed.

    This method is for internal use, it does *not* assert OK() at the end,
    so it can be used for invalid systems.
  */
  void insert_no_ok(Row& r, Recycle_Input);

  //! Sets the space dimension of the rows in the system to \p space_dim .
  /*!
    This method is for internal use, it does *not* assert OK() at the end,
    so it can be used for invalid systems.
  */
  void set_space_dimension_no_ok(dimension_type space_dim);

  //! Swaps the [first,last) row interval with the
  //! [first + offset, last + offset) interval.
  /*!
    These intervals may not be disjunct.

    Sorting of these intervals is *not* preserved.

    Either both intervals contain only not-pending rows, or they both
    contain pending rows.
  */
  void swap_row_intervals(dimension_type first, dimension_type last,
                          dimension_type offset);

  //! The space dimension of each row. All rows must have this number of
  //! space dimensions.
  dimension_type space_dimension_;

  //! The topological kind of the rows in the system. All rows must have this
  //! topology.
  Topology row_topology;

  //! The index of the first pending row.
  dimension_type index_first_pending;

  /*! \brief
    <CODE>true</CODE> if rows are sorted in the ascending order as defined by
    <CODE>bool compare(const Row&, const Row&)</CODE>.
    If <CODE>false</CODE> may not be sorted.
  */
  bool sorted;

  Representation representation_;

  //! Ordering predicate (used when implementing the sort algorithm).
  struct Row_Less_Than {
    bool operator()(const Row& x, const Row& y) const;
  };

  //! Comparison predicate (used when implementing the unique algorithm).
  struct Unique_Compare {
    Unique_Compare(const Swapping_Vector<Row>& cont,
                   dimension_type base = 0);

    bool operator()(dimension_type i, dimension_type j) const;

    const Swapping_Vector<Row>& container;
    const dimension_type base_index;
  };

  friend class Polyhedron;
  friend class Generator_System;
};

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates Linear_System */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
void swap(Parma_Polyhedra_Library::Linear_System<Row>& x,
          Parma_Polyhedra_Library::Linear_System<Row>& y);

} // namespace std

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are identical.
/*! \relates Linear_System */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
bool operator==(const Linear_System<Row>& x, const Linear_System<Row>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates Linear_System */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
bool operator!=(const Linear_System<Row>& x, const Linear_System<Row>& y);

} // namespace Parma_Polyhedra_Library

#include "Linear_System_inlines.hh"
#include "Linear_System_templates.hh"

#endif // !defined(PPL_Linear_System_defs_hh)
