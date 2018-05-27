/* Constraint Java class declaration and implementation.
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

import java.io.Writer;
import java.io.IOException;

//! A linear equality or inequality.
/*! \ingroup PPL_Java_interface
  An object of the class Constraint is either:
  - a linear equality;
  - a non-strict linear inequality;
  - a strict linear inequality.
*/
public class Constraint {

    //! The value of the left hand side of \p this.
    private Linear_Expression lhs;

    //! The value of the right hand side of \p this.
    private Linear_Expression rhs;

    //! The relation symbol of \p this.
    private Relation_Symbol kind;

    /*! \brief
      Builds a constraint from two linear expressions with a specified
      relation symbol.
    */
    public Constraint(Linear_Expression le1, Relation_Symbol rel_sym,
                      Linear_Expression le2) {
	lhs = le1.clone();
	rhs = le2.clone();
	kind = rel_sym;
    }

    //! Returns the left hand side of \p this.
    public Linear_Expression left_hand_side() {
	return lhs;
    }

    //! Returns the right hand side of \p this.
    public Linear_Expression right_hand_side() {
	return rhs;
    }

    //! Returns the relation symbol of \p this.
    public Relation_Symbol kind() {
	return kind;
    }

    //! Returns an ascii formatted internal representation of \p this.
    public native String ascii_dump();

    //! Returns a string representation of \p this.
    public native String toString();

    private static native void initIDs();
    static {
        initIDs();
    }
}
