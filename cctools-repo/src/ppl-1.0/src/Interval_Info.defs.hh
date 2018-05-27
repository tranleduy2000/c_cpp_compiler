/* Interval_Info class declaration and implementation.
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

#ifndef PPL_Interval_Info_defs_hh
#define PPL_Interval_Info_defs_hh 1

#include "Boundary.defs.hh"

#include <iostream>

namespace Parma_Polyhedra_Library {

namespace Interval_NS {

struct Property {
  enum Type {
    CARDINALITY_0_,
    CARDINALITY_1_,
    CARDINALITY_IS_
  };
  typedef bool Value;
  static const Value default_value = true;
  static const Value unsupported_value = false;
  Property(Type t)
    : type(t) {
  }
  Type type;
};

const Property CARDINALITY_0(Property::CARDINALITY_0_);
const Property CARDINALITY_1(Property::CARDINALITY_1_);
const Property CARDINALITY_IS(Property::CARDINALITY_IS_);

template <typename T>
inline void
reset_bits(T& bits) {
  bits = 0;
}

template <typename T>
inline void
reset_bit(T& bits, unsigned int bit) {
  bits &= ~(static_cast<T>(1) << bit);
}

template <typename T>
inline void
set_bit(T& bits, unsigned int bit, bool value) {
  if (value)
    bits |= static_cast<T>(1) << bit;
  else
    reset_bit(bits, bit);
}

template <typename T>
inline bool
get_bit(const T& bits, unsigned int bit) {
  return (bits & (static_cast<T>(1) << bit)) != 0;
}

template <typename T>
inline void
set_bits(T& bits, unsigned int start, unsigned int len, T value) {
  bits &= ~(((static_cast<T>(1) << len) - 1) << start);
  bits |= value << start;
}

template <typename T>
inline T
get_bits(T& bits, unsigned int start, unsigned int len) {
  return (bits >> start) & ((static_cast<T>(1) << len) - 1);
}

} // namespace Interval_NS

using namespace Interval_NS;
using namespace Boundary_NS;


template <typename Policy>
class Interval_Info_Null {
public:
  const_bool_nodef(may_be_empty, Policy::may_be_empty);
  const_bool_nodef(may_contain_infinity, Policy::may_contain_infinity);
  const_bool_nodef(check_inexact, Policy::check_inexact);
  const_bool_nodef(store_special, false);
  const_bool_nodef(store_open, false);
  const_bool_nodef(cache_empty, false);
  const_bool_nodef(cache_singleton, false);
  Interval_Info_Null() {
  }
  void clear() {
  }
  void clear_boundary_properties(Boundary_Type) {
  }

  template <typename Property>
  void set_boundary_property(Boundary_Type, const Property&, typename Property::Value = Property::default_value) {
  }
  template <typename Property>
  typename Property::Value get_boundary_property(Boundary_Type, const Property&) const {
    return Property::unsupported_value;
  }
  template <typename Property>
  void set_interval_property(const Property&, typename Property::Value = Property::default_value) {
  }
  template <typename Property>
  typename Property::Value get_interval_property(const Property&) const {
    return Property::unsupported_value;
  }

  //! Swaps \p *this with \p y.
  void m_swap(Interval_Info_Null& y);

  void ascii_dump(std::ostream& s) const;
  bool ascii_load(std::istream& s);
};

template <typename Policy>
class Interval_Info_Null_Open : public Interval_Info_Null<Policy> {
public:
  const_bool_nodef(store_open, true);
  Interval_Info_Null_Open(bool o)
    : open(o) {
  }
  bool get_boundary_property(Boundary_Type,
                             const Boundary_NS::Property& p) const {
    if (p.type == Boundary_NS::Property::OPEN_)
      return open;
    else
      return Boundary_NS::Property::unsupported_value;
  }

  void ascii_dump(std::ostream& s) const;
  bool ascii_load(std::istream& s);

private:
  bool open;
};


template <typename T, typename Policy>
class Interval_Info_Bitset {
public:
  const_bool_nodef(may_be_empty, Policy::may_be_empty);
  const_bool_nodef(may_contain_infinity, Policy::may_contain_infinity);
  const_bool_nodef(check_inexact, Policy::check_inexact);
  const_bool_nodef(store_special, Policy::store_special);
  const_bool_nodef(store_open, Policy::store_open);
  const_bool_nodef(cache_empty, Policy::cache_empty);
  const_bool_nodef(cache_singleton, Policy::cache_singleton);
  const_int_nodef(lower_special_bit, Policy::next_bit);
  const_int_nodef(lower_open_bit, lower_special_bit + (store_special ? 1 : 0));
  const_int_nodef(upper_special_bit, lower_open_bit + (store_open ? 1 : 0));
  const_int_nodef(upper_open_bit, upper_special_bit + (store_special ? 1 : 0));
  const_int_nodef(cardinality_is_bit, upper_open_bit + (store_open ? 1 : 0));
  const_int_nodef(cardinality_0_bit, cardinality_is_bit
                  + ((cache_empty || cache_singleton) ? 1 : 0));
  const_int_nodef(cardinality_1_bit, cardinality_0_bit + (cache_empty ? 1 : 0));
  const_int_nodef(next_bit, cardinality_1_bit + (cache_singleton ? 1 : 0));

  Interval_Info_Bitset() {
    // FIXME: would we have speed benefits with uninitialized info?
    // (Dirty_Temp)
    clear();
  }

  void clear() {
    reset_bits(bitset);
  }
  void clear_boundary_properties(Boundary_Type t) {
    set_boundary_property(t, SPECIAL, false);
    set_boundary_property(t, OPEN, false);
  }
  void set_boundary_property(Boundary_Type t,
                             const Boundary_NS::Property& p,
                             bool value = true) {
    switch (p.type) {
    case Boundary_NS::Property::SPECIAL_:
      if (store_special) {
	if (t == LOWER)
	  set_bit(bitset, lower_special_bit, value);
	else
	  set_bit(bitset, upper_special_bit, value);
      }
      break;
    case Boundary_NS::Property::OPEN_:
      if (store_open) {
	if (t == LOWER)
	  set_bit(bitset, lower_open_bit, value);
	else
	  set_bit(bitset, upper_open_bit, value);
      }
      break;
    default:
      break;
    }
  }
  bool get_boundary_property(Boundary_Type t, const Boundary_NS::Property& p) const {
    switch (p.type) {
    case Boundary_NS::Property::SPECIAL_:
      if (!store_special)
	return false;
      if (t == LOWER)
	return get_bit(bitset, lower_special_bit);
      else
	return get_bit(bitset, upper_special_bit);
    case Boundary_NS::Property::OPEN_:
      if (!store_open)
	return false;
      else if (t == LOWER)
	return get_bit(bitset, lower_open_bit);
      else
	return get_bit(bitset, upper_open_bit);
    default:
      return false;
    }
  }
  void set_interval_property(const Interval_NS::Property& p, bool value = true) {
    switch (p.type) {
    case Interval_NS::Property::CARDINALITY_0_:
      if (cache_empty)
	set_bit(bitset, cardinality_0_bit, value);
      break;
    case Interval_NS::Property::CARDINALITY_1_:
      if (cache_singleton)
	set_bit(bitset, cardinality_1_bit, value);
      break;
    case Interval_NS::Property::CARDINALITY_IS_:
      if (cache_empty || cache_singleton)
	set_bit(bitset, cardinality_is_bit, value);
      break;
    default:
      break;
    }
  }
  bool get_interval_property(Interval_NS::Property p) const {
    switch (p.type) {
    case Interval_NS::Property::CARDINALITY_0_:
      return cache_empty && get_bit(bitset, cardinality_0_bit);
    case Interval_NS::Property::CARDINALITY_1_:
      return cache_singleton && get_bit(bitset, cardinality_1_bit);
    case Interval_NS::Property::CARDINALITY_IS_:
      return (cache_empty || cache_singleton)
        && get_bit(bitset, cardinality_is_bit);
    default:
      return false;
    }
  }

  //! Swaps \p *this with \p y.
  void m_swap(Interval_Info_Bitset& y);

  void ascii_dump(std::ostream& s) const;
  bool ascii_load(std::istream& s);

protected:
  T bitset;
};

}

#include "Interval_Info.inlines.hh"

#endif // !defined(PPL_Interval_Info_defs_hh)
