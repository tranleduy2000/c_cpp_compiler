/* Linear_System class implementation: non-inline template functions.
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

#ifndef PPL_Linear_System_templates_hh
#define PPL_Linear_System_templates_hh 1

#include "Bit_Matrix_defs.hh"
#include "Linear_Expression_defs.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include <algorithm>
#include <iostream>
#include <string>
#include <deque>

#include "swapping_sort_templates.hh"

namespace Parma_Polyhedra_Library {

template <typename Row>
dimension_type
Linear_System<Row>::num_lines_or_equalities() const {
  PPL_ASSERT(num_pending_rows() == 0);
  const Linear_System& x = *this;
  dimension_type n = 0;
  for (dimension_type i = num_rows(); i-- > 0; )
    if (x[i].is_line_or_equality())
      ++n;
  return n;
}

template <typename Row>
void
Linear_System<Row>::merge_rows_assign(const Linear_System& y) {
  PPL_ASSERT(space_dimension() >= y.space_dimension());
  // Both systems have to be sorted and have no pending rows.
  PPL_ASSERT(check_sorted() && y.check_sorted());
  PPL_ASSERT(num_pending_rows() == 0 && y.num_pending_rows() == 0);

  Linear_System& x = *this;

  // A temporary vector...
  Swapping_Vector<Row> tmp;
  // ... with enough capacity not to require any reallocations.
  tmp.reserve(compute_capacity(x.rows.size() + y.rows.size(),
                               tmp.max_num_rows()));

  dimension_type xi = 0;
  const dimension_type x_num_rows = x.num_rows();
  dimension_type yi = 0;
  const dimension_type y_num_rows = y.num_rows();

  while (xi < x_num_rows && yi < y_num_rows) {
    const int comp = compare(x[xi], y[yi]);
    if (comp <= 0) {
      // Elements that can be taken from `x' are actually _stolen_ from `x'
      tmp.resize(tmp.size() + 1);
      swap(tmp.back(), x.rows[xi++]);
      tmp.back().set_representation(representation());
      if (comp == 0)
        // A duplicate element.
        ++yi;
    }
    else {
      // (comp > 0)
      tmp.resize(tmp.size() + 1);
      Row copy(y[yi++], space_dimension(), representation());
      swap(tmp.back(), copy);
    }
  }
  // Insert what is left.
  if (xi < x_num_rows)
    while (xi < x_num_rows) {
      tmp.resize(tmp.size() + 1);
      swap(tmp.back(), x.rows[xi++]);
      tmp.back().set_representation(representation());
    }
  else
    while (yi < y_num_rows) {
      tmp.resize(tmp.size() + 1);
      Row copy(y[yi++], space_dimension(), representation());
      swap(tmp.back(), copy);
    }

  // We get the result matrix and let the old one be destroyed.
  swap(tmp, rows);
  // There are no pending rows.
  unset_pending_rows();
  PPL_ASSERT(check_sorted());
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::ascii_dump(std::ostream& s) const {
  // Prints the topology, the number of rows, the number of columns
  // and the sorted flag.  The specialized methods provided by
  // Constraint_System and Generator_System take care of properly
  // printing the contents of the system.
  s << "topology " << (is_necessarily_closed()
                       ? "NECESSARILY_CLOSED"
                       : "NOT_NECESSARILY_CLOSED")
    << "\n"
    << num_rows() << " x " << space_dimension() << " ";
  Parma_Polyhedra_Library::ascii_dump(s, representation());
  s << " " << (sorted ? "(sorted)" : "(not_sorted)")
    << "\n"
    << "index_first_pending " << first_pending_row()
    << "\n";
  for (dimension_type i = 0; i < rows.size(); ++i)
    rows[i].ascii_dump(s);
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS_ASCII_ONLY(Row, Linear_System<Row>)

template <typename Row>
bool
Linear_System<Row>::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str) || str != "topology")
    return false;
  if (!(s >> str))
    return false;

  clear();

  Topology t;
  if (str == "NECESSARILY_CLOSED")
    t = NECESSARILY_CLOSED;
  else {
    if (str != "NOT_NECESSARILY_CLOSED")
      return false;
    t = NOT_NECESSARILY_CLOSED;
  }

  set_topology(t);

  dimension_type nrows;
  dimension_type space_dims;
  if (!(s >> nrows))
    return false;
  if (!(s >> str) || str != "x")
    return false;
  if (!(s >> space_dims))
    return false;

  space_dimension_ = space_dims;

  if (!Parma_Polyhedra_Library::ascii_load(s, representation_))
    return false;

  if (!(s >> str) || (str != "(sorted)" && str != "(not_sorted)"))
    return false;
  const bool sortedness = (str == "(sorted)");
  dimension_type index;
  if (!(s >> str) || str != "index_first_pending")
    return false;
  if (!(s >> index))
    return false;

  Row row;
  for (dimension_type i = 0; i < nrows; ++i) {
    if (!row.ascii_load(s))
      return false;
    insert(row, Recycle_Input());
  }
  index_first_pending = index;
  sorted = sortedness;

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

template <typename Row>
void
Linear_System<Row>::insert(const Row& r) {
  Row tmp(r, representation());
  insert(tmp, Recycle_Input());
}

template <typename Row>
void
Linear_System<Row>::insert(Row& r, Recycle_Input) {
  insert_no_ok(r, Recycle_Input());
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::insert_no_ok(Row& r, Recycle_Input) {
  PPL_ASSERT(topology() == r.topology());
  // This method is only used when the system has no pending rows.
  PPL_ASSERT(num_pending_rows() == 0);

  const bool was_sorted = is_sorted();

  insert_pending_no_ok(r, Recycle_Input());

  if (was_sorted) {
    const dimension_type nrows = num_rows();
    // The added row may have caused the system to be not sorted anymore.
    if (nrows > 1) {
      // If the system is not empty and the inserted row is the
      // greatest one, the system is set to be sorted.
      // If it is not the greatest one then the system is no longer sorted.
      sorted = (compare(rows[nrows-2], rows[nrows-1]) <= 0);
    }
    else
      // A system having only one row is sorted.
      sorted = true;
  }

  unset_pending_rows();
}

template <typename Row>
void
Linear_System<Row>::insert_pending_no_ok(Row& r, Recycle_Input) {
  // TODO: A Grid_Generator_System may contain non-normalized lines that
  // represent parameters, so this check is disabled. Consider re-enabling it
  // when it's possibile.
#if 0
  // The added row must be strongly normalized and have the same
  // number of elements as the existing rows of the system.
  PPL_ASSERT(r.check_strong_normalized());
#endif

  PPL_ASSERT(r.topology() == topology());

  r.set_representation(representation());

  if (space_dimension() < r.space_dimension())
    set_space_dimension_no_ok(r.space_dimension());
  else
    r.set_space_dimension_no_ok(space_dimension());

  rows.resize(rows.size() + 1);
  swap(rows.back(), r);
}

template <typename Row>
void
Linear_System<Row>::insert_pending(const Row& r) {
  Row tmp(r, representation());
  insert_pending(tmp, Recycle_Input());
}

template <typename Row>
void
Linear_System<Row>::insert_pending(Row& r, Recycle_Input) {
  insert_pending_no_ok(r, Recycle_Input());
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::insert_pending(const Linear_System& y) {
  Linear_System tmp(y, representation(), With_Pending());
  insert_pending(tmp, Recycle_Input());
}

template <typename Row>
void
Linear_System<Row>::insert_pending(Linear_System& y, Recycle_Input) {
  Linear_System& x = *this;
  PPL_ASSERT(x.space_dimension() == y.space_dimension());

  // Steal the rows of `y'.
  // This loop must use an increasing index (instead of a decreasing one) to
  // preserve the row ordering.
  for (dimension_type i = 0; i < y.num_rows(); ++i)
    x.insert_pending(y.rows[i], Recycle_Input());

  y.clear();

  PPL_ASSERT(x.OK());
}

template <typename Row>
void
Linear_System<Row>::insert(const Linear_System& y) {
  Linear_System tmp(y, representation(), With_Pending());
  insert(tmp, Recycle_Input());
}

template <typename Row>
void
Linear_System<Row>::insert(Linear_System& y, Recycle_Input) {
  PPL_ASSERT(num_pending_rows() == 0);

  // Adding no rows is a no-op.
  if (y.has_no_rows())
    return;

  // Check if sortedness is preserved.
  if (is_sorted()) {
    if (!y.is_sorted() || y.num_pending_rows() > 0)
      sorted = false;
    else {
      // `y' is sorted and has no pending rows.
      const dimension_type n_rows = num_rows();
      if (n_rows > 0)
        sorted = (compare(rows[n_rows-1], y[0]) <= 0);
    }
  }

  // Add the rows of `y' as if they were pending.
  insert_pending(y, Recycle_Input());

  // TODO: May y have pending rows? Should they remain pending?

  // There are no pending_rows.
  unset_pending_rows();

  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::remove_space_dimensions(const Variables_Set& vars) {
  // Dimension-compatibility assertion.
  PPL_ASSERT(space_dimension() >= vars.space_dimension());

  // The removal of no dimensions from any system is a no-op.  This
  // case also captures the only legal removal of dimensions from a
  // 0-dim system.
  if (vars.empty())
    return;

  // NOTE: num_rows() is *not* constant, because it may be decreased by
  // remove_row_no_ok().
  for (dimension_type i = 0; i < num_rows(); ) {
    const bool valid = rows[i].remove_space_dimensions(vars);
    if (!valid) {
      // Remove the current row.
      // We can't call remove_row(i) here, because the system is not OK as
      // some rows already have the new space dimension and others still have
      // the old one.
      remove_row_no_ok(i, false);
    }
    else
      ++i;
  }

  space_dimension_ -= vars.size();

  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::shift_space_dimensions(Variable v, dimension_type n) {
  // NOTE: v.id() may be equal to the space dimension of the system
  // (when no space dimension need to be shifted).
  PPL_ASSERT(v.id() <= space_dimension());
  for (dimension_type i = rows.size(); i-- > 0; )
    rows[i].shift_space_dimensions(v, n);
  space_dimension_ += n;
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::sort_rows() {
  // We sort the non-pending rows only.
  sort_rows(0, first_pending_row());
  sorted = true;
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::sort_rows(const dimension_type first_row,
                              const dimension_type last_row) {
  PPL_ASSERT(first_row <= last_row && last_row <= num_rows());
  // We cannot mix pending and non-pending rows.
  PPL_ASSERT(first_row >= first_pending_row()
             || last_row <= first_pending_row());

  const bool sorting_pending = (first_row >= first_pending_row());
  const dimension_type old_num_pending = num_pending_rows();

  const dimension_type num_elems = last_row - first_row;
  if (num_elems < 2)
    return;

  // Build the function objects implementing indirect sort comparison,
  // indirect unique comparison and indirect swap operation.
  using namespace Implementation;
  typedef Swapping_Vector<Row> Cont;
  typedef Indirect_Sort_Compare<Cont, Row_Less_Than> Sort_Compare;
  typedef Indirect_Swapper<Cont> Swapper;
  const dimension_type num_duplicates
    = indirect_sort_and_unique(num_elems,
                               Sort_Compare(rows, first_row),
                               Unique_Compare(rows, first_row),
                               Swapper(rows, first_row));

  if (num_duplicates > 0) {
    typedef typename Cont::iterator Iter;
    typedef typename std::iterator_traits<Iter>::difference_type diff_t;
    Iter last = rows.begin() + static_cast<diff_t>(last_row);
    Iter first = last - + static_cast<diff_t>(num_duplicates);
    rows.erase(first, last);
  }

  if (sorting_pending) {
    PPL_ASSERT(old_num_pending >= num_duplicates);
    index_first_pending = num_rows() - (old_num_pending - num_duplicates);
  }
  else {
    index_first_pending = num_rows() - old_num_pending;
  }

  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::strong_normalize() {
  const dimension_type nrows = rows.size();
  // We strongly normalize also the pending rows.
  for (dimension_type i = nrows; i-- > 0; )
    rows[i].strong_normalize();
  sorted = (nrows <= 1);
  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::sign_normalize() {
  const dimension_type nrows = rows.size();
  // We sign-normalize also the pending rows.
  for (dimension_type i = nrows; i-- > 0; )
    rows[i].sign_normalize();
  sorted = (nrows <= 1);
  PPL_ASSERT(OK());
}

/*! \relates Parma_Polyhedra_Library::Linear_System */
template <typename Row>
bool
operator==(const Linear_System<Row>& x, const Linear_System<Row>& y) {
  if (x.space_dimension() != y.space_dimension())
    return false;
  const dimension_type x_num_rows = x.num_rows();
  const dimension_type y_num_rows = y.num_rows();
  if (x_num_rows != y_num_rows)
    return false;
  if (x.first_pending_row() != y.first_pending_row())
    return false;
  // TODO: Check if the following comment is up to date.
  // Notice that calling operator==(const Swapping_Vector<Row>&,
  //                                const Swapping_Vector<Row>&)
  // would be wrong here, as equality of the type fields would
  // not be checked.
  for (dimension_type i = x_num_rows; i-- > 0; )
    if (x[i] != y[i])
      return false;
  return true;
}

template <typename Row>
void
Linear_System<Row>::sort_and_remove_with_sat(Bit_Matrix& sat) {
  // We can only sort the non-pending part of the system.
  PPL_ASSERT(first_pending_row() == sat.num_rows());
  if (first_pending_row() <= 1) {
    set_sorted(true);
    return;
  }

  const dimension_type num_elems = sat.num_rows();
  // Build the function objects implementing indirect sort comparison,
  // indirect unique comparison and indirect swap operation.
  typedef Swapping_Vector<Row> Cont;
  const Implementation::Indirect_Sort_Compare<Cont, Row_Less_Than>
    sort_cmp(rows);
  const Unique_Compare unique_cmp(rows);
  const Implementation::Indirect_Swapper2<Cont, Bit_Matrix> swapper(rows, sat);

  const dimension_type num_duplicates
    = Implementation::indirect_sort_and_unique(num_elems, sort_cmp,
                                               unique_cmp, swapper);

  const dimension_type new_first_pending_row
    = first_pending_row() - num_duplicates;

  if (num_pending_rows() > 0) {
    // In this case, we must put the duplicates after the pending rows.
    const dimension_type n_rows = num_rows() - 1;
    for (dimension_type i = 0; i < num_duplicates; ++i)
      swap(rows[new_first_pending_row + i], rows[n_rows - i]);
  }

  // Erasing the duplicated rows...
  rows.resize(rows.size() - num_duplicates);
  index_first_pending = new_first_pending_row;
  // ... and the corresponding rows of the saturation matrix.
  sat.remove_trailing_rows(num_duplicates);

  // Now the system is sorted.
  sorted = true;

  PPL_ASSERT(OK());
}

template <typename Row>
dimension_type
Linear_System<Row>::gauss(const dimension_type n_lines_or_equalities) {
  // This method is only applied to a linear system having no pending rows and
  // exactly `n_lines_or_equalities' lines or equalities, all of which occur
  // before the rays or points or inequalities.
  PPL_ASSERT(num_pending_rows() == 0);
  PPL_ASSERT(n_lines_or_equalities == num_lines_or_equalities());
#ifndef NDEBUG
  for (dimension_type i = n_lines_or_equalities; i-- > 0; )
    PPL_ASSERT((*this)[i].is_line_or_equality());
#endif

  dimension_type rank = 0;
  // Will keep track of the variations on the system of equalities.
  bool changed = false;
  // TODO: Don't use the number of columns.
  const dimension_type num_cols
    = is_necessarily_closed() ? space_dimension() + 1 : space_dimension() + 2;
  // TODO: Consider exploiting the row (possible) sparseness of rows in the
  // following loop, if needed. It would probably make it more cache-efficient
  // for dense rows, too.
  for (dimension_type j = num_cols; j-- > 0; )
    for (dimension_type i = rank; i < n_lines_or_equalities; ++i) {
      // Search for the first row having a non-zero coefficient
      // (the pivot) in the j-th column.
      if ((*this)[i].expr.get(j) == 0)
        continue;
      // Pivot found: if needed, swap rows so that this one becomes
      // the rank-th row in the linear system.
      if (i > rank) {
        swap(rows[i], rows[rank]);
        // After swapping the system is no longer sorted.
        changed = true;
      }
      // Combine the row containing the pivot with all the lines or
      // equalities following it, so that all the elements on the j-th
      // column in these rows become 0.
      for (dimension_type k = i + 1; k < n_lines_or_equalities; ++k) {
        if (rows[k].expr.get(Variable(j - 1)) != 0) {
          rows[k].linear_combine(rows[rank], j);
          changed = true;
        }
      }
      // Already dealt with the rank-th row.
      ++rank;
      // Consider another column index `j'.
      break;
    }
  if (changed)
    sorted = false;

  PPL_ASSERT(OK());
  return rank;
}

template <typename Row>
void
Linear_System<Row>
::back_substitute(const dimension_type n_lines_or_equalities) {
  // This method is only applied to a system having no pending rows and
  // exactly `n_lines_or_equalities' lines or equalities, all of which occur
  // before the first ray or point or inequality.
  PPL_ASSERT(num_pending_rows() == 0);
  PPL_ASSERT(n_lines_or_equalities <= num_lines_or_equalities());
#ifndef NDEBUG
  for (dimension_type i = n_lines_or_equalities; i-- > 0; )
    PPL_ASSERT((*this)[i].is_line_or_equality());
#endif

  const dimension_type nrows = num_rows();
  // Trying to keep sortedness.
  bool still_sorted = is_sorted();
  // This deque of Booleans will be used to flag those rows that,
  // before exiting, need to be re-checked for sortedness.
  std::deque<bool> check_for_sortedness;
  if (still_sorted)
    check_for_sortedness.insert(check_for_sortedness.end(), nrows, false);

  for (dimension_type k = n_lines_or_equalities; k-- > 0; ) {
    // For each line or equality, starting from the last one,
    // looks for the last non-zero element.
    // `j' will be the index of such a element.
    Row& row_k = rows[k];
    const dimension_type j = row_k.expr.last_nonzero();
    // TODO: Check this.
    PPL_ASSERT(j != 0);

    // Go through the equalities above `row_k'.
    for (dimension_type i = k; i-- > 0; ) {
      Row& row_i = rows[i];
      if (row_i.expr.get(Variable(j - 1)) != 0) {
        // Combine linearly `row_i' with `row_k'
        // so that `row_i[j]' becomes zero.
        row_i.linear_combine(row_k, j);
        if (still_sorted) {
          // Trying to keep sortedness: remember which rows
          // have to be re-checked for sortedness at the end.
          if (i > 0)
            check_for_sortedness[i-1] = true;
          check_for_sortedness[i] = true;
        }
      }
    }

    // Due to strong normalization during previous iterations,
    // the pivot coefficient `row_k[j]' may now be negative.
    // Since an inequality (or ray or point) cannot be multiplied
    // by a negative factor, the coefficient of the pivot must be
    // forced to be positive.
    const bool have_to_negate = (row_k.expr.get(Variable(j - 1)) < 0);
    if (have_to_negate)
      neg_assign(row_k.expr);

    // NOTE: Here row_k will *not* be ok if we have negated it.

    // Note: we do not mark index `k' in `check_for_sortedness',
    // because we will later negate back the row.

    // Go through all the other rows of the system.
    for (dimension_type i = n_lines_or_equalities; i < nrows; ++i) {
      Row& row_i = rows[i];
      if (row_i.expr.get(Variable(j - 1)) != 0) {
        // Combine linearly the `row_i' with `row_k'
        // so that `row_i[j]' becomes zero.
        row_i.linear_combine(row_k, j);
        if (still_sorted) {
          // Trying to keep sortedness: remember which rows
          // have to be re-checked for sortedness at the end.
          if (i > n_lines_or_equalities)
            check_for_sortedness[i-1] = true;
          check_for_sortedness[i] = true;
        }
      }
    }
    if (have_to_negate)
      // Negate `row_k' to restore strong-normalization.
      neg_assign(row_k.expr);

    PPL_ASSERT(row_k.OK());
  }

  // Trying to keep sortedness.
  for (dimension_type i = 0; still_sorted && i+1 < nrows; ++i)
    if (check_for_sortedness[i])
      // Have to check sortedness of `(*this)[i]' with respect to `(*this)[i+1]'.
      still_sorted = (compare((*this)[i], (*this)[i+1]) <= 0);

  // Set the sortedness flag.
  sorted = still_sorted;

  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>::simplify() {
  // This method is only applied to a system having no pending rows.
  PPL_ASSERT(num_pending_rows() == 0);

  // Partially sort the linear system so that all lines/equalities come first.
  const dimension_type old_nrows = num_rows();
  dimension_type nrows = old_nrows;
  dimension_type n_lines_or_equalities = 0;
  for (dimension_type i = 0; i < nrows; ++i)
    if ((*this)[i].is_line_or_equality()) {
      if (n_lines_or_equalities < i) {
        swap(rows[i], rows[n_lines_or_equalities]);
        // The system was not sorted.
        PPL_ASSERT(!sorted);
      }
      ++n_lines_or_equalities;
    }
  // Apply Gaussian elimination to the subsystem of lines/equalities.
  const dimension_type rank = gauss(n_lines_or_equalities);
  // Eliminate any redundant line/equality that has been detected.
  if (rank < n_lines_or_equalities) {
    const dimension_type
      n_rays_or_points_or_inequalities = nrows - n_lines_or_equalities;
    const dimension_type
      num_swaps = std::min(n_lines_or_equalities - rank,
                           n_rays_or_points_or_inequalities);
    for (dimension_type i = num_swaps; i-- > 0; )
      swap(rows[--nrows], rows[rank + i]);
    remove_trailing_rows(old_nrows - nrows);
    if (n_rays_or_points_or_inequalities > num_swaps)
      set_sorted(false);
    unset_pending_rows();
    n_lines_or_equalities = rank;
  }
  // Apply back-substitution to the system of rays/points/inequalities.
  back_substitute(n_lines_or_equalities);

  PPL_ASSERT(OK());
}

template <typename Row>
void
Linear_System<Row>
::add_universe_rows_and_space_dimensions(const dimension_type n) {
  PPL_ASSERT(n > 0);
  const bool was_sorted = is_sorted();
  const dimension_type old_n_rows = num_rows();
  const dimension_type old_space_dim
    = is_necessarily_closed() ? space_dimension() : space_dimension() + 1;
  set_space_dimension(space_dimension() + n);
  rows.resize(rows.size() + n);
  // The old system is moved to the bottom.
  for (dimension_type i = old_n_rows; i-- > 0; )
    swap(rows[i], rows[i + n]);
  for (dimension_type i = n, c = old_space_dim; i-- > 0; ) {
    // The top right-hand sub-system (i.e., the system made of new
    // rows and columns) is set to the specular image of the identity
    // matrix.
    if (Variable(c).space_dimension() <= space_dimension()) {
      // Variable(c) is a user variable.
      Linear_Expression le(representation());
      le.set_space_dimension(space_dimension());
      le += Variable(c);
      Row r(le, Row::LINE_OR_EQUALITY, row_topology);
      swap(r, rows[i]);
    }
    else {
      // Variable(c) is the epsilon dimension.
      PPL_ASSERT(row_topology == NOT_NECESSARILY_CLOSED);
      Linear_Expression le(Variable(c), representation());
      Row r(le, Row::LINE_OR_EQUALITY, NECESSARILY_CLOSED);
      r.mark_as_not_necessarily_closed();
      swap(r, rows[i]);
      // Note: `r' is strongly normalized.
    }
    ++c;
  }
  // If the old system was empty, the last row added is either
  // a positivity constraint or a point.
  if (was_sorted)
    sorted = (compare(rows[n-1], rows[n]) <= 0);

  // If the system is not necessarily closed, move the epsilon coefficients to
  // the last column.
  if (!is_necessarily_closed()) {
    // Try to preserve sortedness of `gen_sys'.
    PPL_ASSERT(old_space_dim != 0);
    if (!is_sorted()) {
      for (dimension_type i = n; i-- > 0; ) {
        rows[i].expr.swap_space_dimensions(Variable(old_space_dim - 1),
                                           Variable(old_space_dim - 1 + n));
        PPL_ASSERT(rows[i].OK());
      }
    }
    else {
      dimension_type old_eps_index = old_space_dim - 1;
      // The upper-right corner of `rows' contains the J matrix:
      // swap coefficients to preserve sortedness.
      for (dimension_type i = n; i-- > 0; ++old_eps_index) {
        rows[i].expr.swap_space_dimensions(Variable(old_eps_index),
                                           Variable(old_eps_index + 1));
        PPL_ASSERT(rows[i].OK());
      }

      sorted = true;
    }
  }
  // NOTE: this already checks for OK().
  set_index_first_pending_row(index_first_pending + n);
}

template <typename Row>
void
Linear_System<Row>::sort_pending_and_remove_duplicates() {
  PPL_ASSERT(num_pending_rows() > 0);
  PPL_ASSERT(is_sorted());

  // The non-pending part of the system is already sorted.
  // Now sorting the pending part..
  const dimension_type first_pending = first_pending_row();
  sort_rows(first_pending, num_rows());
  // Recompute the number of rows, because we may have removed
  // some rows occurring more than once in the pending part.
  const dimension_type old_num_rows = num_rows();
  dimension_type num_rows = old_num_rows;

  dimension_type k1 = 0;
  dimension_type k2 = first_pending;
  dimension_type num_duplicates = 0;
  // In order to erase them, put at the end of the system
  // those pending rows that also occur in the non-pending part.
  while (k1 < first_pending && k2 < num_rows) {
    const int cmp = compare(rows[k1], rows[k2]);
    if (cmp == 0) {
      // We found the same row.
      ++num_duplicates;
      --num_rows;
      // By initial sortedness, we can increment index `k1'.
      ++k1;
      // Do not increment `k2'; instead, swap there the next pending row.
      if (k2 < num_rows)
        swap(rows[k2], rows[k2 + num_duplicates]);
    }
    else if (cmp < 0)
      // By initial sortedness, we can increment `k1'.
      ++k1;
    else {
      // Here `cmp > 0'.
      // Increment `k2' and, if we already found any duplicate,
      // swap the next pending row in position `k2'.
      ++k2;
      if (num_duplicates > 0 && k2 < num_rows)
        swap(rows[k2], rows[k2 + num_duplicates]);
    }
  }
  // If needed, swap any duplicates found past the pending rows
  // that has not been considered yet; then erase the duplicates.
  if (num_duplicates > 0) {
    if (k2 < num_rows)
      for (++k2; k2 < num_rows; ++k2)
        swap(rows[k2], rows[k2 + num_duplicates]);
    rows.resize(num_rows);
  }
  sorted = true;
  PPL_ASSERT(OK());
}

template <typename Row>
bool
Linear_System<Row>::check_sorted() const {
  for (dimension_type i = first_pending_row(); i-- > 1; )
    if (compare(rows[i], rows[i-1]) < 0)
      return false;
  return true;
}

template <typename Row>
bool
Linear_System<Row>::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  for (dimension_type i = rows.size(); i-- > 0; ) {
    if (rows[i].representation() != representation()) {
#ifndef NDEBUG
      cerr << "Linear_System has a row with the wrong representation!"
           << endl;
#endif
      return false;
    }
    if (rows[i].space_dimension() != space_dimension()) {
#ifndef NDEBUG
      cerr << "Linear_System has a row with the wrong number of space dimensions!"
           << endl;
#endif
      return false;
    }
  }

  for (dimension_type i = rows.size(); i-- > 0; )
    if (rows[i].topology() != topology()) {
#ifndef NDEBUG
      cerr << "Linear_System has a row with the wrong topology!"
           << endl;
#endif
      return false;
    }

  // `index_first_pending' must be less than or equal to `num_rows()'.
  if (first_pending_row() > num_rows()) {
#ifndef NDEBUG
    cerr << "Linear_System has a negative number of pending rows!"
         << endl;
#endif
    return false;
  }

  // Check for topology mismatches.
  const dimension_type n_rows = num_rows();
  for (dimension_type i = 0; i < n_rows; ++i)
    if (topology() != rows[i].topology()) {
#ifndef NDEBUG
      cerr << "Topology mismatch between the system "
           << "and one of its rows!"
           << endl;
#endif
      return false;
    }

  if (sorted && !check_sorted()) {
#ifndef NDEBUG
    cerr << "The system declares itself to be sorted but it is not!"
         << endl;
#endif
    return false;
  }

  // All checks passed.
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Linear_System_templates_hh)
