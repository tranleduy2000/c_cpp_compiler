/* A sort of clone of the cddlib test program `lcdd'.
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

//#define USE_POLKA 1
//#define USE_POLYLIB 1

#if (!defined(USE_PPL) && !defined(USE_POLKA) && !defined(USE_POLYLIB))
#define USE_PPL 1
#elif \
   (defined(USE_PPL)   && defined(USE_POLKA)) \
|| (defined(USE_PPL)   && defined(USE_POLYLIB)) \
|| (defined(USE_POLKA) && defined(USE_POLYLIB))
#error "Exactly one among USE_PPL, USE_POLKA and USE_POLYLIB must be defined"
#endif

#if defined(USE_PPL)

#include "ppl.hh"

namespace PPL = Parma_Polyhedra_Library;

#if PPL_VERSION_MAJOR == 0 && PPL_VERSION_MINOR < 6
#error "PPL version 0.6 or following is required"
#endif

typedef PPL::C_Polyhedron POLYHEDRON_TYPE;

#if !PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK
extern "C" void
ppl_set_GMP_memory_allocation_functions(void) {
}
#endif

#elif defined(USE_POLKA)

#include <ppl-config.h>
#include <gmp.h>

extern "C" {
#define bool polka_bool
#define true polka_true
#define false polka_false
#define POLKA_NUM 3
#include <polka/poly.h>
#undef POLKA_NUM
#undef false
#undef true
#undef bool
#undef pkint_set
#define pkint_set(a,b) mpz_set((a).rep, (b))
}

typedef poly_t* POLYHEDRON_TYPE;

#elif defined(USE_POLYLIB)

#include <ppl-config.h>
#include <gmp.h>

extern "C" {
#include "polylib/polylibgmp.h"
}

// This is required (and, yes, 20000 is a magic number ;-)
const unsigned max_constraints_or_generators = 20000;

typedef Polyhedron* POLYHEDRON_TYPE;

#endif

#include "timings.hh"
#include <gmpxx.h>
#include <vector>
#include <set>
#include <limits>
#include <climits>
#include <cassert>
#include <cstdarg>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#ifdef PPL_HAVE_GETOPT_H
#include <getopt.h>

// Try to accommodate non-GNU implementations of `getopt()'.
#if !defined(no_argument) && defined(NO_ARG)
#define no_argument NO_ARG
#endif

#if !defined(required_argument) && defined(REQUIRED_ARG)
#define required_argument REQUIRED_ARG
#endif

#if !defined(optional_argument) && defined(OPTIONAL_ARG)
#define optional_argument OPTIONAL_ARG
#endif

#endif // defined(PPL_HAVE_GETOPT_H)

#ifdef PPL_HAVE_UNISTD_H
// Include this for `getopt()': especially important if we do not have
// <getopt.h>.
# include <unistd.h>
#endif

#ifdef PPL_HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#ifdef PPL_HAVE_SYS_RESOURCE_H
// This should be included after <time.h> and <sys/time.h> so as to make
// sure we have the definitions for, e.g., `ru_utime'.
# include <sys/resource.h>
#endif

#if defined(PPL_HAVE_SYS_RESOURCE_H) \
  && PPL_CXX_SUPPORTS_LIMITING_MEMORY \
  && (defined(SA_ONESHOT) || defined(SA_RESETHAND))
# define PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME
#endif

namespace {

#ifdef PPL_HAVE_GETOPT_H
struct option long_options[] = {
  {"max-cpu",        required_argument, 0, 'C'},
  {"max-memory",     required_argument, 0, 'R'},
  {"help",           no_argument,       0, 'h'},
  {"output",         required_argument, 0, 'o'},
  {"timings",        no_argument,       0, 't'},
  {"verbose",        no_argument,       0, 'v'},
#if defined(USE_PPL)
  {"version",        no_argument,       0, 'V'},
  {"check",          required_argument, 0, 'c'},
#endif
  {0, 0, 0, 0}
};
#endif

static const char* usage_string
= "Usage: %s [OPTION]... [FILE]\n"
"Reads an H-representation (resp., a V-representation) of a polyhedron\n"
"and generates a V-representation (resp., an H-representation) of\n"
"the same polyhedron.\n\n"
"Options:\n"
#ifdef PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME
"  -CSECS, --max-cpu=SECS  limits CPU usage to SECS seconds\n"
#endif // defined(PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME)
"  -RMB, --max-memory=MB   limits memory usage to MB megabytes\n"
"  -h, --help              prints this help text to stdout\n"
"  -oPATH, --output=PATH   appends output to PATH\n"
"  -t, --timings           prints timings to stderr\n"
"  -v, --verbose           produces lots of output\n"
#if defined(USE_PPL)
"  -V, --version           prints version information to stdout\n"
"  -cPATH, --check=PATH    checks if the result is equal to what is in PATH\n"
#endif
#ifndef PPL_HAVE_GETOPT_H
"\n"
"NOTE: this version does not support long options.\n"
#endif
"\n"
"Report bugs to <ppl-devel@cs.unipr.it>.\n";

#if defined(USE_PPL)
#define OPTION_LETTERS "C:R:ho:tvVc:"
#else
#define OPTION_LETTERS "C:R:ho:tv"
#endif

const char* program_name = 0;

#ifdef PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME
unsigned long max_seconds_of_cpu_time = 0;
#endif // defined(PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME)

unsigned long max_bytes_of_virtual_memory = 0;
bool print_timings = false;
bool verbose = false;
const char* check_file_name = 0;

void
fatal(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "%s: ", program_name);
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

namespace {

const char* input_file_name = 0;
std::istream* input_stream_p = 0;

void
set_input(const char* file_name) {
  if (input_stream_p && *input_stream_p != std::cin)
    delete input_stream_p;

  if (file_name) {
    input_stream_p = new std::ifstream(file_name, std::ios_base::in);
    if (!*input_stream_p)
      fatal("cannot open input file `%s'", file_name);
    input_file_name = file_name;
  }
  else {
    input_stream_p = &std::cin;
    input_file_name = "<cin>";
  }
}

std::istream&
input() {
  assert(input_stream_p != 0);
  return *input_stream_p;
}

const char* output_file_name = 0;
std::ostream* output_stream_p = 0;

void
set_output(const char* file_name) {
  if (output_stream_p && *output_stream_p != std::cout)
    delete output_stream_p;

  if (file_name) {
    output_stream_p = new std::ofstream(file_name,
					std::ios_base::out
					| std::ios_base::app);
    if (!*output_stream_p)
      fatal("cannot open output file `%s'", file_name);
    output_file_name = file_name;
  }
  else {
    output_stream_p = &std::cout;
    output_file_name = "<cout>";
  }
}

std::ostream&
output() {
  assert(output_stream_p != 0);
  return *output_stream_p;
}

} // namespace

void
error(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "%s: in `%s': ", program_name, input_file_name);
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

void
warning(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "%s: Warning: in `%s': ", program_name, input_file_name);
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

#ifdef PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME

extern "C" typedef void (*sig_handler_type)(int);

void
set_alarm_on_cpu_time(const unsigned long seconds, sig_handler_type handler) {
  sigset_t mask;
  sigemptyset(&mask);

  struct sigaction s;
  s.sa_handler = handler;
  s.sa_mask = mask;
#if defined(SA_ONESHOT)
  s.sa_flags = SA_ONESHOT;
#elif defined(SA_RESETHAND)
  s.sa_flags = SA_RESETHAND;
#else
# error "Either SA_ONESHOT or SA_RESETHAND must be defined."
#endif

  if (sigaction(SIGXCPU, &s, 0) != 0)
    fatal("sigaction failed: %s", strerror(errno));

  struct rlimit t;
  if (getrlimit(RLIMIT_CPU, &t) != 0)
    fatal("getrlimit failed: %s", strerror(errno));

  if (seconds < t.rlim_cur) {
    t.rlim_cur = seconds;
    if (setrlimit(RLIMIT_CPU, &t) != 0)
      fatal("setrlimit failed: %s", strerror(errno));
  }
}

#endif // PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME

#if PPL_CXX_SUPPORTS_LIMITING_MEMORY && PPL_HAVE_DECL_RLIMIT_AS

void
limit_virtual_memory(const unsigned long bytes) {
  struct rlimit t;

  if (getrlimit(RLIMIT_AS, &t) != 0)
    fatal("getrlimit failed: %s", strerror(errno));

  if (bytes < t.rlim_cur) {
    t.rlim_cur = bytes;
    if (setrlimit(RLIMIT_AS, &t) != 0)
      fatal("setrlimit failed: %s", strerror(errno));
  }
}

#else

void
limit_virtual_memory(unsigned long) {
}

#endif // !PPL_HAVE_DECL_RLIMIT_AS

extern "C" void
timeout(int) {
  try {
    std::cerr << "TIMEOUT"
	      << std::endl;
  }
  catch (...) {
  }

  try {
    if (output_file_name)
      output() << "TIMEOUT"
	       << std::endl;
  }
  catch (...) {
  }

  exit(0);
}

void
process_options(int argc, char* argv[]) {
  while (true) {
#ifdef PPL_HAVE_GETOPT_H
    int option_index = 0;
    int c = getopt_long(argc, argv, OPTION_LETTERS, long_options,
			&option_index);
#else
    int c = getopt(argc, argv, OPTION_LETTERS);
#endif

    if (c == EOF)
      break;

    char* endptr;
    long l;
    switch (c) {
    case 0:
      break;

    case '?':
    case 'h':
      fprintf(stdout, usage_string, argv[0]);
      exit(0);
      break;

#ifdef PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME

    case 'C':
      l = strtol(optarg, &endptr, 10);
      if (*endptr || l < 0)
	fatal("a non-negative integer must follow `-C'");
      else
	max_seconds_of_cpu_time = l;
      break;

#endif // defined(PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME)

    case 'R':
      {
        const int MEGA = 1024*1024;
        l = strtol(optarg, &endptr, 10);
        if (*endptr || l < 0)
          fatal("a non-negative integer must follow `-R'");
        else if (static_cast<unsigned long>(l) > ULONG_MAX/MEGA)
          max_bytes_of_virtual_memory = ULONG_MAX;
        else
          max_bytes_of_virtual_memory = l*MEGA;
      }
      break;

    case 'o':
      output_file_name = optarg;
      break;

    case 't':
      print_timings = true;
      break;

    case 'v':
      verbose = true;
      break;

#if defined(USE_PPL)

    case 'V':
      fprintf(stdout, "%s\n", PPL_VERSION);
      exit(0);
      break;

    case 'c':
      check_file_name = optarg;
      break;

#endif

    default:
      abort();
    }
  }

  if (argc - optind > 1)
    // We have multiple input files.
    fatal("at most one input file is accepted");

  // We have one input files.
  if (optind < argc)
    input_file_name = argv[optind];
  else
    // If no input files have been specified: we will read from standard input.
    assert(input_file_name == 0);
}

void
maybe_start_clock() {
  if (print_timings)
    start_clock();
}

void
maybe_print_clock() {
  if (print_timings) {
    std::cerr << input_file_name << " ";
    print_clock(std::cerr);
    std::cerr << std::endl;
  }
}


void
normalize(const std::vector<mpq_class>& source,
	  std::vector<mpz_class>& dest,
	  mpz_class& denominator) {
  typedef std::vector<mpq_class>::size_type size_type;
  size_type n = source.size();
  denominator = 1;
  for (size_type i = 0; i < n; ++i)
    mpz_lcm(denominator.get_mpz_t(),
	    denominator.get_mpz_t(),
	    source[i].get_den().get_mpz_t());
  for (size_type i = 0; i < n; ++i)
    dest[i] = denominator*source[i];
}

template <typename T>
bool
guarded_read(std::istream& in, T& x) {
  try {
    in >> x;
    return !in.fail();
  }
  catch (...) {
    return false;
  }
}

template <typename T>
void
guarded_write(std::ostream& out, const T& x) {
  bool succeeded = false;
  try {
    out << x;
    succeeded = !out.fail();
  }
  catch (...) {
  }
  if (!succeeded)
    fatal("cannot write to output file `%s'", output_file_name);
}

#if defined (USE_POLKA)
template <>
void
guarded_write(std::ostream& out, const pkint_t& x) {
  bool succeeded = false;
  try {
    succeeded = out << x.rep;
  }
  catch (...) {
  }
  if (!succeeded)
    fatal("cannot write to output file `%s'", output_file_name);
}
#endif

enum Number_Type { INTEGER, RATIONAL, REAL };

void
read_coefficients(std::istream& in,
		  const Number_Type number_type,
		  std::vector<mpz_class>& coefficients,
		  mpz_class& denominator) {
  typedef std::vector<mpz_class>::size_type size_type;
  size_type num_coefficients = coefficients.size();
  switch (number_type) {
  case INTEGER:
    {
      for (unsigned i = 0; i < num_coefficients; ++i)
	if (!guarded_read(in, coefficients[i]))
	  error("missing or invalid integer coefficient");
      denominator = 1;
      break;
    }
  case RATIONAL:
    {
      std::vector<mpq_class> rational_coefficients(num_coefficients);
      for (unsigned i = 0; i < num_coefficients; ++i)
	if (!guarded_read(in, rational_coefficients[i]))
	  error("missing or invalid rational coefficient");
      normalize(rational_coefficients, coefficients, denominator);
      break;
    }
  case REAL:
    {
      std::vector<mpq_class> rational_coefficients(num_coefficients);
      for (unsigned i = 0; i < num_coefficients; ++i) {
	double d;
	if (!guarded_read(in, d))
	  error("missing or invalid real coefficient");
	rational_coefficients[i] = mpq_class(d);
      }
      normalize(rational_coefficients, coefficients, denominator);
      break;
    }
  }
}

void
read_indexes_set(std::istream& in,
		 std::set<unsigned>& dest,
		 const char* what) {
  assert(dest.empty());
  unsigned num_elements;
  if (!guarded_read(in, num_elements))
    error("missing or invalid number of set elements in `%s'", what);
  while (num_elements--) {
    unsigned i;
    if (!guarded_read(in, i))
      error("missing or invalid set element in `%s'", what);
    dest.insert(i);
  }
}

enum Representation { H, V };

Representation
read_polyhedron(std::istream& in, POLYHEDRON_TYPE& ph) {
  // By default we have an H-representation.
  Representation rep = H;

  std::string s;
  std::set<unsigned> linearity;
  while (true) {
    if (!guarded_read(in, s))
      error("premature end of file while seeking for `begin'");

    if (s == "V-representation")
      rep = V;
    else if (s == "H-representation")
      rep = H;
    else if (s == "linearity" || s == "equality" || s == "partial_enum") {
      read_indexes_set(in, linearity, "linearity");
      if (verbose) {
	std::cerr << "Linearity: ";
	for (std::set<unsigned>::const_iterator j = linearity.begin(),
	       linearity_end = linearity.end(); j != linearity_end; ++j)
	  std::cerr << *j << " ";
	std::cerr << std::endl;
      }
    }
    else if (s == "begin")
      break;
    else
      // A comment: skip to end of line.
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  // Tools such as `lrs' produce "*****" instead of the number of
  // rows.  We will accept that as valid input and compute the number
  // of rows ourselves.
  bool has_num_rows = false;
  unsigned num_rows;
  if (!guarded_read(in, s))
    error("missing number of rows");
  if (s != "*****") {
    std::istringstream iss(s);
    if (!guarded_read(iss, num_rows))
      error("illegal number of rows `%s' (\"*****\" would be accepted)",
	    s.c_str());
    has_num_rows = true;
  }

  unsigned num_columns;
  if (!guarded_read(in, num_columns))
    error("illegal or missing number of columns");
  unsigned space_dim = num_columns - 1;

  if (!guarded_read(in, s))
    error("missing number type");
  Number_Type number_type = INTEGER;
  if (s == "integer")
    number_type = INTEGER;
  else if (s == "rational")
    number_type = RATIONAL;
  else if (s == "real")
    number_type = REAL;
  else
    error("illegal number type `%s'", s.c_str());

  if (verbose) {
    std::cerr << "Problem dimension: ";
    if (has_num_rows)
      std::cerr << num_rows;
    else
      std::cerr << '?';
    std::cerr << " x " << num_columns
	      << "; number type: " << s
	      << std::endl;
  }

#if defined(USE_PPL)

  PPL::Constraint_System cs;
  PPL::Generator_System gs;

#elif defined(USE_POLKA)

  // Initialize polka in non-strict mode.
  // 25000 is a magic number: with 22500 Polka 2.0.2 dies on
  // mit31-20.ine with the error "Chernikova: out of table space".
  polka_initialize(polka_false, space_dim, 25000);
  // Declare and allocate a Polka matrix.
  matrix_t* mat = matrix_alloc(num_rows+1, space_dim+2, polka_false);

#elif defined(USE_POLYLIB)

  // Declare and allocate a PolyLib matrix.
  Matrix* mat = Matrix_Alloc(num_rows+1, space_dim+2);

#endif

  unsigned row = 0;
  std::set<unsigned>::iterator linearity_end = linearity.end();
  if (rep == V) {
    // The V representation allows for `space_dim' coordinates.
    std::vector<mpz_class> coefficients(space_dim);
    mpz_class denominator;
    bool has_a_point = false;
    for (row = 0; !has_num_rows || row < num_rows; ++row) {
      int vertex_marker;
      if (!has_num_rows) {
	// Must be prepared to read an "end" here.
	if (!guarded_read(in, s))
	  error("missing vertex marker");
	if (s == "end")
	  break;
	std::istringstream iss(s);
	if (!guarded_read(iss, vertex_marker)
	    || vertex_marker < 0 || vertex_marker > 1)
	  error("illegal vertex marker `%s'", s.c_str());
      }
      else if (!guarded_read(in, vertex_marker)
		 || vertex_marker < 0 || vertex_marker > 1)
	error("illegal or missing vertex marker");
      read_coefficients(in, number_type, coefficients, denominator);

#if defined(USE_PPL)
      // PPL variables have indices 0, 1, ..., space_dim-1.
      PPL::Linear_Expression e;
      for (unsigned j = space_dim; j-- > 0; )
	e += coefficients[j] * PPL::Variable(j);
#elif defined(USE_POLKA)
      // NewPolka variables have indices 2, 3, ..., space_dim+1.
      for (unsigned j = space_dim; j-- > 0; )
	pkint_set(mat->p[row][j+2], coefficients[j].get_mpz_t());
#elif defined(USE_POLYLIB)
      // PolyLib variables have indices 1, 2, ..., space_dim.
      for (unsigned j = space_dim; j-- > 0; )
	value_assign(mat->p[row][j+1], coefficients[j].get_mpz_t());
#endif
      if (vertex_marker == 1) {
	assert(linearity.find(row+1) == linearity_end);
#if defined(USE_PPL)
	gs.insert(point(e, denominator));
#elif defined(USE_POLKA)
	// NewPolka stores the generator kind at index 0 (1 = ray/point)
	// and the common denominator at index 1.
	pkint_set_si(mat->p[row][0], 1);
	pkint_set(mat->p[row][1], denominator.get_mpz_t());
#elif defined(USE_POLYLIB)
	// PolyLib stores the generator kind at index 0 (1 = ray/point)
	// and the common denominator at index space_dim+1.
	value_set_si(mat->p[row][0], 1);
	value_assign(mat->p[row][space_dim+1], denominator.get_mpz_t());
#endif
	has_a_point = true;
      }
      else if (linearity.find(row+1) != linearity_end) {
#if defined(USE_PPL)
	gs.insert(line(e));
#elif defined(USE_POLKA)
	// NewPolka stores the generator kind at index 0 (0 = line)
	// and the common denominator at index 1 (0 for ray/line).
	pkint_set_si(mat->p[row][0], 0);
	pkint_set_si(mat->p[row][1], 0);
#elif defined(USE_POLYLIB)
	// PolyLib stores the generator kind at index 0 (0 = line)
	// and the common denominator at index space_dim+1 (0 for ray/line).
	value_set_si(mat->p[row][0], 0);
	value_set_si(mat->p[row][space_dim+1], 0);
#endif
      }
      else {
#if defined(USE_PPL)
	gs.insert(ray(e));
#elif defined(USE_POLKA)
	// NewPolka stores the generator kind at index 0 (1 = ray/point)
	// and the common denominator at index 1 (0 for ray/line).
	pkint_set_si(mat->p[row][0], 1);
	pkint_set_si(mat->p[row][1], 0);
#elif defined(USE_POLYLIB)
	// PolyLib stores the generator kind at index 0 (1 = ray/point)
	// and the common denominator at index space_dim+1 (0 for ray/line).
	value_set_si(mat->p[row][0], 1);
	value_set_si(mat->p[row][space_dim+1], 0);
#endif
      }
    }
    // Every non-empty generator system must have at least one point.
    if (row > 0 && !has_a_point) {
#if defined(USE_PPL)
      gs.insert(PPL::point());
#elif defined(USE_POLKA)
      // Add the origin as a point.
      pkint_set_si(mat->p[num_rows][0], 1);
      pkint_set_si(mat->p[num_rows][1], 1);
      for (unsigned j = space_dim; j-- > 0; )
	pkint_set_si(mat->p[num_rows][j+2], 0);
      ++num_rows;
#elif defined(USE_POLYLIB)
      // Add the origin as a point.
      value_set_si(mat->p[num_rows][0], 1);
      value_set_si(mat->p[num_rows][space_dim+1], 1);
      for (unsigned j = space_dim; j-- > 0; )
	value_set_si(mat->p[num_rows][j+1], 0);
      ++num_rows;
#endif
    }

    if (verbose) {
      if (!has_num_rows)
	std::cerr << "Problem dimension: " << row << " x " << num_columns
		  << "; number type: " << s
		  << std::endl;

#if defined(USE_PPL)
      using namespace PPL::IO_Operators;
      std::cerr << "Generator system:\n" << gs << std::endl;
#elif defined(USE_POLKA)
      // Polka can only print to stdout.
      printf("Generator system:\n");
      matrix_print(mat);
#elif defined(USE_POLYLIB)
      fprintf(stderr, "Generator system:\n");
      Matrix_Print(stderr, 0, mat);
#endif
    }
  }
  else {
    assert(rep == H);
    // The H representation stores the inhomogeneous term at index 0,
    // and the variables' coefficients at indices 1, 2, ..., space_dim.
    std::vector<mpz_class> coefficients(space_dim+1);
    mpz_class denominator;
    for (row = 0; !has_num_rows || row < num_rows; ++row) {
      if (!has_num_rows) {
	// Must be prepared to read an "end" here.
	std::getline(in, s);
	if (!in)
	  error("premature end of file while seeking "
		"for coefficients or `end'");
	if (s.substr(0, 2) == "end")
	  break;
	std::istringstream iss(s);
	read_coefficients(iss, number_type, coefficients, denominator);
      }
      else
	read_coefficients(in, number_type, coefficients, denominator);

#if defined(USE_PPL)
      // PPL variables have indices 0, 1, ..., space_dim-1.
      PPL::Linear_Expression e;
      for (unsigned j = num_columns; j-- > 1; )
	e += coefficients[j] * PPL::Variable(j-1);
      e += coefficients[0];
#elif defined(USE_POLKA)
      // NewPolka variables have indices 2, 3, ..., space_dim+1.
      for (unsigned j = num_columns; j-- > 1; )
	pkint_set(mat->p[row][j+1], coefficients[j].get_mpz_t());
      // NewPolka stores the inhomogeneous term at index 1.
      pkint_set(mat->p[row][1], coefficients[0].get_mpz_t());
#elif defined(USE_POLYLIB)
      // PolyLib variables have indices 1, 2, ..., space_dim.
      for (unsigned j = num_columns; j-- > 1; )
	value_assign(mat->p[row][j], coefficients[j].get_mpz_t());
      // PolyLib stores the inhomogeneous term at index space_dim+1.
      value_assign(mat->p[row][space_dim+1], coefficients[0].get_mpz_t());
#endif

      if (linearity.find(row+1) != linearity_end) {
#if defined(USE_PPL)
	cs.insert(e == 0);
#elif defined(USE_POLKA)
	// NewPolka stores the constraint kind at index 0 (0 = equality).
	pkint_set_si(mat->p[row][0], 0);
#elif defined(USE_POLYLIB)
	// PolyLib stores the constraint kind at index 0 (0 = equality).
	value_set_si(mat->p[row][0], 0);
#endif
      }
      else {
#if defined(USE_PPL)
	cs.insert(e >= 0);
#elif defined(USE_POLKA)
	// NewPolka stores the constraint kind at index 0 (1 = inequality).
	pkint_set_si(mat->p[row][0], 1);
#elif defined(USE_POLYLIB)
	// PolyLib stores the constraint kind at index 0 (1 = inequality).
	value_set_si(mat->p[row][0], 1);
#endif
      }
    }

    if (verbose) {
      if (!has_num_rows)
	std::cerr << "Problem dimension: " << row << " x " << num_columns
		  << "; number type: " << s
		  << std::endl;

#if defined(USE_PPL)
      using namespace PPL::IO_Operators;
      std::cerr << "Constraint system:\n" << cs << std::endl;
#elif defined(USE_POLKA)
      // Polka can only print to stdout.
      printf("Constraint system:\n");
      matrix_print(mat);
#elif defined(USE_POLYLIB)
      fprintf(stderr, "Constraint system:\n");
      Matrix_Print(stderr, 0, mat);
#endif
    }
  }

  if (has_num_rows) {
    if (!guarded_read(in, s))
      error("premature end of file while seeking for `end'");

    if (s != "end")
      error("`%s' found while seeking for `end'", s.c_str());
  }

  if (rep == H) {
#if defined(USE_PPL)
    ph = PPL::C_Polyhedron(cs, PPL::Recycle_Input());
#elif defined(USE_POLKA)
    ph = poly_universe(space_dim);
    ph = poly_add_constraints_lazy(ph, mat);
#elif defined(USE_POLYLIB)
    ph = Universe_Polyhedron(space_dim);

    // PolyLib is not lazy: it will perform the conversion immediately.
    maybe_start_clock();
    ph = AddConstraints(mat->p[0], num_rows, ph,
			max_constraints_or_generators);
    maybe_print_clock();
#endif
  }
  else {
#if defined(USE_PPL)
    ph = PPL::C_Polyhedron(gs, PPL::Recycle_Input());
#elif defined(USE_POLKA)
    ph = poly_of_frames(mat);
#elif defined(USE_POLYLIB)
    ph = Empty_Polyhedron(space_dim);

    // PolyLib is not lazy: it will perform the conversion immediately.
    maybe_start_clock();
    ph = AddRays(mat->p[0], num_rows, ph,
		 max_constraints_or_generators);
    maybe_print_clock();
#endif
  }
  return rep;
}

void
write_polyhedron(std::ostream& out,
		 const POLYHEDRON_TYPE& ph,
		 const Representation rep) {
  if (rep == H)
    guarded_write(out, "H-representation\n");
  else {
    assert(rep == V);
    guarded_write(out, "V-representation\n");
  }

  std::set<unsigned> linearity;
#if defined(USE_PPL)
  unsigned num_rows = 0;
  if (rep == H) {
    const PPL::Constraint_System& cs = ph.constraints();
    for (PPL::Constraint_System::const_iterator i = cs.begin(),
	   cs_end = cs.end(); i != cs_end; ++i) {
      ++num_rows;
      if (i->is_equality())
	linearity.insert(linearity.end(), num_rows);
    }
  }
  else {
    const PPL::Generator_System& gs = ph.generators();
    for (PPL::Generator_System::const_iterator i = gs.begin(),
	   gs_end = gs.end(); i != gs_end; ++i) {
      ++num_rows;
      if (i->is_line())
	linearity.insert(linearity.end(), num_rows);
    }
  }
#elif defined(USE_POLKA)
  // Don't even try to get frames if the polyhedron is empty.
  const matrix_t* mat = (rep == H)
    ? poly_constraints(ph)
    : (poly_is_empty(ph) ? 0 : poly_frames(ph));
  const unsigned num_rows = (rep == V && poly_is_empty(ph)) ? 0 : mat->nbrows;
  for (unsigned i = 0; i < num_rows; ++i)
    if (pkint_sgn(mat->p[i][0]) == 0)
      linearity.insert(linearity.end(), i+1);
#elif defined(USE_POLYLIB)
  const Matrix* mat = (rep == H)
    ? Polyhedron2Constraints(ph)
    : Polyhedron2Rays(ph);
  const unsigned num_rows = mat->NbRows;
  for (unsigned i = 0; i < num_rows; ++i)
    if (value_sign(mat->p[i][0]) == 0)
      linearity.insert(linearity.end(), i+1);
#endif

  if (!linearity.empty()) {
    guarded_write(out, "linearity ");
    guarded_write(out, linearity.size());
    for (std::set<unsigned>::const_iterator j = linearity.begin(),
	   linearity_end = linearity.end(); j != linearity_end; ++j) {
      guarded_write(out, ' ');
      guarded_write(out, *j);
    }
    guarded_write(out, '\n');
  }

#if defined(USE_PPL)
  PPL::dimension_type space_dim = ph.space_dimension();
#elif defined(USE_POLKA)
  unsigned space_dim = poly_dimension(ph);
#elif defined(USE_POLYLIB)
  unsigned space_dim = mat->NbColumns - 2;
#endif

  guarded_write(out, "begin\n");
  guarded_write(out, num_rows);
  guarded_write(out, ' ');
  guarded_write(out, space_dim+1);
  guarded_write(out, ' ');
  if (rep == H)
    guarded_write(out, "integer\n");
  else
    guarded_write(out, "rational\n");

#if defined(USE_PPL)
  if (rep == H) {
    const PPL::Constraint_System& cs = ph.constraints();
    for (PPL::Constraint_System::const_iterator i = cs.begin(),
	   cs_end = cs.end(); i != cs_end; ++i) {
      const PPL::Constraint& c = *i;
      guarded_write(out, c.inhomogeneous_term());
      for (PPL::dimension_type j = 0; j < space_dim; ++j) {
	guarded_write(out, ' ');
	guarded_write(out, c.coefficient(PPL::Variable(j)));
      }
      guarded_write(out, '\n');
    }
  }
  else {
    assert(rep == V);
    const PPL::Generator_System& gs = ph.generators();
    for (PPL::Generator_System::const_iterator i = gs.begin(),
	   gs_end = gs.end(); i != gs_end; ++i) {
      const PPL::Generator& g = *i;
      if (g.is_point()) {
	guarded_write(out, '1');
	const PPL::Coefficient& divisor = g.divisor();
	for (PPL::dimension_type j = 0; j < space_dim; ++j) {
	  guarded_write(out, ' ');
	  if (g.coefficient(PPL::Variable(j)) == 0)
	    guarded_write(out, '0');
	  else {
	    mpz_class numer, denom;
	    PPL::assign_r(numer,
			g.coefficient(PPL::Variable(j)),
			PPL::ROUND_NOT_NEEDED);
	    PPL::assign_r(denom, divisor, PPL::ROUND_NOT_NEEDED);
	    guarded_write(out, mpq_class(numer, denom));
	  }
	}
      }
      else {
	// `g' is a ray or a line.
	guarded_write(out, '0');
	for (PPL::dimension_type j = 0; j < space_dim; ++j) {
	  guarded_write(out, ' ');
	  guarded_write(out, g.coefficient(PPL::Variable(j)));
	}
      }
      guarded_write(out, '\n');
    }
  }
#elif defined(USE_POLKA)
  if (rep == H) {
    for (unsigned i = 0; i < num_rows; ++i) {
      const pkint_t* c = mat->p[i];
      // The inhomogeneous term.
      guarded_write(out, c[1]);
      // The variables' coefficients.
      for (unsigned j = 0; j < space_dim; ++j) {
	guarded_write(out, ' ');
	guarded_write(out, c[j+2]);
      }
      guarded_write(out, '\n');
    }
  }
  else {
    assert(rep == V);
    for (unsigned i = 0; i < num_rows; ++i) {
      const pkint_t* g = mat->p[i];
      guarded_write(out, g[0]);
      const pkint_t divisor = g[1];
      if (pkint_sgn(divisor) != 0)
	// `g' is a point.
	for (unsigned j = 0; j < space_dim; ++j) {
	  guarded_write(out, ' ');
	  if (pkint_sgn(g[j+2]) == 0)
	    guarded_write(out, '0');
	  else
	    guarded_write(out, mpq_class(mpz_class(g[j+2].rep),
					 mpz_class(divisor.rep)));
	}
      else
	// `g' is a ray or a line.
	for (unsigned j = 0; j < space_dim; ++j) {
	  guarded_write(out, ' ');
	  guarded_write(out, g[j+2]);
	}
      guarded_write(out, '\n');
    }
  }
#elif defined (USE_POLYLIB)
  if (rep == H) {
    for (unsigned i = 0; i < num_rows; ++i) {
      const Value* c = mat->p[i];
      // The inhomogeneous term.
      guarded_write(out, c[space_dim+1]);
      // The variables' coefficients.
      for (unsigned j = 0; j < space_dim; ++j) {
	guarded_write(out, ' ');
	guarded_write(out, c[j+1]);
      }
      guarded_write(out, '\n');
    }
  }
  else {
    assert(rep == V);
    for (unsigned i = 0; i < num_rows; ++i) {
      const Value* g = mat->p[i];
      guarded_write(out, g[0]);
      const Value& divisor = g[space_dim+1];
      if (value_sign(divisor) != 0)
	// `g' is a point.
	for (unsigned j = 0; j < space_dim; ++j) {
	  guarded_write(out, ' ');
	  if (value_sign(g[j+1]) == 0)
	    guarded_write(out, '0');
	  else
	    guarded_write(out, mpq_class(mpz_class(g[j+1]),
					 mpz_class(divisor)));
	}
      else
	// `g' is a ray or a line.
	for (unsigned j = 0; j < space_dim; ++j) {
	  guarded_write(out, ' ');
	  guarded_write(out, g[j+1]);
	}
      guarded_write(out, '\n');
    }
  }
#endif
  guarded_write(out, "end\n");

  // Flush `out'.
  bool flush_succeeded = false;
  try {
    out.flush();
    flush_succeeded = !out.fail();
  }
  catch (...) {
  }
  if (!flush_succeeded)
    fatal("cannot write to output file `%s'", output_file_name);
}

} // namespace

int
main(int argc, char* argv[]) try {
  program_name = argv[0];

#if defined(USE_PPL)
  if (strcmp(PPL_VERSION, PPL::version()) != 0)
    fatal("was compiled with PPL version %s, but linked with version %s",
	  PPL_VERSION, PPL::version());

  if (verbose)
    std::cerr << "Parma Polyhedra Library version:\n" << PPL::version()
	      << "\n\nParma Polyhedra Library banner:\n" << PPL::banner()
	      << std::endl;
#endif

  // Process command line options.
  process_options(argc, argv);

#ifdef PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME

  if (max_seconds_of_cpu_time > 0)
    set_alarm_on_cpu_time(max_seconds_of_cpu_time, &timeout);

#endif // defined(PPL_LCDD_SUPPORTS_LIMIT_ON_CPU_TIME)

  if (max_bytes_of_virtual_memory > 0)
    limit_virtual_memory(max_bytes_of_virtual_memory);

  // Set up the input and output streams.
  set_input(input_file_name);
  set_output(output_file_name);

  POLYHEDRON_TYPE ph;
  Representation rep = read_polyhedron(input(), ph);

  enum Command { None, H_to_V, V_to_H };
  Command command = None;

  // Warn for misplaced linearity commands, and ignore all what follows.
  std::string s;
  while (guarded_read(input(), s)) {
    if (s == "linearity" || s == "equality" || s == "partial_enum")
      error("the `linearity' command must occur before `begin'");
    input().ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }


#if defined(USE_PPL) || defined(USE_POLKA)
  maybe_start_clock();
#endif

  // Compute the dual representation.
  if (rep == V) {
    command = V_to_H;
#if defined(USE_PPL)
    ph.minimized_constraints();
#elif defined(USE_POLKA)
    poly_minimize(ph);
#endif
  }
  else {
    command = H_to_V;
#if defined(USE_PPL)
    ph.minimized_generators();
#elif defined(USE_POLKA)
    poly_minimize(ph);
#endif
  }

#if defined(USE_PPL) || defined(USE_POLKA)
  maybe_print_clock();
#endif

  // Write the result of the conversion.
  if (rep == V)
    write_polyhedron(output(), ph, H);
  else
    write_polyhedron(output(), ph, V);

#if defined(USE_PPL)
  // Check the result, if requested to do so.
  if (check_file_name) {
    set_input(check_file_name);
    // Read the polyhedron containing the expected result.
    PPL::C_Polyhedron e_ph;
    Representation e_rep = read_polyhedron(input(), e_ph);

    switch (command) {
    case H_to_V:
      {
	if (e_rep == H)
	  warning("checking an H-to-V conversion with an H representation");

	// Count the number of generators of `ph'.
	unsigned ph_num_generators = 0;
	const PPL::Generator_System& ph_gs = ph.generators();
	for (PPL::Generator_System::const_iterator i = ph_gs.begin(),
	       ph_gs_end = ph_gs.end(); i != ph_gs_end; ++i)
	  ++ph_num_generators;

	// Count the number of generators of `e_ph'.
	unsigned e_ph_num_generators = 0;
	const PPL::Generator_System& e_ph_gs = e_ph.generators();
	for (PPL::Generator_System::const_iterator i = e_ph_gs.begin(),
	       e_ph_gs_end = e_ph_gs.end(); i != e_ph_gs_end; ++i)
	  ++e_ph_num_generators;

	// If the polyhedra differ, that is the problem.
	if (ph != e_ph) {
	  if (verbose)
	    std::cerr << "Check failed: polyhedra differ"
		      << std::endl;
	  return 1;
	}
	else if (ph_num_generators != e_ph_num_generators)
	  // If we have different number of generators, we fail.
	  std::cerr << "Check failed: different number of generators:\n"
		    << "expected " << e_ph_num_generators
		    << ", obtained " << ph_num_generators
		    << std::endl;
	break;
      }
    case V_to_H:
      {
	if (e_rep == V)
	  warning("checking an V-to-H conversion with a V representation");

	// Count the number of constraints of `ph'.
	unsigned ph_num_constraints = 0;
	const PPL::Constraint_System& ph_cs = ph.constraints();
	for (PPL::Constraint_System::const_iterator i = ph_cs.begin(),
	       ph_cs_end = ph_cs.end(); i != ph_cs_end; ++i)
	  ++ph_num_constraints;

	// Count the number of constraints of `e_ph'.
	unsigned e_ph_num_constraints = 0;
	const PPL::Constraint_System& e_ph_cs = e_ph.constraints();
	for (PPL::Constraint_System::const_iterator i = e_ph_cs.begin(),
	       e_ph_cs_end = e_ph_cs.end(); i != e_ph_cs_end; ++i)
	  ++e_ph_num_constraints;

	// If the polyhedra differ, that is the problem.
	if (ph != e_ph) {
	  if (verbose)
	    std::cerr << "Check failed: polyhedra differ"
		      << std::endl;
	  return 1;
	}
	else if (ph_num_constraints != e_ph_num_constraints)
	  // If we have different number of constraints, we fail.
	  std::cerr << "Check failed: different number of constraints:\n"
		    << "expected " << e_ph_num_constraints
		    << ", obtained " << ph_num_constraints
		    << std::endl;
	break;
      }
    case None:
      break;
    }
  }
#endif

#if defined(USE_POLKA)
    // Finalize the library.
    polka_finalize();
#endif

  return 0;
}
catch (const std::bad_alloc&) {
  fatal("out of memory");
  exit(1);
}
catch (const std::overflow_error& e) {
  fatal("arithmetic overflow (%s)", e.what());
  exit(1);
}
catch (...) {
  fatal("internal error: please submit a bug report to ppl-devel@cs.unipr.it");
  exit(1);
}
