/* Checked_Number class implementation: non-inline template functions.
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

#ifndef PPL_Checked_Number_templates_hh
#define PPL_Checked_Number_templates_hh 1

#include "assert.hh"
#include <iomanip>
#include <limits>

namespace Parma_Polyhedra_Library {

template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, void>::type
ascii_dump(std::ostream& s, const T& t) {
  if (std::numeric_limits<T>::is_exact)
    // An exact data type: pretty printer is accurate.
    s << t;
  else {
    // An inexact data type (probably floating point):
    // first dump its hexadecimal representation ...
    const std::ios::fmtflags old_flags = s.setf(std::ios::hex,
                                                std::ios::basefield);
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&t);
    for (unsigned i = 0; i < sizeof(T); ++i) {
      s << std::setw(2) << std::setfill('0') << static_cast<unsigned>(p[i]);
    }
    s.flags(old_flags);
    // ... and then pretty print it for readability.
    s << " (" << t << ")";
  }
}

template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
ascii_load(std::istream& s, T& t) {
  if (std::numeric_limits<T>::is_exact) {
    // An exact data type: input from pretty printed version is accurate.
    s >> t;
    return !s.fail();
  }
  else {
    // An inexact data type (probably floating point):
    // first load its hexadecimal representation ...
    std::string str;
    if (!(s >> str) || str.size() != 2*sizeof(T))
      return false;
    unsigned char* p = reinterpret_cast<unsigned char*>(&t);
    // CHECKME: any (portable) simpler way?
    for (unsigned i = 0; i < sizeof(T); ++i) {
      unsigned byte_value = 0;
      for (unsigned j = 0; j < 2; ++j) {
        byte_value <<= 4;
        unsigned half_byte_value;
        // Interpret single hex character.
        switch (str[2*i + j]) {
        case '0':
          half_byte_value = 0;
          break;
        case '1':
          half_byte_value = 1;
          break;
        case '2':
          half_byte_value = 2;
          break;
        case '3':
          half_byte_value = 3;
          break;
        case '4':
          half_byte_value = 4;
          break;
        case '5':
          half_byte_value = 5;
          break;
        case '6':
          half_byte_value = 6;
          break;
        case '7':
          half_byte_value = 7;
          break;
        case '8':
          half_byte_value = 8;
          break;
        case '9':
          half_byte_value = 9;
          break;
        case 'A':
        case 'a':
          half_byte_value = 10;
          break;
        case 'B':
        case 'b':
          half_byte_value = 11;
          break;
        case 'C':
        case 'c':
          half_byte_value = 12;
          break;
        case 'D':
        case 'd':
          half_byte_value = 13;
          break;
        case 'E':
        case 'e':
          half_byte_value = 14;
          break;
        case 'F':
        case 'f':
          half_byte_value = 15;
          break;
        default:
          return false;
        }
        byte_value += half_byte_value;
      }
      PPL_ASSERT(byte_value <= 255);
      p[i] = static_cast<unsigned char>(byte_value);
    }
    // ... then read and discard pretty printed value.
    if (!(s >> str))
      return false;
    const std::string::size_type sz = str.size();
    return sz > 2 && str[0] == '(' && str[sz-1] == ')';
  }
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Checked_Number_templates_hh)
