/* SICStus Prolog extended foreign language interface: definitions.
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

#include "sicstus_efli.hh"

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace Prolog {

namespace SICStus {

bool Prolog_has_unbounded_integers;

long Prolog_min_integer;

long Prolog_max_integer;

void
ppl_Prolog_sysdep_init() {
  Prolog_has_unbounded_integers = true;
  Prolog_min_integer = 0;
  Prolog_max_integer = 0;
}

void
ppl_Prolog_sysdep_deinit() {
}

int
Prolog_get_Coefficient(Prolog_term_ref t, Coefficient& n) {
  assert(SP_is_integer(t));
  long v;
  if (SP_get_integer(t, &v) == SP_SUCCESS)
    n = v;
  else {
#if SICSTUS_MAJOR_VERSION == 3
    char* s;
    if (SP_get_number_chars(t, &s) == SP_SUCCESS)
      n = Coefficient(s);
    else
      return SP_FAILURE;
#else
    const char* s;
    if (SP_get_number_codes(t, &s) == SP_SUCCESS)
      n = Coefficient(s);
    else
      return SP_FAILURE;
#endif
  }
  return SP_SUCCESS;
}

int
Prolog_put_Coefficient(Prolog_term_ref t, const Coefficient& n) {
  long l = 0;
  Result r = assign_r(l, n, ROUND_IGNORE);
  if (!result_overflow(r))
    return SP_put_integer(t, l);
  else {
    std::ostringstream s;
    s << n;
    std::string str = s.str();
#if SICSTUS_MAJOR_VERSION == 3
    return SP_put_number_chars(t, str.c_str());
#else
    return SP_put_number_codes(t, str.c_str());
#endif
  }
}

int
Prolog_unify_Coefficient(Prolog_term_ref t, const Coefficient& n) {
  Prolog_term_ref u = Prolog_new_term_ref();
  if (Prolog_put_Coefficient(u, n) == SP_SUCCESS)
    return SP_unify(t, u);
  else
    return SP_FAILURE;
}

} // namespace SICStus

} // namespace Prolog

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library
