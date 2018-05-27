/* CO_Tree class declaration.
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

#ifndef PPL_CO_Tree_defs_hh
#define PPL_CO_Tree_defs_hh 1

#include "CO_Tree_types.hh"

#include "Coefficient_defs.hh"
#include <memory>

#ifndef PPL_CO_TREE_EXTRA_DEBUG
#ifdef PPL_ABI_BREAKING_EXTRA_DEBUG
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*!
  \brief
  Enables extra debugging information for class CO_Tree.

  \ingroup PPL_CXX_interface
  When <CODE>PPL_CO_TREE_EXTRA_DEBUG</CODE> evaluates to <CODE>true</CODE>,
  each CO_Tree iterator and const_iterator carries a pointer to the associated
  tree; this enables extra consistency checks to be performed.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define PPL_CO_TREE_EXTRA_DEBUG 1
#else // !defined(PPL_ABI_BREAKING_EXTRA_DEBUG)
#define PPL_CO_TREE_EXTRA_DEBUG 0
#endif // !defined(PPL_ABI_BREAKING_EXTRA_DEBUG)
#endif // !defined(PPL_CO_TREE_EXTRA_DEBUG)


namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A cache-oblivious binary search tree of pairs.
/*! \ingroup PPL_CXX_interface
  This class implements a binary search tree with keys of dimension_type type
  and data of Coefficient type, laid out in a dynamically-sized array.

  The array-based layout saves calls to new/delete (to insert \f$n\f$ elements
  only \f$O(\log n)\f$ allocations are performed) and, more importantly, is
  much more cache-friendly than a standard (pointer-based) tree, because the
  elements are stored sequentially in memory (leaving some holes to allow
  fast insertion of new elements).
  The downside of this representation is that all iterators are invalidated
  when an element is added or removed, because the array could have been
  enlarged or shrunk. This is partially addressed by providing references to
  internal end iterators that are updated when needed.

  B-trees are cache-friendly too, but the cache size is fixed (usually at
  compile-time). This raises two problems: firstly the cache size must be
  known in advance and those data structures do not perform well with other
  cache sizes and, secondly, even if the cache size is known, the
  optimizations target only one level of cache. This kind of data structures
  are called cache aware. This implementation, instead, is cache oblivious:
  it performs well with every cache size, and thus exploits all of the
  available caches.

  Assuming \p n is the number of elements in the tree and \p B is the number
  of (dimension_type, Coefficient) pairs that fit in a cache line, the
  time and cache misses complexities are the following:

  - Insertions/Queries/Deletions: \f$O(\log^2 n)\f$ time,
                                  \f$O(\log \frac{n}{B}))\f$ cache misses.
  - Tree traversal from begin() to end(), using an %iterator: \f$O(n)\f$ time,
         \f$O(\frac{n}{B})\f$  cache misses.
  - Queries with a hint: \f$O(\log k)\f$ time and \f$O(\log \frac{k}{B})\f$
    cache misses, where k is the distance between the given %iterator and the
    searched element (or the position where it would have been).

  The binary search tree is embedded in a (slightly bigger) complete tree,
  that is enlarged and shrunk when needed. The complete tree is laid out
  in an in-order DFS layout in two arrays: one for the keys and one for the
  associated data.
  The indexes and values are stored in different arrays to reduce
  cache-misses during key queries.

  The tree can store up to \f$(-(dimension_type)1)/100\f$ elements.
  This limit allows faster density computations, but can be removed if needed.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
class CO_Tree {

public:
  class const_iterator;
  class iterator;

private:
  //! This is used for node heights and depths in the tree.
  typedef unsigned height_t;

  PPL_COMPILE_TIME_CHECK(C_Integer<height_t>::max
                         >= sizeof_to_bits(sizeof(dimension_type)),
                         "height_t is too small to store depths.");

  class tree_iterator;

  // This must be declared here, because it is a friend of const_iterator.
  //! Returns the index of the current element in the DFS layout of the
  //! complete tree.
  /*!
    \return the index of the current element in the DFS layout of the complete
            tree.

    \param itr the iterator that points to the desired element.
  */
  dimension_type dfs_index(const_iterator itr) const;

  // This must be declared here, because it is a friend of iterator.
  //! Returns the index of the current element in the DFS layout of the
  //! complete tree.
  /*!
    \return the index of the current element in the DFS layout of the complete
            tree.

    \param itr the iterator that points to the desired element.
  */
  dimension_type dfs_index(iterator itr) const;

public:

  //! The type of the data elements associated with keys.
  /*!
    If this is changed, occurrences of Coefficient_zero() in the CO_Tree
    implementation have to be replaced with constants of the correct type.
  */
  typedef Coefficient data_type;
  typedef Coefficient_traits::const_reference data_type_const_reference;

  //! A const %iterator on the tree elements, ordered by key.
  /*!
    Iterator increment and decrement operations are \f$O(1)\f$ time.
    These iterators are invalidated by operations that add or remove elements
    from the tree.
  */
  class const_iterator {
  private:
  public:

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef const data_type value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef data_type_const_reference reference;

    //! Constructs an invalid const_iterator.
    /*!
      This constructor takes \f$O(1)\f$ time.
    */
    explicit const_iterator();

    //! Constructs an %iterator pointing to the first element of the tree.
    /*!
      \param tree
      The tree that the new %iterator will point to.

      This constructor takes \f$O(1)\f$ time.
    */
    explicit const_iterator(const CO_Tree& tree);

    //! Constructs a const_iterator pointing to the i-th node of the tree.
    /*!
      \param tree
      The tree that the new %iterator will point to.

      \param i
      The index of the element in \p tree to which the %iterator will point
      to.

      The i-th node must be a node with a value or end().

      This constructor takes \f$O(1)\f$ time.
    */
    const_iterator(const CO_Tree& tree, dimension_type i);

    //! The copy constructor.
    /*!
      \param itr
      The %iterator that will be copied.

      This constructor takes \f$O(1)\f$ time.
    */
    const_iterator(const const_iterator& itr);

    //! Converts an iterator into a const_iterator.
    /*!
      \param itr
      The iterator that will be converted into a const_iterator.

      This constructor takes \f$O(1)\f$ time.
    */
    const_iterator(const iterator& itr);

    //! Swaps itr with *this.
    /*!
      \param itr
      The %iterator that will be swapped with *this.

      This method takes \f$O(1)\f$ time.
    */
    void m_swap(const_iterator& itr);

    //! Assigns \p itr to *this .
    /*!
      \param itr
      The %iterator that will be assigned into *this.

      This method takes \f$O(1)\f$ time.
    */
    const_iterator& operator=(const const_iterator& itr);

    //! Assigns \p itr to *this .
    /*!
      \param itr
      The %iterator that will be assigned into *this.

      This method takes \f$O(1)\f$ time.
    */
    const_iterator& operator=(const iterator& itr);

    //! Navigates to the next element.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    const_iterator& operator++();

    //! Navigates to the previous element.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    const_iterator& operator--();

    //! Navigates to the next element.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    const_iterator operator++(int);

    //! Navigates to the previous element.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    const_iterator operator--(int);

    //! Returns the current element.
    data_type_const_reference operator*() const;

    //! Returns the index of the element pointed to by \c *this.
    /*!
      \returns the index of the element pointed to by \c *this.
    */
    dimension_type index() const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    bool operator==(const const_iterator& x) const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    bool operator!=(const const_iterator& x) const;

  private:
    //! Checks the internal invariants, in debug mode only.
    bool OK() const;

    //! A pointer to the corresponding element of the tree's indexes[] array.
    const dimension_type* current_index;

    //! A pointer to the corresponding element of the tree's data[] array.
    const data_type* current_data;

#if PPL_CO_TREE_EXTRA_DEBUG
    //! A pointer to the corresponding tree, used for debug purposes only.
    const CO_Tree* tree;
#endif

    friend dimension_type CO_Tree::dfs_index(const_iterator itr) const;
  };

  //! An %iterator on the tree elements, ordered by key.
  /*!
    Iterator increment and decrement operations are \f$O(1)\f$ time.
    These iterators are invalidated by operations that add or remove elements
    from the tree.
  */
  class iterator {
  public:

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef data_type value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    //! Constructs an invalid iterator.
    /*!
      This constructor takes \f$O(1)\f$ time.
    */
    iterator();

    //! Constructs an %iterator pointing to first element of the tree.
    /*!
      \param tree
      The tree to which the new %iterator will point to.

      This constructor takes \f$O(1)\f$ time.
    */
    explicit iterator(CO_Tree& tree);

    //! Constructs an %iterator pointing to the i-th node.
    /*!
      \param tree
      The tree to which the new %iterator will point to.

      \param i
      The index of the element in \p tree to which the new %iterator will
      point to.

      The i-th node must be a node with a value or end().

      This constructor takes \f$O(1)\f$ time.
    */
    iterator(CO_Tree& tree, dimension_type i);

    //! The constructor from a tree_iterator.
    /*!
      \param itr
      The tree_iterator that will be converted into an iterator.

      This is meant for use by CO_Tree only.
      This is not private to avoid the friend declaration.

      This constructor takes \f$O(1)\f$ time.
    */
    explicit iterator(const tree_iterator& itr);

    //! The copy constructor.
    /*!
      \param itr
      The %iterator that will be copied.

      This constructor takes \f$O(1)\f$ time.
    */
    iterator(const iterator& itr);

    //! Swaps itr with *this.
    /*!
      \param itr
      The %iterator that will be swapped with *this.

      This method takes \f$O(1)\f$ time.
    */
    void m_swap(iterator& itr);

    //! Assigns \p itr to *this .
    /*!
      \param itr
      The %iterator that will be assigned into *this.

      This method takes \f$O(1)\f$ time.
    */
    iterator& operator=(const iterator& itr);

    //! Assigns \p itr to *this .
    /*!
      \param itr
      The %iterator that will be assigned into *this.

      This method takes \f$O(1)\f$ time.
    */
    iterator& operator=(const tree_iterator& itr);

    //! Navigates to the next element in the tree.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    iterator& operator++();

    //! Navigates to the previous element in the tree.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    iterator& operator--();

    //! Navigates to the next element in the tree.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    iterator operator++(int);

    //! Navigates to the previous element in the tree.
    /*!
      This method takes \f$O(1)\f$ time.
    */
    iterator operator--(int);

    //! Returns the current element.
    data_type& operator*();

    //! Returns the current element.
    data_type_const_reference operator*() const;

    //! Returns the index of the element pointed to by \c *this.
    /*!
      \returns the index of the element pointed to by \c *this.
    */
    dimension_type index() const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    bool operator==(const iterator& x) const;

    //! Compares \p *this with x .
    /*!
      \param x
      The %iterator that will be compared with *this.
    */
    bool operator!=(const iterator& x) const;

  private:
    //! Checks the internal invariants, in debug mode only.
    bool OK() const;

    //! A pointer to the corresponding element of the tree's indexes[] array.
    const dimension_type* current_index;

    //! A pointer to the corresponding element of the tree's data[] array.
    data_type* current_data;

#if PPL_CO_TREE_EXTRA_DEBUG
    //! A pointer to the corresponding tree, used for debug purposes only.
    CO_Tree* tree;
#endif

    friend const_iterator& const_iterator::operator=(const iterator&);
    friend dimension_type CO_Tree::dfs_index(iterator itr) const;
  };

  //! Constructs an empty tree.
  /*!
    This constructor takes \f$O(1)\f$ time.
  */
  CO_Tree();

  //! The copy constructor.
  /*!
    \param y
    The tree that will be copied.

    This constructor takes \f$O(n)\f$ time.
  */
  CO_Tree(const CO_Tree& y);

  //! A constructor from a sequence of \p n elements.
  /*!
    \param i
    An iterator that points to the first element of the sequence.

    \param n
    The number of elements in the [i, i_end) sequence.

    i must be an input iterator on a sequence of data_type elements,
    sorted by index.
    Objects of Iterator type must have an index() method that returns the
    index with which the element pointed to by the iterator must be inserted.

    This constructor takes \f$O(n)\f$ time, so it is more efficient than
    the construction of an empty tree followed by n insertions, that would
    take \f$O(n*\log^2 n)\f$ time.
  */
  template <typename Iterator>
  CO_Tree(Iterator i, dimension_type n);

  //! The assignment operator.
  /*!
    \param y
    The tree that will be assigned to *this.

    This method takes \f$O(n)\f$ time.
  */
  CO_Tree& operator=(const CO_Tree& y);

  //! Removes all elements from the tree.
  /*!
    This method takes \f$O(n)\f$ time.
  */
  void clear();

  //! The destructor.
  /*!
    This destructor takes \f$O(n)\f$ time.
  */
  ~CO_Tree();

  //! Returns \p true if the tree has no elements.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  bool empty() const;

  //! Returns the number of elements stored in the tree.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  dimension_type size() const;

  //! Returns the size() of the largest possible CO_Tree.
  static dimension_type max_size();

  //! Dumps the tree to stdout, for debugging purposes.
  void dump_tree() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  /*!
    This method takes \f$O(n)\f$ time.
  */
  dimension_type external_memory_in_bytes() const;

  //! Inserts an element in the tree.
  /*!
    \returns
    An %iterator that points to the inserted pair.

    \param key
    The key that will be inserted into the tree, associated with the default
    data.

    If such a pair already exists, an %iterator pointing to that pair is
    returned.

    This operation invalidates existing iterators.

    This method takes \f$O(\log n)\f$ time if the element already exists, and
    \f$O(\log^2 n)\f$ amortized time otherwise.
  */
  iterator insert(dimension_type key);

  //! Inserts an element in the tree.
  /*!
    \returns
    An %iterator that points to the inserted element.

    \param key
    The key that will be inserted into the tree..

    \param data
    The data that will be inserted into the tree.

    If an element with the specified key already exists, its associated data
    is set to \p data and an %iterator pointing to that pair is returned.

    This operation invalidates existing iterators.

    This method takes \f$O(\log n)\f$ time if the element already exists, and
    \f$O(\log^2 n)\f$ amortized time otherwise.amortized
  */
  iterator insert(dimension_type key, data_type_const_reference data);

  //! Inserts an element in the tree.
  /*!
    \return
    An %iterator that points to the inserted element.

    \param itr
    The %iterator used as hint

    \param key
    The key that will be inserted into the tree, associated with the default
    data.

    This will be faster if \p itr points near to the place where the new
    element will be inserted (or where is already stored).
    However, the value of \p itr does not affect the result of this
    method, as long it is a valid %iterator for this tree. \p itr may even be
    end().

    If an element with the specified key already exists, an %iterator pointing
    to that pair is returned.

    This operation invalidates existing iterators.

    This method takes \f$O(\log n)\f$ time if the element already exists, and
    \f$O(\log^2 n)\f$ amortized time otherwise.
  */
  iterator insert(iterator itr, dimension_type key);

  //! Inserts an element in the tree.
  /*!
    \return
    An iterator that points to the inserted element.

    \param itr
    The iterator used as hint

    \param key
    The key that will be inserted into the tree.

    \param data
    The data that will be inserted into the tree.

    This will be faster if \p itr points near to the place where the new
    element will be inserted (or where is already stored).
    However, the value of \p itr does not affect the result of this
    method, as long it is a valid iterator for this tree. \p itr may even be
    end().

    If an element with the specified key already exists, its associated data
    is set to \p data and an iterator pointing to that pair is returned.

    This operation invalidates existing iterators.

    This method takes \f$O(\log n)\f$ time if the element already exists,
    and \f$O(\log^2 n)\f$ amortized time otherwise.
  */
  iterator insert(iterator itr, dimension_type key,
                  data_type_const_reference data);

  //! Erases the element with key \p key from the tree.
  /*!
    This operation invalidates existing iterators.

    \returns an iterator to the next element (or end() if there are no
             elements with key greater than \p key ).

    \param key
    The key of the element that will be erased from the tree.

    This method takes \f$O(\log n)\f$ time if the element already exists,
    and \f$O(\log^2 n)\f$ amortized time otherwise.
  */
  iterator erase(dimension_type key);

  //! Erases the element pointed to by \p itr from the tree.
  /*!
    This operation invalidates existing iterators.

    \returns an iterator to the next element (or end() if there are no
             elements with key greater than \p key ).

    \param itr
    An iterator pointing to the element that will be erased from the tree.

    This method takes \f$O(\log n)\f$ time if the element already exists, and
    \f$O(\log^2 n)\f$ amortized time otherwise.
  */
  iterator erase(iterator itr);

  /*!
    \brief Removes the element with key \p key (if it exists) and decrements
           by 1 all elements' keys that were greater than \p key.

    \param key
    The key of the element that will be erased from the tree.

    This operation invalidates existing iterators.

    This method takes \f$O(k+\log^2 n)\f$ expected time, where k is the number
    of elements with keys greater than \p key.
  */
  void erase_element_and_shift_left(dimension_type key);

  //! Adds \p n to all keys greater than or equal to \p key.
  /*!
    \param key
    The key of the first element whose key will be increased.

    \param n
    Specifies how much the keys will be increased.

    This method takes \f$O(k+\log n)\f$ expected time, where k is the number
    of elements with keys greater than or equal to \p key.
  */
  void increase_keys_from(dimension_type key, dimension_type n);

  //! Sets to \p i the key of *itr. Assumes that i<=itr.index() and that there
  //! are no elements with keys in [i,itr.index()).
  /*!
    All existing iterators remain valid.

    This method takes \f$O(1)\f$ time.
  */
  void fast_shift(dimension_type i, iterator itr);

  //! Swaps x with *this.
  /*!
    \param x
    The tree that will be swapped with *this.

    This operation invalidates existing iterators.

    This method takes \f$O(1)\f$ time.
  */
  void m_swap(CO_Tree& x);

  //! Returns an iterator that points at the first element.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  iterator begin();

  //! Returns an iterator that points after the last element.
  /*!
    This method always returns a reference to the same internal %iterator,
    that is updated at each operation that modifies the structure.
    Client code can keep a const reference to that %iterator instead of
    keep updating a local %iterator.

    This method takes \f$O(1)\f$ time.
  */
  const iterator& end();

  //! Equivalent to cbegin().
  const_iterator begin() const;

  //! Equivalent to cend().
  const const_iterator& end() const;

  //! Returns a const_iterator that points at the first element.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  const_iterator cbegin() const;

  //! Returns a const_iterator that points after the last element.
  /*!
    This method always returns a reference to the same internal %iterator,
    that is updated at each operation that modifies the structure.
    Client code can keep a const reference to that %iterator instead of
    keep updating a local %iterator.

    This method takes \f$O(1)\f$ time.
  */
  const const_iterator& cend() const;

  //! Searches an element with key \p key using bisection.
  /*!
    \param key
    The key that will be searched for.

    If the element is found, an %iterator pointing to that element is
    returned; otherwise, the returned %iterator refers to the immediately
    preceding or succeeding value.
    If the tree is empty, end() is returned.

    This method takes \f$O(\log n)\f$ time.
  */
  iterator bisect(dimension_type key);

  //! Searches an element with key \p key using bisection.
  /*!
    \param key
    The key that will be searched for.

    If the element is found, an %iterator pointing to that element is
    returned; otherwise, the returned %iterator refers to the immediately
    preceding or succeeding value.
    If the tree is empty, end() is returned.

    This method takes \f$O(\log n)\f$ time.
  */
  const_iterator bisect(dimension_type key) const;

  //! Searches an element with key \p key in [first, last] using bisection.
  /*!
    \param first
    An %iterator pointing to the first element in the range.
    It must not be end().

    \param last
    An %iterator pointing to the last element in the range.
    Note that this is included in the search.
    It must not be end().

    \param key
    The key that will be searched for.

    \return
    If the specified key is found, an %iterator pointing to that element is
    returned; otherwise, the returned %iterator refers to the immediately
    preceding or succeeding value.
    If the tree is empty, end() is returned.

    This method takes \f$O(\log(last - first + 1))\f$ time.
  */
  iterator bisect_in(iterator first, iterator last, dimension_type key);

  //! Searches an element with key \p key in [first, last] using bisection.
  /*!
    \param first
    An %iterator pointing to the first element in the range.
    It must not be end().

    \param last
    An %iterator pointing to the last element in the range.
    Note that this is included in the search.
    It must not be end().

    \param key
    The key that will be searched for.

    \return
    If the specified key is found, an %iterator pointing to that element is
    returned; otherwise, the returned %iterator refers to the immediately
    preceding or succeeding value.
    If the tree is empty, end() is returned.

    This method takes \f$O(\log(last - first + 1))\f$ time.
  */
  const_iterator bisect_in(const_iterator first, const_iterator last,
                           dimension_type key) const;

  //! Searches an element with key \p key near \p hint.
  /*!
    \param hint
    An %iterator used as a hint.

    \param key
    The key that will be searched for.

    If the element is found, the returned %iterator points to that element;
    otherwise, it points to the immediately preceding or succeeding value.
    If the tree is empty, end() is returned.

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this tree. \p itr may even be end().

    This method takes \f$O(\log n)\f$ time. If the distance between the
    returned position and \p hint is \f$O(1)\f$ it takes \f$O(1)\f$ time.
  */
  iterator bisect_near(iterator hint, dimension_type key);

  //! Searches an element with key \p key near \p hint.
  /*!
    \param hint
    An %iterator used as a hint.

    \param key
    The key that will be searched for.

    If the element is found, the returned %iterator points to that element;
    otherwise, it points to the immediately preceding or succeeding value.
    If the tree is empty, end() is returned.

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this tree. \p itr may even be end().

    This method takes \f$O(\log n)\f$ time. If the distance between the
    returned position and \p hint is \f$O(1)\f$ it takes \f$O(1)\f$ time.
  */
  const_iterator bisect_near(const_iterator hint, dimension_type key) const;

private:

  //! Searches an element with key \p key in [first, last] using bisection.
  /*!
    \param first
    The index of the first element in the range.
    It must be the index of an element with a value.

    \param last
    The index of the last element in the range.
    It must be the index of an element with a value.
    Note that this is included in the search.

    \param key
    The key that will be searched for.

    \return
    If the element is found, the index of that element is returned; otherwise,
    the returned index refers to the immediately preceding or succeeding
    value.

    This method takes \f$O(\log n)\f$ time.
  */
  dimension_type bisect_in(dimension_type first, dimension_type last,
                           dimension_type key) const;

  //! Searches an element with key \p key near \p hint.
  /*!
    \param hint
    An index used as a hint.
    It must be the index of an element with a value.

    \param key
    The key that will be searched for.

    \return
    If the element is found, the index of that element is returned; otherwise,
    the returned index refers to the immediately preceding or succeeding
    value.

    This uses a binary progression and then a bisection, so this method is
    \f$O(\log n)\f$, and it is \f$O(1)\f$ if the distance between the returned
    position and \p hint is \f$O(1)\f$.

    This method takes \f$O(\log n)\f$ time. If the distance between the
    returned position and \p hint is \f$O(1)\f$ it takes \f$O(1)\f$ time.
  */
  dimension_type bisect_near(dimension_type hint, dimension_type key) const;

  //! Inserts an element in the tree.
  /*!
    If there is already an element with key \p key in the tree, its
    associated data is set to \p data.

    This operation invalidates existing iterators.

    \return
    An %iterator that points to the inserted element.

    \param key
    The key that will be inserted into the tree.

    \param data
    The data that will be associated with \p key.

    \param itr
    It must point to the element in the tree with key \p key or, if no such
    element exists, it must point to the node that would be his parent.

    This method takes \f$O(1)\f$ time if the element already exists, and
    \f$O(\log^2 n)\f$ amortized time otherwise.
  */
  tree_iterator insert_precise(dimension_type key,
                               data_type_const_reference data,
                               tree_iterator itr);

  //! Helper for \c insert_precise.
  /*!
    This helper method takes the same arguments as \c insert_precise,
    but besides assuming that \p itr is a correct hint, it also assumes
    that \p key and \p data are not in the tree; namely, a proper
    insertion has to be done and the insertion can not invalidate \p data.
  */
  tree_iterator insert_precise_aux(dimension_type key,
                                   data_type_const_reference data,
                                   tree_iterator itr);

  //! Inserts an element in the tree.
  /*!

    \param key
    The key that will be inserted into the tree.

    \param data
    The data that will be associated with \p key.

    The tree must be empty.

    This operation invalidates existing iterators.

    This method takes \f$O(1)\f$ time.
  */
  void insert_in_empty_tree(dimension_type key,
                            data_type_const_reference data);

  //! Erases from the tree the element pointed to by \p itr .
  /*!
    This operation invalidates existing iterators.

    \returns
    An %iterator to the next element (or end() if there are no elements with
    key greater than \p key ).

    \param itr
    An %iterator pointing to the element that will be erased.

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  iterator erase(tree_iterator itr);

  //! Initializes a tree with reserved size at least \p n .
  /*!
    \param n
    A lower bound on the tree's desired reserved size.

    This method takes \f$O(n)\f$ time.
  */
  void init(dimension_type n);

  //! Deallocates the tree's dynamic arrays.
  /*!
    After this call, the tree fields are uninitialized, so init() must be
    called again before using the tree.

    This method takes \f$O(n)\f$ time.
  */
  void destroy();

  //! Checks the internal invariants, but not the densities.
  bool structure_OK() const;

  //! Checks the internal invariants.
  bool OK() const;

  //! Returns the floor of the base-2 logarithm of \p n .
  /*!
    \param n
    It must be greater than zero.

    This method takes \f$O(\log n)\f$ time.
  */
  static unsigned integer_log2(dimension_type n);

  //! Compares the fractions numer/denom with ratio/100.
  /*!
    \returns Returns true if the fraction numer/denom is less
    than the fraction ratio/100.

    \param ratio
    It must be less than or equal to 100.

    \param numer
    The numerator of the fraction.

    \param denom
    The denominator of the fraction.

    This method takes \f$O(1)\f$ time.
  */
  static bool is_less_than_ratio(dimension_type numer, dimension_type denom,
                                 dimension_type ratio);

  //! Compares the fractions numer/denom with ratio/100.
  /*!
    \returns
    Returns true if the fraction numer/denom is greater than the fraction
    ratio/100.

    \param ratio
    It must be less than or equal to 100.

    \param numer
    The numerator of the fraction.

    \param denom
    The denominator of the fraction.

    This method takes \f$O(1)\f$ time.
  */
  static bool is_greater_than_ratio(dimension_type numer, dimension_type denom,
                                    dimension_type ratio);

  //! Dumps the subtree rooted at \p itr to stdout, for debugging purposes.
  /*!
    \param itr
    A tree_iterator pointing to the root of the desired subtree.
  */
  static void dump_subtree(tree_iterator itr);

  //! Increases the tree's reserved size.
  /*!
    This is called when the density is about to exceed the maximum density
    (specified by max_density_percent).

    This method takes \f$O(n)\f$ time.
  */
  void rebuild_bigger_tree();

  //! Decreases the tree's reserved size.
  /*!
    This is called when the density is about to become less than the minimum
    allowed density (specified by min_density_percent).

    \p reserved_size must be greater than 3 (otherwise the tree can just be
    cleared).

    This method takes \f$O(n)\f$ time.
  */
  void rebuild_smaller_tree();

  //! Re-initializes the cached iterators.
  /*!
    This method must be called when the indexes[] and data[] vector are
    reallocated.

    This method takes \f$O(1)\f$ time.
  */
  void refresh_cached_iterators();

  //! Rebalances the tree.
  /*!
    For insertions, it adds the pair (key, value) in the process.

    This operation invalidates existing iterators that point to nodes in the
    rebalanced subtree.

    \returns an %iterator pointing to the root of the subtree that was
             rebalanced.

    \param itr
    It points to the node where the new element has to be inserted or where an
    element has just been deleted.

    \param key
    The index that will be inserted in the tree (for insertions only).

    \param value
    The value that will be inserted in the tree (for insertions only).

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  tree_iterator rebalance(tree_iterator itr, dimension_type key,
                          data_type_const_reference value);

  //! Moves all elements of a subtree to the rightmost end.
  /*!
    \returns
    The index of the rightmost unused node in the subtree after the process.

    \param last_in_subtree
    It is the index of the last element in the subtree.

    \param subtree_size
    It is the number of valid elements in the subtree.
    It must be greater than zero.

    \param key
    The key that may be added to the tree if add_element is \c true.

    \param value
    The value that may be added to the tree if add_element is \c true.

    \param add_element
    If it is true, it tries to add an element with key \p key and value
    \p value in the process (but it may not).

    This method takes \f$O(k)\f$ time, where k is \p subtree_size.
  */
  dimension_type compact_elements_in_the_rightmost_end(
    dimension_type last_in_subtree, dimension_type subtree_size,
    dimension_type key, data_type_const_reference value,
    bool add_element);

  //! Redistributes the elements in the subtree rooted at \p root_index.
  /*!
    The subtree's elements must be compacted to the rightmost end.

    \param root_index
    The index of the subtree's root node.

    \param subtree_size
    It is the number of used elements in the subtree.
    It must be greater than zero.

    \param last_used
    It points to the leftmost element with a value in the subtree.

    \param add_element
    If it is true, this method adds an element with the specified key and
    value in the process.

    \param key
    The key that will be added to the tree if \p add_element is \c true.

    \param value
    The data that will be added to the tree if \p add_element is \c true.

    This method takes \f$O(k)\f$ time, where k is \p subtree_size.
  */
  void redistribute_elements_in_subtree(dimension_type root_index,
                                        dimension_type subtree_size,
                                        dimension_type last_used,
                                        dimension_type key,
                                        data_type_const_reference value,
                                        bool add_element);

  //! Moves all data in the tree \p tree into *this.
  /*!
    \param tree
    The tree from which the element will be moved into *this.

    *this must be empty and big enough to contain all of tree's data without
    exceeding max_density.

    This method takes \f$O(n)\f$ time.
  */
  void move_data_from(CO_Tree& tree);

  //! Copies all data in the tree \p tree into *this.
  /*!
    \param tree
    The tree from which the element will be copied into *this.

    *this must be empty and must have the same reserved size of \p tree.
    this->OK() may return false before this method is called, but
    this->structure_OK() must return true.

    This method takes \f$O(n)\f$ time.
  */
  void copy_data_from(const CO_Tree& tree);

  //! Counts the number of used elements in the subtree rooted at itr.
  /*!
    \param itr
    An %iterator pointing to the root of the desired subtree.

    This method takes \f$O(k)\f$ time, where k is the number of elements in
    the subtree.
  */
  static dimension_type count_used_in_subtree(tree_iterator itr);

  //! Moves the value of \p from in \p to .
  /*!
    \param from
    It must be a valid value.

    \param to
    It must be a non-constructed chunk of memory.

    After the move, \p from becomes a non-constructed chunk of memory and
    \p to gets the value previously stored by \p from.

    The implementation of this method assumes that data_type values do not
    keep pointers to themselves nor to their fields.

    This method takes \f$O(1)\f$ time.
  */
  static void move_data_element(data_type& to, data_type& from);

  //! The maximum density of used nodes.
  /*!
    This must be greater than or equal to 50 and lower than 100.
  */
  static const dimension_type max_density_percent = 91;

  //! The minimum density of used nodes.
  /*!
    Must be strictly lower than the half of max_density_percent.
  */
  static const dimension_type min_density_percent = 38;

  //! The minimum density at the leaves' depth.
  /*!
    Must be greater than zero and strictly lower than min_density_percent.

    Increasing the value is safe but leads to time inefficiencies
    (measured against ppl_lpsol on 24 August 2010), because it forces trees to
    be more balanced, increasing the cost of rebalancing.
  */
  static const dimension_type min_leaf_density_percent = 1;

  //! An index used as a marker for unused nodes in the tree.
  /*!
    This must not be used as a key.
  */
  static const dimension_type unused_index = C_Integer<dimension_type>::max;

  //! The %iterator returned by end().
  /*!
    It is updated when needed, to keep it valid.
  */
  iterator cached_end;

  //! The %iterator returned by the const version of end().
  /*!
    It is updated when needed, to keep it valid.
  */
  const_iterator cached_const_end;

  //! The depth of the leaves in the complete tree.
  height_t max_depth;

  //! The vector that contains the keys in the tree.
  /*!
    If an element of this vector is \p unused_index , it means that that
    element and the corresponding element of data[] are not used.

    Its size is reserved_size + 2, because the first and the last elements
    are used as markers for iterators.
  */
  dimension_type* indexes;

  //! The allocator used to allocate/deallocate data.
  std::allocator<data_type> data_allocator;

  //! The vector that contains the data of the keys in the tree.
  /*!
    If index[i] is \p unused_index, data[i] is unused.
    Otherwise, data[i] contains the data associated to the indexes[i] key.

    Its size is reserved_size + 1, because the first element is not used (to
    allow using the same index in both indexes[] and data[] instead of
    adding 1 to access data[]).
  */
  data_type* data;

  //! The number of nodes in the complete tree.
  /*!
    It is one less than a power of 2.
    If this is 0, data and indexes are set to NULL.
  */
  dimension_type reserved_size;

  //! The number of values stored in the tree.
  dimension_type size_;
};

class CO_Tree::tree_iterator {

public:

  /*!
    \brief Constructs a tree_iterator pointing at the root node of the
           specified tree

    \param tree
    The tree to which the new %iterator will point to.
    It must not be empty.
  */
  explicit tree_iterator(CO_Tree& tree);

  //! Constructs a tree_iterator pointing at the specified node of the tree.
  /*!
    \param tree
    The tree to which the new %iterator will point to.
    It must not be empty.

    \param i
    The index of the element in \p tree to which the new %iterator will point
    to.
  */
  tree_iterator(CO_Tree& tree, dimension_type i);

  //! Constructs a tree_iterator from an iterator.
  /*!
    \param itr
    The iterator that will be converted into a tree_iterator.
    It must not be end().

    \param tree
    The tree to which the new %iterator will point to.
    It must not be empty.
  */
  tree_iterator(const iterator& itr, CO_Tree& tree);

  //! The assignment operator.
  /*!
    \param itr
    The %iterator that will be assigned into *this.
  */
  tree_iterator& operator=(const tree_iterator& itr);

  //! The assignment operator from an iterator.
  /*!
    \param itr
    The iterator that will be assigned into *this.
  */
  tree_iterator& operator=(const iterator& itr);

  //! Compares *this with \p itr.
  /*!
    \param itr
    The %iterator that will compared with *this.
  */
  bool operator==(const tree_iterator& itr) const;

  //! Compares *this with \p itr.
  /*!
    \param itr
    The %iterator that will compared with *this.
  */
  bool operator!=(const tree_iterator& itr) const;

  //! Makes the %iterator point to the root of \p tree.
  /*!
    The values of all fields (beside tree) are overwritten.

    This method takes \f$O(1)\f$ time.
  */
  void get_root();

  //! Makes the %iterator point to the left child of the current node.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  void get_left_child();

  //! Makes the %iterator point to the right child of the current node.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  void get_right_child();

  //! Makes the %iterator point to the parent of the current node.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  void get_parent();

  /*!
    \brief Searches for an element with key \p key in the subtree rooted at
           \p *this.

    \param key
    The searched for key.

    After this method, *this points to the found node (if it exists) or to
    the node that would be his parent (otherwise).

    This method takes \f$O(\log n)\f$ time.
  */
  void go_down_searching_key(dimension_type key);

  /*!
    \brief Follows left children with a value, until it arrives at a leaf or at
           a node with no value.

    This method takes \f$O(1)\f$ time.
  */
  void follow_left_children_with_value();

  /*!
    \brief Follows right children with a value, until it arrives at a leaf or at
           a node with no value.

    This method takes \f$O(1)\f$ time.
  */
  void follow_right_children_with_value();

  //! Returns true if the pointed node is the root node.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  bool is_root() const;

  //! Returns true if the pointed node has a parent and is its right child.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  bool is_right_child() const;

  //! Returns true if the pointed node is a leaf of the complete tree.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  bool is_leaf() const;

  //! Returns the key and value of the current node.
  data_type& operator*();

  //! Returns the key and value of the current node.
  Coefficient_traits::const_reference operator*() const;

  //! Returns a reference to the index of the element pointed to by \c *this.
  /*!
    \returns a reference to the index of the element pointed to by \c *this.
  */
  dimension_type& index();

  //! Returns the index of the element pointed to by \c *this.
  /*!
    \returns the index of the element pointed to by \c *this.
  */
  dimension_type index() const;

  //! Returns the index of the node pointed to by \c *this.
  /*!
    \returns the key of the node pointed to by \c *this, or unused_index if
             the current node does not contain a valid element.
  */
  dimension_type key() const;

  //! The tree containing the element pointed to by this %iterator.
  CO_Tree& tree;

  /*!
    \brief Returns the index of the current node in the DFS layout of the
           complete tree.
  */
  dimension_type dfs_index() const;

  /*!
    \brief Returns 2^h, with h the height of the current node in the tree,
           counting from 0.

    Thus leaves have offset 1.
    This is faster than depth(), so it is useful to compare node depths.

    This method takes \f$O(1)\f$ time.
  */
  dimension_type get_offset() const;

  //! Returns the depth of the current node in the complete tree.
  /*!
    This method takes \f$O(\log n)\f$ time.
  */
  height_t depth() const;

private:
  //! Checks the internal invariant.
  bool OK() const;

  //! The index of the current node in the DFS layout of the complete tree.
  dimension_type i;

  /*!
    \brief This is 2^h, with h the height of the current node in the tree,
           counting from 0.

    Thus leaves have offset 1.
    This is equal to (i & -i), and is only stored to increase performance.
  */
  dimension_type offset;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates CO_Tree */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void swap(CO_Tree& x, CO_Tree& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates CO_Tree::const_iterator */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void swap(CO_Tree::const_iterator& x, CO_Tree::const_iterator& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates CO_Tree::iterator */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void swap(CO_Tree::iterator& x, CO_Tree::iterator& y);

} // namespace Parma_Polyhedra_Library

#include "CO_Tree_inlines.hh"
#include "CO_Tree_templates.hh"

#endif // !defined(PPL_CO_Tree_defs_hh)
