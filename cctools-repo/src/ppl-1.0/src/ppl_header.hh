/* This is the header file of the Parma Polyhedra Library.
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

#ifndef PPL_ppl_hh
#define PPL_ppl_hh 1

#ifdef NDEBUG
# define PPL_SAVE_NDEBUG NDEBUG
# undef NDEBUG
#endif

#ifdef __STDC_LIMIT_MACROS
# define PPL_SAVE_STDC_LIMIT_MACROS __STDC_LIMIT_MACROS
#endif

#include "ppl-config.h"
#include "version.hh"
#include "ppl_include_files.hh"

//! Defined to 1 if PPL::Watchdog objects are supported, to 0 otherwise.
#define PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED \
  (PPL_HAVE_DECL_SETITIMER && PPL_HAVE_DECL_SIGACTION)

#undef PPL_SPECIALIZE_ABS
#undef PPL_SPECIALIZE_ADD
#undef PPL_SPECIALIZE_ADD_MUL
#undef PPL_SPECIALIZE_ASSIGN
#undef PPL_SPECIALIZE_ASSIGN_SPECIAL
#undef PPL_SPECIALIZE_CEIL
#undef PPL_SPECIALIZE_CLASSIFY
#undef PPL_SPECIALIZE_CMP
#undef PPL_SPECIALIZE_CONSTRUCT
#undef PPL_SPECIALIZE_CONSTRUCT_SPECIAL
#undef PPL_SPECIALIZE_COPY
#undef PPL_SPECIALIZE_DIV
#undef PPL_SPECIALIZE_DIV2EXP
#undef PPL_SPECIALIZE_FLOOR
#undef PPL_SPECIALIZE_FUN1_0_0
#undef PPL_SPECIALIZE_FUN1_0_1
#undef PPL_SPECIALIZE_FUN1_0_2
#undef PPL_SPECIALIZE_FUN1_0_3
#undef PPL_SPECIALIZE_FUN1_1_1
#undef PPL_SPECIALIZE_FUN1_1_2
#undef PPL_SPECIALIZE_FUN1_2_2
#undef PPL_SPECIALIZE_FUN2_0_0
#undef PPL_SPECIALIZE_FUN2_0_1
#undef PPL_SPECIALIZE_FUN2_0_2
#undef PPL_SPECIALIZE_FUN3_0_1
#undef PPL_SPECIALIZE_FUN5_0_1
#undef PPL_SPECIALIZE_GCD
#undef PPL_SPECIALIZE_GCDEXT
#undef PPL_SPECIALIZE_IDIV
#undef PPL_SPECIALIZE_INPUT
#undef PPL_SPECIALIZE_IS_INT
#undef PPL_SPECIALIZE_IS_MINF
#undef PPL_SPECIALIZE_IS_NAN
#undef PPL_SPECIALIZE_IS_PINF
#undef PPL_SPECIALIZE_LCM
#undef PPL_SPECIALIZE_MUL
#undef PPL_SPECIALIZE_MUL2EXP
#undef PPL_SPECIALIZE_NEG
#undef PPL_SPECIALIZE_OUTPUT
#undef PPL_SPECIALIZE_REM
#undef PPL_SPECIALIZE_SGN
#undef PPL_SPECIALIZE_SQRT
#undef PPL_SPECIALIZE_SUB
#undef PPL_SPECIALIZE_SUB_MUL
#undef PPL_SPECIALIZE_TRUNC

#undef PPL_COMPILE_TIME_CHECK
#undef PPL_COMPILE_TIME_CHECK_AUX
#undef PPL_COMPILE_TIME_CHECK_NAME

#ifdef __STDC_LIMIT_MACROS
# undef __STDC_LIMIT_MACROS
#endif
#ifdef PPL_SAVE_STDC_LIMIT_MACROS
# define __STDC_LIMIT_MACROS PPL_SAVE_STDC_LIMIT_MACROS
# undef PPL_SAVE_STDC_LIMIT_MACROS
#endif

#ifdef PPL_SAVE_NDEBUG
# ifndef NDEBUG
#  define NDEBUG PPL_SAVE_NDEBUG
# endif
# undef PPL_SAVE_NDEBUG
#else
# ifdef NDEBUG
#  undef NDEBUG
# endif
#endif
// Must include <cassert> again in order to make the latest changes to
// NDEBUG effective.
#include <cassert>
#include "assert.hh"

#ifdef PPL_NO_AUTOMATIC_INITIALIZATION
 #undef PPL_NO_AUTOMATIC_INITIALIZATION
#endif

#endif // !defined(PPL_ppl_hh)
