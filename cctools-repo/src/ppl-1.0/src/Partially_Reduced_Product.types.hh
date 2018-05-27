/* Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

This file is free software; as a special exception the author gives
unlimited permission to copy and/or distribute it, with or without
modifications, as long as this notice is preserved.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. */

#ifndef PPL_Partially_Reduced_Product_types_hh
#define PPL_Partially_Reduced_Product_types_hh 1

namespace Parma_Polyhedra_Library {

template <typename D1, typename D2>
class Smash_Reduction;

template <typename D1, typename D2>
class Constraints_Reduction;

template <typename D1, typename D2>
class Congruences_Reduction;

template <typename D1, typename D2>
class Shape_Preserving_Reduction;

template <typename D1, typename D2>
class No_Reduction;

template <typename D1, typename D2, typename R>
class Partially_Reduced_Product;

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Partially_Reduced_Product_types_hh)
