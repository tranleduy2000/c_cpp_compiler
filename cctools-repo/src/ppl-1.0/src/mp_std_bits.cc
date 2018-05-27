/* Definitions of specializations of std::numeric_limits objects for
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

#include "ppl-config.h"
#include "mp_std_bits.defs.hh"

const bool std::numeric_limits<mpz_class>::is_specialized;
const int std::numeric_limits<mpz_class>::digits;
const int std::numeric_limits<mpz_class>::digits10;
const bool std::numeric_limits<mpz_class>::is_signed;
const bool std::numeric_limits<mpz_class>::is_integer;
const bool std::numeric_limits<mpz_class>::is_exact;
const int std::numeric_limits<mpz_class>::radix;
const int std::numeric_limits<mpz_class>::min_exponent;
const int std::numeric_limits<mpz_class>::min_exponent10;
const int std::numeric_limits<mpz_class>::max_exponent;
const int std::numeric_limits<mpz_class>::max_exponent10;
const bool std::numeric_limits<mpz_class>::has_infinity;
const bool std::numeric_limits<mpz_class>::has_quiet_NaN;
const bool std::numeric_limits<mpz_class>::has_signaling_NaN;
const std::float_denorm_style std::numeric_limits<mpz_class>::has_denorm;
const bool std::numeric_limits<mpz_class>::has_denorm_loss;
const bool std::numeric_limits<mpz_class>::is_iec559;
const bool std::numeric_limits<mpz_class>::is_bounded;
const bool std::numeric_limits<mpz_class>::is_modulo;
const bool std::numeric_limits<mpz_class>::traps;
const bool std::numeric_limits<mpz_class>::tininess_before;
const std::float_round_style std::numeric_limits<mpz_class>::round_style;

const bool std::numeric_limits<mpq_class>::is_specialized;
const int std::numeric_limits<mpq_class>::digits;
const int std::numeric_limits<mpq_class>::digits10;
const bool std::numeric_limits<mpq_class>::is_signed;
const bool std::numeric_limits<mpq_class>::is_integer;
const bool std::numeric_limits<mpq_class>::is_exact;
const int std::numeric_limits<mpq_class>::radix;
const int std::numeric_limits<mpq_class>::min_exponent;
const int std::numeric_limits<mpq_class>::min_exponent10;
const int std::numeric_limits<mpq_class>::max_exponent;
const int std::numeric_limits<mpq_class>::max_exponent10;
const bool std::numeric_limits<mpq_class>::has_infinity;
const bool std::numeric_limits<mpq_class>::has_quiet_NaN;
const bool std::numeric_limits<mpq_class>::has_signaling_NaN;
const std::float_denorm_style std::numeric_limits<mpq_class>::has_denorm;
const bool std::numeric_limits<mpq_class>::has_denorm_loss;
const bool std::numeric_limits<mpq_class>::is_iec559;
const bool std::numeric_limits<mpq_class>::is_bounded;
const bool std::numeric_limits<mpq_class>::is_modulo;
const bool std::numeric_limits<mpq_class>::traps;
const bool std::numeric_limits<mpq_class>::tininess_before;
const std::float_round_style std::numeric_limits<mpq_class>::round_style;
