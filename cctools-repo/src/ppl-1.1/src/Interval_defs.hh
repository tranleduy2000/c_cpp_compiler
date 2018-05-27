/* Declarations for the Interval class and its constituents.
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

#ifndef PPL_Interval_defs_hh
#define PPL_Interval_defs_hh 1

#include "globals_defs.hh"
#include "meta_programming.hh"
#include "assign_or_swap.hh"
#include "intervals_defs.hh"
#include "Interval_types.hh"
#include "Interval_Info_defs.hh"
#include <iosfwd>

// Temporary!
#include <iostream>

namespace Parma_Polyhedra_Library {

enum Ternary { T_YES, T_NO, T_MAYBE };

inline I_Result
combine(Result l, Result u) {
  const unsigned res
    = static_cast<unsigned>(l) | (static_cast<unsigned>(u) << 6);
  return static_cast<I_Result>(res);
}

struct Interval_Base {
};

using namespace Boundary_NS;
using namespace Interval_NS;

template <typename T, typename Enable = void>
struct Is_Singleton : public Is_Native_Or_Checked<T> {};

template <typename T>
struct Is_Interval : public Is_Same_Or_Derived<Interval_Base, T> {};

//! A generic, not necessarily closed, possibly restricted interval.
/*! \ingroup PPL_CXX_interface
  The class template type parameter \p Boundary represents the type
  of the interval boundaries, and can be chosen, among other possibilities,
  within one of the following number families:

  - a bounded precision native integer type (that is,
    from <CODE>signed char</CODE> to <CODE>long long</CODE>
    and from <CODE>int8_t</CODE> to <CODE>int64_t</CODE>);
  - a bounded precision floating point type (<CODE>float</CODE>,
    <CODE>double</CODE> or <CODE>long double</CODE>);
  - an unbounded integer or rational type, as provided by the C++ interface
    of GMP (<CODE>mpz_class</CODE> or <CODE>mpq_class</CODE>).

  The class template type parameter \p Info allows to control a number
  of features of the class, among which:

  - the ability to support open as well as closed boundaries;
  - the ability to represent empty intervals in addition to nonempty ones;
  - the ability to represent intervals of extended number families
    that contain positive and negative infinities;
*/
template <typename Boundary, typename Info>
class Interval : public Interval_Base, private Info {
private:
  PPL_COMPILE_TIME_CHECK(!Info::store_special
                         || !std::numeric_limits<Boundary>::has_infinity,
                         "store_special is meaningless"
                         " when boundary type may contains infinity");
  Info& w_info() const {
    return const_cast<Interval&>(*this);
  }

public:
  typedef Boundary boundary_type;
  typedef Info info_type;

  typedef Interval_NS::Property Property;

  template <typename T>
  typename Enable_If<Is_Singleton<T>::value || Is_Interval<T>::value, Interval&>::type
  operator=(const T& x) {
    assign(x);
    return *this;
  }

  template <typename T>
  typename Enable_If<Is_Singleton<T>::value || Is_Interval<T>::value, Interval&>::type
  operator+=(const T& x) {
    add_assign(*this, x);
    return *this;
  }
  template <typename T>
  typename Enable_If<Is_Singleton<T>::value || Is_Interval<T>::value, Interval&>::type
  operator-=(const T& x) {
    sub_assign(*this, x);
    return *this;
  }
  template <typename T>
  typename Enable_If<Is_Singleton<T>::value || Is_Interval<T>::value, Interval&>::type
  operator*=(const T& x) {
    mul_assign(*this, x);
    return *this;
  }
  template <typename T>
  typename Enable_If<Is_Singleton<T>::value || Is_Interval<T>::value, Interval&>::type
  operator/=(const T& x) {
    div_assign(*this, x);
    return *this;
  }

  //! Swaps \p *this with \p y.
  void m_swap(Interval& y);

  Info& info() {
    return *this;
  }

  const Info& info() const {
    return *this;
  }

  Boundary& lower() {
    return lower_;
  }

  const Boundary& lower() const {
    return lower_;
  }

  Boundary& upper() {
    return upper_;
  }

  const Boundary& upper() const {
    return upper_;
  }

  I_Constraint<boundary_type> lower_constraint() const {
    PPL_ASSERT(!is_empty());
    if (info().get_boundary_property(LOWER, SPECIAL))
      return I_Constraint<boundary_type>();
    return i_constraint(lower_is_open() ? GREATER_THAN : GREATER_OR_EQUAL,
                        lower(), true);
  }
  I_Constraint<boundary_type> upper_constraint() const {
    PPL_ASSERT(!is_empty());
    if (info().get_boundary_property(UPPER, SPECIAL))
      return I_Constraint<boundary_type>();
    return i_constraint(upper_is_open() ? LESS_THAN : LESS_OR_EQUAL,
                        upper(), true);
  }

  bool is_empty() const {
    return lt(UPPER, upper(), info(), LOWER, lower(), info());
  }

  bool check_empty(I_Result r) const {
    return (r & I_ANY) == I_EMPTY
      || ((r & I_ANY) != I_NOT_EMPTY && is_empty());
  }

  bool is_singleton() const {
    return eq(LOWER, lower(), info(), UPPER, upper(), info());
  }

  bool lower_is_open() const {
    PPL_ASSERT(OK());
    return is_open(LOWER, lower(), info());
  }

  bool upper_is_open() const {
    PPL_ASSERT(OK());
    return is_open(UPPER, upper(), info());
  }

  bool lower_is_boundary_infinity() const {
    PPL_ASSERT(OK());
    return Boundary_NS::is_boundary_infinity(LOWER, lower(), info());
  }

  bool upper_is_boundary_infinity() const {
    PPL_ASSERT(OK());
    return Boundary_NS::is_boundary_infinity(UPPER, upper(), info());
  }

  bool lower_is_domain_inf() const {
    PPL_ASSERT(OK());
    return Boundary_NS::is_domain_inf(LOWER, lower(), info());
  }

  bool upper_is_domain_sup() const {
    PPL_ASSERT(OK());
    return Boundary_NS::is_domain_sup(UPPER, upper(), info());
  }

  bool is_bounded() const {
    PPL_ASSERT(OK());
    return !lower_is_boundary_infinity() && !upper_is_boundary_infinity();
  }

  bool is_universe() const {
    PPL_ASSERT(OK());
    return lower_is_domain_inf() && upper_is_domain_sup();
  }

  I_Result lower_extend() {
    info().clear_boundary_properties(LOWER);
    set_unbounded(LOWER, lower(), info());
    return I_ANY;
  }

  template <typename C>
  typename Enable_If<Is_Same_Or_Derived<I_Constraint_Base, C>::value, I_Result>::type
  lower_extend(const C& c);

  I_Result upper_extend() {
    info().clear_boundary_properties(UPPER);
    set_unbounded(UPPER, upper(), info());
    return I_ANY;
  }

  template <typename C>
  typename Enable_If<Is_Same_Or_Derived<I_Constraint_Base, C>::value, I_Result>::type
  upper_extend(const C& c);

  I_Result build() {
    return assign(UNIVERSE);
  }

  template <typename C>
  typename Enable_If<Is_Same_Or_Derived<I_Constraint_Base, C>::value, I_Result>::type
  build(const C& c) {
    Relation_Symbol rs;
    switch (c.rel()) {
    case V_LGE:
    case V_GT_MINUS_INFINITY:
    case V_LT_PLUS_INFINITY:
      return assign(UNIVERSE);
    default:
      return assign(EMPTY);
    case V_LT:
    case V_LE:
    case V_GT:
    case V_GE:
    case V_EQ:
    case V_NE:
      assign(UNIVERSE);
      rs = static_cast<Relation_Symbol>(c.rel());
      return refine_existential(rs, c.value());
    }
  }

  template <typename C1, typename C2>
  typename Enable_If<Is_Same_Or_Derived<I_Constraint_Base, C1>::value
                     &&
                     Is_Same_Or_Derived<I_Constraint_Base, C2>::value,
                     I_Result>::type
  build(const C1& c1, const C2& c2) {
    switch (c1.rel()) {
    case V_LGE:
      return build(c2);
    case V_NAN:
      return assign(EMPTY);
    default:
      break;
    }
    switch (c2.rel()) {
    case V_LGE:
      return build(c1);
    case V_NAN:
      return assign(EMPTY);
    default:
      break;
    }
    build(c1);
    const I_Result r = add_constraint(c2);
    return r - (I_CHANGED | I_UNCHANGED);
  }

  template <typename C>
  typename Enable_If<Is_Same_Or_Derived<I_Constraint_Base, C>::value, I_Result>::type
  add_constraint(const C& c) {
    Interval x;
    x.build(c);
    return intersect_assign(x);
  }

  I_Result assign(Degenerate_Element e) {
    I_Result r;
    info().clear();
    switch (e) {
    case EMPTY:
      lower_ = 1;
      upper_ = 0;
      r = I_EMPTY | I_EXACT;
      break;
    case UNIVERSE:
      set_unbounded(LOWER, lower(), info());
      set_unbounded(UPPER, upper(), info());
      r = I_UNIVERSE | I_EXACT;
      break;
    default:
      PPL_UNREACHABLE;
      r = I_EMPTY;
      break;
    }
    PPL_ASSERT(OK());
    return r;
  }

  template <typename From>
  typename Enable_If<Is_Special<From>::value, I_Result>::type
  assign(const From&) {
    info().clear();
    Result rl;
    Result ru;
    switch (From::vclass) {
    case VC_MINUS_INFINITY:
      rl = Boundary_NS::set_minus_infinity(LOWER, lower(), info());
      ru = Boundary_NS::set_minus_infinity(UPPER, upper(), info());
      break;
    case VC_PLUS_INFINITY:
      rl = Boundary_NS::set_plus_infinity(LOWER, lower(), info());
      ru = Boundary_NS::set_plus_infinity(UPPER, upper(), info());
      break;
    default:
      PPL_UNREACHABLE;
      rl = V_NAN;
      ru = V_NAN;
      break;
    }
    PPL_ASSERT(OK());
    return combine(rl, ru);
  }

  I_Result set_infinities() {
    info().clear();
    Result rl = Boundary_NS::set_minus_infinity(LOWER, lower(), info());
    Result ru = Boundary_NS::set_plus_infinity(UPPER, upper(), info());
    PPL_ASSERT(OK());
    return combine(rl, ru);
  }

  static bool is_always_topologically_closed() {
    return !Info::store_open;
  }

  bool is_topologically_closed() const {
    PPL_ASSERT(OK());
    return is_always_topologically_closed()
      || is_empty()
      || ((lower_is_boundary_infinity() || !lower_is_open())
          && (upper_is_boundary_infinity() || !upper_is_open()));
  }

  //! Assigns to \p *this its topological closure.
  void topological_closure_assign() {
    if (!Info::store_open || is_empty())
      return;
    if (lower_is_open() && !lower_is_boundary_infinity())
      info().set_boundary_property(LOWER, OPEN, false);
    if (upper_is_open() && !upper_is_boundary_infinity())
      info().set_boundary_property(UPPER, OPEN, false);
  }

  void remove_inf() {
    PPL_ASSERT(!is_empty());
    if (!Info::store_open)
      return;
    info().set_boundary_property(LOWER, OPEN, true);
  }

  void remove_sup() {
    PPL_ASSERT(!is_empty());
    if (!Info::store_open)
      return;
    info().set_boundary_property(UPPER, OPEN, true);
  }

  int infinity_sign() const {
    PPL_ASSERT(OK());
    if (is_reverse_infinity(LOWER, lower(), info()))
      return 1;
    else if (is_reverse_infinity(UPPER, upper(), info()))
      return -1;
    else
      return 0;
  }

  bool contains_integer_point() const {
    PPL_ASSERT(OK());
    if (is_empty())
      return false;
    if (!is_bounded())
      return true;
    Boundary l;
    if (lower_is_open()) {
      add_assign_r(l, lower(), Boundary(1), ROUND_DOWN);
      floor_assign_r(l, l, ROUND_DOWN);
    }
    else
      ceil_assign_r(l, lower(), ROUND_DOWN);
    Boundary u;
    if (upper_is_open()) {
      sub_assign_r(u, upper(), Boundary(1), ROUND_UP);
      ceil_assign_r(u, u, ROUND_UP);
    }
    else
      floor_assign_r(u, upper(), ROUND_UP);
    return u >= l;
  }

  void drop_some_non_integer_points() {
    if (is_empty())
      return;
    if (lower_is_open() && !lower_is_boundary_infinity()) {
      add_assign_r(lower(), lower(), Boundary(1), ROUND_DOWN);
      floor_assign_r(lower(), lower(), ROUND_DOWN);
      info().set_boundary_property(LOWER, OPEN, false);
    }
    else
      ceil_assign_r(lower(), lower(), ROUND_DOWN);
    if (upper_is_open() && !upper_is_boundary_infinity()) {
      sub_assign_r(upper(), upper(), Boundary(1), ROUND_UP);
      ceil_assign_r(upper(), upper(), ROUND_UP);
      info().set_boundary_property(UPPER, OPEN, false);
    }
    else
      floor_assign_r(upper(), upper(), ROUND_UP);
  }

  template <typename From>
  typename Enable_If<Is_Singleton<From>::value || Is_Interval<From>::value, I_Result>::type
  wrap_assign(Bounded_Integer_Type_Width w,
              Bounded_Integer_Type_Representation r,
              const From& refinement) {
    if (is_empty())
      return I_EMPTY;
    if (lower_is_boundary_infinity() || upper_is_boundary_infinity())
      return assign(refinement);
    PPL_DIRTY_TEMP(Boundary, u);
    Result result = sub_2exp_assign_r(u, upper(), w, ROUND_UP);
    if (result_overflow(result) == 0 && u > lower())
      return assign(refinement);
    info().clear();
    switch (r) {
    case UNSIGNED:
      umod_2exp_assign(LOWER, lower(), info(),
                       LOWER, lower(), info(), w);
      umod_2exp_assign(UPPER, upper(), info(),
                       UPPER, upper(), info(), w);
      break;
    case SIGNED_2_COMPLEMENT:
      smod_2exp_assign(LOWER, lower(), info(),
                       LOWER, lower(), info(), w);
      smod_2exp_assign(UPPER, upper(), info(),
                       UPPER, upper(), info(), w);
      break;
    default:
      PPL_UNREACHABLE;
      break;
    }
    if (le(LOWER, lower(), info(), UPPER, upper(), info()))
      return intersect_assign(refinement);
    PPL_DIRTY_TEMP(Interval, tmp);
    tmp.info().clear();
    Boundary_NS::assign(LOWER, tmp.lower(), tmp.info(),
                        LOWER, lower(), info());
    set_unbounded(UPPER, tmp.upper(), tmp.info());
    tmp.intersect_assign(refinement);
    lower_extend();
    intersect_assign(refinement);
    return join_assign(tmp);
  }

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  void ascii_dump(std::ostream& s) const;
  bool ascii_load(std::istream& s);

  bool OK() const {
    if (!Info::may_be_empty && is_empty()) {
#ifndef NDEBUG
      std::cerr << "The interval is unexpectedly empty.\n";
#endif
      return false;
    }

    if (is_open(LOWER, lower(), info())) {
      if (is_plus_infinity(LOWER, lower(), info())) {
#ifndef NDEBUG
        std::cerr << "The lower boundary is +inf open.\n";
#endif
      }
    }
    else if (!Info::may_contain_infinity
             && (is_minus_infinity(LOWER, lower(), info())
                 || is_plus_infinity(LOWER, lower(), info()))) {
#ifndef NDEBUG
      std::cerr << "The lower boundary is unexpectedly infinity.\n";
#endif
      return false;
    }
    if (!info().get_boundary_property(LOWER, SPECIAL)) {
      if (is_not_a_number(lower())) {
#ifndef NDEBUG
        std::cerr << "The lower boundary is not a number.\n";
#endif
        return false;
      }
    }

    if (is_open(UPPER, upper(), info())) {
      if (is_minus_infinity(UPPER, upper(), info())) {
#ifndef NDEBUG
        std::cerr << "The upper boundary is -inf open.\n";
#endif
      }
    }
    else if (!Info::may_contain_infinity
             && (is_minus_infinity(UPPER, upper(), info())
                 || is_plus_infinity(UPPER, upper(), info()))) {
#ifndef NDEBUG
      std::cerr << "The upper boundary is unexpectedly infinity."
                << std::endl;
#endif
      return false;
    }
    if (!info().get_boundary_property(UPPER, SPECIAL)) {
      if (is_not_a_number(upper())) {
#ifndef NDEBUG
        std::cerr << "The upper boundary is not a number.\n";
#endif
        return false;
      }
    }

    // Everything OK.
    return true;
  }

  Interval() {
  }

  template <typename T>
  explicit Interval(const T& x) {
    assign(x);
  }

  /*! \brief
    Builds the smallest interval containing the number whose textual
    representation is contained in \p s.
  */
  explicit Interval(const char* s);

  template <typename T>
  typename Enable_If<Is_Singleton<T>::value
                     || Is_Interval<T>::value, bool>::type
  contains(const T& y) const;

  template <typename T>
  typename Enable_If<Is_Singleton<T>::value
                     || Is_Interval<T>::value, bool>::type
  strictly_contains(const T& y) const;

  template <typename T>
  typename Enable_If<Is_Singleton<T>::value
                     || Is_Interval<T>::value, bool>::type
  is_disjoint_from(const T& y) const;


  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  assign(const From& x);

  template <typename Type>
  typename Enable_If<Is_Singleton<Type>::value
                     || Is_Interval<Type>::value, bool>::type
  can_be_exactly_joined_to(const Type& x) const;

  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  join_assign(const From& x);

  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value
                       || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value
                          || Is_Interval<From2>::value)), I_Result>::type
  join_assign(const From1& x, const From2& y);

  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  intersect_assign(const From& x);

  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value
                       || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value
                          || Is_Interval<From2>::value)), I_Result>::type
  intersect_assign(const From1& x, const From2& y);

  /*! \brief
    Assigns to \p *this the smallest interval containing the set-theoretic
    difference of \p *this and \p x.
  */
  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  difference_assign(const From& x);

  /*! \brief
    Assigns to \p *this the smallest interval containing the set-theoretic
    difference of \p x and \p y.
  */
  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value
                       || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value
                          || Is_Interval<From2>::value)), I_Result>::type
  difference_assign(const From1& x, const From2& y);

  /*! \brief
    Assigns to \p *this the largest interval contained in the set-theoretic
    difference of \p *this and \p x.
  */
  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  lower_approximation_difference_assign(const From& x);

  /*! \brief
    Assigns to \p *this a \ref Meet_Preserving_Simplification
    "meet-preserving simplification" of \p *this with respect to \p y.

    \return
    \c false if and only if the meet of \p *this and \p y is empty.
  */
  template <typename From>
  typename Enable_If<Is_Interval<From>::value, bool>::type
  simplify_using_context_assign(const From& y);

  /*! \brief
    Assigns to \p *this an interval having empty intersection with \p y.
    The assigned interval should be as large as possible.
  */
  template <typename From>
  typename Enable_If<Is_Interval<From>::value, void>::type
  empty_intersection_assign(const From& y);

  /*! \brief
    Refines \p to according to the existential relation \p rel with \p x.

    The \p to interval is restricted to become, upon successful exit,
    the smallest interval of its type that contains the set
    \f[
      \{\,
        a \in \mathtt{to}
      \mid
        \exists b \in \mathtt{x} \st a \mathrel{\mathtt{rel}} b
      \,\}.
    \f]
    \return
    ???
  */
  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  refine_existential(Relation_Symbol rel, const From& x);

  /*! \brief
    Refines \p to so that it satisfies the universal relation \p rel with \p x.

    The \p to interval is restricted to become, upon successful exit,
    the smallest interval of its type that contains the set
    \f[
      \{\,
        a \in \mathtt{to}
      \mid
        \forall b \in \mathtt{x} \itc a \mathrel{\mathtt{rel}} b
      \,\}.
    \f]
    \return
    ???
  */
  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  refine_universal(Relation_Symbol rel, const From& x);

  template <typename From>
  typename Enable_If<Is_Singleton<From>::value
                     || Is_Interval<From>::value, I_Result>::type
  neg_assign(const From& x);

  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value || Is_Interval<From2>::value)), I_Result>::type
  add_assign(const From1& x, const From2& y);

  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value || Is_Interval<From2>::value)), I_Result>::type
  sub_assign(const From1& x, const From2& y);

  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value || Is_Interval<From2>::value)), I_Result>::type
  mul_assign(const From1& x, const From2& y);

  template <typename From1, typename From2>
  typename Enable_If<((Is_Singleton<From1>::value || Is_Interval<From1>::value)
                      && (Is_Singleton<From2>::value || Is_Interval<From2>::value)), I_Result>::type
  div_assign(const From1& x, const From2& y);

  template <typename From, typename Iterator>
  typename Enable_If<Is_Interval<From>::value, void>::type
  CC76_widening_assign(const From& y, Iterator first, Iterator last);

private:
  Boundary lower_;
  Boundary upper_;
};

//! Swaps \p x with \p y.
/*! \relates Interval */
template <typename Boundary, typename Info>
void swap(Interval<Boundary, Info>& x, Interval<Boundary, Info>& y);

} // namespace Parma_Polyhedra_Library

#include "Interval_inlines.hh"
#include "Interval_templates.hh"

#endif // !defined(PPL_Interval_defs_hh)
