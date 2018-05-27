/* Domain-independent part of the Java interface: declarations.
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

#ifndef PPL_ppl_java_common_defs_hh
#define PPL_ppl_java_common_defs_hh 1

#define PPL_NO_AUTOMATIC_INITIALIZATION
#include "ppl.hh"

#include <jni.h>
#include "interfaced_boxes.hh"
#include "marked_pointers.hh"

#define PPL_JAVA_UNEXPECTED \
  throw std::runtime_error("PPL Java interface internal error")

#define CATCH_ALL \
  catch(const Java_ExceptionOccurred& e) { \
  } \
  catch(const std::overflow_error& e) { \
    handle_exception(env, e); \
  } \
  catch(const std::length_error& e) { \
    handle_exception(env, e); \
  } \
  catch (const std::bad_alloc& e) { \
    handle_exception(env, e); \
  } \
  catch (const std::domain_error& e) { \
    handle_exception(env, e); \
  } \
 catch (const std::invalid_argument& e) { \
    handle_exception(env, e); \
  } \
 catch(const std::logic_error& e) { \
    handle_exception(env, e); \
  } \
  catch (const std::exception& e) { \
    handle_exception(env, e); \
  } \
  catch (const timeout_exception& e) { \
    handle_exception(env, e); \
  } \
  catch (const deterministic_timeout_exception& e) { \
    handle_exception(env, e); \
  } \
  catch (...) { \
    handle_exception(env); \
  }

#define CHECK_EXCEPTION_ASSERT(env)      \
  assert(!env->ExceptionOccurred())
#define CHECK_EXCEPTION_THROW(env)       \
  do {                                   \
    if (env->ExceptionOccurred())        \
      throw Java_ExceptionOccurred();    \
  } while (0)
#define CHECK_EXCEPTION_RETURN(env, val) \
  do {                                   \
    if (env->ExceptionOccurred())        \
      return val;                        \
  } while (0)
#define CHECK_EXCEPTION_RETURN_VOID(env) \
  do {                                   \
    if (env->ExceptionOccurred())        \
      return;                            \
  } while (0)
#define CHECK_RESULT_ABORT(env, cond)	 \
  do {                                   \
    if (!(cond))			 \
      abort();                           \
  } while (0)
#define CHECK_RESULT_ASSERT(env, cond)   \
  assert(cond)
#define CHECK_RESULT_THROW(env, cond)    \
  do {                                   \
    if (!(cond))			 \
      throw Java_ExceptionOccurred();    \
  } while (0)
#define CHECK_RESULT_RETURN(env, cond, val)   \
  do {                                        \
    if (!(cond))			      \
      return val;                             \
  } while (0)
#define CHECK_RESULT_RETURN_VOID(env, cond)   \
  do {                                        \
    if (!(cond))			      \
      return;                                 \
  } while (0)


namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace Java {

struct Java_ExceptionOccurred : public std::exception {
};

class timeout_exception
  : public Parma_Polyhedra_Library::Throwable {
public:
  void throw_me() const {
    throw *this;
  }
  int priority() const {
    return 0;
  }
};

class deterministic_timeout_exception
  : public Parma_Polyhedra_Library::Throwable {
public:
  void throw_me() const {
    throw *this;
  }
  int priority() const {
    return 0;
  }
};

extern Parma_Polyhedra_Library::Watchdog* p_timeout_object;

typedef
Parma_Polyhedra_Library::Threshold_Watcher
<Parma_Polyhedra_Library::Weightwatch_Traits> Weightwatch;

extern Weightwatch* p_deterministic_timeout_object;

void reset_timeout();

void reset_deterministic_timeout();

void
handle_exception(JNIEnv* env, const std::logic_error& e);

void
handle_exception(JNIEnv* env, const std::invalid_argument& e);

void
handle_exception(JNIEnv* env, const std::domain_error& e);

void
handle_exception(JNIEnv* env, const std::overflow_error& e);

void
handle_exception(JNIEnv* env, const std::length_error& e);

void
handle_exception(JNIEnv* env, const std::bad_alloc&);

void
handle_exception(JNIEnv* env, const std::exception& e);

void
handle_exception(JNIEnv* env, const timeout_exception& e);

void
handle_exception(JNIEnv* env, const deterministic_timeout_exception& e);

void
handle_exception(JNIEnv* env);

//! A cache for global references to Java classes.
/*!
  The cache is loaded by \c Parma_Polyhedra_Library.initialize_library();
  It is cleared by \c Parma_Polyhedra_Library.finalize_library().
*/
class Java_Class_Cache {
public:
  // Non-PPL types.
  jclass Boolean;
  jclass Integer;
  jclass Long;
  jclass Iterator;
  // PPL types.
  jclass Artificial_Parameter;
  jclass Artificial_Parameter_Sequence;
  jclass Bounded_Integer_Type_Overflow;
  jclass Bounded_Integer_Type_Representation;
  jclass Bounded_Integer_Type_Width;
  jclass By_Reference;
  jclass Coefficient;
  jclass Congruence;
  jclass Constraint;
  jclass Generator;
  jclass Grid_Generator;
  jclass Generator_Type;
  jclass Grid_Generator_Type;
  jclass Constraint_System;
  jclass Congruence_System;
  jclass Generator_System;
  jclass Grid_Generator_System;
  jclass Linear_Expression;
  jclass Linear_Expression_Coefficient;
  jclass Linear_Expression_Difference;
  jclass Linear_Expression_Sum;
  jclass Linear_Expression_Times;
  jclass Linear_Expression_Unary_Minus;
  jclass Linear_Expression_Variable;
  jclass MIP_Problem_Status;
  jclass Optimization_Mode;
  jclass Pair;
  jclass PIP_Problem_Control_Parameter_Name;
  jclass PIP_Problem_Control_Parameter_Value;
  jclass PIP_Problem_Status;
  jclass Poly_Con_Relation;
  jclass Poly_Gen_Relation;
  jclass PPL_Object;
  jclass Relation_Symbol;
  jclass Variable;
  jclass Variables_Set;

  //! Default constructor.
  Java_Class_Cache();

  //! Initializes all cache fields.
  void init_cache(JNIEnv* env);

  //! Resets all fields to \c NULL.
  void clear_cache(JNIEnv* env);

private:
  //! Sets \p field to a global reference to Java class called \p name.
  void init_cache(JNIEnv* env, jclass& field, const char* name);

  //! Resets \p field to \c NULL, deleting the global reference (if any).
  void clear_cache(JNIEnv* env, jclass& field);

  // Private and not implemented: copy construction not allowed.
  Java_Class_Cache(const Java_Class_Cache&);
  // Private and not implemented: copy assignment not allowed.
  Java_Class_Cache& operator=(const Java_Class_Cache&);
};

//! A cache for field and method IDs of Java classes.
/*!
  The IDs for fields and methods of PPL Java classes are automatically
  by the static initializer of the corresponding Java class.
  The static initializers of some PPL Java class also stores the IDs
  for fields and methods of non-PPL classes (e.g., Boolean, Long, etc.).
*/
struct Java_FMID_Cache {
  // Non-PPL type method IDs: stored when loading Coefficient.
  jmethodID Boolean_valueOf_ID;
  jmethodID Boolean_boolValue_ID;
  jmethodID Integer_valueOf_ID;
  jmethodID Integer_intValue_ID;
  jmethodID Long_valueOf_ID;
  jmethodID Long_longValue_ID;

  // PPL type field and method IDs.
  // Artificial_Parameter.
  jfieldID Artificial_Parameter_le_ID;
  jfieldID Artificial_Parameter_den_ID;
  jmethodID Artificial_Parameter_init_ID;
  jmethodID Artificial_Parameter_Sequence_init_ID;
  jmethodID Artificial_Parameter_Sequence_add_ID;
  // Bounded_Integer_Type_Overflow.
  jfieldID Bounded_Integer_Type_Overflow_OVERFLOW_WRAPS_ID;
  jfieldID Bounded_Integer_Type_Overflow_OVERFLOW_UNDEFINED_ID;
  jfieldID Bounded_Integer_Type_Overflow_OVERFLOW_IMPOSSIBLE_ID;
  jmethodID Bounded_Integer_Type_Overflow_ordinal_ID;
  // Bounded_Integer_Type_Representation.
  jfieldID Bounded_Integer_Type_Representation_UNSIGNED_ID;
  jfieldID Bounded_Integer_Type_Representation_SIGNED_2_COMPLEMENT_ID;
  jmethodID Bounded_Integer_Type_Representation_ordinal_ID;
  // Bounded_Integer_Type_Width.
  jfieldID Bounded_Integer_Type_Width_BITS_8_ID;
  jfieldID Bounded_Integer_Type_Width_BITS_16_ID;
  jfieldID Bounded_Integer_Type_Width_BITS_32_ID;
  jfieldID Bounded_Integer_Type_Width_BITS_64_ID;
  jfieldID Bounded_Integer_Type_Width_BITS_128_ID;
  jmethodID Bounded_Integer_Type_Width_ordinal_ID;
  // By_Reference.
  jfieldID By_Reference_obj_ID;
  jmethodID By_Reference_init_ID;
  // Coefficient.
  jfieldID Coefficient_value_ID;
  jmethodID Coefficient_init_from_String_ID;
  jmethodID Coefficient_toString_ID;
  // Complexity_Class.
  jmethodID Complexity_Class_ordinal_ID;
  // Congruence.
  jfieldID Congruence_mod_ID;
  jfieldID Congruence_lhs_ID;
  jfieldID Congruence_rhs_ID;
  jmethodID Congruence_init_ID;
  // Constraint.
  jfieldID Constraint_lhs_ID;
  jfieldID Constraint_rhs_ID;
  jfieldID Constraint_kind_ID;
  jmethodID Constraint_init_ID;
  // Degenerate_Element.
  jmethodID Degenerate_Element_ordinal_ID;
  // Generator.
  jfieldID Generator_gt_ID;
  jfieldID Generator_le_ID;
  jfieldID Generator_div_ID;
  jmethodID Generator_line_ID;
  jmethodID Generator_ray_ID;
  jmethodID Generator_point_ID;
  jmethodID Generator_closure_point_ID;
  // Grid_Generator.
  jfieldID Grid_Generator_gt_ID;
  jfieldID Grid_Generator_le_ID;
  jfieldID Grid_Generator_div_ID;
  jmethodID Grid_Generator_grid_line_ID;
  jmethodID Grid_Generator_parameter_ID;
  jmethodID Grid_Generator_grid_point_ID;
  // (Grid_) Generator_Type.
  jmethodID Generator_Type_ordinal_ID;
  jmethodID Grid_Generator_Type_ordinal_ID;
  // Systems of Constraint, Congruence, Generator and Grid_Generator.
  jmethodID Constraint_System_init_ID;
  jmethodID Constraint_System_add_ID;
  jmethodID Congruence_System_init_ID;
  jmethodID Congruence_System_add_ID;
  jmethodID Generator_System_init_ID;
  jmethodID Generator_System_add_ID;
  jmethodID Grid_Generator_System_init_ID;
  jmethodID Grid_Generator_System_add_ID;
  // System_Iterator: stored when loading Constraint_System.
  jmethodID System_iterator_ID;
  jmethodID System_Iterator_has_next_ID;
  jmethodID System_Iterator_next_ID;
  // Linear_Expression.
  jmethodID Linear_Expression_sum_ID;
  jmethodID Linear_Expression_times_ID;
  // Classes extending Linear_Expression.
  jfieldID Linear_Expression_Coefficient_coeff_ID;
  jmethodID Linear_Expression_Coefficient_init_ID;
  jfieldID Linear_Expression_Difference_lhs_ID;
  jfieldID Linear_Expression_Difference_rhs_ID;
  jfieldID Linear_Expression_Sum_lhs_ID;
  jfieldID Linear_Expression_Sum_rhs_ID;
  jfieldID Linear_Expression_Times_coeff_ID;
  jfieldID Linear_Expression_Times_lin_expr_ID;
  jmethodID Linear_Expression_Times_init_from_coeff_var_ID;
  jfieldID Linear_Expression_Unary_Minus_arg_ID;
  jmethodID Linear_Expression_Variable_init_ID;
  jmethodID Linear_Expression_Variable_var_id_ID;
  // MIP_Problem_Status.
  jfieldID MIP_Problem_Status_UNFEASIBLE_MIP_PROBLEM_ID;
  jfieldID MIP_Problem_Status_UNBOUNDED_MIP_PROBLEM_ID;
  jfieldID MIP_Problem_Status_OPTIMIZED_MIP_PROBLEM_ID;
  jmethodID MIP_Problem_Status_ordinal_ID;
  // Optmization_Mode.
  jfieldID Optimization_Mode_MAXIMIZATION_ID;
  jfieldID Optimization_Mode_MINIMIZATION_ID;
  jmethodID Optimization_Mode_ordinal_ID;
  // PIP_Problem_Control_Parameter_Name.
  jfieldID PIP_Problem_Control_Parameter_Name_CUTTING_STRATEGY_ID;
  jfieldID PIP_Problem_Control_Parameter_Name_PIVOT_ROW_STRATEGY;
  jmethodID PIP_Problem_Control_Parameter_Name_ordinal_ID;
  // PIP_Problem_Control_Parameter_Value.
  jfieldID PIP_Problem_Control_Parameter_Value_CUTTING_STRATEGY_FIRST_ID;
  jfieldID PIP_Problem_Control_Parameter_Value_CUTTING_STRATEGY_DEEPEST_ID;
  jfieldID PIP_Problem_Control_Parameter_Value_CUTTING_STRATEGY_ALL_ID;
  jfieldID PIP_Problem_Control_Parameter_Value_PIVOT_ROW_STRATEGY_FIRST_ID;
  jfieldID PIP_Problem_Control_Parameter_Value_PIVOT_ROW_STRATEGY_MAX_COLUMN_ID;
  jmethodID PIP_Problem_Control_Parameter_Value_ordinal_ID;
  // PIP_Problem_Status.
  jfieldID PIP_Problem_Status_UNFEASIBLE_PIP_PROBLEM_ID;
  jfieldID PIP_Problem_Status_OPTIMIZED_PIP_PROBLEM_ID;
  jmethodID PIP_Problem_Status_ordinal_ID;
  // Pair.
  jfieldID Pair_first_ID;
  jfieldID Pair_second_ID;
  // Poly_Con_Relation and Poly_Gen_Relation.
  jmethodID Poly_Con_Relation_init_ID;
  jmethodID Poly_Gen_Relation_init_ID;
  // PPL_Object.
  jfieldID PPL_Object_ptr_ID;
  // Relation_Symbol.
  jfieldID Relation_Symbol_EQUAL_ID;
  jfieldID Relation_Symbol_GREATER_OR_EQUAL_ID;
  jfieldID Relation_Symbol_GREATER_THAN_ID;
  jmethodID Relation_Symbol_ordinal_ID;
  // Variable.
  jfieldID Variable_varid_ID;
  jmethodID Variable_init_ID;
  // Variables_Set.
  jmethodID Variables_Set_init_ID;
  jmethodID Variables_Set_add_ID;
  jmethodID Variables_Set_iterator_ID;
  // Iterators on Variables_Set.
  jmethodID Variables_Set_Iterator_has_next_ID;
  jmethodID Variables_Set_Iterator_next_ID;
};

//! The cached class references.
extern Java_Class_Cache cached_classes;

//! The field and method ID cache.
extern Java_FMID_Cache cached_FMIDs;


/*! \brief
  Builds an unsigned C++ number from the Java native number \p value.

  \param value
  The Java native number of type V to be converted.

  \exception std::invalid_argument
  Thrown if \p value is negative.
*/
template <typename U, typename V>
U
jtype_to_unsigned(const V& value);

//! Builds a Java Boolean from C++ bool \p value.
jobject
bool_to_j_boolean_class(JNIEnv* env, const bool value);

//! Returns the Java int stored in Java Integer \p j_integer.
jint
j_integer_to_j_int(JNIEnv* env, jobject j_integer);

//! Builds a Java Integer from Java int \p value.
jobject
j_int_to_j_integer(JNIEnv* env, jint value);

//! Returns the Java long stored in Java Long \p j_long.
jlong
j_long_class_to_j_long(JNIEnv* env, jobject j_long);

//! Builds a Java Long from Java long \p value.
jobject
j_long_to_j_long_class(JNIEnv* env, jlong value);

/*! \brief
  Returns \c true if and only if the Java object \p ppl_object
  refers to a C++ object.
*/
bool
is_java_marked(JNIEnv* env, jobject ppl_object);


/*! \brief
  Builds a Java parma_polyhedra_library::Poly_Gen_Relation
  from C++ Poly_Gen_Relation \p r.
*/
jobject
build_java_poly_gen_relation(JNIEnv* env, Poly_Gen_Relation& r);

/*! \brief
  Builds a Java parma_polyhedra_library::Poly_Con_Relation
  from C++ Poly_Con_Relation \p r.
*/
jobject
build_java_poly_con_relation(JNIEnv* env, Poly_Con_Relation& r);

/*! \brief
  Builds a C++ Variables_Set
  from Java parma_polyhedra_library::Variables_Set \p v_set.
*/
Variables_Set
build_cxx_variables_set(JNIEnv* env, jobject v_set);

/*! \brief
  Builds a Java parma_polyhedra_library::Variables_Set
  from C++ Variables_Set \p v_set.
*/
jobject
build_java_variables_set(JNIEnv* env, const Variables_Set& v_set);

/*! \brief
  Builds a C++ Relation_Symbol
  from Java parma_polyhedra_library::Relation_Symbol \p j_relsym.
*/
Relation_Symbol
build_cxx_relsym(JNIEnv* env, jobject j_relsym);

/*! \brief
  Builds a C++ Bounded_Integer_Type_Overflow
  from Java parma_polyhedra_library::Bounded_Integer_Type_Overflow
  \p j_bounded_overflow.
*/
Bounded_Integer_Type_Overflow
build_cxx_bounded_overflow(JNIEnv* env, jobject j_bounded_overflow);

/*! \brief
  Builds a C++ Bounded_Integer_Type_Width
  from Java parma_polyhedra_library::Bounded_Integer_Type_Width
  \p j_bounded_width.
*/
Bounded_Integer_Type_Width
build_cxx_bounded_width(JNIEnv* env, jobject j_bounded_width);

/*! \brief
  Builds a C++ Bounded_Integer_Type_Representation
  from Java parma_polyhedra_library::Bounded_Integer_Type_Representation
  \p j_bounded_rep.
*/
Bounded_Integer_Type_Representation
build_cxx_bounded_rep(JNIEnv* env, jobject j_bounded_rep);

/*! \brief
  Builds a C++ Optimization_Mode
  from Java parma_polyhedra_library::Optimization_Mode \p j_opt_mode.
*/
Optimization_Mode
build_cxx_optimization_mode(JNIEnv* env, jobject j_opt_mode);

/*! \brief
  Builds a Java parma_polyhedra_library::Optimization_Mode
  from C++ Optimization_Mode \p opt_mode.
*/
jobject
build_java_optimization_mode(JNIEnv* env, const Optimization_Mode& opt_mode);

/*! \brief
  Builds a C++ MIP_Problem::Control_Parameter_Name
  from Java parma_polyhedra_library::Control_Parameter_Name \p j_cp_name.
*/
MIP_Problem::Control_Parameter_Name
build_cxx_control_parameter_name(JNIEnv* env, jobject j_cp_name);

/*! \brief
  Builds a Java parma_polyhedra_library::Control_Parameter_Name
  from C++ MIP_Problem::Control_Parameter_Name \p cp_name.
*/
jobject
build_java_control_parameter_name
(JNIEnv* env, const MIP_Problem::Control_Parameter_Name& cp_name);

/*! \brief
  Builds a C++ MIP_Problem::Control_Parameter_Value
  from Java parma_polyhedra_library::Control_Parameter_Value \p j_cp_value.
*/
MIP_Problem::Control_Parameter_Value
build_cxx_control_parameter_value(JNIEnv* env, jobject j_cp_value);

/*! \brief
  Builds a Java parma_polyhedra_library::Control_Parameter_Value
  from C++ MIP_Problem::Control_Parameter_Value \p cp_value.
*/
jobject
build_java_control_parameter_value
(JNIEnv* env, const MIP_Problem::Control_Parameter_Value& cp_value);

/*! \brief
  Builds a C++ PIP_Problem::Control_Parameter_Name
  from Java parma_polyhedra_library::PIP_Problem_Control_Parameter_Name
  \p j_cp_name.
*/
PIP_Problem::Control_Parameter_Name
build_cxx_pip_problem_control_parameter_name(JNIEnv* env, jobject j_cp_name);

/*! \brief
  Builds a Java parma_polyhedra_library::PIP_Problem_Control_Parameter_Name
  from C++ PIP_Problem::Control_Parameter_Name \p cp_name.
*/
jobject
build_java_pip_problem_control_parameter_name
(JNIEnv* env, const PIP_Problem::Control_Parameter_Name& cp_name);

/*! \brief
  Builds a C++ PIP_Problem::Control_Parameter_Value
  from Java parma_polyhedra_library::PIP_Problem_Control_Parameter_Value
  \p j_cp_value.
*/
PIP_Problem::Control_Parameter_Value
build_cxx_pip_problem_control_parameter_value(JNIEnv* env, jobject j_cp_value);

/*! \brief
  Builds a Java parma_polyhedra_library::Control_Parameter_Value
  from C++ PIP_Problem::Control_Parameter_Value \p cp_value.
*/
jobject
build_java_pip_problem_control_parameter_value
(JNIEnv* env, const PIP_Problem::Control_Parameter_Value& cp_value);

/*! \brief
  Builds a Java parma_polyhedra_library::MIP_Problem_Status
  from C++ MIP_Problem_Status \p mip_status.
*/
jobject
build_java_mip_status(JNIEnv* env, const MIP_Problem_Status& mip_status);

/*! \brief
  Builds a Java parma_polyhedra_library::PIP_Problem_Status
  from C++ PIP_Problem_Status \p pip_status.
*/
jobject
build_java_pip_status(JNIEnv* env, const PIP_Problem_Status& pip_status);

/*! \brief
  Builds a C++ Variable
  from Java parma_polyhedra_library::Variable \p j_var.
*/
Variable
build_cxx_variable(JNIEnv* env, jobject j_var);

/*! \brief
  Builds a Java parma_polyhedra_library::Variable
  from C++ Variable \p var.
*/
jobject
build_java_variable(JNIEnv* env, const Variable var);

/*! \brief
  Builds a C++ Coefficient
  from Java parma_polyhedra_library::Coefficient \p j_coeff.
*/
Coefficient
build_cxx_coeff(JNIEnv* env, jobject j_coeff);

/*! \brief
  Builds a Java parma_polyhedra_library::Coefficient
  from C++ Coefficient \p ppl_coeff.
*/
jobject
build_java_coeff(JNIEnv* env, const Coefficient& ppl_coeff);

/*! \brief
  Builds a C++ Constraint
  from Java parma_polyhedra_library::Constraint \p j_constraint.
*/
Constraint
build_cxx_constraint(JNIEnv* env, jobject j_constraint);

/*! \brief
  Builds a C++ Artificial_Parameter
  from Java parma_polyhedra_library::Artificial_Parameter
  \p j_artificial_parameter.
*/
PIP_Tree_Node::Artificial_Parameter
build_cxx_artificial_parameter(JNIEnv* env, jobject j_ap);

/*! \brief
  Builds a C++ Linear_Expression
  from Java parma_polyhedra_library::Linear_Expression \p j_le.
*/
Linear_Expression
build_cxx_linear_expression(JNIEnv* env, jobject j_le);

/*! \brief
  Builds a C++ Congruence
  from Java parma_polyhedra_library::Congruence \p j_cg.
*/
Congruence
build_cxx_congruence(JNIEnv* env, jobject j_cg);

/*! \brief
  Builds a C++ Generator
  from Java parma_polyhedra_library::Generator \p j_g.
*/
Generator
build_cxx_generator(JNIEnv* env, jobject j_g);

/*! \brief
  Builds a C++ Grid_Generator
  from Java parma_polyhedra_library::Grid_Generator \p j_g.
*/
Grid_Generator
build_cxx_grid_generator(JNIEnv* env, jobject j_g);

/*! \brief
  Builds a C++ Grid_Generator_System
  from Java parma_polyhedra_library::Grid_Generator_System \p j_gs.
*/
Grid_Generator_System
build_cxx_grid_generator_system(JNIEnv* env, jobject j_gs);

/*! \brief
  Builds a C++ Constraint_System
  from Java parma_polyhedra_library::Constraint_System \p j_cs.
*/
Constraint_System
build_cxx_constraint_system(JNIEnv* env, jobject j_cs);

/*! \brief
  Builds a C++ Artificial_Parameter
  from Java parma_polyhedra_library::Artificial_Parameter \p j_art_param.
*/
PIP_Tree_Node::Artificial_Parameter
build_cxx_artificial_parameter(JNIEnv* env, jobject j_art_param);

/*! \brief
  Builds a C++ Artificial_Parameter_Sequence
  from Java parma_polyhedra_library::Artificial_Parameter_Sequence \p j_aps.
*/
PIP_Tree_Node::Artificial_Parameter_Sequence
build_cxx_artificial_parameter_sequence(JNIEnv* env, jobject j_aps);

/*! \brief
  Builds a C++ Generator_System
  from Java parma_polyhedra_library::Generator_System \p j_gs.
*/
Generator_System
build_cxx_generator_system(JNIEnv* env, jobject j_gs);

/*! \brief
  Builds a C++ Congruence_System
  from Java parma_polyhedra_library::Congruence_System \p j_cgs.
*/
Congruence_System
build_cxx_congruence_system(JNIEnv* env, jobject j_cgs);

/*! \brief
  Builds a Java parma_polyhedra_library::Constraint
  from C++ Constraint \p c.
*/
jobject
build_java_constraint(JNIEnv* env, const Constraint& c);

/*! \brief
  Builds a Java parma_polyhedra_library::Artificial_Parameter
  from C++ Artificial_Parameter \p ap.
*/
jobject
build_java_artificial_parameter(JNIEnv* env,
                                const PIP_Tree_Node::Artificial_Parameter& ap);

/*! \brief
  Builds a Java parma_polyhedra_library::Congruence
  from C++ Congruence \p cg.
*/
jobject
build_java_congruence(JNIEnv* env, const Congruence& cg);

/*! \brief
  Builds a Java parma_polyhedra_library::Generator
  from C++ Generator \p g.
*/
jobject
build_java_generator(JNIEnv* env, const Generator& g);

/*! \brief
  Builds a Java parma_polyhedra_library::Grid_Generator
  from C++ Grid_Generator \p g.
*/
jobject
build_java_grid_generator(JNIEnv* env, const Grid_Generator& g);

/*! \brief
  Builds a Java parma_polyhedra_library::Constraint_System
  from C++ Constraint_System \p cs.
*/
jobject
build_java_constraint_system(JNIEnv* env, const Constraint_System& cs);

/*! \brief
  Builds a Java parma_polyhedra_library::Artificial_Parameter_Sequence
  from C++ Artificial_Parameter_Sequence \p aps.
*/
jobject
build_java_artificial_parameter_sequence
(JNIEnv* env, const PIP_Tree_Node::Artificial_Parameter_Sequence& aps);

/*! \brief
  Builds a Java parma_polyhedra_library::Grid_Generator_System
  from C++ Grid_Generator_System \p gs.
*/
jobject
build_java_grid_generator_system(JNIEnv* env, const Grid_Generator_System& gs);

/*! \brief
  Builds a Java parma_polyhedra_library::Generator_System
  from C++ Generator_System \p gs.
*/
jobject
build_java_generator_system(JNIEnv* env, const Generator_System& gs);

/*! \brief
  Builds a Java parma_polyhedra_library::Congruence_System
  from C++ Congruence_System \p cgs.
*/
jobject
build_java_congruence_system(JNIEnv* env, const Congruence_System& cgs);

/*! \brief
  Builds a Java parma_polyhedra_library::Linear_Expression_Coefficient
  from C++ Coefficient \p coeff.
*/
jobject
build_java_linear_expression_coefficient(JNIEnv* env, const Coefficient& coeff);

/*! \brief
  Sets Java parma_polyhedra_library::Generator \p dst
  to have the same value as \p src.
*/
void
set_generator(JNIEnv* env, jobject dst, jobject src);

/*! \brief
  Sets Java Coefficient \p dst to have the same value as \p src.
*/
void
set_coefficient(JNIEnv* env, jobject dst, jobject src);

/*! \brief
  Modifies parma_polyhedra_library::By_Reference object \p by_ref_dst
  so that it references object \p src.
*/
void
set_by_reference(JNIEnv* env, jobject by_ref_dst, jobject src);

/*! \brief
  Returns the object referenced by
  parma_polyhedra_library::By_Reference object \p by_reference.
*/
jobject
get_by_reference(JNIEnv* env, jobject by_reference);


/*! \brief
  Assigns \p src to one of the fields of
  parma_polyhedra_library::Pair object \p dst_pair.

  If \p arg is 0, the first element of \p dst_pair is overwritten;
  if \p arg is 1, the second element of \p dst_pair is overwritten.

  \exception std::runtime_error
  Thrown if \p arg is neither 0 nor 1.
*/
void
set_pair_element(JNIEnv* env, jobject dst_pair, int arg, jobject src);

/*! \brief
  Returns one of the fields of the
  parma_polyhedra_library::Pair object \p pair.

  If \p arg is 0, the first element of \p pair is returned;
  if \p arg is 1, the second element of \p pair is returned.

  \exception std::runtime_error
  Thrown if \p arg is neither 0 nor 1.
*/
jobject
get_pair_element(JNIEnv* env, int arg, jobject pair);

//! Returns a pointer to the C++ object wrapped by \p ppl_object.
void*
get_ptr(JNIEnv* env, jobject ppl_object);

//! Sets the pointer of the underlying C++ object in the Java object.
template <typename T>
void
set_ptr(JNIEnv* env, jobject ppl_object,
	const T* address, bool to_be_marked = false);

/*! \brief
  Builds a Java parma_polyhedra_library::Linear_Expression
  from the C++ constraint/congruence \p r.
*/
template <typename R>
jobject
build_linear_expression(JNIEnv* env, const R& r);

} // namespace Java

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library


#include "ppl_java_common.inlines.hh"

#endif // !defined(PPL_ppl_prolog_common_defs_hh)
