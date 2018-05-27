/* PIP_Problem PIP_Problem_Control_Parameter_Value enum declaration.
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

//! Possible values for PIP problems' control parameters.
/*! \ingroup PPL_Java_interface */
public enum PIP_Problem_Control_Parameter_Value {
    //! Choose the first non-integer row
    CUTTING_STRATEGY_FIRST,
    //! Choose row which generates the deepest cut
    CUTTING_STRATEGY_DEEPEST,
    //! Always generate all possible cuts
    CUTTING_STRATEGY_ALL,
    //! Choose the first row with negative parameter sign
    PIVOT_ROW_STRATEGY_FIRST,
    //! Choose the row which generates the lexico-maximal pivot column
    PIVOT_ROW_STRATEGY_MAX_COLUMN
}
