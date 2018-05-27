/* Congruence Java class declaration and implementation.
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

//! A linear congruence.
/*! \ingroup PPL_Java_interface
  An object of the class Congruence is an object represeting a congruence:
  - \f$\cg = \sum_{i=0}^{n-1} a_i x_i + b = 0 \pmod{m}\f$

  where \f$n\f$ is the dimension of the space,
  \f$a_i\f$ is the integer coefficient of variable \f$x_i\f$,
  \f$b\f$ is the integer inhomogeneous term and \f$m\f$ is the integer modulus;
  if \f$m = 0\f$, then \f$\cg\f$ represents the equality congruence
  \f$\sum_{i=0}^{n-1} a_i x_i + b = 0\f$
  and, if \f$m \neq 0\f$, then the congruence \f$\cg\f$ is
  said to be a proper congruence.
*/
public class Congruence {

    //! The modulus of the congruence.
    protected Coefficient mod;

    //! The value of the left hand side of \p this.
    Linear_Expression lhs;

    //! The value of the right hand side of \p this.
    Linear_Expression rhs;

    /*! \brief
      Returns the congruence
      \f$\mathtt{e1} = \mathtt{e2} \pmod{\mathtt{m}}\f$.
    */
    public Congruence(Linear_Expression e1, Linear_Expression e2,
		      Coefficient m) {
	mod = new Coefficient(m);
	lhs = e1.clone();
	rhs = e2.clone();
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
    public Coefficient modulus() {
	return mod;
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
