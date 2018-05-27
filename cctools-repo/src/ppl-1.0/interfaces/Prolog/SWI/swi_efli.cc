/* SWI-Prolog extended foreign language interface: definitions.
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

#include "swi_efli.hh"

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace Prolog {

namespace SWI {

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
  assert(Prolog_is_integer(t));
  // FIXME: avoid the temporary when Coefficient is mpz_class.
  PPL_DIRTY_TEMP(mpz_class, tmp);
  int r;
#if PLVERSION >= 50800
  r = PL_get_mpz(t, tmp.get_mpz_t());
#else
  PL_get_mpz(t, tmp.get_mpz_t());
  r = 1;
#endif
  n = tmp;
  return r;
}

int
Prolog_unify_Coefficient(Prolog_term_ref t, const Coefficient& n) {
  PPL_DIRTY_TEMP(mpz_class, tmp);
  assign_r(tmp, n, ROUND_NOT_NEEDED);
  return PL_unify_mpz(t, tmp.get_mpz_t());
}

int
Prolog_put_Coefficient(Prolog_term_ref t, const Coefficient& n) {
  PL_put_variable(t);
  return Prolog_unify_Coefficient(t, n);
}

} // namespace SWI

} // namespace Prolog

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library
