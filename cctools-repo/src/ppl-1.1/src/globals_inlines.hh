/* Implementation of global objects: inline functions.
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

#ifndef PPL_globals_inlines_hh
#define PPL_globals_inlines_hh 1

#include "compiler.hh"
#include <limits>
#include <cassert>
#include <istream>
#include <ostream>
#include <cctype>
#include <stdexcept>

namespace Parma_Polyhedra_Library {

inline dimension_type
not_a_dimension() {
  return std::numeric_limits<dimension_type>::max();
}

inline int32_t
hash_code_from_dimension(dimension_type dim) {
  const dimension_type divisor = 1U << (32 - 1);
  dim = dim % divisor;
  return static_cast<int32_t>(dim);
}

inline const Weightwatch_Traits::Threshold&
Weightwatch_Traits::get() {
  return weight;
}

inline bool
Weightwatch_Traits::less_than(const Threshold& a, const Threshold& b) {
  return b - a < (1ULL << (sizeof_to_bits(sizeof(Threshold)) - 1));
}

inline Weightwatch_Traits::Delta
Weightwatch_Traits::compute_delta(unsigned long unscaled, unsigned scale) {
  if ((std::numeric_limits<Delta>::max() >> scale) < unscaled)
    throw std::invalid_argument("PPL::Weightwatch_Traits::"
                                "compute_delta(u, s):\n"
                                "values of u and s cause wrap around.");
  return static_cast<Delta>(unscaled) << scale;
}

inline void
Weightwatch_Traits::from_delta(Threshold& threshold, const Delta& delta) {
  threshold = weight + delta;
}

inline
Throwable::~Throwable() {
}

inline void
maybe_abandon() {
#ifndef NDEBUG
  if (In_Assert::asserting())
    return;
#endif
  if (Weightwatch_Traits::check_function != 0)
    Weightwatch_Traits::check_function();
  if (const Throwable* const p = abandon_expensive_computations)
    p->throw_me();
}

inline dimension_type
compute_capacity(const dimension_type requested_size,
                 const dimension_type maximum_size) {
  assert(requested_size <= maximum_size);
  // Speculation factor 2.
  return (requested_size < maximum_size/2)
    ? (2*(requested_size + 1))
    : maximum_size;
  // Speculation factor 1.5.
  // return (maximum_size - requested_size > requested_size/2)
  //   ? requested_size + requested_size/2 + 1
  //   : maximum_size;
}

template <typename T>
inline typename
Enable_If<Is_Native<T>::value, memory_size_type>::type
external_memory_in_bytes(const T&) {
  return 0;
}

template <typename T>
inline typename
Enable_If<Is_Native<T>::value, memory_size_type>::type
total_memory_in_bytes(const T&) {
  return sizeof(T);
}

inline memory_size_type
external_memory_in_bytes(const mpz_class& x) {
  return static_cast<memory_size_type>(x.get_mpz_t()[0]._mp_alloc)
    * PPL_SIZEOF_MP_LIMB_T;
}

inline memory_size_type
total_memory_in_bytes(const mpz_class& x) {
  return sizeof(x) + external_memory_in_bytes(x);
}

inline memory_size_type
external_memory_in_bytes(const mpq_class& x) {
  return external_memory_in_bytes(x.get_num())
    + external_memory_in_bytes(x.get_den());
}

inline memory_size_type
total_memory_in_bytes(const mpq_class& x) {
  return sizeof(x) + external_memory_in_bytes(x);
}

inline void
ascii_dump(std::ostream& s, Representation r) {
  if (r == DENSE)
    s << "DENSE";
  else
    s << "SPARSE";
}

inline bool
ascii_load(std::istream& is, Representation& r) {
  std::string s;
  if (!(is >> s))
    return false;

  if (s == "DENSE")  {
    r = DENSE;
    return true;
  }
  if (s == "SPARSE")  {
    r = SPARSE;
    return true;
  }
  return false;
}

inline bool
is_space(char c) {
  return isspace(c) != 0;
}

template <typename RA_Container>
inline typename RA_Container::iterator
nth_iter(RA_Container& cont, dimension_type n) {
  typedef typename RA_Container::difference_type diff_t;
  return cont.begin() + static_cast<diff_t>(n);
}

template <typename RA_Container>
inline typename RA_Container::const_iterator
nth_iter(const RA_Container& cont, dimension_type n) {
  typedef typename RA_Container::difference_type diff_t;
  return cont.begin() + static_cast<diff_t>(n);
}

inline dimension_type
least_significant_one_mask(const dimension_type i) {
  return i & (~i + 1U);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_globals_inlines_hh)
