/* Inline functions for the Interval class and its constituents.
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

#ifndef PPL_Interval_inlines_hh
#define PPL_Interval_inlines_hh 1

namespace Parma_Polyhedra_Library {

template <typename Boundary, typename Info>
inline memory_size_type
Interval<Boundary, Info>::external_memory_in_bytes() const {
  return Parma_Polyhedra_Library::external_memory_in_bytes(lower())
    + Parma_Polyhedra_Library::external_memory_in_bytes(upper());
}

template <typename Boundary, typename Info>
inline memory_size_type
Interval<Boundary, Info>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename Boundary, typename Info>
inline void
Interval<Boundary, Info>::m_swap(Interval<Boundary, Info>& y) {
  using std::swap;
  swap(lower(), y.lower());
  swap(upper(), y.upper());
  swap(info(), y.info());
}

template <typename Boundary, typename Info>
inline bool
f_is_empty(const Interval<Boundary, Info>& x) {
  return x.is_empty();
}
template <typename Boundary, typename Info>
inline bool
f_is_singleton(const Interval<Boundary, Info>& x) {
  return x.is_singleton();
}
template <typename Boundary, typename Info>
inline int
infinity_sign(const Interval<Boundary, Info>& x) {
  return x.infinity_sign();
}

namespace Interval_NS {

template <typename Boundary, typename Info>
inline const Boundary&
f_lower(const Interval<Boundary, Info>& x) {
  return x.lower();
}
template <typename Boundary, typename Info>
inline const Boundary&
f_upper(const Interval<Boundary, Info>& x) {
  return x.upper();
}
template <typename Boundary, typename Info>
inline const Info&
f_info(const Interval<Boundary, Info>& x) {
  return x.info();
}

struct Scalar_As_Interval_Policy {
  const_bool_nodef(may_be_empty, true);
  const_bool_nodef(may_contain_infinity, true);
  const_bool_nodef(check_inexact, false);
};

typedef Interval_Info_Null<Scalar_As_Interval_Policy>
Scalar_As_Interval_Info;

const Scalar_As_Interval_Info SCALAR_INFO;

typedef Interval_Info_Null_Open<Scalar_As_Interval_Policy>
Scalar_As_Interval_Info_Open;

template <typename T>
inline typename Enable_If<Is_Singleton<T>::value, const T&>::type
f_lower(const T& x) {
  return x;
}
template <typename T>
inline typename Enable_If<Is_Singleton<T>::value, const T&>::type
f_upper(const T& x) {
  return x;
}
template <typename T>
inline typename Enable_If<Is_Singleton<T>::value,
                          const Scalar_As_Interval_Info&>::type
f_info(const T&) {
  return SCALAR_INFO;
}
template <typename T>
inline typename Enable_If<Is_Singleton<T>::value,
                          Scalar_As_Interval_Info_Open>::type
f_info(const T&, bool open) {
  return Scalar_As_Interval_Info_Open(open);
}

template <typename T>
inline typename Enable_If<Is_Singleton<T>::value, bool>::type
f_is_empty(const T& x) {
  return is_not_a_number(x);
}

template <typename T>
inline typename Enable_If<Is_Singleton<T>::value, bool>::type
f_is_singleton(const T& x) {
  return !f_is_empty(x);
}

} // namespace Interval_NS

template <typename T>
inline typename Enable_If<Is_Singleton<T>::value
                          || Is_Interval<T>::value, bool>::type
is_singleton_integer(const T& x) {
  return is_singleton(x) && is_integer(f_lower(x));
}

template <typename T>
inline typename Enable_If<Is_Singleton<T>::value
                          || Is_Interval<T>::value, bool>::type
check_empty_arg(const T& x) {
  if (f_info(x).may_be_empty)
    return f_is_empty(x);
  else {
    PPL_ASSERT(!f_is_empty(x));
    return false;
  }
}

template <typename T1, typename T2>
inline typename Enable_If<((Is_Singleton<T1>::value
                            || Is_Interval<T1>::value)
                           && (Is_Singleton<T2>::value
                               || Is_Interval<T2>::value)
                           && (Is_Interval<T1>::value
                               || Is_Interval<T2>::value)),
                          bool>::type
operator==(const T1& x, const T2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(x))
    return check_empty_arg(y);
  else if (check_empty_arg(y))
    return false;
  return eq(LOWER, f_lower(x), f_info(x), LOWER, f_lower(y), f_info(y))
    && eq(UPPER, f_upper(x), f_info(x), UPPER, f_upper(y), f_info(y));
}

template <typename T1, typename T2>
inline typename Enable_If<((Is_Singleton<T1>::value
                            || Is_Interval<T1>::value)
                           && (Is_Singleton<T2>::value
                               || Is_Interval<T2>::value)
                           && (Is_Interval<T1>::value
                               || Is_Interval<T2>::value)),
                          bool>::type
operator!=(const T1& x, const T2& y) {
  return !(x == y);
}

template <typename Boundary, typename Info>
template <typename T>
inline typename Enable_If<Is_Singleton<T>::value
                          || Is_Interval<T>::value, bool>::type
Interval<Boundary, Info>::contains(const T& y) const {
  PPL_ASSERT(OK());
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(y))
    return true;
  if (check_empty_arg(*this))
    return false;
  return le(LOWER, lower(), info(), LOWER, f_lower(y), f_info(y))
    && ge(UPPER, upper(), info(), UPPER, f_upper(y), f_info(y));
}

template <typename Boundary, typename Info>
template <typename T>
inline typename Enable_If<Is_Singleton<T>::value
                          || Is_Interval<T>::value, bool>::type
Interval<Boundary, Info>::strictly_contains(const T& y) const {
  PPL_ASSERT(OK());
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(y))
    return !check_empty_arg(*this);
  if (check_empty_arg(*this))
    return false;
  return (lt(LOWER, lower(), info(), LOWER, f_lower(y), f_info(y))
          && ge(UPPER, upper(), info(), UPPER, f_upper(y), f_info(y)))
    || (le(LOWER, lower(), info(), LOWER, f_lower(y), f_info(y))
        && gt(UPPER, upper(), info(), UPPER, f_upper(y), f_info(y)));
}

template <typename Boundary, typename Info>
template <typename T>
inline typename Enable_If<Is_Singleton<T>::value
                          || Is_Interval<T>::value, bool>::type
Interval<Boundary, Info>::is_disjoint_from(const T& y) const {
  PPL_ASSERT(OK());
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(*this) || check_empty_arg(y))
    return true;
  return gt(LOWER, lower(), info(), UPPER, f_upper(y), f_info(y))
    || lt(UPPER, upper(), info(), LOWER, f_lower(y), f_info(y));
}

template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>::assign(const From& x) {
  PPL_ASSERT(f_OK(x));
  if (check_empty_arg(x))
    return assign(EMPTY);
  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  const Result rl = Boundary_NS::assign(LOWER, lower(), to_info,
                                        LOWER, f_lower(x), f_info(x));
  const Result ru = Boundary_NS::assign(UPPER, upper(), to_info,
                                        UPPER, f_upper(x), f_info(x));
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>::join_assign(const From& x) {
  PPL_ASSERT(f_OK(x));
  if (check_empty_arg(*this))
    return assign(x);
  if (check_empty_arg(x))
    return combine(V_EQ, V_EQ);
  Result rl;
  Result ru;
  rl = min_assign(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x));
  ru = max_assign(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x));
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::join_assign(const From1& x, const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(x))
    return assign(y);
  if (check_empty_arg(y))
    return assign(x);
  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  Result rl;
  Result ru;
  rl = min_assign(LOWER, lower(), to_info,
                  LOWER, f_lower(x), f_info(x),
                  LOWER, f_lower(y), f_info(y));
  ru = max_assign(UPPER, upper(), to_info,
                  UPPER, f_upper(x), f_info(x),
                  UPPER, f_upper(y), f_info(y));
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename Boundary, typename Info>
template <typename Type>
inline typename Enable_If<Is_Singleton<Type>::value
                          || Is_Interval<Type>::value, bool>::type
Interval<Boundary, Info>::can_be_exactly_joined_to(const Type& x) const {
  PPL_DIRTY_TEMP(Boundary, b);
  if (gt(LOWER, lower(), info(), UPPER, f_upper(x), f_info(x))) {
    b = lower();
    return eq(LOWER, b, info(), UPPER, f_upper(x), f_info(x));
  }
  else if (lt(UPPER, upper(), info(), LOWER, f_lower(x), f_info(x))) {
    b = upper();
    return eq(UPPER, b, info(), LOWER, f_lower(x), f_info(x));
  }
  return true;
}


template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>::intersect_assign(const From& x) {
  PPL_ASSERT(f_OK(x));
  max_assign(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x));
  min_assign(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x));
  PPL_ASSERT(OK());
  return I_ANY;
}

template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::intersect_assign(const From1& x,
                                                 const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  max_assign(LOWER, lower(), to_info,
             LOWER, f_lower(x), f_info(x),
             LOWER, f_lower(y), f_info(y));
  min_assign(UPPER, upper(), to_info,
             UPPER, f_upper(x), f_info(x),
             UPPER, f_upper(y), f_info(y));
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return I_NOT_EMPTY;
}

template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>::difference_assign(const From& x) {
  PPL_ASSERT(f_OK(x));
  if (lt(UPPER, upper(), info(), LOWER, f_lower(x), f_info(x))
      || gt(LOWER, lower(), info(), UPPER, f_upper(x), f_info(x)))
    return combine(V_EQ, V_EQ);
  bool nl = ge(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x));
  bool nu = le(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x));
  Result rl = V_EQ;
  Result ru = V_EQ;
  if (nl) {
    if (nu)
      return assign(EMPTY);
    else {
      info().clear_boundary_properties(LOWER);
      rl = complement(LOWER, lower(), info(), UPPER, f_upper(x), f_info(x));
    }
  }
  else if (nu) {
    info().clear_boundary_properties(UPPER);
    ru = complement(UPPER, upper(), info(), LOWER, f_lower(x), f_info(x));
  }
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::difference_assign(const From1& x,
                                                  const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  if (lt(UPPER, f_upper(x), f_info(x), LOWER, f_lower(y), f_info(y))
      || gt(LOWER, f_lower(x), f_info(x), UPPER, f_upper(y), f_info(y)))
    return assign(x);
  bool nl = ge(LOWER, f_lower(x), f_info(x), LOWER, f_lower(y), f_info(y));
  bool nu = le(UPPER, f_upper(x), f_info(x), UPPER, f_upper(y), f_info(y));
  Result rl = V_EQ;
  Result ru = V_EQ;
  if (nl) {
    if (nu)
      return assign(EMPTY);
    else {
      rl = complement(LOWER, lower(), info(), UPPER, f_upper(y), f_info(y));
      ru = Boundary_NS::assign(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x));
    }
  }
  else if (nu) {
    ru = complement(UPPER, upper(), info(), LOWER, f_lower(y), f_info(y));
    rl = Boundary_NS::assign(LOWER, lower(), info(),
                             LOWER, f_lower(x), f_info(x));
  }
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>
::refine_existential(Relation_Symbol rel, const From& x) {
  PPL_ASSERT(OK());
  PPL_ASSERT(f_OK(x));
  if (check_empty_arg(x))
    return assign(EMPTY);
  switch (rel) {
  case LESS_THAN:
    {
      if (lt(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(UPPER);
      Boundary_NS::assign(UPPER, upper(), info(),
                          UPPER, f_upper(x), f_info(x), true);
      return I_ANY;
    }
  case LESS_OR_EQUAL:
    {
      if (le(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(UPPER);
      Boundary_NS::assign(UPPER, upper(), info(),
                          UPPER, f_upper(x), f_info(x));
      return I_ANY;
    }
  case GREATER_THAN:
    {
      if (gt(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(LOWER);
      Boundary_NS::assign(LOWER, lower(), info(),
                          LOWER, f_lower(x), f_info(x), true);
      return I_ANY;
    }
  case GREATER_OR_EQUAL:
    {
      if (ge(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(LOWER);
      Boundary_NS::assign(LOWER, lower(), info(),
                          LOWER, f_lower(x), f_info(x));
      return I_ANY;
    }
  case EQUAL:
    return intersect_assign(x);
  case NOT_EQUAL:
    {
      if (!f_is_singleton(x))
        return combine(V_EQ, V_EQ);
      if (check_empty_arg(*this))
        return I_EMPTY;
      if (eq(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x)))
        remove_inf();
      if (eq(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x)))
        remove_sup();
      return I_ANY;
    }
  default:
    PPL_UNREACHABLE;
    return I_EMPTY;
  }
}

template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>::refine_universal(Relation_Symbol rel,
                                                 const From& x) {
  PPL_ASSERT(OK());
  PPL_ASSERT(f_OK(x));
  if (check_empty_arg(x))
    return combine(V_EQ, V_EQ);
  switch (rel) {
  case LESS_THAN:
    {
      if (lt(UPPER, upper(), info(), LOWER, f_lower(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(UPPER);
      Result ru = Boundary_NS::assign(UPPER, upper(), info(),
                                      LOWER, f_lower(x), SCALAR_INFO,
                                      !is_open(LOWER, f_lower(x), f_info(x)));
      PPL_USED(ru);
      return I_ANY;
    }
  case LESS_OR_EQUAL:
    {
      if (le(UPPER, upper(), info(), LOWER, f_lower(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(UPPER);
      Result ru = Boundary_NS::assign(UPPER, upper(), info(),
                                      LOWER, f_lower(x), SCALAR_INFO);
      PPL_USED(ru);
      return I_ANY;
    }
  case GREATER_THAN:
    {
      if (gt(LOWER, lower(), info(), UPPER, f_upper(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(LOWER);
      Result rl = Boundary_NS::assign(LOWER, lower(), info(),
                                      UPPER, f_upper(x), SCALAR_INFO,
                                      !is_open(UPPER, f_upper(x), f_info(x)));
      PPL_USED(rl);
      return I_ANY;
    }
  case GREATER_OR_EQUAL:
    {
      if (ge(LOWER, lower(), info(), UPPER, f_upper(x), f_info(x)))
        return combine(V_EQ, V_EQ);
      info().clear_boundary_properties(LOWER);
      Result rl = Boundary_NS::assign(LOWER, lower(), info(),
                                      UPPER, f_upper(x), SCALAR_INFO);
      PPL_USED(rl);
      return I_ANY;
    }
  case EQUAL:
    if (!f_is_singleton(x))
      return assign(EMPTY);
    return intersect_assign(x);
  case NOT_EQUAL:
    {
      if (check_empty_arg(*this))
        return I_EMPTY;
      if (eq(LOWER, lower(), info(), LOWER, f_lower(x), f_info(x)))
        remove_inf();
      if (eq(UPPER, upper(), info(), UPPER, f_upper(x), f_info(x)))
        remove_sup();
      return I_ANY;
    }
  default:
    PPL_UNREACHABLE;
    return I_EMPTY;
  }
}

template <typename To_Boundary, typename To_Info>
template <typename From>
inline typename Enable_If<Is_Singleton<From>::value
                          || Is_Interval<From>::value, I_Result>::type
Interval<To_Boundary, To_Info>::neg_assign(const From& x) {
  PPL_ASSERT(f_OK(x));
  if (check_empty_arg(x))
    return assign(EMPTY);
  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  Result rl;
  Result ru;
  PPL_DIRTY_TEMP(To_Boundary, to_lower);
  rl = Boundary_NS::neg_assign(LOWER, to_lower, to_info, UPPER, f_upper(x), f_info(x));
  ru = Boundary_NS::neg_assign(UPPER, upper(), to_info, LOWER, f_lower(x), f_info(x));
  assign_or_swap(lower(), to_lower);
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::add_assign(const From1& x, const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(x) || check_empty_arg(y))
    return assign(EMPTY);
  int inf_sign = Parma_Polyhedra_Library::infinity_sign(x);
  if (inf_sign != 0) {
    if (Parma_Polyhedra_Library::infinity_sign(y) == -inf_sign)
      return assign(EMPTY);
  }
  else
    inf_sign = Parma_Polyhedra_Library::infinity_sign(y);
  if (inf_sign < 0)
    return assign(MINUS_INFINITY);
  else if (inf_sign > 0)
    return assign(PLUS_INFINITY);
  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  Result rl = Boundary_NS::add_assign(LOWER, lower(), to_info,
                                      LOWER, f_lower(x), f_info(x),
                                      LOWER, f_lower(y), f_info(y));
  Result ru = Boundary_NS::add_assign(UPPER, upper(), to_info,
                                      UPPER, f_upper(x), f_info(x),
                                      UPPER, f_upper(y), f_info(y));
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::sub_assign(const From1& x, const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(x) || check_empty_arg(y))
    return assign(EMPTY);
  int inf_sign = Parma_Polyhedra_Library::infinity_sign(x);
  if (inf_sign != 0) {
    if (Parma_Polyhedra_Library::infinity_sign(y) == inf_sign)
      return assign(EMPTY);
  }
  else
    inf_sign = -Parma_Polyhedra_Library::infinity_sign(y);
  if (inf_sign < 0)
    return assign(MINUS_INFINITY);
  else if (inf_sign > 0)
    return assign(PLUS_INFINITY);

  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  Result rl;
  Result ru;
  PPL_DIRTY_TEMP(To_Boundary, to_lower);
  rl = Boundary_NS::sub_assign(LOWER, to_lower, to_info,
                               LOWER, f_lower(x), f_info(x),
                               UPPER, f_upper(y), f_info(y));
  ru = Boundary_NS::sub_assign(UPPER, upper(), to_info,
                               UPPER, f_upper(x), f_info(x),
                               LOWER, f_lower(y), f_info(y));
  assign_or_swap(lower(), to_lower);
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

/**
+---------+-----------+-----------+-----------------+
|    *    |  yl > 0   |  yu < 0   |  yl < 0, yu > 0 |
+---------+-----------+-----------+-----------------+
| xl > 0  |xl*yl,xu*yu|xu*yl,xl*yu|   xu*yl,xu*yu   |
+---------+-----------+-----------+-----------------+
| xu < 0  |xl*yu,xu*yl|xu*yu,xl*yl|   xl*yu,xl*yl   |
+---------+-----------+-----------+-----------------+
|xl<0 xu>0|xl*yu,xu*yu|xu*yl,xl*yl|min(xl*yu,xu*yl),|
|         |           |           |max(xl*yl,xu*yu) |
+---------+-----------+-----------+-----------------+
**/
template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::mul_assign(const From1& x, const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(x) || check_empty_arg(y))
    return assign(EMPTY);
  int xls = sgn_b(LOWER, f_lower(x), f_info(x));
  int xus = (xls > 0) ? 1 : sgn_b(UPPER, f_upper(x), f_info(x));
  int yls = sgn_b(LOWER, f_lower(y), f_info(y));
  int yus = (yls > 0) ? 1 : sgn_b(UPPER, f_upper(y), f_info(y));
  int inf_sign = Parma_Polyhedra_Library::infinity_sign(x);
  int ls;
  int us;
  if (inf_sign != 0) {
    ls = yls;
    us = yus;
    goto inf;
  }
  else {
    inf_sign = Parma_Polyhedra_Library::infinity_sign(y);
    if (inf_sign != 0) {
      ls = xls;
      us = xus;
    inf:
      if (ls == 0 && us == 0)
        return assign(EMPTY);
      if (ls == -us)
        return set_infinities();
      if (ls < 0 || us < 0)
        inf_sign = -inf_sign;
      if (inf_sign < 0)
        return assign(MINUS_INFINITY);
      else
        return assign(PLUS_INFINITY);
    }
  }

  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  Result rl;
  Result ru;
  PPL_DIRTY_TEMP(To_Boundary, to_lower);

  if (xls >= 0) {
    if (yls >= 0) {
      // 0 <= xl <= xu, 0 <= yl <= yu
      rl = mul_assign_z(LOWER, to_lower, to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        LOWER, f_lower(y), f_info(y), yls);
      ru = mul_assign_z(UPPER, upper(), to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        UPPER, f_upper(y), f_info(y), yus);
    }
    else if (yus <= 0) {
      // 0 <= xl <= xu, yl <= yu <= 0
      rl = mul_assign_z(LOWER, to_lower, to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        LOWER, f_lower(y), f_info(y), yls);
      ru = mul_assign_z(UPPER, upper(), to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        UPPER, f_upper(y), f_info(y), yus);
    }
    else {
      // 0 <= xl <= xu, yl < 0 < yu
      rl = mul_assign_z(LOWER, to_lower, to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        LOWER, f_lower(y), f_info(y), yls);
      ru = mul_assign_z(UPPER, upper(), to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        UPPER, f_upper(y), f_info(y), yus);
    }
  }
  else if (xus <= 0) {
    if (yls >= 0) {
      // xl <= xu <= 0, 0 <= yl <= yu
      rl = mul_assign_z(LOWER, to_lower, to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        UPPER, f_upper(y), f_info(y), yus);
      ru = mul_assign_z(UPPER, upper(), to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        LOWER, f_lower(y), f_info(y), yls);
    }
    else if (yus <= 0) {
      // xl <= xu <= 0, yl <= yu <= 0
      rl = mul_assign_z(LOWER, to_lower, to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        UPPER, f_upper(y), f_info(y), yus);
      ru = mul_assign_z(UPPER, upper(), to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        LOWER, f_lower(y), f_info(y), yls);
    }
    else {
      // xl <= xu <= 0, yl < 0 < yu
      rl = mul_assign_z(LOWER, to_lower, to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        UPPER, f_upper(y), f_info(y), yus);
      ru = mul_assign_z(UPPER, upper(), to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        LOWER, f_lower(y), f_info(y), yls);
    }
  }
  else if (yls >= 0) {
    // xl < 0 < xu, 0 <= yl <= yu
    rl = mul_assign_z(LOWER, to_lower, to_info,
                      LOWER, f_lower(x), f_info(x), xls,
                      UPPER, f_upper(y), f_info(y), yus);
    ru = mul_assign_z(UPPER, upper(), to_info,
                      UPPER, f_upper(x), f_info(x), xus,
                      UPPER, f_upper(y), f_info(y), yus);
  }
  else if (yus <= 0) {
    // xl < 0 < xu, yl <= yu <= 0
    rl = mul_assign_z(LOWER, to_lower, to_info,
                      UPPER, f_upper(x), f_info(x), xus,
                      LOWER, f_lower(y), f_info(y), yls);
    ru = mul_assign_z(UPPER, upper(), to_info,
                      LOWER, f_lower(x), f_info(x), xls,
                      LOWER, f_lower(y), f_info(y), yls);
  }
  else {
    // xl < 0 < xu, yl < 0 < yu
    PPL_DIRTY_TEMP(To_Boundary, tmp);
    PPL_DIRTY_TEMP(To_Info, tmp_info);
    tmp_info.clear();
    Result tmp_r;
    tmp_r = Boundary_NS::mul_assign(LOWER, tmp, tmp_info,
                                    UPPER, f_upper(x), f_info(x),
                                    LOWER, f_lower(y), f_info(y));
    rl = Boundary_NS::mul_assign(LOWER, to_lower, to_info,
                                 LOWER, f_lower(x), f_info(x),
                                 UPPER, f_upper(y), f_info(y));
    if (gt(LOWER, to_lower, to_info, LOWER, tmp, tmp_info)) {
      to_lower = tmp;
      rl = tmp_r;
    }
    tmp_info.clear();
    tmp_r = Boundary_NS::mul_assign(UPPER, tmp, tmp_info,
                                    UPPER, f_upper(x), f_info(x),
                                    UPPER, f_upper(y), f_info(y));
    ru = Boundary_NS::mul_assign(UPPER, upper(), to_info,
                                 LOWER, f_lower(x), f_info(x),
                                 LOWER, f_lower(y), f_info(y));
    if (lt(UPPER, upper(), to_info, UPPER, tmp, tmp_info)) {
      upper() = tmp;
      ru = tmp_r;
    }
  }
  assign_or_swap(lower(), to_lower);
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

/**
+-----------+-----------+-----------+
|     /     |  yu < 0   |  yl > 0   |
+-----------+-----------+-----------+
|   xu<=0   |xu/yl,xl/yu|xl/yl,xu/yu|
+-----------+-----------+-----------+
|xl<=0 xu>=0|xu/yu,xl/yu|xl/yl,xu/yl|
+-----------+-----------+-----------+
|   xl>=0   |xu/yu,xl/yl|xl/yu,xu/yl|
+-----------+-----------+-----------+
**/
template <typename To_Boundary, typename To_Info>
template <typename From1, typename From2>
inline typename Enable_If<((Is_Singleton<From1>::value
                            || Is_Interval<From1>::value)
                           && (Is_Singleton<From2>::value
                               || Is_Interval<From2>::value)), I_Result>::type
Interval<To_Boundary, To_Info>::div_assign(const From1& x, const From2& y) {
  PPL_ASSERT(f_OK(x));
  PPL_ASSERT(f_OK(y));
  if (check_empty_arg(x) || check_empty_arg(y))
    return assign(EMPTY);
  int yls = sgn_b(LOWER, f_lower(y), f_info(y));
  int yus = (yls > 0) ? 1 : sgn_b(UPPER, f_upper(y), f_info(y));
  if (yls == 0 && yus == 0)
    return assign(EMPTY);
  int inf_sign = Parma_Polyhedra_Library::infinity_sign(x);
  if (inf_sign != 0) {
    if (Parma_Polyhedra_Library::infinity_sign(y) != 0)
      return assign(EMPTY);
    if (yls == -yus)
      return set_infinities();
    if (yls < 0 || yus < 0)
    inf_sign = -inf_sign;
    if (inf_sign < 0)
      return assign(MINUS_INFINITY);
    else
      return assign(PLUS_INFINITY);
  }
  int xls = sgn_b(LOWER, f_lower(x), f_info(x));
  int xus = (xls > 0) ? 1 : sgn_b(UPPER, f_upper(x), f_info(x));

  PPL_DIRTY_TEMP(To_Info, to_info);
  to_info.clear();
  Result rl;
  Result ru;
  PPL_DIRTY_TEMP(To_Boundary, to_lower);
  if (yls >= 0) {
    if (xls >= 0) {
      rl = div_assign_z(LOWER, to_lower, to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        UPPER, f_upper(y), f_info(y), yus);
      ru = div_assign_z(UPPER, upper(), to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        LOWER, f_lower(y), f_info(y), yls);
    }
    else if (xus <= 0) {
      rl = div_assign_z(LOWER, to_lower, to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        LOWER, f_lower(y), f_info(y), yls);
      ru = div_assign_z(UPPER, upper(), to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        UPPER, f_upper(y), f_info(y), yus);
    }
    else {
      rl = div_assign_z(LOWER, to_lower, to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        LOWER, f_lower(y), f_info(y), yls);
      ru = div_assign_z(UPPER, upper(), to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        LOWER, f_lower(y), f_info(y), yls);
    }
  }
  else if (yus <= 0) {
    if (xls >= 0) {
      rl = div_assign_z(LOWER, to_lower, to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        UPPER, f_upper(y), f_info(y), yus);
      ru = div_assign_z(UPPER, upper(), to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        LOWER, f_lower(y), f_info(y), yls);
    }
    else if (xus <= 0) {
      rl = div_assign_z(LOWER, to_lower, to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        LOWER, f_lower(y), f_info(y), yls);
      ru = div_assign_z(UPPER, upper(), to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        UPPER, f_upper(y), f_info(y), yus);
    }
    else {
      rl = div_assign_z(LOWER, to_lower, to_info,
                        UPPER, f_upper(x), f_info(x), xus,
                        UPPER, f_upper(y), f_info(y), yus);
      ru = div_assign_z(UPPER, upper(), to_info,
                        LOWER, f_lower(x), f_info(x), xls,
                        UPPER, f_upper(y), f_info(y), yus);
    }
  }
  else {
    return static_cast<I_Result>(assign(UNIVERSE) | I_SINGULARITIES);
  }
  assign_or_swap(lower(), to_lower);
  assign_or_swap(info(), to_info);
  PPL_ASSERT(OK());
  return combine(rl, ru);
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator+(const Interval<B, Info>& x, const T& y) {
  Interval<B, Info> z;
  z.add_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator+(const T& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.add_assign(x, y);
  return z;
}

template <typename B, typename Info>
inline Interval<B, Info>
operator+(const Interval<B, Info>& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.add_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator-(const Interval<B, Info>& x, const T& y) {
  Interval<B, Info> z;
  z.sub_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator-(const T& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.sub_assign(x, y);
  return z;
}

template <typename B, typename Info>
inline Interval<B, Info>
operator-(const Interval<B, Info>& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.sub_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator*(const Interval<B, Info>& x, const T& y) {
  Interval<B, Info> z;
  z.mul_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator*(const T& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.mul_assign(x, y);
  return z;
}

template <typename B, typename Info>
inline Interval<B, Info>
operator*(const Interval<B, Info>& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.mul_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator/(const Interval<B, Info>& x, const T& y) {
  Interval<B, Info> z;
  z.div_assign(x, y);
  return z;
}

template <typename B, typename Info, typename T>
inline typename Enable_If<Is_Singleton<T>::value, Interval<B, Info> >::type
operator/(const T& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.div_assign(x, y);
  return z;
}

template <typename B, typename Info>
inline Interval<B, Info>
operator/(const Interval<B, Info>& x, const Interval<B, Info>& y) {
  Interval<B, Info> z;
  z.div_assign(x, y);
  return z;
}

template <typename Boundary, typename Info>
inline std::ostream&
operator<<(std::ostream& os, const Interval<Boundary, Info>& x) {
  if (check_empty_arg(x))
    return os << "[]";
  if (x.is_singleton()) {
    output(os, x.lower(), Numeric_Format(), ROUND_NOT_NEEDED);
    return os;
  }
  os << (x.lower_is_open() ? "(" : "[");
  if (x.info().get_boundary_property(LOWER, SPECIAL))
    os << "-inf";
  else
    output(os, x.lower(), Numeric_Format(), ROUND_NOT_NEEDED);
  os << ", ";
  if (x.info().get_boundary_property(UPPER, SPECIAL))
    os << "+inf";
  else
    output(os, x.upper(), Numeric_Format(), ROUND_NOT_NEEDED);
  os << (x.upper_is_open() ? ")" : "]");
  return os;
}

template <typename Boundary, typename Info>
inline void
Interval<Boundary, Info>::ascii_dump(std::ostream& s) const {
  using Parma_Polyhedra_Library::ascii_dump;
  s << "info ";
  info().ascii_dump(s);
  s << " lower ";
  ascii_dump(s, lower());
  s << " upper ";
  ascii_dump(s, upper());
  s << '\n';
}

template <typename Boundary, typename Info>
inline bool
Interval<Boundary, Info>::ascii_load(std::istream& s) {
  using Parma_Polyhedra_Library::ascii_load;
  std::string str;
  if (!(s >> str) || str != "info")
    return false;
  if (!info().ascii_load(s))
    return false;
  if (!(s >> str) || str != "lower")
    return false;
  if (!ascii_load(s, lower()))
    return false;
  if (!(s >> str) || str != "upper")
    return false;
  if (!ascii_load(s, upper()))
    return false;
  PPL_ASSERT(OK());
  return true;
}

/*! \brief
  Helper class to select the appropriate numerical type to perform
  boundary computations so as to reduce the chances of overflow without
  incurring too much overhead.
*/
template <typename Interval_Boundary_Type> struct Select_Temp_Boundary_Type;

template <typename Interval_Boundary_Type>
struct Select_Temp_Boundary_Type {
  typedef Interval_Boundary_Type type;
};

#if PPL_SUPPORTED_DOUBLE
template <>
struct Select_Temp_Boundary_Type<float> {
  typedef double type;
};
#endif

template <>
struct Select_Temp_Boundary_Type<char> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<signed char> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<unsigned char> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<signed short> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<unsigned short> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<signed int> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<unsigned int> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<signed long> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<unsigned long> {
  typedef signed long long type;
};

template <>
struct Select_Temp_Boundary_Type<unsigned long long> {
  typedef signed long long type;
};

/*! \relates Interval */
template <typename Boundary, typename Info>
inline void
swap(Interval<Boundary, Info>& x, Interval<Boundary, Info>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Interval_inlines_hh)
