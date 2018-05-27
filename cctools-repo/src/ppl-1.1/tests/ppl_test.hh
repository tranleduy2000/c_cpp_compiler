/* Header file for test programs.
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

#ifndef PPL_ppl_test_hh
#define PPL_ppl_test_hh 1

#include "ppl_header.hh"
#include "Random_Number_Generator_defs.hh"
#include <stdexcept>
#include <sstream>
#include <list>
#include <map>
#include <iterator>
#include <string>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <cstdlib>

#ifndef NOISY
#define NOISY 0
#endif

#ifndef VERY_NOISY
#define VERY_NOISY 0
#endif

#define TRY try

#define CATCH \
catch (const std::overflow_error& e) { \
  std::cerr << "arithmetic overflow (" << e.what() << ")" \
            << std::endl; \
  exit(1); \
} \
catch (const std::exception& e) { \
  std::cerr << "std::exception caught: " \
            << e.what() << " (type == " << typeid(e).name() << ")" \
            << std::endl; \
  exit(1); \
}

#define BEGIN_MAIN                                       \
int                                                      \
main() try {                                             \
  set_handlers();                                        \
  bool succeeded = false;                                \
  bool overflow = false;                                 \
  (void) overflow;                                       \
  std::list<std::string> failed_tests;                   \
  std::list<std::string> unexpectedly_succeeded_tests;

#define END_MAIN                                                        \
  if (!failed_tests.empty()) {                                          \
    std::cerr << "tests failed: ";                                      \
    std::copy(failed_tests.begin(),                                     \
              failed_tests.end(),                                       \
              std::ostream_iterator<std::string>(std::cerr, " "));      \
    std::cerr << std::endl;                                             \
    return 1;                                                           \
  }                                                                     \
  if (!unexpectedly_succeeded_tests.empty()) {                          \
    std::cerr << "tests unexpectedly succeeded: ";                      \
    std::copy(unexpectedly_succeeded_tests.begin(),                     \
              unexpectedly_succeeded_tests.end(),                       \
              std::ostream_iterator<std::string>(std::cerr, " "));      \
    std::cerr << std::endl;                                             \
    return 1;                                                           \
  }                                                                     \
  return 0;                                                             \
}                                                                       \
catch (const std::overflow_error& e) {                                  \
  std::cerr << "arithmetic overflow (" << e.what() << ")"               \
            << std::endl;                                               \
  exit(1);                                                              \
}                                                                       \
catch (const std::exception& e) {                                       \
  std::cerr << "std::exception caught: "                                \
            << e.what() << " (type == " << typeid(e).name() << ")"      \
            << std::endl;                                               \
  exit(1);                                                              \
}

#define ANNOUNCE_TEST(test)              \
  nout << "\n=== " #test " ===" << std::endl

#define RUN_TEST(test)                                                  \
  try {                                                                 \
    overflow = false;                                                   \
    succeeded = test();                                                 \
  }                                                                     \
  catch (const std::overflow_error& e) {                                \
    nout << "arithmetic overflow (" << e.what() << ")"                  \
         << std::endl;                                                  \
    overflow = true;                                                    \
    succeeded = false;                                                  \
  }                                                                     \
  catch (const std::exception& e) {                                     \
    nout << "std::exception caught: "                                   \
         << e.what() << " (type == " << typeid(e).name() << ")"         \
         << std::endl;                                                  \
    succeeded = false;                                                  \
  }                                                                     \
  catch (...) {                                                         \
    nout << "unknown exception caught"                                  \
         << std::endl;                                                  \
    succeeded = false;                                                  \
  }

#define DO_TEST(test)                    \
  ANNOUNCE_TEST(test);                   \
  RUN_TEST(test);                        \
  if (!succeeded)                        \
    failed_tests.push_back(#test);

#define DO_TEST_F(test)                                 \
  ANNOUNCE_TEST(test);                                  \
  RUN_TEST(test);                                       \
  if (succeeded)                                        \
    unexpectedly_succeeded_tests.push_back(#test);

#define DO_TEST_OVERFLOW(test)                          \
  ANNOUNCE_TEST(test);                                  \
  RUN_TEST(test);                                       \
  if (succeeded)                                        \
    unexpectedly_succeeded_tests.push_back(#test);      \
  else if (!overflow)                                   \
    failed_tests.push_back(#test);

#define DO_TEST_MAY_OVERFLOW_IF_INEXACT(test, shape)                    \
  ANNOUNCE_TEST(test);                                                  \
  RUN_TEST(test);                                                       \
  if (!succeeded)                                                       \
    if (!overflow || has_exact_coefficient_type(shape(0, EMPTY)))       \
      failed_tests.push_back(#test);


// Macros for arbitrary combination of preprocessor conditions.

#define PPL_CPP_AND_false_false false
#define PPL_CPP_AND_false_true false
#define PPL_CPP_AND_true_false false
#define PPL_CPP_AND_true_true true
#define PPL_CPP_AND_(x,y) PPL_CPP_AND_ ## x ## _ ## y
#define PPL_CPP_AND(x,y) PPL_CPP_AND_(x, y)

#define PPL_CPP_OR_false_false false
#define PPL_CPP_OR_false_true true
#define PPL_CPP_OR_true_false true
#define PPL_CPP_OR_true_true true
#define PPL_CPP_OR_(x,y) PPL_CPP_OR_ ## x ## _ ## y
#define PPL_CPP_OR(x,y) PPL_CPP_OR_(x, y)

#define PPL_CPP_NOT_false true
#define PPL_CPP_NOT_true false
#define PPL_CPP_NOT_(x) PPL_CPP_NOT_ ## x
#define PPL_CPP_NOT(x) PPL_CPP_NOT_(x)

#define PPL_CPP_VAL_true_ true
#define PPL_CPP_VAL_false_ false
#define PPL_CPP_VAL_false(v) PPL_CPP_VAL_true
#define PPL_CPP_VAL_b(v) PPL_CPP_VAL_false
#define PPL_CPP_VAL_a(v) PPL_CPP_VAL_b(v)

#define PPL_CPP_IS_NEGx_arg_neg 0)(0
#define PPL_CPP_IS_NEGx(v) PPL_CPP_VAL_a(PPL_CPP_IS_NEGx_arg_##v)

#define PPL_CPP_IS_NEG__(v) v ## _
#define PPL_CPP_IS_NEG_(v) PPL_CPP_IS_NEG__(v)
#define PPL_CPP_IS_NEG(v) PPL_CPP_IS_NEG_(PPL_CPP_IS_NEGx(v))

#define PPL_CPP_IS_ZEROx_arg_0 0)(0
#define PPL_CPP_IS_ZEROx(v) PPL_CPP_VAL_a(PPL_CPP_IS_ZEROx_arg_##v)

#define PPL_CPP_IS_ZERO__(v) v ## _
#define PPL_CPP_IS_ZERO_(v) PPL_CPP_IS_ZERO__(v)
#define PPL_CPP_IS_ZERO(v) PPL_CPP_IS_ZERO_(PPL_CPP_IS_ZEROx(v))

#define PPL_CPP_DECR_neg neg
#define PPL_CPP_DECR_0 neg
#define PPL_CPP_DECR_1 0
#define PPL_CPP_DECR_2 1
#define PPL_CPP_DECR_3 2
#define PPL_CPP_DECR_4 3
#define PPL_CPP_DECR_5 4
#define PPL_CPP_DECR_6 5
#define PPL_CPP_DECR_7 6
#define PPL_CPP_DECR_8 7
#define PPL_CPP_DECR_9 8

#define PPL_CPP_DECR_(x) PPL_CPP_DECR_ ## x
#define PPL_CPP_DECR(x) PPL_CPP_DECR_(x)

#define PPL_CPP_SUB_0(x) x
#define PPL_CPP_SUB_1(x) PPL_CPP_DECR(x)
#define PPL_CPP_SUB_2(x) PPL_CPP_DECR(PPL_CPP_SUB_1(x))
#define PPL_CPP_SUB_3(x) PPL_CPP_DECR(PPL_CPP_SUB_2(x))
#define PPL_CPP_SUB_4(x) PPL_CPP_DECR(PPL_CPP_SUB_3(x))
#define PPL_CPP_SUB_5(x) PPL_CPP_DECR(PPL_CPP_SUB_4(x))
#define PPL_CPP_SUB_6(x) PPL_CPP_DECR(PPL_CPP_SUB_5(x))
#define PPL_CPP_SUB_7(x) PPL_CPP_DECR(PPL_CPP_SUB_6(x))
#define PPL_CPP_SUB_8(x) PPL_CPP_DECR(PPL_CPP_SUB_7(x))
#define PPL_CPP_SUB_9(x) PPL_CPP_DECR(PPL_CPP_SUB_8(x))

#define PPL_CPP_SUB_(x, y) PPL_CPP_SUB_ ## y (x)
#define PPL_CPP_SUB(x, y) PPL_CPP_SUB_(x, y)

#define PPL_CPP_LT(x, y) PPL_CPP_IS_NEG(PPL_CPP_SUB(x, y))
#define PPL_CPP_GT(x, y) PPL_CPP_LT(y, x)
#define PPL_CPP_LE(x, y) PPL_CPP_NOT(PPL_CPP_LT(y, x))
#define PPL_CPP_GE(x, y) PPL_CPP_NOT(PPL_CPP_LT(x, y))
#define PPL_CPP_EQ(x, y) PPL_CPP_IS_ZERO(PPL_CPP_SUB(x, y))
#define PPL_CPP_NE(x, y) PPL_CPP_NOT(PPL_CPP_EQ(x,y))

#define PPL_CPP_LOG2_64 6
#define PPL_CPP_LOG2_32 5
#define PPL_CPP_LOG2_16 4
#define PPL_CPP_LOG2_8  3
#define PPL_CPP_LOG2_0  neg

#define PPL_CPP_LOG2_(x) PPL_CPP_LOG2_ ## x
#define PPL_CPP_LOG2(x) PPL_CPP_LOG2_(x)

#define COND_MACRO_2(prefix, v) prefix ## _ ## v
#define COND_MACRO_1(prefix, v) COND_MACRO_2(prefix, v)
#define COND_MACRO(prefix, expr) COND_MACRO_1(prefix, expr)


#define PPL_CPP_LOGBITS PPL_CPP_LOG2(PPL_COEFFICIENT_BITS)

#define COND_F64 PPL_CPP_LT(PPL_CPP_LOGBITS, 0)
#define COND_F32 PPL_CPP_OR(PPL_CPP_GT(PPL_CPP_LOGBITS, 5), \
                            PPL_CPP_LT(PPL_CPP_LOGBITS, 0))
#define COND_F16 PPL_CPP_OR(PPL_CPP_GT(PPL_CPP_LOGBITS, 4), \
                            PPL_CPP_LT(PPL_CPP_LOGBITS, 0))
#define COND_F8  PPL_CPP_OR(PPL_CPP_GT(PPL_CPP_LOGBITS, 3), \
                            PPL_CPP_LT(PPL_CPP_LOGBITS, 0))

#ifndef NDEBUG
# define COND_ASSERT_OFF false
#else
# define COND_ASSERT_OFF true
#endif

#define COND_SUCC_64_ONLY \
  PPL_CPP_AND(PPL_CPP_EQ(PPL_CPP_LOGBITS, 6), COND_ASSERT_OFF)
#define COND_SUCC_32_ONLY \
  PPL_CPP_AND(PPL_CPP_EQ(PPL_CPP_LOGBITS, 5), COND_ASSERT_OFF)
#define COND_SUCC_16_ONLY \
  PPL_CPP_AND(PPL_CPP_EQ(PPL_CPP_LOGBITS, 4), COND_ASSERT_OFF)
#define COND_SUCC_8_ONLY \
  PPL_CPP_AND(PPL_CPP_EQ(PPL_CPP_LOGBITS, 3), COND_ASSERT_OFF)

#define COND_F64A PPL_CPP_OR(COND_F64, COND_SUCC_64_ONLY)
#define COND_F32A PPL_CPP_OR(COND_F32, COND_SUCC_32_ONLY)
#define COND_F16A PPL_CPP_OR(COND_F16, COND_SUCC_16_ONLY)
#define COND_F8A  PPL_CPP_OR(COND_F8,  COND_SUCC_8_ONLY)


#define COND_DO_TEST_false(test) DO_TEST_OVERFLOW(test)
#define COND_DO_TEST_true(test)  DO_TEST(test)
#define COND_DO_TEST(cond, test) COND_MACRO(COND_DO_TEST, cond)(test)

#define DO_TEST_F64(test)  COND_DO_TEST(COND_F64, test)
#define DO_TEST_F64A(test) COND_DO_TEST(COND_F64A, test)
#define DO_TEST_F32(test)  COND_DO_TEST(COND_F32, test)
#define DO_TEST_F32A(test) COND_DO_TEST(COND_F32A, test)
#define DO_TEST_F16(test)  COND_DO_TEST(COND_F16, test)
#define DO_TEST_F16A(test) COND_DO_TEST(COND_F16A, test)
#define DO_TEST_F8(test)   COND_DO_TEST(COND_F8, test)
#define DO_TEST_F8A(test)  COND_DO_TEST(COND_F8A, test)


#define COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT_false(test, shape) \
  DO_TEST_OVERFLOW(test)
#define COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT_true(test, shape)  \
  DO_TEST_MAY_OVERFLOW_IF_INEXACT(test, shape)
#define COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(cond, test, shape) \
  COND_MACRO(COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT, cond)(test, shape)

#define DO_TEST_F64_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F64, test, shape)
#define DO_TEST_F64A_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F64A, test, shape)
#define DO_TEST_F32_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F32, test, shape)
#define DO_TEST_F32A_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F32A, test, shape)
#define DO_TEST_F16_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F16, test, shape)
#define DO_TEST_F16A_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F16A, test, shape)
#define DO_TEST_F8_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F8, test, shape)
#define DO_TEST_F8A_MAY_OVERFLOW_IF_INEXACT(test, shape) \
  COND_DO_TEST_MAY_OVERFLOW_IF_INEXACT(COND_F8A, test, shape)

// Encoding floating point formats.
#define PPL_CPP_FP_FORMAT_float                1
#define PPL_CPP_FP_FORMAT_double               2
#define PPL_CPP_FP_FORMAT_long_double          3
#define PPL_CPP_FP_FORMAT_float_ieee754_single 4
#define PPL_CPP_FP_FORMAT_float_ieee754_double 5
#define PPL_CPP_FP_FORMAT_float_ieee754_quad   6
#define PPL_CPP_FP_FORMAT_(format) PPL_CPP_FP_FORMAT_ ## format
#define PPL_CPP_FP_FORMAT(format) PPL_CPP_FP_FORMAT_(format)


// Turn s into a string: PPL_TEST_STR(x + y) => "x + y".
#define PPL_TEST_STR(s) #s

// Turn the expansion of s into a string: PPL_TEST_XSTR(x) => "s expanded".
#define PPL_TEST_XSTR(s) PPL_TEST_STR(s)


#ifdef DERIVED_TEST
#define C_Polyhedron NNC_Polyhedron
#endif

#ifndef BOX_INSTANCE
#define BOX_INSTANCE rt_r_oc
#endif

#ifndef BD_SHAPE_INSTANCE
#define BD_SHAPE_INSTANCE mpq_class
#endif

#ifndef OCTAGONAL_SHAPE_INSTANCE
#define OCTAGONAL_SHAPE_INSTANCE mpq_class
#endif

namespace Parma_Polyhedra_Library {

namespace Test {

static bool
check_noisy(const char*
#if PPL_HAVE_DECL_GETENV || NOISY || VERY_NOISY
            environment_variable
#endif
) {
#if PPL_HAVE_DECL_GETENV
  return getenv(environment_variable) != 0;
#else
#if NOISY
  if (strcmp(environment_variable, "PPL_NOISY_TESTS") == 0)
    return true;
#endif
#if VERY_NOISY
  if (strcmp(environment_variable, "PPL_VERY_NOISY_TESTS") == 0)
    return true;
#endif
  return false;
#endif // !PPL_HAVE_DECL_GETENV
}

template<typename CharT, typename Traits = std::char_traits<CharT> >
class nullbuf : public std::basic_streambuf<CharT, Traits> {
protected:
  virtual typename Traits::int_type overflow(typename Traits::int_type c) {
    return Traits::not_eof(c);
  }
};

template <class CharT, class Traits = std::char_traits<CharT> >
class noisy_ostream : public std::basic_ostream<CharT, Traits> {
private:
  nullbuf<CharT, Traits> black_hole;

public:
  noisy_ostream(const std::basic_ostream<CharT, Traits>& os,
                const char* environment_variable)
    : std::basic_ostream<CharT, Traits>(check_noisy(environment_variable)
                                        ? os.rdbuf()
                                        : &black_hole) {
  }
};

static noisy_ostream<char> nout(std::cout, "PPL_NOISY_TESTS");
static noisy_ostream<char> vnout(std::cout, "PPL_VERY_NOISY_TESTS");

void
set_handlers();

//! Utility typedef to allow a macro argument to denote the long double type.
typedef long double long_double;

struct Floating_Real_Open_Interval_Info_Policy {
  const_bool_nodef(store_special, false);
  const_bool_nodef(store_open, true);
  const_bool_nodef(cache_empty, true);
  const_bool_nodef(cache_singleton, true);
  const_bool_nodef(cache_normalized, false);
  const_int_nodef(next_bit, 0);
  const_bool_nodef(may_be_empty, true);
  const_bool_nodef(may_contain_infinity, false);
  const_bool_nodef(check_empty_result, false);
  const_bool_nodef(check_inexact, false);
};

struct Floating_Real_Closed_Interval_Info_Policy {
  const_bool_nodef(store_special, false);
  const_bool_nodef(store_open, false);
  const_bool_nodef(cache_empty, false);
  const_bool_nodef(cache_singleton, true);
  const_bool_nodef(cache_normalized, false);
  const_int_nodef(next_bit, 0);
  const_bool_nodef(may_be_empty, false);
  const_bool_nodef(may_contain_infinity, false);
  const_bool_nodef(check_empty_result, false);
  const_bool_nodef(check_inexact, false);
};

typedef Interval_Info_Bitset<unsigned int,
                             Floating_Real_Open_Interval_Info_Policy> Floating_Real_Open_Interval_Info;

typedef Interval<float, Floating_Real_Open_Interval_Info> fl_r_oc;
typedef Interval<double, Floating_Real_Open_Interval_Info> db_r_oc;
typedef Interval<long double, Floating_Real_Open_Interval_Info> ld_r_oc;

struct Rational_Real_Open_Interval_Info_Policy {
  const_bool_nodef(store_special, true);
  const_bool_nodef(store_open, true);
  const_bool_nodef(cache_empty, true);
  const_bool_nodef(cache_singleton, true);
  const_bool_nodef(cache_normalized, false);
  const_int_nodef(next_bit, 0);
  const_bool_nodef(may_be_empty, true);
  const_bool_nodef(may_contain_infinity, false);
  const_bool_nodef(check_empty_result, false);
  const_bool_nodef(check_inexact, false);
};

typedef Interval_Info_Bitset<unsigned int,
                             Rational_Real_Open_Interval_Info_Policy> Rational_Real_Open_Interval_Info;

typedef Interval<mpq_class, Rational_Real_Open_Interval_Info> rt_r_oc;

//! The incarnation of Box under test.
typedef Box<BOX_INSTANCE> TBox;

//! The incarnation of BD_Shape under test.
typedef BD_Shape<BD_SHAPE_INSTANCE> TBD_Shape;

//! The incarnation of Octagonal_Shape under test.
typedef Octagonal_Shape<OCTAGONAL_SHAPE_INSTANCE> TOctagonal_Shape;

// For floating point analysis.
#ifdef ANALYZER_FP_FORMAT
#ifdef ANALYZED_FP_FORMAT
//! The type of an interval with floating point boundaries.
typedef Interval<ANALYZER_FP_FORMAT,
                 Floating_Real_Open_Interval_Info> FP_Interval;

//! The type of an interval linear form.
typedef Linear_Form<FP_Interval> FP_Linear_Form;

//! The type of an interval abstract store.
typedef Box<FP_Interval> FP_Interval_Abstract_Store;

//! The type of a linear form abstract store.
typedef std::map<dimension_type, FP_Linear_Form>
                                 FP_Linear_Form_Abstract_Store;

/*! \brief
  The incarnation of BD_Shape under test for analyzing
  floating point computations.
*/
typedef BD_Shape<ANALYZER_FP_FORMAT> FP_BD_Shape;

/*! \brief
  The incarnation of Octagonal_Shape under test for analyzing
  floating point computations.
*/
typedef Octagonal_Shape<ANALYZER_FP_FORMAT> FP_Octagonal_Shape;
#endif // ANALYZED_FP_FORMAT
#endif // ANALYZER_FP_FORMAT

template <typename Shape>
inline bool
has_exact_coefficient_type(const Shape&) {
  return std::numeric_limits<typename Shape::coefficient_type>::is_exact;
}

template <typename Interval>
inline bool
has_exact_coefficient_type(const Box<Interval>&) {
  return std::numeric_limits<typename Interval::boundary_type>::is_exact;
}

bool
check_distance(const Checked_Number<mpq_class, Extended_Number_Policy>& d,
               const char* max_d_s, const char* d_name);

template <typename T>
bool
check_result_i(const BD_Shape<T>& computed_result,
               const BD_Shape<mpq_class>& known_result,
               const char* max_r_d_s,
               const char* max_e_d_s,
               const char* max_l_d_s) {
  BD_Shape<mpq_class> q_computed_result(computed_result);
  // Handle in a more efficient way the case where equality is expected.
  if (max_r_d_s == 0 && max_e_d_s == 0 && max_l_d_s == 0) {
    if (q_computed_result != known_result) {
      using IO_Operators::operator<<;
      nout << "Equality does not hold:"
           << "\ncomputed result is\n"
           << q_computed_result
           << "\nknown result is\n"
           << known_result
           << std::endl;
      return false;
    }
    else
      return true;
  }

  if (!q_computed_result.contains(known_result)) {
    using IO_Operators::operator<<;
    nout << "Containment does not hold:"
         << "\ncomputed result is\n"
         << q_computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
    return false;
  }

  Checked_Number<mpq_class, Extended_Number_Policy> r_d;
  rectilinear_distance_assign(r_d, known_result, q_computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> e_d;
  euclidean_distance_assign(e_d, known_result, q_computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> l_d;
  l_infinity_distance_assign(l_d, known_result, q_computed_result, ROUND_UP);
  bool ok_r = check_distance(r_d, max_r_d_s, "rectilinear");
  bool ok_e = check_distance(e_d, max_e_d_s, "euclidean");
  bool ok_l = check_distance(l_d, max_l_d_s, "l_infinity");
  bool ok = ok_r && ok_e && ok_l;
  if (!ok) {
    using IO_Operators::operator<<;
    nout << "Computed result is\n"
         << q_computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
  }
  return ok;
}

template <typename T>
bool
check_result(const BD_Shape<T>& computed_result,
             const BD_Shape<mpq_class>& known_result,
             const char* max_r_d_s,
             const char* max_e_d_s,
             const char* max_l_d_s) {
  return std::numeric_limits<T>::is_integer
    ? check_result_i(computed_result, known_result,
                     "+inf", "+inf", "+inf")
    : check_result_i(computed_result, known_result,
                     max_r_d_s, max_e_d_s, max_l_d_s);
}

template <typename T>
bool
check_result(const BD_Shape<T>& computed_result,
             const BD_Shape<T>& known_result) {
  if (computed_result == known_result)
    return true;
  else {
    using IO_Operators::operator<<;
    nout << "Equality does not hold:"
         << "\ncomputed result is\n"
         << computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
    return false;
  }
}

template <typename T>
bool
check_result(const BD_Shape<T>& computed_result,
             const BD_Shape<mpq_class>& known_result) {
  return std::numeric_limits<T>::is_integer
    ? check_result_i(computed_result, known_result, "+inf", "+inf", "+inf")
    : check_result_i(computed_result, known_result, 0, 0, 0);
}

bool
check_result(const BD_Shape<mpq_class>& computed_result,
             const BD_Shape<mpq_class>& known_result);

template <typename T>
bool
check_result_i(const Octagonal_Shape<T>& computed_result,
               const Octagonal_Shape<mpq_class>& known_result,
               const char* max_r_d_s,
               const char* max_e_d_s,
               const char* max_l_d_s) {
  Octagonal_Shape<mpq_class> q_computed_result(computed_result);
  // Handle in a more efficient way the case where equality is expected.
  if (max_r_d_s == 0 && max_e_d_s == 0 && max_l_d_s == 0) {
    if (q_computed_result != known_result) {
      using IO_Operators::operator<<;
      nout << "Equality does not hold:"
           << "\ncomputed result is\n"
           << q_computed_result
           << "\nknown result is\n"
           << known_result
           << std::endl;
      return false;
    }
    else
      return true;
  }

  if (!q_computed_result.contains(known_result)) {
    using IO_Operators::operator<<;
    nout << "Containment does not hold:"
         << "\ncomputed result is\n"
         << q_computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
    return false;
  }

  Checked_Number<mpq_class, Extended_Number_Policy> r_d;
  rectilinear_distance_assign(r_d, known_result, q_computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> e_d;
  euclidean_distance_assign(e_d, known_result, q_computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> l_d;
  l_infinity_distance_assign(l_d, known_result, q_computed_result, ROUND_UP);
  bool ok_r = check_distance(r_d, max_r_d_s, "rectilinear");
  bool ok_e = check_distance(e_d, max_e_d_s, "euclidean");
  bool ok_l = check_distance(l_d, max_l_d_s, "l_infinity");
  bool ok = ok_r && ok_e && ok_l;
  if (!ok) {
    using IO_Operators::operator<<;
    nout << "Computed result is\n"
         << q_computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
  }
  return ok;
}

template <typename T>
bool
check_result(const Octagonal_Shape<T>& computed_result,
             const Octagonal_Shape<mpq_class>& known_result,
             const char* max_r_d_s,
             const char* max_e_d_s,
             const char* max_l_d_s) {
  return std::numeric_limits<T>::is_integer
    ? check_result_i(computed_result, known_result,
                     "+inf", "+inf", "+inf")
    : check_result_i(computed_result, known_result,
                     max_r_d_s, max_e_d_s, max_l_d_s);
}

template <>
inline bool
check_result(const Octagonal_Shape<mpq_class>& computed_result,
             const Octagonal_Shape<mpq_class>& known_result,
             const char*,
             const char*,
             const char*) {
  return check_result_i(computed_result, known_result,
                        0, 0, 0);
}

template <typename T>
bool
check_result(const Octagonal_Shape<T>& computed_result,
             const Octagonal_Shape<mpq_class>& known_result) {
  return std::numeric_limits<T>::is_integer
    ? check_result_i(computed_result, known_result, "+inf", "+inf", "+inf")
    : check_result_i(computed_result, known_result, 0, 0, 0);
}


template <typename Interval>
bool
check_result_i(const Box<Interval>& computed_result,
               const Rational_Box& known_result,
               const char* max_r_d_s,
               const char* max_e_d_s,
               const char* max_l_d_s) {
  Rational_Box q_computed_result(computed_result);
  // Handle in a more efficient way the case where equality is expected.
  if (max_r_d_s == 0 && max_e_d_s == 0 && max_l_d_s == 0) {
    if (q_computed_result != known_result) {
      using IO_Operators::operator<<;
      nout << "Equality does not hold:"
           << "\ncomputed result is\n"
           << q_computed_result
           << "\nknown result is\n"
           << known_result
           << std::endl;
      return false;
    }
    else
      return true;
  }

  if (!q_computed_result.contains(known_result)) {
    using IO_Operators::operator<<;
    nout << "Containment does not hold:"
         << "\ncomputed result is\n"
         << q_computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
    nout << "Individual dimensions where containment does not hold"
         << "\n(Variable: computed-result known-result):\n";
    for (dimension_type i = 0; i < computed_result.space_dimension(); ++i) {
      if (!q_computed_result.get_interval(Variable(i))
          .contains(known_result.get_interval(Variable(i)))) {
        using IO_Operators::operator<<;
        nout << Variable(i) << ": "
             << q_computed_result.get_interval(Variable(i))
             << ' '
             << known_result.get_interval(Variable(i))
             << std::endl;
      }
    }
    return false;
  }

  Checked_Number<mpq_class, Extended_Number_Policy> r_d;
  rectilinear_distance_assign(r_d, known_result, q_computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> e_d;
  euclidean_distance_assign(e_d, known_result, q_computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> l_d;
  l_infinity_distance_assign(l_d, known_result, q_computed_result, ROUND_UP);
  bool ok_r = check_distance(r_d, max_r_d_s, "rectilinear");
  bool ok_e = check_distance(e_d, max_e_d_s, "euclidean");
  bool ok_l = check_distance(l_d, max_l_d_s, "l_infinity");
  bool ok = ok_r && ok_e && ok_l;
  if (!ok) {
    using IO_Operators::operator<<;
    nout << "Computed result is\n"
         << q_computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
  }
  return ok;
}

template <typename Interval>
bool
check_result(const Box<Interval>& computed_result,
             const Rational_Box& known_result,
             const char* max_r_d_s,
             const char* max_e_d_s,
             const char* max_l_d_s) {
  return std::numeric_limits<typename Interval::boundary_type>::is_integer
    ? check_result_i(computed_result, known_result,
                     "+inf", "+inf", "+inf")
    : check_result_i(computed_result, known_result,
                     max_r_d_s, max_e_d_s, max_l_d_s);
}

template <typename Interval>
bool
check_result(const Box<Interval>& computed_result,
             const Box<Interval>& known_result) {
  if (computed_result == known_result)
    return true;
  else {
    using IO_Operators::operator<<;
    nout << "Equality does not hold:"
         << "\ncomputed result is\n"
         << computed_result
         << "\nknown result is\n"
         << known_result
         << std::endl;
    return false;
  }
}

template <typename Interval>
bool
check_result(const Box<Interval>& computed_result,
             const Rational_Box& known_result) {
  return std::numeric_limits<typename Interval::boundary_type>::is_integer
    ? check_result_i(computed_result, known_result, "+inf", "+inf", "+inf")
    : check_result_i(computed_result, known_result, 0, 0, 0);
}

bool
check_result(const Rational_Box& computed_result,
             const Rational_Box& known_result);

bool
check_result(const Generator& computed_result,
             const Generator& known_result,
             const char* max_r_d_s,
             const char* max_e_d_s,
             const char* max_l_d_s);

bool
check_result(const Checked_Number<mpq_class, Extended_Number_Policy>& computed,
             const Checked_Number<mpq_class, Extended_Number_Policy>& known,
             const char* max_r_d_s);

class FCAIBVP;

bool
operator==(const FCAIBVP& x, const FCAIBVP& y);

bool
operator!=(const FCAIBVP& x, const FCAIBVP& y);

std::ostream&
operator<<(std::ostream& s, const FCAIBVP& x);

/*! \brief
  A class for representing Finite Conjunctions of Attribute
  Independent Boolean Variable Properties.
*/
class FCAIBVP {
private:
  typedef size_t dim_t;

  typedef std::set<dim_t> Set;

  Set set;

public:
  FCAIBVP();

  explicit FCAIBVP(const Variable x);

  explicit FCAIBVP(const Variables_Set& y);

  FCAIBVP(const FCAIBVP& y, unsigned offset);

  memory_size_type total_memory_in_bytes() const;

  memory_size_type external_memory_in_bytes() const;

  bool is_top() const;

  bool is_bottom() const;

  bool definitely_entails(const FCAIBVP& y) const;

  void upper_bound_assign(const FCAIBVP& y);

  void difference_assign(const FCAIBVP& y);

  void meet_assign(const FCAIBVP& y);

  void weakening_assign(const FCAIBVP& y);

  static bool has_nontrivial_weakening();

  bool OK() const;

  friend std::ostream&
  Parma_Polyhedra_Library::Test::operator<<(std::ostream& s,
                                            const FCAIBVP& x);
};

inline
FCAIBVP::FCAIBVP()
  : set() {
}

inline
FCAIBVP::FCAIBVP(const Variable x)
  : set() {
  set.insert(x.id());
}

inline
FCAIBVP::FCAIBVP(const Variables_Set& y)
  : set() {
  for (Variables_Set::const_iterator i = y.begin(),
         y_end = y.end(); i != y_end; ++i)
    set.insert(*i);
}

inline
FCAIBVP::FCAIBVP(const FCAIBVP& y, unsigned offset)
  : set() {
  for (Set::const_iterator i = y.set.begin(),
         y_set_end = y.set.end(); i != y_set_end; ++i)
    set.insert(*i + offset);
}

inline memory_size_type
FCAIBVP::total_memory_in_bytes() const {
  return 1;
}

inline bool
FCAIBVP::is_top() const {
  return set.empty();
}

inline bool
FCAIBVP::is_bottom() const {
  return false;
}

inline bool
FCAIBVP::definitely_entails(const FCAIBVP& y) const{
  const FCAIBVP& x = *this;
  return std::includes(x.set.begin(), x.set.end(),
                       y.set.begin(), y.set.end());
}

inline void
FCAIBVP::upper_bound_assign(const FCAIBVP& y) {
  FCAIBVP& x = *this;
  FCAIBVP z;
  std::set_intersection(x.set.begin(), x.set.end(),
                        y.set.begin(), y.set.end(),
                        std::inserter(z.set, z.set.begin()));
  using std::swap;
  swap(x, z);
}

inline void
FCAIBVP::difference_assign(const FCAIBVP& y) {
  FCAIBVP& x = *this;
  FCAIBVP z;
  std::set_difference(x.set.begin(), x.set.end(),
                      y.set.begin(), y.set.end(),
                      std::inserter(z.set, z.set.begin()));
  using std::swap;
  swap(x, z);
}

inline void
FCAIBVP::meet_assign(const FCAIBVP& y) {
  set.insert(y.set.begin(), y.set.end());
}

inline void
FCAIBVP::weakening_assign(const FCAIBVP& y) {
  difference_assign(y);
}

inline bool
FCAIBVP::has_nontrivial_weakening() {
  return true;
}

inline bool
FCAIBVP::OK() const {
  return true;
}

inline bool
operator==(const FCAIBVP& x, const FCAIBVP& y) {
  return x.definitely_entails(y) && y.definitely_entails(x);
}

inline bool
operator!=(const FCAIBVP& x, const FCAIBVP& y) {
  return !(x == y);
}

void
print_constraint(const Constraint& c,
                 const std::string& intro = "",
                 std::ostream& s = nout);

void
print_constraints(const Constraint_System& cs,
                  const std::string& intro = "",
                  std::ostream& s = nout);

void
print_constraints(const Polyhedron& ph,
                  const std::string& intro = "",
                  std::ostream& s = nout);

#if 0
void
print_constraints(const Affine_Space& affs,
                  const std::string& intro = "",
                  std::ostream& s = nout);
#endif

template <typename Interval>
void
print_constraints(const Box<Interval>& box,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  if (!intro.empty())
    s << intro << std::endl;
  using IO_Operators::operator<<;
  s << box << std::endl;
}

template <typename T>
void
print_constraints(const BD_Shape<T>& bd,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  if (!intro.empty())
    s << intro << std::endl;
  using IO_Operators::operator<<;
  s << bd << std::endl;
}

template <typename T>
void
print_constraints(const Octagonal_Shape<T>& oc,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  if (!intro.empty())
    s << intro << std::endl;
  using IO_Operators::operator<<;
  s << oc << std::endl;
}

template <typename PH>
void
print_constraints(const Pointset_Powerset<PH>& pps,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  if (!intro.empty())
    s << intro << std::endl;
  using IO_Operators::operator<<;
  s << pps << std::endl;
}

template <typename PH>
void
print_congruences(const Pointset_Powerset<PH>& pps,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  if (!intro.empty())
    s << intro << std::endl;
  using IO_Operators::operator<<;
  s << pps << std::endl;
}

/*
template <typename PH>
void
print_constraints(const Pointset_Ask_Tell<PH>& pat,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  if (!intro.empty())
    s << intro << std::endl;
  using IO_Operators::operator<<;
  s << pat << std::endl;
}
*/

void
print_congruence(const Congruence& c,
                 const std::string& intro = "",
                 std::ostream& s = nout);

void
print_congruences(const Congruence_System& cgs,
                  const std::string& intro = "",
                  std::ostream& s = nout);

void
print_congruences(const Grid& gr,
                  const std::string& intro = "",
                  std::ostream& s = nout);

void
print_generator(const Generator& g,
                const std::string& intro = "",
                std::ostream& s = nout);

void
print_generator(const Grid_Generator& g,
                const std::string& intro = "",
                std::ostream& s = nout);

void
print_generators(const Generator_System& gs,
                 const std::string& intro = "",
                 std::ostream& s = nout);

void
print_generators(const Grid_Generator_System& gs,
                 const std::string& intro = "",
                 std::ostream& s = nout);

void
print_generators(const Polyhedron& ph,
                 const std::string& intro = "",
                 std::ostream& s = nout);

void
print_generators(const Grid& gr,
                 const std::string& intro = "",
                 std::ostream& s = nout);

template <typename D1, typename D2, typename R>
void
print_constraints(const Partially_Reduced_Product<D1, D2, R>& pd,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  print_constraints(pd.constraints(), intro, s);
}

template <typename D1, typename D2, typename R>
void
print_congruences(const Partially_Reduced_Product<D1, D2, R>& pd,
                  const std::string& intro = "",
                  std::ostream& s = nout) {
  print_congruences(pd.congruences(), intro, s);
}

void
print_function(const Parma_Polyhedra_Library::Partial_Function& function,
               const std::string& intro = "",
               std::ostream& s = nout);

} // namespace Test

} // namespace Parma_Polyhedra_Library

// These using directive and declaration are just to avoid the
// corresponding namespace qualifications in all the tests.
using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::Test;
using std::endl;

#endif // !defined(PPL_ppl_test_hh)
