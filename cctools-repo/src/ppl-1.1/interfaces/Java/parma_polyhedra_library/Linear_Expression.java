/* Linear_Expression Java declaration and implementation.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

//! A linear expression.
/*! \ingroup PPL_Java_interface
  An object of the class Linear_Expression represents a linear expression
  that can be built from a Linear_Expression_Variable,
  Linear_Expression_Coefficient, Linear_Expression_Sum,
  Linear_Expression_Difference, Linear_Expression_Unary_Minus.
*/
public abstract class Linear_Expression implements Cloneable {

    //! Returns the sum of \p this and \p y.
    public Linear_Expression sum(Linear_Expression y) {
        return new Linear_Expression_Sum(this, y);
    }

    //! Returns the difference of \p this and \p y.
    public Linear_Expression subtract(Linear_Expression y) {
        return new Linear_Expression_Difference(this, y);
    }

    //! Returns the product of \p this times \p c.
    public Linear_Expression times(Coefficient c) {
        return new Linear_Expression_Times(this, c);
    }

    //! Returns the negation of \p this.
    public Linear_Expression unary_minus() {
        return new Linear_Expression_Unary_Minus(this);
    }

    //! Returns a copy of the linear expression.
    public abstract Linear_Expression clone();

    //! Returns an ascii formatted internal representation of \p this.
    public native String ascii_dump();

    //! Returns a string representation of \p this.
    public native String toString();

    //! Returns <CODE>true</CODE> if and only if \p this is \f$0\f$.
    public native boolean is_zero();

    /*! \brief Returns <CODE>true</CODE> if and only if all the homogeneous
        terms of \p this are \f$0\f$.
    */
    public native boolean all_homogeneous_terms_are_zero();

    private static native void initIDs();
    static {
        initIDs();
    }
}
