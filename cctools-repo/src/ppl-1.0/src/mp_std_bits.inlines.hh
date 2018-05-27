/* Definitions of specializations of std:: functions and methods for
   multi-precision types.
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

#ifndef PPL_mp_std_bits_inlines_hh
#define PPL_mp_std_bits_inlines_hh 1

inline void
swap(mpz_class& x, mpz_class& y) {
  mpz_swap(x.get_mpz_t(), y.get_mpz_t());
}

inline void
swap(mpq_class& x, mpq_class& y) {
  mpq_swap(x.get_mpq_t(), y.get_mpq_t());
}

#endif // !defined(PPL_mp_std_bits_inlines_hh)
