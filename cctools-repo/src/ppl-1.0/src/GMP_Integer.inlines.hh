/* GMP_Integer class implementation: inline functions.
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

#ifndef PPL_GMP_Integer_inlines_hh
#define PPL_GMP_Integer_inlines_hh 1

#include "assert.hh"

namespace Parma_Polyhedra_Library {

inline void
neg_assign(GMP_Integer& x) {
  mpz_neg(x.get_mpz_t(), x.get_mpz_t());
}

inline void
neg_assign(GMP_Integer& x, const GMP_Integer& y) {
  mpz_neg(x.get_mpz_t(), y.get_mpz_t());
}

inline void
abs_assign(GMP_Integer& x) {
  mpz_abs(x.get_mpz_t(), x.get_mpz_t());
}

inline void
abs_assign(GMP_Integer& x, const GMP_Integer& y) {
  mpz_abs(x.get_mpz_t(), y.get_mpz_t());
}

inline void
gcd_assign(GMP_Integer& x, const GMP_Integer& y, const GMP_Integer& z) {
  mpz_gcd(x.get_mpz_t(), y.get_mpz_t(), z.get_mpz_t());
}

inline void
rem_assign(GMP_Integer& x, const GMP_Integer& y, const GMP_Integer& z) {
  mpz_tdiv_r(x.get_mpz_t(), y.get_mpz_t(), z.get_mpz_t());
}

inline void
gcdext_assign(GMP_Integer& x, GMP_Integer& s, GMP_Integer& t,
	      const GMP_Integer& y, const GMP_Integer& z) {
  mpz_gcdext(x.get_mpz_t(),
	     s.get_mpz_t(), t.get_mpz_t(),
	     y.get_mpz_t(), z.get_mpz_t());
}

inline void
lcm_assign(GMP_Integer& x, const GMP_Integer& y, const GMP_Integer& z) {
  mpz_lcm(x.get_mpz_t(), y.get_mpz_t(), z.get_mpz_t());
}

inline void
add_mul_assign(GMP_Integer& x, const GMP_Integer& y, const GMP_Integer& z) {
  mpz_addmul(x.get_mpz_t(), y.get_mpz_t(), z.get_mpz_t());
}

inline void
sub_mul_assign(GMP_Integer& x, const GMP_Integer& y, const GMP_Integer& z) {
  mpz_submul(x.get_mpz_t(), y.get_mpz_t(), z.get_mpz_t());
}

inline void
mul_2exp_assign(GMP_Integer& x, const GMP_Integer& y, unsigned int exp) {
  mpz_mul_2exp(x.get_mpz_t(), y.get_mpz_t(), exp);
}

inline void
div_2exp_assign(GMP_Integer& x, const GMP_Integer& y, unsigned int exp) {
  mpz_tdiv_q_2exp(x.get_mpz_t(), y.get_mpz_t(), exp);
}

inline void
exact_div_assign(GMP_Integer& x, const GMP_Integer& y, const GMP_Integer& z) {
  PPL_ASSERT(y % z == 0);
  mpz_divexact(x.get_mpz_t(), y.get_mpz_t(), z.get_mpz_t());
}

inline void
sqrt_assign(GMP_Integer& x, const GMP_Integer& y) {
  mpz_sqrt(x.get_mpz_t(), y.get_mpz_t());
}

inline int
cmp(const GMP_Integer& x, const GMP_Integer& y) {
  return mpz_cmp(x.get_mpz_t(), y.get_mpz_t());
}

inline const mpz_class&
raw_value(const GMP_Integer& x) {
  return x;
}

inline mpz_class&
raw_value(GMP_Integer& x) {
  return x;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_GMP_Integer_inlines_hh)
