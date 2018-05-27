/* YAP extended foreign language interface: definitions.
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

#include "yap_efli.hh"

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace Prolog {

namespace YAP {

Prolog_atom a_throw;

bool Prolog_has_unbounded_integers;

long Prolog_min_integer;

long Prolog_max_integer;

mpz_class tmp_mpz_class;

void
ppl_Prolog_sysdep_init() {
  Prolog_has_unbounded_integers = true;
  Prolog_min_integer = 0;
  Prolog_max_integer = 0;

  a_throw = YAP_LookupAtom("throw");
}

void
ppl_Prolog_sysdep_deinit() {
}

int
Prolog_get_Coefficient(Prolog_term_ref t, Coefficient& n) {
  assert(Prolog_is_integer(t));
  if (YAP_IsBigNumTerm(t) != FALSE) {
    YAP_BigNumOfTerm(t, tmp_mpz_class.get_mpz_t());
    n = tmp_mpz_class;
  }
  else
    n = YAP_IntOfTerm(t);
  return 1;
}

int
Prolog_put_Coefficient(Prolog_term_ref& t, const Coefficient& n) {
  if (n >= LONG_MIN && n <= LONG_MAX) {
    long l = 0;
    assign_r(l, n, ROUND_NOT_NEEDED);
    t = YAP_MkIntTerm(l);
  }
  else {
    assign_r(tmp_mpz_class, n, ROUND_NOT_NEEDED);
    t = YAP_MkBigNumTerm(tmp_mpz_class.get_mpz_t());
  }
  return 1;
}

int
Prolog_unify_Coefficient(Prolog_term_ref t, const Coefficient& n) {
  Prolog_term_ref u = Prolog_new_term_ref();
  return Prolog_put_Coefficient(u, n) && YAP_Unify(t, u);
}

} // namespace YAP

} // namespace Prolog

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library
