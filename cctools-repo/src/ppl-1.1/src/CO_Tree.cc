/* CO_Tree class implementation.
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
#include "CO_Tree_defs.hh"

namespace PPL = Parma_Polyhedra_Library;

PPL::dimension_type
PPL::CO_Tree::external_memory_in_bytes() const {
  dimension_type memory_size = 0;
  if (reserved_size != 0) {
    // Add the size of data[]
    memory_size += (reserved_size + 1)*sizeof(data[0]);
    // Add the size of indexes[]
    memory_size += (reserved_size + 2)*sizeof(indexes[0]);
    for (const_iterator itr = begin(), itr_end = end(); itr != itr_end; ++itr)
      memory_size += PPL::external_memory_in_bytes(*itr);
  }
  return memory_size;
}

PPL::CO_Tree::iterator
PPL::CO_Tree::insert(iterator itr, dimension_type key1) {
  PPL_ASSERT(key1 != unused_index);

  if (empty()) {
    insert_in_empty_tree(key1, Coefficient_zero());
    return iterator(*this);
  }

  if (itr == end())
    return insert(key1);

  iterator candidate1 = bisect_near(itr, key1);

  if (key1 == candidate1.index())
    return candidate1;

  dimension_type candidate2_index = dfs_index(candidate1);

  if (key1 < candidate1.index()) {
    --candidate2_index;
    while (indexes[candidate2_index] == unused_index)
      --candidate2_index;
  }
  else {
    ++candidate2_index;
    while (indexes[candidate2_index] == unused_index)
      ++candidate2_index;
  }

  tree_iterator candidate1_node(candidate1, *this);

  PPL_ASSERT(candidate2_index <= reserved_size + 1);

  if (candidate2_index == 0 || candidate2_index > reserved_size)
    // Use candidate1
    return iterator(insert_precise(key1, Coefficient_zero(),
                                   candidate1_node));

  tree_iterator candidate2_node(*this, candidate2_index);

  // Adjacent nodes in an in-order visit of a tree always have different
  // heights. This fact can be easily proven by induction on the tree's
  // height, using the definition of the in-order layout.
  PPL_ASSERT(candidate1_node.get_offset() != candidate2_node.get_offset());

  if (candidate1_node.get_offset() < candidate2_node.get_offset()) {
    PPL_ASSERT(candidate1_node.depth() > candidate2_node.depth());
    // candidate1_node is deeper in the tree than candidate2_node, so use
    // candidate1_node.
    return iterator(insert_precise(key1, Coefficient_zero(),
                                   candidate1_node));
  }
  else {
    PPL_ASSERT(candidate1_node.depth() < candidate2_node.depth());
    // candidate2_node is deeper in the tree than candidate1_node, so use
    // candidate2_node.
    return iterator(insert_precise(key1, Coefficient_zero(),
                                    candidate2_node));
  }
}

PPL::CO_Tree::iterator
PPL::CO_Tree::insert(iterator itr, dimension_type key1,
                     data_type_const_reference data1) {
  PPL_ASSERT(key1 != unused_index);

  if (empty()) {
    insert_in_empty_tree(key1, data1);
    return iterator(*this);
  }

  if (itr == end())
    return insert(key1, data1);

  iterator candidate1 = bisect_near(itr, key1);

  if (key1 == candidate1.index()) {
    *candidate1 = data1;
    return candidate1;
  }

  dimension_type candidate2_index = dfs_index(candidate1);

  if (key1 < candidate1.index()) {
    --candidate2_index;
    while (indexes[candidate2_index] == unused_index)
      --candidate2_index;
  }
  else {
    ++candidate2_index;
    while (indexes[candidate2_index] == unused_index)
      ++candidate2_index;
  }

  tree_iterator candidate1_node(candidate1, *this);

  if (candidate2_index == 0 || candidate2_index > reserved_size)
    // Use candidate1
    return iterator(insert_precise(key1, data1, candidate1_node));

  tree_iterator candidate2_node(*this, candidate2_index);

  // Adjacent nodes in an in-order visit of a tree always have different
  // heights. This fact can be easily proven by induction on the tree's
  // height, using the definition of the in-order layout.
  PPL_ASSERT(candidate1_node.get_offset() != candidate2_node.get_offset());

  if (candidate1_node.get_offset() < candidate2_node.get_offset()) {
    PPL_ASSERT(candidate1_node.depth() > candidate2_node.depth());
    // candidate1_node is deeper in the tree than candidate2_node, so
    // use candidate1_node.
    return iterator(insert_precise(key1, data1, candidate1_node));
  }
  else {
    PPL_ASSERT(candidate1_node.depth() < candidate2_node.depth());
    // candidate2_node is deeper in the tree than candidate1_node, so
    // use candidate2_node.
    return iterator(insert_precise(key1, data1, candidate2_node));
  }
}

void
PPL::CO_Tree::erase_element_and_shift_left(dimension_type key) {
  iterator itr = erase(key);
  if (itr == end())
    return;
  const dimension_type i = dfs_index(itr);
  dimension_type* p = indexes + i;
  const dimension_type* const p_end = indexes + (reserved_size + 1);
  for ( ; p != p_end; ++p)
    if (*p != unused_index)
      --(*p);
  PPL_ASSERT(OK());
}

void
PPL::CO_Tree::increase_keys_from(dimension_type key, dimension_type n) {
  if (empty())
    return;
  dimension_type* p = indexes + reserved_size;
  while (*p == unused_index)
    --p;
  while (p != indexes && *p >= key) {
    *p += n;
    --p;
    while (*p == unused_index)
      --p;
  }
  PPL_ASSERT(OK());
}

PPL::dimension_type
PPL::CO_Tree::bisect_in(dimension_type first, dimension_type last,
                   dimension_type key) const {
  PPL_ASSERT(first != 0);
  PPL_ASSERT(last <= reserved_size);
  PPL_ASSERT(first <= last);
  PPL_ASSERT(indexes[first] != unused_index);
  PPL_ASSERT(indexes[last] != unused_index);

  while (first < last) {
    dimension_type half = (first + last) / 2;
    dimension_type new_half = half;

    while (indexes[new_half] == unused_index)
      ++new_half;

    if (indexes[new_half] == key)
      return new_half;

    if (indexes[new_half] > key) {

      while (indexes[half] == unused_index)
        --half;

      last = half;

    }
    else {

      ++new_half;
      while (indexes[new_half] == unused_index)
        ++new_half;

      first = new_half;
    }
  }

  // It is important that last is returned instead of first, because first
  // may have increased beyond last, even beyond the original value of last
  // at the beginning of this method.
  return last;
}

PPL::dimension_type
PPL::CO_Tree::bisect_near(dimension_type hint, dimension_type key) const {
  PPL_ASSERT(hint != 0);
  PPL_ASSERT(hint <= reserved_size);
  PPL_ASSERT(indexes[hint] != unused_index);

  if (indexes[hint] == key)
    return hint;

  dimension_type new_hint;
  dimension_type offset = 1;

  if (indexes[hint] > key) {
    // The searched element is before `hint'.

    while (true) {

      if (hint <= offset) {
        // The searched element is in (0,hint).
        new_hint = hint;
        hint = 1;
        // The searched element is in [hint,new_hint).
        while (indexes[hint] == unused_index)
          ++hint;
        if (indexes[hint] >= key)
          return hint;
        // The searched element is in (hint,new_hint) and both indexes point
        // to valid elements.
        break;
      }
      else
        new_hint = hint - offset;

      PPL_ASSERT(new_hint > 0);
      PPL_ASSERT(new_hint <= reserved_size);

      // Find the element at `new_hint' (or the first after it).
      while (indexes[new_hint] == unused_index)
        ++new_hint;

      PPL_ASSERT(new_hint <= hint);

      if (indexes[new_hint] == key)
        return new_hint;
      else
        if (indexes[new_hint] < key) {
          // The searched element is in (new_hint,hint)
          using std::swap;
          swap(hint, new_hint);
          // The searched element is now in (hint,new_hint).
          break;
        }

      hint = new_hint;
      offset *= 2;
    }

  }
  else {
    // The searched element is after `hint'.
    while (true) {

      if (hint + offset > reserved_size) {
        // The searched element is in (hint,reserved_size+1).
        new_hint = reserved_size;
        // The searched element is in (hint,new_hint].
        while (indexes[new_hint] == unused_index)
          --new_hint;
        if (indexes[new_hint] <= key)
          return new_hint;
        // The searched element is in (hint,new_hint) and both indexes point
        // to valid elements.
        break;
      }
      else
        new_hint = hint + offset;

      PPL_ASSERT(new_hint > 0);
      PPL_ASSERT(new_hint <= reserved_size);

      // Find the element at `new_hint' (or the first after it).
      while (indexes[new_hint] == unused_index)
        --new_hint;

      PPL_ASSERT(hint <= new_hint);

      if (indexes[new_hint] == key)
        return new_hint;
      else
        if (indexes[new_hint] > key)
          // The searched element is in (hint,new_hint).
          break;

      hint = new_hint;
      offset *= 2;
    }
  }
  // The searched element is in (hint,new_hint).
  PPL_ASSERT(hint > 0);
  PPL_ASSERT(hint < new_hint);
  PPL_ASSERT(new_hint <= reserved_size);
  PPL_ASSERT(indexes[hint] != unused_index);
  PPL_ASSERT(indexes[new_hint] != unused_index);

  ++hint;
  while (indexes[hint] == unused_index)
    ++hint;

  if (hint == new_hint)
    return hint;

  --new_hint;
  while (indexes[new_hint] == unused_index)
    --new_hint;

  PPL_ASSERT(hint <= new_hint);
  PPL_ASSERT(indexes[hint] != unused_index);
  PPL_ASSERT(indexes[new_hint] != unused_index);

  return bisect_in(hint, new_hint, key);
}

PPL::CO_Tree::tree_iterator
PPL::CO_Tree::insert_precise(dimension_type key1,
                             data_type_const_reference data1,
                             tree_iterator itr) {
  PPL_ASSERT(key1 != unused_index);
  PPL_ASSERT(!empty());

#ifndef NDEBUG
  // Check that `itr' is a correct hint.
  tree_iterator itr2(*this);
  itr2.go_down_searching_key(key1);
  PPL_ASSERT(itr == itr2);
#endif

  if (itr.index() == key1) {
    // Replacement, rather than insertion.
    *itr = data1;
    PPL_ASSERT(OK());
    return itr;
  }

  // Proper insertion: check if it would invalidate `data1'.
  const bool invalidating
    = (data <= &data1) && (&data1 < data + (reserved_size + 1));

  if (!invalidating)
    return insert_precise_aux(key1, data1, itr);

  // `data1' could be invalidated by the insert, because it is
  // a coefficient of this row. Avoid the issue by copying it.
  data_type data1_copy = data1;

#ifndef NDEBUG
  dimension_type i = &data1 - data;
  dimension_type key2 = indexes[i];
  PPL_ASSERT(key2 != unused_index);
  // This is true since `key1' is not in the tree.
  PPL_ASSERT(key2 != key1);
#endif

  // Insert a dummy coefficient.
  // NOTE: this may invalidate `data1', because it may reallocate the tree
  // and/or move coefficients during rebalancing.
  itr = insert_precise_aux(key1, Coefficient_zero(), itr);

  PPL_ASSERT(itr.index() == key1);

  // Swap the correct coefficient in place.
  using std::swap;
  swap(*itr, data1_copy);

  PPL_ASSERT(OK());
  return itr;
}

PPL::CO_Tree::tree_iterator
PPL::CO_Tree::insert_precise_aux(dimension_type key1,
                                 data_type_const_reference data1,
                                 tree_iterator itr) {
  PPL_ASSERT(key1 != unused_index);
  PPL_ASSERT(!empty());
  // This is a proper insert.
  PPL_ASSERT(itr.index() != key1);
  // `data1' is not going to be invalidated.
  PPL_ASSERT(!(data <= &data1 && &data1 < data + (reserved_size + 1)));

  if (is_greater_than_ratio(size_ + 1, reserved_size, max_density_percent)) {
    rebuild_bigger_tree();
    // `itr' was invalidated by the rebuild operation
    itr.get_root();
    itr.go_down_searching_key(key1);
    PPL_ASSERT(itr.index() != key1);
  }

  PPL_ASSERT(!is_greater_than_ratio(size_ + 1, reserved_size,
                                    max_density_percent));

  ++size_;

  if (!itr.is_leaf()) {
    if (key1 < itr.index())
      itr.get_left_child();
    else
      itr.get_right_child();
    PPL_ASSERT(itr.index() == unused_index);

    new (&(*itr)) data_type(data1);
    // Set the index only if the construction was successful.
    itr.index() = key1;
  }
  else {
    itr = rebalance(itr, key1, data1);
    itr.go_down_searching_key(key1);
    PPL_ASSERT(itr.index() == key1);
  }
  PPL_ASSERT(OK());

  return itr;
}

PPL::CO_Tree::iterator
PPL::CO_Tree::erase(tree_iterator itr) {
  PPL_ASSERT(itr.index() != unused_index);

  PPL_ASSERT(size_ != 0);

  if (size_ == 1) {
    // Deleting the only element of this tree, now it is empty.
    clear();
    return end();
  }

  if (is_less_than_ratio(size_ - 1, reserved_size, min_density_percent)
      && !is_greater_than_ratio(size_ - 1, reserved_size/2,
                                max_density_percent)) {

    const dimension_type key = itr.index();

    PPL_ASSERT(!is_greater_than_ratio(size_, reserved_size,
                                      max_density_percent));

    rebuild_smaller_tree();
    itr.get_root();
    itr.go_down_searching_key(key);

    PPL_ASSERT(itr.index() == key);
  }

#ifndef NDEBUG
  if (size_ > 1)
    PPL_ASSERT(!is_less_than_ratio(size_ - 1, reserved_size,
                                   min_density_percent)
               || is_greater_than_ratio(size_ - 1, reserved_size/2,
                                        max_density_percent));
#endif

  const dimension_type deleted_key = itr.index();
  tree_iterator deleted_node = itr;
  (*itr).~data_type();
  while (true) {
    dimension_type& current_key  = itr.index();
    data_type&      current_data = *itr;
    if (itr.is_leaf())
      break;
    itr.get_left_child();
    if (itr.index() != unused_index)
      // The left child has a value.
      itr.follow_right_children_with_value();
    else {
      // The left child has not a value, try the right child.
      itr.get_parent();
      itr.get_right_child();
      if (itr.index() != unused_index)
        // The right child has a value.
        itr.follow_left_children_with_value();
      else {
        // The right child has not a value, too.
        itr.get_parent();
        break;
      }
    }
    using std::swap;
    swap(current_key, itr.index());
    move_data_element(current_data, *itr);
  }

  PPL_ASSERT(itr.index() != unused_index);
  itr.index() = unused_index;
  --size_;

  PPL_ASSERT(OK());

  itr = rebalance(itr, 0, Coefficient_zero());

  if (itr.get_offset() < deleted_node.get_offset())
    // deleted_node is an ancestor of itr
    itr = deleted_node;

  itr.go_down_searching_key(deleted_key);

  iterator result(itr);

  if (result.index() < deleted_key)
    ++result;

  PPL_ASSERT(OK());
  PPL_ASSERT(result == end() || result.index() > deleted_key);
#ifndef NDEBUG
  if (!empty()) {
    iterator last = end();
    --last;
    PPL_ASSERT((result == end()) == (last.index() < deleted_key));
  }
#endif

  return result;
}

void
PPL::CO_Tree::init(dimension_type n) {
  indexes = NULL;
  data = NULL;
  size_ = 0;
  reserved_size = 0;
  max_depth = 0;

  if (n > 0) {
    const dimension_type max_d = integer_log2(n) + 1;
    const height_t new_max_depth = static_cast<height_t>(max_d);
    const dimension_type new_reserved_size
      = (static_cast<dimension_type>(1) << new_max_depth) - 1;
    // If this throws, *this will be the empty tree.
    indexes = new dimension_type[new_reserved_size + 2];
    try {
      data = data_allocator.allocate(new_reserved_size + 1);
    }
    catch (...) {
      delete[] indexes;
      indexes = 0;
      PPL_ASSERT(OK());
      throw;
    }
    max_depth = new_max_depth;
    reserved_size = new_reserved_size;

    // Mark all pairs as unused.
    for (dimension_type i = 1; i <= reserved_size; ++i)
      indexes[i] = unused_index;

    // These are used as markers by iterators.
    indexes[0] = 0;
    indexes[reserved_size + 1] = 0;
  }

  refresh_cached_iterators();

  PPL_ASSERT(structure_OK());
}

void
PPL::CO_Tree::destroy() {

  if (reserved_size != 0) {
    for (dimension_type i = 1; i <= reserved_size; ++i) {
      if (indexes[i] != unused_index)
        data[i].~data_type();
    }

    delete[] indexes;
    data_allocator.deallocate(data, reserved_size + 1);
  }
}

bool
PPL::CO_Tree::structure_OK() const {

  if (size_ > reserved_size)
    return false;

  if (reserved_size == 0) {
    if (indexes != NULL)
      return false;
    if (data != NULL)
      return false;
    if (max_depth != 0)
      return false;

    return true;
  }

  if (reserved_size < 3)
    return false;

  if (reserved_size != (static_cast<dimension_type>(1) << max_depth) - 1)
    return false;

  if (data == NULL)
    return false;

  if (indexes == NULL)
    return false;

  if (max_depth == 0)
    return false;

  if (size_ == 0) {

    // This const_cast could be removed by adding a const_tree_iterator,
    // but it would add much code duplication without a real need.
    tree_iterator itr(*const_cast<CO_Tree*>(this));
    if (itr.index() != unused_index)
      return false;

  }
  else {
    // This const_cast could be removed by adding a const_tree_iterator,
    // but it would add much code duplication without a real need.
    tree_iterator itr(*const_cast<CO_Tree*>(this));
    const dimension_type real_size = count_used_in_subtree(itr);
    if (real_size != size_)
      // There are \p real_size elements in the tree that are reachable by the
      // root, but size is \p size.
      return false;
  }

  if (size_ != 0) {
    const_iterator itr = begin();
    const_iterator itr_end = end();

    if (itr != itr_end) {
      dimension_type last_index = itr.index();
      for (++itr; itr != itr_end; ++itr) {
        if (last_index >= itr.index())
          // Found index \p itr->first after index \p last_index.
          return false;
        last_index = itr.index();
      }
    }
  }

  if (const_iterator(cached_end) != const_iterator(*this, reserved_size + 1))
    return false;
  if (cached_const_end != const_iterator(*this, reserved_size + 1))
    return false;

  return true;
}

bool
PPL::CO_Tree::OK() const {

  if (!structure_OK())
    return false;

  {
    dimension_type real_size = 0;

    for (const_iterator itr = begin(), itr_end = end(); itr != itr_end; ++itr)
      ++real_size;

    if (real_size != size_)
      // There are \p real_size elements in the tree, but size is \p size.
      return false;
  }

  if (reserved_size > 0) {
    if (is_greater_than_ratio(size_, reserved_size, max_density_percent)
        && reserved_size != 3)
      // Found too high density.
      return false;
    if (is_less_than_ratio(size_, reserved_size, min_density_percent)
        && !is_greater_than_ratio(size_, reserved_size/2, max_density_percent))
      // Found too low density
      return false;
  }

  return true;
}

unsigned
PPL::CO_Tree::integer_log2(dimension_type n) {
  PPL_ASSERT(n != 0);
  unsigned result = 0;
  while (n != 1) {
    n /= 2;
    ++result;
  }
  return result;
}

void
PPL::CO_Tree::dump_subtree(tree_iterator itr) {
  if (!itr.is_leaf()) {
    itr.get_left_child();
    dump_subtree(itr);
    itr.get_parent();
  }
  std::cout << "At depth: " << itr.depth();
  if (itr.index() == unused_index)
    std::cout << " (no data)" << std::endl;
  else
    std::cout << " pair (" << itr.index() << "," << *itr << ")" << std::endl;
  if (!itr.is_leaf()) {
    itr.get_right_child();
    dump_subtree(itr);
    itr.get_parent();
  }
}

void
PPL::CO_Tree::rebuild_bigger_tree() {
  if (reserved_size == 0) {
    init(3);
    PPL_ASSERT(structure_OK());
    return;
  }

  const dimension_type new_reserved_size = reserved_size*2 + 1;

  dimension_type* const new_indexes = new dimension_type[new_reserved_size + 2];

  data_type* new_data;

  try {
    new_data = data_allocator.allocate(new_reserved_size + 1);
  } catch (...) {
    delete[] new_indexes;
    throw;
  }

  new_indexes[1] = unused_index;

  for (dimension_type i = 1, j = 2; i <= reserved_size; ++i, ++j) {
    new_indexes[j] = indexes[i];
    if (indexes[i] != unused_index)
      move_data_element(new_data[j], data[i]);
    ++j;
    new_indexes[j] = unused_index;
  }

  // These are used as markers by iterators.
  new_indexes[0] = 0;
  new_indexes[new_reserved_size + 1] = 0;

  delete[] indexes;
  data_allocator.deallocate(data, reserved_size + 1);

  indexes = new_indexes;
  data = new_data;
  reserved_size = new_reserved_size;
  ++max_depth;

  refresh_cached_iterators();

  PPL_ASSERT(structure_OK());
}

PPL::CO_Tree::tree_iterator
PPL::CO_Tree::rebalance(tree_iterator itr, dimension_type key,
                        data_type_const_reference value) {
  // Trees with reserved size 3 need not to be rebalanced.
  // This check is needed because they can't be shrunk, so they may violate
  // the density thresholds, and this would prevent the following while from
  // working correctly.
  if (reserved_size == 3) {
    PPL_ASSERT(OK());
    return tree_iterator(*this);
  }
  PPL_ASSERT(itr.index() == unused_index || itr.is_leaf());
  height_t itr_depth_minus_1 = itr.depth() - 1;
  const height_t height = max_depth - itr_depth_minus_1;
  dimension_type subtree_size;
  dimension_type subtree_reserved_size = (static_cast<dimension_type>(1)
                                          << height) - 1;
  const bool deleting = itr.index() == unused_index;
  PPL_ASSERT(deleting || key != unused_index);
  if (deleting)
    subtree_size = 0;
  else
    // The existing element and the element with index key we want to add.
    subtree_size = 2;

  while (is_greater_than_ratio(subtree_size, subtree_reserved_size,
                               max_density_percent
                               + ((itr_depth_minus_1
                                   * (100 - max_density_percent))
                                  / (max_depth - 1)))
         || is_less_than_ratio(subtree_size, subtree_reserved_size,
                               min_density_percent
                               - ((itr_depth_minus_1
                                   * (min_density_percent
                                      - min_leaf_density_percent))
                                  / (max_depth - 1)))) {
    // The density in the tree is correct, so the while condition is always
    // false for the root.
    PPL_ASSERT(itr_depth_minus_1 != 0);
    const bool is_right_brother = itr.is_right_child();
    itr.get_parent();
    if (is_right_brother)
      itr.get_left_child();
    else
      itr.get_right_child();
    subtree_size += count_used_in_subtree(itr);
    itr.get_parent();
    PPL_ASSERT(itr.index() != unused_index);
    ++subtree_size;
    subtree_reserved_size = 2*subtree_reserved_size + 1;
    --itr_depth_minus_1;
    PPL_ASSERT(itr.depth() - 1 == itr_depth_minus_1);
  }

  // Now the subtree rooted at itr has been chosen as the subtree to be
  // rebalanced.

  // Step 1: compact elements of this subtree in the rightmost end, from right
  //         to left.
  const dimension_type last_index_in_subtree
    = itr.dfs_index() + itr.get_offset() - 1;

  const dimension_type first_unused
    = compact_elements_in_the_rightmost_end(last_index_in_subtree,
                                            subtree_size, key, value,
                                            !deleting);

  // Step 2: redistribute the elements, from left to right.
  redistribute_elements_in_subtree(itr.dfs_index(), subtree_size,
                                   first_unused + 1, key, value,
                                   first_unused != last_index_in_subtree
                                                   - subtree_size);

  PPL_ASSERT(OK());

  return itr;
}

PPL::dimension_type
PPL::CO_Tree
::compact_elements_in_the_rightmost_end(dimension_type last_in_subtree,
                                        dimension_type subtree_size,
                                        dimension_type key,
                                        data_type_const_reference value,
                                        bool add_element) {

  PPL_ASSERT(subtree_size != 0);

  PPL_ASSERT(subtree_size != 1 || !add_element);

  dimension_type* last_index_in_subtree = &(indexes[last_in_subtree]);
  data_type* last_data_in_subtree = &(data[last_in_subtree]);

  dimension_type* first_unused_index = last_index_in_subtree;
  data_type* first_unused_data = last_data_in_subtree;

  while (*last_index_in_subtree == unused_index) {
    --last_index_in_subtree;
    --last_data_in_subtree;
  }

  // From now on, last_index_in_subtree and last_data_in_subtree point to the
  // rightmost node with a value in the subtree. first_unused_index and
  // first_unused_data point to the rightmost unused node in the subtree.

  if (add_element)
    while (subtree_size != 0) {
      --subtree_size;
      if (last_index_in_subtree == indexes || key > *last_index_in_subtree) {
        if (last_index_in_subtree == indexes
            || last_index_in_subtree != first_unused_index) {
          PPL_ASSERT(first_unused_index != indexes);
          PPL_ASSERT(*first_unused_index == unused_index);
          new (first_unused_data) data_type(value);
          // Set the index only if the construction was successful.
          *first_unused_index = key;
          --first_unused_index;
          --first_unused_data;
        }
        break;
      }
      else {
        if (last_index_in_subtree != first_unused_index) {
          PPL_ASSERT(first_unused_index != indexes);
          PPL_ASSERT(last_index_in_subtree != indexes);
          PPL_ASSERT(*first_unused_index == unused_index);
          *first_unused_index = *last_index_in_subtree;
          *last_index_in_subtree = unused_index;
          move_data_element(*first_unused_data, *last_data_in_subtree);
        }
        --last_index_in_subtree;
        --last_data_in_subtree;
        while (*last_index_in_subtree == unused_index) {
          --last_index_in_subtree;
          --last_data_in_subtree;
        }
        --first_unused_index;
        --first_unused_data;
      }
    }
  while (subtree_size != 0) {
    if (last_index_in_subtree != first_unused_index) {
      PPL_ASSERT(first_unused_index != indexes);
      PPL_ASSERT(last_index_in_subtree != indexes);
      PPL_ASSERT(*first_unused_index == unused_index);
      *first_unused_index = *last_index_in_subtree;
      *last_index_in_subtree = unused_index;
      move_data_element(*first_unused_data, *last_data_in_subtree);
    }
    --last_index_in_subtree;
    --last_data_in_subtree;
    while (*last_index_in_subtree == unused_index) {
      --last_index_in_subtree;
      --last_data_in_subtree;
    }
    --first_unused_index;
    --first_unused_data;
    --subtree_size;
  }

  const ptrdiff_t distance = first_unused_index - indexes;
  PPL_ASSERT(distance >= 0);
  return static_cast<dimension_type>(distance);
}

void
PPL::CO_Tree::redistribute_elements_in_subtree(
    dimension_type root_index,
    dimension_type subtree_size,
    dimension_type last_used,
    dimension_type key,
    data_type_const_reference value,
    bool add_element) {

  // This is static and with static allocation, to improve performance.
  // sizeof_to_bits(sizeof(dimension_type)) is the maximum k such that
  // 2^k-1 is a dimension_type, so it is the maximum tree height.
  // For each node level, the stack may contain up to two element (one for the
  // subtree rooted at the right son of a node of that level, and one for the
  // node itself). An additional element can be at the top of the tree.
  static std::pair<dimension_type,dimension_type>
    stack[2U * sizeof_to_bits(sizeof(dimension_type)) + 1U];

  std::pair<dimension_type,dimension_type>* stack_first_empty = stack;

  // A pair (n, i) in the stack means to visit the subtree with root index i
  // and size n.

  PPL_ASSERT(subtree_size != 0);

  stack_first_empty->first  = subtree_size;
  stack_first_empty->second = root_index;
  ++stack_first_empty;

  while (stack_first_empty != stack) {

    --stack_first_empty;

    // Implement
    //
    // <CODE>
    //   top_n = stack.top().first;
    //   top_i = stack.top().second;
    // </CODE>
    const dimension_type top_n = stack_first_empty->first;
    const dimension_type top_i = stack_first_empty->second;

    PPL_ASSERT(top_n != 0);
    if (top_n == 1) {
      if (add_element
          && (last_used > reserved_size || indexes[last_used] > key)) {
        PPL_ASSERT(last_used != top_i);
        PPL_ASSERT(indexes[top_i] == unused_index);
        add_element = false;
        new (&(data[top_i])) data_type(value);
        // Set the index only if the construction was successful.
        indexes[top_i] = key;
      }
      else {
        if (last_used != top_i) {
          PPL_ASSERT(indexes[top_i] == unused_index);
          indexes[top_i] = indexes[last_used];
          indexes[last_used] = unused_index;
          move_data_element(data[top_i], data[last_used]);
        }
        ++last_used;
      }
    }
    else {
      PPL_ASSERT(stack_first_empty + 2
                 < stack + sizeof(stack)/sizeof(stack[0]));

      const dimension_type offset = (top_i & -top_i) / 2;
      const dimension_type half = (top_n + 1) / 2;

      PPL_ASSERT(half > 0);

      // Right subtree
      PPL_ASSERT(top_n - half > 0);
      stack_first_empty->first  = top_n - half;
      stack_first_empty->second = top_i + offset;
      ++stack_first_empty;

      // Root of the current subtree
      stack_first_empty->first   = 1;
      stack_first_empty->second  = top_i;
      ++stack_first_empty;

      // Left subtree
      if (half - 1 != 0) {
        stack_first_empty->first   = half - 1;
        stack_first_empty->second  = top_i - offset;
        ++stack_first_empty;
      }
    }
  }

  PPL_ASSERT(!add_element);
}

void
PPL::CO_Tree::move_data_from(CO_Tree& tree) {
  PPL_ASSERT(size_ == 0);
  if (tree.size_ == 0)
    return;

  tree_iterator root(*this);

  dimension_type source_index = 1;
  while (tree.indexes[source_index] == unused_index)
    ++source_index;

  // This is static and with static allocation, to improve performance.
  // sizeof_to_bits(sizeof(dimension_type)) is the maximum k such that 2^k-1 is a
  // dimension_type, so it is the maximum tree height.
  // For each node level, the stack may contain up to 4 elements: two elements
  // with operation 0, one element with operation 2 and one element
  // with operation 3. An additional element with operation 1 can be at the
  // top of the tree.
  static std::pair<dimension_type, signed char>
    stack[5U * sizeof_to_bits(sizeof(dimension_type))];

  dimension_type stack_first_empty = 0;

  // A pair (n, operation) in the stack means:
  //
  // * Go to the parent, if operation is 0.
  // * Go to the left child, then visit the current tree (with size n), if
  //   operation is 1.
  // * Go to the right child, then visit the current tree (with size n), if
  //   operation is 2.
  // * Visit the current tree (with size n), if operation is 3.

  stack[0].first = tree.size_;
  stack[0].second = 3;
  ++stack_first_empty;

  while (stack_first_empty != 0) {

    // Implement
    //
    // <CODE>
    //   top_n         = stack.top().first;
    //   top_operation = stack.top().second;
    // </CODE>
    const dimension_type top_n = stack[stack_first_empty - 1].first;
    const signed char top_operation = stack[stack_first_empty - 1].second;

    switch (top_operation) {

    case 0:
      root.get_parent();
      --stack_first_empty;
      continue;

    case 1:
      root.get_left_child();
      break;

    case 2:
      root.get_right_child();
      break;

#ifndef NDEBUG
    case 3:
      break;

    default:
      PPL_UNREACHABLE;
      break;
#endif
    }

    // We now visit the current tree

    if (top_n == 0) {
      --stack_first_empty;
    }
    else {
      if (top_n == 1) {
        PPL_ASSERT(root.index() == unused_index);
        PPL_ASSERT(tree.indexes[source_index] != unused_index);
        root.index() = tree.indexes[source_index];
        tree.indexes[source_index] = unused_index;
        move_data_element(*root, tree.data[source_index]);
        PPL_ASSERT(source_index <= tree.reserved_size);
        ++source_index;
        while (tree.indexes[source_index] == unused_index)
          ++source_index;
        --stack_first_empty;
      }
      else {
        PPL_ASSERT(stack_first_empty + 3 < sizeof(stack)/sizeof(stack[0]));

        const dimension_type half = (top_n + 1) / 2;
        stack[stack_first_empty - 1].second = 0;
        stack[stack_first_empty    ] = std::make_pair(top_n - half, 2);
        stack[stack_first_empty + 1] = std::make_pair(1, 3);
        stack[stack_first_empty + 2].second = 0;
        stack[stack_first_empty + 3] = std::make_pair(half - 1, 1);
        stack_first_empty += 4;
      }
    }
  }
  size_ = tree.size_;
  tree.size_ = 0;
  PPL_ASSERT(tree.structure_OK());
  PPL_ASSERT(structure_OK());
}

void
PPL::CO_Tree::copy_data_from(const CO_Tree& x) {
  PPL_ASSERT(size_ == 0);
  PPL_ASSERT(reserved_size == x.reserved_size);
  PPL_ASSERT(structure_OK());

  if (x.size_ == 0) {
    PPL_ASSERT(OK());
    return;
  }

  dimension_type i;
  try {
    for (i = x.reserved_size; i > 0; --i)
      if (x.indexes[i] != unused_index) {
        indexes[i] = x.indexes[i];
        new (&(data[i])) data_type(x.data[i]);
      }
      else {
        PPL_ASSERT(indexes[i] == unused_index);
      }
  } catch (...) {
    // The (used) data elements in (i,x.reserved_size] have been constructed
    // successfully.
    // The constructor of data[i] has thrown an exception, so data[i] has not
    // been constructed.

    // 1. Destroy the data elements that have been constructed successfully.
    for (dimension_type j = x.reserved_size; j > i; --j)
      if (indexes[j] != unused_index)
        data[j].~data_type();

    // 2. Deallocate index[] and data[]
    delete[] indexes;
    data_allocator.deallocate(data, reserved_size + 1);

    // 3. Set the tree to an empty tree and rethrow exception.
    init(0);
    throw;
  }

  size_ = x.size_;
  PPL_ASSERT(OK());
}

PPL::dimension_type
PPL::CO_Tree::count_used_in_subtree(tree_iterator itr) {
  dimension_type n = 0;

  const dimension_type k = itr.get_offset();
  const dimension_type root_index = itr.dfs_index();

  // The complete subtree rooted at itr has 2*k - 1 nodes.

  PPL_ASSERT(root_index > (k - 1));

  const dimension_type* current_index
    = &(itr.tree.indexes[root_index - (k - 1)]);

  for (dimension_type j = 2*k - 1; j > 0; --j, ++current_index)
    if (*current_index != unused_index)
      ++n;

  return n;
}

bool
PPL::CO_Tree::const_iterator::OK() const {
#if PPL_CO_TREE_EXTRA_DEBUG
  if (tree == 0) {
    if (current_index != 0)
      return false;
    if (current_data != 0)
      return false;
  }
  else
    if (tree->reserved_size == 0) {
      if (current_index != 1 + static_cast<dimension_type*>(0)
          || current_data != 1 + static_cast<data_type*>(0))
        return false;
    }
    else {
      if (current_index <= &(tree->indexes[0]))
        return false;
      if (current_index > &(tree->indexes[tree->reserved_size + 1]))
        return false;
      if (current_data <= &(tree->data[0]))
        return false;
      if (current_data > &(tree->data[tree->reserved_size + 1]))
        return false;
      if (*current_index == unused_index)
        return false;
      if (current_index - tree->indexes != current_data - tree->data)
        return false;
    }
#endif
  return true;
}

bool
PPL::CO_Tree::iterator::OK() const {
#if PPL_CO_TREE_EXTRA_DEBUG
  if (tree == 0) {
    if (current_index != 0)
      return false;
    if (current_data != 0)
      return false;
  }
  else
    if (tree->reserved_size == 0) {
      if (current_index != 1 + static_cast<dimension_type*>(0)
          || current_data != 1 + static_cast<data_type*>(0))
        return false;
    }
    else {
      if (current_index <= &(tree->indexes[0]))
        return false;
      if (current_index > &(tree->indexes[tree->reserved_size + 1]))
        return false;
      if (current_data <= &(tree->data[0]))
        return false;
      if (current_data > &(tree->data[tree->reserved_size + 1]))
        return false;
      if (*current_index == unused_index)
        return false;
      if (current_index - tree->indexes != current_data - tree->data)
        return false;
    }
#endif
  return true;
}

bool
PPL::CO_Tree::tree_iterator::OK() const {
  if (i == 0 || i > tree.reserved_size)
    return false;

  // This assumes two's complement encoding.
  const dimension_type correct_offset = i & -i;

  if (offset != correct_offset)
    return false;

  return true;
}

void
PPL::CO_Tree::tree_iterator::go_down_searching_key(dimension_type key) {
  // *this points to a node, so the tree is not empty.
  PPL_ASSERT(!tree.empty());
  PPL_ASSERT(key != unused_index);
  PPL_ASSERT(index() != unused_index);
  while (!is_leaf()) {
    if (key == index())
      break;
    if (key < index()) {
      get_left_child();
      if (index() == unused_index) {
        get_parent();
        break;
      }
    }
    else {
      get_right_child();
      if (index() == unused_index) {
        get_parent();
        break;
      }
    }
  }
}
