/* Solve linear programming problems by either vertex/point enumeration
   or the primal simplex algorithm.
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

#include <ppl-config.h>
#include "ppl_c.h"
#include <gmp.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#if defined(PPL_HAVE_GLPK_GLPK_H)
#include <glpk/glpk.h>
#elif defined(PPL_HAVE_GLPK_H)
#include <glpk.h>
#endif

#ifdef PPL_HAVE_GETOPT_H
# include <getopt.h>

/* Try to accommodate non-GNU implementations of `getopt()'. */
#if !defined(no_argument) && defined(NO_ARG)
#define no_argument NO_ARG
#endif

#if !defined(required_argument) && defined(REQUIRED_ARG)
#define required_argument REQUIRED_ARG
#endif

#if !defined(optional_argument) && defined(OPTIONAL_ARG)
#define optional_argument OPTIONAL_ARG
#endif

#endif /* defined(PPL_HAVE_GETOPT_H) */

#ifdef PPL_HAVE_UNISTD_H
/* Include this for `getopt()': especially important if we do not have
   <getopt.h>. */
# include <unistd.h>
#endif

#ifdef PPL_HAVE_SIGNAL_H
# include <signal.h>
#endif

#ifdef PPL_HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#ifdef PPL_HAVE_SYS_RESOURCE_H
/* This should be included after <time.h> and <sys/time.h> so as to make
   sure we have the definitions for, e.g., `ru_utime'. */
# include <sys/resource.h>
#endif

#if PPL_VERSION_MAJOR == 0 && PPL_VERSION_MINOR < 10
# error "PPL version 0.10 or following is required"
#endif

static const char* ppl_source_version = PPL_VERSION;

#ifdef __GNUC__
# define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#else
# define ATTRIBUTE_UNUSED
#endif

#if PPL_HAVE_DECL_GETRUSAGE
# define PPL_LPSOL_SUPPORTS_TIMINGS
#endif

#if defined(PPL_HAVE_SYS_RESOURCE_H) \
  && PPL_CXX_SUPPORTS_LIMITING_MEMORY \
  && (defined(SA_ONESHOT) || defined(SA_RESETHAND))
# define PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME
#endif

#ifdef PPL_HAVE_GETOPT_H
static struct option long_options[] = {
  {"check",           optional_argument, 0, 'c'},
  {"help",            no_argument,       0, 'h'},
  {"incremental",     no_argument,       0, 'i'},
  {"min",             no_argument,       0, 'm'},
  {"max",             no_argument,       0, 'M'},
  {"no-optimization", no_argument,       0, 'n'},
  {"no-mip",          no_argument,       0, 'r'},
  {"max-cpu",         required_argument, 0, 'C'},
  {"max-memory",      required_argument, 0, 'R'},
  {"output",          required_argument, 0, 'o'},
  {"pricing",         required_argument, 0, 'p'},
  {"enumerate",       no_argument,       0, 'e'},
  {"simplex",         no_argument,       0, 's'},
#ifdef PPL_LPSOL_SUPPORTS_TIMINGS
  {"timings",         no_argument,       0, 't'},
#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */
  {"verbosity",       required_argument, 0, 'v'},
  {"version",         no_argument,       0, 'V'},
  {0, 0, 0, 0}
};
#endif

#define USAGE_STRING0                                                   \
  "Usage: %s [OPTION]... [FILE]\n"                                      \
  "Reads a file in MPS format and attempts solution using the optimization\n" \
  "algorithms provided by the PPL.\n\n"                                 \
  "Options:\n"                                                          \
  "  -c, --check[=THRESHOLD] checks the obtained results using GLPK;\n" \
  "                          optima are checked with a tolerance of\n"  \
  "                          THRESHOLD (default %.10g);  input data\n"  \
  "                          are also perturbed the same way as GLPK does\n" \
  "  -i, --incremental       solves the problem incrementally\n"
#define USAGE_STRING1                                                   \
  "  -m, --min               minimizes the objective function\n"        \
  "  -M, --max               maximizes the objective function (default)\n" \
  "  -n, --no-optimization   checks for satisfiability only\n"          \
  "  -r, --no-mip            consider integer variables as real variables\n" \
  "  -CSECS, --max-cpu=SECS  limits CPU usage to SECS seconds\n"        \
  "  -RMB, --max-memory=MB   limits memory usage to MB megabytes\n"     \
  "  -h, --help              prints this help text to stdout\n"         \
  "  -oPATH, --output=PATH   appends output to PATH\n"
#define USAGE_STRING2                                                   \
  "  -e, --enumerate         use the (expensive!) enumeration method\n" \
  "  -pM, --pricing=M        use pricing method M for simplex (assumes -s);\n" \
  "                          M is an int from 0 to 2, default 0:\n"     \
  "                          0 --> steepest-edge using floating point\n" \
  "                          1 --> steepest-edge using exact arithmetic\n" \
  "                          2 --> textbook\n"                          \
  "  -s, --simplex           use the simplex method\n"
#ifdef PPL_LPSOL_SUPPORTS_TIMINGS
#define USAGE_STRING3                                                   \
  "  -t, --timings           prints timings to stderr\n"
#else /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */
#define USAGE_STRING3                                                   \
  ""
#endif /* !defined(PPL_LPSOL_SUPPORTS_TIMINGS) */
#define USAGE_STRING4                                                   \
  "  -v, --verbosity=LEVEL   sets verbosity level (from 0 to 4, default 3):\n" \
  "                          0 --> quiet: no output except for errors and\n" \
  "                                explicitly required notifications\n" \
  "                          1 --> solver state only\n"                 \
  "                          2 --> state + optimal value\n"             \
  "                          3 --> state + optimal value + optimum location\n" \
  "                          4 --> lots of output\n"                    \
  "  -V, --version           prints version information to stdout\n"
#ifndef PPL_HAVE_GETOPT_H
#define USAGE_STRING5                                                   \
  "\n"                                                                  \
  "NOTE: this version does not support long options.\n"
#else /* defined(PPL_HAVE_GETOPT_H) */
#define USAGE_STRING5                                                   \
  ""
#endif /* !defined(PPL_HAVE_GETOPT_H) */
#define USAGE_STRING6                                                   \
  "\n"                                                                  \
  "Report bugs to <ppl-devel@cs.unipr.it>.\n"


#define OPTION_LETTERS "bc::eimnMC:R:ho:p:rstVv:"

static const char* program_name = 0;
static unsigned long max_bytes_of_virtual_memory = 0;
static const char* output_argument = 0;
FILE* output_file = NULL;
static int check_results = 0;
static int use_simplex = 0;
static int pricing_method = 0;
static int verbosity = 3;
static int maximize = 1;
static int incremental = 0;
static int no_optimization = 0;
static int no_mip = 0;
static int check_results_failed = 0;
static double check_threshold = 0.0;
static const double default_check_threshold = 0.000000001;

#ifdef PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME
static unsigned long max_seconds_of_cpu_time = 0;
#endif /* defined (PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME) */

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS
static int print_timings = 0;
#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

static void
my_exit(int status) {
  (void) ppl_finalize();
  exit(status);
}

void
fatal(const char* format, ...) {
  va_list ap;
  fprintf(stderr, "%s: ", program_name);
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  my_exit(1);
}

#if 0
static void
warning(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "%s: warning: ", program_name);
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
#endif

static void
error(const char* format, ...) {
  va_list ap;
  fprintf(stderr, "%s: ", program_name);
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  if (output_argument) {
    va_start(ap, format);
    vfprintf(output_file, format, ap);
    va_end(ap);
    fprintf(output_file, "\n");
  }
}

static const char*
get_ppl_version() {
  const char* p;
  (void) ppl_version(&p);
  return p;
}

static const char*
get_ppl_banner() {
  const char* p;
  (void) ppl_banner(&p);
  return p;
}

static void
process_options(int argc, char* argv[]) {
#ifdef PPL_HAVE_GETOPT_H
  int option_index;
#endif
  int enumerate_required = 0;
  int simplex_required = 0;
  int incremental_required = 0;
  int no_optimization_required = 0;
  int no_mip_required = 0;
  int c;
  char* endptr;
  long l;
  double d;

  while (1) {
#ifdef PPL_HAVE_GETOPT_H
    option_index = 0;
    c = getopt_long(argc, argv, OPTION_LETTERS, long_options, &option_index);
#else
    c = getopt(argc, argv, OPTION_LETTERS);
#endif
    if (c == EOF)
      break;

    switch (c) {
    case 0:
      break;

    case 'c':
      check_results = 1;
      if (optarg) {
	d = strtod(optarg, &endptr);
	if (*endptr || errno == ERANGE || d < 0.0)
	  fatal("only a non-negative floating point number can `-c'");
	else
	  check_threshold = d;
      }
      else
	check_threshold = default_check_threshold;
      break;

    case 'm':
      maximize = 0;
      break;

    case 'M':
      maximize = 1;
      break;

    case '?':
    case 'h':
      fprintf(stdout, USAGE_STRING0, argv[0], default_check_threshold);
      fputs(USAGE_STRING1, stdout);
      fputs(USAGE_STRING2, stdout);
      fputs(USAGE_STRING3, stdout);
      fputs(USAGE_STRING4, stdout);
      my_exit(0);
      break;

#ifdef PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME

    case 'C':
      l = strtol(optarg, &endptr, 10);
      if (*endptr || l < 0)
	fatal("a non-negative integer must follow `-C'");
      else
	max_seconds_of_cpu_time = l;
      break;

#endif /* defined (PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME) */

    case 'R':
      l = strtol(optarg, &endptr, 10);
      if (*endptr || l < 0)
	fatal("a non-negative integer must follow `-R'");
      else if (((unsigned long) l) > ULONG_MAX/(1024*1024))
        max_bytes_of_virtual_memory = ULONG_MAX;
      else
	max_bytes_of_virtual_memory = l*1024*1024;
      break;

    case 'o':
      output_argument = optarg;
      break;

    case 'p':
      l = strtol(optarg, &endptr, 10);
      if (*endptr || l < 0 || l > 2)
	fatal("0 or 1 or 2 must follow `-p'");
      else
	pricing_method = l;
      break;

    case 'e':
      enumerate_required = 1;
      break;

    case 's':
      simplex_required = 1;
      break;

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

    case 't':
      print_timings = 1;
      break;

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

    case 'v':
      l = strtol(optarg, &endptr, 10);
      if (*endptr || l < 0 || l > 4)
	fatal("verbosity must be an integer between 0 and 4");
      else
	verbosity = l;
      break;

    case 'V':
      fprintf(stdout, "%s\n", PPL_VERSION);
      my_exit(0);
      break;

    case 'i':
      incremental_required = 1;
      break;

    case 'n':
      no_optimization_required = 1;
      break;

    case 'r':
      no_mip_required = 1;
      break;

    default:
      abort();
    }
  }

  if (enumerate_required
      && (simplex_required
	  || incremental_required))
      fatal("-e option is incompatible with -i and -s");

  if (enumerate_required)
    use_simplex = 0;
  else if (simplex_required)
    use_simplex = 1;

  if (incremental_required)
    incremental = 1;

  if (no_optimization_required)
    no_optimization = 1;

  if (no_mip_required)
    no_mip = 1;

  if (optind >= argc) {
    if (verbosity >= 4)
      fprintf(stderr,
	      "Parma Polyhedra Library version:\n%s\n\n"
	      "Parma Polyhedra Library banner:\n%s\n",
	      get_ppl_version(),
	      get_ppl_banner());
    else
      fatal("no input files");
  }

  if (argc - optind > 1)
    /* We have multiple input files. */
    fatal("only one input file is accepted");

  if (output_argument) {
    output_file = fopen(output_argument, "a");
    if (output_file == NULL)
      fatal("cannot open output file `%s'", output_argument);
  }
  else
    output_file = stdout;
}

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

/* To save the time when start_clock is called. */
static struct timeval saved_ru_utime;

static void
start_clock() {
  struct rusage rsg;
  if (getrusage(RUSAGE_SELF, &rsg) != 0)
    fatal("getrusage failed: %s", strerror(errno));
  else
    saved_ru_utime = rsg.ru_utime;
}

static void
print_clock(FILE* f) {
  struct rusage rsg;
  if (getrusage(RUSAGE_SELF, &rsg) != 0)
    fatal("getrusage failed: %s", strerror(errno));
  else {
    time_t current_secs = rsg.ru_utime.tv_sec;
    time_t current_usecs = rsg.ru_utime.tv_usec;
    time_t saved_secs = saved_ru_utime.tv_sec;
    time_t saved_usecs = saved_ru_utime.tv_usec;
    int secs;
    int csecs;
    if (current_usecs < saved_usecs) {
      csecs = (((1000000 + current_usecs) - saved_usecs) + 5000) / 10000;
      secs = (current_secs - saved_secs) -1;
    }
    else {
      csecs = ((current_usecs - saved_usecs) + 5000) / 10000;
      secs = current_secs - saved_secs;
    }
    assert(csecs >= 0 && csecs < 100 && secs >= 0);
    fprintf(f, "%d.%.2d", secs, csecs);
  }
}

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

#ifdef PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME

void
set_alarm_on_cpu_time(unsigned seconds, void (*handler)(int)) {
  sigset_t mask;
  struct sigaction s;
  struct rlimit t;

  sigemptyset(&mask);

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

  if (getrlimit(RLIMIT_CPU, &t) != 0)
    fatal("getrlimit failed: %s", strerror(errno));

  if (seconds < t.rlim_cur) {
    t.rlim_cur = seconds;
    if (setrlimit(RLIMIT_CPU, &t) != 0)
      fatal("setrlimit failed: %s", strerror(errno));
  }
}

#endif /* defined(PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME) */

#if PPL_CXX_SUPPORTS_LIMITING_MEMORY && PPL_HAVE_DECL_RLIMIT_AS

void
limit_virtual_memory(unsigned long bytes) {
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
limit_virtual_memory(unsigned long bytes ATTRIBUTE_UNUSED) {
}

#endif /* !PPL_HAVE_DECL_RLIMIT_AS */

#ifdef PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME

static void
my_timeout(int dummy ATTRIBUTE_UNUSED) {
  fprintf(stderr, "TIMEOUT\n");
  if (output_argument)
    fprintf(output_file, "TIMEOUT\n");
  my_exit(0);
}

#endif /* defined(PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME) */

static mpz_t tmp_z;
static mpq_t tmp1_q;
static mpq_t tmp2_q;
static ppl_Coefficient_t ppl_coeff;
static LPX* glpk_lp;
static int glpk_lp_problem_kind;
static int glpk_lp_num_int;
static ppl_dimension_type* integer_variables;

static void
maybe_check_results(const int ppl_status, const double ppl_optimum_value) {
  const char* ppl_status_string;
  const char* glpk_status_string;
  int glpk_status;
  int treat_as_lp = 0;
  if (!check_results)
    return;

  /* Disable GLPK output. */
  lpx_set_int_parm(glpk_lp, LPX_K_MSGLEV, 0);

  if (no_mip || glpk_lp_problem_kind == LPX_LP)
    treat_as_lp = 1;

  lpx_set_obj_dir(glpk_lp, (maximize ? LPX_MAX : LPX_MIN));

  if (treat_as_lp) {
    /* Set the problem class to LP: MIP problems are thus treated as
       LP ones. */
    lpx_set_class(glpk_lp, LPX_LP);
    lpx_exact(glpk_lp);
    glpk_status = lpx_get_status(glpk_lp);
  }
  else {
    /* MIP case. */
    lpx_intopt(glpk_lp);
    glpk_status = lpx_mip_status(glpk_lp);
  }
  /* If no_optimization is enabled, the second case is not possibile. */
  if (!((ppl_status == PPL_MIP_PROBLEM_STATUS_UNFEASIBLE
	 && (glpk_status == LPX_NOFEAS || glpk_status == LPX_I_NOFEAS))
	|| (ppl_status == PPL_MIP_PROBLEM_STATUS_UNBOUNDED
	    && (glpk_status == LPX_UNBND || glpk_status == LPX_I_UNDEF))
	|| (ppl_status == PPL_MIP_PROBLEM_STATUS_OPTIMIZED
	    && ((glpk_status == LPX_OPT || glpk_status == LPX_I_OPT)
		/* If no_optimization is enabled, check if the problem is
		   unbounded for GLPK.  */
		|| (no_optimization && (glpk_status == LPX_UNBND
					|| glpk_status == LPX_I_UNDEF))))))  {

    if (ppl_status == PPL_MIP_PROBLEM_STATUS_UNFEASIBLE)
      ppl_status_string = "unfeasible";
    else if (ppl_status == PPL_MIP_PROBLEM_STATUS_UNBOUNDED)
      ppl_status_string = "unbounded";
    else if (ppl_status == PPL_MIP_PROBLEM_STATUS_OPTIMIZED)
      ppl_status_string = "optimizable";
    else
      ppl_status_string = "<?>";

    switch (glpk_status) {
    case LPX_NOFEAS:
      glpk_status_string = "unfeasible";
      break;
    case LPX_UNBND:
      glpk_status_string = "unbounded";
      break;
    case LPX_OPT:
      glpk_status_string = "optimizable";
      break;
    case LPX_I_NOFEAS:
      glpk_status_string = "unfeasible";
      break;
    case LPX_I_OPT:
      glpk_status_string = "optimizable";
      break;
    case LPX_I_UNDEF:
      glpk_status_string = "undefined";
      break;
    default:
      glpk_status_string = "<?>";
      break;
    }

    error("check failed: for GLPK the problem is %s, not %s",
	  glpk_status_string, ppl_status_string);

    check_results_failed = 1;
  }
  else if (!no_optimization
	   && ppl_status == PPL_MIP_PROBLEM_STATUS_OPTIMIZED) {

    double glpk_optimum_value = treat_as_lp ? lpx_get_obj_val(glpk_lp)
      : lpx_mip_obj_val(glpk_lp);

    if (fabs(ppl_optimum_value - glpk_optimum_value) > check_threshold) {
      error("check failed: for GLPK the problem's optimum is %.20g,"
	    " not %.20g", glpk_optimum_value, ppl_optimum_value);
      check_results_failed = 1;
    }
  }
  return;
}


static const char*
variable_output_function(ppl_dimension_type var) {
  const char* name = lpx_get_col_name(glpk_lp, var+1);
  if (name != NULL)
    return name;
  else
    return 0;
}

static void
add_constraints(ppl_Linear_Expression_t ppl_le,
		int type, mpq_t rational_lb, mpq_t rational_ub, mpz_t den_lcm,
		ppl_Constraint_System_t ppl_cs) {
  ppl_Constraint_t ppl_c;
  ppl_Linear_Expression_t ppl_le2;
  switch (type) {
  case LPX_FR:
    break;

  case LPX_LO:
    mpz_mul(tmp_z, den_lcm, mpq_numref(rational_lb));
    mpz_divexact(tmp_z, tmp_z, mpq_denref(rational_lb));
    mpz_neg(tmp_z, tmp_z);
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
    ppl_Linear_Expression_add_to_inhomogeneous(ppl_le, ppl_coeff);
    ppl_new_Constraint(&ppl_c, ppl_le, PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL);
    if (verbosity >= 4) {
      ppl_io_fprint_Constraint(output_file, ppl_c);
      fprintf(output_file, "\n");
    }
    ppl_Constraint_System_insert_Constraint(ppl_cs, ppl_c);
    ppl_delete_Constraint(ppl_c);
    break;

  case LPX_UP:
    mpz_mul(tmp_z, den_lcm, mpq_numref(rational_ub));
    mpz_divexact(tmp_z, tmp_z, mpq_denref(rational_ub));
    mpz_neg(tmp_z, tmp_z);
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
    ppl_Linear_Expression_add_to_inhomogeneous(ppl_le, ppl_coeff);
    ppl_new_Constraint(&ppl_c, ppl_le,
		       PPL_CONSTRAINT_TYPE_LESS_OR_EQUAL);
    if (verbosity >= 4) {
      ppl_io_fprint_Constraint(output_file, ppl_c);
      fprintf(output_file, "\n");
    }
    ppl_Constraint_System_insert_Constraint(ppl_cs, ppl_c);
    ppl_delete_Constraint(ppl_c);
    break;

  case LPX_DB:
    ppl_new_Linear_Expression_from_Linear_Expression(&ppl_le2, ppl_le);

    mpz_mul(tmp_z, den_lcm, mpq_numref(rational_lb));
    mpz_divexact(tmp_z, tmp_z, mpq_denref(rational_lb));
    mpz_neg(tmp_z, tmp_z);
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
    ppl_Linear_Expression_add_to_inhomogeneous(ppl_le, ppl_coeff);
    ppl_new_Constraint(&ppl_c, ppl_le, PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL);
    if (verbosity >= 4) {
      ppl_io_fprint_Constraint(output_file, ppl_c);
      fprintf(output_file, "\n");
    }
    ppl_Constraint_System_insert_Constraint(ppl_cs, ppl_c);
    ppl_delete_Constraint(ppl_c);

    mpz_mul(tmp_z, den_lcm, mpq_numref(rational_ub));
    mpz_divexact(tmp_z, tmp_z, mpq_denref(rational_ub));
    mpz_neg(tmp_z, tmp_z);
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
    ppl_Linear_Expression_add_to_inhomogeneous(ppl_le2, ppl_coeff);
    ppl_new_Constraint(&ppl_c, ppl_le2, PPL_CONSTRAINT_TYPE_LESS_OR_EQUAL);
    ppl_delete_Linear_Expression(ppl_le2);
    if (verbosity >= 4) {
      ppl_io_fprint_Constraint(output_file, ppl_c);
      fprintf(output_file, "\n");
    }
    ppl_Constraint_System_insert_Constraint(ppl_cs, ppl_c);
    ppl_delete_Constraint(ppl_c);
    break;

  case LPX_FX:
    mpz_mul(tmp_z, den_lcm, mpq_numref(rational_lb));
    mpz_divexact(tmp_z, tmp_z, mpq_denref(rational_lb));
    mpz_neg(tmp_z, tmp_z);
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
    ppl_Linear_Expression_add_to_inhomogeneous(ppl_le, ppl_coeff);
    ppl_new_Constraint(&ppl_c, ppl_le,
		       PPL_CONSTRAINT_TYPE_EQUAL);
    if (verbosity >= 4) {
      ppl_io_fprint_Constraint(output_file, ppl_c);
      fprintf(output_file, "\n");
    }
    ppl_Constraint_System_insert_Constraint(ppl_cs, ppl_c);
    ppl_delete_Constraint(ppl_c);
    break;

  default:
    fatal("internal error");
    break;
  }
}

static int
solve_with_generators(ppl_Constraint_System_t ppl_cs,
		      ppl_const_Linear_Expression_t ppl_objective_le,
		      ppl_Coefficient_t optimum_n,
		      ppl_Coefficient_t optimum_d,
		      ppl_Generator_t point) {
  ppl_Polyhedron_t ppl_ph;
  int optimum_found = 0;
  int empty;
  int unbounded;
  int included;

  /* Create the polyhedron (recycling the data structures of ppl_cs). */
  ppl_new_C_Polyhedron_recycle_Constraint_System(&ppl_ph, ppl_cs);

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings) {
    fprintf(stderr, "Time to create a PPL polyhedron: ");
    print_clock(stderr);
    fprintf(stderr, " s\n");
    start_clock();
  }

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  empty = ppl_Polyhedron_is_empty(ppl_ph);

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings) {
    fprintf(stderr, "Time to check for emptiness: ");
    print_clock(stderr);
    fprintf(stderr, " s\n");
    start_clock();
  }

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  if (empty) {
    if (verbosity >= 1)
      fprintf(output_file, "Unfeasible problem.\n");
    maybe_check_results(PPL_MIP_PROBLEM_STATUS_UNFEASIBLE, 0.0);
    goto exit;
  }

  if (!empty && no_optimization) {
    if (verbosity >= 1)
      fprintf(output_file, "Feasible problem.\n");
    /* Kludge: let's pass PPL_MIP_PROBLEM_STATUS_OPTIMIZED,
       to let work `maybe_check_results'. */
    maybe_check_results(PPL_MIP_PROBLEM_STATUS_OPTIMIZED, 0.0);
    goto exit;
  }

  /* Check whether the problem is unbounded. */
  unbounded = maximize
    ? !ppl_Polyhedron_bounds_from_above(ppl_ph, ppl_objective_le)
    : !ppl_Polyhedron_bounds_from_below(ppl_ph, ppl_objective_le);

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings) {
    fprintf(stderr, "Time to check for unboundedness: ");
    print_clock(stderr);
    fprintf(stderr, " s\n");
    start_clock();
  }

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  if (unbounded) {
    if (verbosity >= 1)
      fprintf(output_file, "Unbounded problem.\n");
    maybe_check_results(PPL_MIP_PROBLEM_STATUS_UNBOUNDED, 0.0);
    goto exit;
  }

  optimum_found = maximize
    ? ppl_Polyhedron_maximize_with_point(ppl_ph, ppl_objective_le,
                                         optimum_n, optimum_d, &included,
                                         point)
    : ppl_Polyhedron_minimize_with_point(ppl_ph, ppl_objective_le,
                                         optimum_n, optimum_d, &included,
                                         point);

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings) {
    fprintf(stderr, "Time to find the optimum: ");
    print_clock(stderr);
    fprintf(stderr, " s\n");
    start_clock();
  }

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  if (!optimum_found)
    fatal("internal error");

  if (!included)
    fatal("internal error");

 exit:
  ppl_delete_Polyhedron(ppl_ph);
  return optimum_found;
}

static int
solve_with_simplex(ppl_const_Constraint_System_t cs,
		   ppl_const_Linear_Expression_t objective,
		   ppl_Coefficient_t optimum_n,
		   ppl_Coefficient_t optimum_d,
		   ppl_Generator_t point) {
  ppl_MIP_Problem_t ppl_mip;
  int optimum_found = 0;
  int pricing = 0;
  int status = 0;
  int satisfiable = 0;
  ppl_dimension_type space_dim;
  ppl_const_Constraint_t c;
  ppl_const_Generator_t g;
  ppl_Constraint_System_const_iterator_t i;
  ppl_Constraint_System_const_iterator_t iend;
  int counter;
  int mode = maximize
    ? PPL_OPTIMIZATION_MODE_MAXIMIZATION
    : PPL_OPTIMIZATION_MODE_MINIMIZATION;

  ppl_Constraint_System_space_dimension(cs, &space_dim);
  ppl_new_MIP_Problem_from_space_dimension(&ppl_mip, space_dim);
  switch (pricing_method) {
  case 0:
    pricing = PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_FLOAT;
    break;
  case 1:
    pricing = PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_EXACT;
    break;
  case 2:
    pricing = PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_TEXTBOOK;
    break;
  default:
    fatal("ppl_lpsol internal error");
  }
  ppl_MIP_Problem_set_control_parameter(ppl_mip, pricing);
  ppl_MIP_Problem_set_objective_function(ppl_mip, objective);
  ppl_MIP_Problem_set_optimization_mode(ppl_mip, mode);
  if (!no_mip)
    ppl_MIP_Problem_add_to_integer_space_dimensions(ppl_mip, integer_variables,
						    glpk_lp_num_int);
  if (incremental) {
    /* Add the constraints of `cs' one at a time. */
    ppl_new_Constraint_System_const_iterator(&i);
    ppl_new_Constraint_System_const_iterator(&iend);
    ppl_Constraint_System_begin(cs, i);
    ppl_Constraint_System_end(cs, iend);

    counter = 0;
    while (!ppl_Constraint_System_const_iterator_equal_test(i, iend)) {
      ++counter;
      if (verbosity >= 4)
	fprintf(output_file, "\nSolving constraint %d\n", counter);
      ppl_Constraint_System_const_iterator_dereference(i, &c);
      ppl_MIP_Problem_add_constraint(ppl_mip, c);

      if (no_optimization) {
	satisfiable = ppl_MIP_Problem_is_satisfiable(ppl_mip);
	if (!satisfiable)
	  break;
      }
      else
	status = ppl_MIP_Problem_solve(ppl_mip);
      ppl_Constraint_System_const_iterator_increment(i);
    }
    ppl_delete_Constraint_System_const_iterator(i);
    ppl_delete_Constraint_System_const_iterator(iend);
  }

  else {
    ppl_MIP_Problem_add_constraints(ppl_mip, cs);
    if (no_optimization)
      satisfiable = ppl_MIP_Problem_is_satisfiable(ppl_mip);
    else
      status = ppl_MIP_Problem_solve(ppl_mip);
  }

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings) {
    fprintf(stderr, "Time to solve the problem: ");
    print_clock(stderr);
    fprintf(stderr, " s\n");
    start_clock();
  }

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  if ((no_optimization && !satisfiable)
      || (!no_optimization && status == PPL_MIP_PROBLEM_STATUS_UNFEASIBLE)) {
    if (verbosity >= 1)
      fprintf(output_file, "Unfeasible problem.\n");
    maybe_check_results(status, 0.0);
    goto exit;
  }
  else if (no_optimization && satisfiable) {
    if (verbosity >= 1)
      fprintf(output_file, "Feasible problem.\n");
    /* Kludge: let's pass PPL_MIP_PROBLEM_STATUS_OPTIMIZED,
       to let work `maybe_check_results'. */
    maybe_check_results(PPL_MIP_PROBLEM_STATUS_OPTIMIZED, 0.0);
    goto exit;
  }
  else if (status == PPL_MIP_PROBLEM_STATUS_UNBOUNDED) {
    if (verbosity >= 1)
      fprintf(output_file, "Unbounded problem.\n");
    maybe_check_results(status, 0.0);
    goto exit;
  }
  else if (status == PPL_MIP_PROBLEM_STATUS_OPTIMIZED) {
    ppl_MIP_Problem_optimal_value(ppl_mip, optimum_n, optimum_d);
    ppl_MIP_Problem_optimizing_point(ppl_mip, &g);
    ppl_assign_Generator_from_Generator(point, g);
    optimum_found = 1;
    goto exit;
  }
  else
    fatal("internal error");

 exit:
  ppl_delete_MIP_Problem(ppl_mip);
  return optimum_found;
}

extern void set_d_eps(mpq_t x, double val);

static void
set_mpq_t_from_double(mpq_t q, double d) {
  if (check_results)
    set_d_eps(q, d);
  else
    mpq_set_d(q, d);
}

static void
solve(char* file_name) {
  ppl_Constraint_System_t ppl_cs;
#ifndef NDEBUG
  ppl_Constraint_System_t ppl_cs_copy;
#endif
  ppl_Generator_t optimum_location;
  ppl_Linear_Expression_t ppl_le;
  int dimension, row, num_rows, column, nz, i, j, type;
  int* coefficient_index;
  double lb, ub;
  double* coefficient_value;
  mpq_t rational_lb, rational_ub;
  mpq_t* rational_coefficient;
  mpq_t* objective;
  ppl_Linear_Expression_t ppl_objective_le;
  ppl_Coefficient_t optimum_n;
  ppl_Coefficient_t optimum_d;
  mpq_t optimum;
  mpz_t den_lcm;
  int optimum_found;

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings)
    start_clock();

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  if (verbosity == 0) {
    /* FIXME: find a way to suppress output from lpx_read_mps. */
  }

  glpk_lp = lpx_read_mps(file_name);

  if (glpk_lp == NULL)
    fatal("cannot read MPS file `%s'", file_name);

#ifdef PPL_LPSOL_SUPPORTS_TIMINGS

  if (print_timings) {
    fprintf(stderr, "Time to read the input file: ");
    print_clock(stderr);
    fprintf(stderr, " s\n");
    start_clock();
  }

#endif /* defined(PPL_LPSOL_SUPPORTS_TIMINGS) */

  glpk_lp_problem_kind = lpx_get_class(glpk_lp);
  if (glpk_lp_problem_kind == LPX_MIP && !no_mip && !use_simplex)
     fatal("the enumeration solving method can not handle MIP problems");

  dimension = lpx_get_num_cols(glpk_lp);

  /* Read variables constrained to be integer. */
    if (glpk_lp_problem_kind == LPX_MIP && !no_mip && use_simplex) {
      if (verbosity >= 4)
	fprintf(output_file, "Integer variables:\n");
      glpk_lp_num_int = lpx_get_num_int(glpk_lp);
      integer_variables = (ppl_dimension_type*)
	malloc((glpk_lp_num_int + 1)*sizeof(ppl_dimension_type));
      for (i = 0, j = 0; i < dimension; ++i)
	if (lpx_get_col_kind(glpk_lp, i+1) == LPX_IV) {
	  integer_variables[j] = i;
	  if (verbosity >= 4) {
	    ppl_io_fprint_variable(output_file, i);
	    fprintf(output_file, " ");
	  }
	  ++j;
	}
    }
  coefficient_index = (int*) malloc((dimension+1)*sizeof(int));
  coefficient_value = (double*) malloc((dimension+1)*sizeof(double));
  rational_coefficient = (mpq_t*) malloc((dimension+1)*sizeof(mpq_t));


  ppl_new_Constraint_System(&ppl_cs);

  mpq_init(rational_lb);
  mpq_init(rational_ub);
  for (i = 1; i <= dimension; ++i)
    mpq_init(rational_coefficient[i]);

  mpz_init(den_lcm);

  if (verbosity >= 4)
    fprintf(output_file, "\nConstraints:\n");

  /* Set up the row (ordinary) constraints. */
  num_rows = lpx_get_num_rows(glpk_lp);
  for (row = 1; row <= num_rows; ++row) {
    /* Initialize the least common multiple computation. */
    mpz_set_si(den_lcm, 1);
    /* Set `nz' to the number of non-zero coefficients. */
    nz = lpx_get_mat_row(glpk_lp, row, coefficient_index, coefficient_value);
    for (i = 1; i <= nz; ++i) {
      set_mpq_t_from_double(rational_coefficient[i], coefficient_value[i]);
      /* Update den_lcm. */
      mpz_lcm(den_lcm, den_lcm, mpq_denref(rational_coefficient[i]));
    }
    lpx_get_row_bnds(glpk_lp, row, &type, &lb, &ub);
    set_mpq_t_from_double(rational_lb, lb);
    mpz_lcm(den_lcm, den_lcm, mpq_denref(rational_lb));
    set_mpq_t_from_double(rational_ub, ub);
    mpz_lcm(den_lcm, den_lcm, mpq_denref(rational_ub));

    ppl_new_Linear_Expression_with_dimension(&ppl_le, dimension);

    for (i = 1; i <= nz; ++i) {
      mpz_mul(tmp_z, den_lcm, mpq_numref(rational_coefficient[i]));
      mpz_divexact(tmp_z, tmp_z, mpq_denref(rational_coefficient[i]));
      ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
      ppl_Linear_Expression_add_to_coefficient(ppl_le, coefficient_index[i]-1,
					       ppl_coeff);
    }

    add_constraints(ppl_le, type, rational_lb, rational_ub, den_lcm, ppl_cs);

    ppl_delete_Linear_Expression(ppl_le);
  }

  free(coefficient_value);
  for (i = 1; i <= dimension; ++i)
    mpq_clear(rational_coefficient[i]);
  free(rational_coefficient);
  free(coefficient_index);

#ifndef NDEBUG
  ppl_new_Constraint_System_from_Constraint_System(&ppl_cs_copy, ppl_cs);
#endif

  /*
    FIXME: here we could build the polyhedron and minimize it before
    adding the variable bounds.
  */

  /* Set up the columns constraints, i.e., variable bounds. */
  for (column = 1; column <= dimension; ++column) {
    lpx_get_col_bnds(glpk_lp, column, &type, &lb, &ub);

    set_mpq_t_from_double(rational_lb, lb);
    set_mpq_t_from_double(rational_ub, ub);

    /* Initialize the least common multiple computation. */
    mpz_set_si(den_lcm, 1);
    mpz_lcm(den_lcm, den_lcm, mpq_denref(rational_lb));
    mpz_lcm(den_lcm, den_lcm, mpq_denref(rational_ub));

    ppl_new_Linear_Expression_with_dimension(&ppl_le, dimension);
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, den_lcm);
    ppl_Linear_Expression_add_to_coefficient(ppl_le, column-1, ppl_coeff);

    add_constraints(ppl_le, type, rational_lb, rational_ub, den_lcm, ppl_cs);

    ppl_delete_Linear_Expression(ppl_le);
  }

  mpq_clear(rational_ub);
  mpq_clear(rational_lb);

  /* Deal with the objective function. */
  objective = (mpq_t*) malloc((dimension+1)*sizeof(mpq_t));

  /* Initialize the least common multiple computation. */
  mpz_set_si(den_lcm, 1);

  mpq_init(objective[0]);
  set_mpq_t_from_double(objective[0], lpx_get_obj_coef(glpk_lp, 0));
  for (i = 1; i <= dimension; ++i) {
    mpq_init(objective[i]);
    set_mpq_t_from_double(objective[i], lpx_get_obj_coef(glpk_lp, i));
    /* Update den_lcm. */
    mpz_lcm(den_lcm, den_lcm, mpq_denref(objective[i]));
  }

  /* Set the ppl_objective_le to be the objective function. */
  ppl_new_Linear_Expression_with_dimension(&ppl_objective_le, dimension);
  /* Set value for objective function's inhomogeneous term. */
  mpz_mul(tmp_z, den_lcm, mpq_numref(objective[0]));
  mpz_divexact(tmp_z, tmp_z, mpq_denref(objective[0]));
  ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
  ppl_Linear_Expression_add_to_inhomogeneous(ppl_objective_le, ppl_coeff);
  /* Set values for objective function's variable coefficients. */
  for (i = 1; i <= dimension; ++i) {
    mpz_mul(tmp_z, den_lcm, mpq_numref(objective[i]));
    mpz_divexact(tmp_z, tmp_z, mpq_denref(objective[i]));
    ppl_assign_Coefficient_from_mpz_t(ppl_coeff, tmp_z);
    ppl_Linear_Expression_add_to_coefficient(ppl_objective_le, i-1, ppl_coeff);
  }

  if (verbosity >= 4) {
    fprintf(output_file, "Objective function:\n");
    if (mpz_cmp_si(den_lcm, 1) != 0)
      fprintf(output_file, "(");
    ppl_io_fprint_Linear_Expression(output_file, ppl_objective_le);
  }

  for (i = 0; i <= dimension; ++i)
    mpq_clear(objective[i]);
  free(objective);

  if (verbosity >= 4) {
    if (mpz_cmp_si(den_lcm, 1) != 0) {
      fprintf(output_file, ")/");
      mpz_out_str(output_file, 10, den_lcm);
    }
    fprintf(output_file, "\n%s\n",
	    (maximize ? "Maximizing." : "Minimizing."));
  }

  ppl_new_Coefficient(&optimum_n);
  ppl_new_Coefficient(&optimum_d);
  ppl_new_Generator_zero_dim_point(&optimum_location);

  optimum_found = use_simplex
    ? solve_with_simplex(ppl_cs,
			 ppl_objective_le,
			 optimum_n,
			 optimum_d,
			 optimum_location)
    : solve_with_generators(ppl_cs,
			    ppl_objective_le,
			    optimum_n,
			    optimum_d,
			    optimum_location);

  ppl_delete_Linear_Expression(ppl_objective_le);

  if (glpk_lp_problem_kind == LPX_MIP)
      free(integer_variables);

  if (optimum_found) {
    mpq_init(optimum);
    ppl_Coefficient_to_mpz_t(optimum_n, tmp_z);
    mpq_set_num(optimum, tmp_z);
    ppl_Coefficient_to_mpz_t(optimum_d, tmp_z);
    mpz_mul(tmp_z, tmp_z, den_lcm);
    mpq_set_den(optimum, tmp_z);
    if (verbosity == 1)
      fprintf(output_file, "Optimized problem.\n");
    if (verbosity >= 2)
      fprintf(output_file, "Optimum value: %.10g\n", mpq_get_d(optimum));
    if (verbosity >= 3) {
      fprintf(output_file, "Optimum location:\n");
      ppl_Generator_divisor(optimum_location, ppl_coeff);
      ppl_Coefficient_to_mpz_t(ppl_coeff, tmp_z);
      for (i = 0; i < dimension; ++i) {
	mpz_set(mpq_denref(tmp1_q), tmp_z);
	ppl_Generator_coefficient(optimum_location, i, ppl_coeff);
	ppl_Coefficient_to_mpz_t(ppl_coeff, mpq_numref(tmp1_q));
	ppl_io_fprint_variable(output_file, i);
	fprintf(output_file, " = %.10g\n", mpq_get_d(tmp1_q));
      }
    }
#ifndef NDEBUG
    {
      ppl_Polyhedron_t ph;
      unsigned int relation;
      ppl_new_C_Polyhedron_recycle_Constraint_System(&ph, ppl_cs_copy);
      ppl_delete_Constraint_System(ppl_cs_copy);
      relation = ppl_Polyhedron_relation_with_Generator(ph, optimum_location);
      ppl_delete_Polyhedron(ph);
      assert(relation == PPL_POLY_GEN_RELATION_SUBSUMES);
    }
#endif
    maybe_check_results(PPL_MIP_PROBLEM_STATUS_OPTIMIZED,
			mpq_get_d(optimum));
    mpq_clear(optimum);
  }

  ppl_delete_Constraint_System(ppl_cs);
  ppl_delete_Coefficient(optimum_d);
  ppl_delete_Coefficient(optimum_n);
  ppl_delete_Generator(optimum_location);

  lpx_delete_prob(glpk_lp);
}

static void
error_handler(enum ppl_enum_error_code code,
	      const char* description) {
  if (output_argument)
    fprintf(output_file, "PPL error code %d: %s\n", code, description);
  fatal("PPL error code %d: %s", code, description);
}

#if !PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK
void
ppl_set_GMP_memory_allocation_functions(void) {
}
#endif

#if defined(NDEBUG)

#if !(defined(PPL_GLPK_HAS_GLP_TERM_OUT) && defined(GLP_OFF))

#if defined(PPL_GLPK_HAS_GLP_TERM_HOOK) \
  || defined(PPL_GLPK_HAS__GLP_LIB_PRINT_HOOK)

static int
glpk_message_interceptor(void* info, const char* msg) {
  (void) info;
  (void) msg;
  return 1;
}

#elif defined(PPL_GLPK_HAS_LIB_SET_PRINT_HOOK)

static int
glpk_message_interceptor(void* info, char* msg) {
  (void) info;
  (void) msg;
  return 1;
}

#endif /* !(defined(PPL_GLPK_HAS_GLP_TERM_HOOK)
            || defined(PPL_GLPK_HAS__GLP_LIB_PRINT_HOOK))
          && defined(PPL_GLPK_HAS_LIB_SET_PRINT_HOOK) */

#endif /* !(defined(PPL_GLPK_HAS_GLP_TERM_OUT) && defined(GLP_OFF)) */

#endif /* defined(NDEBUG) */

int
main(int argc, char* argv[]) {
#if defined(PPL_GLPK_HAS__GLP_LIB_PRINT_HOOK)
  extern void _glp_lib_print_hook(int (*func)(void *info, const char *buf),
				  void *info);
#endif
  program_name = argv[0];
  if (ppl_initialize() < 0)
    fatal("cannot initialize the Parma Polyhedra Library");

  /* The PPL solver does not use floating point numbers, except
     perhaps for the steepest edge heuristics.  In contrast, GLPK does
     use them, so it is best to restore the rounding mode as it was
     prior to the PPL initialization.  */
  if (ppl_restore_pre_PPL_rounding() < 0)
    fatal("cannot restore the rounding mode");

  if (ppl_set_error_handler(error_handler) < 0)
    fatal("cannot install the custom error handler");

  if (strcmp(ppl_source_version, get_ppl_version()) != 0)
    fatal("was compiled with PPL version %s, but linked with version %s",
	  ppl_source_version, get_ppl_version());

  if (ppl_io_set_variable_output_function(variable_output_function) < 0)
    fatal("cannot install the custom variable output function");

#if defined(NDEBUG)
#if defined(PPL_GLPK_HAS_GLP_TERM_OUT) && defined(GLP_OFF)
  glp_term_out(GLP_OFF);
#elif defined(PPL_GLPK_HAS_GLP_TERM_HOOK)
  glp_term_hook(glpk_message_interceptor, 0);
#elif defined(PPL_GLPK_HAS__GLP_LIB_PRINT_HOOK)
  _glp_lib_print_hook(glpk_message_interceptor, 0);
#elif defined(PPL_GLPK_HAS_LIB_SET_PRINT_HOOK)
  lib_set_print_hook(0, glpk_message_interceptor);
#endif
#endif

  /* Process command line options. */
  process_options(argc, argv);

  /* Initialize globals. */
  mpz_init(tmp_z);
  mpq_init(tmp1_q);
  mpq_init(tmp2_q);
  ppl_new_Coefficient(&ppl_coeff);

#ifdef PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME

  if (max_seconds_of_cpu_time > 0)
    set_alarm_on_cpu_time(max_seconds_of_cpu_time, my_timeout);

#endif /* defined (PPL_LPSOL_SUPPORTS_LIMIT_ON_CPU_TIME) */

  if (max_bytes_of_virtual_memory > 0)
    limit_virtual_memory(max_bytes_of_virtual_memory);

  while (optind < argc) {
    if (check_results)
      check_results_failed = 0;

    solve(argv[optind++]);

    if (check_results && check_results_failed)
      break;
  }

  /* Finalize globals. */
  ppl_delete_Coefficient(ppl_coeff);
  mpq_clear(tmp2_q);
  mpq_clear(tmp1_q);
  mpz_clear(tmp_z);

  /* Close output file, if any. */
  if (output_argument)
    fclose(output_file);

  my_exit((check_results && check_results_failed) ? 1 : 0);

  /* This is just to avoid a compiler warning. */
  return 0;
}
