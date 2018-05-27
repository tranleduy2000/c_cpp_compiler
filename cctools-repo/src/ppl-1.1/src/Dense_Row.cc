/* Dense_Row class implementation (non-inline functions).
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

#include "ppl-config.h"
#include "Dense_Row_defs.hh"
#include "Coefficient_defs.hh"
#include "assert.hh"
#include "Sparse_Row_defs.hh"
#include <iostream>
#include <iomanip>

namespace PPL = Parma_Polyhedra_Library;

PPL::Dense_Row::Dense_Row(const Sparse_Row& y, dimension_type sz, dimension_type capacity) {
  resize(sz, capacity);
  for (Sparse_Row::const_iterator i = y.begin(),
         i_end = y.lower_bound(sz); i != i_end; ++i)
    (*this)[i.index()] = *i;
  PPL_ASSERT(OK());
}

void
PPL::Dense_Row::resize(dimension_type new_size) {
  if (new_size <= size())
    shrink(new_size);
  else {
    if (new_size > capacity()) {
      // Reallocation is required.
      // TODO: Consider using realloc() here.
      // TODO: Consider using a smarter allocation strategy.
      const dimension_type new_capacity = new_size;
      Coefficient* const new_vec = impl.coeff_allocator.allocate(new_capacity);

      if (impl.vec != 0) {
        memcpy(new_vec, impl.vec, sizeof(Coefficient) * impl.size);
        impl.coeff_allocator.deallocate(impl.vec, impl.capacity);
      }

      impl.vec = new_vec;
      impl.capacity = new_capacity;
    }
    PPL_ASSERT(new_size <= impl.capacity);
    // Construct the additional elements.
    while (impl.size != new_size) {
      new (&impl.vec[impl.size]) Coefficient();
      ++impl.size;
    }
  }
  PPL_ASSERT(size() == new_size);
  PPL_ASSERT(OK());
}

void
PPL::Dense_Row::resize(dimension_type new_size, dimension_type new_capacity) {
  PPL_ASSERT(new_size <= new_capacity);

  if (new_capacity == 0) {
    destroy();
    impl.vec = 0;
    impl.size = 0;
    impl.capacity = 0;

    PPL_ASSERT(size() == new_size);
    PPL_ASSERT(capacity() == new_capacity);
    PPL_ASSERT(OK());

    return;
  }

  if (new_capacity < capacity()) {

    shrink(new_size);

    PPL_ASSERT(impl.size == new_size);

    Coefficient* const new_vec = impl.coeff_allocator.allocate(new_capacity);

    PPL_ASSERT(impl.vec != 0);

    memcpy(new_vec, impl.vec, sizeof(Coefficient) * impl.size);

    impl.coeff_allocator.deallocate(impl.vec, impl.capacity);

    impl.vec = new_vec;
    impl.capacity = new_capacity;
  }
  else {
    if (new_capacity > capacity()) {

      Coefficient* const new_vec = impl.coeff_allocator.allocate(new_capacity);

      if (impl.vec != 0) {
        memcpy(new_vec, impl.vec, sizeof(Coefficient) * impl.size);
        impl.coeff_allocator.deallocate(impl.vec, impl.capacity);
      }

      impl.vec = new_vec;
      impl.capacity = new_capacity;

      resize(new_size);
    }
  }

  PPL_ASSERT(size() == new_size);
  PPL_ASSERT(capacity() == new_capacity);
  PPL_ASSERT(OK());
}

void
PPL::Dense_Row::clear() {
  for (iterator i = begin(), i_end = end(); i != i_end; ++i)
    *i = 0;
}

void
PPL::Dense_Row::add_zeroes_and_shift(dimension_type n, dimension_type i) {
  PPL_ASSERT(i <= size());
  const dimension_type new_size = size() + n;
  if (new_size > capacity()) {
    Dense_Row new_row;
    const dimension_type new_capacity = compute_capacity(new_size, max_size());
    // This may throw.
    new_row.impl.vec = new_row.impl.coeff_allocator.allocate(new_capacity);
    new_row.impl.capacity = new_capacity;

    dimension_type j = i;
    try {
      // Construct coefficients with value 0 in
      // new_row.impl.vec[i ... i + n - 1]
      for ( ; j < i + n; ++j)
        new (&(new_row.impl.vec[j])) Coefficient(0);
    } catch (...) {
      // Destroy the zeroes constructed so far.
      while (j != i) {
        --j;
        new_row.impl.vec[j].~Coefficient();
      }
      // The new_row's destructor will de-allocate the memory.
      throw;
    }

    // Raw-copy the coefficients.
    memcpy(new_row.impl.vec, impl.vec, sizeof(Coefficient) * i);
    memcpy(&(new_row.impl.vec[i + n]), &impl.vec[i],
           sizeof(Coefficient) * (impl.size - i));

    using std::swap;
    swap(impl.vec, new_row.impl.vec);
    swap(impl.capacity, new_row.impl.capacity);

    // *this now owns all coefficients, including the newly-added zeroes.
    impl.size = new_size;

    // The old vec will be de-allocated at the end of this block.

  }
  else {
    memmove(&impl.vec[n + i], &impl.vec[i], sizeof(Coefficient)
            * (impl.size - i));
    impl.size = i;
    const dimension_type target_size = impl.size + n;
    PPL_ASSERT(target_size == i + n);
    try {
      // Construct n zeroes where the moved elements resided.
      while (impl.size != target_size) {
        new (&impl.vec[impl.size]) Coefficient(0);
        ++impl.size;
      }
      impl.size = new_size;
    } catch (...) {
      // impl.vec[impl.size]..impl.vec[target_size-1] are still unconstructed,
      // but impl.vec[target_size]..impl.vec[new_size] are constructed,
      // because the memmove() moved already-constructed objects.

      // NOTE: This loop can't throw, because destructors must not throw.
      for (dimension_type j = target_size; j < new_size; ++j)
        impl.vec[j].~Coefficient();

      throw;
    }
  }

  PPL_ASSERT(OK());
}

void
PPL::Dense_Row::expand_within_capacity(const dimension_type new_size) {
  PPL_ASSERT(new_size <= impl.capacity);
  PPL_ASSERT(size() <= new_size && new_size <= max_size());
  while (impl.size != new_size) {
    new (&impl.vec[impl.size]) Coefficient();
    ++impl.size;
  }
  PPL_ASSERT(size() == new_size);
  PPL_ASSERT(OK());
}

void
PPL::Dense_Row::shrink(dimension_type new_size) {
  PPL_ASSERT(new_size <= size());
  // Since ~Coefficient() does not throw exceptions, nothing here does.

  // We assume construction was done "forward".
  // We thus perform destruction "backward".
  while (impl.size != new_size) {
    --impl.size;
    impl.vec[impl.size].~Coefficient();
  }

  PPL_ASSERT(size() == new_size);
  PPL_ASSERT(OK());
}

PPL::Dense_Row::Dense_Row(const Sparse_Row& row)
  : impl() {

  init(row);

  PPL_ASSERT(size() == row.size());
  PPL_ASSERT(OK());
}

void
PPL::Dense_Row::init(const Sparse_Row& row) {
  impl.capacity = row.size();
  impl.vec = impl.coeff_allocator.allocate(impl.capacity);
  Sparse_Row::const_iterator itr = row.begin();
  Sparse_Row::const_iterator itr_end = row.end();
  while (impl.size != impl.capacity) {
    // Constructs (*this)[impl.size] with row[impl.size].
    if (itr != itr_end && itr.index() == impl.size) {
      new (&impl.vec[impl.size]) Coefficient(*itr);
      ++itr;
    }
    else
      new (&impl.vec[impl.size]) Coefficient();
    ++impl.size;
  }
  PPL_ASSERT(size() == row.size());
  PPL_ASSERT(OK());
}

PPL::Dense_Row&
PPL::Dense_Row::operator=(const Sparse_Row& row) {
  if (size() > row.size()) {
    // TODO: If the shrink() is modified to reallocate a smaller chunk,
    // this can be optimized.
    shrink(row.size());
    Sparse_Row::const_iterator itr = row.begin();
    Sparse_Row::const_iterator itr_end = row.end();
    for (dimension_type i = 0; i < impl.size; ++i) {
      // Computes (*this)[impl.size] = row[impl.size].
      if (itr != itr_end && itr.index() == i) {
        impl.vec[impl.size] = *itr;
        ++itr;
      }
      else
        impl.vec[impl.size] = Coefficient_zero();
    }
  }
  else {
    if (capacity() >= row.size()) {
      // size() <= row.size() <= capacity().
      Sparse_Row::const_iterator itr = row.begin();
      Sparse_Row::const_iterator itr_end = row.end();
      for (dimension_type i = 0; i < impl.size; ++i) {
        // The following code is equivalent to (*this)[i] = row[i].
        if (itr != itr_end && itr.index() == impl.size) {
          new (&impl.vec[impl.size]) Coefficient(*itr);
          ++itr;
        }
        else
          new (&impl.vec[impl.size]) Coefficient();
      }
      // Construct the additional elements.
      for ( ; impl.size != row.size(); ++impl.size) {
        // Constructs (*this)[impl.size] with row[impl.size].
        if (itr != itr_end && itr.index() == impl.size) {
          new (&impl.vec[impl.size]) Coefficient(*itr);
          ++itr;
        }
        else
          new (&impl.vec[impl.size]) Coefficient();
      }
    }
    else {
      // Reallocation is required.
      destroy();
      init(row);
    }
  }
  PPL_ASSERT(size() == row.size());
  PPL_ASSERT(OK());

  return *this;
}

void
PPL::Dense_Row::normalize() {
  Dense_Row& x = *this;
  // Compute the GCD of all the coefficients.
  const dimension_type sz = size();
  dimension_type i = sz;
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  while (i > 0) {
    Coefficient_traits::const_reference x_i = x[--i];
    if (const int x_i_sign = sgn(x_i)) {
      gcd = x_i;
      if (x_i_sign < 0)
        neg_assign(gcd);
      goto compute_gcd;
    }
  }
  // We reach this point only if all the coefficients were zero.
  return;

compute_gcd:
  if (gcd == 1)
    return;
  while (i > 0) {
    Coefficient_traits::const_reference x_i = x[--i];
    if (x_i != 0) {
      // Note: we use the ternary version instead of a more concise
      // gcd_assign(gcd, x_i) to take advantage of the fact that
      // `gcd' will decrease very rapidly (see D. Knuth, The Art of
      // Computer Programming, second edition, Section 4.5.2,
      // Algorithm C, and the discussion following it).  Our
      // implementation of gcd_assign(x, y, z) for checked numbers is
      // optimized for the case where `z' is smaller than `y', so that
      // on checked numbers we gain.  On the other hand, for the
      // implementation of gcd_assign(x, y, z) on GMP's unbounded
      // integers we cannot make any assumption, so here we draw.
      // Overall, we win.
      gcd_assign(gcd, x_i, gcd);
      if (gcd == 1)
        return;
    }
  }
  // Divide the coefficients by the GCD.
  for (dimension_type j = sz; j-- > 0; ) {
    Coefficient& x_j = x[j];
    exact_div_assign(x_j, x_j, gcd);
  }
}

void
PPL::Dense_Row::reset(dimension_type first, dimension_type last) {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= size());
  for (dimension_type i = first; i < last; ++i)
    (*this)[i] = 0;
}

void
PPL::Dense_Row::linear_combine(const Dense_Row& y,
                               Coefficient_traits::const_reference coeff1,
                               Coefficient_traits::const_reference coeff2) {
  Dense_Row& x = *this;
  PPL_ASSERT(x.size() == y.size());

  x.linear_combine(y, coeff1, coeff2, 0, x.size());
}

void
PPL::Dense_Row::linear_combine(const Dense_Row& y,
                               Coefficient_traits::const_reference coeff1,
                               Coefficient_traits::const_reference coeff2,
                               dimension_type start, dimension_type end) {
  Dense_Row& x = *this;
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= x.size());
  PPL_ASSERT(end <= y.size());
  PPL_ASSERT(coeff1 != 0);
  PPL_ASSERT(coeff2 != 0);

  // If coeff1 is 1 and/or coeff2 is 1 or -1, we use an optimized
  // implementation.

  if (coeff1 == 1) {
    if (coeff2 == 1) {
      // Optimized implementation for coeff1==1, coeff2==1.
      for (dimension_type i = start; i < end; ++i)
        if (y[i] != 0)
          x[i] += y[i];
      return;
    }
    if (coeff2 == -1) {
      // Optimized implementation for coeff1==1, coeff2==-1.
      for (dimension_type i = start; i < end; ++i)
        if (y[i] != 0)
          x[i] -= y[i];
      return;
    }
    // Optimized implementation for coeff1==1.
    for (dimension_type i = start; i < end; ++i) {
      Coefficient& x_i = x[i];
      // The test against 0 gives rise to a consistent speed up: see
      // http://www.cs.unipr.it/pipermail/ppl-devel/2009-February/014000.html
      Coefficient_traits::const_reference y_i = y[i];
      if (y_i != 0)
        add_mul_assign(x_i, y_i, coeff2);
    }
    return;
  }

  if (coeff2 == 1) {
    // Optimized implementation for coeff2==1.
    for (dimension_type i = start; i < end; ++i) {
      x[i] *= coeff1;
      if (y[i] != 0)
        x[i] += y[i];
    }
    return;
  }
  if (coeff2 == -1) {
    // Optimized implementation for coeff2==-1.
    for (dimension_type i = start; i < end; ++i) {
      x[i] *= coeff1;
      if (y[i] != 0)
        x[i] -= y[i];
    }
    return;
  }
  // General case.
  for (dimension_type i = start; i < end; ++i) {
    Coefficient& x_i = x[i];
    x[i] *= coeff1;
    // The test against 0 gives rise to a consistent speed up: see
    // http://www.cs.unipr.it/pipermail/ppl-devel/2009-February/014000.html
    Coefficient_traits::const_reference y_i = y[i];
    if (y_i != 0)
      add_mul_assign(x_i, y_i, coeff2);
  }
}

void
PPL::Dense_Row::ascii_dump(std::ostream& s) const {
  const Dense_Row& x = *this;
  const dimension_type x_size = x.size();
  s << "size " << x_size << " ";
  for (dimension_type i = 0; i < x_size; ++i)
    s << x[i] << ' ';
  s << "\n";
}

PPL_OUTPUT_DEFINITIONS_ASCII_ONLY(Dense_Row)

bool
PPL::Dense_Row::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str) || str != "size")
    return false;
  dimension_type new_size;
  if (!(s >> new_size))
    return false;

  resize(new_size);

  for (dimension_type col = 0; col < new_size; ++col)
    if (!(s >> (*this)[col]))
      return false;

  return true;
}

PPL::memory_size_type
PPL::Dense_Row::external_memory_in_bytes(dimension_type /* capacity */) const {
  return external_memory_in_bytes();
}

PPL::memory_size_type
PPL::Dense_Row::external_memory_in_bytes() const {
  memory_size_type n = impl.capacity * sizeof(Coefficient);
  for (dimension_type i = size(); i-- > 0; )
    n += PPL::external_memory_in_bytes(impl.vec[i]);
  return n;
}

bool
PPL::Dense_Row::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  bool is_broken = false;

  if (impl.capacity > max_size()) {
#ifndef NDEBUG
    cerr << "Dense_Row capacity exceeds the maximum allowed size:" << endl
         << "is " << impl.capacity
         << ", should be less than or equal to " << max_size() << "."
         << endl;
#endif
    is_broken = true;
  }

  if (size() > max_size()) {
#ifndef NDEBUG
    cerr << "Dense_Row size exceeds the maximum allowed size:" << endl
         << "is " << size()
         << ", should be less than or equal to " << max_size() << "." << endl;
#endif
    is_broken = true;
  }

  if (impl.capacity < size()) {
#ifndef NDEBUG
    cerr << "Dense_Row is completely broken: capacity is " << impl.capacity
         << ", size is " << size() << "." << endl;
#endif
    is_broken = true;
  }

  if (capacity() == 0) {
    if (impl.vec != 0)
      is_broken = true;
  }
  else {
    if (impl.vec == 0)
      is_broken = true;
  }

  return !is_broken;
}

bool
PPL::Dense_Row::OK(const dimension_type row_size) const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  bool is_broken = !OK();

  // Check the declared size.
  if (size() != row_size) {
#ifndef NDEBUG
    cerr << "Dense_Row size mismatch: is " << size()
         << ", should be " << row_size << "." << endl;
#endif
    is_broken = true;
  }
  return !is_broken;
}

/*! \relates Parma_Polyhedra_Library::Dense_Row */
bool
PPL::operator==(const Dense_Row& x, const Dense_Row& y) {
  const dimension_type x_size = x.size();
  const dimension_type y_size = y.size();

  if (x_size != y_size)
    return false;

  for (dimension_type i = x_size; i-- > 0; )
    if (x[i] != y[i])
      return false;

  return true;
}
