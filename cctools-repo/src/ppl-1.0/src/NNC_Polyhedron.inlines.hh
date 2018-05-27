/* NNC_Polyhedron class implementation: inline functions.
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

#ifndef PPL_NNC_Polyhedron_inlines_hh
#define PPL_NNC_Polyhedron_inlines_hh 1

#include "C_Polyhedron.defs.hh"

namespace Parma_Polyhedra_Library {

inline
NNC_Polyhedron::~NNC_Polyhedron() {
}

inline
NNC_Polyhedron::NNC_Polyhedron(dimension_type num_dimensions,
			       Degenerate_Element kind)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_space_dimension_overflow(num_dimensions,
                                              NOT_NECESSARILY_CLOSED,
                                              "NNC_Polyhedron(n, k)",
                                              "n exceeds the maximum "
                                              "allowed space dimension"),
	       kind) {
}

inline
NNC_Polyhedron::NNC_Polyhedron(const Constraint_System& cs)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_obj_space_dimension_overflow(cs, NOT_NECESSARILY_CLOSED,
                                                  "NNC_Polyhedron(cs)",
                                                  "the space dimension of cs "
                                                  "exceeds the maximum allowed "
                                                  "space dimension")) {
}

inline
NNC_Polyhedron::NNC_Polyhedron(Constraint_System& cs, Recycle_Input)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_obj_space_dimension_overflow(cs, NOT_NECESSARILY_CLOSED,
                                                  "NNC_Polyhedron(cs, recycle)",
                                                  "the space dimension of cs "
                                                  "exceeds the maximum allowed "
                                                  "space dimension"),
	       Recycle_Input()) {
}

inline
NNC_Polyhedron::NNC_Polyhedron(const Generator_System& gs)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_obj_space_dimension_overflow(gs, NOT_NECESSARILY_CLOSED,
                                                  "NNC_Polyhedron(gs)",
                                                  "the space dimension of gs "
                                                  "exceeds the maximum allowed "
                                                  "space dimension")) {
}

inline
NNC_Polyhedron::NNC_Polyhedron(Generator_System& gs, Recycle_Input)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_obj_space_dimension_overflow(gs, NOT_NECESSARILY_CLOSED,
                                                  "NNC_Polyhedron(gs, recycle)",
                                                  "the space dimension of gs "
                                                  "exceeds the maximum allowed "
                                                  "space dimension"),
	       Recycle_Input()) {
}

template <typename Interval>
inline
NNC_Polyhedron::NNC_Polyhedron(const Box<Interval>& box, Complexity_Class)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_obj_space_dimension_overflow(box, NOT_NECESSARILY_CLOSED,
                                                  "NNC_Polyhedron(box)",
                                                  "the space dimension of box "
                                                  "exceeds the maximum allowed "
                                                  "space dimension")) {
}

template <typename U>
inline
NNC_Polyhedron::NNC_Polyhedron(const BD_Shape<U>& bd, Complexity_Class)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_space_dimension_overflow(bd.space_dimension(),
                                              NOT_NECESSARILY_CLOSED,
                                              "NNC_Polyhedron(bd)",
                                              "the space dimension of bd "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
               UNIVERSE) {
  add_constraints(bd.constraints());
}

template <typename U>
inline
NNC_Polyhedron::NNC_Polyhedron(const Octagonal_Shape<U>& os, Complexity_Class)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_space_dimension_overflow(os.space_dimension(),
                                              NOT_NECESSARILY_CLOSED,
                                              "NNC_Polyhedron(os)",
                                              "the space dimension of os "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
               UNIVERSE) {
  add_constraints(os.constraints());
}

inline
NNC_Polyhedron::NNC_Polyhedron(const NNC_Polyhedron& y, Complexity_Class)
  : Polyhedron(y) {
}

inline NNC_Polyhedron&
NNC_Polyhedron::operator=(const NNC_Polyhedron& y) {
  Polyhedron::operator=(y);
  return *this;
}

inline NNC_Polyhedron&
NNC_Polyhedron::operator=(const C_Polyhedron& y) {
  NNC_Polyhedron nnc_y(y);
  m_swap(nnc_y);
  return *this;
}

inline bool
NNC_Polyhedron::upper_bound_assign_if_exact(const NNC_Polyhedron& y) {
  return poly_hull_assign_if_exact(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_NNC_Polyhedron_inlines_hh)
