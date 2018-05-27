/* Polyhedron::Status class implementation: inline functions.
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

#ifndef PPL_Status_inlines_hh
#define PPL_Status_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
Polyhedron::Status::Status(flags_t mask)
  : flags(mask) {
}

inline
Polyhedron::Status::Status()
  : flags(ZERO_DIM_UNIV) {
}

inline bool
Polyhedron::Status::test_all(flags_t mask) const {
  return (flags & mask) == mask;
}

inline bool
Polyhedron::Status::test_any(flags_t mask) const {
  return (flags & mask) != 0;
}

inline void
Polyhedron::Status::set(flags_t mask) {
  flags |= mask;
}

inline void
Polyhedron::Status::reset(flags_t mask) {
  flags &= ~mask;
}

inline bool
Polyhedron::Status::test_zero_dim_univ() const {
  return flags == ZERO_DIM_UNIV;
}

inline void
Polyhedron::Status::reset_zero_dim_univ() {
  // This is a no-op if the current status is not zero-dim.
  if (flags == ZERO_DIM_UNIV)
    // In the zero-dim space, if it is not the universe it is empty.
    flags = EMPTY;
}

inline void
Polyhedron::Status::set_zero_dim_univ() {
  // Zero-dim universe is incompatible with anything else.
  flags = ZERO_DIM_UNIV;
}

inline bool
Polyhedron::Status::test_empty() const {
  return test_any(EMPTY);
}

inline void
Polyhedron::Status::reset_empty() {
  reset(EMPTY);
}

inline void
Polyhedron::Status::set_empty() {
  flags = EMPTY;
}

inline bool
Polyhedron::Status::test_c_up_to_date() const {
  return test_any(C_UP_TO_DATE);
}

inline void
Polyhedron::Status::reset_c_up_to_date() {
  reset(C_UP_TO_DATE);
}

inline void
Polyhedron::Status::set_c_up_to_date() {
  set(C_UP_TO_DATE);
}

inline bool
Polyhedron::Status::test_g_up_to_date() const {
  return test_any(G_UP_TO_DATE);
}

inline void
Polyhedron::Status::reset_g_up_to_date() {
  reset(G_UP_TO_DATE);
}

inline void
Polyhedron::Status::set_g_up_to_date() {
  set(G_UP_TO_DATE);
}

inline bool
Polyhedron::Status::test_c_minimized() const {
  return test_any(C_MINIMIZED);
}

inline void
Polyhedron::Status::reset_c_minimized() {
  reset(C_MINIMIZED);
}

inline void
Polyhedron::Status::set_c_minimized() {
  set(C_MINIMIZED);
}

inline bool
Polyhedron::Status::test_g_minimized() const {
  return test_any(G_MINIMIZED);
}

inline void
Polyhedron::Status::reset_g_minimized() {
  reset(G_MINIMIZED);
}

inline void
Polyhedron::Status::set_g_minimized() {
  set(G_MINIMIZED);
}


inline bool
Polyhedron::Status::test_c_pending() const {
  return test_any(CS_PENDING);
}

inline void
Polyhedron::Status::reset_c_pending() {
  reset(CS_PENDING);
}

inline void
Polyhedron::Status::set_c_pending() {
  set(CS_PENDING);
}

inline bool
Polyhedron::Status::test_g_pending() const {
  return test_any(GS_PENDING);
}

inline void
Polyhedron::Status::reset_g_pending() {
  reset(GS_PENDING);
}

inline void
Polyhedron::Status::set_g_pending() {
  set(GS_PENDING);
}


inline bool
Polyhedron::Status::test_sat_c_up_to_date() const {
  return test_any(SAT_C_UP_TO_DATE);
}

inline void
Polyhedron::Status::reset_sat_c_up_to_date() {
  reset(SAT_C_UP_TO_DATE);
}

inline void
Polyhedron::Status::set_sat_c_up_to_date() {
  set(SAT_C_UP_TO_DATE);
}

inline bool
Polyhedron::Status::test_sat_g_up_to_date() const {
  return test_any(SAT_G_UP_TO_DATE);
}

inline void
Polyhedron::Status::reset_sat_g_up_to_date() {
  reset(SAT_G_UP_TO_DATE);
}

inline void
Polyhedron::Status::set_sat_g_up_to_date() {
  set(SAT_G_UP_TO_DATE);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Status_inlines_hh)
