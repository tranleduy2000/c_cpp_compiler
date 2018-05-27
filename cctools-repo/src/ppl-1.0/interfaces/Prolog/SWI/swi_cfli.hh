/* SWI-Prolog Common Foreign Language Interface.
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

#ifndef PCFLI_swi_cfli_hh
#define PCFLI_swi_cfli_hh 1

/* Include gmp.h before SWI-Prolog.h.  This is required in order to
   get access to interface functions dealing with GMP numbers and
   SWI-Prolog terms.  */
#include <gmp.h>
#include <SWI-Prolog.h>
#include <cassert>
#include <climits>

typedef term_t Prolog_term_ref;
typedef atom_t Prolog_atom;
typedef foreign_t Prolog_foreign_return_type;

const Prolog_foreign_return_type PROLOG_SUCCESS = TRUE;
const Prolog_foreign_return_type PROLOG_FAILURE = FALSE;

/*!
  Return a new term reference.
*/
inline Prolog_term_ref
Prolog_new_term_ref() {
  return PL_new_term_ref();
}

/*!
  Make \p t be a reference to the same term referenced by \p u,
  i.e., assign \p u to \p t.
*/
inline int
Prolog_put_term(Prolog_term_ref t, Prolog_term_ref u) {
#if PLVERSION >= 50800
  return PL_put_term(t, u);
#else
  PL_put_term(t, u);
  return 1;
#endif
}

/*!
  Assign to \p t a Prolog integer with value \p l.
*/
inline int
Prolog_put_long(Prolog_term_ref t, long l) {
#if PLVERSION >= 50800
  return PL_put_integer(t, l);
#else
  PL_put_integer(t, l);
  return 1;
#endif
}

static int tmp_mpz_t_initialized = 0;
static mpz_t tmp_mpz_t;

/*!
  Assign to \p t a Prolog integer with value \p ul.
*/
static int
Prolog_put_big_ulong(Prolog_term_ref t, unsigned long ul) {
  assert(ul > LONG_MAX && ul > (uint64_t) INT64_MAX);
  if (!tmp_mpz_t_initialized) {
    mpz_init_set_ui(tmp_mpz_t, ul);
    tmp_mpz_t_initialized = 1;
  }
  else
    mpz_set_ui(tmp_mpz_t, ul);
  return PL_unify_mpz(t, tmp_mpz_t);
}

/*!
  Assign to \p t a Prolog integer with value \p ul.
*/
inline int
Prolog_put_ulong(Prolog_term_ref t, unsigned long ul) {
  if (ul <= LONG_MAX) {
#if PLVERSION >= 50800
    return PL_put_integer(t, ul);
#else
    PL_put_integer(t, ul);
    return 1;
#endif
  }
  else if (ul <= (uint64_t) INT64_MAX) {
#if PLVERSION >= 50800
    return PL_put_int64(t, (int64_t) ul);
#else
    PL_put_int64(t, (int64_t) ul);
    return 1;
#endif
  }
  else
    return Prolog_put_big_ulong(t, ul);
}

/*!
  Assign to \p t an atom whose name is given
  by the null-terminated string \p s.
*/
inline int
Prolog_put_atom_chars(Prolog_term_ref t, const char* s) {
#if PLVERSION >= 50800
  return PL_put_atom_chars(t, s);
#else
  PL_put_atom_chars(t, s);
  return 1;
#endif
}

/*!
  Assign to \p t the Prolog atom \p a.
*/
inline int
Prolog_put_atom(Prolog_term_ref t, Prolog_atom a) {
#if PLVERSION >= 50800
  return PL_put_atom(t, a);
#else
  PL_put_atom(t, a);
  return 1;
#endif
}

/*!
  Assign to \p t a term representing the address contained in \p p.
*/
inline int
Prolog_put_address(Prolog_term_ref t, void* p) {
#if PLVERSION >= 50800
  return PL_put_pointer(t, p);
#else
  PL_put_pointer(t, p);
  return 1;
#endif
}

/*!
  Return an atom whose name is given by the null-terminated string \p s.
*/
inline Prolog_atom
Prolog_atom_from_string(const char* s) {
  return PL_new_atom(s);
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 1 with argument \p a1.
*/
inline int
Prolog_construct_compound(Prolog_term_ref t, Prolog_atom f,
			  Prolog_term_ref a1) {
#if PLVERSION >= 50800
  return PL_cons_functor(t, PL_new_functor(f, 1), a1);
#else
  PL_cons_functor(t, PL_new_functor(f, 1), a1);
  return 1;
#endif
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 2 with arguments \p a1 and \p a2.
*/
inline int
Prolog_construct_compound(Prolog_term_ref t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2) {
#if PLVERSION >= 50800
  return PL_cons_functor(t, PL_new_functor(f, 2), a1, a2);
#else
  PL_cons_functor(t, PL_new_functor(f, 2), a1, a2);
  return 1;
#endif
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 3 with arguments \p a1, \p a2 and \p a3.
*/
inline int
Prolog_construct_compound(Prolog_term_ref t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2,
			  Prolog_term_ref a3) {
#if PLVERSION >= 50800
  return PL_cons_functor(t, PL_new_functor(f, 3), a1, a2, a3);
#else
  PL_cons_functor(t, PL_new_functor(f, 3), a1, a2, a3);
  return 1;
#endif
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 4 with arguments \p a1, \p a2, \p a3 and \p a4.
*/
inline int
Prolog_construct_compound(Prolog_term_ref t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2,
			  Prolog_term_ref a3, Prolog_term_ref a4) {
#if PLVERSION >= 50800
  return PL_cons_functor(t, PL_new_functor(f, 4), a1, a2, a3, a4);
#else
  PL_cons_functor(t, PL_new_functor(f, 4), a1, a2, a3, a4);
  return 1;
#endif
}

/*!
  Assign to \p c a Prolog list whose head is \p h and tail is \p t.
*/
inline int
Prolog_construct_cons(Prolog_term_ref c,
		      Prolog_term_ref h, Prolog_term_ref t) {
#if PLVERSION >= 50800
  return PL_cons_list(c, h, t);
#else
  PL_cons_list(c, h, t);
  return 1;
#endif
}

/*!
  Raise a Prolog exception with \p t as the exception term.
*/
inline void
Prolog_raise_exception(Prolog_term_ref t) {
  (void) PL_raise_exception(t);
}

/*!
  Return true if \p t is a Prolog variable, false otherwise.
*/
inline int
Prolog_is_variable(Prolog_term_ref t) {
  return PL_is_variable(t);
}

/*!
  Return true if \p t is a Prolog atom, false otherwise.
*/
inline int
Prolog_is_atom(Prolog_term_ref t) {
  return PL_is_atom(t);
}

/*!
  Return true if \p t is a Prolog integer, false otherwise.
*/
inline int
Prolog_is_integer(Prolog_term_ref t) {
  return PL_is_integer(t);
}

/*!
  Return true if \p t is the representation of an address, false otherwise.
*/
inline int
Prolog_is_address(Prolog_term_ref t) {
  return PL_is_integer(t);
}

/*!
  Return true if \p t is a Prolog compound term, false otherwise.
*/
inline int
Prolog_is_compound(Prolog_term_ref t) {
  return PL_is_compound(t);
}

/*!
  Return true if \p t is a Prolog cons (list constructor), false otherwise.
*/
inline int
Prolog_is_cons(Prolog_term_ref t) {
  return (!PL_is_atom(t) && PL_is_list(t)) ? 1 : 0;
}

/*!
  Assuming \p t is a Prolog integer, return true if its value fits
  in a long, in which case the value is assigned to \p v,
  return false otherwise.  The behavior is undefined if \p t is
  not a Prolog integer.
*/
inline int
Prolog_get_long(Prolog_term_ref t, long* lp) {
  assert(Prolog_is_integer(t));
  return PL_get_long(t, lp);
}

/*!
  If \p t is the Prolog representation for a memory address, return
  true and store that address into to \p v; return false otherwise.
  The behavior is undefined if \p t is not an address.
*/
inline int
Prolog_get_address(Prolog_term_ref t, void** vpp) {
  assert(Prolog_is_address(t));
  return PL_get_pointer(t, vpp);
}

/*!
  If \p t is a Prolog atom, return true and store its name into \p name.
  The behavior is undefined if \p t is not a Prolog atom.
*/
inline int
Prolog_get_atom_name(Prolog_term_ref t, Prolog_atom* ap) {
  assert(Prolog_is_atom(t));
  return PL_get_atom(t, ap);
}

/*!
  If \p t is a Prolog compound term, return true and store its name
  and arity into \p name and \p arity, respectively.
  The behavior is undefined if \p t is not a Prolog compound term.
*/
inline int
Prolog_get_compound_name_arity(Prolog_term_ref t, Prolog_atom* ap, int* ip) {
  assert(Prolog_is_compound(t));
  return PL_get_name_arity(t, ap, ip);
}

/*!
  If \p t is a Prolog compound term and \p i is a positive integer
  less than or equal to its arity, return true and assign to \p a the
  i-th (principal) argument of \p t.
  The behavior is undefined if \p t is not a Prolog compound term.
*/
inline int
Prolog_get_arg(int i, Prolog_term_ref t, Prolog_term_ref a) {
  assert(Prolog_is_compound(t));
  return PL_get_arg(i, t, a);
}

/*!
  If \p c is a Prolog cons (list constructor), assign its head and
  tail to \p h and \p t, respectively.
  The behavior is undefined if \p c is not a Prolog cons.
*/
inline int
Prolog_get_cons(Prolog_term_ref c, Prolog_term_ref h, Prolog_term_ref t) {
  assert(Prolog_is_cons(c));
  return PL_get_list(c, h, t);
}

/*!
  Unify the terms referenced by \p t and \p u and return true
  if the unification is successful; return false otherwise.
*/
inline int
Prolog_unify(Prolog_term_ref t, Prolog_term_ref u) {
  return PL_unify(t, u);
}

#endif // !defined(PCFLI_swi_cfli_hh)
