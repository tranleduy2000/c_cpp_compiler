/* Widening_Function class implementation: inline functions.
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

#ifndef PPL_Widening_Function_inlines_hh
#define PPL_Widening_Function_inlines_hh 1

#include "assert.hh"

namespace Parma_Polyhedra_Library {

template <typename PSET>
Widening_Function<PSET>::Widening_Function(Widening_Method wm)
  : w_method(wm) {
}

template <typename PSET>
inline void
Widening_Function<PSET>::
operator()(PSET& x, const PSET& y, unsigned* tp) const {
  (x.*w_method)(y, tp);
}

template <typename PSET, typename CSYS>
Limited_Widening_Function<PSET, CSYS>::
Limited_Widening_Function(Limited_Widening_Method lwm,
			  const CSYS& cs)
  : lw_method(lwm), limiting_cs(cs) {
}

template <typename PSET, typename CSYS>
inline void
Limited_Widening_Function<PSET, CSYS>::
operator()(PSET& x, const PSET& y, unsigned* tp) const {
  (x.*lw_method)(y, limiting_cs, tp);
}

/*! \relates Pointset_Powerset */
template <typename PSET>
inline Widening_Function<PSET>
widen_fun_ref(void (PSET::* wm)(const PSET&, unsigned*)) {
  return Widening_Function<PSET>(wm);
}

/*! \relates Pointset_Powerset */
template <typename PSET, typename CSYS>
inline Limited_Widening_Function<PSET, CSYS>
widen_fun_ref(void (PSET::* lwm)(const PSET&, const CSYS&, unsigned*),
	      const CSYS& cs) {
  return Limited_Widening_Function<PSET, CSYS>(lwm, cs);
}

} // namespace Parma_Polyhedra_Library


#endif // !defined(PPL_Widening_Function_inlines_hh)
