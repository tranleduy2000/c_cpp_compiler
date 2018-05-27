/* Helper classes for intervals.
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

#ifndef PPL_intervals_defs_hh
#define PPL_intervals_defs_hh 1

#include "assert.hh"
#include <cstdlib>

#include "Checked_Number.defs.hh"

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The result of an operation on intervals.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
enum I_Result {
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result may be empty.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_EMPTY = 1U,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result may have only one value.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_SINGLETON = 2U,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief \hideinitializer
    Result may have more than one value, but it is not the domain universe.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_SOME = 4U,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result may be the domain universe.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_UNIVERSE = 8U,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result is not empty.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_NOT_EMPTY = I_SINGLETON | I_SOME | I_UNIVERSE,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result may be empty or not empty.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_ANY = I_EMPTY | I_NOT_EMPTY,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result may be empty or not empty.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_NOT_UNIVERSE = I_EMPTY | I_SINGLETON | I_SOME,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result is neither empty nor the domain universe.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_NOT_DEGENERATE = I_SINGLETON | I_SOME,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result is definitely exact.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_EXACT = 16,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Result is definitely inexact.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_INEXACT = 32,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Operation has definitely changed the set.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_CHANGED = 64,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Operation has left the set definitely unchanged.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_UNCHANGED = 128,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! \hideinitializer Operation is undefined for some combination of values.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  I_SINGULARITIES = 256
};

inline I_Result
operator|(I_Result a, I_Result b) {
  return static_cast<I_Result>(static_cast<unsigned>(a)
                               | static_cast<unsigned>(b));
}

inline I_Result
operator&(I_Result a, I_Result b) {
  return static_cast<I_Result>(static_cast<unsigned>(a)
                               & static_cast<unsigned>(b));
}

inline I_Result
operator-(I_Result a, I_Result b) {
    return static_cast<I_Result>(static_cast<unsigned>(a)
                                 & ~static_cast<unsigned>(b));
}

template <typename Criteria, typename T>
struct Use_By_Ref;

struct Use_Slow_Copy;
template <typename T>
struct Use_By_Ref<Use_Slow_Copy, T>
  : public Bool<Slow_Copy<T>::value> {
};

struct By_Value;
template <typename T>
struct Use_By_Ref<By_Value, T>
  : public False {
};

struct By_Ref;
template <typename T>
struct Use_By_Ref<By_Ref, T>
  : public True {
};

template <typename T, typename Criteria = Use_Slow_Copy, typename Enable = void>
class Val_Or_Ref;

template <typename T, typename Criteria>
class Val_Or_Ref<T, Criteria,
		 typename Enable_If<!Use_By_Ref<Criteria, T>::value>::type> {
  T value;
public:
  typedef T Arg_Type;
  typedef T Return_Type;
  Val_Or_Ref()
    : value() {
  }
  explicit Val_Or_Ref(Arg_Type v, bool = false)
    : value(v) {
  }
  Val_Or_Ref& operator=(Arg_Type v) {
    value = v;
    return *this;
  }
  void set(Arg_Type v, bool = false) {
    value = v;
  }
  Return_Type get() const {
    return value;
  }
  operator Return_Type () const {
    return get();
  }
};

template <typename T, typename Criteria>
class Val_Or_Ref<T, Criteria,
		 typename Enable_If<Use_By_Ref<Criteria, T>::value>::type> {
  const T* ptr;
public:
  typedef T& Arg_Type;
  typedef const T& Return_Type;
  Val_Or_Ref()
    : ptr(0) {
  }
  explicit Val_Or_Ref(Arg_Type v)
    : ptr(&v) {
  }
  Val_Or_Ref(const T& v, bool)
    : ptr(&v) {
  }
  Val_Or_Ref& operator=(Arg_Type v) {
    ptr = &v;
    return *this;
  }
  void set(Arg_Type v) {
    ptr = &v;
  }
  void set(const T& v, bool) {
    ptr = &v;
  }
  Return_Type get() const {
    return *ptr;
  }
  operator Return_Type () const {
    return get();
  }
};

class I_Constraint_Base {
};

template <typename Derived>
class I_Constraint_Common : public I_Constraint_Base {
public:
  template <typename T>
  Result convert_real(T& to) const {
    const Derived& c = static_cast<const Derived&>(*this);
    Result r = c.rel();
    switch (r) {
    case V_EMPTY:
    case V_LGE:
      return r;
    case V_LE:
      r = assign_r(to, c.value(), (ROUND_UP | ROUND_STRICT_RELATION));
      r = result_relation_class(r);
      if (r == V_EQ)
	return V_LE;
      goto lt;
    case V_LT:
      r = assign_r(to, c.value(), ROUND_UP);
      r = result_relation_class(r);
    lt:
      switch (r) {
      case V_EMPTY:
      case V_LT_PLUS_INFINITY:
      case V_EQ_MINUS_INFINITY:
	return r;
      case V_LT:
      case V_LE:
      case V_EQ:
	return V_LT;
      default:
	break;
      }
      break;
    case V_GE:
      r = assign_r(to, c.value(), (ROUND_DOWN | ROUND_STRICT_RELATION));
      r = result_relation_class(r);
      if (r == V_EQ)
	return V_GE;
      goto gt;
    case V_GT:
      r = assign_r(to, c.value(), ROUND_DOWN);
      r = result_relation_class(r);
    gt:
      switch (r) {
      case V_EMPTY:
      case V_GT_MINUS_INFINITY:
      case V_EQ_PLUS_INFINITY:
	return r;
      case V_LT:
      case V_LE:
      case V_EQ:
	return V_GT;
      default:
	break;
      }
      break;
    case V_EQ:
      r = assign_r(to, c.value(), ROUND_CHECK);
      r = result_relation_class(r);
      PPL_ASSERT(r != V_LT && r != V_GT);
      if (r == V_EQ)
	return V_EQ;
      else
	return V_EMPTY;
    case V_NE:
      r = assign_r(to, c.value(), ROUND_CHECK);
      r = result_relation_class(r);
      if (r == V_EQ)
	return V_NE;
      else
	return V_LGE;
    default:
      break;
    }
    PPL_UNREACHABLE;
    return V_EMPTY;
  }
  template <typename T>
  Result convert_real(T& to1, Result& rel2, T& to2) const {
    const Derived& c = static_cast<const Derived&>(*this);
    Result rel1;
    if (c.rel() != V_EQ) {
      rel2 = convert(to2);
      return V_LGE;
    }
    rel2 = assign_r(to2, c.value(), ROUND_UP);
    rel2 = result_relation_class(rel2);
    switch (rel2) {
    case V_EMPTY:
    case V_EQ_MINUS_INFINITY:
    case V_EQ:
      return V_LGE;
    default:
      break;
    }
    rel1 = assign_r(to1, c.value(), ROUND_DOWN);
    rel1 = result_relation_class(rel1);
    switch (rel1) {
    case V_EQ:
      PPL_ASSERT(rel2 == V_LE);
      goto eq;
    case V_EQ_PLUS_INFINITY:
    case V_EMPTY:
      rel2 = rel1;
      return V_LGE;
    case V_GE:
      if (rel2 == V_LE && to1 == to2) {
      eq:
	rel2 = V_EQ;
	return V_LGE;
      }
      /* Fall through*/
    case V_GT:
    case V_GT_MINUS_INFINITY:
      return rel1;
    default:
      PPL_UNREACHABLE;
      return V_EMPTY;
    }
    switch (rel2) {
    case V_LE:
    case V_LT:
    case V_LT_PLUS_INFINITY:
      return rel1;
    default:
      PPL_UNREACHABLE;
      return V_EMPTY;
    }
  }
  template <typename T>
  Result convert_integer(T& to) const {
    Result rel = convert_real(to);
    switch (rel) {
    case V_LT:
      if (is_integer(to)) {
	rel = sub_assign_r(to, to, T(1), (ROUND_UP | ROUND_STRICT_RELATION));
	rel = result_relation_class(rel);
	return (rel == V_EQ) ? V_LE : rel;
      }
      /* Fall through */
    case V_LE:
      rel = floor_assign_r(to, to, ROUND_UP);
      rel = result_relation_class(rel);
      PPL_ASSERT(rel == V_EQ);
      return V_LE;
    case V_GT:
      if (is_integer(to)) {
	rel = add_assign_r(to, to, T(1), (ROUND_DOWN | ROUND_STRICT_RELATION));
	rel = result_relation_class(rel);
	return (rel == V_EQ) ? V_GE : rel;
      }
      /* Fall through */
    case V_GE:
      rel = ceil_assign_r(to, to, ROUND_DOWN);
      rel = result_relation_class(rel);
      PPL_ASSERT(rel == V_EQ);
      return V_GE;
    case V_EQ:
      if (is_integer(to))
	return V_EQ;
      return V_EMPTY;
    case V_NE:
      if (is_integer(to))
	return V_NE;
      return V_LGE;
    default:
      return rel;
    }
  }
};

struct I_Constraint_Rel {
  Result rel;
  I_Constraint_Rel(Result r)
    : rel(r) {
    PPL_ASSERT(result_relation_class(r) == r);
  }
  I_Constraint_Rel(Relation_Symbol r)
    : rel(static_cast<Result>(r)) {
  }
  operator Result() const {
    return rel;
  }
};

template <typename T, typename Val_Or_Ref_Criteria = Use_Slow_Copy,
	  bool extended = false>
class I_Constraint
  : public I_Constraint_Common<I_Constraint<T, Val_Or_Ref_Criteria,
                                            extended> > {
  typedef Val_Or_Ref<T, Val_Or_Ref_Criteria> Val_Ref;
  typedef typename Val_Ref::Arg_Type Arg_Type;
  typedef typename Val_Ref::Return_Type Return_Type;
  Result rel_;
  Val_Ref value_;
public:
  typedef T value_type;
  explicit I_Constraint()
    : rel_(V_LGE) {
  }
  I_Constraint(I_Constraint_Rel r, Arg_Type v)
    : rel_(r), value_(v) {
  }
  I_Constraint(I_Constraint_Rel r, const T& v, bool force)
    : rel_(r), value_(v, force) {
  }
  template <typename U>
  I_Constraint(I_Constraint_Rel r, const U& v)
    : rel_(r), value_(v) {
  }
  void set(I_Constraint_Rel r, Arg_Type v) {
    rel_ =  r;
    value_.set(v);
  }
  void set(I_Constraint_Rel r, const T& v, bool force) {
    rel_ =  r;
    value_.set(v, force);
  }
  template <typename U>
  void set(I_Constraint_Rel r, const U& v) {
    rel_ = r;
    value_.set(v);
  }
  Return_Type value() const {
    return value_;
  }
  Result rel() const {
    return rel_;
  }
};

template <typename T>
inline I_Constraint<T>
i_constraint(I_Constraint_Rel rel, const T& v) {
  return I_Constraint<T>(rel, v);
}

template <typename T>
inline I_Constraint<T>
i_constraint(I_Constraint_Rel rel, const T& v, bool force) {
  return I_Constraint<T>(rel, v, force);
}

template <typename T>
inline I_Constraint<T>
i_constraint(I_Constraint_Rel rel, T& v) {
  return I_Constraint<T>(rel, v);
}

template <typename T, typename Val_Or_Ref_Criteria>
inline I_Constraint<T, Val_Or_Ref_Criteria>
i_constraint(I_Constraint_Rel rel, const T& v, const Val_Or_Ref_Criteria&) {
  return I_Constraint<T, Val_Or_Ref_Criteria>(rel, v);
}

template <typename T, typename Val_Or_Ref_Criteria>
inline I_Constraint<T, Val_Or_Ref_Criteria>
i_constraint(I_Constraint_Rel rel, const T& v, bool force,
             const Val_Or_Ref_Criteria&) {
  return I_Constraint<T, Val_Or_Ref_Criteria>(rel, v, force);
}

template <typename T, typename Val_Or_Ref_Criteria>
inline I_Constraint<T, Val_Or_Ref_Criteria>
i_constraint(I_Constraint_Rel rel, T& v, const Val_Or_Ref_Criteria&) {
  return I_Constraint<T, Val_Or_Ref_Criteria>(rel, v);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_intervals_defs_hh)
