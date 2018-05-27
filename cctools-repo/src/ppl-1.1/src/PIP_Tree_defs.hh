/* PIP_Tree_Node class declaration.
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

#ifndef PPL_PIP_Tree_defs_hh
#define PPL_PIP_Tree_defs_hh 1

#include "PIP_Tree_types.hh"
#include "Variable_defs.hh"
#include "Linear_Expression_types.hh"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "Constraint_defs.hh"
#include "Variables_Set_defs.hh"
#include "globals_defs.hh"
#include "PIP_Problem_defs.hh"

#include "Matrix_defs.hh"
#include "Dense_Row_defs.hh"
#include "Sparse_Row_defs.hh"

namespace Parma_Polyhedra_Library {

//! A node of the PIP solution tree.
/*!
  This is the base class for the nodes of the binary trees representing
  the solutions of PIP problems. From this one, two classes are derived:
    - PIP_Decision_Node, for the internal nodes of the tree;
    - PIP_Solution_Node, for the leaves of the tree.
*/
class PIP_Tree_Node {
protected:
  //! Constructor: builds a node owned by \p *owner.
  explicit PIP_Tree_Node(const PIP_Problem* owner);

  //! Copy constructor.
  PIP_Tree_Node(const PIP_Tree_Node& y);

  //! Returns a pointer to the PIP_Problem owning object.
  const PIP_Problem* get_owner() const;

  //! Sets the pointer to the PIP_Problem owning object.
  virtual void set_owner(const PIP_Problem* owner) = 0;

  /*! \brief
    Returns \c true if and only if all the nodes in the subtree
    rooted in \p *this are owned by \p *owner.
  */
  virtual bool check_ownership(const PIP_Problem* owner) const = 0;

public:
#if PPL_USE_SPARSE_MATRIX
  typedef Sparse_Row Row;
#else
  typedef Dense_Row Row;
#endif

  //! Returns a pointer to a dynamically-allocated copy of \p *this.
  virtual PIP_Tree_Node* clone() const = 0;

  //! Destructor.
  virtual ~PIP_Tree_Node();

  //! Returns \c true if and only if \p *this is well formed.
  virtual bool OK() const = 0;

  //! Returns \p this if \p *this is a solution node, 0 otherwise.
  virtual const PIP_Solution_Node* as_solution() const = 0;

  //! Returns \p this if \p *this is a decision node, 0 otherwise.
  virtual const PIP_Decision_Node* as_decision() const = 0;

  /*! \brief
    Returns the system of parameter constraints controlling \p *this.

    The indices in the constraints are the same as the original variables and
    parameters. Coefficients in indices corresponding to variables always are
    zero.
  */
  const Constraint_System& constraints() const;

  class Artificial_Parameter;

  //! A type alias for a sequence of Artificial_Parameter's.
  typedef std::vector<Artificial_Parameter> Artificial_Parameter_Sequence;

  //! Returns a const_iterator to the beginning of local artificial parameters.
  Artificial_Parameter_Sequence::const_iterator art_parameter_begin() const;

  //! Returns a const_iterator to the end of local artificial parameters.
  Artificial_Parameter_Sequence::const_iterator art_parameter_end() const;

  //! Returns the number of local artificial parameters.
  dimension_type art_parameter_count() const;

  //! Prints on \p s the tree rooted in \p *this.
  /*!
    \param s
    The output stream.

    \param indent
    The amount of indentation.
  */
  void print(std::ostream& s, int indent = 0) const;

  //! Dumps to \p s an ASCII representation of \p *this.
  void ascii_dump(std::ostream& s) const;

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  virtual memory_size_type total_memory_in_bytes() const = 0;
  //! Returns the size in bytes of the memory managed by \p *this.
  virtual memory_size_type external_memory_in_bytes() const = 0;

protected:
  //! A type alias for a sequence of constraints.
  typedef std::vector<Constraint> Constraint_Sequence;

  // Only PIP_Problem and PIP_Decision_Node are allowed to use the
  // constructor and methods.
  friend class PIP_Problem;
  friend class PIP_Decision_Node;
  friend class PIP_Solution_Node;

  //! A pointer to the PIP_Problem object owning this node.
  const PIP_Problem* owner_;

  //! A pointer to the parent of \p *this, null if \p *this is the root.
  const PIP_Decision_Node* parent_;

  //! The local system of parameter constraints.
  Constraint_System constraints_;

  //! The local sequence of expressions for local artificial parameters.
  Artificial_Parameter_Sequence artificial_parameters;

  //! Returns a pointer to this node's parent.
  const PIP_Decision_Node* parent() const;

  //! Set this node's parent to \p *p.
  void set_parent(const PIP_Decision_Node* p);

  /*! \brief
    Populates the parametric simplex tableau using external data.

    \param pip
    The PIP_Problem object containing this node.

    \param external_space_dim
    The number of all problem variables and problem parameters
    (excluding artificial parameters).

    \param first_pending_constraint
    The first element in \p input_cs to be added to the tableau,
    which already contains the previous elements.

    \param input_cs
    All the constraints of the PIP problem.

    \param parameters
    The set of indices of the problem parameters.
  */
  virtual void update_tableau(const PIP_Problem& pip,
                              dimension_type external_space_dim,
                              dimension_type first_pending_constraint,
                              const Constraint_Sequence& input_cs,
                              const Variables_Set& parameters) = 0;

  /*! \brief
    Executes a parametric simplex on the tableau, under specified context.

    \return
    The root of the PIP tree solution, or 0 if unfeasible.

    \param pip
    The PIP_Problem object containing this node.

    \param check_feasible_context
    Whether the resolution process should (re-)check feasibility of
    context (since the initial context may have been modified).

    \param context
    The context, being a set of constraints on the parameters.

    \param params
    The local parameter set, including parent's artificial parameters.

    \param space_dim
    The space dimension of parent, including artificial parameters.

    \param indent_level
    The indentation level (for debugging output only).
  */
  virtual PIP_Tree_Node* solve(const PIP_Problem& pip,
                               bool check_feasible_context,
                               const Matrix<Row>& context,
                               const Variables_Set& params,
                               dimension_type space_dim,
                               int indent_level) = 0;

  //! Inserts a new parametric constraint in internal row format.
  void add_constraint(const Row& row, const Variables_Set& parameters);

  //! Merges parent's artificial parameters into \p *this.
  void parent_merge();

  //! Prints on \p s the tree rooted in \p *this.
  /*!
    \param s
    The output stream.

    \param indent
    The amount of indentation.

    \param pip_dim_is_param
    A vector of Boolean flags telling which PIP problem dimensions are
    problem parameters. The size of the vector is equal to the PIP
    problem internal space dimension (i.e., no artificial parameters).

    \param first_art_dim
    The first space dimension corresponding to an artificial parameter
    that was created in this node (if any).
  */
  virtual void print_tree(std::ostream& s,
                          int indent,
                          const std::vector<bool>& pip_dim_is_param,
                          dimension_type first_art_dim) const = 0;

  //! A helper function used when printing PIP trees.
  static void
  indent_and_print(std::ostream& s, int indent, const char* str);

  /*! \brief
    Checks whether a context matrix is satisfiable.

    The satisfiability check is implemented by the revised dual simplex
    algorithm on the context matrix. The algorithm ensures the feasible
    solution is integer by applying a cut generation method when
    intermediate non-integer solutions are found.
  */
  static bool compatibility_check(Matrix<Row>& s);

  /*! \brief
    Helper method: checks for satisfiability of the restricted context
    obtained by adding \p row to \p context.
  */
  static bool compatibility_check(const Matrix<Row>& context, const Row& row);

}; // class PIP_Tree_Node


/*! \brief
  Artificial parameters in PIP solution trees.

  These parameters are built from a linear expression combining other
  parameters (constant term included) divided by a positive integer
  denominator. Coefficients at variables indices corresponding to
  PIP problem variables are always zero.
*/
class PIP_Tree_Node::Artificial_Parameter
  : public Linear_Expression {
public:
  //! Default constructor: builds a zero artificial parameter.
  Artificial_Parameter();

  //! Constructor.
  /*!
    Builds artificial parameter \f$\frac{\mathtt{expr}}{\mathtt{d}}\f$.

    \param expr
    The expression that, after normalization, will form the numerator of
    the artificial parameter.

    \param d
    The integer constant that, after normalization, will form the
    denominator of the artificial parameter.

    \exception std::invalid_argument
    Thrown if \p d is zero.

    Normalization will ensure that the denominator is positive.
  */
  Artificial_Parameter(const Linear_Expression& expr,
                       Coefficient_traits::const_reference d);

  //! Copy constructor.
  Artificial_Parameter(const Artificial_Parameter& y);

  //! Returns the normalized (i.e., positive) denominator.
  Coefficient_traits::const_reference denominator() const;

  //! Swaps \p *this with \p y.
  void m_swap(Artificial_Parameter& y);

  //! Returns \c true if and only if \p *this and \p y are equal.
  /*!
    Note that two artificial parameters having different space dimensions
    are considered to be different.
  */
  bool operator==(const Artificial_Parameter& y) const;
  //! Returns \c true if and only if \p *this and \p y are different.
  bool operator!=(const Artificial_Parameter& y) const;

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

  //! Returns \c true if and only if the parameter is well-formed.
  bool OK() const;

private:
  //! The normalized (i.e., positive) denominator.
  Coefficient denom;
}; // class PIP_Tree_Node::Artificial_Parameter


//! Swaps \p x with \p y.
/*! \relates PIP_Tree_Node::Artificial_Parameter */
void
swap(PIP_Tree_Node::Artificial_Parameter& x,
     PIP_Tree_Node::Artificial_Parameter& y);


//! A tree node representing part of the space of solutions.
class PIP_Solution_Node : public PIP_Tree_Node {
public:

  //! Constructor: builds a solution node owned by \p *owner.
  explicit PIP_Solution_Node(const PIP_Problem* owner);

  //! Returns a pointer to a dynamically-allocated copy of \p *this.
  virtual PIP_Tree_Node* clone() const;

  //! Destructor.
  virtual ~PIP_Solution_Node();

  //! Returns \c true if and only if \p *this is well formed.
  virtual bool OK() const;

  //! Returns \p this.
  virtual const PIP_Solution_Node* as_solution() const;

  //! Returns 0, since \p this is not a decision node.
  virtual const PIP_Decision_Node* as_decision() const;

  /*! \brief
    Returns a parametric expression for the values of problem variable \p var.

    The returned linear expression may involve problem parameters
    as well as artificial parameters.

    \param var
    The problem variable which is queried about.

    \exception std::invalid_argument
    Thrown if \p var is dimension-incompatible with the PIP_Problem
    owning this solution node, or if \p var is a problem parameter.
  */
  const Linear_Expression& parametric_values(Variable var) const;

  //! Dumps to \p os an ASCII representation of \p *this.
  void ascii_dump(std::ostream& os) const;

  /*! \brief
    Loads from \p is an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& is);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  virtual memory_size_type total_memory_in_bytes() const;
  //! Returns the size in bytes of the memory managed by \p *this.
  virtual memory_size_type external_memory_in_bytes() const;

private:
  //! The type for parametric simplex tableau.
  struct Tableau {
    //! The matrix of simplex coefficients.
    Matrix<Row> s;
    //! The matrix of parameter coefficients.
    Matrix<Row> t;
    //! A common denominator for all matrix elements
    Coefficient denom;

    //! Default constructor.
    Tableau();
    //! Copy constructor.
    Tableau(const Tableau& y);
    //! Destructor.
    ~Tableau();

    //! Tests whether the matrix is integer, i.e., the denominator is 1.
    bool is_integer() const;

    //! Multiplies all coefficients and denominator with ratio.
    void scale(Coefficient_traits::const_reference ratio);

    //! Normalizes the modulo of coefficients so that they are mutually prime.
    /*!
      Computes the Greatest Common Divisor (GCD) among the elements of
      the matrices and normalizes them and the denominator by the GCD itself.
    */
    void normalize();

    /*! \brief
      Compares two pivot row and column pairs before pivoting.

      The algorithm searches the first (ie, leftmost) column \f$k\f$ in
      parameter matrix for which the \f$c=s_{*j}\frac{t_{ik}}{s_{ij}}\f$
      and \f$c'=s_{*j'}\frac{t_{i'k}}{s_{i'j'}}\f$ columns are different,
      where \f$s_{*j}\f$ denotes the \f$j\f$<sup>th</sup> column from the
      \f$s\f$ matrix and \f$s_{*j'}\f$ is the \f$j'\f$<sup>th</sup> column
      of \f$s\f$.

      \f$c\f$ is the computed column that would be subtracted to column
      \f$k\f$ in parameter matrix if pivoting is done using the \f$(i,j)\f$
      row and column pair.
      \f$c'\f$ is the computed column that would be subtracted to column
      \f$k\f$ in parameter matrix if pivoting is done using the
      \f$(i',j')\f$ row and column pair.

      The test is true if the computed \f$-c\f$ column is lexicographically
      bigger than the \f$-c'\f$ column. Due to the column ordering in the
      parameter matrix of the tableau, leftmost search will enforce solution
      increase with respect to the following priority order:
       - the constant term
       - the coefficients for the original parameters
       - the coefficients for the oldest artificial parameters.

      \return
      \c true if pivot row and column pair \f$(i,j)\f$ is more
      suitable for pivoting than the \f$(i',j')\f$ pair

      \param mapping
      The PIP_Solution_Node::mapping vector for the tableau.

      \param basis
      The PIP_Solution_Node::basis vector for the tableau.

      \param row_0
      The row number for the first pivot row and column pair to be compared.

      \param col_0
      The column number for the first pivot row and column pair to be
      compared.

      \param row_1
      The row number for the second pivot row and column pair to be compared.

      \param col_1
      The column number for the second pivot row and column pair to be
      compared.
    */
    bool is_better_pivot(const std::vector<dimension_type>& mapping,
                         const std::vector<bool>& basis,
                         const dimension_type row_0,
                         const dimension_type col_0,
                         const dimension_type row_1,
                         const dimension_type col_1) const;

    //! Returns the value of the denominator.
    Coefficient_traits::const_reference denominator() const;

    //! Dumps to \p os an ASCII representation of \p *this.
    void ascii_dump(std::ostream& os) const;

    /*! \brief
      Loads from \p is an ASCII representation (as produced by
      ascii_dump(std::ostream&) const) and sets \p *this accordingly.
      Returns \c true if successful, \c false otherwise.
    */
    bool ascii_load(std::istream& is);

    //! Returns the size in bytes of the memory managed by \p *this.
    /*!
      \note
      No need for a \c total_memory_in_bytes() method, since
      class Tableau is a private inner class of PIP_Solution_Node.
    */
    memory_size_type external_memory_in_bytes() const;

    //! Returns \c true if and only if \p *this is well formed.
    bool OK() const;
  }; // struct Tableau

  //! The parametric simplex tableau.
  Tableau tableau;

  /*! \brief
    A boolean vector for identifying the basic variables.

    Variable identifiers are numbered from 0 to <CODE>n+m-1</CODE>, where \p n
    is the number of columns in the simplex tableau corresponding to variables,
    and \p m is the number of rows.

    Indices from 0 to <CODE>n-1</CODE> correspond to the original variables.

    Indices from \p n to <CODE>n+m-1</CODE> correspond to the slack variables
    associated to the internal constraints, which do not strictly correspond
    to original constraints, since these may have been transformed to fit the
    standard form of the dual simplex.

    The value for <CODE>basis[i]</CODE> is:
     - \b true if variable \p i is basic,
     - \b false if variable \p i is nonbasic.
  */
  std::vector<bool> basis;

  /*! \brief
    A mapping between the tableau rows/columns and the original variables.

    The value of <CODE>mapping[i]</CODE> depends of the value of <CODE>basis[i]</CODE>.

     - If <CODE>basis[i]</CODE> is \b true, <CODE>mapping[i]</CODE> encodes the column
       index of variable \p i in the \p s matrix of the tableau.
     - If <CODE>basis[i]</CODE> is \b false, <CODE>mapping[i]</CODE> encodes the row
       index of variable \p i in the tableau.
  */
  std::vector<dimension_type> mapping;

  /*! \brief
    The variable identifiers associated to the rows of the simplex tableau.
  */
  std::vector<dimension_type> var_row;

  /*! \brief
    The variable identifiers associated to the columns of the simplex tableau.
  */
  std::vector<dimension_type> var_column;

  /*! \brief
    The variable number of the special inequality used for modeling
    equality constraints.

    The subset of equality constraints in a specific problem can be expressed
    as: \f$f_i(x,p) = 0 ; 1 \leq i \leq n\f$. As the dual simplex standard form
    requires constraints to be inequalities, the following constraints can be
    modeled as follows:

     - \f$f_i(x,p) \geq 0 ; 1 \leq i \leq n\f$

     - \f$\sum\limits_{i=1}^n f_i(x,p) \leq 0\f$

    The \p special_equality_row value stores the variable number of the
    specific constraint which is used to model the latter sum of
    constraints. If no such constraint exists, the value is set to \p 0.
  */
  dimension_type special_equality_row;

  /*! \brief
    The column index in the parametric part of the simplex tableau
    corresponding to the big parameter; \c not_a_dimension() if not set.
  */
  dimension_type big_dimension;

  //! The possible values for the sign of a parametric linear expression.
  enum Row_Sign {
    //! Not computed yet (default).
    UNKNOWN,
    //! All row coefficients are zero.
    ZERO,
    //! All nonzero row coefficients are positive.
    POSITIVE,
    //! All nonzero row coefficients are negative.
    NEGATIVE,
    //! The row contains both positive and negative coefficients.
    MIXED
  };

  //! A cache for computed sign values of constraint parametric RHS.
  std::vector<Row_Sign> sign;

  //! Parametric values for the solution.
  std::vector<Linear_Expression> solution;

  //! An indicator for solution validity.
  bool solution_valid;

  //! Returns the sign of row \p x.
  static Row_Sign row_sign(const Row& x,
                           dimension_type big_dimension);

protected:
  //! Copy constructor.
  PIP_Solution_Node(const PIP_Solution_Node& y);

  //! A tag type to select the alternative copy constructor.
  struct No_Constraints {};

  //! Alternative copy constructor.
  /*!
    This constructor differs from the default copy constructor in that
    it will not copy the constraint system, nor the artificial parameters.
  */
  PIP_Solution_Node(const PIP_Solution_Node& y, No_Constraints);

  // PIP_Problem::ascii load() method needs access set_owner().
  friend bool PIP_Problem::ascii_load(std::istream& s);

  //! Sets the pointer to the PIP_Problem owning object.
  virtual void set_owner(const PIP_Problem* owner);

  /*! \brief
    Returns \c true if and only if all the nodes in the subtree
    rooted in \p *this is owned by \p *pip.
  */
  virtual bool check_ownership(const PIP_Problem* owner) const;

  //! Implements pure virtual method PIP_Tree_Node::update_tableau.
  virtual void update_tableau(const PIP_Problem& pip,
                              dimension_type external_space_dim,
                              dimension_type first_pending_constraint,
                              const Constraint_Sequence& input_cs,
                              const Variables_Set& parameters);

  /*! \brief
    Update the solution values.

    \param pip_dim_is_param
    A vector of Boolean flags telling which PIP problem dimensions are
    problem parameters. The size of the vector is equal to the PIP
    problem internal space dimension (i.e., no artificial parameters).
  */
  void update_solution(const std::vector<bool>& pip_dim_is_param) const;

  //! Helper method.
  void update_solution() const;

  //! Implements pure virtual method PIP_Tree_Node::solve.
  virtual PIP_Tree_Node* solve(const PIP_Problem& pip,
                               bool check_feasible_context,
                               const Matrix<Row>& context,
                               const Variables_Set& params,
                               dimension_type space_dim,
                               int indent_level);

  /*! \brief
    Generate a Gomory cut using non-integer tableau row \p index.

    \param index
    Row index in simplex tableau from which the cut is generated.

    \param parameters
    A std::set of the current parameter dimensions (including artificials);
    to be updated if a new artificial parameter is to be created.

    \param context
    A set of linear inequalities on the parameters, in matrix form; to be
    updated if a new artificial parameter is to be created.

    \param space_dimension
    The current space dimension, including variables and all parameters; to
    be updated if an extra parameter is to be created.

    \param indent_level
    The indentation level (for debugging output only).
  */
  void generate_cut(dimension_type index, Variables_Set& parameters,
                    Matrix<Row>& context, dimension_type& space_dimension,
                    int indent_level);

  //! Prints on \p s the tree rooted in \p *this.
  virtual void print_tree(std::ostream& s, int indent,
                          const std::vector<bool>& pip_dim_is_param,
                          dimension_type first_art_dim) const;

}; // class PIP_Solution_Node


//! A tree node representing a decision in the space of solutions.
class PIP_Decision_Node : public PIP_Tree_Node {
public:
  //! Returns a pointer to a dynamically-allocated copy of \p *this.
  virtual PIP_Tree_Node* clone() const;

  //! Destructor.
  virtual ~PIP_Decision_Node();

  //! Returns \c true if and only if \p *this is well formed.
  virtual bool OK() const;

  //! Returns \p this.
  virtual const PIP_Decision_Node* as_decision() const;

  //! Returns 0, since \p this is not a solution node.
  virtual const PIP_Solution_Node* as_solution() const;

  //! Returns a const pointer to the \p b (true or false) branch of \p *this.
  const PIP_Tree_Node* child_node(bool b) const;

  //! Returns a pointer to the \p b (true or false) branch of \p *this.
  PIP_Tree_Node* child_node(bool b);

  //! Dumps to \p s an ASCII representation of \p *this.
  void ascii_dump(std::ostream& s) const;

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  virtual memory_size_type total_memory_in_bytes() const;
  //! Returns the size in bytes of the memory managed by \p *this.
  virtual memory_size_type external_memory_in_bytes() const;

private:
  // PIP_Solution_Node is allowed to use the constructor and methods.
  friend class PIP_Solution_Node;

  // PIP_Problem ascii load method needs access to private constructors.
  friend bool PIP_Problem::ascii_load(std::istream& s);

  //! Pointer to the "false" child of \p *this.
  PIP_Tree_Node* false_child;

  //! Pointer to the "true" child of \p *this.
  PIP_Tree_Node* true_child;

  /*! \brief
    Builds a decision node having \p fcp and \p tcp as child.

    The decision node will encode the structure
    "if \c cs then \p tcp else \p fcp",
    where the system of constraints \c cs is initially empty.

    \param owner
    Pointer to the owning PIP_Problem object; it may be null if and
    only if both children are null.

    \param fcp
    Pointer to "false" child; it may be null.

    \param tcp
    Pointer to "true" child; it may be null.

    \note
    If any of \p fcp or \p tcp is not null, then \p owner is required
    to be not null and equal to the owner of its non-null children;
    otherwise the behavior is undefined.
  */
  explicit PIP_Decision_Node(const PIP_Problem* owner,
                             PIP_Tree_Node* fcp,
                             PIP_Tree_Node* tcp);

  //! Sets the pointer to the PIP_Problem owning object.
  virtual void set_owner(const PIP_Problem* owner);

  /*! \brief
    Returns \c true if and only if all the nodes in the subtree
    rooted in \p *this is owned by \p *pip.
  */
  virtual bool check_ownership(const PIP_Problem* owner) const;

protected:
  //! Copy constructor.
  PIP_Decision_Node(const PIP_Decision_Node& y);

  //! Implements pure virtual method PIP_Tree_Node::update_tableau.
  virtual void update_tableau(const PIP_Problem& pip,
                              dimension_type external_space_dim,
                              dimension_type first_pending_constraint,
                              const Constraint_Sequence& input_cs,
                              const Variables_Set& parameters);

  //! Implements pure virtual method PIP_Tree_Node::solve.
  virtual PIP_Tree_Node* solve(const PIP_Problem& pip,
                               bool check_feasible_context,
                               const Matrix<Row>& context,
                               const Variables_Set& params,
                               dimension_type space_dim,
                               int indent_level);

  //! Prints on \p s the tree rooted in \p *this.
  virtual void print_tree(std::ostream& s, int indent,
                          const std::vector<bool>& pip_dim_is_param,
                          dimension_type first_art_dim) const;

}; // class PIP_Decision_Node

namespace IO_Operators {

//! Output operator: prints the solution tree rooted in \p x.
/*! \relates Parma_Polyhedra_Library::PIP_Tree_Node */
std::ostream& operator<<(std::ostream& os, const PIP_Tree_Node& x);

//! Output operator.
/*! \relates Parma_Polyhedra_Library::PIP_Tree_Node::Artificial_Parameter */
std::ostream& operator<<(std::ostream& os,
                         const PIP_Tree_Node::Artificial_Parameter& x);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

#include "PIP_Tree_inlines.hh"

#endif // !defined(PPL_PIP_Tree_defs_hh)
