/* Partial_Function Java interface declaration.
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

//! A partial function on space dimension indices.
/*! \ingroup PPL_Java_interface
  This class is used in order to specify how space dimensions should
  be mapped by methods named \c map_space_dimensions.
*/
public class Partial_Function extends PPL_Object {

    /*! \brief
      Builds the empty map.
    */
    public Partial_Function() {
	build_cpp_object();
    }

    /*! \brief
      Inserts mapping from \p i to \p j.
    */
    public native void insert(long i, long j);

    /*! \brief
      Returns \c true if and only if the partial function has
      an empty codomain (i.e., it is always undefined).

      This method will always be called before the other methods
      of the interface. Moreover, if \c true is returned, then
      none of the other interface methods will be called.
    */
    public native boolean has_empty_codomain();

    /*! \brief
      Returns the maximum value that belongs to the codomain
      of the partial function.
    */
    public native long max_in_codomain();

    /*! \brief
      If the partial function is defined on index \p i, returns its value.

      The function returns a negative value if the partial function
      is not defined on domain value \p i.
    */
    public native long maps(long i);

    /*! \brief
      Releases all resources managed by \p this,
      also resetting it to a null reference.
    */
    public native void free();

    //! Releases all resources managed by \p this.
    protected native void finalize();

    //! Builds the underlying C++ object.
    private native void build_cpp_object();
}
