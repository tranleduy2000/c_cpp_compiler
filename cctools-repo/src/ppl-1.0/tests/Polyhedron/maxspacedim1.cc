/* Test the *::max_space_dimension() methods.
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

#include "ppl_test.hh"

#define test01_DO_CLASS(T)				\
  nout << #T "::max_space_dimension() = "		\
    << T::max_space_dimension() << endl;		\
  if (T::max_space_dimension() < max_space_dimension())	\
    return false

#if PPL_SUPPORTED_FLOAT
#define test01_DO_WRD_FLOAT(WRD) test01_DO_CLASS(WRD<float>)
#else
#define test01_DO_WRD_FLOAT(WRD)
#endif
#if PPL_SUPPORTED_DOUBLE
#define test01_DO_WRD_DOUBLE(WRD) test01_DO_CLASS(WRD<double>)
#else
#define test01_DO_WRD_DOUBLE(WRD)
#endif
#if PPL_SUPPORTED_LONG_DOUBLE
#define test01_DO_WRD_LONG_DOUBLE(WRD) test01_DO_CLASS(WRD<long double>)
#else
#define test01_DO_WRD_LONG_DOUBLE(WRD)
#endif

#define test01_DO_WRD(WRD)			\
  test01_DO_CLASS(WRD<int8_t>);			\
  test01_DO_CLASS(WRD<int16_t>);		\
  test01_DO_CLASS(WRD<int32_t>);		\
  test01_DO_CLASS(WRD<int64_t>);		\
  test01_DO_CLASS(WRD<mpz_class>);		\
  test01_DO_CLASS(WRD<mpq_class>);		\
  test01_DO_WRD_FLOAT(WRD);                     \
  test01_DO_WRD_DOUBLE(WRD);                    \
  test01_DO_WRD_LONG_DOUBLE(WRD)

#define test01_DO_CONSTR_CLASS(CONSTR, T)				\
  nout << #CONSTR "<" #T ">::max_space_dimension() = "			\
       << CONSTR<T>::max_space_dimension() << endl;			\
  if (CONSTR<T>::max_space_dimension() < max_space_dimension())		\
    return false

#if PPL_SUPPORTED_FLOAT
#define test01_DO_CONSTR_WRD_FLOAT(CONSTR, WRD) \
  test01_DO_CONSTR_CLASS(CONSTR, WRD<float>)
#else
#define test01_DO_CONSTR_WRD_FLOAT(CONSTR, WRD)
#endif
#if PPL_SUPPORTED_DOUBLE
#define test01_DO_CONSTR_WRD_DOUBLE(CONSTR, WRD)        \
  test01_DO_CONSTR_CLASS(CONSTR, WRD<double>)
#else
#define test01_DO_CONSTR_WRD_DOUBLE(CONSTR, WRD)
#endif
#if PPL_SUPPORTED_LONG_DOUBLE
#define test01_DO_CONSTR_WRD_LONG_DOUBLE(CONSTR, WRD)   \
  test01_DO_CONSTR_CLASS(CONSTR, WRD<long double>)
#else
#define test01_DO_CONSTR_WRD_LONG_DOUBLE(CONSTR, WRD)
#endif

#define test01_DO_CONSTR_WRD(CONSTR, WRD)			\
  test01_DO_CONSTR_CLASS(CONSTR, WRD<int8_t>);			\
  test01_DO_CONSTR_CLASS(CONSTR, WRD<int16_t>);			\
  test01_DO_CONSTR_CLASS(CONSTR, WRD<int32_t>);			\
  test01_DO_CONSTR_CLASS(CONSTR, WRD<int64_t>);			\
  test01_DO_CONSTR_CLASS(CONSTR, WRD<mpz_class>);		\
  test01_DO_CONSTR_CLASS(CONSTR, WRD<mpq_class>);		\
  test01_DO_CONSTR_WRD_FLOAT(CONSTR, WRD);                      \
  test01_DO_CONSTR_WRD_DOUBLE(CONSTR, WRD);                     \
  test01_DO_CONSTR_WRD_LONG_DOUBLE(CONSTR, WRD)

bool
test01() {
  test01_DO_CLASS(Variable);
  test01_DO_CLASS(Variables_Set);
  test01_DO_CLASS(Linear_Expression);
  test01_DO_CLASS(Constraint);
  test01_DO_CLASS(Congruence);
  test01_DO_CLASS(Generator);
  test01_DO_CLASS(Grid_Generator);
  test01_DO_CLASS(Constraint_System);
  test01_DO_CLASS(Congruence_System);
  test01_DO_CLASS(Generator_System);
  test01_DO_CLASS(Grid_Generator_System);
  test01_DO_CLASS(C_Polyhedron);
  test01_DO_CLASS(NNC_Polyhedron);
  test01_DO_CLASS(Grid);
  // FIXME: what about all other boxes?
  test01_DO_CLASS(Rational_Box);
  test01_DO_WRD(BD_Shape);
  test01_DO_WRD(Octagonal_Shape);
  test01_DO_CONSTR_CLASS(Pointset_Powerset, C_Polyhedron);
  test01_DO_CONSTR_CLASS(Pointset_Powerset, NNC_Polyhedron);
  test01_DO_CONSTR_CLASS(Pointset_Powerset, Grid);
  test01_DO_CONSTR_WRD(Pointset_Powerset, BD_Shape);
  test01_DO_CONSTR_WRD(Pointset_Powerset, Octagonal_Shape);
  //test01_DO_CONSTR_CLASS(Pointset_Ask_Tell, C_Polyhedron);
  //test01_DO_CONSTR_CLASS(Pointset_Ask_Tell, NNC_Polyhedron);
  //test01_DO_CONSTR_CLASS(Pointset_Ask_Tell, Grid);
  //test01_DO_CONSTR_WRD(Pointset_Ask_Tell, BD_Shape);
  //test01_DO_CONSTR_WRD(Pointset_Ask_Tell, Octagonal_Shape);

  nout << "Parma_Polyhedra_Library::max_space_dimension() = "
       << max_space_dimension() << endl;

  // 9458 is the value of max_space_dimension()
  // computed on a 32bit architecture.
  return max_space_dimension() >= 9458;
}

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN

