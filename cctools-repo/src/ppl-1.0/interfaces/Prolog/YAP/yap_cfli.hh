/* YAP Prolog Common Foreign Language Interface.
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

#ifndef PCFLI_yap_cfli_hh
#define PCFLI_yap_cfli_hh 1

#include <Yap/YapInterface.h>
#include <cassert>
#include <climits>
#include <gmp.h>

typedef YAP_Term Prolog_term_ref;
typedef YAP_Atom Prolog_atom;
typedef YAP_Bool Prolog_foreign_return_type;

const Prolog_foreign_return_type PROLOG_SUCCESS = TRUE;
const Prolog_foreign_return_type PROLOG_FAILURE = FALSE;

/*!
  Return a new term reference.
*/
inline Prolog_term_ref
Prolog_new_term_ref() {
  return 0;
}

/*!
  Make \p t be a reference to the same term referenced by \p u,
  i.e., assign \p u to \p t.
*/
inline int
Prolog_put_term(Prolog_term_ref& t, Prolog_term_ref u) {
  t = u;
  return 1;
}

/*!
  Assign to \p t a Prolog integer with value \p l.
*/
inline int
Prolog_put_long(Prolog_term_ref& t, long l) {
  t = YAP_MkIntTerm(l);
  return 1;
}

static int tmp_mpz_t_initialized = 0;
static mpz_t tmp_mpz_t;

/*!
  Assign to \p t a Prolog integer with value \p ul.
*/
inline int
Prolog_put_ulong(Prolog_term_ref& t, unsigned long ul) {
  if (ul <= LONG_MAX)
    t = YAP_MkIntTerm(ul);
  else {
    if (!tmp_mpz_t_initialized) {
      mpz_init_set_ui(tmp_mpz_t, ul);
      tmp_mpz_t_initialized = 1;
    }
    else
      mpz_set_ui(tmp_mpz_t, ul);
    t = YAP_MkBigNumTerm(tmp_mpz_t);
  }
  return 1;
}

/*!
  Assign to \p t an atom whose name is given
  by the null-terminated string \p s.
*/
inline int
Prolog_put_atom_chars(Prolog_term_ref& t, const char* s) {
  t = YAP_MkAtomTerm(YAP_FullLookupAtom(s));
  return 1;
}

/*!
  Assign to \p t the Prolog atom \p a.
*/
inline int
Prolog_put_atom(Prolog_term_ref& t, Prolog_atom a) {
  t = YAP_MkAtomTerm(a);
  return 1;
}

/*!
  Assign to \p t a term representing the address contained in \p p.
*/
inline int
Prolog_put_address(Prolog_term_ref& t, void* p) {
  assert(sizeof(long) >= sizeof(void*));
  t = YAP_MkIntTerm(reinterpret_cast<long>(p));
  return 1;
}

/*!
  Return an atom whose name is given by the null-terminated string \p s.
*/
inline Prolog_atom
Prolog_atom_from_string(const char* s) {
  return YAP_FullLookupAtom(s);
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 1 with argument \p a1.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1) {
  YAP_Term args[1];
  args[0] = a1;
  t = YAP_MkApplTerm(YAP_MkFunctor(f, 1), 1, args);
  return 1;
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 2 with arguments \p a1 and \p a2.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2) {
  YAP_Term args[2];
  args[0] = a1;
  args[1] = a2;
  t = YAP_MkApplTerm(YAP_MkFunctor(f, 2), 2, args);
  return 1;
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 3 with arguments \p a1, \p a2 and \p a3.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2,
			  Prolog_term_ref a3) {
  YAP_Term args[3];
  args[0] = a1;
  args[1] = a2;
  args[2] = a3;
  t = YAP_MkApplTerm(YAP_MkFunctor(f, 3), 3, args);
  return 1;
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 4 with arguments \p a1, \p a2, \p a3 and \p a4.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2,
			  Prolog_term_ref a3, Prolog_term_ref a4) {
  YAP_Term args[4];
  args[0] = a1;
  args[1] = a2;
  args[2] = a3;
  args[3] = a4;
  t = YAP_MkApplTerm(YAP_MkFunctor(f, 4), 4, args);
  return 1;
}

/*!
  Assign to \p c a Prolog list whose head is \p h and tail is \p t.
*/
inline int
Prolog_construct_cons(Prolog_term_ref& c,
		      Prolog_term_ref h, Prolog_term_ref t) {
  c = YAP_MkPairTerm(h, t);
  return 1;
}

/*!
  Raise a Prolog exception with \p t as the exception term.
*/
inline void
Prolog_raise_exception(Prolog_term_ref t) {
  YAP_Throw(t);
}

/*!
  Return true if \p t is a Prolog variable, false otherwise.
*/
inline int
Prolog_is_variable(Prolog_term_ref t) {
  return YAP_IsVarTerm(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog atom, false otherwise.
*/
inline int
Prolog_is_atom(Prolog_term_ref t) {
  return YAP_IsAtomTerm(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog integer, false otherwise.
*/
inline int
Prolog_is_integer(Prolog_term_ref t) {
  return YAP_IsIntTerm(t) != FALSE || YAP_IsBigNumTerm(t) != FALSE;
}

/*!
  Return true if \p t is the representation of an address, false otherwise.
*/
inline int
Prolog_is_address(Prolog_term_ref t) {
  return YAP_IsIntTerm(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog compound term, false otherwise.
*/
inline int
Prolog_is_compound(Prolog_term_ref t) {
  return YAP_IsApplTerm(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog cons (list constructor), false otherwise.
*/
inline int
Prolog_is_cons(Prolog_term_ref t) {
  return YAP_IsPairTerm(t) != FALSE;
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
  if (YAP_IsBigNumTerm(t) != FALSE) {
    if (!tmp_mpz_t_initialized) {
      mpz_init(tmp_mpz_t);
      tmp_mpz_t_initialized = 1;
    }
    YAP_BigNumOfTerm(t, tmp_mpz_t);
    if (mpz_cmp_si(tmp_mpz_t, LONG_MIN) >= 0
	&& mpz_cmp_si(tmp_mpz_t, LONG_MAX) <= 0)
      *lp = mpz_get_si(tmp_mpz_t);
    else
      return 0;
  }
  else
    *lp = YAP_IntOfTerm(t);
  return 1;
}

/*!
  If \p t is the Prolog representation for a memory address, return
  true and store that address into to \p v; return false otherwise.
  The behavior is undefined if \p t is not an address.
*/
inline int
Prolog_get_address(Prolog_term_ref t, void** vpp) {
  assert(Prolog_is_address(t));
  *vpp = reinterpret_cast<void*>(YAP_IntOfTerm(t));
  return 1;
}

/*!
  If \p t is a Prolog atom, return true and store its name into \p name.
  The behavior is undefined if \p t is not a Prolog atom.
*/
inline int
Prolog_get_atom_name(Prolog_term_ref t, Prolog_atom* ap) {
  assert(Prolog_is_atom(t));
  *ap = YAP_AtomOfTerm(t);
  return 1;
}

/*!
  If \p t is a Prolog compound term, return true and store its name
  and arity into \p name and \p arity, respectively.
  The behavior is undefined if \p t is not a Prolog compound term.
*/
inline int
Prolog_get_compound_name_arity(Prolog_term_ref t, Prolog_atom* ap, int* ip) {
  assert(Prolog_is_compound(t));
  YAP_Functor f = YAP_FunctorOfTerm(t);
  *ap = YAP_NameOfFunctor(f);
  *ip = YAP_ArityOfFunctor(f);
  return 1;
}

/*!
  If \p t is a Prolog compound term and \p i is a positive integer
  less than or equal to its arity, return true and assign to \p a the
  i-th (principal) argument of \p t.
  The behavior is undefined if \p t is not a Prolog compound term.
*/
inline int
Prolog_get_arg(int i, Prolog_term_ref t, Prolog_term_ref& a) {
  assert(Prolog_is_compound(t));
  a = YAP_ArgOfTerm(i, t);
  return 1;
}

/*!
  If \p c is a Prolog cons (list constructor), assign its head and
  tail to \p h and \p t, respectively.
  The behavior is undefined if \p c is not a Prolog cons.
*/
inline int
Prolog_get_cons(Prolog_term_ref c, Prolog_term_ref& h, Prolog_term_ref& t) {
  assert(Prolog_is_cons(c));
  h = YAP_HeadOfTerm(c);
  t = YAP_TailOfTerm(c);
  return 1;
}

/*!
  Unify the terms referenced by \p t and \p u and return true
  if the unification is successful; return false otherwise.
*/
inline int
Prolog_unify(Prolog_term_ref t, Prolog_term_ref u) {
  return YAP_Unify(t, u) != FALSE;
}

#endif // !defined(PCFLI_yap_cfli_hh)
