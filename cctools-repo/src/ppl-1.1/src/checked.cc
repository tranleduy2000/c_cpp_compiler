/* Helper functions for checked numbers.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
#include "checked_defs.hh"
#include "C_Integer.hh"

namespace Parma_Polyhedra_Library {

Minus_Infinity MINUS_INFINITY;
Plus_Infinity PLUS_INFINITY;
Not_A_Number NOT_A_NUMBER;

namespace Checked {

//! Holds the precision parameter used for irrational calculations.
unsigned irrational_precision;

struct number_struct {
  unsigned int base;
  bool neg_mantissa;
  bool neg_exponent;
  std::string mantissa;
  unsigned int base_for_exponent;
  unsigned long exponent;
};

/*! \brief
  Returns the integer value associated with the ASCII code \p c, in
  the base \p base positional number system, if there is such an
  association; returns \f$-1\f$ otherwise.
*/
inline int
get_digit(char c, unsigned int base = 10) {
  unsigned int n;
  switch (c) {
  case '0': n = 0; break;
  case '1': n = 1; break;
  case '2': n = 2; break;
  case '3': n = 3; break;
  case '4': n = 4; break;
  case '5': n = 5; break;
  case '6': n = 6; break;
  case '7': n = 7; break;
  case '8': n = 8; break;
  case '9': n = 9; break;
  case 'a': case 'A': n = 10; break;
  case 'b': case 'B': n = 11; break;
  case 'c': case 'C': n = 12; break;
  case 'd': case 'D': n = 13; break;
  case 'e': case 'E': n = 14; break;
  case 'f': case 'F': n = 15; break;
  case 'g': case 'G': n = 16; break;
  case 'h': case 'H': n = 17; break;
  case 'i': case 'I': n = 18; break;
  case 'j': case 'J': n = 19; break;
  case 'k': case 'K': n = 20; break;
  case 'l': case 'L': n = 21; break;
  case 'm': case 'M': n = 22; break;
  case 'n': case 'N': n = 23; break;
  case 'o': case 'O': n = 24; break;
  case 'p': case 'P': n = 25; break;
  case 'q': case 'Q': n = 26; break;
  case 'r': case 'R': n = 27; break;
  case 's': case 'S': n = 28; break;
  case 't': case 'T': n = 29; break;
  case 'u': case 'U': n = 30; break;
  case 'v': case 'V': n = 31; break;
  case 'w': case 'W': n = 32; break;
  case 'x': case 'X': n = 33; break;
  case 'y': case 'Y': n = 34; break;
  case 'z': case 'Z': n = 35; break;
  default:
    return -1;
  }
  if (n >= base)
    return -1;
  return static_cast<int>(n);
}

/*! \brief
  Adds the number represented (in the modulus-and-sign representation)
  by \p b_neg and \p b_mod to the number represented by \p a_neg and
  \p a_mod, assigning the result to the latter.  Returns
  <CODE>false</CODE> is the result cannot be represented; returns
  <CODE>true</CODE> otherwise.
*/
inline bool
sum_sign(bool& a_neg, unsigned long& a_mod,
         bool b_neg, unsigned long b_mod) {
  if (a_neg == b_neg) {
    if (a_mod > C_Integer<unsigned long>::max - b_mod)
      return false;
    a_mod += b_mod;
  }
  else if (a_mod >= b_mod)
    a_mod -= b_mod;
  else {
    a_neg = !a_neg;
    a_mod = b_mod - a_mod;
  }
  return true;
}


/*! \brief
  Helper function for parse_number(): reads the numerator or
  denominator part of a number from \p is into \p numer, returning the
  appropriate Result value.
*/
Result
parse_number_part(std::istream& is, number_struct& numer) {
  enum anonymous_enum { BASE, INTEGER, FRACTIONAL, EXPONENT } state = BASE;
  PPL_UNINITIALIZED(unsigned long, max_exp_div);
  PPL_UNINITIALIZED(int, max_exp_rem);
  bool empty_exponent = true;
  bool empty_mantissa = true;
  long exponent_offset = 0;
  unsigned exponent_offset_scale = 1;
  numer.base = 10;
  numer.base_for_exponent = 10;
  numer.neg_mantissa = false;
  numer.neg_exponent = false;
  numer.mantissa.erase();
  numer.exponent = 0;
  char c;
  do {
    if (!is.get(c))
      return V_CVT_STR_UNK;
  } while (is_space(c));
  switch (c) {
  case '-':
    numer.neg_mantissa = true;
    // Fall through.
  case '+':
    if (!is.get(c))
      return V_CVT_STR_UNK;
    if (c == 'i' || c == 'I')
      goto inf;
    if (c != '.')
      break;
    // Fall through.
  case '.':
    state = FRACTIONAL;
    if (!is.get(c))
      return V_CVT_STR_UNK;
    break;
  case 'n':
  case 'N':
    if (!is.get(c))
      return V_CVT_STR_UNK;
    if (c != 'a' && c != 'A')
      goto unexpected;
    if (!is.get(c))
      return V_CVT_STR_UNK;
    if (c != 'n' && c != 'N')
      goto unexpected;
    return V_NAN;
  inf:
  case 'i':
  case 'I':
    if (!is.get(c))
      return V_CVT_STR_UNK;
    if (c != 'n' && c != 'n')
      goto unexpected;
    if (!is.get(c))
      return V_CVT_STR_UNK;
    if (c != 'f' && c != 'F')
      goto unexpected;
    return numer.neg_mantissa ? V_EQ_MINUS_INFINITY : V_EQ_PLUS_INFINITY;
  }
  if (state != FRACTIONAL) {
    if (get_digit(c, 10) < 0)
      goto unexpected;
    char d;
    if (c == '0' && !is.get(d).fail()) {
      if (d == 'x' || d == 'X') {
        numer.base = 16;
        numer.base_for_exponent = 16;
        state = INTEGER;
        if (!is.get(c))
          return V_CVT_STR_UNK;
      }
      else
        is.unget();
    }
  }
  do {
    switch (state) {
    case BASE:
      if (get_digit(c, 10) >= 0) {
        if (c != '0' || !numer.mantissa.empty())
          numer.mantissa += c;
        empty_mantissa = false;
        break;
      }
      if (c == '^') {
        if (!is.get(c))
          return V_CVT_STR_UNK;
        if (c != '^')
          goto unexpected;
        numer.base = 0;
        for (std::string::const_iterator
               i = numer.mantissa.begin(); i != numer.mantissa.end(); ++i) {
          numer.base = numer.base * 10 + static_cast<unsigned>(get_digit(*i, 10));
          if (numer.base > 36)
            goto unexpected;
        }
        if (numer.base < 2)
          goto unexpected;
        numer.base_for_exponent = numer.base;
        numer.mantissa.erase();
        empty_mantissa = true;
        state = INTEGER;
        break;
      }
      goto integer;
    case INTEGER:
      if (get_digit(c, numer.base) >= 0) {
        if (c != '0' || !numer.mantissa.empty())
          numer.mantissa += c;
        empty_mantissa = false;
        break;
      }
    integer:
      if (c == '.') {
        state = FRACTIONAL;
        break;
      }
      goto fractional;
    case FRACTIONAL:
      if (get_digit(c, numer.base) >= 0) {
        --exponent_offset;
        if (c != '0' || !numer.mantissa.empty())
          numer.mantissa += c;
        empty_mantissa = false;
        break;
      }
    fractional:
      if (empty_mantissa)
        goto unexpected;
      if (c == 'e' || c == 'E')
        goto exp;
      if (c == 'p' || c == 'P') {
        if (numer.base == 16) {
          numer.base_for_exponent = 2;
          exponent_offset_scale = 4;
          goto exp;
        }
        else
          goto unexpected;
      }
      if (c == '*') {
        if (!is.get(c))
          return V_CVT_STR_UNK;
        if (c != '^')
          goto unexpected;
      exp:
        state = EXPONENT;
        PPL_ASSERT(numer.base >= 2);
        const long l_max = C_Integer<long>::max;
        max_exp_div = static_cast<unsigned long>(l_max) / numer.base;
        max_exp_rem = static_cast<int>(l_max % static_cast<long>(numer.base));
        if (!is.get(c))
          return V_CVT_STR_UNK;
        if (c == '-') {
          numer.neg_exponent = true;
          break;
        }
        if (c == '+')
          break;
        continue;
      }
      is.unget();
      goto ok;
    case EXPONENT:
      const int d = get_digit(c, 10);
      if (d >= 0) {
        empty_exponent = false;
        if (numer.exponent > max_exp_div
            || (numer.exponent == max_exp_div && d > max_exp_rem))
          return V_CVT_STR_UNK;
        numer.exponent = 10 * numer.exponent + static_cast<unsigned long>(d);
        break;
      }
      if (empty_exponent)
        goto unexpected;
      is.unget();
      goto ok;
    }
    is.get(c);
  } while (!is.fail());

  if (empty_mantissa || is.bad())
    return V_CVT_STR_UNK;

 ok:
  {
    std::string::size_type n = numer.mantissa.size();
    while (n > 0 && numer.mantissa[n - 1] == '0') {
      --n;
      ++exponent_offset;
    }
    numer.mantissa.erase(n);
    bool neg;
    if (exponent_offset < 0) {
      neg = true;
      exponent_offset = -exponent_offset;
    }
    else
      neg = false;
    sum_sign(numer.neg_exponent, numer.exponent,
             neg, static_cast<unsigned long>(exponent_offset) * exponent_offset_scale);
    return V_EQ;
  }

 unexpected:
  is.unget();
  return V_CVT_STR_UNK;
}

/*! \brief
  Reads a number from \p is writing it into \p numer, the numerator,
  and \p denom, the denominator; the appropriate Result value is
  returned.
*/
Result
parse_number(std::istream& is, number_struct& numer, number_struct& denom) {
  // Read the numerator.
  Result r = parse_number_part(is, numer);
  if (r != V_EQ)
    return r;
  char c;
  is.get(c);
  if (is.bad())
    return V_CVT_STR_UNK;
  if (!is) {
    denom.base = 0;
    return r;
  }
  if (c != '/') {
    is.unget();
    denom.base = 0;
    return r;
  }
  // Read the denominator.
  r = parse_number_part(is, denom);
  if (r != V_EQ)
    return V_CVT_STR_UNK;
  if (numer.base == denom.base
      && numer.base_for_exponent == denom.base_for_exponent) {
    if (sum_sign(numer.neg_exponent, numer.exponent,
                 !denom.neg_exponent, denom.exponent)) {
      if (numer.neg_exponent) {
        denom.neg_exponent = false;
        denom.exponent = numer.exponent;
        numer.exponent = 0;
      }
      else
        denom.exponent = 0;
    }
  }
  return V_EQ;
}


Result
input_mpq(mpq_class& to, std::istream& is) {
  number_struct numer_struct;
  number_struct denom_struct;
  const Result r = parse_number(is, numer_struct, denom_struct);
  if (r == V_CVT_STR_UNK) {
    is.setstate(is.failbit);
    return r;
  }
  is.clear(is.rdstate() & ~is.failbit);
  if (r != V_EQ)
    return r;
  if (denom_struct.base != 0 && denom_struct.mantissa.empty())
      return V_NAN;
  if (numer_struct.mantissa.empty()) {
    to = 0;
    return V_EQ;
  }
  const mpz_ptr numer = to.get_num().get_mpz_t();
  const mpz_ptr denom = to.get_den().get_mpz_t();
  mpz_set_str(numer, numer_struct.mantissa.c_str(),
              static_cast<int>(numer_struct.base));
  if (denom_struct.base != 0) {
    if (numer_struct.neg_mantissa != denom_struct.neg_mantissa)
      mpz_neg(numer, numer);
    mpz_set_str(denom, denom_struct.mantissa.c_str(),
                static_cast<int>(denom_struct.base));
    if (numer_struct.exponent != 0 || denom_struct.exponent != 0) {
      // Multiply the exponents into the numerator and denominator.
      mpz_t z;
      mpz_init(z);
      if (numer_struct.exponent != 0) {
        mpz_ui_pow_ui(z,
                      numer_struct.base_for_exponent, numer_struct.exponent);
        if (numer_struct.neg_exponent)
          mpz_mul(denom, denom, z);
        else
          mpz_mul(numer, numer, z);
      }
      if (denom_struct.exponent != 0) {
        mpz_ui_pow_ui(z,
                      denom_struct.base_for_exponent, denom_struct.exponent);
        if (denom_struct.neg_exponent)
          mpz_mul(numer, numer, z);
        else
          mpz_mul(denom, denom, z);
      }
      mpz_clear(z);
    }
  }
  else {
    if (numer_struct.neg_mantissa)
      mpz_neg(numer, numer);
    if (numer_struct.exponent != 0) {
      if (numer_struct.neg_exponent) {
        // Add the negative exponent as a denominator.
        mpz_ui_pow_ui(denom,
                      numer_struct.base_for_exponent, numer_struct.exponent);
        goto end;
      }
      // Multiply the exponent into the numerator.
      mpz_t z;
      mpz_init(z);
      mpz_ui_pow_ui(z,
                    numer_struct.base_for_exponent, numer_struct.exponent);
      mpz_mul(numer, numer, z);
      mpz_clear(z);
    }
    mpz_set_ui(denom, 1);
    return V_EQ;
  }
 end:
  // GMP operators require rationals in canonical form.
  to.canonicalize();
  return V_EQ;
}

/* NOTE: q is overwritten! */
std::string float_mpq_to_string(mpq_class& q) {
  const mpz_ptr n = q.get_num().get_mpz_t();
  const mpz_ptr d = q.get_den().get_mpz_t();
  const unsigned long decimals = mpz_sizeinbase(d, 2) - 1;
  if (decimals != 0) {
    mpz_ui_pow_ui(d, 5, decimals);
    mpz_mul(n, n, d);
  }
  size_t bufsize = mpz_sizeinbase(n, 10);
  if (bufsize < decimals)
    bufsize = decimals + 4;
  else
    bufsize += 3;
  char buf[bufsize];
  mpz_get_str(buf, 10, n);
  if (decimals != 0) {
    const size_t len = strlen(buf);
    if (decimals < len) {
      memmove(&buf[len - decimals + 1], &buf[len - decimals], decimals + 1);
      buf[len - decimals] = '.';
    }
    else {
      const size_t zeroes = decimals - len;
      memmove(&buf[2 + zeroes], &buf[0], len + 1);
      buf[0] = '0';
      buf[1] = '.';
      memset(&buf[2], '0', zeroes);
    }
  }
  return buf;
}

} // namespace Checked

} // namespace Parma_Polyhedra_Library

