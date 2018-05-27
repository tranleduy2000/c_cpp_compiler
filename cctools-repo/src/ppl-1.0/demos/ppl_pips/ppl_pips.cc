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

//#define USE_PIPLIB 1

#if (!defined(USE_PPL) && !defined(USE_PIPLIB))
#define USE_PPL 1
#elif (defined(USE_PPL) && defined(USE_PIPLIB))
#error "Exactly one among USE_PPL and USE_PIPLIB must be defined"
#endif

#if defined(USE_PPL)

#include "ppl.hh"

namespace PPL = Parma_Polyhedra_Library;

#if PPL_VERSION_MAJOR == 0 && PPL_VERSION_MINOR < 11
#error "PPL version 0.11 or following is required"
#endif

typedef PPL::C_Polyhedron POLYHEDRON_TYPE;

#if !PPL_CXX_SUPPORTS_ATTRIBUTE_WEAK
extern "C" void
ppl_set_GMP_memory_allocation_functions(void) {
}
#endif

#elif defined(USE_PIPLIB)

#error "PipLib not supported yet"

#endif

#include "timings.hh"
#include <gmpxx.h>
#include <vector>
#include <set>
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
#include <memory>

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

namespace {

PPL::PIP_Problem::Control_Parameter_Value cutting_strategy
  = PPL::PIP_Problem::CUTTING_STRATEGY_FIRST;

PPL::PIP_Problem::Control_Parameter_Value pivot_row_strategy
    = PPL::PIP_Problem::PIVOT_ROW_STRATEGY_FIRST;

long loop_iterations = 1;

void
pip_display_sol(std::ostream& out,
                const Parma_Polyhedra_Library::PIP_Tree pip,
                const Parma_Polyhedra_Library::Variables_Set& parameters,
                const Parma_Polyhedra_Library::Variables_Set& vars,
                Parma_Polyhedra_Library::dimension_type space_dimension,
                int indent = 0) {
  using namespace std;
  using namespace Parma_Polyhedra_Library::IO_Operators;
  if (!pip) {
    out << setw(indent*2) << "" << "_|_" << endl;
  }
  else {
    for (PIP_Tree_Node::Artificial_Parameter_Sequence::const_iterator
           i = pip->art_parameter_begin(),
           i_end = pip->art_parameter_end();
         i != i_end;
         ++i) {
      out << setw(indent*2) << "" << "Parameter "
          << Linear_Expression(Variable(space_dimension++))
          << " = " << *i << endl;
    }
    const Constraint_System &constraints = pip->constraints();
    bool constraints_empty = constraints.empty();
    if (!constraints_empty) {
      out << setw(indent*2) << "" << "if ";
      for (Constraint_System::const_iterator
             cs_begin = constraints.begin(),
             cs_end = constraints.end(),
             i = cs_begin; i != cs_end; ++i)
        out << ((i == cs_begin) ? "" : " and ") << *i;
      out << " then" << endl;
    }
    const PIP_Decision_Node* decision_node_p = pip->as_decision();
    if (decision_node_p) {
      pip_display_sol(out, decision_node_p->child_node(true),
                      parameters, vars, space_dimension, indent+1);
      out << setw(indent*2) << "" << "else" << endl;
      pip_display_sol(out, decision_node_p->child_node(false),
                      parameters, vars, space_dimension, indent+1);
    }
    else {
      const PIP_Solution_Node* solution_node_p = pip->as_solution();
      out << setw(indent*2 + (constraints_empty ? 0 : 2)) << "" << "{";
      for (Variables_Set::const_iterator
             v_begin = vars.begin(),
             v_end = vars.end(),
             i = v_begin; i != v_end; ++i)
        out << ((i == v_begin) ? "" : " ; ")
            << solution_node_p->parametric_values(Variable(*i));
      out << "}" << endl;
      if (!constraints_empty) {
        out << setw(indent*2) << "" << "else" << endl;
        out << setw(indent*2 + 2) << "" << "_|_" << endl;
      }
    }
  }
}

class PIP_Parser {
public:
  PIP_Parser() : pip() {
    pip.set_control_parameter(cutting_strategy);
    pip.set_control_parameter(pivot_row_strategy);
  }

  virtual ~PIP_Parser() {
  }

  const PPL::PIP_Problem& problem() const {
    return pip;
  }

  virtual bool read(std::istream& in) = 0;

  //! Output the solution in "if-then-else" format.
  void output_solution_tree(std::ostream& out) {
    const PPL::Variables_Set& params = pip.parameter_space_dimensions();
    PPL::Variables_Set vars;
    for (PPL::dimension_type i = 0; i < pip.space_dimension(); ++i) {
      if (params.count(i) == 0)
        vars.insert(i);
    }
    const PPL::PIP_Tree solution = pip.solution();
    pip_display_sol(out, solution, params, vars, pip.space_dimension());
  }

  typedef std::vector<PPL::Coefficient> Coeff_Vector;
  typedef std::vector<int> Int_Vector;

  bool update_pip(PPL::dimension_type num_vars,
                  PPL::dimension_type num_params,
                  PPL::dimension_type num_constraints,
                  PPL::dimension_type num_ctx_rows,
                  const Coeff_Vector& constraints,
                  const Coeff_Vector& context,
                  const Int_Vector& constraint_type,
                  const Int_Vector& ctx_type,
                  PPL::dimension_type bignum_column) {
    pip.add_space_dimensions_and_embed(num_vars, num_params);
    for (PPL::dimension_type k = 0, i = 0; i < num_constraints; ++i) {
      PPL::Linear_Expression expr;
      for (PPL::dimension_type j = 0; j < num_vars + num_params; ++j)
        add_mul_assign(expr, constraints[k++], PPL::Variable(j));
      expr += constraints[k++];
      if (constraint_type[i])
        pip.add_constraint(PPL::Constraint(expr >= 0));
      else
        pip.add_constraint(PPL::Constraint(expr == 0));
    }
    if (num_params > 0) {
      for (PPL::dimension_type k = 0, i = 0; i < num_ctx_rows; ++i) {
        PPL::Linear_Expression expr;
        for (PPL::dimension_type j = 0; j < num_params; ++j)
          add_mul_assign(expr, context[k++], PPL::Variable(num_vars+j));
        expr += context[k++];
        if (ctx_type[i])
          pip.add_constraint(PPL::Constraint(expr >= 0));
        else
          pip.add_constraint(PPL::Constraint(expr == 0));
      }
    }
    if (bignum_column != PPL::not_a_dimension())
      pip.set_big_parameter_dimension(bignum_column);
    return true;
  }

protected:
  //! The problem object.
  PPL::PIP_Problem pip;
}; // class PIP_Parser

class PIP_PolyLib_Parser : public PIP_Parser {
public:
  PIP_PolyLib_Parser(): PIP_Parser() {
  }

  bool read(std::istream& in) {
    std::string line;

    PPL::dimension_type num_ctx_rows;
    PPL::dimension_type num_params;
    getline_no_comment(in, line);
    {
      std::istringstream iss(line);
      iss >> num_ctx_rows >> num_params;
    }
    PPL_ASSERT(num_params >= 2);
    num_params -= 2;

    Coeff_Vector context(num_ctx_rows * (1+num_params));
    Int_Vector ctx_type(num_ctx_rows);
    for (PPL::dimension_type i = 0; i < num_ctx_rows; ++i) {
      getline_no_comment(in, line);
      std::istringstream iss(line);
      iss >> ctx_type[i];
      for (PPL::dimension_type j = 0; j <= num_params; ++j) {
        iss >> context[i*num_ctx_rows + j];
      }
    }

    int bignum_column_coding;
    getline_no_comment(in, line);
    {
      std::istringstream iss(line);
      iss >> bignum_column_coding;
    }

    PPL::dimension_type num_constraints;
    PPL::dimension_type constraint_width;
    PPL::dimension_type num_vars;
    getline_no_comment(in, line);
    {
      std::istringstream iss(line);
      iss >> num_constraints >> constraint_width;
    }
    constraint_width -= 1;
    num_vars = constraint_width - num_params - 1;

    Coeff_Vector constraints(num_constraints * constraint_width);
    Int_Vector constraint_type(num_constraints);
    for (PPL::dimension_type i = 0; i < num_constraints; ++i) {
      getline_no_comment(in, line);
      std::istringstream iss(line);
      iss >> constraint_type[i];
      for (PPL::dimension_type j = 0; j < constraint_width; ++j) {
        iss >> constraints[i*constraint_width + j];
      }
    }

    PPL::dimension_type bignum_column = (bignum_column_coding == -1)
      ? PPL::not_a_dimension()
      : (num_vars + (bignum_column_coding - 1));

    bool result = update_pip(num_vars, num_params,
                             num_constraints, num_ctx_rows,
                             constraints, context,
                             constraint_type, ctx_type,
                             bignum_column);
    return result;
  }

protected:
  static void getline_no_comment(std::istream& in, std::string& s) {
    do {
      getline(in, s);
    } while (s.size() == 0 || s[0] == '\r' || s[0] == '#');
  }
}; // class PIP_PolyLib_Parser

class PIP_PipLib_Parser : public PIP_Parser {
public:
  PIP_PipLib_Parser()
    : PIP_Parser(), comment() {
  }

  bool read(std::istream& in) {
    if (!expect(in, '('))
      return false;
    if (!expect(in, '('))
      return false;
    if (!read_comment(in))
      return false;

    PPL::dimension_type num_vars;
    PPL::dimension_type num_params;
    in >> num_vars >> num_params;

    PPL::dimension_type num_constraints;
    PPL::dimension_type num_ctx_rows;
    in >> num_constraints >> num_ctx_rows;

    int bignum_column_coding;
    in >> bignum_column_coding;
    PPL::dimension_type bignum_column = (bignum_column_coding == -1)
      ? PPL::not_a_dimension()
      : (bignum_column_coding - 1);

    int solve_integer;
    in >> solve_integer;
    if (solve_integer != 1) {
      std::cerr << "Can only solve integer problems." << std::endl;
      return false;
    }

    if (!expect(in, '('))
      return false;
    PPL::dimension_type constraint_width = num_vars+num_params+1;
    Coeff_Vector constraints(num_constraints * constraint_width);
    Int_Vector constraint_type(num_constraints);
    for (PPL::dimension_type i = 0; i < num_constraints; ++i)
      constraint_type[i] = 1;
    for (PPL::dimension_type i = 0; i < num_constraints; ++i)
      if (!read_vector(in, i, constraint_width, num_vars, constraints))
        return false;

    Coeff_Vector context(num_ctx_rows * (1+num_params));
    Int_Vector ctx_type(num_ctx_rows);
    for (PPL::dimension_type i = 0; i < num_ctx_rows; ++i)
      ctx_type[i] = 1;
    for (PPL::dimension_type i = 0; i < num_ctx_rows; ++i)
      if (!read_vector(in, i, num_params+1, num_params, context))
        return false;

    bool result = update_pip(num_vars, num_params,
                             num_constraints, num_ctx_rows,
                             constraints, context,
                             constraint_type, ctx_type,
                             bignum_column);
    return result;
  }

protected:
  bool read_comment(std::istream& in) {
    comment = "";
    int count = 1;
    do {
      char c;
      if (!in.get(c))
        return false;
      if (c == '(')
        ++count;
      else if (c == ')')
        --count;
      if (count > 0)
        comment += c;
    } while (count > 0);
    return true;
  }

  static bool expect(std::istream& in, char c) {
    char a;
    do {
      in >> a;
    } while (a != c && in.good());
    return a == c;
  }

  static bool read_vector(std::istream& in,
                          PPL::dimension_type row_index,
                          PPL::dimension_type row_size,
                          PPL::dimension_type cst_col,
                          Coeff_Vector& tab) {
    if (!expect(in, '#'))
      return false;
    if (!expect(in, '['))
      return false;
    std::string s;
    getline(in, s, ']');
    if (in.fail())
      return false;
    std::istringstream iss(s);
    PPL::dimension_type start_index = row_index * row_size;
    PPL::dimension_type k = start_index;
    for (PPL::dimension_type i = 0; i < cst_col; ++i, ++k) {
      iss >> tab[k];
      if (iss.fail())
        return false;
    }
    iss >> tab[start_index + row_size - 1];
    if (iss.fail())
      return false;
    for (PPL::dimension_type i = cst_col + 1; i < row_size; ++i, ++k) {
      iss >> tab[k];
      if (iss.fail())
        return false;
    }
    return true;
  }

  // The comment string in the source file
  std::string comment;
}; // class PIP_PipLib_Parser

#ifdef PPL_HAVE_GETOPT_H
struct option long_options[] = {
  {"max-cpu",        required_argument, 0, 'C'},
  {"max-memory",     required_argument, 0, 'R'},
  {"help",           no_argument,       0, 'h'},
  {"output",         required_argument, 0, 'o'},
  {"polylib",        no_argument,       0, 'P'},
  {"piplib",         no_argument,       0, 'p'},
  {"timings",        no_argument,       0, 't'},
  {"verbose",        no_argument,       0, 'v'},
  {"iterations",     required_argument, 0, 'i'},
#if defined(USE_PPL)
  {"version",        no_argument,       0, 'V'},
  {"check",          required_argument, 0, 'c'},
#endif
  {"cut-first",      no_argument,       0, 'f'},
  {"cut-deepest",    no_argument,       0, 'd'},
  {"cut-all",        no_argument,       0, 'a'},
  {"row-first",      no_argument,       0, 'F'},
  {"row-max",        no_argument,       0, 'M'},
  {0, 0, 0, 0}
};
#endif

static const char* usage_string
= "Usage: %s [OPTION]... [FILE]\n"
"Reads the definition of a Parametric Integer Programming problem\n"
"and displays the lexicographic minimum in terms of the values of the\n"
"parameters.\n\n"
"Options:\n"
"  -RMB, --max-memory=MB   limits memory usage to MB megabytes\n"
"  -h, --help              prints this help text to stdout\n"
"  -oPATH, --output=PATH   appends output to PATH\n"
"  -P, --polylib           reads problem in PolyLib format (default)\n"
"  -p, --piplib            reads problem in PipLib format\n"
"  -t, --timings           prints timings to stderr\n"
"  -v, --verbose           produces lots of output\n"
"  -i, --iterations=N      executes the resolution N times (default=1)\n"
#if defined(USE_PPL)
"  -V, --version           prints version information to stdout\n"
"  -cPATH, --check=PATH    checks if the result is equal to what is in PATH\n"
#endif
"\nCut generation options:\n"
"  -f, --cut-first         uses the first non-integer row (default)\n"
"  -d, --cut-deepest       tries to generate the deepest cut\n"
"  -a, --cut-all           always generates all possible cuts\n"
"\nPivot row strategy options:\n"
"  -F, --row-first         uses the first row with negative parameter (default)\n"
"  -M, --row-max           chooses row generating the lexico-maximal pivot column\n"
#ifndef PPL_HAVE_GETOPT_H
"\n"
"NOTE: this version does not support long options.\n"
#endif
"\n"
"Report bugs to <ppl-devel@cs.unipr.it>.\n";

#if defined(USE_PPL)
#define OPTION_LETTERS "R:ho:Pptvi:Vc:fdaFM"
#else
#define OPTION_LETTERS "R:ho:Pptvi:fdaFM"
#endif

const char* program_name = 0;

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

bool piplib_format = false;

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
    switch (c) {
    case 0:
      break;

    case '?':
    case 'h':
      fprintf(stdout, usage_string, argv[0]);
      exit(0);
      break;

    case 'R':
      {
        const int MEGA = 1024*1024;
        long l = strtol(optarg, &endptr, 10);
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

    case 'P':
      piplib_format = false;
      break;

    case 'p':
      piplib_format = true;
      break;

    case 't':
      print_timings = true;
      break;

    case 'v':
      verbose = true;
      break;

    case 'i':
      loop_iterations = strtol(optarg, &endptr, 10);
      if (*endptr || loop_iterations < 1)
	fatal("a positive integer must follow `-i'");
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

    case 'f':
      cutting_strategy = PPL::PIP_Problem::CUTTING_STRATEGY_FIRST;
      break;

    case 'd':
      cutting_strategy = PPL::PIP_Problem::CUTTING_STRATEGY_DEEPEST;
      break;

    case 'a':
      cutting_strategy = PPL::PIP_Problem::CUTTING_STRATEGY_ALL;
      break;

    case 'F':
      pivot_row_strategy = PPL::PIP_Problem::PIVOT_ROW_STRATEGY_FIRST;
      break;

    case 'M':
      pivot_row_strategy = PPL::PIP_Problem::PIVOT_ROW_STRATEGY_MAX_COLUMN;
      break;

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

  if (max_bytes_of_virtual_memory > 0)
    limit_virtual_memory(max_bytes_of_virtual_memory);

  // Set up the input and output streams.
  set_input(input_file_name);
  set_output(output_file_name);

//  POLYHEDRON_TYPE ph;
//  Representation rep = read_polyhedron(input(), ph);
  std::auto_ptr<PIP_Parser> parser;
  if (piplib_format)
    parser.reset(new PIP_PipLib_Parser);
  else
    parser.reset(new PIP_PolyLib_Parser);
  if (!parser->read(*input_stream_p))
    return 1;

  maybe_start_clock();

  const PPL::PIP_Problem& pip = parser->problem();

  if (loop_iterations == 1) {
    // Compute the dual simplex on the problem.
    pip.solve();
    // Write the solution.
    parser->output_solution_tree(*output_stream_p);
  }
  else {
    std::auto_ptr<PPL::PIP_Problem> pip_p;
    // Perform a time benchmark loop executing the resolution several times.
    for (long i = 0; i < loop_iterations; ++i) {
      pip_p.reset(new PPL::PIP_Problem(pip));
      pip_p->solve();
    }
  }

#if defined(USE_PPL) || defined(USE_PIPLIB)
  maybe_print_clock();
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
