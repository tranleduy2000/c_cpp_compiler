/* Test the allocation error recovery facility of the library.
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
#include <new>
#include <cstdlib>
#include <limits>

// If GMP does not support exceptions the test is pointless.
#if !PPL_GMP_SUPPORTS_EXCEPTIONS

int
main() TRY {
  return 0;
}
CATCH

#else // PPL_GMP_SUPPORTS_EXCEPTIONS

namespace {

bool before_main;

unsigned long mallocated = 0;
unsigned long reallocated = 0;
unsigned long freed = 0;
unsigned long malloc_threshold = 0;
unsigned long realloc_threshold = 0;

void
reset_allocators(unsigned long new_malloc_threshold,
                 unsigned long new_realloc_threshold) {
  mallocated = reallocated = freed = 0;
  malloc_threshold = new_malloc_threshold;
  realloc_threshold = new_realloc_threshold;
}

void
fail_if_leaked() {
  if (mallocated != freed) {
    nout << "Memory leak: allocated " << mallocated
         << ", freed " << freed
         << endl;
    exit(1);
  }
  else {
    nout << "allocated = freed = " << mallocated
         << endl;
  }
}

extern "C" void*
cxx_malloc(size_t size) {
  if (mallocated >= malloc_threshold) {
    nout << "std::bad_alloc thrown from cxx_malloc()" << endl;
    throw std::bad_alloc();
  }

  void* p = malloc(size);
  if (p != 0 || size == 0) {
    if (!before_main)
      vnout << "allocated " << size << " @ " << p << endl;
    ++mallocated;
    return p;
  }

  nout << "malloc() returned 0: std::bad_alloc thrown from cxx_malloc()"
       << endl;
  throw std::bad_alloc();
}

extern "C" void
cxx_free(void* p, size_t) {
  free(p);
  if (!before_main)
    vnout << "freed " << p << endl;
  ++freed;
}

extern "C" void*
cxx_realloc(void* q, size_t old_size, size_t new_size) {
  if (q == 0)
    return cxx_malloc(new_size);

  if (new_size == 0) {
    cxx_free(q, old_size);
    return 0;
  }

  if (new_size <= old_size) {
    void* p = realloc(q, new_size);
    if (p != 0 || new_size == 0) {
      if (!before_main)
        vnout << "reallocated " << old_size << " @ " << p
              << " down to " << new_size << " @ " << p
              << endl;
      return p;
    }

    nout << "malloc() returned 0: std::bad_alloc thrown from cxx_malloc()"
         << endl;
    throw std::bad_alloc();
  }

  assert(new_size > old_size);
  if (reallocated >= realloc_threshold) {
    if (!before_main)
      nout << "std::bad_alloc thrown from cxx_realloc()" << endl;
    throw std::bad_alloc();
  }

  void* p = realloc(q, new_size);
  if (p != 0 || new_size == 0) {
    if (!before_main)
      vnout << "reallocated " << old_size << " @ " << q
            << " up to " << new_size << " @ " << p
            << endl;
    ++reallocated;
    return p;
  }

  nout << "malloc() returned 0: std::bad_alloc thrown from cxx_malloc()"
       << endl;
  throw std::bad_alloc();
}

void
test1() {
  nout << "test1()" << endl;

  reset_allocators(6, ULONG_MAX);
  try {
    Matrix<Dense_Row>* matrix = new Matrix<Dense_Row>(2, 5);
    // We will get here only if no exception is thrown:
    // this happens when using native coefficients.
    delete matrix;
  }
  catch (const std::bad_alloc&) {
    nout << "std::bad_alloc caught" << endl;
    fail_if_leaked();
  }
  catch (...) {
    nout << "exception different from std::bad_alloc caught" << endl;
    fail_if_leaked();
  }
}

enum Threshold { Malloc, Realloc };

void
test_every_allocation(const dimension_type d, const Threshold threshold) {
  // Run once without checking so as to allow for the allocation of
  // statically allocated coefficients.
  bool dry_run = true;
  unsigned long k = ULONG_MAX;
  bool go_ahead;
  do {
    nout << "**************** k = " << k << " ****************" << endl;
    if (dry_run)
      nout << "*************** dry run ***************" << endl;
    go_ahead = dry_run;
    if (threshold == Malloc)
      reset_allocators(k, ULONG_MAX);
    else
      reset_allocators(ULONG_MAX, k);
    try {
      C_Polyhedron ph(d);
      ph.add_constraint(Variable(0) == ULONG_MAX);
      for (dimension_type i = 1; i < d; ++i)
        ph.add_constraint(Variable(i) == ULONG_MAX*Variable(i-1));

      (void) ph.minimized_generators();
    }
    catch (const std::bad_alloc&) {
      nout << "std::bad_alloc caught" << endl;
      fail_if_leaked();
      go_ahead = true;
      ++k;
    }
    catch (...) {
      nout << "exception different from std::bad_alloc caught" << endl;
      fail_if_leaked();
      // Notice that we do not go ahead if we did not catch a bad_alloc.
    }
    if (dry_run) {
      dry_run = false;
      k = 0;
    }
  } while (go_ahead);
}

void
test2() {
  nout << "test2()" << endl;
  test_every_allocation(5, Malloc);
}

void
test3() {
  nout << "test3()" << endl;
  test_every_allocation(10, Realloc);
}


} // namespace

#define IGNORE_OVERFLOWS(fun)                   \
  try {                                         \
    fun;                                        \
  }                                             \
  catch (const std::overflow_error&) {          \
  }

extern "C" void
ppl_set_GMP_memory_allocation_functions() {
  before_main = true;
  // Allow the static coefficients of the library to be allocated
  // without any limit.
  reset_allocators(ULONG_MAX, ULONG_MAX);
  mp_set_memory_functions(cxx_malloc, cxx_realloc, cxx_free);
}

int
main() TRY {
  before_main = false;

  set_handlers();

  // The point of this test is to detect memory leaks.
  // So we plainly ignore overflow exceptions.
  IGNORE_OVERFLOWS(test1());
  IGNORE_OVERFLOWS(test2());
  IGNORE_OVERFLOWS(test3());

  return 0;
}
CATCH

#endif // PPL_GMP_SUPPORTS_EXCEPTIONS
