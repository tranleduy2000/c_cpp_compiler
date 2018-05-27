/* Documentation for used namespaces.
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

#ifndef PPL_namespaces_hh
#define PPL_namespaces_hh 1

//! The entire library is confined to this namespace.
namespace Parma_Polyhedra_Library {

//! All input/output operators are confined to this namespace.
/*! \ingroup PPL_CXX_interface
  This is done so that the library's input/output operators
  do not interfere with those the user might want to define.
  In fact, it is highly unlikely that any predefined I/O
  operator will suit the needs of a client application.
  On the other hand, those applications for which the PPL
  I/O operator are enough can easily obtain access to them.
  For example, a directive like
  \code
    using namespace Parma_Polyhedra_Library::IO_Operators;
  \endcode
  would suffice for most uses.
  In more complex situations, such as
  \code
    const Constraint_System& cs = ...;
    copy(cs.begin(), cs.end(),
	 ostream_iterator<Constraint>(cout, "\n"));
  \endcode
  the Parma_Polyhedra_Library namespace must be suitably extended.
  This can be done as follows:
  \code
    namespace Parma_Polyhedra_Library {
      // Import all the output operators into the main PPL namespace.
      using IO_Operators::operator<<;
    }
  \endcode
*/
namespace IO_Operators {
} // namespace IO_Operators

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Types and functions implementing checked numbers.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace Checked {
} // namespace Checked

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! %Implementation related data and functions.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace Implementation {
} // namespace Implementation

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to language interfaces.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace Interfaces {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the C language interface.
/*! \ingroup PPL_C_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace C {

} // namespace C

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the Java language interface.
/*! \ingroup PPL_Java_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace Java {

} // namespace Java

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the OCaml language interface.
/*! \ingroup PPL_OCaml_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace OCaml {

} // namespace OCaml

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the Prolog language interfaces.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace Prolog {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the Ciao Prolog language interface.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace Ciao {

} // namespace Ciao

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the GNU Prolog language interface.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace GNU {

} // namespace GNU

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the SICStus language interface.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace SICStus {

} // namespace SICStus

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the SWI-Prolog language interface.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace SWI {

} // namespace SWI

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the XSB language interface.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace XSB {

} // namespace XSB

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Data and functions related to the YAP language interface.
/*! \ingroup PPL_Prolog_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
namespace YAP {

} // namespace YAP

} // namespace Prolog

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library


//! The standard C++ namespace.
/*! \ingroup PPL_CXX_interface
  The Parma Polyhedra Library conforms to the C++ standard and,
  in particular, as far as reserved names are concerned (17.4.3.1,
  [lib.reserved.names]).  The PPL, however, defines several
  template specializations for the standard library class template
  <CODE>numeric_limits</CODE> (18.2.1, [lib.limits]).

  \note
  The PPL provides the specializations of the class template
  <CODE>numeric_limits</CODE> not only for PPL-specific numeric types,
  but also for the GMP types <CODE>mpz_class</CODE> and
  <CODE>mpq_class</CODE>. These specializations will be removed
  as soon as they will be provided by the C++ interface of GMP.
*/
namespace std {
} // namespace std


#endif // !defined(PPL_namespaces_hh)
