/* Box<ITV>::Status class implementation: inline functions.
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

#ifndef PPL_Box_Status_inlines_hh
#define PPL_Box_Status_inlines_hh 1

#include <string>

namespace Parma_Polyhedra_Library {

template <typename ITV>
inline
Box<ITV>::Status::Status(flags_t mask)
  : flags(mask) {
}

template <typename ITV>
inline
Box<ITV>::Status::Status(const Status& y)
  : flags(y.flags) {
}

template <typename ITV>
template <typename Other_ITV>
inline
Box<ITV>::Status::Status(const typename Box<Other_ITV>::Status& y)
  : flags(y.flags) {
}

template <typename ITV>
inline
Box<ITV>::Status::Status()
  : flags(NONE) {
}

template <typename ITV>
inline bool
Box<ITV>::Status::test_all(flags_t mask) const {
  return (flags & mask) == mask;
}

template <typename ITV>
inline bool
Box<ITV>::Status::test_any(flags_t mask) const {
  return (flags & mask) != 0;
}

template <typename ITV>
inline void
Box<ITV>::Status::set(flags_t mask) {
  flags |= mask;
}

template <typename ITV>
inline void
Box<ITV>::Status::reset(flags_t mask) {
  flags &= ~mask;
}

template <typename ITV>
inline bool
Box<ITV>::Status::test_empty_up_to_date() const {
  return test_any(EMPTY_UP_TO_DATE);
}

template <typename ITV>
inline void
Box<ITV>::Status::reset_empty_up_to_date() {
  reset(EMPTY_UP_TO_DATE);
}

template <typename ITV>
inline void
Box<ITV>::Status::set_empty_up_to_date() {
  set(EMPTY_UP_TO_DATE);
}

template <typename ITV>
inline bool
Box<ITV>::Status::test_empty() const {
  return test_any(EMPTY);
}

template <typename ITV>
inline void
Box<ITV>::Status::reset_empty() {
  reset(EMPTY);
}

template <typename ITV>
inline void
Box<ITV>::Status::set_empty() {
  set(EMPTY);
}

template <typename ITV>
inline bool
Box<ITV>::Status::test_universe() const {
  return test_any(UNIVERSE);
}

template <typename ITV>
inline void
Box<ITV>::Status::reset_universe() {
  reset(UNIVERSE);
}

template <typename ITV>
inline void
Box<ITV>::Status::set_universe() {
  set(UNIVERSE);
}

template <typename ITV>
bool
Box<ITV>::Status::OK() const {
  if (test_empty_up_to_date()
      && test_empty()
      && test_universe()) {
#ifndef NDEBUG
    std::cerr
      << "The status asserts emptiness and universality at the same time."
      << std::endl;
#endif
    return false;
  }

  // Any other case is OK.
  return true;
}


namespace Implementation {

namespace Boxes {

// These are the keywords that indicate the individual assertions.
const std::string empty_up_to_date = "EUP";
const std::string empty = "EM";
const std::string universe = "UN";
const char yes = '+';
const char no = '-';
const char separator = ' ';

/*! \relates Parma_Polyhedra_Library::Box::Status
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

} // namespace Boxes

} // namespace Implementation

template <typename ITV>
void
Box<ITV>::Status::ascii_dump(std::ostream& s) const {
  using namespace Implementation::Boxes;
  s << (test_empty_up_to_date() ? yes : no) << empty_up_to_date << separator
    << (test_empty() ? yes : no) << empty << separator
    << (test_universe() ? yes : no) << universe << separator;
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS_ASCII_ONLY(ITV, Box<ITV>::Status)

template <typename ITV>
bool
Box<ITV>::Status::ascii_load(std::istream& s) {
  using namespace Implementation::Boxes;
  PPL_UNINITIALIZED(bool, positive);

  if (!get_field(s, Implementation::Boxes::empty_up_to_date, positive))
    return false;
  if (positive)
    set_empty_up_to_date();

  if (!get_field(s, Implementation::Boxes::empty, positive))
    return false;
  if (positive)
    set_empty();

  if (!get_field(s, universe, positive))
    return false;
  if (positive)
    set_universe();
  else
    reset_universe();

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Box_Status_inlines_hh)
