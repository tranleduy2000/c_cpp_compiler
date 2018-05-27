/* Metaprogramming utilities.
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

#ifndef PPL_meta_programming_hh
#define PPL_meta_programming_hh 1

#include <gmpxx.h>

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Declares a per-class constant of type <CODE>bool</CODE>, called \p name
  and with value \p value.

  \ingroup PPL_CXX_interface
  Differently from static constants, \p name needs not (and cannot) be
  defined (for static constants, the need for a further definition is
  mandated by Section 9.4.2/4 of the C++ standard).
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define const_bool_nodef(name, value)		\
  enum const_bool_value_ ## name { PPL_U(name) = (value) }

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Declares a per-class constant of type <CODE>int</CODE>, called \p name
  and with value \p value.

  \ingroup PPL_CXX_interface
  Differently from static constants, \p name needs not (and cannot) be
  defined (for static constants, the need for a further definition is
  mandated by Section 9.4.2/4 of the C++ standard).
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define const_int_nodef(name, value) \
  enum anonymous_enum_ ## name { PPL_U(name) = (value) }

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Declares a per-class constant of type \p type, called \p name
  and with value \p value.  The value of the constant is accessible
  by means of the syntax <CODE>name()</CODE>.

  \ingroup PPL_CXX_interface
  Differently from static constants, \p name needs not (and cannot) be
  defined (for static constants, the need for a further definition is
  mandated by Section 9.4.2/4 of the C++ standard).
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define const_value_nodef(type, name, value)	\
  static type PPL_U(name)() {                   \
    return (value);				\
  }

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Declares a per-class constant of type \p type, called \p name
  and with value \p value.  A constant reference to the constant
  is accessible by means of the syntax <CODE>name()</CODE>.

  \ingroup PPL_CXX_interface
  Differently from static constants, \p name needs not (and cannot) be
  defined (for static constants, the need for a further definition is
  mandated by Section 9.4.2/4 of the C++ standard).
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define const_ref_nodef(type, name, value)				\
  static const type& PPL_U(name)() {                                    \
    static type PPL_U(name) = (value);                                       \
    return (name);							\
  }

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class that is only defined if \p b evaluates to <CODE>true</CODE>.

  \ingroup PPL_CXX_interface
  This is the non-specialized case, so the class is declared but not defined.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <bool b>
struct Compile_Time_Check;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class that is only defined if \p b evaluates to <CODE>true</CODE>.

  \ingroup PPL_CXX_interface
  This is the specialized case with \p b equal to <CODE>true</CODE>,
  so the class is declared and (trivially) defined.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <>
struct Compile_Time_Check<true> {
};

#define PPL_COMPILE_TIME_CHECK_NAME(suffix) compile_time_check_ ## suffix
#define PPL_COMPILE_TIME_CHECK_AUX(e, suffix)				\
  enum anonymous_enum_compile_time_check_ ## suffix {			\
    /* If e evaluates to false, then the sizeof cannot be compiled. */  \
    PPL_COMPILE_TIME_CHECK_NAME(suffix)					\
      = sizeof(Parma_Polyhedra_Library::Compile_Time_Check<(e)>)        \
  }

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Produces a compilation error if the compile-time constant \p e does
  not evaluate to <CODE>true</CODE>
  \ingroup PPL_CXX_interface
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define PPL_COMPILE_TIME_CHECK(e, msg) PPL_COMPILE_TIME_CHECK_AUX(e, __LINE__)

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class holding a constant called <CODE>value</CODE> that evaluates
  to \p b.
  \ingroup PPL_CXX_interface
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <bool b>
struct Bool {
  enum const_bool_value {
    value = b
  };
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class holding a constant called <CODE>value</CODE> that evaluates
  to <CODE>true</CODE>.
  \ingroup PPL_CXX_interface
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct True : public Bool<true> {
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class holding a constant called <CODE>value</CODE> that evaluates
  to <CODE>false</CODE>.
  \ingroup PPL_CXX_interface
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct False : public Bool<false> {
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class holding a constant called <CODE>value</CODE> that evaluates
  to <CODE>true</CODE> if and only if \p T1 is the same type as \p T2.

  \ingroup PPL_CXX_interface
  This is the non-specialized case, in which \p T1 and \p T2 can be different.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T1, typename T2>
struct Is_Same : public False {
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class holding a constant called <CODE>value</CODE> that evaluates
  to <CODE>true</CODE> if and only if \p T1 is the same type as \p T2.

  \ingroup PPL_CXX_interface
  This is the specialization in which \p T1 and \p T2 are equal.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
struct Is_Same<T, T> : public True {
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class holding a constant called <CODE>value</CODE> that evaluates
  to <CODE>true</CODE> if and only if \p Base is the same type as \p Derived
  or \p Derived is a class derived from \p Base.

  \ingroup PPL_CXX_interface
  \note
  Care must be taken to use this predicate with template classes.
  Suppose we have
  \code
  template <typename T> struct B;
  template <typename T> struct D : public B<T>;
  \endcode
  Of course, we cannot test if, for some type variable <CODE>U</CODE>, we have
  <CODE>Is_Same_Or_Derived<B<U>, Type>:: const_bool_value:: value == true</CODE>.
  But we can do as follows:
  \code
  struct B_Base {
  };

  template <typename T> struct B : public B_Base;
  \endcode
  This enables us to inquire
  <CODE>Is_Same_Or_Derived<B_Base, Type>:: const_bool_value:: value</CODE>.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Base, typename Derived>
struct Is_Same_Or_Derived {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! A class that is constructible from just anything.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  struct Any {
    //! The universal constructor.
    template <typename T>
    Any(const T&);
  };

  //! Overloading with \p Base.
  static char func(const Base&);

  //! Overloading with \p Any.
  static double func(Any);

  //! A function obtaining a const reference to a \p Derived object.
  static const Derived& derived_object();

  PPL_COMPILE_TIME_CHECK(sizeof(char) != sizeof(double),
                         "architecture with sizeof(char) == sizeof(double)"
                         " (!?)");

  enum const_bool_value {
    /*!
      Assuming <CODE>sizeof(char) != sizeof(double)</CODE>, the C++
      overload resolution mechanism guarantees that <CODE>value</CODE>
      evaluates to <CODE>true</CODE> if and only if <CODE>Base</CODE>
      is the same type as <CODE>Derived</CODE> or <CODE>Derived</CODE>
      is a class derived from <CODE>Base</CODE>.
    */
    value = (sizeof(func(derived_object())) == sizeof(char))
  };
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class that provides a type member called <CODE>type</CODE> equivalent
  to \p T if and only if \p b is <CODE>true</CODE>.

  \ingroup PPL_CXX_interface
  This is the non-specialized case, in which the <CODE>type</CODE> member
  is not present.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <bool b, typename T = void>
struct Enable_If {
};

template <typename Type, Type, typename T = void>
struct Enable_If_Is {
  typedef T type;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  A class that provides a type member called <CODE>type</CODE> equivalent
  to \p T if and only if \p b is <CODE>true</CODE>.

  \ingroup PPL_CXX_interface
  This is the specialization in which the <CODE>type</CODE> member
  is present.

  \note
  Let <CODE>T</CODE>, <CODE>T1</CODE> and <CODE>T2</CODE> be any type
  expressions and suppose we have some template function
  <CODE>T f(T1, T2)</CODE>.  If we want to declare a specialization
  that is enabled only if some compile-time checkable condition holds,
  we simply declare the specialization by
  \code
  template ...
  typename Enable_If<condition, T>::type
  foo(T1 x, T2 y);
  \endcode
  For all the instantiations of the template parameters that cause
  <CODE>condition</CODE> to evaluate to <CODE>false</CODE>,
  the <CODE>Enable_If<condition, T>::type</CODE> member will not be defined.
  Hence, for that instantiations, the specialization will not be eligible.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
struct Enable_If<true, T> {
  typedef T type;
};

template <typename T>
struct Is_Native : public False {
};

template <> struct Is_Native<char> : public True { };
template <> struct Is_Native<signed char> : public True { };
template <> struct Is_Native<signed short> : public True { };
template <> struct Is_Native<signed int> : public True { };
template <> struct Is_Native<signed long> : public True { };
template <> struct Is_Native<signed long long> : public True { };
template <> struct Is_Native<unsigned char> : public True { };
template <> struct Is_Native<unsigned short> : public True { };
template <> struct Is_Native<unsigned int> : public True { };
template <> struct Is_Native<unsigned long> : public True { };
template <> struct Is_Native<unsigned long long> : public True { };

#if PPL_SUPPORTED_FLOAT
template <> struct Is_Native<float> : public True { };
#endif
#if PPL_SUPPORTED_DOUBLE
template <> struct Is_Native<double> : public True { };
#endif
#if PPL_SUPPORTED_LONG_DOUBLE
template <> struct Is_Native<long double> : public True { };
#endif

template <> struct Is_Native<mpz_class> : public True { };

template <> struct Is_Native<mpq_class> : public True { };

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_meta_programming_hh)
