/* Polyhedron::Status class implementation (non-inline functions).
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
#include "Polyhedron.defs.hh"
#include "assert.hh"
#include <iostream>
#include <string>

namespace PPL = Parma_Polyhedra_Library;

namespace {

// These are the keywords that indicate the individual assertions.
const char* zero_dim_univ = "ZE";
const char* empty = "EM";
const char* consys_min = "CM";
const char* gensys_min = "GM";
const char* consys_upd = "CS";
const char* gensys_upd = "GS";
const char* satc_upd = "SC";
const char* satg_upd = "SG";
const char* consys_pending = "CP";
const char* gensys_pending = "GP";

/*! \relates Parma_Polyhedra_Library::Polyhedron::Status
  Reads a keyword and its associated on/off, +/- flag from \p s.
  Returns <CODE>true</CODE> if the operation is successful,
  returns <CODE>false</CODE> otherwise.
  When successful, \p positive is set to <CODE>true</CODE> if the flag
  is on; it is set to <CODE>false</CODE> otherwise.
*/
bool
get_field(std::istream& s, const char* keyword, bool& positive) {
  std::string str;
  if (!(s >> str)
      || (str[0] != '+' && str[0] != '-')
      || str.substr(1) != keyword)
    return false;
  positive = (str[0] == '+');
  return true;
}

} // namespace

void
PPL::Polyhedron::Status::ascii_dump(std::ostream& s) const {
  s << (test_zero_dim_univ() ? '+' : '-') << zero_dim_univ << ' '
    << (test_empty() ? '+' : '-') << empty << ' '
    << ' '
    << (test_c_minimized() ? '+' : '-') << consys_min << ' '
    << (test_g_minimized() ? '+' : '-') << gensys_min << ' '
    << ' '
    << (test_c_up_to_date() ? '+' : '-') << consys_upd << ' '
    << (test_g_up_to_date() ? '+' : '-') << gensys_upd << ' '
    << ' '
    << (test_c_pending() ? '+' : '-') << consys_pending << ' '
    << (test_g_pending() ? '+' : '-') << gensys_pending << ' '
    << ' '
    << (test_sat_c_up_to_date() ? '+' : '-') << satc_upd << ' '
    << (test_sat_g_up_to_date() ? '+' : '-') << satg_upd << ' ';
}

PPL_OUTPUT_DEFINITIONS_ASCII_ONLY(Polyhedron::Status)

bool
PPL::Polyhedron::Status::ascii_load(std::istream& s) {
  PPL_UNINITIALIZED(bool, positive);

  if (!get_field(s, zero_dim_univ, positive))
    return false;
  if (positive)
    set_zero_dim_univ();

  if (!get_field(s, empty, positive))
    return false;
  if (positive)
    set_empty();

  if (!get_field(s, consys_min, positive))
    return false;
  if (positive)
    set_c_minimized();
  else
    reset_c_minimized();

  if (!get_field(s, gensys_min, positive))
    return false;
  if (positive)
    set_g_minimized();
  else
    reset_g_minimized();

  if (!get_field(s, consys_upd, positive))
    return false;
  if (positive)
    set_c_up_to_date();
  else
    reset_c_up_to_date();

  if (!get_field(s, gensys_upd, positive))
    return false;
  if (positive)
    set_g_up_to_date();
  else
    reset_g_up_to_date();

  if (!get_field(s, consys_pending, positive))
    return false;
  if (positive)
    set_c_pending();
  else
    reset_c_pending();

  if (!get_field(s, gensys_pending, positive))
    return false;
  if (positive)
    set_g_pending();
  else
    reset_g_pending();

  if (!get_field(s, satc_upd, positive))
    return false;
  if (positive)
    set_sat_c_up_to_date();
  else
    reset_sat_c_up_to_date();

  if (!get_field(s, satg_upd, positive))
    return false;
  if (positive)
    set_sat_g_up_to_date();
  else
    reset_sat_g_up_to_date();

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

bool
PPL::Polyhedron::Status::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  if (test_zero_dim_univ())
    // Zero-dim universe is OK.
    return true;

  if (test_empty()) {
    Status copy = *this;
    copy.reset_empty();
    if (copy.test_zero_dim_univ())
      return true;
    else {
#ifndef NDEBUG
      cerr << "The empty flag is incompatible with any other one."
	   << endl;
#endif
      return false;
    }
  }

  if ((test_sat_c_up_to_date() || test_sat_g_up_to_date())
      && !(test_c_up_to_date() && test_g_up_to_date())) {
#ifndef NDEBUG
    cerr <<
      "If a saturation matrix is up-to-date, constraints and\n"
      "generators have to be both up-to-date."
	 << endl;
#endif
    return false;
  }

  if (test_c_minimized() && !test_c_up_to_date()) {
#ifndef NDEBUG
    cerr << "If constraints are minimized they must be up-to-date."
	 << endl;
#endif
    return false;
  }

  if (test_g_minimized() && !test_g_up_to_date()) {
#ifndef NDEBUG
    cerr << "If generators are minimized they must be up-to-date."
	 << endl;
#endif
    return false;
  }

  if (test_c_pending() && test_g_pending()) {
#ifndef NDEBUG
    cerr << "There cannot be both pending constraints and pending generators."
	 << endl;
#endif
    return false;
  }

  if (test_c_pending() || test_g_pending()) {
    if (!test_c_minimized() || !test_g_minimized()) {
#ifndef NDEBUG
    cerr <<
      "If there are pending constraints or generators, constraints\n"
      "and generators must be minimized."
	 << endl;
#endif
      return false;
    }

    if (!test_sat_c_up_to_date() && !test_sat_g_up_to_date()) {
#ifndef NDEBUG
    cerr <<
      "If there are pending constraints or generators, there must\n"
      "be at least a saturation matrix up-to-date."
	 << endl;
#endif
      return false;
    }
  }

  // Any other case is OK.
  return true;
}
