/* Checked_Number class implementation: inline functions.
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

#ifndef PPL_Checked_Number_inlines_hh
#define PPL_Checked_Number_inlines_hh 1

#include "globals.defs.hh"
#include <stdexcept>
#include <sstream>

namespace Parma_Polyhedra_Library {

#ifndef NDEBUG
#define DEBUG_ROUND_NOT_NEEDED
#endif

inline Rounding_Dir
rounding_dir(Rounding_Dir dir) {
  if (dir == ROUND_NOT_NEEDED) {
#ifdef DEBUG_ROUND_NOT_NEEDED
    return ROUND_CHECK;
#endif
  }
  return dir;
}

inline Result
check_result(Result r, Rounding_Dir dir) {
  if (dir == ROUND_NOT_NEEDED) {
#ifdef DEBUG_ROUND_NOT_NEEDED
    PPL_ASSERT(result_relation(r) == VR_EQ);
#endif
    return r;
  }
  return r;
}


template <typename T>
inline void
Checked_Number_Transparent_Policy<T>::handle_result(Result) {
}

inline void
Extended_Number_Policy::handle_result(Result r) {
  if (result_class(r) == VC_NAN)
    throw_result_exception(r);
}

template <typename T, typename Policy>
inline
Checked_Number<T, Policy>::Checked_Number()
 : v(0) {
}

template <typename T, typename Policy>
inline
Checked_Number<T, Policy>::Checked_Number(const Checked_Number& y) {
  // TODO: avoid default construction of value member.
  Checked::copy<Policy, Policy>(v, y.raw_value());
}

template <typename T, typename Policy>
template <typename From, typename From_Policy>
inline
Checked_Number<T, Policy>
::Checked_Number(const Checked_Number<From, From_Policy>& y,
		 Rounding_Dir dir) {
  // TODO: avoid default construction of value member.
  Policy::handle_result(check_result(Checked::assign_ext<Policy, From_Policy>
				     (v,
				      y.raw_value(),
				      rounding_dir(dir)),
				     dir)
			);
}

template <typename T, typename Policy>
template <typename From, typename From_Policy>
inline
Checked_Number<T, Policy>
::Checked_Number(const Checked_Number<From, From_Policy>& y) {
  // TODO: avoid default construction of value member.
  Rounding_Dir dir = Policy::ROUND_DEFAULT_CONSTRUCTOR;
  Policy::handle_result(check_result(Checked::assign_ext<Policy, From_Policy>
				     (v,
				      y.raw_value(),
				      rounding_dir(dir)),
				     dir));
}

// TODO: avoid default construction of value member.
#define PPL_DEFINE_CTOR(type) \
template <typename T, typename Policy> \
inline \
Checked_Number<T, Policy>::Checked_Number(const type y, Rounding_Dir dir) { \
  Policy::handle_result							\
    (check_result(Checked::assign_ext<Policy,                           \
                                      Checked_Number_Transparent_Policy<PPL_U(type)> > \
		  (v, y, rounding_dir(dir)),				\
		  dir));						\
}									\
template <typename T, typename Policy>					\
inline									\
Checked_Number<T, Policy>::Checked_Number(const type y) {		\
  Rounding_Dir dir = Policy::ROUND_DEFAULT_CONSTRUCTOR;			\
  Policy::handle_result							\
    (check_result(Checked::assign_ext<Policy,                           \
                                      Checked_Number_Transparent_Policy<PPL_U(type)> > \
		  (v, y, rounding_dir(dir)),				\
		  dir));						\
}

PPL_DEFINE_CTOR(char)
PPL_DEFINE_CTOR(signed char)
PPL_DEFINE_CTOR(signed short)
PPL_DEFINE_CTOR(signed int)
PPL_DEFINE_CTOR(signed long)
PPL_DEFINE_CTOR(signed long long)
PPL_DEFINE_CTOR(unsigned char)
PPL_DEFINE_CTOR(unsigned short)
PPL_DEFINE_CTOR(unsigned int)
PPL_DEFINE_CTOR(unsigned long)
PPL_DEFINE_CTOR(unsigned long long)
#if PPL_SUPPORTED_FLOAT
PPL_DEFINE_CTOR(float)
#endif
#if PPL_SUPPORTED_DOUBLE
PPL_DEFINE_CTOR(double)
#endif
#if PPL_SUPPORTED_LONG_DOUBLE
PPL_DEFINE_CTOR(long double)
#endif
PPL_DEFINE_CTOR(mpq_class&)
PPL_DEFINE_CTOR(mpz_class&)

#undef PPL_DEFINE_CTOR


template <typename T, typename Policy>
inline
Checked_Number<T, Policy>::Checked_Number(const char* y, Rounding_Dir dir) {
  std::istringstream s(y);
  Policy::handle_result(check_result(Checked::input<Policy>(v,
							    s,
							    rounding_dir(dir)),
				     dir));
}

template <typename T, typename Policy>
inline
Checked_Number<T, Policy>::Checked_Number(const char* y) {
  std::istringstream s(y);
  Rounding_Dir dir = Policy::ROUND_DEFAULT_CONSTRUCTOR;
  Policy::handle_result(check_result(Checked::input<Policy>(v,
							    s,
							    rounding_dir(dir)),
				     dir));
}

template <typename T, typename Policy>
template <typename From>
inline
Checked_Number<T, Policy>
::Checked_Number(const From&,
                 Rounding_Dir dir,
                 typename Enable_If<Is_Special<From>::value, bool>::type) {
  Policy::handle_result(check_result(Checked::assign_special<Policy>(v,
                                                                     From::vclass,
                                                                     rounding_dir(dir)),
                                     dir));
}

template <typename T, typename Policy>
template <typename From>
inline
Checked_Number<T, Policy>::Checked_Number(const From&, typename Enable_If<Is_Special<From>::value, bool>::type) {
  Rounding_Dir dir = Policy::ROUND_DEFAULT_CONSTRUCTOR;
  Policy::handle_result(check_result(Checked::assign_special<Policy>(v,
							    From::vclass,
							    rounding_dir(dir)),
				     dir));
}

template <typename To, typename From>
inline typename Enable_If<Is_Native_Or_Checked<To>::value
                          && Is_Special<From>::value, Result>::type
assign_r(To& to, const From&, Rounding_Dir dir) {
  return check_result(Checked::assign_special<typename Native_Checked_To_Wrapper<To>
		      ::Policy>(Native_Checked_To_Wrapper<To>::raw_value(to),
				From::vclass,
				rounding_dir(dir)),
		      dir);
}

template <typename To, typename From>
inline typename Enable_If<Is_Native_Or_Checked<To>::value && Is_Special<From>::value, Result>::type
construct(To& to, const From&, Rounding_Dir dir) {
  return check_result(Checked::construct_special<typename Native_Checked_To_Wrapper<To>
		      ::Policy>(Native_Checked_To_Wrapper<To>::raw_value(to),
				From::vclass,
				rounding_dir(dir)),
		      dir);
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_minus_infinity(const T& x) {
  return Checked::is_minf<typename Native_Checked_From_Wrapper<T>
    ::Policy>(Native_Checked_From_Wrapper<T>::raw_value(x));
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_plus_infinity(const T& x) {
  return Checked::is_pinf<typename Native_Checked_From_Wrapper<T>
    ::Policy>(Native_Checked_From_Wrapper<T>::raw_value(x));
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, int>::type
infinity_sign(const T& x) {
  return is_minus_infinity(x) ? -1 : (is_plus_infinity(x) ? 1 : 0);
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_not_a_number(const T& x) {
  return Checked::is_nan<typename Native_Checked_From_Wrapper<T>
    ::Policy>(Native_Checked_From_Wrapper<T>::raw_value(x));
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_integer(const T& x) {
  return Checked::is_int<typename Native_Checked_From_Wrapper<T>
    ::Policy>(Native_Checked_From_Wrapper<T>::raw_value(x));
}

template <typename T, typename Policy>
inline
Checked_Number<T, Policy>::operator T() const {
  if (Policy::convertible)
    return v;
}

template <typename T, typename Policy>
inline T&
Checked_Number<T, Policy>::raw_value() {
  return v;
}

template <typename T, typename Policy>
inline const T&
Checked_Number<T, Policy>::raw_value() const {
  return v;
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline const T&
raw_value(const Checked_Number<T, Policy>& x) {
  return x.raw_value();
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline T&
raw_value(Checked_Number<T, Policy>& x) {
  return x.raw_value();
}

template <typename T, typename Policy>
inline bool
Checked_Number<T, Policy>::OK() const {
  return true;
}

template <typename T, typename Policy>
inline Result
Checked_Number<T, Policy>::classify(bool nan, bool inf, bool sign) const {
  return Checked::classify<Policy>(v, nan, inf, sign);
}

template <typename T, typename Policy>
inline bool
is_not_a_number(const Checked_Number<T, Policy>& x) {
  return Checked::is_nan<Policy>(x.raw_value());
}

template <typename T, typename Policy>
inline bool
is_minus_infinity(const Checked_Number<T, Policy>& x) {
  return Checked::is_minf<Policy>(x.raw_value());
}

template <typename T, typename Policy>
inline bool
is_plus_infinity(const Checked_Number<T, Policy>& x) {
  return Checked::is_pinf<Policy>(x.raw_value());
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline memory_size_type
total_memory_in_bytes(const Checked_Number<T, Policy>& x) {
  return total_memory_in_bytes(x.raw_value());
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline memory_size_type
external_memory_in_bytes(const Checked_Number<T, Policy>& x) {
  return external_memory_in_bytes(x.raw_value());
}


/*! \relates Checked_Number */
template <typename To>
inline typename Enable_If<Is_Native_Or_Checked<To>::value, Result>::type
assign_r(To& to, const char* x, Rounding_Dir dir) {
  std::istringstream s(x);
  return check_result(Checked::input<typename Native_Checked_To_Wrapper<To>
		      ::Policy>(Native_Checked_To_Wrapper<To>::raw_value(to),
				s,
				rounding_dir(dir)),
		      dir);
}

#define PPL_DEFINE_FUNC1_A(name, func) \
template <typename To, typename From>					\
inline typename Enable_If<Is_Native_Or_Checked<To>::value               \
                          && Is_Native_Or_Checked<From>::value,         \
                          Result>::type                                 \
 PPL_U(name)(To& to, const From& x, Rounding_Dir dir) {                 \
  return								\
    check_result(Checked::func<typename Native_Checked_To_Wrapper<To>	\
		 ::Policy,						\
		 typename Native_Checked_From_Wrapper<From>		\
		 ::Policy>(Native_Checked_To_Wrapper<To>::raw_value(to), \
			   Native_Checked_From_Wrapper<From>::raw_value(x), \
			   rounding_dir(dir)), dir);			\
}

PPL_DEFINE_FUNC1_A(construct, construct_ext)
PPL_DEFINE_FUNC1_A(assign_r, assign_ext)
PPL_DEFINE_FUNC1_A(floor_assign_r, floor_ext)
PPL_DEFINE_FUNC1_A(ceil_assign_r, ceil_ext)
PPL_DEFINE_FUNC1_A(trunc_assign_r, trunc_ext)
PPL_DEFINE_FUNC1_A(neg_assign_r, neg_ext)
PPL_DEFINE_FUNC1_A(abs_assign_r, abs_ext)
PPL_DEFINE_FUNC1_A(sqrt_assign_r, sqrt_ext)

#undef PPL_DEFINE_FUNC1_A

#define PPL_DEFINE_FUNC1_B(name, func) \
template <typename To, typename From>					\
inline typename Enable_If<Is_Native_Or_Checked<To>::value		\
                          && Is_Native_Or_Checked<From>::value,         \
                          Result>::type					\
 PPL_U(name)(To& to, const From& x, unsigned int exp, Rounding_Dir dir) { \
  return								\
    check_result(Checked::func<typename Native_Checked_To_Wrapper<To>	\
		 ::Policy,						\
		 typename Native_Checked_From_Wrapper<From>		\
		 ::Policy>(Native_Checked_To_Wrapper<To>::raw_value(to), \
			   Native_Checked_From_Wrapper<From>::raw_value(x), \
			   exp,						\
			   rounding_dir(dir)),				\
		 dir);							\
}

PPL_DEFINE_FUNC1_B(add_2exp_assign_r, add_2exp_ext)
PPL_DEFINE_FUNC1_B(sub_2exp_assign_r, sub_2exp_ext)
PPL_DEFINE_FUNC1_B(mul_2exp_assign_r, mul_2exp_ext)
PPL_DEFINE_FUNC1_B(div_2exp_assign_r, div_2exp_ext)
PPL_DEFINE_FUNC1_B(smod_2exp_assign_r, smod_2exp_ext)
PPL_DEFINE_FUNC1_B(umod_2exp_assign_r, umod_2exp_ext)

#undef PPL_DEFINE_FUNC1_B

#define PPL_DEFINE_FUNC2(name, func) \
template <typename To, typename From1, typename From2>			\
inline typename Enable_If<Is_Native_Or_Checked<To>::value		\
                          && Is_Native_Or_Checked<From1>::value         \
                          && Is_Native_Or_Checked<From2>::value,        \
                          Result>::type					\
 PPL_U(name)(To& to, const From1& x, const From2& y, Rounding_Dir dir) { \
  return								\
    check_result(Checked::func<typename Native_Checked_To_Wrapper<To>	\
		 ::Policy,						\
		 typename Native_Checked_From_Wrapper<From1>		\
		 ::Policy,						\
		 typename Native_Checked_From_Wrapper<From2>		\
		 ::Policy>(Native_Checked_To_Wrapper<To>::raw_value(to), \
			   Native_Checked_From_Wrapper<From1>::raw_value(x), \
			   Native_Checked_From_Wrapper<From2>::raw_value(y), \
			   rounding_dir(dir)),				\
		 dir);							\
}

PPL_DEFINE_FUNC2(add_assign_r, add_ext)
PPL_DEFINE_FUNC2(sub_assign_r, sub_ext)
PPL_DEFINE_FUNC2(mul_assign_r, mul_ext)
PPL_DEFINE_FUNC2(div_assign_r, div_ext)
PPL_DEFINE_FUNC2(idiv_assign_r, idiv_ext)
PPL_DEFINE_FUNC2(rem_assign_r, rem_ext)
PPL_DEFINE_FUNC2(gcd_assign_r, gcd_ext)
PPL_DEFINE_FUNC2(lcm_assign_r, lcm_ext)
PPL_DEFINE_FUNC2(add_mul_assign_r, add_mul_ext)
PPL_DEFINE_FUNC2(sub_mul_assign_r, sub_mul_ext)

#undef PPL_DEFINE_FUNC2

#define PPL_DEFINE_FUNC4(name, func)                                    \
template <typename To1,							\
          typename To2,							\
	  typename To3,							\
          typename From1,						\
          typename From2>						\
inline typename Enable_If<Is_Native_Or_Checked<To1>::value		\
                          && Is_Native_Or_Checked<To2>::value           \
                          && Is_Native_Or_Checked<To3>::value           \
                          && Is_Native_Or_Checked<From1>::value         \
                          && Is_Native_Or_Checked<From2>::value,        \
                          Result>::type					\
 PPL_U(name)(To1& to, To2& s, To3& t, const From1& x, const From2& y,   \
     Rounding_Dir dir) {						\
  return								\
    check_result							\
    (Checked::func<typename Native_Checked_To_Wrapper<To1>::Policy,	\
                   typename Native_Checked_To_Wrapper<To2>::Policy,	\
                   typename Native_Checked_To_Wrapper<To3>::Policy,	\
                   typename Native_Checked_From_Wrapper<From1>::Policy,	\
                   typename Native_Checked_From_Wrapper<From2>::Policy>	\
     (Native_Checked_To_Wrapper<To1>::raw_value(to),			\
      Native_Checked_To_Wrapper<To2>::raw_value(s),			\
      Native_Checked_To_Wrapper<To3>::raw_value(t),			\
      Native_Checked_From_Wrapper<From1>::raw_value(x),			\
      Native_Checked_From_Wrapper<From2>::raw_value(y),			\
      rounding_dir(dir)),						\
     dir);								\
}

PPL_DEFINE_FUNC4(gcdext_assign_r, gcdext_ext)

#undef PPL_DEFINE_PPL_DEFINE_FUNC4

#define PPL_DEFINE_INCREMENT(f, fun) \
template <typename T, typename Policy> \
inline Checked_Number<T, Policy>& \
Checked_Number<T, Policy>::f() { \
  Policy::handle_result((fun)(*this, *this, T(1),             \
			    Policy::ROUND_DEFAULT_OPERATOR)); \
  return *this; \
} \
template <typename T, typename Policy> \
inline Checked_Number<T, Policy> \
Checked_Number<T, Policy>::f(int) {\
  T r = v;\
  Policy::handle_result((fun)(*this, *this, T(1),             \
			    Policy::ROUND_DEFAULT_OPERATOR)); \
  return r;\
}

PPL_DEFINE_INCREMENT(operator ++, add_assign_r)
PPL_DEFINE_INCREMENT(operator --, sub_assign_r)

#undef PPL_DEFINE_INCREMENT

template <typename T, typename Policy>
inline Checked_Number<T, Policy>&
Checked_Number<T, Policy>::operator=(const Checked_Number<T, Policy>& y) {
  Checked::copy<Policy, Policy>(v, y.raw_value());
  return *this;
}
template <typename T, typename Policy>
template <typename From>
inline Checked_Number<T, Policy>&
Checked_Number<T, Policy>::operator=(const From& y) {
  Policy::handle_result(assign_r(*this, y, Policy::ROUND_DEFAULT_OPERATOR));
  return *this;
}

#define PPL_DEFINE_BINARY_OP_ASSIGN(f, fun) \
template <typename T, typename Policy> \
template <typename From_Policy> \
inline Checked_Number<T, Policy>& \
Checked_Number<T, Policy>::f(const Checked_Number<T, From_Policy>& y) { \
  Policy::handle_result((fun)(*this, *this, y,                          \
			    Policy::ROUND_DEFAULT_OPERATOR)); \
  return *this; \
} \
template <typename T, typename Policy> \
inline Checked_Number<T, Policy>& \
Checked_Number<T, Policy>::f(const T& y) { \
  Policy::handle_result((fun)(*this, *this, y,                \
			    Policy::ROUND_DEFAULT_OPERATOR)); \
  return *this; \
} \
template <typename T, typename Policy> \
template <typename From> \
inline typename Enable_If<Is_Native_Or_Checked<From>::value, \
                          Checked_Number<T, Policy>& >::type \
Checked_Number<T, Policy>::f(const From& y) { \
  Checked_Number<T, Policy> cy(y); \
  Policy::handle_result((fun)(*this, *this, cy,               \
			    Policy::ROUND_DEFAULT_OPERATOR)); \
  return *this; \
}

PPL_DEFINE_BINARY_OP_ASSIGN(operator +=, add_assign_r)
PPL_DEFINE_BINARY_OP_ASSIGN(operator -=, sub_assign_r)
PPL_DEFINE_BINARY_OP_ASSIGN(operator *=, mul_assign_r)
PPL_DEFINE_BINARY_OP_ASSIGN(operator /=, div_assign_r)
PPL_DEFINE_BINARY_OP_ASSIGN(operator %=, rem_assign_r)

#undef PPL_DEFINE_BINARY_OP_ASSIGN

#define PPL_DEFINE_BINARY_OP(f, fun) \
template <typename T, typename Policy> \
inline Checked_Number<T, Policy> \
 PPL_U(f)(const Checked_Number<T, Policy>& x,   \
         const Checked_Number<T, Policy>& y) {  \
  Checked_Number<T, Policy> r; \
  Policy::handle_result((fun)(r, x, y, Policy::ROUND_DEFAULT_OPERATOR)); \
  return r; \
} \
template <typename Type, typename T, typename Policy>	\
inline \
typename Enable_If<Is_Native<Type>::value, Checked_Number<T, Policy> >::type \
 PPL_U(f)(const Type& x, const Checked_Number<T, Policy>& y) {          \
  Checked_Number<T, Policy> r(x); \
  Policy::handle_result((fun)(r, r, y, Policy::ROUND_DEFAULT_OPERATOR)); \
  return r; \
} \
template <typename T, typename Policy, typename Type>	\
inline \
typename Enable_If<Is_Native<Type>::value, Checked_Number<T, Policy> >::type \
 PPL_U(f)(const Checked_Number<T, Policy>& x, const Type& y) {          \
  Checked_Number<T, Policy> r(y); \
  Policy::handle_result((fun)(r, x, r, Policy::ROUND_DEFAULT_OPERATOR)); \
  return r; \
}

PPL_DEFINE_BINARY_OP(operator +, add_assign_r)
PPL_DEFINE_BINARY_OP(operator -, sub_assign_r)
PPL_DEFINE_BINARY_OP(operator *, mul_assign_r)
PPL_DEFINE_BINARY_OP(operator /, div_assign_r)
PPL_DEFINE_BINARY_OP(operator %, rem_assign_r)

#undef PPL_DEFINE_BINARY_OP

#define PPL_DEFINE_COMPARE_OP(f, fun)                                   \
template <typename T1, typename T2>					\
inline									\
typename Enable_If<Is_Native_Or_Checked<T1>::value                      \
                   && Is_Native_Or_Checked<T2>::value                   \
                   && (Is_Checked<T1>::value || Is_Checked<T2>::value),	\
		   bool>::type						\
 PPL_U(f)(const T1& x, const T2& y) {                                   \
  return Checked::fun<typename Native_Checked_From_Wrapper<T1>::Policy,	\
    		      typename Native_Checked_From_Wrapper<T2>::Policy>	\
    (Native_Checked_From_Wrapper<T1>::raw_value(x),			\
     Native_Checked_From_Wrapper<T2>::raw_value(y));			\
}

PPL_DEFINE_COMPARE_OP(operator ==, eq_ext)
PPL_DEFINE_COMPARE_OP(operator !=, ne_ext)
PPL_DEFINE_COMPARE_OP(operator >=, ge_ext)
PPL_DEFINE_COMPARE_OP(operator >, gt_ext)
PPL_DEFINE_COMPARE_OP(operator <=, le_ext)
PPL_DEFINE_COMPARE_OP(operator <, lt_ext)

#undef PPL_DEFINE_COMPARE_OP

#define PPL_DEFINE_COMPARE(f, fun)                                      \
template <typename T1, typename T2>					\
inline typename Enable_If<Is_Native_Or_Checked<T1>::value		\
			  && Is_Native_Or_Checked<T2>::value,		\
                          bool>::type					\
 PPL_U(f)(const T1& x, const T2& y) {                                   \
  return Checked::fun<typename Native_Checked_From_Wrapper<T1>::Policy,	\
    		      typename Native_Checked_From_Wrapper<T2>::Policy>	\
    (Native_Checked_From_Wrapper<T1>::raw_value(x),			\
     Native_Checked_From_Wrapper<T2>::raw_value(y));			\
}

PPL_DEFINE_COMPARE(equal, eq_ext)
PPL_DEFINE_COMPARE(not_equal, ne_ext)
PPL_DEFINE_COMPARE(greater_or_equal, ge_ext)
PPL_DEFINE_COMPARE(greater_than, gt_ext)
PPL_DEFINE_COMPARE(less_or_equal, le_ext)
PPL_DEFINE_COMPARE(less_than, lt_ext)

#undef PPL_DEFINE_COMPARE

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline Checked_Number<T, Policy>
operator+(const Checked_Number<T, Policy>& x) {
  return x;
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline Checked_Number<T, Policy>
operator-(const Checked_Number<T, Policy>& x) {
  Checked_Number<T, Policy> r;
  Policy::handle_result(neg_assign_r(r, x, Policy::ROUND_DEFAULT_OPERATOR));
  return r;
}

#define PPL_DEFINE_ASSIGN_FUN2_1(f, fun) \
template <typename T, typename Policy> \
inline void \
 PPL_U(f)(Checked_Number<T, Policy>& x) {                               \
  Policy::handle_result((fun)(x, x, Policy::ROUND_DEFAULT_FUNCTION));	\
}

#define PPL_DEFINE_ASSIGN_FUN2_2(f, fun) \
template <typename T, typename Policy> \
inline void \
 PPL_U(f)(Checked_Number<T, Policy>& x, const Checked_Number<T, Policy>& y) { \
  Policy::handle_result((fun)(x, y, Policy::ROUND_DEFAULT_FUNCTION)); \
}

#define PPL_DEFINE_ASSIGN_FUN3_3(f, fun) \
template <typename T, typename Policy> \
inline void \
 PPL_U(f)(Checked_Number<T, Policy>& x, const Checked_Number<T, Policy>& y, \
  const Checked_Number<T, Policy>& z) { \
  Policy::handle_result((fun)(x, y, z, Policy::ROUND_DEFAULT_FUNCTION)); \
}

#define PPL_DEFINE_ASSIGN_FUN5_5(f, fun)					\
template <typename T, typename Policy>					\
inline void								\
 PPL_U(f)(Checked_Number<T, Policy>& x,                                 \
  Checked_Number<T, Policy>& s, Checked_Number<T, Policy>& t,		\
  const Checked_Number<T, Policy>& y,					\
  const Checked_Number<T, Policy>& z) {					\
  Policy::handle_result((fun)(x, s, t, y, z, Policy::ROUND_DEFAULT_FUNCTION)); \
}

PPL_DEFINE_ASSIGN_FUN2_2(sqrt_assign, sqrt_assign_r)

PPL_DEFINE_ASSIGN_FUN2_1(floor_assign, floor_assign_r)
PPL_DEFINE_ASSIGN_FUN2_2(floor_assign, floor_assign_r)

PPL_DEFINE_ASSIGN_FUN2_1(ceil_assign, ceil_assign_r)
PPL_DEFINE_ASSIGN_FUN2_2(ceil_assign, ceil_assign_r)

PPL_DEFINE_ASSIGN_FUN2_1(trunc_assign, trunc_assign_r)
PPL_DEFINE_ASSIGN_FUN2_2(trunc_assign, trunc_assign_r)

PPL_DEFINE_ASSIGN_FUN2_1(neg_assign, neg_assign_r)
PPL_DEFINE_ASSIGN_FUN2_2(neg_assign, neg_assign_r)

PPL_DEFINE_ASSIGN_FUN2_1(abs_assign, abs_assign_r)
PPL_DEFINE_ASSIGN_FUN2_2(abs_assign, abs_assign_r)

PPL_DEFINE_ASSIGN_FUN3_3(add_mul_assign, add_mul_assign_r)

PPL_DEFINE_ASSIGN_FUN3_3(sub_mul_assign, sub_mul_assign_r)

PPL_DEFINE_ASSIGN_FUN3_3(rem_assign, rem_assign_r)

PPL_DEFINE_ASSIGN_FUN3_3(gcd_assign, gcd_assign_r)

PPL_DEFINE_ASSIGN_FUN5_5(gcdext_assign, gcdext_assign_r)

PPL_DEFINE_ASSIGN_FUN3_3(lcm_assign, lcm_assign_r)

#undef PPL_DEFINE_ASSIGN_FUN2_1
#undef PPL_DEFINE_ASSIGN_FUN2_2
#undef PPL_DEFINE_ASSIGN_FUN3_2
#undef PPL_DEFINE_ASSIGN_FUN3_3
#undef PPL_DEFINE_ASSIGN_FUN5_5

#define PPL_DEFINE_ASSIGN_2EXP(f, fun)					\
template <typename T, typename Policy>                                  \
inline void								\
 PPL_U(f)(Checked_Number<T, Policy>& x,                                 \
          const Checked_Number<T, Policy>& y, unsigned int exp) {       \
  Policy::handle_result((fun)(x, y, exp, Policy::ROUND_DEFAULT_FUNCTION)); \
}

PPL_DEFINE_ASSIGN_2EXP(mul_2exp_assign, mul_2exp_assign_r)
PPL_DEFINE_ASSIGN_2EXP(div_2exp_assign, div_2exp_assign_r)

template <typename T, typename Policy>
inline void
exact_div_assign(Checked_Number<T, Policy>& x,
		 const Checked_Number<T, Policy>& y,
		 const Checked_Number<T, Policy>& z) {
  Policy::handle_result(div_assign_r(x, y, z, ROUND_NOT_NEEDED));
}

/*! \relates Checked_Number */
template <typename From>
inline typename Enable_If<Is_Native_Or_Checked<From>::value, int>::type
sgn(const From& x) {
  Result_Relation r = Checked::sgn_ext<typename Native_Checked_From_Wrapper<From>::Policy>(Native_Checked_From_Wrapper<From>::raw_value(x));
  switch (r) {
  case VR_LT:
    return -1;
  case VR_EQ:
    return 0;
  case VR_GT:
    return 1;
  default:
    throw(0);
  }
}

/*! \relates Checked_Number */
template <typename From1, typename From2>
inline typename Enable_If<Is_Native_Or_Checked<From1>::value
                          && Is_Native_Or_Checked<From2>::value,
                          int>::type
cmp(const From1& x, const From2& y) {
  Result_Relation r
    = Checked::cmp_ext<typename Native_Checked_From_Wrapper<From1>::Policy,
                       typename Native_Checked_From_Wrapper<From2>::Policy>
                 (Native_Checked_From_Wrapper<From1>::raw_value(x),
		  Native_Checked_From_Wrapper<From2>::raw_value(y));
  switch (r) {
  case VR_LT:
    return -1;
  case VR_EQ:
    return 0;
  case VR_GT:
    return 1;
  default:
    throw(0);
  }
}

/*! \relates Checked_Number */
template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, Result>::type
output(std::ostream& os, const T& x,
       const Numeric_Format& format, Rounding_Dir dir) {
  return check_result(Checked::output_ext<typename Native_Checked_From_Wrapper<T>::Policy>
		      (os,
		       Native_Checked_From_Wrapper<T>::raw_value(x),
		       format,
		       rounding_dir(dir)),
		      dir);
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline std::ostream&
operator<<(std::ostream& os, const Checked_Number<T, Policy>& x) {
  Policy::handle_result(output(os, x, Numeric_Format(), ROUND_IGNORE));
  return os;
}

/*! \relates Checked_Number */
template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, Result>::type
input(T& x, std::istream& is, Rounding_Dir dir) {
  return check_result(Checked::input_ext<typename Native_Checked_To_Wrapper<T>::Policy>
		      (Native_Checked_To_Wrapper<T>::raw_value(x),
		       is,
		       rounding_dir(dir)),
		      dir);
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline std::istream& operator>>(std::istream& is,
				Checked_Number<T, Policy>& x) {
  Result r = input(x, is, Policy::ROUND_DEFAULT_INPUT);
  if (r == V_CVT_STR_UNK)
    is.setstate(std::ios::failbit);
  else
    Policy::handle_result(r);
  return is;
}

template <typename T>
inline T
plus_infinity() {
  return PLUS_INFINITY;
}

template <typename T>
inline T
minus_infinity() {
  return MINUS_INFINITY;
}

template <typename T>
inline T
not_a_number() {
  return NOT_A_NUMBER;
}

/*! \relates Checked_Number */
template <typename T, typename Policy>
inline void
swap(Checked_Number<T, Policy>& x, Checked_Number<T, Policy>& y) {
  using std::swap;
  swap(x.raw_value(), y.raw_value());
}

template <typename T>
inline void
maybe_reset_fpu_inexact() {
  if (FPU_Related<T>::value)
    return fpu_reset_inexact();
}

template <typename T>
inline int
maybe_check_fpu_inexact() {
  if (FPU_Related<T>::value)
    return fpu_check_inexact();
  else
    return 0;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Checked_Number_inlines_hh)
