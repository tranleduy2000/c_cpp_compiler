/* Variable Java class declaration and implementation.
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

import javax.management.RuntimeErrorException;

//! A dimension of the vector space.
/*! \ingroup PPL_Java_interface
  An object of the class Variable represents a dimension of the space,
  that is one of the Cartesian axes.
  Variables are used as basic blocks in order to build
  more complex linear expressions.
  Each variable is identified by a non-negative integer,
  representing the index of the corresponding Cartesian axis
  (the first axis has index 0).
*/
public class Variable implements Comparable<Variable> {
    //! The index of the Cartesian axis.
    private int varid;

    //! Builds the variable corresponding to the Cartesian axis of index \p i.
    /*!
      \exception RuntimeErrorException
      Thrown if <CODE>i</CODE> is has negative value.
    */
    public Variable(int i) {
	if (i < 0)
	    throw new
		RuntimeErrorException(new Error("parma_polyhedra_library.Variable::"
						+ "Variable:"
						+ " an index variable"
						+ " cannot be negative."));
	varid = i;
    }

    //! Returns the index of the Cartesian axis associated to \p this.
    public int id() {
	return varid;
    }

  /*! \brief
    Returns a negative number if \p this comes first than \p v,
    a zero if \p this equals \p v, a positive number if
   if \p this comes first than \p v.
   */
    public int compareTo(Variable v) {
        return varid - v.varid;
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}
