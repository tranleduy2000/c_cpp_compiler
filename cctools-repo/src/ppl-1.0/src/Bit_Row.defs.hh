/* Bit_Row class declaration.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Bit_Row_defs_hh
#define PPL_Bit_Row_defs_hh 1

#include "Bit_Row.types.hh"
#include "globals.types.hh"
#include <iosfwd>
#include <gmpxx.h>
#include <vector>

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates Bit_Row */
void swap(Bit_Row& x, Bit_Row& y);
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps objects referred by \p x and \p y.
/*! \relates Bit_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void
iter_swap(std::vector<Bit_Row>::iterator x,
          std::vector<Bit_Row>::iterator y);

// Put them in the namespace here to declare them friends later.

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are equal.
/*! \relates Bit_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool operator==(const Bit_Row& x, const Bit_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are not equal.
/*! \relates Bit_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool operator!=(const Bit_Row& x, const Bit_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The basic comparison function.
/*! \relates Bit_Row
  Compares \p x with \p y starting from the least significant bits.
  The ordering is total and has the following property: if \p x and \p y
  are two rows seen as sets of naturals, if \p x is a strict subset
  of \p y, then \p x comes before \p y.

  Returns
  - -1 if \p x comes before \p y in the ordering;
  -  0 if \p x and \p y are equal;
  -  1 if \p x comes after \p y in the ordering.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
int compare(const Bit_Row& x, const Bit_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Set-theoretic inclusion test.
/*! \relates Bit_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool subset_or_equal(const Bit_Row& x, const Bit_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Set-theoretic inclusion test: sets \p strict_subset to a Boolean
  indicating whether the inclusion is strict or not.

  \relates Bit_Row
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool subset_or_equal(const Bit_Row& x, const Bit_Row& y,
		     bool& strict_subset);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Set-theoretic strict inclusion test.
/*! \relates Bit_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool strict_subset(const Bit_Row& x, const Bit_Row& y);

} // namespace Parma_Polyhedra_Library

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A row in a matrix of bits.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
class Parma_Polyhedra_Library::Bit_Row {
public:
  //! Default constructor.
  Bit_Row();

  //! Copy constructor.
  Bit_Row(const Bit_Row& y);

  //! Set-union constructor.
  /*!
    Constructs an object containing the set-union of \p y and \p z.
  */
  Bit_Row(const Bit_Row& y, const Bit_Row& z);

  //! Destructor.
  ~Bit_Row();

  //! Assignment operator.
  Bit_Row& operator=(const Bit_Row& y);

  //! Swaps \p *this with \p y.
  void m_swap(Bit_Row& y);

  //! Returns the truth value corresponding to the bit in position \p k.
  bool operator[](unsigned long k) const;

  //! Sets the bit in position \p k.
  void set(unsigned long k);

  //! Sets bits up to position \p k (excluded).
  void set_until(unsigned long k);

  //! Clears the bit in position \p k.
  void clear(unsigned long k);

  //! Clears bits from position \p k (included) onward.
  void clear_from(unsigned long k);

  //! Clears all the bits of the row.
  void clear();

  //! Assigns to \p *this the set-theoretic union of \p x and \p y.
  void union_assign(const Bit_Row& x, const Bit_Row& y);

  //! Assigns to \p *this the set-theoretic intersection of \p x and \p y.
  void intersection_assign(const Bit_Row& x, const Bit_Row& y);

  //! Assigns to \p *this the set-theoretic difference of \p x and \p y.
  void difference_assign(const Bit_Row& x, const Bit_Row& y);


  friend int compare(const Bit_Row& x, const Bit_Row& y);
  friend bool operator==(const Bit_Row& x, const Bit_Row& y);
  friend bool operator!=(const Bit_Row& x, const Bit_Row& y);
  friend bool subset_or_equal(const Bit_Row& x, const Bit_Row& y);
  friend bool subset_or_equal(const Bit_Row& x, const Bit_Row& y,
			      bool& strict_subset);
  friend bool strict_subset(const Bit_Row& x, const Bit_Row& y);

  //! Returns the index of the first set bit or ULONG_MAX if no bit is set.
  unsigned long first() const;

  /*! \brief
    Returns the index of the first set bit after \p position
    or ULONG_MAX if no bit after \p position is set.
  */
  unsigned long next(unsigned long position) const;

  //! Returns the index of the last set bit or ULONG_MAX if no bit is set.
  unsigned long last() const;

  /*! \brief
    Returns the index of the first set bit before \p position
    or ULONG_MAX if no bits before \p position is set.
  */
  unsigned long prev(unsigned long position) const;

  //! Returns the number of set bits in the row.
  unsigned long count_ones() const;

  //! Returns <CODE>true</CODE> if no bit is set in the row.
  bool empty() const;

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Checks if all the invariants are satisfied
  bool OK() const;

private:
  //! Bit-vector representing the row.
  mpz_t vec;

  //! Assigns to \p *this the union of \p y and \p z.
  /*!
    The size of \p y must be be less than or equal to the size of \p z.
    Upon entry, \p vec must have allocated enough space to contain the result.
  */
  void union_helper(const Bit_Row& y, const Bit_Row& z);
};

#include "Bit_Row.inlines.hh"

#endif // !defined(PPL_Bit_Row_defs_hh)
