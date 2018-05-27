/* Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

This file is free software; as a special exception the author gives
unlimited permission to copy and/or distribute it, with or without
modifications, as long as this notice is preserved.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. */

#ifndef PPL_PIP_Problem_types_hh
#define PPL_PIP_Problem_types_hh 1

namespace Parma_Polyhedra_Library {

//! Possible outcomes of the PIP_Problem solver.
/*! \ingroup PPL_CXX_interface */
enum PIP_Problem_Status {
  //! The problem is unfeasible.
  UNFEASIBLE_PIP_PROBLEM,
  //! The problem has an optimal solution.
  OPTIMIZED_PIP_PROBLEM
};

class PIP_Problem;

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_PIP_Problem_types_hh)
