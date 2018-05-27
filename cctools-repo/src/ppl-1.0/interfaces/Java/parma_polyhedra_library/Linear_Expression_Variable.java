/* Linear_Expression_Variable class definition and implementation.
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

//! A linear expression built from a variable.
/*! \ingroup PPL_Java_interface */
public class Linear_Expression_Variable
extends Linear_Expression {

    protected Variable arg;

    //! Builds the object associated to the copy of \p v.
    public Linear_Expression_Variable(Variable v) {
	arg = new Variable(v.id());
    }

    //! Returns the variable representing the linear expression.
    public Variable argument() {
        return arg;
    }

    //! Builds a copy of this.
    public Linear_Expression_Variable clone() {
	return new Linear_Expression_Variable(arg);
    }

    private int var_id() {
        return arg.id();
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}

