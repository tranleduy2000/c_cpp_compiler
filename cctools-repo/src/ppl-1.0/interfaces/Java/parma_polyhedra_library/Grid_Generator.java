/* Grid Generator Java class declaration and implementation.
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
import javax.management.RuntimeErrorException;

//! A grid line, parameter or grid point.
/*! \ingroup PPL_Java_interface
  An object of the class Grid_Generator is one of the following:
  - a grid_line;
  - a parameter;
  - a grid_point.
*/
public class Grid_Generator {

    //! The grid generator type.
    private Grid_Generator_Type gt;

    //! The linear expression.
    private Linear_Expression le;

    /*! \brief
      The coefficient used if the grid generator is a
      parameter or a grid point a parameter.
    */
    private Coefficient div;

    private Grid_Generator(Linear_Expression e, Coefficient d,
			   Grid_Generator_Type generator_type) {
	le = e.clone();
	div = new Coefficient(d);
	gt = generator_type;
    }

    //! Returns the line of direction \p e.
    /*!
      \exception RuntimeErrorException
      Thrown if the homogeneous part of \p e represents the origin of
      the vector space.
    */
    public static Grid_Generator grid_line(Linear_Expression e) {
	return new Grid_Generator(e, new Coefficient(0),
				  Grid_Generator_Type.LINE);
    }

    //! Returns the parameter at \p e / \p d.
    /*!
      \exception RuntimeErrorException
      Thrown if \p d is zero.
    */
    public static Grid_Generator parameter(Linear_Expression e,
					   Coefficient d) {
	return new Grid_Generator(e, d, Grid_Generator_Type.PARAMETER);
    }

    //! Returns the point at \p e / \p d.
    /*!
      \exception RuntimeErrorException
      Thrown if \p d is zero.
    */
    public static Grid_Generator grid_point(Linear_Expression e,
					    Coefficient d) {
	return new Grid_Generator(e, d, Grid_Generator_Type.POINT);
    }

    //! Returns the generator type.
    public Grid_Generator_Type type() {
        return gt;
    }

    //! Returns the linear expression in \p this.
    public Linear_Expression linear_expression() {
        return le;
    }

    //! If \p this is either a grid point or a parameter, returns its divisor.
    /*!
      \exception RuntimeErrorException
      Thrown if \p this is a line.
    */
    public Coefficient divisor() {
        if (this.gt != Grid_Generator_Type.LINE)
	    return div;
	Error cause = new Error("parma_polyhedra_library."
                                + "Grid_Generator::divisor:\n"
				+ "this is neither a grid point"
				+ " nor a parameter.");
	throw new RuntimeErrorException(cause);
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

