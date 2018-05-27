/* Scalar_Products class implementation (non-inline functions).
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

#include "ppl-config.h"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "Coefficient_defs.hh"

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Scalar_Products::assign(Coefficient& z,
                             const Linear_Expression& x,
                             const Linear_Expression& y) {
  x.scalar_product_assign(z, y);
}

void
PPL::Scalar_Products::assign(Coefficient& z,
                             const Constraint& c, const Generator& g) {
  assign(z, c.expr, g.expr);
}

void
PPL::Scalar_Products::assign(Coefficient& z,
                             const Generator& g, const Constraint& c) {
  assign(z, g.expr, c.expr);
}

void
PPL::Scalar_Products::assign(Coefficient& z,
                             const Grid_Generator& gg, const Congruence& cg) {
  gg.expr.scalar_product_assign(z, cg.expr, 0, gg.space_dimension() + 1);
}

void
PPL::Scalar_Products::assign(Coefficient& z,
                             const Constraint& c,
                             const Grid_Generator& gg) {
  assign(z, c.expr, gg.expr);
}

void
PPL::Scalar_Products::assign(Coefficient& z,
                             const Congruence& cg, const Grid_Generator& gg) {
  // Scalar product is only defined if `cg' and `gg' are
  // dimension-compatible.
  PPL_ASSERT(cg.space_dimension() <= gg.space_dimension());
  cg.expr.scalar_product_assign(z, gg.expr);
}

void
PPL::Scalar_Products::reduced_assign(Coefficient& z,
                                     const Linear_Expression& x,
                                     const Linear_Expression& y) {
  // The reduced scalar product is only defined
  // if `y' has enough coefficients.
  PPL_ASSERT(x.space_dimension() - 1 <= y.space_dimension());
  x.scalar_product_assign(z, y, 0, x.space_dimension());
}

void
PPL::Scalar_Products::reduced_assign(Coefficient& z,
                                     const Grid_Generator& gg,
                                     const Congruence& cg) {
  // The reduced scalar product is only defined
  // if `cg' has enough coefficients.
  PPL_ASSERT(gg.space_dimension() <= cg.space_dimension());
  gg.expr.scalar_product_assign(z, cg.expr, 0, gg.space_dimension());
}

void
PPL::Scalar_Products::homogeneous_assign(Coefficient& z,
                                         const Linear_Expression& x,
                                         const Linear_Expression& y) {
  // Scalar product is only defined  if `x' and `y' are
  // dimension-compatible.
  PPL_ASSERT(x.space_dimension() <= y.space_dimension());
  x.scalar_product_assign(z, y, 1, x.space_dimension() + 1);
}

void
PPL::Scalar_Products::homogeneous_assign(Coefficient& z,
                                         const Grid_Generator& gg,
                                         const Congruence& cg) {
  // Scalar product is only defined if `gg' and `cg' are
  // dimension-compatible.
  PPL_ASSERT(gg.space_dimension() <= cg.space_dimension());
  gg.expr.scalar_product_assign(z, cg.expr, 1, gg.space_dimension() + 1);
}

void
PPL::Scalar_Products::homogeneous_assign(Coefficient& z,
                                         const Grid_Generator& gg,
                                         const Constraint& c) {
  // Scalar product is only defined if `gg' and `c' are
  // dimension-compatible.
  PPL_ASSERT(gg.space_dimension() <= c.space_dimension());
  gg.expr.scalar_product_assign(z, c.expr, 1, gg.space_dimension() + 1);
}
