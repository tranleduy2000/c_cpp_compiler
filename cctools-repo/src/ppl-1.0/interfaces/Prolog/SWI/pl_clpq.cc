/* C++ main program for the toy PPL/SWI-Prolog CLP(Q) interpreter.
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

#include <SWI-Prolog.h>

extern "C" install_t install();
extern "C" install_t uninstall();

int
main(int, char** argv) {
#if PLVERSION > 50643
  // Tell SWI-Prolog not to fiddle with GMP allocation functions.
  PL_action(PL_GMP_SET_ALLOC_FUNCTIONS, FALSE);
#endif

  char* pl_args[2];
  pl_args[0] = argv[0];
  pl_args[1] = 0;
  if (!PL_initialise(1, pl_args))
    PL_halt(1);

  install();
  predicate_t pred = PL_predicate("main", 0, "user");
  term_t h0 = PL_new_term_refs(0);
  int ret_val = PL_call_predicate(0, PL_Q_NORMAL, pred, h0);
  uninstall();
  PL_halt(ret_val ? 0 : 1);
}
