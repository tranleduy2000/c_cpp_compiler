/* XSB Prolog Common Foreign Language Interface.
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

#ifndef PCFLI_xsb_cfli_hh
#define PCFLI_xsb_cfli_hh 1

#include <cinterf.h>

// In XSB versions up to and including 2.7.1, <error_xsb.h> does not
// come with the extern "C" wrapper.
extern "C" {
#include <error_xsb.h>
}

#include <cassert>

typedef prolog_term Prolog_term_ref;
typedef char* Prolog_atom;
typedef xsbBool Prolog_foreign_return_type;

const Prolog_foreign_return_type PROLOG_SUCCESS = TRUE;
const Prolog_foreign_return_type PROLOG_FAILURE = FALSE;

// XSB people claim XSB supports 32-bit integers.  However, experiments
// suggest this is not the case.
// See http://sourceforge.net/tracker/index.php?func=detail&aid=1400271&group_id=1176&atid=101176
#if 0
#define XSB_min_integer (-2147483647-1)
#define XSB_max_integer  2147483647
#else
#define XSB_min_integer -268435456
#define XSB_max_integer  268435455
#endif

/*!
  Return a new term reference.
*/
inline Prolog_term_ref
Prolog_new_term_ref() {
  return p2p_new();
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
  assert(is_var(t) == TRUE);
  if (l < XSB_min_integer || l > XSB_max_integer)
    return 0;
  return c2p_int(l, t) != FALSE;
}

/*!
  Assign to \p t a Prolog integer with value \p ul.
*/
inline int
Prolog_put_ulong(Prolog_term_ref& t, unsigned long ul) {
  assert(is_var(t) == TRUE);
  if (ul > static_cast<unsigned long>(XSB_max_integer))
    return 0;
  return c2p_int(ul, t) != FALSE;
}

/*!
  Assign to \p t an atom whose name is given
  by the null-terminated string \p s.
*/
inline int
Prolog_put_atom_chars(Prolog_term_ref& t, const char* s) {
  assert(is_var(t) == TRUE);
  return c2p_string(string_find(const_cast<char*>(s), 1), t) != FALSE;
}

/*!
  Assign to \p t the Prolog atom \p a.
*/
inline int
Prolog_put_atom(Prolog_term_ref& t, Prolog_atom a) {
  assert(is_var(t) == TRUE);
  return c2p_string(a, t) != FALSE;
}

/*!
  Assign to \p t a term representing the address contained in \p p.
*/
inline int
Prolog_put_address(Prolog_term_ref& t, void* p) {
  assert(is_var(t) == TRUE);
  return c2p_int(reinterpret_cast<long>(p), t) != FALSE;
}

/*!
  Return an atom whose name is given by the null-terminated string \p s.
*/
inline Prolog_atom
Prolog_atom_from_string(const char* s) {
  return string_find(const_cast<char*>(s), 1);
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 1 with argument \p a1.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1) {
  prolog_term new_compound = p2p_new();
  c2p_functor(f, 1, new_compound);
  p2p_unify(p2p_arg(new_compound, 1), a1);
  t = new_compound;
  return 1;
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 2 with arguments \p a1 and \p a2.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2) {
  prolog_term new_compound = p2p_new();
  c2p_functor(f, 2, new_compound);
  p2p_unify(p2p_arg(new_compound, 1), a1);
  p2p_unify(p2p_arg(new_compound, 2), a2);
  t = new_compound;
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
  prolog_term new_compound = p2p_new();
  c2p_functor(f, 3, new_compound);
  p2p_unify(p2p_arg(new_compound, 1), a1);
  p2p_unify(p2p_arg(new_compound, 2), a2);
  p2p_unify(p2p_arg(new_compound, 3), a3);
  t = new_compound;
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
  prolog_term new_compound = p2p_new();
  c2p_functor(f, 4, new_compound);
  p2p_unify(p2p_arg(new_compound, 1), a1);
  p2p_unify(p2p_arg(new_compound, 2), a2);
  p2p_unify(p2p_arg(new_compound, 3), a3);
  p2p_unify(p2p_arg(new_compound, 4), a4);
  t = new_compound;
  return 1;
}

/*!
  Assign to \p c a Prolog list whose head is \p h and tail is \p t.
*/
inline int
Prolog_construct_cons(Prolog_term_ref& c,
		      Prolog_term_ref h, Prolog_term_ref t) {
  prolog_term new_cons = p2p_new();
  c2p_list(new_cons);
  p2p_unify(p2p_car(new_cons), h);
  p2p_unify(p2p_cdr(new_cons), t);
  c = new_cons;
  return 1;
}

/*!
  Raise a Prolog exception with \p t as the exception term.
*/
inline void
Prolog_raise_exception(Prolog_term_ref t) {
  xsb_throw(t);
}

/*!
  Return true if \p t is a Prolog variable, false otherwise.
*/
inline int
Prolog_is_variable(Prolog_term_ref t) {
  return is_var(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog atom, false otherwise.
*/
inline int
Prolog_is_atom(Prolog_term_ref t) {
  return is_string(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog integer, false otherwise.
*/
inline int
Prolog_is_integer(Prolog_term_ref t) {
  return is_int(t) != FALSE;
}

/*!
  Return true if \p t is the representation of an address, false otherwise.
*/
inline int
Prolog_is_address(Prolog_term_ref t) {
  return is_int(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog compound term, false otherwise.
*/
inline int
Prolog_is_compound(Prolog_term_ref t) {
  return is_functor(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog cons (list constructor), false otherwise.
*/
inline int
Prolog_is_cons(Prolog_term_ref t) {
  return is_list(t) != FALSE;
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
  *lp = p2c_int(t);
  return 1;
}

/*!
  If \p t is the Prolog representation for a memory address, return
  true and store that address into \p v; return false otherwise.
  The behavior is undefined if \p t is not an address.
*/
inline int
Prolog_get_address(Prolog_term_ref t, void** vpp) {
  assert(Prolog_is_address(t));
  *vpp = reinterpret_cast<void*>(p2c_int(t));
  return 1;
}

/*!
  If \p t is a Prolog atom, return true and store its name into \p name.
  The behavior is undefined if \p t is not a Prolog atom.
*/
inline int
Prolog_get_atom_name(Prolog_term_ref t, Prolog_atom* ap) {
  assert(Prolog_is_atom(t));
  *ap = p2c_string(t);
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
  *ap = p2c_functor(t);
  *ip = p2c_arity(t);
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
  a = p2p_arg(t, i);
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
  h = p2p_car(c);
  t = p2p_cdr(c);
  return 1;
}

/*!
  Unify the terms referenced by \p t and \p u and return true
  if the unification is successful; return false otherwise.
*/
inline int
Prolog_unify(Prolog_term_ref t, Prolog_term_ref u) {
  return p2p_unify(t, u) != FALSE;
}



#endif // !defined(PCFLI_xsb_cfli_hh)
