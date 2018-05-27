/* Scalar_Products class implementation (inline functions).
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Scalar_Products_inlines_hh
#define PPL_Scalar_Products_inlines_hh 1

#include "Linear_Expression_defs.hh"
#include "Constraint_defs.hh"
#include "Generator_defs.hh"
#include "Congruence_defs.hh"
#include "Grid_Generator_defs.hh"

namespace Parma_Polyhedra_Library {

inline int
Scalar_Products::sign(const Linear_Expression& x, const Linear_Expression& y) {
  PPL_DIRTY_TEMP_COEFFICIENT(z);
  assign(z, x, y);
  return sgn(z);
}

inline int
Scalar_Products::reduced_sign(const Linear_Expression& x,
                              const Linear_Expression& y) {
  PPL_DIRTY_TEMP_COEFFICIENT(z);
  reduced_assign(z, x, y);
  return sgn(z);
}

inline int
Scalar_Products::homogeneous_sign(const Linear_Expression& x,
                                  const Linear_Expression& y) {
  PPL_DIRTY_TEMP_COEFFICIENT(z);
  homogeneous_assign(z, x, y);
  return sgn(z);
}

inline int
Scalar_Products::sign(const Constraint& c, const Generator& g) {
  return sign(c.expr, g.expr);
}

inline int
Scalar_Products::sign(const Generator& g, const Constraint& c) {
  return sign(g.expr, c.expr);
}

inline int
Scalar_Products::sign(const Constraint& c, const Grid_Generator& g) {
  PPL_DIRTY_TEMP_COEFFICIENT(z);
  assign(z, c, g);
  return sgn(z);
}

inline int
Scalar_Products::reduced_sign(const Constraint& c, const Generator& g) {
  // The reduced scalar product is only defined if the topology of `c' is
  // NNC.
  PPL_ASSERT(!c.is_necessarily_closed());
  return reduced_sign(c.expr, g.expr);
}

inline int
Scalar_Products::reduced_sign(const Generator& g, const Constraint& c) {
  // The reduced scalar product is only defined if the topology of `g' is
  // NNC.
  PPL_ASSERT(!c.is_necessarily_closed());
  return reduced_sign(g.expr, c.expr);
}

inline void
Scalar_Products::homogeneous_assign(Coefficient& z,
                                    const Linear_Expression& e,
                                    const Generator& g) {
  homogeneous_assign(z, e, g.expr);
}

inline void
Scalar_Products::homogeneous_assign(Coefficient& z,
                                    const Linear_Expression& e,
                                    const Grid_Generator& g) {
  homogeneous_assign(z, e, g.expr);
}

inline int
Scalar_Products::homogeneous_sign(const Linear_Expression& e,
                                  const Generator& g) {
  return homogeneous_sign(e, g.expr);
}

inline int
Scalar_Products::homogeneous_sign(const Linear_Expression& e,
                                  const Grid_Generator& g) {
  return homogeneous_sign(e, g.expr);
}

inline int
Scalar_Products::homogeneous_sign(const Grid_Generator& g,
                                  const Constraint& c) {
  PPL_DIRTY_TEMP_COEFFICIENT(z);
  homogeneous_assign(z, g, c);
  return sgn(z);
}

inline
Topology_Adjusted_Scalar_Product_Sign
::Topology_Adjusted_Scalar_Product_Sign(const Constraint& c)
  : sps_fp(c.is_necessarily_closed()
           ? static_cast<SPS_type>(&Scalar_Products::sign)
           : static_cast<SPS_type>(&Scalar_Products::reduced_sign)) {
}

inline
Topology_Adjusted_Scalar_Product_Sign
::Topology_Adjusted_Scalar_Product_Sign(const Generator& g)
  : sps_fp(g.is_necessarily_closed()
           ? static_cast<SPS_type>(&Scalar_Products::sign)
           : static_cast<SPS_type>(&Scalar_Products::reduced_sign)) {
}

inline int
Topology_Adjusted_Scalar_Product_Sign::operator()(const Constraint& c,
                                                  const Generator& g) const {
  PPL_ASSERT(c.space_dimension() <= g.space_dimension());
  PPL_ASSERT(sps_fp == (c.is_necessarily_closed()
                    ? static_cast<SPS_type>(&Scalar_Products::sign)
                    : static_cast<SPS_type>(&Scalar_Products::reduced_sign)));
  return sps_fp(c.expr, g.expr);
}

inline int
Topology_Adjusted_Scalar_Product_Sign::operator()(const Generator& g,
                                                  const Constraint& c) const {
  PPL_ASSERT(g.space_dimension() <= c.space_dimension());
  PPL_ASSERT(sps_fp == (g.is_necessarily_closed()
                    ? static_cast<SPS_type>(&Scalar_Products::sign)
                    : static_cast<SPS_type>(&Scalar_Products::reduced_sign)));
  return sps_fp(g.expr, c.expr);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Scalar_Products_inlines_hh)
