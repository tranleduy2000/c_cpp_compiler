/* IO Java class declaration and implementation.
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

package parma_polyhedra_library;

//! A class collecting I/O functions.
/*! \ingroup PPL_Java_interface */
public class IO {
    //! Utility function for the wrapping of lines of text.
    /*!
      \param str
      The source string holding the lines to wrap.

      \param indent_depth
      The indentation depth.

      \param preferred_first_line_length
      The preferred length for the first line of text.

      \param preferred_line_length
      The preferred length for all the lines but the first one.

      \return
      The wrapped string.
    */
    public static native String wrap_string(String str, int indent_depth,
					    int preferred_first_line_length, int preferred_line_length);
}
