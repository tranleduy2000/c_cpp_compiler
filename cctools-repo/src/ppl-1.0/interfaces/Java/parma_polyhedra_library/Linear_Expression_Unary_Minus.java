/* Linear_Expression_Unary_Minus class definition and implementation.
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

//! The negation of a linear expression.
/*! \ingroup PPL_Java_interface */
public class Linear_Expression_Unary_Minus
    extends Linear_Expression {

    //! The value that \p this negates.
    protected Linear_Expression arg;

    //! Builds an object that represents the negation of the copy \p x.
    public Linear_Expression_Unary_Minus(Linear_Expression x) {
	arg = x.clone();
    }

    //! Returns the value that \p this negates.
    public Linear_Expression argument() {
	return arg;
    }

    //! Builds a copy of this.
    public Linear_Expression_Unary_Minus clone() {
        return new Linear_Expression_Unary_Minus(arg);
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}

