/* Linear_System class implementation: inline functions.
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

#ifndef PPL_Linear_System_inlines_hh
#define PPL_Linear_System_inlines_hh 1

#include "Bit_Row_defs.hh"
#include "Coefficient_defs.hh"

#include <algorithm>

namespace Parma_Polyhedra_Library {

template <typename Row>
inline memory_size_type
Linear_System<Row>::external_memory_in_bytes() const {
  return rows.external_memory_in_bytes();
}

template <typename Row>
inline memory_size_type
Linear_System<Row>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename Row>
inline bool
Linear_System<Row>::is_sorted() const {
  // The flag `sorted' does not really reflect the sortedness status
  // of a system (if `sorted' evaluates to `false' nothing is known).
  // This assertion is used to ensure that the system
  // is actually sorted when `sorted' value is 'true'.
  PPL_ASSERT(!sorted || check_sorted());
  return sorted;
}

template <typename Row>
inline void
Linear_System<Row>::set_sorted(const bool b) {
  sorted = b;
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(Topology topol, Representation r)
  : rows(),
    space_dimension_(0),
    row_topology(topol),
    index_first_pending(0),
    sorted(true),
    representation_(r) {

  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(Topology topol,
                                  dimension_type space_dim,
                                  Representation r)
  : rows(),
    space_dimension_(0),
    row_topology(topol),
    index_first_pending(0),
    sorted(true),
    representation_(r) {
  set_space_dimension(space_dim);
  PPL_ASSERT(OK());
}

template <typename Row>
inline dimension_type
Linear_System<Row>::first_pending_row() const {
  return index_first_pending;
}

template <typename Row>
inline dimension_type
Linear_System<Row>::num_pending_rows() const {
  PPL_ASSERT(num_rows() >= first_pending_row());
  return num_rows() - first_pending_row();
}

template <typename Row>
inline void
Linear_System<Row>::unset_pending_rows() {
  index_first_pending = num_rows();
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::set_index_first_pending_row(const dimension_type i) {
  index_first_pending = i;
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(const Linear_System& y)
  : rows(y.rows),
    space_dimension_(y.space_dimension_),
    row_topology(y.row_topology),
    representation_(y.representation_) {
  // Previously pending rows may violate sortedness.
  sorted = (y.num_pending_rows() > 0) ? false : y.sorted;
  unset_pending_rows();
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(const Linear_System& y, Representation r)
  : rows(),
    space_dimension_(y.space_dimension_),
    row_topology(y.row_topology),
    representation_(r) {
  rows.resize(y.num_rows());
  for (dimension_type i = 0; i < y.num_rows(); ++i) {
    // Create the copies with the right representation.
    Row row(y.rows[i], r);
    swap(rows[i], row);
  }
  // Previously pending rows may violate sortedness.
  sorted = (y.num_pending_rows() > 0) ? false : y.sorted;
  unset_pending_rows();
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(const Linear_System& y, With_Pending)
  : rows(y.rows),
    space_dimension_(y.space_dimension_),
    row_topology(y.row_topology),
    index_first_pending(y.index_first_pending),
    sorted(y.sorted),
    representation_(y.representation_) {
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(const Linear_System& y, Representation r,
                                  With_Pending)
  : rows(),
    space_dimension_(y.space_dimension_),
    row_topology(y.row_topology),
    index_first_pending(y.index_first_pending),
    sorted(y.sorted),
    representation_(r) {
  rows.resize(y.num_rows());
  for (dimension_type i = 0; i < y.num_rows(); ++i) {
    // Create the copies with the right representation.
    Row row(y.rows[i], r);
    swap(rows[i], row);
  }
  PPL_ASSERT(OK());
}

template <typename Row>
inline Linear_System<Row>&
Linear_System<Row>::operator=(const Linear_System& y) {
  // NOTE: Pending rows are transformed into non-pending ones.
  Linear_System<Row> tmp = y;
  swap(*this, tmp);
  return *this;
}

template <typename Row>
inline void
Linear_System<Row>::assign_with_pending(const Linear_System& y) {
  Linear_System<Row> tmp(y, With_Pending());
  swap(*this, tmp);
}

template <typename Row>
inline void
Linear_System<Row>::m_swap(Linear_System& y) {
  using std::swap;
  swap(rows, y.rows);
  swap(space_dimension_, y.space_dimension_);
  swap(row_topology, y.row_topology);
  swap(index_first_pending, y.index_first_pending);
  swap(sorted, y.sorted);
  swap(representation_, y.representation_);
  PPL_ASSERT(OK());
  PPL_ASSERT(y.OK());
}

template <typename Row>
inline void
Linear_System<Row>::clear() {
  // Note: do NOT modify the value of `row_topology' and `representation'.
  rows.clear();
  index_first_pending = 0;
  sorted = true;
  space_dimension_ = 0;

  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::mark_as_necessarily_closed() {
  PPL_ASSERT(topology() == NOT_NECESSARILY_CLOSED);
  row_topology = NECESSARILY_CLOSED;
  ++space_dimension_;
  for (dimension_type i = num_rows(); i-- > 0; )
    rows[i].mark_as_necessarily_closed();
}

template <typename Row>
inline void
Linear_System<Row>::mark_as_not_necessarily_closed() {
  PPL_ASSERT(topology() == NECESSARILY_CLOSED);
  PPL_ASSERT(space_dimension() > 0);
  row_topology = NOT_NECESSARILY_CLOSED;
  --space_dimension_;
  for (dimension_type i = num_rows(); i-- > 0; )
    rows[i].mark_as_not_necessarily_closed();
}

template <typename Row>
inline void
Linear_System<Row>::set_topology(Topology t) {
  if (topology() == t)
    return;
  for (dimension_type i = num_rows(); i-- > 0; )
    rows[i].set_topology(t);
  row_topology = t;
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::set_necessarily_closed() {
  set_topology(NECESSARILY_CLOSED);
}

template <typename Row>
inline void
Linear_System<Row>::set_not_necessarily_closed() {
  set_topology(NOT_NECESSARILY_CLOSED);
}

template <typename Row>
inline bool
Linear_System<Row>::is_necessarily_closed() const {
  return row_topology == NECESSARILY_CLOSED;
}

template <typename Row>
inline const Row&
Linear_System<Row>::operator[](const dimension_type k) const {
  return rows[k];
}

template <typename Row>
inline typename Linear_System<Row>::iterator
Linear_System<Row>::begin() {
  return rows.begin();
}

template <typename Row>
inline typename Linear_System<Row>::iterator
Linear_System<Row>::end() {
  return rows.end();
}

template <typename Row>
inline typename Linear_System<Row>::const_iterator
Linear_System<Row>::begin() const {
  return rows.begin();
}

template <typename Row>
inline typename Linear_System<Row>::const_iterator
Linear_System<Row>::end() const {
  return rows.end();
}

template <typename Row>
inline bool
Linear_System<Row>::has_no_rows() const {
  return rows.empty();
}

template <typename Row>
inline dimension_type
Linear_System<Row>::num_rows() const {
  return rows.size();
}

template <typename Row>
inline Topology
Linear_System<Row>::topology() const {
  return row_topology;
}

template <typename Row>
inline Representation
Linear_System<Row>::representation() const {
  return representation_;
}

template <typename Row>
inline void
Linear_System<Row>::set_representation(Representation r) {
  representation_ = r;
  for (dimension_type i = 0; i < rows.size(); ++i)
    rows[i].set_representation(r);
  PPL_ASSERT(OK());
}

template <typename Row>
inline dimension_type
Linear_System<Row>::max_space_dimension() {
  return Row::max_space_dimension();
}

template <typename Row>
inline dimension_type
Linear_System<Row>::space_dimension() const {
  return space_dimension_;
}

template <typename Row>
inline void
Linear_System<Row>::set_space_dimension_no_ok(dimension_type space_dim) {
  for (dimension_type i = rows.size(); i-- > 0; )
    rows[i].set_space_dimension_no_ok(space_dim);
  space_dimension_ = space_dim;
}

template <typename Row>
inline void
Linear_System<Row>::set_space_dimension(dimension_type space_dim) {
  set_space_dimension_no_ok(space_dim);
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::remove_row_no_ok(const dimension_type i,
                                     const bool keep_sorted) {
  PPL_ASSERT(i < num_rows());
  const bool was_pending = (i >= index_first_pending);

  if (sorted && keep_sorted && !was_pending) {
    for (dimension_type j = i + 1; j < rows.size(); ++j)
      swap(rows[j], rows[j-1]);
    rows.pop_back();
  }
  else {
    if (!was_pending)
      sorted = false;
    const bool last_row_is_pending = (num_rows() - 1 >= index_first_pending);
    if (was_pending == last_row_is_pending)
      // Either both rows are pending or both rows are not pending.
      swap(rows[i], rows.back());
    else {
      // Pending rows are stored after the non-pending ones.
      PPL_ASSERT(!was_pending);
      PPL_ASSERT(last_row_is_pending);

      // Swap the row with the last non-pending row.
      swap(rows[i], rows[index_first_pending - 1]);

      // Now the (non-pending) row that has to be deleted is between the
      // non-pending and the pending rows.
      swap(rows[i], rows.back());
    }
    rows.pop_back();
  }
  if (!was_pending)
    // A non-pending row has been removed.
    --index_first_pending;
}

template <typename Row>
inline void
Linear_System<Row>::remove_row(const dimension_type i, bool keep_sorted) {
  remove_row_no_ok(i, keep_sorted);
  PPL_ASSERT(OK());
}


template <typename Row>
inline void
Linear_System<Row>::remove_rows(dimension_type first,
                                dimension_type last,
                                bool keep_sorted) {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= num_rows());
  const dimension_type n = last - first;

  if (n == 0)
    return;

  // All the rows that have to be removed must have the same (pending or
  // non-pending) status.
  PPL_ASSERT(first >= index_first_pending || last <= index_first_pending);

  const bool were_pending = (first >= index_first_pending);

  // Move the rows in [first,last) at the end of the system.
  if (sorted && keep_sorted && !were_pending) {
    // Preserve the row ordering.
    for (dimension_type i = last; i < rows.size(); ++i)
      swap(rows[i], rows[i - n]);

    rows.resize(rows.size() - n);

    // `n' non-pending rows have been removed.
    index_first_pending -= n;

    PPL_ASSERT(OK());
    return;
  }

  // We can ignore the row ordering, but we must not mix pending and
  // non-pending rows.

  const dimension_type offset = rows.size() - n - first;
  // We want to swap the rows in [first, last) and
  // [first + offset, last + offset) (note that these intervals may not be
  // disjunct).

  if (index_first_pending == num_rows()) {
    // There are no pending rows.
    PPL_ASSERT(!were_pending);

    swap_row_intervals(first, last, offset);

    rows.resize(rows.size() - n);

    // `n' non-pending rows have been removed.
    index_first_pending -= n;
  }
  else {
    // There are some pending rows in [first + offset, last + offset).
    if (were_pending) {
      // Both intervals contain only pending rows, because the second
      // interval is after the first.

      swap_row_intervals(first, last, offset);

      rows.resize(rows.size() - n);

      // `n' non-pending rows have been removed.
      index_first_pending -= n;
    }
    else {
      PPL_ASSERT(rows.size() - n < index_first_pending);
      PPL_ASSERT(rows.size() > index_first_pending);
      PPL_ASSERT(!were_pending);
      // In the [size() - n, size()) interval there are some non-pending
      // rows and some pending ones. Be careful not to mix them.

      PPL_ASSERT(index_first_pending >= last);
      swap_row_intervals(first, last, index_first_pending - last);

      // Mark the rows that must be deleted as pending.
      index_first_pending -= n;
      first = index_first_pending;
      last = first + n;

      // Move them at the end of the system.
      swap_row_intervals(first, last, num_rows() - last);

      // Actually remove the rows.
      rows.resize(rows.size() - n);
    }
  }

  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::swap_row_intervals(dimension_type first,
                                       dimension_type last,
                                       dimension_type offset) {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last + offset <= num_rows());
#ifndef NDEBUG
  if (first < last) {
    bool first_interval_has_pending_rows = (last > index_first_pending);
    bool second_interval_has_pending_rows = (last + offset > index_first_pending);
    bool first_interval_has_not_pending_rows = (first < index_first_pending);
    bool second_interval_has_not_pending_rows = (first + offset < index_first_pending);
    PPL_ASSERT(first_interval_has_not_pending_rows
               == !first_interval_has_pending_rows);
    PPL_ASSERT(second_interval_has_not_pending_rows
               == !second_interval_has_pending_rows);
    PPL_ASSERT(first_interval_has_pending_rows
               == second_interval_has_pending_rows);
  }
#endif
  if (first + offset < last) {
    // The intervals are not disjunct, make them so.
    const dimension_type k = last - first - offset;
    last -= k;
    offset += k;
  }

  if (first == last)
    // Nothing to do.
    return;

  for (dimension_type i = first; i < last; ++i)
    swap(rows[i], rows[i + offset]);

  if (first < index_first_pending)
    // The swaps involved not pending rows, so they may not be sorted anymore.
    set_sorted(false);

  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::remove_rows(const std::vector<dimension_type>& indexes) {
#ifndef NDEBUG
  {
    // Check that `indexes' is sorted.
    std::vector<dimension_type> sorted_indexes = indexes;
    std::sort(sorted_indexes.begin(), sorted_indexes.end());
    PPL_ASSERT(indexes == sorted_indexes);

    // Check that the last index (if any) is lower than num_rows().
    // This guarantees that all indexes are in [0, num_rows()).
    if (!indexes.empty())
      PPL_ASSERT(indexes.back() < num_rows());
  }
#endif

  if (indexes.empty())
    return;

  const dimension_type rows_size = rows.size();
  typedef std::vector<dimension_type>::const_iterator itr_t;

  // `i' and last_unused_row' start with the value `indexes[0]' instead
  // of `0', because the loop would just increment `last_unused_row' in the
  // preceding iterations.
  dimension_type last_unused_row = indexes[0];
  dimension_type i = indexes[0];
  itr_t itr = indexes.begin();
  itr_t itr_end = indexes.end();
  while (itr != itr_end) {
    // i <= *itr < rows_size
    PPL_ASSERT(i < rows_size);
    if (*itr == i) {
      // The current row has to be removed, don't increment last_unused_row.
      ++itr;
    }
    else {
      // The current row must not be removed, swap it after the last used row.
      swap(rows[last_unused_row], rows[i]);
      ++last_unused_row;
    }
    ++i;
  }

  // Move up the remaining rows, if any.
  for ( ; i < rows_size; ++i) {
    swap(rows[last_unused_row], rows[i]);
    ++last_unused_row;
  }

  PPL_ASSERT(last_unused_row == num_rows() - indexes.size());

  // The rows that have to be removed are now at the end of the system, just
  // remove them.
  rows.resize(last_unused_row);

  // Adjust index_first_pending.
  if (indexes[0] >= index_first_pending) {
    // Removing pending rows only.
  }
  else {
    if (indexes.back() < index_first_pending) {
      // Removing non-pending rows only.
      index_first_pending -= indexes.size();
    }
    else {
      // Removing some pending and some non-pending rows, count the
      // non-pending rows that must be removed.
      // This exploits the fact that `indexes' is sorted by using binary
      // search.
      itr_t j = std::lower_bound(indexes.begin(), indexes.end(),
                                 index_first_pending);
      std::iterator_traits<itr_t>::difference_type
        non_pending = j - indexes.begin();
      index_first_pending -= static_cast<dimension_type>(non_pending);
    }
  }

  // NOTE: This method does *not* call set_sorted(false), because it preserves
  // the relative row ordering.

  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>::remove_trailing_rows(const dimension_type n) {
  PPL_ASSERT(rows.size() >= n);
  rows.resize(rows.size() - n);
  if (first_pending_row() > rows.size())
    index_first_pending = rows.size();
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  for (dimension_type i = num_rows(); i-- > 0; )
    rows[i].permute_space_dimensions(cycle);
  sorted = false;
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_System<Row>
::swap_space_dimensions(Variable v1, Variable v2) {
  PPL_ASSERT(v1.space_dimension() <= space_dimension());
  PPL_ASSERT(v2.space_dimension() <= space_dimension());
  for (dimension_type k = num_rows(); k-- > 0; )
    rows[k].swap_space_dimensions(v1, v2);
  sorted = false;
  PPL_ASSERT(OK());
}

/*! \relates Linear_System */
template <typename Row>
inline bool
operator!=(const Linear_System<Row>& x, const Linear_System<Row>& y) {
  return !(x == y);
}

template <typename Row>
inline bool
Linear_System<Row>::Row_Less_Than::operator()(const Row& x,
                                              const Row& y) const {
  return compare(x, y) < 0;
}

template <typename Row>
inline
Linear_System<Row>::Unique_Compare
::Unique_Compare(const Swapping_Vector<Row>& cont,
                 dimension_type base)
  : container(cont), base_index(base) {
}

template <typename Row>
inline bool
Linear_System<Row>::Unique_Compare
::operator()(dimension_type i, dimension_type j) const {
  return container[base_index + i].is_equal_to(container[base_index + j]);
}

/*! \relates Linear_System */
template <typename Row>
inline void
swap(Linear_System<Row>& x, Linear_System<Row>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Linear_System_inlines_hh)
