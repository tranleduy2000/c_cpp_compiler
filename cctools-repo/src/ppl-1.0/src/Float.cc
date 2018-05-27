/* IEC 559 floating point format related functions.
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
#include "Float.defs.hh"

namespace Parma_Polyhedra_Library {

const uint64_t float_intel_double_extended::LSP_INF;
const uint64_t float_intel_double_extended::LSP_ZERO;
const uint64_t float_intel_double_extended::LSP_DMAX;
const uint64_t float_intel_double_extended::LSP_NMAX;

const uint64_t float_ieee754_quad::MSP_SGN_MASK;
const uint64_t float_ieee754_quad::MSP_POS_INF;
const uint64_t float_ieee754_quad::MSP_NEG_INF;
const uint64_t float_ieee754_quad::MSP_POS_ZERO;
const uint64_t float_ieee754_quad::MSP_NEG_ZERO;
const uint64_t float_ieee754_quad::LSP_INF;
const uint64_t float_ieee754_quad::LSP_ZERO;
const uint64_t float_ieee754_quad::LSP_MAX;

} // Parma_Polyhedra_Library
