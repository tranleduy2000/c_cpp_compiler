/* stdiobuf class declaration.
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

#ifndef PPL_stdiobuf_defs_hh
#define PPL_stdiobuf_defs_hh 1

#include "stdiobuf.types.hh"
#include <cstdio>
#include <streambuf>

class Parma_Polyhedra_Library::stdiobuf
  : public std::basic_streambuf<char, std::char_traits<char> > {
public:
  //! Constructor.
  stdiobuf(FILE* file);

protected:
  /*! \brief
    Gets a character in case of underflow.

    \remarks
    Specified by ISO/IEC 14882:1998: 27.5.2.4.3.
  */
  virtual int_type underflow();

  /*! \brief
    In case of underflow, gets a character and advances the next pointer.

    \remarks
    Specified by ISO/IEC 14882:1998: 27.5.2.4.3.
  */
  virtual int_type uflow();

  /*! \brief
    Gets a sequence of characters.

    \remarks
    Specified by ISO/IEC 14882:1998: 27.5.2.4.3.
  */
  virtual std::streamsize xsgetn(char_type* s, std::streamsize n);

  /*! \brief
    Puts character back in case of backup underflow.

    \remarks
    Specified by ISO/IEC 14882:1998: 27.5.2.4.4.
  */
  virtual int_type pbackfail(int_type c = traits_type::eof());

  /*! \brief
    Writes a sequence of characters.

    \remarks
    Specified by ISO/IEC 14882:1998: 27.5.2.4.5.
  */
  virtual std::streamsize xsputn(const char_type* s, std::streamsize n);

  /*! \brief
    Writes a character in case of overflow.

    Specified by ISO/IEC 14882:1998: 27.5.2.4.5.
  */
  virtual int_type overflow(int_type c);

  /*! \brief
    Synchronizes the stream buffer.

    Specified by ISO/IEC 14882:1998: 27.5.2.4.2.
  */
  virtual int sync();

private:
  //! Character type of the streambuf.
  typedef char char_type;

  //! Traits type of the streambuf.
  typedef std::char_traits<char_type> traits_type;

  //! Integer type of the streambuf.
  typedef traits_type::int_type int_type;

  //! The encapsulated stdio file.
  FILE* fp;

  //! Buffer for the last character read.
  int_type unget_char_buf;
};

#include "stdiobuf.inlines.hh"

#endif // !defined(PPL_stdiobuf_defs_hh)
