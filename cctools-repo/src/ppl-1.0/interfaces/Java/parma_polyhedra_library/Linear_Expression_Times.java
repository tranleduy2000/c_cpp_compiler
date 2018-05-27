/* Linear_Expression_Times class definition and implementation.
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

//! The product of a linear expression and a coefficient.
/*! \ingroup PPL_Java_interface */
public class Linear_Expression_Times
    extends Linear_Expression {

    //! The value of the coefficient.
    protected Coefficient coeff;

    //! The value of the inner linear expression.
    protected Linear_Expression lin_expr;

    //! Builds an object cloning the input arguments.
    public Linear_Expression_Times(Coefficient c, Variable v) {
	coeff = new Coefficient(c);
	lin_expr = new Linear_Expression_Variable(v);
    }

    //! Builds an object cloning the input arguments.
    public Linear_Expression_Times(Coefficient c, Linear_Expression l) {
	coeff = new Coefficient(c);
	lin_expr = l.clone();
    }

    //! Builds an object cloning the input arguments.
    public Linear_Expression_Times(Linear_Expression l, Coefficient c) {
	coeff = new Coefficient(c);
	lin_expr = l.clone();
    }

    //! Returns the coefficient of \p this.
    public Coefficient coefficient() {
	return coeff;
    }

    //! Returns the linear expression subobject of \p this.
    public Linear_Expression linear_expression() {
	return lin_expr;
    }

    //! Builds a copy of this.
    public Linear_Expression_Times clone() {
	return new Linear_Expression_Times(coeff, lin_expr);
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}
