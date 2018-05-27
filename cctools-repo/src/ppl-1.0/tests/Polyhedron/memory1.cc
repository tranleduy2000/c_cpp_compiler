/* Test the allocation error recovery facility of the library.
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

// Note: we cannot know, at this stage whether
// PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK evaluates to true.
#define PPL_NO_AUTOMATIC_INITIALIZATION
#include "ppl_test.hh"
#include <new>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#ifdef PPL_HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef PPL_HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#ifdef PPL_HAVE_SYS_RESOURCE_H
// This should be included after <time.h> and <sys/time.h> so as to make
// sure we have the definitions for, e.g., `ru_utime'.
# include <sys/resource.h>
#endif

#ifdef PPL_HAVE_UNISTD_H
# include <unistd.h>
#endif

// If GMP does not support exceptions, or if we are unable to limit
// the memory available to processes using setrlimit(), the test is
// pointless.
//
// On the Itanium the test fails because of the bug reported in
// http://www.cs.unipr.it/pipermail/ppl-devel/2008-September/012943.html
//
// On s390x-linux the test fails, we do not know why (and without access
// to such a machine there is little we can do).
// See http://www.cs.unipr.it/pipermail/ppl-devel/2009-April/014489.html
#if !PPL_GMP_SUPPORTS_EXCEPTIONS || !PPL_CXX_SUPPORTS_LIMITING_MEMORY \
  || defined(__ia64) || defined(__s390x__)

int
main() TRY {
  return 0;
}
CATCH

#else // PPL_GMP_SUPPORTS_EXCEPTIONS && !defined(__CYGWIN__) && ...

namespace {

void
compute_open_hypercube_generators(dimension_type dimension) {
  NNC_Polyhedron hypercube(dimension);
  for (dimension_type i = 0; i < dimension; ++i) {
    Variable x(i);
    hypercube.add_constraint(x > 0);
    hypercube.add_constraint(x < 1);
  }
  (void) hypercube.generators();
}

#define LIMIT(WHAT) \
  do {									\
    if (getrlimit(WHAT, &t) != 0) {					\
      std::cerr << "getrlimit failed: " << strerror(errno) << endl;	\
      exit(1);								\
    }									\
    t.rlim_cur = bytes;							\
    if (setrlimit(WHAT, &t) != 0) {					\
      std::cerr << "setrlimit failed: " << strerror(errno) << endl;	\
      exit(1);								\
    }									\
  } while (0)

void
limit_memory(unsigned long bytes) {
  struct rlimit t;
#if PPL_HAVE_DECL_RLIMIT_DATA
  // Limit heap size.
  LIMIT(RLIMIT_DATA);
#endif
#if PPL_HAVE_DECL_RLIMIT_RSS
  // Limit resident set size.
  LIMIT(RLIMIT_RSS);
#endif
#if PPL_HAVE_DECL_RLIMIT_VMEM
  // Limit mapped memory (brk + mmap).
  LIMIT(RLIMIT_VMEM);
#endif
#if PPL_HAVE_DECL_RLIMIT_AS
  // Limit virtual memory.
  LIMIT(RLIMIT_AS);
#endif
}

bool
guarded_compute_open_hypercube_generators(dimension_type dimension,
					  unsigned long max_memory_in_bytes) {
  try {
    limit_memory(max_memory_in_bytes);
    compute_open_hypercube_generators(dimension);
    return true;
  }
  catch (const std::bad_alloc&) {
    nout << "out of virtual memory" << endl;
    return false;
  }
  catch (...) {
    exit(1);
  }
  // Should never get here.
  exit(1);
}

} // namespace

extern "C" void*
cxx_malloc(size_t size) {
  void* p = malloc(size);
  if (p != 0 || size == 0)
    return p;

  throw std::bad_alloc();
}

extern "C" void*
cxx_realloc(void* q, size_t, size_t new_size) {
  void* p = realloc(q, new_size);
  if (p != 0 || new_size == 0)
    return p;

  throw std::bad_alloc();
}

extern "C" void
cxx_free(void* p, size_t) {
  free(p);
}

#define INIT_MEMORY 3*1024*1024

#if PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK
extern "C" void
ppl_set_GMP_memory_allocation_functions(void) {
  mp_set_memory_functions(cxx_malloc, cxx_realloc, cxx_free);
}
#endif // PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK

int
main() TRY {
#if !PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK
  mp_set_memory_functions(cxx_malloc, cxx_realloc, cxx_free);
#endif // !PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK

  // Note: we have included <ppl.hh> under the definition of
  // PPL_NO_AUTOMATIC_INITIALIZATION.
  Parma_Polyhedra_Library::initialize();

  set_handlers();

  // Find a dimension that cannot be computed with INIT_MEMORY bytes.
  dimension_type dimension = 0;
  do {
    ++dimension;
    nout << "Trying dimension " << dimension << endl;
  } while (guarded_compute_open_hypercube_generators(dimension, INIT_MEMORY));

  // Now find an upper bound to the memory necessary to compute it.
  unsigned long upper_bound = INIT_MEMORY;
  do {
    upper_bound *= 2;
    nout << "Trying upper bound " << upper_bound << endl;
  } while (!guarded_compute_open_hypercube_generators(dimension, upper_bound));

  // Search the "exact" amount of memory.
  int lower_bound = upper_bound/2;
  do {
    int test_memory = (lower_bound+upper_bound)/2;
    nout << "Probing " << test_memory << endl;
    if (guarded_compute_open_hypercube_generators(dimension, test_memory))
      upper_bound = test_memory;
    else
      lower_bound = test_memory;
  } while (upper_bound-lower_bound > 1024);

  nout << "Estimated memory for dimension " << dimension
       << ": " << (lower_bound+upper_bound)/2 << " bytes" << endl;

  // Note: we have included <ppl.hh> under the definition of
  // PPL_NO_AUTOMATIC_INITIALIZATION.
  Parma_Polyhedra_Library::finalize();

  return 0;
}
CATCH

#endif // PPL_GMP_SUPPORTS_EXCEPTIONS && !defined(__CYGWIN__) && ...
