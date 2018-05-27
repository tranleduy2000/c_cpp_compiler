/* Nifty counter object for the initialization of the library.
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

#ifndef PPL_initializer_hh
#define PPL_initializer_hh 1

#include "Init.defs.hh"

#ifndef PPL_NO_AUTOMATIC_INITIALIZATION

static Parma_Polyhedra_Library::Init Parma_Polyhedra_Library_initializer;

#else

static Parma_Polyhedra_Library::Init* Parma_Polyhedra_Library_initializer_p;

#endif

namespace Parma_Polyhedra_Library {

//! Initializes the library.
inline void
initialize() {
#ifdef PPL_NO_AUTOMATIC_INITIALIZATION
  if (Parma_Polyhedra_Library_initializer_p == 0)
    Parma_Polyhedra_Library_initializer_p = new Init();
#endif
}

//! Finalizes the library.
inline void
finalize() {
#ifdef PPL_NO_AUTOMATIC_INITIALIZATION
  PPL_ASSERT(Parma_Polyhedra_Library_initializer_p != 0);
  delete Parma_Polyhedra_Library_initializer_p;
  Parma_Polyhedra_Library_initializer_p = 0;
#endif
}

} //namespace Parma_Polyhedra_Library

#endif // !defined(PPL_initializer_hh)
