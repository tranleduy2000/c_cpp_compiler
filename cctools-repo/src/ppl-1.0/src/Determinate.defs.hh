/* Determinate class declaration.
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

#ifndef PPL_Determinate_defs_hh
#define PPL_Determinate_defs_hh

#include "Determinate.types.hh"
#include "Constraint_System.types.hh"
#include "Congruence_System.types.hh"
#include "Variable.defs.hh"
#include "globals.types.hh"
#include <iosfwd>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

//! Swaps \p x with \p y.
/*! \relates Determinate */
template <typename PSET>
void swap(Determinate<PSET>& x, Determinate<PSET>& y);

/*! \brief
  Returns <CODE>true</CODE> if and only if \p x and \p y are the same
  COW-wrapped pointset.

  \relates Determinate
*/
template <typename PSET>
bool operator==(const Determinate<PSET>& x, const Determinate<PSET>& y);

/*! \brief
  Returns <CODE>true</CODE> if and only if \p x and \p y are different
  COW-wrapped pointsets.

  \relates Determinate
*/
template <typename PSET>
bool operator!=(const Determinate<PSET>& x, const Determinate<PSET>& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Determinate */
template <typename PSET>
std::ostream&
operator<<(std::ostream&, const Determinate<PSET>&);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

/*! \brief
  A wrapper for PPL pointsets, providing them with a
  <EM>determinate constraint system</EM> interface, as defined
  in \ref Bag98 "[Bag98]".

  The implementation uses a copy-on-write optimization, making the
  class suitable for constructions, like the <EM>finite powerset</EM></A>
  and <EM>ask-and-tell</EM> of \ref Bag98 "[Bag98]", that are likely
  to perform many copies.

  \ingroup PPL_CXX_interface
*/
template <typename PSET>
class Parma_Polyhedra_Library::Determinate {
public:
  //! \name Constructors and Destructor
  //@{

  /*! \brief
    Constructs a COW-wrapped object corresponding to the pointset \p pset.
  */
  Determinate(const PSET& pset);

  /*! \brief
    Constructs a COW-wrapped object corresponding to the pointset
    defined by \p cs.
  */
  Determinate(const Constraint_System& cs);

  /*! \brief
    Constructs a COW-wrapped object corresponding to the pointset
    defined by \p cgs.
  */
  Determinate(const Congruence_System& cgs);

  //! Copy constructor.
  Determinate(const Determinate& y);

  //! Destructor.
  ~Determinate();

  //@} // Constructors and Destructor

  //! \name Member Functions that Do Not Modify the Domain Element
  //@{

  //! Returns a const reference to the embedded pointset.
  const PSET& pointset() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this embeds the universe
    element \p PSET.
  */
  bool is_top() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this embeds the empty
    element of \p PSET.
  */
  bool is_bottom() const;

  //! Returns <CODE>true</CODE> if and only if \p *this entails \p y.
  bool definitely_entails(const Determinate& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this and \p y
    are definitely equivalent.
  */
  bool is_definitely_equivalent_to(const Determinate& y) const;

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  memory_size_type total_memory_in_bytes() const;

  /*! \brief
    Returns a lower bound to the size in bytes of the memory
    managed by \p *this.
  */
  memory_size_type external_memory_in_bytes() const;

  /*!
    Returns <CODE>true</CODE> if and only if this domain
    has a nontrivial weakening operator.
  */
  static bool has_nontrivial_weakening();

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  //@} // Member Functions that Do Not Modify the Domain Element


  //! \name Member Functions that May Modify the Domain Element
  //@{

  //! Assigns to \p *this the upper bound of \p *this and \p y.
  void upper_bound_assign(const Determinate& y);

  //! Assigns to \p *this the meet of \p *this and \p y.
  void meet_assign(const Determinate& y);

  //! Assigns to \p *this the result of weakening \p *this with \p y.
  void weakening_assign(const Determinate& y);

  /*! \brief
    Assigns to \p *this the \ref Concatenating_Polyhedra "concatenation"
    of \p *this and \p y, taken in this order.
  */
  void concatenate_assign(const Determinate& y);

  //! Returns a reference to the embedded element.
  PSET& pointset();

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    On return from this method, the representation of \p *this
    is not shared by different Determinate objects.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  void mutate();

  //! Assignment operator.
  Determinate& operator=(const Determinate& y);

  //! Swaps \p *this with \p y.
  void m_swap(Determinate& y);

  //@} // Member Functions that May Modify the Domain Element

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! A function adapter for the Determinate class.
  /*! \ingroup PPL_CXX_interface
    It lifts a Binary_Operator_Assign function object, taking arguments
    of type PSET, producing the corresponding function object taking
    arguments of type Determinate<PSET>.

    The template parameter Binary_Operator_Assign is supposed to
    implement an <EM>apply and assign</EM> function, i.e., a function
    having signature <CODE>void foo(PSET& x, const PSET& y)</CODE> that
    applies an operator to \c x and \c y and assigns the result to \c x.
    For instance, such a function object is obtained by
    <CODE>std::mem_fun_ref(&C_Polyhedron::intersection_assign)</CODE>.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  template <typename Binary_Operator_Assign>
  class Binary_Operator_Assign_Lifter {
  public:
    //! Explicit unary constructor.
    explicit
    Binary_Operator_Assign_Lifter(Binary_Operator_Assign op_assign);

    //! Function-application operator.
    void operator()(Determinate& x, const Determinate& y) const;

  private:
    //! The function object to be lifted.
    Binary_Operator_Assign op_assign_;
  };

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Helper function returning a Binary_Operator_Assign_Lifter object,
    also allowing for the deduction of template arguments.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  template <typename Binary_Operator_Assign>
  static Binary_Operator_Assign_Lifter<Binary_Operator_Assign>
  lift_op_assign(Binary_Operator_Assign op_assign);

private:
  //! The possibly shared representation of a Determinate object.
  /*! \ingroup PPL_CXX_interface
    By adopting the <EM>copy-on-write</EM> technique, a single
    representation of the base-level object may be shared by more than
    one object of the class Determinate.
  */
  class Rep {
  private:
    /*! \brief
      Count the number of references:
      -   0: leaked, \p pset is non-const;
      -   1: one reference, \p pset is non-const;
      - > 1: more than one reference, \p pset is const.
    */
    mutable unsigned long references;

    //! Private and unimplemented: assignment not allowed.
    Rep& operator=(const Rep& y);

    //! Private and unimplemented: copies not allowed.
    Rep(const Rep& y);

    //! Private and unimplemented: default construction not allowed.
    Rep();

  public:
    //! The possibly shared, embedded pointset.
    PSET pset;

    /*! \brief
      Builds a new representation by creating a pointset
      of the specified kind, in the specified vector space.
    */
    Rep(dimension_type num_dimensions, Degenerate_Element kind);

    //! Builds a new representation by copying the pointset \p p.
    Rep(const PSET& p);

    //! Builds a new representation by copying the constraints in \p cs.
    Rep(const Constraint_System& cs);

    //! Builds a new representation by copying the constraints in \p cgs.
    Rep(const Congruence_System& cgs);

    //! Destructor.
    ~Rep();

    //! Registers a new reference.
    void new_reference() const;

    /*! \brief
      Unregisters one reference; returns <CODE>true</CODE> if and only if
      the representation has become unreferenced.
    */
    bool del_reference() const;

    //! True if and only if this representation is currently shared.
    bool is_shared() const;

    /*! \brief
      Returns a lower bound to the total size in bytes of the memory
      occupied by \p *this.
    */
    memory_size_type total_memory_in_bytes() const;

    /*! \brief
      Returns a lower bound to the size in bytes of the memory
      managed by \p *this.
    */
    memory_size_type external_memory_in_bytes() const;
  };

  /*! \brief
    A pointer to the possibly shared representation of
    the base-level domain element.
  */
  Rep* prep;

  friend bool
  operator==<PSET>(const Determinate<PSET>& x, const Determinate<PSET>& y);

  friend bool
  operator!=<PSET>(const Determinate<PSET>& x, const Determinate<PSET>& y);
};

#include "Determinate.inlines.hh"

#endif // !defined(PPL_Determinate_defs_hh)
