/* C_Polyhedron class declaration.
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

#ifndef PPL_C_Polyhedron_defs_hh
#define PPL_C_Polyhedron_defs_hh 1

#include "C_Polyhedron.types.hh"
#include "NNC_Polyhedron.types.hh"
#include "Polyhedron.defs.hh"
#include "Grid.types.hh"
#include "BD_Shape.types.hh"
#include "Octagonal_Shape.types.hh"

//! A closed convex polyhedron.
/*! \ingroup PPL_CXX_interface
    An object of the class C_Polyhedron represents a
    <EM>topologically closed</EM> convex polyhedron
    in the vector space \f$\Rset^n\f$.

    When building a closed polyhedron starting from
    a system of constraints, an exception is thrown if the system
    contains a <EM>strict inequality</EM> constraint.
    Similarly, an exception is thrown when building a closed polyhedron
    starting from a system of generators containing a <EM>closure point</EM>.

    \note
    Such an exception will be obtained even if the system of
    constraints (resp., generators) actually defines
    a topologically closed subset of the vector space, i.e.,
    even if all the strict inequalities (resp., closure points)
    in the system happen to be redundant with respect to the
    system obtained by removing all the strict inequality constraints
    (resp., all the closure points).
    In contrast, when building a closed polyhedron starting from
    an object of the class NNC_Polyhedron,
    the precise topological closure test will be performed.
*/

class Parma_Polyhedra_Library::C_Polyhedron : public Polyhedron {
public:
  //! Builds either the universe or the empty C polyhedron.
  /*!
    \param num_dimensions
    The number of dimensions of the vector space enclosing the C polyhedron;

    \param kind
    Specifies whether a universe or an empty C polyhedron should be built.

    \exception std::length_error
    Thrown if \p num_dimensions exceeds the maximum allowed space dimension.

    Both parameters are optional:
    by default, a 0-dimension space universe C polyhedron is built.
  */
  explicit C_Polyhedron(dimension_type num_dimensions = 0,
			Degenerate_Element kind = UNIVERSE);

  //! Builds a C polyhedron from a system of constraints.
  /*!
    The polyhedron inherits the space dimension of the constraint system.

    \param cs
    The system of constraints defining the polyhedron.

    \exception std::invalid_argument
    Thrown if the system of constraints contains strict inequalities.
  */
  explicit C_Polyhedron(const Constraint_System& cs);

  //! Builds a C polyhedron recycling a system of constraints.
  /*!
    The polyhedron inherits the space dimension of the constraint system.

    \param cs
    The system of constraints defining the polyhedron.  It is not
    declared <CODE>const</CODE> because its data-structures may be
    recycled to build the polyhedron.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.

    \exception std::invalid_argument
    Thrown if the system of constraints contains strict inequalities.
  */
  C_Polyhedron(Constraint_System& cs, Recycle_Input dummy);

  //! Builds a C polyhedron from a system of generators.
  /*!
    The polyhedron inherits the space dimension of the generator system.

    \param gs
    The system of generators defining the polyhedron.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points,
    or if it contains closure points.
  */
  explicit C_Polyhedron(const Generator_System& gs);

  //! Builds a C polyhedron recycling a system of generators.
  /*!
    The polyhedron inherits the space dimension of the generator system.

    \param gs
    The system of generators defining the polyhedron.  It is not
    declared <CODE>const</CODE> because its data-structures may be
    recycled to build the polyhedron.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points,
    or if it contains closure points.
  */
  C_Polyhedron(Generator_System& gs, Recycle_Input dummy);

  //! Builds a C polyhedron from a system of congruences.
  /*!
    The polyhedron inherits the space dimension of the congruence system.

    \param cgs
    The system of congruences defining the polyhedron.
  */
  explicit C_Polyhedron(const Congruence_System& cgs);

  //! Builds a C polyhedron recycling a system of congruences.
  /*!
    The polyhedron inherits the space dimension of the congruence
    system.

    \param cgs
    The system of congruences defining the polyhedron.  It is not
    declared <CODE>const</CODE> because its data-structures may be
    recycled to build the polyhedron.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.
  */
  C_Polyhedron(Congruence_System& cgs, Recycle_Input dummy);

  /*! \brief
    Builds a C polyhedron representing the topological closure
    of the NNC polyhedron \p y.

    \param y
    The NNC polyhedron to be used;

    \param complexity
    This argument is ignored.
  */
  explicit C_Polyhedron(const NNC_Polyhedron& y,
                        Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a C polyhedron out of a box.
  /*!
    The polyhedron inherits the space dimension of the box
    and is the most precise that includes the box.
    The algorithm used has polynomial complexity.

    \param box
    The box representing the polyhedron to be approximated;

    \param complexity
    This argument is ignored.

    \exception std::length_error
    Thrown if the space dimension of \p box exceeds the maximum allowed
    space dimension.
  */
  template <typename Interval>
  explicit C_Polyhedron(const Box<Interval>& box,
                        Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a C polyhedron out of a BD shape.
  /*!
    The polyhedron inherits the space dimension of the BDS and is
    the most precise that includes the BDS.

    \param bd
    The BDS used to build the polyhedron.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.
  */
  template <typename U>
  explicit C_Polyhedron(const BD_Shape<U>& bd,
                        Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a C polyhedron out of an octagonal shape.
  /*!
    The polyhedron inherits the space dimension of the octagonal shape
    and is the most precise that includes the octagonal shape.

    \param os
    The octagonal shape used to build the polyhedron.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.
  */
  template <typename U>
  explicit C_Polyhedron(const Octagonal_Shape<U>& os,
                        Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a C polyhedron out of a grid.
  /*!
    The polyhedron inherits the space dimension of the grid
    and is the most precise that includes the grid.

    \param grid
    The grid used to build the polyhedron.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.
  */
  explicit C_Polyhedron(const Grid& grid,
                        Complexity_Class complexity = ANY_COMPLEXITY);

  //! Ordinary copy constructor.
  /*!
    The complexity argument is ignored.
  */
  C_Polyhedron(const C_Polyhedron& y,
               Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    The assignment operator.
    (\p *this and \p y can be dimension-incompatible.)
  */
  C_Polyhedron& operator=(const C_Polyhedron& y);

  //! Assigns to \p *this the topological closure of the NNC polyhedron \p y.
  C_Polyhedron& operator=(const NNC_Polyhedron& y);

  //! Destructor.
  ~C_Polyhedron();

  /*! \brief
    If the poly-hull of \p *this and \p y is exact it is assigned
    to \p *this and <CODE>true</CODE> is returned,
    otherwise <CODE>false</CODE> is returned.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool poly_hull_assign_if_exact(const C_Polyhedron& y);

  //! Same as poly_hull_assign_if_exact(y).
  bool upper_bound_assign_if_exact(const C_Polyhedron& y);
};

#include "C_Polyhedron.inlines.hh"

#endif // !defined(PPL_C_Polyhedron_defs_hh)
