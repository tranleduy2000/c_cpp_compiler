/* By_Reference Java class declaration and implementation.
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

//! An utility class implementing mutable and non-mutable call-by-reference.
/*! \ingroup PPL_Java_interface */
public class By_Reference<T> {
    //! Stores the object.
    T obj;

    //! Builds an object encapsulating \p object_value.
    public By_Reference(T object_value) {
	obj = object_value;
    }

    //! Set an object to value \p object_value.
    public void set(T y) {
	obj = y;
    }

    //! Returns the value held by \p this.
    public T get() {
	return obj;
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}
