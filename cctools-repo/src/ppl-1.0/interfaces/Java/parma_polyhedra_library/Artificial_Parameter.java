/* Artificial_Parameter Java class declaration and implementation.
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

/*!
  An Artificial_Parameter object represents the result
  of the integer division of a Linear_Expression (on the other
  parameters, including the previously-defined artificials)
  by an integer denominator (a Coefficient object).
  The dimensions of the artificial parameters (if any) in a tree node
  have consecutive indices starting from <code>dim+1</code>, where the value
  of \c dim is computed as follows:
   - for the tree root node, \c dim is the space dimension of the PIP_Problem;
   - for any other node of the tree, it is recusrively obtained by adding
     the value of \c dim computed for the parent node to the number of
     artificial parameters defined in the parent node.
  \par
  Since the numbering of dimensions for artificial parameters follows
  the rule above, the addition of new problem variables and/or new problem
  parameters to an already solved PIP_Problem object (as done when
  incrementally solving a problem) will result in the systematic
  renumbering of all the existing artificial parameters.
*/
public class Artificial_Parameter {

    //! The value of the linear expression of \p this.
    private Linear_Expression le;

    //! The value of the denominatir of \p this.
    private Coefficient den;

    /*! \brief
      Builds an artificial parameter from a linear expression
      and a denominator.
    */
    public Artificial_Parameter(Linear_Expression e, Coefficient d) {
	le = e.clone();
	den = new Coefficient(d);
    }

    /*! \brief
      Returns the linear expression in artificial parameter \p this.
    */
    public Linear_Expression linear_expression() {
	return le;
    }

    /*! \brief
      Returns the denominator in artificial parameter \p this.
    */
    public Coefficient denominator() {
	return den;
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
