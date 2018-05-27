/* Expression_Adapter class declaration.
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

#ifndef PPL_Expression_Adapter_defs_hh
#define PPL_Expression_Adapter_defs_hh 1

#include "Expression_Adapter_types.hh"
#include "Variable_types.hh"
#include "Variables_Set_types.hh"
#include "Dense_Row_defs.hh"
#include "Sparse_Row_defs.hh"

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Adapters' base type (for template meta-programming).
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
class Parma_Polyhedra_Library::Expression_Adapter_Base {
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! An adapter for Linear_Expression objects.
/*!
  The adapters are meant to provide read-only, customized access to the
  Linear_Expression members in Constraint, Generator, Congruence and
  Grid_Generator objects. They typically implement the user-level view
  of these expressions.

  \note
  A few methods implement low-level access routines and will take
  bare indexes as arguments (rather than Variable objects):
  when such a bare index \c i is zero, the inhomogeneous term is meant;
  when the bare index \c i is greater than zero, the coefficient of the
  variable having id <CODE>i - 1</CODE> is meant.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Parma_Polyhedra_Library::Expression_Adapter
  : public Expression_Adapter_Base {
public:
  //! The type of this object.
  typedef Expression_Adapter<T> const_reference;
  //! The type obtained by one-level unwrapping.
  typedef typename T::const_reference inner_type;
  //! The raw, completely unwrapped type.
  typedef typename T::raw_type raw_type;

  //! Returns an adapter after one-level unwrapping.
  inner_type inner() const;

  //! The type of const iterators on coefficients.
  typedef typename raw_type::const_iterator const_iterator;

  //! Returns the current representation of \p *this.
  Representation representation() const;

  //! Iterator pointing to the first nonzero variable coefficient.
  const_iterator begin() const;

  //! Iterator pointing after the last nonzero variable coefficient.
  const_iterator end() const;

  //! Iterator pointing to the first nonzero variable coefficient
  //! of a variable bigger than or equal to \p v.
  const_iterator lower_bound(Variable v) const;

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Returns the coefficient of \p v in \p *this.
  Coefficient_traits::const_reference coefficient(Variable v) const;

  //! Returns the inhomogeneous term of \p *this.
  Coefficient_traits::const_reference inhomogeneous_term() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is zero.
  bool is_zero() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if all the homogeneous
    terms of \p *this are zero.
  */
  bool all_homogeneous_terms_are_zero() const;

  /*! \brief Returns \p true if \p *this is equal to \p y.

    Note that <CODE>(*this == y)</CODE> has a completely different meaning.
  */
  template <typename Expression>
  bool is_equal_to(const Expression& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if the coefficient of each variable in
    \p vars is zero.
  */
  bool all_zeroes(const Variables_Set& vars) const;

  //! Returns the \p i -th coefficient.
  Coefficient_traits::const_reference get(dimension_type i) const;

  //! Returns the coefficient of variable \p v.
  Coefficient_traits::const_reference get(Variable v) const;

  /*! \brief
    Returns <CODE>true</CODE> if (*this)[i] is zero,
    for each i in [start, end).
  */
  bool all_zeroes(dimension_type start, dimension_type end) const;

  //! Returns the number of zero coefficient in [start, end).
  dimension_type num_zeroes(dimension_type start, dimension_type end) const;

  /*! \brief
    Returns the gcd of the nonzero coefficients in [start,end).
    Returns zero if all the coefficients in the range are zero.
  */
  Coefficient gcd(dimension_type start, dimension_type end) const;

  //! Returns the index of the last nonzero element, or zero if there are no
  //! nonzero elements.
  dimension_type last_nonzero() const;

  //! Returns the index of the last nonzero element in [first,last),
  //! or \p last if there are no nonzero elements.
  dimension_type last_nonzero(dimension_type first, dimension_type last) const;

  //! Returns the index of the first nonzero element, or \p last if there
  //! are no nonzero elements, considering only elements in [first,last).
  dimension_type first_nonzero(dimension_type first, dimension_type last) const;

  /*! \brief
    Returns <CODE>true</CODE> if all coefficients in [start,end),
    except those corresponding to variables in \p vars, are zero.
  */
  bool all_zeroes_except(const Variables_Set& vars,
                         dimension_type start, dimension_type end) const;

  //! Removes from set \p x all the indexes of nonzero elements in \p *this.
  void has_a_free_dimension_helper(std::set<dimension_type>& x) const;

  //! Returns \c true if <CODE>(*this)[i]</CODE> is equal to <CODE>y[i]</CODE>,
  //! for each i in [start,end).
  template <typename Expression>
  bool is_equal_to(const Expression& y,
                   dimension_type start, dimension_type end) const;

  //! Returns \c true if <CODE>(*this)[i]*c1</CODE> is equal to
  //! <CODE>y[i]*c2</CODE>, for each i in [start,end).
  template <typename Expression>
  bool is_equal_to(const Expression& y,
                   Coefficient_traits::const_reference c1,
                   Coefficient_traits::const_reference c2,
                   dimension_type start, dimension_type end) const;

  //! Sets \p row to a copy of the row as adapted by \p *this.
  void get_row(Dense_Row& row) const;

  //! Sets \p row to a copy of the row as adapted by \p *this.
  void get_row(Sparse_Row& row) const;

  //! Returns \c true if there is a variable in [first,last) whose coefficient
  //! is nonzero in both \p *this and \p y.
  template <typename Expression>
  bool have_a_common_variable(const Expression& y,
                              Variable first, Variable last) const;

protected:
  //! Constructor.
  explicit Expression_Adapter(const raw_type& expr);
  //! The raw, completely unwrapped object subject to adaptation.
  const raw_type& raw_;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A transparent adapter for Linear_Expression objects.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Parma_Polyhedra_Library::Expression_Adapter_Transparent
  : public Expression_Adapter<T> {
  typedef Expression_Adapter<T> base_type;
public:
  //! The type of this object.
  typedef Expression_Adapter_Transparent<T> const_reference;
  //! The type obtained by one-level unwrapping.
  typedef typename base_type::inner_type inner_type;
  //! The raw, completely unwrapped type.
  typedef typename base_type::raw_type raw_type;

  //! The type of const iterators on coefficients.
  typedef typename base_type::const_iterator const_iterator;

  //! Constructor.
  explicit Expression_Adapter_Transparent(const raw_type& expr);
};

#include "Expression_Adapter_inlines.hh"

#endif // !defined(PPL_Expression_Adapter_defs_hh)
