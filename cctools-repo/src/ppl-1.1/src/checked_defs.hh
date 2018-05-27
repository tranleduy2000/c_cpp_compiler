/* Abstract checked arithmetic function container.
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

#ifndef PPL_checked_defs_hh
#define PPL_checked_defs_hh 1

#include <cassert>
#include <iostream>
#include <gmpxx.h>
#include "mp_std_bits_defs.hh"
#include "Temp_defs.hh"
#include "Rounding_Dir_defs.hh"
#include "Numeric_Format_defs.hh"
#include "Float_defs.hh"

namespace Parma_Polyhedra_Library {

namespace Checked {


// It is a pity that function partial specialization is not permitted
// by C++.  To (partly) overcome this limitation, we use class
// encapsulated functions and partial specialization of containing
// classes.

#define PPL_FUNCTION_CLASS(name) name ## _function_struct

#define PPL_DECLARE_FUN1_0_0(name, ret_type, qual, type)                \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(qual) PPL_U(type)& arg) {           \
    return PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(arg); \
  }

#define PPL_DECLARE_FUN1_0_1(name, ret_type, qual, type, after1)        \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(qual) PPL_U(type)& arg, PPL_U(after1) a1) { \
    return                                                              \
      PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(arg, a1); \
  }

#define PPL_DECLARE_FUN1_0_2(name, ret_type, qual, type, after1, after2) \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(qual) PPL_U(type)& arg, PPL_U(after1) a1, \
                       PPL_U(after2) a2) {                              \
    return                                                              \
      PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(arg,      \
                                                              a1, a2);  \
  }

#define PPL_DECLARE_FUN1_0_3(name, ret_type, qual, type,                \
                             after1, after2, after3)                    \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(qual) PPL_U(type)& arg,             \
                       PPL_U(after1) a1, PPL_U(after2) a2,              \
                       PPL_U(after3) a3) {                              \
    return                                                              \
      PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(arg,      \
                                                              a1, a2,   \
                                                              a3);      \
  }

#define PPL_DECLARE_FUN1_1_1(name, ret_type, before1, qual, type, after1) \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(before1) b1, PPL_U(qual) PPL_U(type)& arg, \
                       PPL_U(after1) a1) {                              \
    return                                                              \
      PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(b1, arg,  \
                                                              a1);      \
  }

#define PPL_DECLARE_FUN1_1_2(name, ret_type, before1, qual, type,       \
                             after1, after2)                            \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(before1) b1, PPL_U(qual) PPL_U(type)& arg, \
                       PPL_U(after1) a1, PPL_U(after2) a2) {            \
    return                                                              \
      PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(b1, arg,  \
                                                              a1, a2);  \
  }

#define PPL_DECLARE_FUN1_2_2(name, ret_type, before1, before2, qual, type, \
                             after1, after2)                            \
  template <typename Policy, typename type>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy, typename type>                             \
  inline ret_type PPL_U(name)(PPL_U(before1) b1, PPL_U(before2) b2,     \
                       PPL_U(qual) PPL_U(type)& arg,                    \
                       PPL_U(after1) a1, PPL_U(after2) a2) {            \
    return                                                              \
      PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)>::function(b1, b2,   \
                                                              arg,      \
                                                              a1, a2);  \
  }

#define PPL_DECLARE_FUN2_0_0(name, ret_type, qual1, type1, qual2, type2) \
  template <typename Policy1, typename Policy2,                         \
            typename type1, typename type2>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy1, typename Policy2,                         \
            typename type1, typename type2>                             \
  inline ret_type PPL_U(name)(PPL_U(qual1) PPL_U(type1)& arg1,          \
                       PPL_U(qual2) PPL_U(type2)& arg2) {               \
    return PPL_FUNCTION_CLASS(name)<Policy1, Policy2,                   \
      type1, PPL_U(type2)>::function(arg1, arg2);                       \
  }

#define PPL_DECLARE_FUN2_0_1(name, ret_type, qual1, type1,      \
                             qual2, type2, after1)              \
  template <typename Policy1, typename Policy2,                 \
            typename type1, typename type2>                     \
  struct PPL_FUNCTION_CLASS(name);                              \
  template <typename Policy1, typename Policy2,                 \
            typename type1, typename type2>                     \
  inline ret_type PPL_U(name)(PPL_U(qual1) PPL_U(type1)& arg1,  \
                       PPL_U(qual2) PPL_U(type2)& arg2,         \
                       PPL_U(after1) a1) {                      \
    return PPL_FUNCTION_CLASS(name)<Policy1, Policy2,           \
      type1, PPL_U(type2)>::function(arg1, arg2, a1);           \
  }

#define PPL_DECLARE_FUN2_0_2(name, ret_type, qual1, type1, qual2, type2, \
                             after1, after2)                            \
  template <typename Policy1, typename Policy2,                         \
            typename type1, typename type2>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy1, typename Policy2,                         \
            typename type1, typename type2>                             \
  inline ret_type PPL_U(name)(PPL_U(qual1) PPL_U(type1)& arg1,          \
                       PPL_U(qual2) PPL_U(type2)& arg2,                 \
                       PPL_U(after1) a1, PPL_U(after2) a2) {            \
    return PPL_FUNCTION_CLASS(name)<Policy1, Policy2,                   \
      type1, PPL_U(type2)>::function(arg1, arg2, a1, a2);               \
  }

#define PPL_DECLARE_FUN3_0_1(name, ret_type, qual1, type1,              \
                             qual2, type2, qual3, type3, after1)        \
  template <typename Policy1, typename Policy2, typename Policy3,       \
            typename type1, typename type2, typename type3>             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy1, typename Policy2, typename Policy3,       \
            typename type1, typename type2, typename type3>             \
  inline ret_type PPL_U(name)(PPL_U(qual1) PPL_U(type1)& arg1,          \
                       PPL_U(qual2) PPL_U(type2)& arg2,                 \
                       PPL_U(qual3) PPL_U(type3)& arg3,                 \
                       PPL_U(after1) a1) {                              \
    return PPL_FUNCTION_CLASS(name)<Policy1, Policy2, Policy3,          \
      type1, type2, PPL_U(type3)>                                       \
      ::function(arg1, arg2, arg3, a1);                                 \
  }

#define PPL_DECLARE_FUN5_0_1(name, ret_type,                            \
                             qual1, type1, qual2, type2, qual3, type3,  \
                             qual4, type4, qual5, type5,                \
                             after1)                                    \
  template <typename Policy1, typename Policy2, typename Policy3,       \
            typename Policy4,typename Policy5,                          \
            typename type1, typename type2, typename type3,             \
            typename type4, typename type5>                             \
  struct PPL_FUNCTION_CLASS(name);                                      \
  template <typename Policy1, typename Policy2, typename Policy3,       \
            typename Policy4,typename Policy5,                          \
            typename type1, typename type2, typename type3,             \
            typename type4, typename type5>                             \
  inline ret_type PPL_U(name)(PPL_U(qual1) PPL_U(type1)& arg1, PPL_U(qual2) \
                       PPL_U(type2)& arg2,                              \
                       PPL_U(qual3) PPL_U(type3)& arg3, PPL_U(qual4)    \
                       PPL_U(type4)& arg4,                              \
                       PPL_U(qual5) PPL_U(type5)& arg5,                 \
                       PPL_U(after1) a1) {                              \
    return PPL_FUNCTION_CLASS(name)<Policy1, Policy2, Policy3,          \
      Policy4, Policy5,                                                 \
      type1, type2,                                                     \
      type3, type4,                                                     \
      PPL_U(type5)>                                                     \
      ::function(arg1, arg2, arg3, arg4, arg5, a1);                     \
  }

#define PPL_SPECIALIZE_FUN1_0_0(name, func, ret_type, qual, type)       \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(qual) PPL_U(type)& arg) {     \
      return PPL_U(func)<Policy>(arg);                                  \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN1_0_1(name, func, ret_type, qual, type, after1) \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(qual) PPL_U(type)& arg,       \
                                    PPL_U(after1) a1) {                 \
      return PPL_U(func)<Policy>(arg, a1);                              \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN1_0_2(name, func, ret_type, qual, type,       \
                                after1, after2)                         \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(qual) PPL_U(type)& arg,       \
                                    PPL_U(after1) a1, PPL_U(after2) a2) \
    {                                                                   \
      return PPL_U(func)<Policy>(arg, a1, a2);                          \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN1_0_3(name, func, ret_type, qual, type,       \
                                after1, after2, after3)                 \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(qual) PPL_U(type)& arg,       \
                                    PPL_U(after1) a1, PPL_U(after2) a2, \
                                    PPL_U(after3) a3) {                 \
      return PPL_U(func)<Policy>(arg, a1, a2, a3);                      \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN1_1_1(name, func, ret_type, before1,          \
                                qual, type, after1)                     \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(before1) b1, PPL_U(qual)      \
                                    PPL_U(type)& arg,                   \
                                    PPL_U(after1) a1) {                 \
      return PPL_U(func)<Policy>(b1, arg, a1);                          \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN1_1_2(name, func, ret_type, before1,          \
                                qual, type, after1, after2)             \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(before1) b1, PPL_U(qual)      \
                                    PPL_U(type)& arg,                   \
                                    PPL_U(after1) a1, PPL_U(after2) a2) \
    {                                                                   \
      return PPL_U(func)<Policy>(b1, arg, a1, a2);                      \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN1_2_2(name, func, ret_type, before1, before2, \
                                qual, type, after1, after2)             \
  template <typename Policy>                                            \
  struct PPL_FUNCTION_CLASS(name)<Policy, PPL_U(type)> {                \
    static inline ret_type function(PPL_U(before1) b1, PPL_U(before2) b2, \
                                    PPL_U(qual) PPL_U(type)& arg,       \
                                    PPL_U(after1) a1, PPL_U(after2) a2) \
    {                                                                   \
      return PPL_U(func)<Policy>(b1, b2, arg, a1, a2);                  \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN2_0_0(name, func, ret_type, qual1, type1,     \
                                qual2, type2)                           \
  template <typename Policy1, typename Policy2>                         \
  struct PPL_FUNCTION_CLASS(name)<Policy1, Policy2, type1,              \
                                  PPL_U(type2)> {                       \
    static inline ret_type function(PPL_U(qual1) PPL_U(type1)& arg1,    \
                                    PPL_U(qual2) PPL_U(type2) &arg2) {  \
      return PPL_U(func)<Policy1, Policy2>(arg1, arg2);                 \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN2_0_1(name, func, ret_type, qual1, type1,     \
                                qual2, type2, after1)                   \
  template <typename Policy1, typename Policy2>                         \
  struct PPL_FUNCTION_CLASS(name)<Policy1, Policy2, type1,              \
                                  PPL_U(type2)> {                       \
    static inline ret_type function(PPL_U(qual1) PPL_U(type1)& arg1,    \
                                    PPL_U(qual2) PPL_U(type2) &arg2,    \
                                    PPL_U(after1) a1) {                 \
      return PPL_U(func)<Policy1, Policy2>(arg1, arg2, a1);             \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN2_0_2(name, func, ret_type, qual1, type1,     \
                                qual2, type2, after1, after2)           \
  template <typename Policy1, typename Policy2>                         \
  struct PPL_FUNCTION_CLASS(name)<Policy1, Policy2, type1,              \
                                  PPL_U(type2)> {                       \
    static inline ret_type function(PPL_U(qual1) PPL_U(type1)& arg1,    \
                                    PPL_U(qual2) PPL_U(type2) &arg2,    \
                                    PPL_U(after1) a1, PPL_U(after2) a2) \
    {                                                                   \
      return PPL_U(func)<Policy1, Policy2>(arg1, arg2, a1, a2);         \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN3_0_1(name, func, ret_type, qual1, type1,     \
                                qual2, type2, qual3, type3, after1)     \
  template <typename Policy1, typename Policy2, typename Policy3>       \
  struct PPL_FUNCTION_CLASS(name) <Policy1, Policy2, Policy3,           \
                                   type1, type2,                        \
                                   PPL_U(type3)> {                      \
    static inline Result function(PPL_U(qual1) PPL_U(type1)& arg1,      \
                                  PPL_U(qual2) PPL_U(type2) &arg2,      \
                                  PPL_U(qual3) PPL_U(type3) &arg3,      \
                                  PPL_U(after1) a1) {                   \
      return PPL_U(func)<Policy1, Policy2, Policy3>(arg1, arg2, arg3,   \
                                                    a1);                \
    }                                                                   \
  };

#define PPL_SPECIALIZE_FUN5_0_1(name, func, ret_type,                   \
                                qual1, type1, qual2, type2,             \
                                qual3, type3,                           \
                                qual4, type4, qual5, type5, after1)     \
  template <typename Policy1, typename Policy2, typename Policy3,       \
            typename Policy4, typename Policy5>                         \
  struct PPL_FUNCTION_CLASS(name) <Policy1, Policy2, Policy3, Policy4,  \
                                   Policy5,                             \
                                   type1, type2,                        \
                                   type3, type4,                        \
                                   PPL_U(type5)> {                      \
    static inline Result                                                \
      function(PPL_U(qual1) PPL_U(type1)& arg1, PPL_U(qual2)            \
               PPL_U(type2) &arg2,                                      \
               PPL_U(qual3) PPL_U(type3) &arg3, PPL_U(qual4)            \
               PPL_U(type4) &arg4,                                      \
               PPL_U(qual5) PPL_U(type5) &arg5, PPL_U(after1) a1) {     \
      return PPL_U(func)<Policy1, Policy2, Policy3, Policy4,            \
        Policy5>(arg1, arg2, arg3, arg4, arg5, a1);                     \
    }                                                                   \
  };

// The `nonconst' macro helps readability of the sequel.
#ifdef nonconst
#define PPL_SAVED_nonconst nonconst
#undef nonconst
#endif
#define nonconst

#define PPL_SPECIALIZE_COPY(func, Type)                                 \
  PPL_SPECIALIZE_FUN2_0_0(copy, func, void, nonconst, Type, const, Type)
#define PPL_SPECIALIZE_SGN(func, From)                                  \
  PPL_SPECIALIZE_FUN1_0_0(sgn, func, Result_Relation, const, From)
#define PPL_SPECIALIZE_CMP(func, Type1, Type2)                          \
  PPL_SPECIALIZE_FUN2_0_0(cmp, func, Result_Relation, const, Type1, const, Type2)
#define PPL_SPECIALIZE_CLASSIFY(func, Type)                             \
  PPL_SPECIALIZE_FUN1_0_3(classify, func, Result, const, Type, bool, bool, bool)
#define PPL_SPECIALIZE_IS_NAN(func, Type)                       \
  PPL_SPECIALIZE_FUN1_0_0(is_nan, func, bool, const, Type)
#define PPL_SPECIALIZE_IS_MINF(func, Type)                      \
  PPL_SPECIALIZE_FUN1_0_0(is_minf, func, bool, const, Type)
#define PPL_SPECIALIZE_IS_PINF(func, Type)                      \
  PPL_SPECIALIZE_FUN1_0_0(is_pinf, func, bool, const, Type)
#define PPL_SPECIALIZE_IS_INT(func, Type)                       \
  PPL_SPECIALIZE_FUN1_0_0(is_int, func, bool, const, Type)
#define PPL_SPECIALIZE_ASSIGN_SPECIAL(func, Type)                       \
  PPL_SPECIALIZE_FUN1_0_2(assign_special, func, Result,                 \
                          nonconst, Type, Result_Class, Rounding_Dir)
#define PPL_SPECIALIZE_CONSTRUCT_SPECIAL(func, Type)                    \
  PPL_SPECIALIZE_FUN1_0_2(construct_special, func, Result, nonconst,    \
                          Type, Result_Class, Rounding_Dir)
#define PPL_SPECIALIZE_CONSTRUCT(func, To, From)                        \
  PPL_SPECIALIZE_FUN2_0_1(construct, func, Result, nonconst, To,        \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_ASSIGN(func, To, From)                   \
  PPL_SPECIALIZE_FUN2_0_1(assign, func, Result, nonconst, To,   \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_FLOOR(func, To, From)                    \
  PPL_SPECIALIZE_FUN2_0_1(floor, func, Result, nonconst, To,    \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_CEIL(func, To, From)                     \
  PPL_SPECIALIZE_FUN2_0_1(ceil, func, Result, nonconst, To,     \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_TRUNC(func, To, From)                    \
  PPL_SPECIALIZE_FUN2_0_1(trunc, func, Result, nonconst, To,    \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_NEG(func, To, From)                      \
  PPL_SPECIALIZE_FUN2_0_1(neg, func, Result, nonconst, To,      \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_ABS(func, To, From)                      \
  PPL_SPECIALIZE_FUN2_0_1(abs, func, Result, nonconst, To,      \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_SQRT(func, To, From)                     \
  PPL_SPECIALIZE_FUN2_0_1(sqrt, func, Result, nonconst, To,     \
                          const, From, Rounding_Dir)
#define PPL_SPECIALIZE_ADD(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(add, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_SUB(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(sub, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_MUL(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(mul, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_DIV(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(div, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_REM(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(rem, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_IDIV(func, To, From1, From2)                     \
  PPL_SPECIALIZE_FUN3_0_1(idiv, func, Result, nonconst, To,             \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_ADD_2EXP(func, To, From)                         \
  PPL_SPECIALIZE_FUN2_0_2(add_2exp, func, Result, nonconst, To,         \
                          const, From, unsigned int, Rounding_Dir)
#define PPL_SPECIALIZE_SUB_2EXP(func, To, From)                         \
  PPL_SPECIALIZE_FUN2_0_2(sub_2exp, func, Result, nonconst, To,         \
                          const, From, unsigned int, Rounding_Dir)
#define PPL_SPECIALIZE_MUL_2EXP(func, To, From)                         \
  PPL_SPECIALIZE_FUN2_0_2(mul_2exp, func, Result, nonconst, To,         \
                          const, From, unsigned int, Rounding_Dir)
#define PPL_SPECIALIZE_DIV_2EXP(func, To, From)                         \
  PPL_SPECIALIZE_FUN2_0_2(div_2exp, func, Result, nonconst, To,         \
                          const, From, unsigned int, Rounding_Dir)
#define PPL_SPECIALIZE_SMOD_2EXP(func, To, From)                        \
  PPL_SPECIALIZE_FUN2_0_2(smod_2exp, func, Result, nonconst, To,        \
                          const, From, unsigned int, Rounding_Dir)
#define PPL_SPECIALIZE_UMOD_2EXP(func, To, From)                        \
  PPL_SPECIALIZE_FUN2_0_2(umod_2exp, func, Result, nonconst, To,        \
                          const, From, unsigned int, Rounding_Dir)
#define PPL_SPECIALIZE_ADD_MUL(func, To, From1, From2)                  \
  PPL_SPECIALIZE_FUN3_0_1(add_mul, func, Result, nonconst, To,          \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_SUB_MUL(func, To, From1, From2)                  \
  PPL_SPECIALIZE_FUN3_0_1(sub_mul, func, Result, nonconst, To,          \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_GCD(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(gcd, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_GCDEXT(func, To1, From1, From2, To2, To3)        \
  PPL_SPECIALIZE_FUN5_0_1(gcdext, func, Result, nonconst, To1,          \
                          nonconst, To2, nonconst, To3,                 \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_LCM(func, To, From1, From2)                      \
  PPL_SPECIALIZE_FUN3_0_1(lcm, func, Result, nonconst, To,              \
                          const, From1, const, From2, Rounding_Dir)
#define PPL_SPECIALIZE_INPUT(func, Type)                        \
  PPL_SPECIALIZE_FUN1_0_2(input, func, Result, nonconst, Type,  \
                          std::istream&, Rounding_Dir)
#define PPL_SPECIALIZE_OUTPUT(func, Type)                       \
  PPL_SPECIALIZE_FUN1_1_2(output, func, Result, std::ostream&,  \
                          const, Type,                          \
                          const Numeric_Format&, Rounding_Dir)


PPL_DECLARE_FUN2_0_0(copy,
                     void, nonconst, Type1, const, Type2)
PPL_DECLARE_FUN1_0_0(sgn,
                     Result_Relation, const, From)
PPL_DECLARE_FUN2_0_0(cmp,
                     Result_Relation, const, Type1, const, Type2)
PPL_DECLARE_FUN1_0_3(classify,
                     Result, const, Type, bool, bool, bool)
PPL_DECLARE_FUN1_0_0(is_nan,
                     bool, const, Type)
PPL_DECLARE_FUN1_0_0(is_minf,
                     bool, const, Type)
PPL_DECLARE_FUN1_0_0(is_pinf,
                     bool, const, Type)
PPL_DECLARE_FUN1_0_0(is_int,
                     bool, const, Type)
PPL_DECLARE_FUN1_0_2(assign_special,
                     Result, nonconst, Type, Result_Class, Rounding_Dir)
PPL_DECLARE_FUN1_0_2(construct_special,
                     Result, nonconst, Type, Result_Class, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(construct,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(assign,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(floor,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(ceil,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(trunc,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(neg,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(abs,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN2_0_1(sqrt,
                     Result, nonconst, To, const, From, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(add,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(sub,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(mul,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(div,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(rem,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(idiv,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN2_0_2(add_2exp,
                     Result, nonconst, To,
                     const, From, unsigned int, Rounding_Dir)
PPL_DECLARE_FUN2_0_2(sub_2exp,
                     Result, nonconst, To,
                     const, From, unsigned int, Rounding_Dir)
PPL_DECLARE_FUN2_0_2(mul_2exp,
                     Result, nonconst, To,
                     const, From, unsigned int, Rounding_Dir)
PPL_DECLARE_FUN2_0_2(div_2exp,
                     Result, nonconst, To,
                     const, From, unsigned int, Rounding_Dir)
PPL_DECLARE_FUN2_0_2(smod_2exp,
                     Result, nonconst, To,
                     const, From, unsigned int, Rounding_Dir)
PPL_DECLARE_FUN2_0_2(umod_2exp,
                     Result, nonconst, To,
                     const, From, unsigned int, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(add_mul,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(sub_mul,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(gcd,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN5_0_1(gcdext,
                     Result, nonconst, To1, nonconst, To2, nonconst, To3,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN3_0_1(lcm,
                     Result, nonconst, To,
                     const, From1, const, From2, Rounding_Dir)
PPL_DECLARE_FUN1_0_2(input,
                     Result, nonconst, Type, std::istream&, Rounding_Dir)
PPL_DECLARE_FUN1_1_2(output,
                     Result, std::ostream&, const, Type,
                     const Numeric_Format&, Rounding_Dir)

#undef PPL_DECLARE_FUN1_0_0
#undef PPL_DECLARE_FUN1_0_1
#undef PPL_DECLARE_FUN1_0_2
#undef PPL_DECLARE_FUN1_0_3
#undef PPL_DECLARE_FUN1_1_1
#undef PPL_DECLARE_FUN1_1_2
#undef PPL_DECLARE_FUN1_2_2
#undef PPL_DECLARE_FUN2_0_0
#undef PPL_DECLARE_FUN2_0_1
#undef PPL_DECLARE_FUN2_0_2
#undef PPL_DECLARE_FUN3_0_1
#undef PPL_DECLARE_FUN5_0_1

template <typename Policy, typename To>
Result round(To& to, Result r, Rounding_Dir dir);

Result input_mpq(mpq_class& to, std::istream& is);

std::string float_mpq_to_string(mpq_class& q);

} // namespace Checked

struct Minus_Infinity {
  static const Result_Class vclass = VC_MINUS_INFINITY;
};
struct Plus_Infinity {
  static const Result_Class vclass = VC_PLUS_INFINITY;
};
struct Not_A_Number {
  static const Result_Class vclass = VC_NAN;
};

template <typename T>
struct Is_Special : public False { };

template <>
struct Is_Special<Minus_Infinity> : public True {};

template <>
struct Is_Special<Plus_Infinity> : public True {};

template <>
struct Is_Special<Not_A_Number> : public True {};

extern Minus_Infinity MINUS_INFINITY;
extern Plus_Infinity PLUS_INFINITY;
extern Not_A_Number NOT_A_NUMBER;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
struct Checked_Number_Transparent_Policy {
  //! Do not check for overflowed result.
  const_bool_nodef(check_overflow, false);

  //! Do not check for attempts to add infinities with different sign.
  const_bool_nodef(check_inf_add_inf, false);

  //! Do not check for attempts to subtract infinities with same sign.
  const_bool_nodef(check_inf_sub_inf, false);

  //! Do not check for attempts to multiply infinities by zero.
  const_bool_nodef(check_inf_mul_zero, false);

  //! Do not check for attempts to divide by zero.
  const_bool_nodef(check_div_zero, false);

  //! Do not check for attempts to divide infinities.
  const_bool_nodef(check_inf_div_inf, false);

  //! Do not check for attempts to compute remainder of infinities.
  const_bool_nodef(check_inf_mod, false);

  //! Do not check for attempts to take the square root of a negative number.
  const_bool_nodef(check_sqrt_neg, false);

  //! Handle not-a-number special value if \p T has it.
  const_bool_nodef(has_nan, std::numeric_limits<T>::has_quiet_NaN);

  //! Handle infinity special values if \p T have them.
  const_bool_nodef(has_infinity, std::numeric_limits<T>::has_infinity);

  /*! \brief
    The checked number can always be safely converted to the
    underlying type \p T and vice-versa.
  */
  const_bool_nodef(convertible, true);

  //! Do not honor requests to check for FPU inexact results.
  const_bool_nodef(fpu_check_inexact, false);

  //! Do not make extra checks to detect FPU NaN results.
  const_bool_nodef(fpu_check_nan_result, false);

  /*! \brief
    For constructors, by default use the same rounding used by
    underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_CONSTRUCTOR = ROUND_NATIVE;

  /*! \brief
    For overloaded operators (operator+(), operator-(), ...), by
    default use the same rounding used by the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_OPERATOR = ROUND_NATIVE;

  /*! \brief
    For input functions, by default use the same rounding used by
    the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_INPUT = ROUND_NATIVE;

  /*! \brief
    For output functions, by default use the same rounding used by
    the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_OUTPUT = ROUND_NATIVE;

  /*! \brief
    For all other functions, by default use the same rounding used by
    the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_FUNCTION = ROUND_NATIVE;

  /*! \brief
    Handles \p r: called by all constructors, operators and functions that
    do not return a Result value.
  */
  static void handle_result(Result r);
};

} // namespace Parma_Polyhedra_Library

#define CHECK_P(cond, check) ((cond) ? (check) : (assert(!(check)), false))

#include "checked_inlines.hh"
#include "checked_int_inlines.hh"
#include "checked_float_inlines.hh"
#include "checked_mpz_inlines.hh"
#include "checked_mpq_inlines.hh"
#include "checked_ext_inlines.hh"

#undef nonconst
#ifdef PPL_SAVED_nonconst
#define nonconst PPL_SAVED_nonconst
#undef PPL_SAVED_nonconst
#endif

#undef PPL_FUNCTION_CLASS
#undef PPL_NAN

#endif // !defined(PPL_checked_defs_hh)
