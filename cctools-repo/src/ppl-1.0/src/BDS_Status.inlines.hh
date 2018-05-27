/* BD_Shape<T>::Status class implementation: inline functions.
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

#ifndef PPL_BDS_Status_inlines_hh
#define PPL_BDS_Status_inlines_hh 1

namespace Parma_Polyhedra_Library {

template <typename T>
inline
BD_Shape<T>::Status::Status(flags_t mask)
  : flags(mask) {
}

template <typename T>
inline
BD_Shape<T>::Status::Status()
  : flags(ZERO_DIM_UNIV) {
}

template <typename T>
inline bool
BD_Shape<T>::Status::test_all(flags_t mask) const {
  return (flags & mask) == mask;
}

template <typename T>
inline bool
BD_Shape<T>::Status::test_any(flags_t mask) const {
  return (flags & mask) != 0;
}

template <typename T>
inline void
BD_Shape<T>::Status::set(flags_t mask) {
  flags |= mask;
}

template <typename T>
inline void
BD_Shape<T>::Status::reset(flags_t mask) {
  flags &= ~mask;
}

template <typename T>
inline bool
BD_Shape<T>::Status::test_zero_dim_univ() const {
  return flags == ZERO_DIM_UNIV;
}

template <typename T>
inline void
BD_Shape<T>::Status::reset_zero_dim_univ() {
  // This is a no-op if the current status is not zero-dim.
  if (flags == ZERO_DIM_UNIV)
    // In the zero-dim space, if it is not the universe it is empty.
    flags = EMPTY;
}

template <typename T>
inline void
BD_Shape<T>::Status::set_zero_dim_univ() {
  // Zero-dim universe is incompatible with anything else.
  flags = ZERO_DIM_UNIV;
}

template <typename T>
inline bool
BD_Shape<T>::Status::test_empty() const {
  return test_any(EMPTY);
}

template <typename T>
inline void
BD_Shape<T>::Status::reset_empty() {
  reset(EMPTY);
}

template <typename T>
inline void
BD_Shape<T>::Status::set_empty() {
  flags = EMPTY;
}

template <typename T>
inline bool
BD_Shape<T>::Status::test_shortest_path_closed() const {
  return test_any(SHORTEST_PATH_CLOSED);
}

template <typename T>
inline void
BD_Shape<T>::Status::reset_shortest_path_closed() {
  // A system is reduced only if it is also closed.
  reset(SHORTEST_PATH_CLOSED | SHORTEST_PATH_REDUCED);
}

template <typename T>
inline void
BD_Shape<T>::Status::set_shortest_path_closed() {
  set(SHORTEST_PATH_CLOSED);
}

template <typename T>
inline bool
BD_Shape<T>::Status::test_shortest_path_reduced() const {
  return test_any(SHORTEST_PATH_REDUCED);
}

template <typename T>
inline void
BD_Shape<T>::Status::reset_shortest_path_reduced() {
  reset(SHORTEST_PATH_REDUCED);
}

template <typename T>
inline void
BD_Shape<T>::Status::set_shortest_path_reduced() {
  PPL_ASSERT(test_shortest_path_closed());
  set(SHORTEST_PATH_REDUCED);
}

template <typename T>
bool
BD_Shape<T>::Status::OK() const {
  if (test_zero_dim_univ())
    // Zero-dim universe is OK.
    return true;

  if (test_empty()) {
    Status copy = *this;
    copy.reset_empty();
    if (copy.test_zero_dim_univ())
      return true;
    else {
#ifndef NDEBUG
      std::cerr << "The empty flag is incompatible with any other one."
		<< std::endl;
#endif
      return false;
    }
  }

  // Shortest-path reduction implies shortest-path closure.
  if (test_shortest_path_reduced()) {
    if (test_shortest_path_closed())
      return true;
    else {
#ifndef NDEBUG
      std::cerr << "The shortest-path reduction flag should also imply "
		<< "the closure flag."
		<< std::endl;
#endif
      return false;
    }
  }

  // Any other case is OK.
  return true;
}


namespace Implementation {

namespace BD_Shapes {

// These are the keywords that indicate the individual assertions.
const std::string zero_dim_univ = "ZE";
const std::string empty = "EM";
const std::string sp_closed = "SPC";
const std::string sp_reduced = "SPR";
const char yes = '+';
const char no = '-';
const char separator = ' ';

/*! \relates Parma_Polyhedra_Library::BD_Shape::Status
  Reads a keyword and its associated on/off flag from \p s.
  Returns <CODE>true</CODE> if the operation is successful,
  returns <CODE>false</CODE> otherwise.
  When successful, \p positive is set to <CODE>true</CODE> if the flag
  is on; it is set to <CODE>false</CODE> otherwise.
*/
inline bool
get_field(std::istream& s, const std::string& keyword, bool& positive) {
  std::string str;
  if (!(s >> str)
      || (str[0] != yes && str[0] != no)
      || str.substr(1) != keyword)
    return false;
  positive = (str[0] == yes);
  return true;
}

} // namespace BD_Shapes

} // namespace Implementation

template <typename T>
void
BD_Shape<T>::Status::ascii_dump(std::ostream& s) const {
  using namespace Implementation::BD_Shapes;
  s << (test_zero_dim_univ() ? yes : no) << zero_dim_univ << separator
    << (test_empty() ? yes : no) << empty << separator
    << separator
    << (test_shortest_path_closed() ? yes : no) << sp_closed << separator
    << (test_shortest_path_reduced() ? yes : no) << sp_reduced << separator;
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS_ASCII_ONLY(T, BD_Shape<T>::Status)

template <typename T>
bool
BD_Shape<T>::Status::ascii_load(std::istream& s) {
  using namespace Implementation::BD_Shapes;
  PPL_UNINITIALIZED(bool, positive);

  if (!get_field(s, zero_dim_univ, positive))
    return false;
  if (positive)
    set_zero_dim_univ();

  if (!get_field(s, empty, positive))
    return false;
  if (positive)
    set_empty();

  if (!get_field(s, sp_closed, positive))
    return false;
  if (positive)
    set_shortest_path_closed();
  else
    reset_shortest_path_closed();

  if (!get_field(s, sp_reduced, positive))
    return false;
  if (positive)
    set_shortest_path_reduced();
  else
    reset_shortest_path_reduced();

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_BDS_Status_inlines_hh)
