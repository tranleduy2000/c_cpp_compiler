/* Implementation of utility functions used in test programs.
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
#include <csignal>
#include <iostream>
#include <exception>
#include <limits>
#ifdef PPL_HAVE_FENV_H
#include <fenv.h>
#endif

namespace {

void
unexpected_exception_handler() {
  std::cerr << "unexpected exception thrown" << std::endl;
  exit(1);
}

void
uncaught_exception_handler() {
  std::cerr << "uncaught exception" << std::endl;
  exit(1);
}

#if PPL_HAVE_DECL_SIGACTION

#if defined(PPL_HAVE_SIGINFO_T) && defined(SA_SIGINFO)

void
fpe_sigaction(int sig, siginfo_t* sip, void*) {
  if (sig != SIGFPE) {
    std::cerr << "fpe_handler called on signal different from SIGFPE"
	      << std::endl;
    exit(1);
  }
  const char* s = 0;
  switch (sip->si_code) {
  case FPE_INTDIV:
    s = "integer divide by zero";
    break;
  case FPE_INTOVF:
    s = "integer overflow";
    break;
  case FPE_FLTDIV:
    s = "floating point divide by zero";
    break;
  case FPE_FLTOVF:
    s = "floating point overflow";
    break;
  case FPE_FLTUND:
    s = "floating point underflow";
    break;
  case FPE_FLTRES:
    s = "floating point inexact result";
    break;
  case FPE_FLTINV:
    s = "floating point invalid operation";
    break;
  case FPE_FLTSUB:
    s = "subscript out of range";
    break;
  default:
    break;
  }
  if (s != 0)
    std::cerr << "SIGFPE caught (cause: " << s << ")"
	      << std::endl;
  else {
    std::cerr << "SIGFPE caught (unknown si_code " << sip->si_code << ")"
	      << std::endl;
#if defined(PPL_HAVE_FENV_H)
    std::cerr << "Inquire with fetestexcept(): ";
#ifdef FE_INEXACT
    if (fetestexcept(FE_INEXACT))
      std::cerr << "FE_INEXACT ";
#endif
#ifdef FE_DIVBYZERO
    if (fetestexcept(FE_DIVBYZERO))
      std::cerr << "FE_DIVBYZERO ";
#endif
#ifdef FE_UNDERFLOW
    if (fetestexcept(FE_UNDERFLOW))
      std::cerr << "FE_UNDERFLOW ";
#endif
#ifdef FE_OVERFLOW
    if (fetestexcept(FE_OVERFLOW))
      std::cerr << "FE_OVERFLOW ";
#endif
#if FE_INVALID
    if (fetestexcept(FE_INVALID))
      std::cerr << "FE_INVALID ";
#endif
    std::cerr << std::endl;
#endif // defined(PPL_HAVE_FENV_H)
  }
  exit(1);
}

#else // !defined(PPL_HAVE_SIGINFO_T) || !defined(SA_SIGINFO)

void
fpe_handler(int sig) {
  if (sig != SIGFPE) {
    std::cerr << "fpe_handler called on signal different from SIGFPE"
	      << std::endl;
    exit(1);
  }
  std::cerr << "SIGFPE caught"
            << std::endl;
#if defined(PPL_HAVE_FENV_H)
  std::cerr << "Inquire with fetestexcept(): ";
#ifdef FE_INEXACT
  if (fetestexcept(FE_INEXACT))
    std::cerr << "FE_INEXACT ";
#endif
#ifdef FE_DIVBYZERO
  if (fetestexcept(FE_DIVBYZERO))
    std::cerr << "FE_DIVBYZERO ";
#endif
#ifdef FE_UNDERFLOW
  if (fetestexcept(FE_UNDERFLOW))
    std::cerr << "FE_UNDERFLOW ";
#endif
#ifdef FE_OVERFLOW
  if (fetestexcept(FE_OVERFLOW))
    std::cerr << "FE_OVERFLOW ";
#endif
#if FE_INVALID
  if (fetestexcept(FE_INVALID))
    std::cerr << "FE_INVALID ";
#endif
  std::cerr << std::endl;
#endif // defined(PPL_HAVE_FENV_H)
  exit(1);
}

#endif // !defined(PPL_HAVE_SIGINFO_T) || !defined(SA_SIGINFO)

#endif // PPL_HAVE_DECL_SIGACTION

} // namespace

namespace Parma_Polyhedra_Library {

namespace Test {

void
set_handlers() {
#if PPL_HAVE_DECL_SIGACTION
  struct sigaction action;
  sigemptyset(&action.sa_mask);
#if defined(PPL_HAVE_SIGINFO_T) && defined(SA_SIGINFO)
  action.sa_sigaction = fpe_sigaction;
  action.sa_flags = SA_SIGINFO;
#else // !defined(PPL_HAVE_SIGINFO_T) || !defined(SA_SIGINFO)
  action.sa_handler = fpe_handler;
  action.sa_flags = 0;
#endif // !defined(PPL_HAVE_SIGINFO_T) || !defined(SA_SIGINFO)
  if (sigaction(SIGFPE, &action, NULL) != 0) {
    std::cerr << "sigaction() failed"
	      << std::endl;
    abort();
  }
#endif // PPL_HAVE_DECL_SIGACTION

  std::set_unexpected(unexpected_exception_handler);
  std::set_terminate(uncaught_exception_handler);
}

bool
check_distance(const Checked_Number<mpq_class, Extended_Number_Policy>& d,
               const char* max_d_s, const char* d_name) {
  Checked_Number<mpq_class, Extended_Number_Policy>
    max_d((max_d_s ? max_d_s : "0"), ROUND_NOT_NEEDED);
  assert(max_d >= 0);
  if (d > max_d) {
    nout << "Excessive " << d_name << " distance ";
    if (is_plus_infinity(d))
      nout << "+inf";
    else if (raw_value(d) > std::numeric_limits<double>::max())
      nout << ">" << std::numeric_limits<double>::max();
    else
      nout << raw_value(d).get_d() << " (rounded towards zero)";
    nout << ": should be at most " << max_d << "."
	 << std::endl;
    return false;
  }
  else
    return true;
}

bool
check_result(const BD_Shape<mpq_class>& computed_result,
	     const BD_Shape<mpq_class>& known_result) {
  if (computed_result == known_result)
    return true;
  else {
    using IO_Operators::operator<<;
    nout << "Equality does not hold:"
         << "\ncomputed result is\n"
         << computed_result
         << "\nknown result is\n"
         << known_result
         << endl;
    return false;
  }
}

bool
check_result(const Rational_Box& computed_result,
	     const Rational_Box& known_result) {
  if (computed_result == known_result)
    return true;
  else {
    using IO_Operators::operator<<;
    nout << "Equality does not hold:"
         << "\ncomputed result is\n"
         << computed_result
         << "\nknown result is\n"
         << known_result
         << endl;
    return false;
  }
}

bool
check_result(const Generator& computed_result,
             const Generator& known_result,
             const char* max_r_d_s,
             const char* max_e_d_s,
             const char* max_l_d_s) {
  // Handle in a more efficient way the case where equality is expected.
  if (max_r_d_s == 0 && max_e_d_s == 0 && max_l_d_s == 0) {
    if (computed_result != known_result) {
      using IO_Operators::operator<<;
      nout << "Equality does not hold:"
           << "\ncomputed result is\n"
           << computed_result
           << "\nknown result is\n"
           << known_result
           << endl;
      return false;
    }
    else
      return true;
  }

  Checked_Number<mpq_class, Extended_Number_Policy> r_d;
  rectilinear_distance_assign(r_d, known_result, computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> e_d;
  euclidean_distance_assign(e_d, known_result, computed_result, ROUND_UP);
  Checked_Number<mpq_class, Extended_Number_Policy> l_d;
  l_infinity_distance_assign(l_d, known_result, computed_result, ROUND_UP);
  bool ok_r = check_distance(r_d, max_r_d_s, "rectilinear");
  bool ok_e = check_distance(e_d, max_e_d_s, "euclidean");
  bool ok_l = check_distance(l_d, max_l_d_s, "l_infinity");
  bool ok = ok_r && ok_e && ok_l;
  if (!ok) {
    using IO_Operators::operator<<;
    nout << "Computed result is\n"
         << computed_result
         << "\nknown result is\n"
         << known_result
         << endl;
  }
  return ok;
}

bool
check_result(const Checked_Number<mpq_class, Extended_Number_Policy>& computed,
             const Checked_Number<mpq_class, Extended_Number_Policy>& known,
             const char* max_r_d_s) {
  // Handle in a more efficient way the case where equality is expected.
  if (max_r_d_s == 0) {
    if (computed != known) {
      using IO_Operators::operator<<;
      nout << "Equality does not hold:"
           << "\ncomputed result is\n"
           << computed
           << "\nknown result is\n"
           << known
           << endl;
      return false;
    }
    else
      return true;
  }

  Checked_Number<mpq_class, Extended_Number_Policy> r_d;
  sub_assign_r(r_d, known, computed, ROUND_NOT_NEEDED);
  abs_assign_r(r_d, r_d, ROUND_NOT_NEEDED);
  bool ok = check_distance(r_d, max_r_d_s, "rectilinear");
  if (!ok) {
    using IO_Operators::operator<<;
    nout << "Computed result is\n"
         << computed
         << "\nknown result is\n"
         << known
         << endl;
  }
  return ok;
}

std::ostream&
operator<<(std::ostream& s, const FCAIBVP& x) {
  s << "{";
  for (FCAIBVP::Set::const_iterator i = x.set.begin(),
	 x_end = x.set.end(); i != x_end; ) {
    const Variable v(*i++);
    using IO_Operators::operator<<;
    s << v;
    if (i != x_end)
      s << ", ";
  }
  s << "}";
  return s;
}

void
print_constraint(const Constraint& c,
		 const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  using IO_Operators::operator<<;
  s << c << std::endl;
}

void
print_constraints(const Polyhedron& ph,
		  const std::string& intro, std::ostream& s) {
  print_constraints(ph.constraints(), intro, s);
}

#if 0
void
print_constraints(const Affine_Space& affs,
		  const std::string& intro, std::ostream& s) {
  print_constraints(affs.constraints(), intro, s);
}
#endif

void
print_constraints(const Constraint_System& cs,
		  const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  Constraint_System::const_iterator i = cs.begin();
  Constraint_System::const_iterator cs_end = cs.end();
  bool printed_something = i != cs_end;
  while (i != cs_end) {
    using IO_Operators::operator<<;
    s << *i++;
    if (i != cs_end)
      s << ",\n";
  }
  s << (printed_something ? "." : "true.") << std::endl;
}

void
print_congruence(const Congruence& c,
		 const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  using IO_Operators::operator<<;
  s << c << std::endl;
}

void
print_congruences(const Congruence_System& cs,
		  const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << std::endl;
  Congruence_System::const_iterator i = cs.begin();
  Congruence_System::const_iterator cs_end = cs.end();
  bool printed_something = i != cs_end;
  while (i != cs_end) {
    using IO_Operators::operator<<;
    s << *i++;
    if (i != cs_end)
      s << "," << std::endl;
  }
  s << (printed_something ? "." : "true.") << std::endl;
}

void
print_congruences(const Grid& gr,
		  const std::string& intro, std::ostream& s) {
  print_congruences(gr.congruences(), intro, s);
}

void
print_generator(const Generator& g,
		const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  using IO_Operators::operator<<;
  s << g << std::endl;
}

void
print_generator(const Grid_Generator& g,
		const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  using IO_Operators::operator<<;
  s << g << std::endl;
}

void
print_generators(const Polyhedron& ph,
		 const std::string& intro, std::ostream& s) {
  print_generators(ph.generators(), intro, s);
}

void
print_generators(const Grid& gr,
		 const std::string& intro, std::ostream& s) {
  print_generators(gr.grid_generators(), intro, s);
}

void
print_generators(const Generator_System& gs,
		 const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  Generator_System::const_iterator i = gs.begin();
  Generator_System::const_iterator gs_end = gs.end();
  bool printed_something = i != gs_end;
  while (i != gs_end) {
    using IO_Operators::operator<<;
    s << *i++;
    if (i != gs_end)
      s << ",\n";
  }
  s << (printed_something ? "." : "false.") << std::endl;
}

void
print_generators(const Grid_Generator_System& gs,
		 const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << "\n";
  Grid_Generator_System::const_iterator i = gs.begin();
  Grid_Generator_System::const_iterator gs_end = gs.end();
  bool printed_something = i != gs_end;
  while (i != gs_end) {
    using IO_Operators::operator<<;
    s << *i++;
    if (i != gs_end)
      s << ",\n";
  }
  s << (printed_something ? "." : "false.") << std::endl;
}

void
print_function(const Parma_Polyhedra_Library::Partial_Function& function,
	       const std::string& intro, std::ostream& s) {
  if (!intro.empty())
    s << intro << std::endl;
  function.print(s);
}

} // namespace Test

} // namespace Parma_Polyhedra_Library
