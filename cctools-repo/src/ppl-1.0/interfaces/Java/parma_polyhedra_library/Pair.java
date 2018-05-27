/* Pair Java class declaration and implementation.
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

//! A pair of values of type K and V.
/*! \ingroup PPL_Java_interface
  An object of this class holds an ordered pair of values of type K and V.
*/
public class Pair<K, V>  {

    //! Stores an object of type K.
    private K first;

    //! Stores an object of type V.
    private V second;

    //! Returns the object of type K.
    public K getFirst() {
	return first;
    }

    //! Returns the object of type V.
    public V getSecond() {
	return second;
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}
