/* String wrapping helper function.
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

#include "ppl-config.h"
#include "wrap_string.hh"
#include "assert.hh"
#include <cstdlib>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

std::string
wrap_string(const std::string& src_string,
	    const unsigned indent_depth,
	    const unsigned preferred_first_line_length,
	    const unsigned preferred_line_length) {
  const unsigned npos = C_Integer<unsigned>::max;
  std::string dst_string;
  const char *src = src_string.c_str();
  for (unsigned line = 0; ; ++line) {
    const unsigned line_length = ((line == 0)
                                  ? preferred_first_line_length
                                  : preferred_line_length);
    unsigned last_comma = npos;
    unsigned last_space = npos;
    unsigned split_pos = npos;
    unsigned idx;
    for (idx = 0; idx <= line_length; ++idx) {
      if (src[idx] == '\0' || src[idx] == '\n') {
	split_pos = idx;
	break;
      }
      if (src[idx] == ',' && idx < line_length)
	last_comma = idx;
      if (is_space(src[idx]) && (idx == 0 || !is_space(src[idx-1])))
	last_space = idx;
    }
    if (split_pos == npos) {
      if (last_comma != npos)
	split_pos = last_comma + 1;
      else if (last_space != npos)
	split_pos = last_space;
      else {
	for ( ; src[idx] != '\0'; ++idx) {
	  if (src[idx] == ',') {
	    ++idx;
	    break;
	  }
	  if (is_space(src[idx]))
	    break;
	}
	split_pos = idx;
      }
    }
    PPL_ASSERT(split_pos != npos);
    if (split_pos > 0 && line > 0 && indent_depth > 0)
      dst_string.append(indent_depth, ' ');
    dst_string.append(src, split_pos);
    src += split_pos;
    if (is_space(*src))
      ++src;
    while (*src == ' ')
      ++src;
    if (*src == '\0')
      break;
    dst_string.push_back('\n');
  }
  return dst_string;
}

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

