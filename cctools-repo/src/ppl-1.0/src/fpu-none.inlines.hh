/* Null floating point unit related functions.
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

#ifndef PPL_fpu_none_inlines_hh
#define PPL_fpu_none_inlines_hh 1

#include <stdexcept>

namespace Parma_Polyhedra_Library {

inline void
fpu_initialize_control_functions() {
  throw std::logic_error("PPL::fpu_initialize_control_functions():"
			 " cannot control the FPU");
}

inline fpu_rounding_direction_type
fpu_get_rounding_direction() {
  throw std::logic_error("PPL::fpu_get_rounding_direction():"
			 " cannot control the FPU");
}

inline void
fpu_set_rounding_direction(int) {
  throw std::logic_error("PPL::fpu_set_rounding_direction():"
			 " cannot control the FPU");
}

inline int
fpu_save_rounding_direction(int) {
  throw std::logic_error("PPL::fpu_save_rounding_direction():"
			 " cannot control the FPU");
}

inline void
fpu_reset_inexact() {
  throw std::logic_error("PPL::fpu_reset_inexact():"
			 " cannot control the FPU");
}

inline void
fpu_restore_rounding_direction(int) {
  throw std::logic_error("PPL::fpu_restore_rounding_direction():"
			 " cannot control the FPU");
}

inline int
fpu_check_inexact() {
  throw std::logic_error("PPL::fpu_check_inexact():"
			 " cannot control the FPU");
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_fpu_none_inlines_hh)
