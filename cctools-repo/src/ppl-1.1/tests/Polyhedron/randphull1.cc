/* Compute poly-hulls of random polytopes.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
#include <vector>
#include <cmath>

#ifndef M_PI
# define M_PI           3.14159265358979323846  /* pi */
#endif

namespace {

namespace test01_namespace {

int
count_points(const C_Polyhedron& ph) {
  int count = 0;
  const Generator_System& gs = ph.minimized_generators();
  for (Generator_System::const_iterator i = gs.begin(),
         gs_end = gs.end(); i != gs_end; ++i)
    if (i->type() == Generator::POINT)
      ++count;
  return count;
}

} // namespace test01_namespace

bool
test01() {
  // Set up a random numbers' generator.
  gmp_randclass rg(gmp_randinit_default);

  Variable x(0);
  Variable y(1);
  Variable z(2);

  const int maxc = 10000;
  C_Polyhedron ph(3, EMPTY);
  nout << test01_namespace::count_points(ph) << endl;
  for (int n = 1; n <= 200; ++n) {
    const Coefficient cx = mpz_class(rg.get_z_range(maxc));
    const Coefficient cy = mpz_class(rg.get_z_range(maxc));
    const Coefficient cz = mpz_class(rg.get_z_range(maxc));
    ph.add_generator(point(cx*x + cy*y + cz*z));
    if (ph.is_empty())
      return false;
    nout << test01_namespace::count_points(ph) << endl;
  }
  return true;
}


namespace test02_namespace {

// Compute random polytopes by generating points on the surface
// of an n-dimensional sphere.

// Please note: the following function picks up random point on the
// n-dimensional sphere, but they are not uniformly distributed.
// See the following paper on how to obtain a uniform distribution:

// J. S. Hicks, R. F. Wheeling.
// An efficient method for generating uniformly distributed points
// on the surface of an n-dimensional sphere,
// Communications of the ACM, Volume 2, Issue 4, pp. 17-19, April 1959.
//
// M. E. Muller.
// A note on a method for generating points uniformly on n-dimensional spheres,
// Communications of the ACM, Volume 2 Issue 4, pp. 19-20, April 1959.
//
// G. F. Schrack.
// Remark on algorithm 381 [G5],
// Communications of the ACM, Volume 15 Issue 6, p. 468, June 1972.

void
point_on_the_unit_n_sphere(unsigned n,
                           const std::vector<float>& theta,
                           std::vector<float>& coordinate) {
  assert(n >= 2);

  if (n == 2) {
    coordinate[0] *= sin(theta[0]);
    coordinate[1] *= cos(theta[0]);
  }
  else {
    point_on_the_unit_n_sphere(n-1, theta, coordinate);
    float sin_theta_n_2 = sin(theta[n-2]);
    for (unsigned i = n-1; i-- > 0; )
      coordinate[i] *= sin_theta_n_2;
    coordinate[n-1] *= cos(theta[n-2]);
  }
}

void
random_polytope(C_Polyhedron& ph,
                unsigned dimension,
                unsigned num_points,
                float radius = 1.0) {
  assert(dimension >= 2);

  // This setting and resetting of the rounding mode was prompted by
  // the GLIBC bug http://sources.redhat.com/bugzilla/show_bug.cgi?id=6869 .
  // However, even when this bug will be fixed, we want to keep this
  // rounding mode setting/resetting code, so that it gets tested.
  restore_pre_PPL_rounding();

  std::vector<float> theta(dimension-1);
  std::vector<float> coordinate(dimension);

  for (unsigned n = num_points; n > 0; --n) {
    // Compute n-1 random angles.
    for (unsigned i = dimension-1; i-- > 0; )
      theta[i] = 2.0*M_PI*static_cast<double>(rand())/RAND_MAX;
    // Compute float coordinates.
    for (unsigned i = dimension; i-- > 0; )
      coordinate[i] = radius;
    point_on_the_unit_n_sphere(dimension, theta, coordinate);

    Linear_Expression le;
    for (unsigned i = dimension; i-- > 0; ) {
#if 0
      le += Variable(i)*Coefficient(coordinate[i]*1000000.0);
#else
      // FIXME: this is a temporary workaround for machines
      // where we cannot control the FPU.
      mpz_class z = coordinate[i]*1000000.0;
      le += Variable(i)*Coefficient(z);
#endif
    }
    ph.add_generator(point(le));
  }

  // Restore the rounding mode as needed by the PPL.
  set_rounding_for_PPL();
}

} // namespace test02_namespace

bool
test02() {
  for (unsigned dimension = 2;
#ifdef NDEBUG
       dimension <= 6;
#else
       dimension <= 4;
#endif
       ++dimension) {
    C_Polyhedron ph(dimension, EMPTY);
    test02_namespace::random_polytope(ph, dimension, dimension*dimension);
    // Count the number of constraints.
    const Constraint_System& cs = ph.minimized_constraints();
    unsigned num_constraints = 0;
    for (Constraint_System::const_iterator i = cs.begin(),
           cs_end = cs.end(); i != cs_end; ++i)
      ++num_constraints;
    // Count the number of generators.
    const Generator_System& gs = ph.minimized_generators();
    unsigned num_points = 0;
    for (Generator_System::const_iterator i = gs.begin(),
           gs_end = gs.end(); i != gs_end; ++i)
      if (i->type() == Generator::POINT)
        ++num_points;
      else {
        using namespace Parma_Polyhedra_Library::IO_Operators;
        nout << "i->type() == " << i->type() << endl;
        return false;
      }
    nout << "dimension = " << dimension
         << ", points = " << num_points << " (" << dimension*dimension << ")"
         << ", constraints = " << num_constraints << endl;
  }
  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F64(test01);
  DO_TEST_F64(test02);
END_MAIN
