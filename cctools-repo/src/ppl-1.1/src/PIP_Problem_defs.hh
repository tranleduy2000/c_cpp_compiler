/* PIP_Problem class declaration.
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

#ifndef PPL_PIP_Problem_defs_hh
#define PPL_PIP_Problem_defs_hh 1

#include "PIP_Problem_types.hh"
#include "PIP_Tree_types.hh"
#include "globals_types.hh"
#include "Linear_Expression_defs.hh"
#include "Constraint_defs.hh"
#include "Constraint_System_types.hh"
#include "Generator_defs.hh"
#include "Variables_Set_defs.hh"
#include <vector>
#include <deque>
#include <iosfwd>

#include "Matrix_defs.hh"

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::PIP_Problem */
std::ostream&
operator<<(std::ostream& s, const PIP_Problem& pip);

} // namespace IO_Operators

//! Swaps \p x with \p y.
/*! \relates PIP_Problem */
void swap(PIP_Problem& x, PIP_Problem& y);

} // namespace Parma_Polyhedra_Library

//! A Parametric Integer (linear) Programming problem.
/*! \ingroup PPL_CXX_interface
  An object of this class encodes a parametric integer (linear)
  programming problem. The PIP problem is specified by providing:
   - the dimension of the vector space;
   - the subset of those dimensions of the vector space that are
     interpreted as integer parameters (the other space dimensions
     are interpreted as non-parameter integer variables);
   - a finite set of linear equality and (strict or non-strict)
     inequality constraints involving variables and/or parameters;
     these constraints are used to define:
       - the <EM>feasible region</EM>, if they involve one or more
         problem variable (and maybe some parameters);
       - the <EM>initial context</EM>, if they only involve the
         parameters;
   - optionally, the so-called <EM>big parameter</EM>,
     i.e., a problem parameter to be considered arbitrarily big.

  Note that all problem variables and problem parameters are assumed
  to take non-negative integer values, so that there is no need
  to specify non-negativity constraints.

  The class provides support for the (incremental) solution of the
  PIP problem based on variations of the revised simplex method and
  on Gomory cut generation techniques.

  The solution for a PIP problem is the lexicographic minimum of the
  integer points of the feasible region, expressed in terms of the
  parameters. As the problem to be solved only involves non-negative
  variables and parameters, the problem will always be either unfeasible
  or optimizable.

  As the feasibility and the solution value of a PIP problem depend on the
  values of the parameters, the solution is a binary decision tree,
  dividing the context parameter set into subsets.
  The tree nodes are of two kinds:
   - \e Decision nodes.
     These are internal tree nodes encoding one or more linear tests
     on the parameters; if all the tests are satisfied, then the solution
     is the node's \e true child; otherwise, the solution is the node's
     \e false child;
   - \e Solution nodes.
     These are leaf nodes in the tree, encoding the solution of the problem
     in the current context subset, where each variable is defined in terms
     of a linear expression of the parameters.
     Solution nodes also optionally embed a set of parameter constraints:
     if all these constraints are satisfied, the solution is described by
     the node, otherwise the problem has no solution.

  It may happen that a decision node has no \e false child. This means
  that there is no solution if at least one of the corresponding
  constraints is not satisfied. Decision nodes having two or more linear
  tests on the parameters cannot have a \e false child. Decision nodes
  always have a \e true child.

  Both kinds of tree nodes may also contain the definition of extra
  parameters which are artificially introduced by the solver to enforce
  an integral solution. Such artificial parameters are defined by
  the integer division of a linear expression on the parameters
  by an integer coefficient.

  By exploiting the incremental nature of the solver, it is possible
  to reuse part of the computational work already done when solving
  variants of a given PIP_Problem: currently, incremental resolution
  supports the addition of space dimensions, the addition of parameters
  and the addition of constraints.

  \par Example problem
  An example PIP problem can be defined the following:
  \code
  3*j >= -2*i+8
  j <= 4*i - 4
  i <= n
  j <= m
  \endcode
  where \c i and \c j are the problem variables
  and \c n and \c m are the problem parameters.
  This problem can be optimized; the resulting solution tree may be
  represented as follows:
  \verbatim
  if 7*n >= 10 then
    if 7*m >= 12 then
      {i = 2 ; j = 2}
    else
      Parameter P = (m) div 2
      if 2*n + 3*m >= 8 then
        {i = -m - P + 4 ; j = m}
      else
        _|_
  else
    _|_
  \endverbatim
  The solution tree starts with a decision node depending on the
  context constraint <code>7*n >= 10</code>.
  If this constraint is satisfied by the values assigned to the
  problem parameters, then the (textually first) \c then branch is taken,
  reaching the \e true child of the root node (which in this case
  is another decision node); otherwise, the (textually last) \c else
  branch is taken, for which there is no corresponding \e false child.
  \par
  The \f$\perp\f$ notation, also called \e bottom, denotes the
  lexicographic minimum of an empty set of solutions,
  here meaning the corresponding subproblem is unfeasible.
  \par
  Notice that a tree node may introduce new (non-problem) parameters,
  as is the case for parameter \c P in the (textually first) \c else
  branch above. These \e artificial parameters are only meaningful
  inside the subtree where they are defined and are used to define
  the parametric values of the problem variables in solution nodes
  (e.g., the <CODE>{i,j}</CODE> vector in the textually third \c then branch).

  \par Context restriction
  The above solution is correct in an unrestricted initial context,
  meaning all possible values are allowed for the parameters. If we
  restrict the context with the following parameter inequalities:
  \code
  m >= n
  n >= 5
  \endcode
  then the resulting optimizing tree will be a simple solution node:
  \verbatim
  {i = 2 ; j = 2}
  \endverbatim

  \par Creating the PIP_Problem object
  The PIP_Problem object corresponding to the above example can be
  created as follows:
  \code
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);
  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);
  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  \endcode
  If you want to restrict the initial context, simply add the parameter
  constraints the same way as for normal constraints.
  \code
  cs.insert(m >= n);
  cs.insert(n >= 5);
  \endcode

  \par Solving the problem
  Once the PIP_Problem object has been created, you can start the
  resolution of the problem by calling the solve() method:
  \code
  PIP_Problem_Status status = pip.solve();
  \endcode
  where the returned \c status indicates if the problem has been optimized
  or if it is unfeasible for any possible configuration of the parameter
  values. The resolution process is also started if an attempt is made
  to get its solution, as follows:
  \code
  const PIP_Tree_Node* node = pip.solution();
  \endcode
  In this case, an unfeasible problem will result in an empty solution
  tree, i.e., assigning a null pointer to \c node.

  \par Printing the solution tree
  A previously computed solution tree may be printed as follows:
  \code
  pip.print_solution(std::cout);
  \endcode
  This will produce the following output (note: variables and parameters
  are printed according to the default output function; see
  <code>Variable::set_output_function</code>):
  \verbatim
  if 7*C >= 10 then
    if 7*D >= 12 then
      {2 ; 2}
    else
      Parameter E = (D) div 2
      if 2*C + 3*D >= 8 then
        {-D - E + 4 ; D}
      else
        _|_
  else
    _|_
  \endverbatim

  \par Spanning the solution tree
  A parameter assignment for a PIP problem binds each of the problem
  parameters to a non-negative integer value. After fixing a parameter
  assignment, the ``spanning'' of the PIP problem solution tree refers
  to the process whereby the solution tree is navigated, starting from
  the root node: the value of artificial parameters is computed according
  to the parameter assignment and the node's constraints are evaluated,
  thereby descending in either the true or the false subtree of decision
  nodes and eventually reaching a solution node or a bottom node.
  If a solution node is found, each of the problem variables is provided
  with a parametric expression, which can be evaluated to a fixed value
  using the given parameter assignment and the computed values for
  artificial parameters.
  \par
  The coding of the spanning process can be done as follows.
  First, the root of the PIP solution tree is retrieved:
  \code
  const PIP_Tree_Node* node = pip.solution();
  \endcode
  If \c node represents an unfeasible solution (i.e., \f$\perp\f$),
  its value will be \c 0. For a non-null tree node, the virtual methods
  \c PIP_Tree_Node::as_decision() and \c PIP_Tree_Node::as_solution()
  can be used to check whether the node is a decision or a solution node:
  \code
  const PIP_Solution_Node* sol = node->as_solution();
  if (sol != 0) {
    // The node is a solution node
    ...
  }
  else {
    // The node is a decision node
    const PIP_Decision_Node* dec = node->as_decision();
    ...
  }
  \endcode
  \par
  The true (resp., false) child node of a Decision Node may be accessed by
  using method \c PIP_Decision_Node::child_node(bool), passing \c true
  (resp., \c false) as the input argument.

  \par Artificial parameters
  A PIP_Tree_Node::Artificial_Parameter object represents the result
  of the integer division of a Linear_Expression (on the other
  parameters, including the previously-defined artificials)
  by an integer denominator (a Coefficient object).
  The dimensions of the artificial parameters (if any) in a tree node
  have consecutive indices starting from <code>dim+1</code>, where the value
  of \c dim is computed as follows:
   - for the tree root node, \c dim is the space dimension of the PIP_Problem;
   - for any other node of the tree, it is recursively obtained by adding
     the value of \c dim computed for the parent node to the number of
     artificial parameters defined in the parent node.
  \par
  Since the numbering of dimensions for artificial parameters follows
  the rule above, the addition of new problem variables and/or new problem
  parameters to an already solved PIP_Problem object (as done when
  incrementally solving a problem) will result in the systematic
  renumbering of all the existing artificial parameters.

  \par Node constraints
  All kind of tree nodes can contain context constraints.
  Decision nodes always contain at least one of them.
  The node's local constraint system can be obtained using method
  PIP_Tree_Node::constraints.
  These constraints only involve parameters, including both the problem
  parameters and the artificial parameters that have been defined
  in nodes occurring on the path from the root node to the current node.
  The meaning of these constraints is as follows:
   - On a decision node, if all tests in the constraints are true, then the
     solution is the \e true child; otherwise it is the \e false child.
   - On a solution node, if the (possibly empty) system of constraints
     evaluates to true for a given parameter assignment, then the solution
     is described by the node; otherwise the solution is \f$\perp\f$
     (i.e., the problem is unfeasible for that parameter assignment).

  \par Getting the optimal values for the variables
  After spanning the solution tree using the given parameter assignment,
  if a solution node has been reached, then it is possible to retrieve
  the parametric expression for each of the problem variables using
  method PIP_Solution_Node::parametric_values. The retrieved expression
  will be defined in terms of all the parameters (problem parameters
  and artificial parameters defined along the path).

  \par Solving maximization problems
  You can solve a lexicographic maximization problem by reformulating its
  constraints using variable substitution. Proceed the following steps:
   - Create a big parameter (see PIP_Problem::set_big_parameter_dimension),
     which we will call \f$M\f$.
   - Reformulate each of the maximization problem constraints by
     substituting each \f$x_i\f$ variable with an expression of the form
     \f$M-x'_i\f$, where the \f$x'_i\f$ variables are positive variables to
     be minimized.
   - Solve the lexicographic minimum for the \f$x'\f$ variable vector.
   - In the solution expressions, the values of the \f$x'\f$ variables will
     be expressed in the form: \f$x'_i = M-x_i\f$. To get back the value of
     the expression of each \f$x_i\f$ variable, just apply the
     formula: \f$x_i = M-x'_i\f$.
  \par
  Note that if the resulting expression of one of the \f$x'_i\f$ variables
  is not in the \f$x'_i = M-x_i\f$ form, this means that the
  sign-unrestricted problem is unbounded.
  \par
  You can choose to maximize only a subset of the variables while minimizing
  the other variables. In that case, just apply the variable substitution
  method on the variables you want to be maximized. The variable
  optimization priority will still be in lexicographic order.

  \par
  \b Example: consider you want to find the lexicographic maximum of the
  \f$(x,y)\f$ vector, under the constraints:
    \f[\left\{\begin{array}{l}
      y \geq 2x - 4\\
      y \leq -x + p
    \end{array}\right.\f]
  \par
  where \f$p\f$ is a parameter.
  \par
  After variable substitution, the constraints become:
    \f[\left\{\begin{array}{l}
      M - y \geq 2M - 2x - 4\\
      M - y \leq -M + x + p
    \end{array}\right.\f]
  \par
  The code for creating the corresponding problem object is the following:
  \code
  Variable x(0);
  Variable y(1);
  Variable p(2);
  Variable M(3);
  Variables_Set params(p, M);
  Constraint_System cs;
  cs.insert(M - y >= 2*M - 2*x - 4);
  cs.insert(M - y <= -M + x + p);
  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  pip.set_big_parameter_dimension(3);     // M is the big parameter
  \endcode
  Solving the problem provides the following solution:
  \verbatim
  Parameter E = (C + 1) div 3
  {D - E - 1 ; -C + D + E + 1}
  \endverbatim
  Under the notations above, the solution is:
  \f[ \left\{\begin{array}{l}
    x' = M - \left\lfloor\frac{p+1}{3}\right\rfloor - 1 \\
    y' = M - p + \left\lfloor\frac{p+1}{3}\right\rfloor + 1
  \end{array}\right.
  \f]
  \par
  Performing substitution again provides us with the values of the original
  variables:
  \f[ \left\{\begin{array}{l}
    x = \left\lfloor\frac{p+1}{3}\right\rfloor + 1 \\
    y = p - \left\lfloor\frac{p+1}{3}\right\rfloor - 1
  \end{array}\right.
  \f]

  \par Allowing variables to be arbitrarily signed
  You can deal with arbitrarily signed variables by reformulating the
  constraints using variable substitution. Proceed the following steps:
   - Create a big parameter (see PIP_Problem::set_big_parameter_dimension),
     which we will call \f$M\f$.
   - Reformulate each of the maximization problem constraints by
     substituting each \f$x_i\f$ variable with an expression of the form
     \f$x'_i-M\f$, where the \f$x'_i\f$ variables are positive.
   - Solve the lexicographic minimum for the \f$x'\f$ variable vector.
   - The solution expression can be read in the form:
   - In the solution expressions, the values of the \f$x'\f$ variables will
     be expressed in the form: \f$x'_i = x_i+M\f$. To get back the value of
     the expression of each signed \f$x_i\f$ variable, just apply the
     formula: \f$x_i = x'_i-M\f$.
  \par
  Note that if the resulting expression of one of the \f$x'_i\f$ variables
  is not in the \f$x'_i = x_i+M\f$ form, this means that the
  sign-unrestricted problem is unbounded.
  \par
  You can choose to define only a subset of the variables to be
  sign-unrestricted. In that case, just apply the variable substitution
  method on the variables you want to be sign-unrestricted.

  \par
  \b Example: consider you want to find the lexicographic minimum of the
  \f$(x,y)\f$ vector, where the \f$x\f$ and \f$y\f$ variables are
  sign-unrestricted, under the constraints:
    \f[\left\{\begin{array}{l}
      y \geq -2x - 4\\
      2y \leq x + 2p
    \end{array}\right.\f]
  \par
  where \f$p\f$ is a parameter.
  \par
  After variable substitution, the constraints become:
    \f[\left\{\begin{array}{l}
      y' - M \geq -2x' + 2M - 4\\
      2y' - 2M \leq x' - M + 2p
    \end{array}\right.\f]
  \par
  The code for creating the corresponding problem object is the following:
  \code
  Variable x(0);
  Variable y(1);
  Variable p(2);
  Variable M(3);
  Variables_Set params(p, M);
  Constraint_System cs;
  cs.insert(y - M >= -2*x + 2*M - 4);
  cs.insert(2*y - 2*M <= x - M + 2*p);
  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  pip.set_big_parameter_dimension(3);     // M is the big parameter
  \endcode
  \par
  Solving the problem provides the following solution:
  \verbatim
  Parameter E = (2*C + 3) div 5
  {D - E - 1 ; D + 2*E - 2}
  \endverbatim
  Under the notations above, the solution is:
  \f[ \left\{\begin{array}{l}
    x' = M - \left\lfloor\frac{2p+3}{5}\right\rfloor - 1 \\
    y' = M + 2\left\lfloor\frac{2p+3}{5}\right\rfloor - 2
  \end{array}\right.
  \f]
  \par
  Performing substitution again provides us with the values of the original
  variables:
  \f[ \left\{\begin{array}{l}
    x = -\left\lfloor\frac{2p+3}{5}\right\rfloor - 1 \\
    y = 2\left\lfloor\frac{2p+3}{5}\right\rfloor - 2
  \end{array}\right.
  \f]

  \par Allowing parameters to be arbitrarily signed
  You can consider a parameter \f$p\f$ arbitrarily signed by replacing
  \f$p\f$ with \f$p^+-p^-\f$, where both \f$p^+\f$ and \f$p^-\f$ are
  positive parameters. To represent a set of arbitrarily signed parameters,
  replace each parameter \f$p_i\f$ with \f$p^+_i-p^-\f$, where \f$-p^-\f$ is
  the minimum negative value of all parameters.

  \par Minimizing a linear cost function
  Lexicographic solving can be used to find the parametric minimum of a
  linear cost function.
  \par
  Suppose the variables are named \f$x_1, x_2, \dots, x_n\f$, and the
  parameters \f$p_1, p_2, \dots, p_m\f$. You can minimize a linear cost
  function \f$f(x_2, \dots, x_n, p_1, \dots, p_m)\f$ by simply adding the
  constraint \f$x_1 \geq f(x_2, \dots, x_n, p_1, \dots, p_m)\f$ to the
  constraint system. As lexicographic minimization ensures \f$x_1\f$ is
  minimized in priority, and because \f$x_1\f$ is forced by a constraint to
  be superior or equal to the cost function, optimal solutions of the
  problem necessarily ensure that the solution value of \f$x_1\f$ is the
  optimal value of the cost function.
*/
class Parma_Polyhedra_Library::PIP_Problem {
public:
  //! Builds a trivial PIP problem.
  /*!
    A trivial PIP problem requires to compute the lexicographic minimum
    on a vector space under no constraints and with no parameters:
    due to the implicit non-negativity constraints, the origin of the
    vector space is an optimal solution.

    \param dim
    The dimension of the vector space enclosing \p *this
    (optional argument with default value \f$0\f$).

    \exception std::length_error
    Thrown if \p dim exceeds <CODE>max_space_dimension()</CODE>.
  */
  explicit PIP_Problem(dimension_type dim = 0);

  /*! \brief
    Builds a PIP problem having space dimension \p dim
    from the sequence of constraints in the range
    \f$[\mathrm{first}, \mathrm{last})\f$;
    those dimensions whose indices occur in \p p_vars are
    interpreted as parameters.

    \param dim
    The dimension of the vector space (variables and parameters) enclosing
    \p *this.

    \param first
    An input iterator to the start of the sequence of constraints.

    \param last
    A past-the-end input iterator to the sequence of constraints.

    \param p_vars
    The set of variables' indexes that are interpreted as parameters.

    \exception std::length_error
    Thrown if \p dim exceeds <CODE>max_space_dimension()</CODE>.

    \exception std::invalid_argument
    Thrown if the space dimension of a constraint in the sequence
    (resp., the parameter variables) is strictly greater than \p dim.
  */
  template <typename In>
  PIP_Problem(dimension_type dim, In first, In last,
              const Variables_Set& p_vars);

  //! Ordinary copy-constructor.
  PIP_Problem(const PIP_Problem& y);

  //! Destructor.
  ~PIP_Problem();

  //! Assignment operator.
  PIP_Problem& operator=(const PIP_Problem& y);

  //! Returns the maximum space dimension a PIP_Problem can handle.
  static dimension_type max_space_dimension();

  //! Returns the space dimension of the PIP problem.
  dimension_type space_dimension() const;

  /*! \brief
    Returns a set containing all the variables' indexes representing
    the parameters of the PIP problem.
  */
  const Variables_Set& parameter_space_dimensions() const;

private:
  //! A type alias for a sequence of constraints.
  typedef std::vector<Constraint> Constraint_Sequence;

public:
  /*! \brief
    A type alias for the read-only iterator on the constraints
    defining the feasible region.
  */
  typedef Constraint_Sequence::const_iterator const_iterator;

  /*! \brief
    Returns a read-only iterator to the first constraint defining
    the feasible region.
  */
  const_iterator constraints_begin() const;

  /*! \brief
    Returns a past-the-end read-only iterator to the sequence of
    constraints defining the feasible region.
  */
  const_iterator constraints_end() const;

  //! Resets \p *this to be equal to the trivial PIP problem.
  /*!
    The space dimension is reset to \f$0\f$.
  */
  void clear();

  /*! \brief
    Adds <CODE>m_vars + m_params</CODE> new space dimensions
    and embeds the old PIP problem in the new vector space.

    \param m_vars
    The number of space dimensions to add that are interpreted as
    PIP problem variables (i.e., non parameters). These are added
    \e before adding the \p m_params parameters.

    \param m_params
    The number of space dimensions to add that are interpreted as
    PIP problem parameters. These are added \e after having added the
    \p m_vars problem variables.

    \exception std::length_error
    Thrown if adding <CODE>m_vars + m_params</CODE> new space
    dimensions would cause the vector space to exceed dimension
    <CODE>max_space_dimension()</CODE>.

    The new space dimensions will be those having the highest indexes
    in the new PIP problem; they are initially unconstrained.
  */
  void add_space_dimensions_and_embed(dimension_type m_vars,
                                      dimension_type m_params);

  /*! \brief
    Sets the space dimensions whose indexes which are in set \p p_vars
    to be parameter space dimensions.

    \exception std::invalid_argument
    Thrown if some index in \p p_vars does not correspond to
    a space dimension in \p *this.
  */
  void add_to_parameter_space_dimensions(const Variables_Set& p_vars);

  /*! \brief
    Adds a copy of constraint \p c to the PIP problem.

    \exception std::invalid_argument
    Thrown if the space dimension of \p c is strictly greater than
    the space dimension of \p *this.
  */
  void add_constraint(const Constraint& c);

  /*! \brief
    Adds a copy of the constraints in \p cs to the PIP problem.

    \exception std::invalid_argument
    Thrown if the space dimension of constraint system \p cs is strictly
    greater than the space dimension of \p *this.
  */
  void add_constraints(const Constraint_System& cs);

  //! Checks satisfiability of \p *this.
  /*!
    \return
    \c true if and only if the PIP problem is satisfiable.
  */
  bool is_satisfiable() const;

  //! Optimizes the PIP problem.
  /*!
    \return
    A PIP_Problem_Status flag indicating the outcome of the optimization
    attempt (unfeasible or optimized problem).
  */
  PIP_Problem_Status solve() const;

  //! Returns a feasible solution for \p *this, if it exists.
  /*!
    A null pointer is returned for an unfeasible PIP problem.
  */
  PIP_Tree solution() const;

  //! Returns an optimizing solution for \p *this, if it exists.
  /*!
    A null pointer is returned for an unfeasible PIP problem.
  */
  PIP_Tree optimizing_solution() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  //! Prints on \p s the solution computed for \p *this.
  /*!
    \param s
    The output stream.

    \param indent
    An indentation parameter (default value 0).

    \exception std::logic_error
    Thrown if trying to print the solution when the PIP problem
    still has to be solved.
  */
  void print_solution(std::ostream& s, int indent = 0) const;

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Swaps \p *this with \p y.
  void m_swap(PIP_Problem& y);

  //! Possible names for PIP_Problem control parameters.
  enum Control_Parameter_Name {
    //! Cutting strategy
    CUTTING_STRATEGY,
    //! Pivot row strategy
    PIVOT_ROW_STRATEGY,
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
    //! Number of different enumeration values.
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
    CONTROL_PARAMETER_NAME_SIZE
  };

  //! Possible values for PIP_Problem control parameters.
  enum Control_Parameter_Value {
    //! Choose the first non-integer row.
    CUTTING_STRATEGY_FIRST,
    //! Choose row which generates the deepest cut.
    CUTTING_STRATEGY_DEEPEST,
    //! Always generate all possible cuts.
    CUTTING_STRATEGY_ALL,

    //! Choose the first row with negative parameter sign.
    PIVOT_ROW_STRATEGY_FIRST,
    //! Choose a row that generates a lexicographically maximal pivot column.
    PIVOT_ROW_STRATEGY_MAX_COLUMN,

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
    //! Number of different enumeration values.
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
    CONTROL_PARAMETER_VALUE_SIZE
  };

  //! Returns the value of control parameter \p name.
  Control_Parameter_Value
  get_control_parameter(Control_Parameter_Name name) const;

  //! Sets control parameter \p value.
  void set_control_parameter(Control_Parameter_Value value);

  //! Sets the dimension for the big parameter to \p big_dim.
  void set_big_parameter_dimension(dimension_type big_dim);

  /*! \brief
    Returns the space dimension for the big parameter.

    If a big parameter was not set, returns \c not_a_dimension().
  */
  dimension_type get_big_parameter_dimension() const;

private:
  //! Initializes the control parameters with default values.
  void control_parameters_init();

  //! Copies the control parameters from problem object \p y.
  void control_parameters_copy(const PIP_Problem& y);

  //! The dimension of the vector space.
  dimension_type external_space_dim;

  /*! \brief
    The space dimension of the current (partial) solution of the
    PIP problem; it may be smaller than \p external_space_dim.
  */
  dimension_type internal_space_dim;

  //! An enumerated type describing the internal status of the PIP problem.
  enum Status {
    //! The PIP problem is unsatisfiable.
    UNSATISFIABLE,
    //! The PIP problem is optimized; the solution tree has been computed.
    OPTIMIZED,
    /*! \brief
      The feasible region of the PIP problem has been changed by adding
      new variables, parameters or constraints; a feasible solution for
      the old feasible region is still available.
    */
    PARTIALLY_SATISFIABLE
  };

  //! The internal state of the MIP problem.
  Status status;

  //! The current solution decision tree
  PIP_Tree_Node* current_solution;

  //! The sequence of constraints describing the feasible region.
  Constraint_Sequence input_cs;

  //! The first index of `input_cs' containing a pending constraint.
  dimension_type first_pending_constraint;

  /*! \brief
    A set containing all the indices of space dimensions that are
    interpreted as problem parameters.
  */
  Variables_Set parameters;

#if PPL_USE_SPARSE_MATRIX
  typedef Sparse_Row Row;
#else
  typedef Dense_Row Row;
#endif

  /*! \brief
    The initial context

    Contains problem constraints on parameters only
  */
  Matrix<Row> initial_context;

  //! The control parameters for the problem object.
  Control_Parameter_Value
  control_parameters[CONTROL_PARAMETER_NAME_SIZE];

  /*! \brief
    The dimension for the big parameter, or \c not_a_dimension()
    if not set.
  */
  dimension_type big_parameter_dimension;

  friend class PIP_Solution_Node;
};

#include "PIP_Problem_inlines.hh"
#include "PIP_Problem_templates.hh"

#endif // !defined(PPL_PIP_Problem_defs_hh)
