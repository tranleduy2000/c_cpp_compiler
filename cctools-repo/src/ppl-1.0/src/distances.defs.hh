/* Class declarations for several distances.
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

#ifndef PPL_distances_defs_hh
#define PPL_distances_defs_hh 1

#include "distances.types.hh"
#include "Rounding_Dir.defs.hh"

template <typename Temp>
struct Parma_Polyhedra_Library::Rectilinear_Distance_Specialization {
  static void combine(Temp& running, const Temp& current, Rounding_Dir dir);

  static void finalize(Temp&, Rounding_Dir);
};

template <typename Temp>
struct Parma_Polyhedra_Library::Euclidean_Distance_Specialization {
  static void combine(Temp& running, Temp& current, Rounding_Dir dir);

  static void finalize(Temp& running, Rounding_Dir dir);
};


template <typename Temp>
struct Parma_Polyhedra_Library::L_Infinity_Distance_Specialization {
  static void combine(Temp& running, const Temp& current, Rounding_Dir);

  static void finalize(Temp&, Rounding_Dir);
};

#include "distances.inlines.hh"

#endif // !defined(PPL_distances_defs_hh)
