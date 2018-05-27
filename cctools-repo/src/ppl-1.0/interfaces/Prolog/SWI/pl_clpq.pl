/* Prolog main program for the toy PPL/SWI-Prolog CLP(Q) interpreter.
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

eat_eol.

main :-
  % FIXME: clearing the prompt as it is done here is a temporary workaround.
  % In SWI-Prolog versions up to and including 5.4.1, the prompt is written
  % to the output stream even if it is not connected to an actual terminal.
  % See http://www.cs.unipr.it/pipermail/ppl-devel/2004-October/005078.html.
  prompt(Old_Prompt, ''),
  common_main,
  prompt(_, Old_Prompt).
