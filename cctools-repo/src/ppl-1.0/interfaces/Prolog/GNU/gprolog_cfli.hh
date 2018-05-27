/* GNU Prolog Common Foreign Language Interface.
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

#ifndef PCFLI_gprolog_cfli_hh
#define PCFLI_gprolog_cfli_hh 1

#if SIZEOF_FP == SIZEOF_INTP
// Horrible kludge working around an horrible bug in <gprolog.h> (see
// http://www.cs.unipr.it/pipermail/ppl-devel/2008-August/012277.html).
#define byte_code byte_code(void)
#define last_read_line last_read_line(void)
#define last_read_col last_read_col(void)
#include <gprolog.h>
#undef byte_code
#undef last_read_line
#undef last_read_col
#else
#include <gprolog.h>
#endif

#if defined(__GPROLOG_VERSION__) && __GPROLOG_VERSION__ >= 10301
#define PPL_GPROLOG_H_IS_CLEAN
#endif

#ifndef PPL_GPROLOG_H_IS_CLEAN
// <gprolog.h> pollutes the namespace: try to clean up
// (see http://www.cs.unipr.it/pipermail/ppl-devel/2004-April/004270.html).
#ifdef B
#undef B
#endif
#ifdef H
#undef H
#endif
#ifdef CP
#undef CP
#endif
#ifdef E
#undef E
#endif
#ifdef CS
#undef CS
#endif
#ifdef S
#undef S
#endif
#ifdef STAMP
#undef STAMP
#endif
#endif

#include <cassert>
#include <cstdlib>

typedef PlTerm Prolog_term_ref;
typedef int Prolog_atom;
#ifndef PPL_GPROLOG_H_IS_CLEAN
typedef Bool Prolog_foreign_return_type;

const Prolog_foreign_return_type PROLOG_SUCCESS = TRUE;
const Prolog_foreign_return_type PROLOG_FAILURE = FALSE;
#else
typedef PlBool Prolog_foreign_return_type;

const Prolog_foreign_return_type PROLOG_SUCCESS = PL_TRUE;
const Prolog_foreign_return_type PROLOG_FAILURE = PL_FALSE;
#endif

namespace {

inline Prolog_atom
a_dollar_address() {
  // We use the `name' variable, instead of directly using the string
  // literal, in order to avoid a compiler warning.
  static char name[] = "$address";
  static Prolog_atom atom = Create_Allocate_Atom(name);
  return atom;
}

inline Prolog_atom
a_throw() {
  // We use the `name' variable, instead of directly using the string
  // literal, in order to avoid a compiler warning.
  static char name[] = "throw";
  static Prolog_atom a = Find_Atom(name);
  return a;
}

} // namespace

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
  if (l < INT_LOWEST_VALUE || l > INT_GREATEST_VALUE)
    return 0;
  else {
    t = Mk_Integer(l);
    return 1;
  }
}

/*!
  Assign to \p t a Prolog integer with value \p ul.
*/
inline int
Prolog_put_ulong(Prolog_term_ref& t, unsigned long ul) {
  if (ul > static_cast<unsigned long>(INT_GREATEST_VALUE))
    return 0;
  else {
    t = Mk_Integer(ul);
    return 1;
  }
}

/*!
  Assign to \p t an atom whose name is given
  by the null-terminated string \p s.
*/
inline int
Prolog_put_atom_chars(Prolog_term_ref& t, const char* s) {
  t = Mk_Atom(Create_Allocate_Atom(const_cast<char*>(s)));
  return 1;
}

/*!
  Assign to \p t the Prolog atom \p a.
*/
inline int
Prolog_put_atom(Prolog_term_ref& t, Prolog_atom a) {
  t = Mk_Atom(a);
  return 1;
}

/*!
  Return an atom whose name is given by the null-terminated string \p s.
*/
inline Prolog_atom
Prolog_atom_from_string(const char* s) {
  return Create_Allocate_Atom(const_cast<char*>(s));
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 1 with argument \p a1.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1) {
  Prolog_term_ref args[1];
  args[0] = a1;
  t = Mk_Compound(f, 1, args);
  return 1;
}

/*!
  Assign to \p t a compound term whose principal functor is \p f
  of arity 2 with arguments \p a1 and \p a2.
*/
inline int
Prolog_construct_compound(Prolog_term_ref& t, Prolog_atom f,
			  Prolog_term_ref a1, Prolog_term_ref a2) {
  Prolog_term_ref args[2];
  args[0] = a1;
  args[1] = a2;
  t = Mk_Compound(f, 2, args);
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
  Prolog_term_ref args[3];
  args[0] = a1;
  args[1] = a2;
  args[2] = a3;
  t = Mk_Compound(f, 3, args);
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
  Prolog_term_ref args[4];
  args[0] = a1;
  args[1] = a2;
  args[2] = a3;
  args[3] = a4;
  t = Mk_Compound(f, 4, args);
  return 1;
}

/*!
  Assign to \p c a Prolog list whose head is \p h and tail is \p t.
*/
inline int
Prolog_construct_cons(Prolog_term_ref& c,
		      Prolog_term_ref h, Prolog_term_ref t) {
  Prolog_term_ref args[2];
  args[0] = h;
  args[1] = t;
  c = Mk_List(args);
  return 1;
}

/*!
  Assign to \p t a term representing the address contained in \p p.
*/
inline int
Prolog_put_address(Prolog_term_ref& t, void* p) {
  union {
    void* l;
    unsigned short s[sizeof(void*)/sizeof(unsigned short)];
  } u;
  u.l = reinterpret_cast<void*>(p);
  if (sizeof(unsigned short)*2 == sizeof(void*))
    return Prolog_construct_compound(t, a_dollar_address(),
                                     Mk_Positive(u.s[0]),
                                     Mk_Positive(u.s[1]));
  else if (sizeof(unsigned short)*4 == sizeof(void*))
    return Prolog_construct_compound(t, a_dollar_address(),
                                     Mk_Positive(u.s[0]),
                                     Mk_Positive(u.s[1]),
                                     Mk_Positive(u.s[2]),
                                     Mk_Positive(u.s[3]));
  else
    abort();
}

/*!
  Raise a Prolog exception with \p t as the exception term.
*/
inline void
Prolog_raise_exception(Prolog_term_ref t) {
  Pl_Exec_Continuation(a_throw(), 1, &t);
}

/*!
  Return true if \p t is a Prolog variable, false otherwise.
*/
inline int
Prolog_is_variable(Prolog_term_ref t) {
  return Blt_Var(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog atom, false otherwise.
*/
inline int
Prolog_is_atom(Prolog_term_ref t) {
  return Blt_Atom(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog integer, false otherwise.
*/
inline int
Prolog_is_integer(Prolog_term_ref t) {
  return Blt_Integer(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog compound term, false otherwise.
*/
inline int
Prolog_is_compound(Prolog_term_ref t) {
  return Blt_Compound(t) != FALSE;
}

/*!
  Return true if \p t is a Prolog cons (list constructor), false otherwise.
*/
inline int
Prolog_is_cons(Prolog_term_ref t) {
  if (Blt_Compound(t) == FALSE)
    return 0;
  Prolog_atom name;
  int arity;
  Rd_Compound(t, &name, &arity);
  return name == ATOM_CHAR('.') && arity == 2;
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
  *lp = Rd_Integer_Check(t);
  return 1;
}

/*!
  Return true if \p t is the representation of an address, false otherwise.
*/
inline int
Prolog_is_address(Prolog_term_ref t) {
  if (!Prolog_is_compound(t))
    return 0;
  Prolog_atom name;
  int arity;
  Prolog_term_ref* a = Rd_Compound_Check(t, &name, &arity);
  if (name != a_dollar_address()
      || sizeof(unsigned short)*arity != sizeof(void*))
    return 0;
  for (unsigned i = 0; i < sizeof(void*)/sizeof(unsigned short); ++i) {
    if (!Prolog_is_integer(a[i]))
      return 0;
    long l;
    if (!Prolog_get_long(a[i], &l))
      return 0;
    if (l < 0 || l > USHRT_MAX)
      return 0;
  }
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
  static Prolog_atom dummy_name;
  static int dummy_arity;
  Prolog_term_ref* a = Rd_Compound_Check(t, &dummy_name, &dummy_arity);
  union {
    void* l;
    unsigned short s[sizeof(void*)/sizeof(unsigned short)];
  } u;
  assert(dummy_arity >= 2);
  u.s[0] = Rd_Integer_Check(a[0]);
  u.s[1] = Rd_Integer_Check(a[1]);
  if (sizeof(unsigned short)*4 == sizeof(void*)) {
    assert(dummy_arity == 4);
    u.s[2] = Rd_Integer_Check(a[2]);
    u.s[3] = Rd_Integer_Check(a[3]);
  }
  *vpp = reinterpret_cast<void*>(u.l);
  return 1;
}

/*!
  If \p t is a Prolog atom, return true and store its name into \p name.
  The behavior is undefined if \p t is not a Prolog atom.
*/
inline int
Prolog_get_atom_name(Prolog_term_ref t, Prolog_atom* ap) {
  assert(Prolog_is_atom(t));
  *ap = Rd_Atom_Check(t);
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
  Rd_Compound_Check(t, ap, ip);
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
  static Prolog_atom dummy_name;
  static int dummy_arity;
  a = Rd_Compound_Check(t, &dummy_name, &dummy_arity)[i-1];
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
  Prolog_term_ref* ht = Rd_List_Check(c);
  h = ht[0];
  t = ht[1];
  return 1;
}

/*!
  Unify the terms referenced by \p t and \p u and return true
  if the unification is successful; return false otherwise.
*/
inline int
Prolog_unify(Prolog_term_ref t, Prolog_term_ref u) {
#ifndef PPL_GPROLOG_H_IS_CLEAN
  return Unify(t, u) != FALSE;
#else
  return Pl_Unif(t, u) != PL_FALSE;
#endif
}

#endif // !defined(PCFLI_gprolog_cfli_hh)
