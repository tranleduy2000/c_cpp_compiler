/* PIP_Tree_Node Java class declaration and implementation.
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

//! A node of the PIP solution tree.
/*!
  This is the base class for the nodes of the binary trees representing
  the solutions of PIP problems. From this one, two classes are derived:
    - PIP_Decision_Node, for the internal nodes of the tree;
    - PIP_Solution_Node, for the leaves of the tree.
*/
public class PIP_Tree_Node extends PPL_Object {

    /*! \brief
      Returns the solution node if \p this is a solution node,
      and 0 otherwise.
    */
    public native PIP_Solution_Node as_solution();

    /*! \brief
      Returns the decision node if \p this is a decision node,
      and 0 otherwise.
    */
    public native PIP_Decision_Node as_decision();

    /*! \brief
      Returns true if the pip tree is well formed, i.e., if it
      satisfies all its implementation invariants; returns 0 and perhaps
      makes some noise if broken.  Useful for debugging purposes.
    */
    public native boolean OK();

    /*! \brief
       Returns the number of artificial parameters in the PIP_Tree_Node.
    */
    public native long number_of_artificials();

    /*! \brief
       Returns the sequence of (Java) artificial parameters
       in the PIP_Tree_Node.
    */
    public native Artificial_Parameter_Sequence artificials();

    /*! \brief
       Returns the system of parameter constraints controlling the
       PIP_Tree_Node.

       The indices in the constraints are the same as the original
       variables and parameters. Coefficients in indices corresponding
       to variables always are zero.
    */
    public native Constraint_System constraints();

    //! Returns a string representation of \p this.
    public native String toString();

}
