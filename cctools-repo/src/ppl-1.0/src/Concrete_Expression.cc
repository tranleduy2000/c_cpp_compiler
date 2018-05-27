/* Concrete_Expression class implementation (non-inline functions).
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

#include "ppl-config.h"
#include "Concrete_Expression.defs.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

bool
PPL::Concrete_Expression_Type::OK() const {
  if (impl.bounded_integer) {
    if (impl.bounded_integer_type_width != BITS_8
        && impl.bounded_integer_type_width != BITS_16
        && impl.bounded_integer_type_width != BITS_32
        && impl.bounded_integer_type_width != BITS_64
        && impl.bounded_integer_type_width != BITS_128) {
#ifndef NDEBUG
      std::cerr << "bounded integer type has illegal width "
                << impl.bounded_integer_type_width
                << std::endl;
#endif
      return false;
    }

    if (impl.bounded_integer_type_representation != UNSIGNED
        && impl.bounded_integer_type_representation != SIGNED_2_COMPLEMENT) {
#ifndef NDEBUG
      std::cerr << "bounded integer type has illegal representation "
                << impl.bounded_integer_type_representation
                << std::endl;
#endif
      return false;
    }

    if (impl.bounded_integer_type_overflow != OVERFLOW_WRAPS
        && impl.bounded_integer_type_overflow != OVERFLOW_UNDEFINED
        && impl.bounded_integer_type_overflow != OVERFLOW_IMPOSSIBLE) {
#ifndef NDEBUG
      std::cerr << "bounded integer type has illegal overflow "
                << impl.bounded_integer_type_overflow
                << std::endl;
#endif
      return false;
    }

    if (impl.floating_point_format != IEEE754_HALF) {
#ifndef NDEBUG
      std::cerr << "bounded integer type has illegal (unused) fp format "
                << impl.floating_point_format
                << std::endl;
#endif
      return false;
    }
  }
  else {
    // Here we have impl.bounded_integer == false.
    if (impl.floating_point_format != IEEE754_HALF
        && impl.floating_point_format != IEEE754_SINGLE
        && impl.floating_point_format != IEEE754_DOUBLE
        && impl.floating_point_format != IBM_SINGLE
        && impl.floating_point_format != IEEE754_QUAD
        && impl.floating_point_format != INTEL_DOUBLE_EXTENDED) {
#ifndef NDEBUG
      std::cerr << "floating point type has illegal format "
                << impl.floating_point_format
                << std::endl;
#endif
      return false;
    }

    if (impl.bounded_integer_type_width != BITS_128) {
#ifdef NDEBUG
      std::cerr << "floating point type has illegal (unused) bi width "
                << impl.bounded_integer_type_width
                << std::endl;
#endif
      return false;
    }

    if (impl.bounded_integer_type_representation != SIGNED_2_COMPLEMENT) {
#ifndef NDEBUG
      std::cerr << "floating point type has illegal (unused) bi representation "
                << impl.bounded_integer_type_representation
                << std::endl;
#endif
      return false;
    }

    if (impl.bounded_integer_type_overflow != OVERFLOW_IMPOSSIBLE) {
#ifndef NDEBUG
      std::cerr << "floating point type has illegal (unused) bi overflow "
                << impl.bounded_integer_type_overflow
                << std::endl;
#endif
      return false;
    }
  }

  // If we got here, everything is OK.
  return true;
}
