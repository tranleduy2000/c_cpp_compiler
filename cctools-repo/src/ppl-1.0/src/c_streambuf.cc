/* c_streambuf class implementation (non-inline functions).
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
#include "c_streambuf.defs.hh"
#include "globals.defs.hh"
#include "assert.hh"

namespace Parma_Polyhedra_Library {

c_streambuf::int_type
c_streambuf::uflow() {
  int_type c = underflow();
  next_char_buf = traits_type::eof();
  return c;
}

c_streambuf::int_type
c_streambuf::underflow() {
  const int_type eof = traits_type::eof();
  if (traits_type::eq_int_type(next_char_buf, eof)) {
    char buf;
    if (cb_read(&buf, 1) == 1)
      next_char_buf = buf;
    else
      next_char_buf = eof;
  }
  return next_char_buf;
}

std::streamsize
c_streambuf::xsgetn(char_type* s, std::streamsize n) {
  PPL_ASSERT(n >= 0);
  if (n == 0)
    return n;
  const int_type eof = traits_type::eof();
  const size_t sz_n = static_cast<size_t>(n);
  size_t a;
  if (traits_type::eq_int_type(next_char_buf, eof))
    a = 0;
  else {
    s[0] = static_cast<char_type>(next_char_buf);
    a = 1;
  }
  const size_t r = cb_read(s + a, sz_n - a) + a;
  if (r > 0)
    unget_char_buf = traits_type::to_int_type(s[r - 1]);
  else
    unget_char_buf = traits_type::eof();
  return static_cast<std::streamsize>(r);
}

c_streambuf::int_type
c_streambuf::pbackfail(int_type c) {
  const int_type eof = traits_type::eof();
  next_char_buf = traits_type::eq_int_type(c, eof) ? unget_char_buf : c;
  unget_char_buf = eof;
  return next_char_buf;
}

std::streamsize
c_streambuf::xsputn(const char_type* s, std::streamsize n) {
  PPL_ASSERT(n >= 0);
  size_t r = cb_write(s, static_cast<size_t>(n));
  return static_cast<std::streamsize>(r);
}

c_streambuf::int_type
c_streambuf::overflow(int_type c) {
  const int_type eof = traits_type::eof();
  if (traits_type::eq_int_type(c, eof))
    return (sync() != 0) ? eof : traits_type::not_eof(c);
  else {
    char buf = static_cast<char>(c);
    if (cb_write(&buf, 1) == 1)
      return c;
    else
      return eof;
  }
}

int
c_streambuf::sync() {
  return cb_sync();
}

} // namespace Parma_Polyhedra_Library
