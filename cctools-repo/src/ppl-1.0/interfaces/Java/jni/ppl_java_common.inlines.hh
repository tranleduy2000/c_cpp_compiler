/* Domain-independent part of the Java interface: inline functions.
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

#ifndef PPL_ppl_java_common_inlines_hh
#define PPL_ppl_java_common_inlines_hh 1

#include <cassert>

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace Java {

template <typename U, typename V>
U
jtype_to_unsigned(const V& value) {
  if (value < 0)
    throw std::invalid_argument("not an unsigned integer.");

  if (sizeof(U) < sizeof(V)) {
    if (value > static_cast<V>(std::numeric_limits<U>::max()))
      throw std::invalid_argument("unsigned integer out of range.");
  }

  return value;
}

template <typename T>
inline void
set_ptr(JNIEnv* env, jobject ppl_object,
	const T* address, bool to_be_marked) {
  const T* ptr = (to_be_marked ? mark(address) : address);
  jlong pointer_value = reinterpret_cast<jlong>(ptr);
  assert(reinterpret_cast<const T*>(pointer_value) == ptr);
  env->SetLongField(ppl_object, cached_FMIDs.PPL_Object_ptr_ID, pointer_value);
}

inline void*
get_ptr(JNIEnv* env, jobject ppl_object) {
  jlong pointer_value
    = env->GetLongField(ppl_object, cached_FMIDs.PPL_Object_ptr_ID);
  void* ptr = reinterpret_cast<void*>(pointer_value);
  assert(reinterpret_cast<jlong>(ptr) == pointer_value);
  return unmark(ptr);
}

inline bool
is_java_marked(JNIEnv* env, jobject ppl_object) {
  jlong pointer_value
    = env->GetLongField(ppl_object, cached_FMIDs.PPL_Object_ptr_ID);
  const void* ptr = reinterpret_cast<const void*>(pointer_value);
  assert(reinterpret_cast<jlong>(ptr) == pointer_value);
  return marked(ptr);
}

inline void
set_coefficient(JNIEnv* env, jobject dst, jobject src) {
  jobject src_bi
    = env->GetObjectField(src, cached_FMIDs.Coefficient_value_ID);
  env->SetObjectField(dst, cached_FMIDs.Coefficient_value_ID, src_bi);
}

inline void
set_by_reference(JNIEnv* env, jobject by_ref_dst, jobject src) {
  env->SetObjectField(by_ref_dst,
                      cached_FMIDs.By_Reference_obj_ID,
                      src);
}

inline jobject
get_by_reference(JNIEnv* env, jobject by_reference) {
  return env->GetObjectField(by_reference, cached_FMIDs.By_Reference_obj_ID);
}

template <typename R>
jobject
build_linear_expression(JNIEnv* env, const R& r) {
  jobject j_ret;
  PPL_DIRTY_TEMP_COEFFICIENT(coefficient);
  dimension_type varid = 0;
  dimension_type space_dimension = r.space_dimension();
  while (varid < space_dimension
 	 && (coefficient = r.coefficient(Variable(varid))) == 0)
    ++varid;
  if (varid >= space_dimension) {
    jobject j_coefficient_zero = build_java_coeff(env, Coefficient(0));
    j_ret = env->NewObject(cached_classes.Linear_Expression_Coefficient,
                           cached_FMIDs.Linear_Expression_Coefficient_init_ID,
                           j_coefficient_zero);
    CHECK_RESULT_THROW(env, j_ret);
  }
  else {
    jmethodID coeff_var_init_ID
      = cached_FMIDs.Linear_Expression_Times_init_from_coeff_var_ID;
    jobject j_coefficient = build_java_coeff(env, coefficient);
    jobject j_variable = build_java_variable(env, Variable(varid));
    jclass j_le_times_class = cached_classes.Linear_Expression_Times;
    jobject j_coeff_var = env->NewObject(j_le_times_class,
                                         coeff_var_init_ID,
                                         j_coefficient, j_variable);
    CHECK_EXCEPTION_THROW(env);
    j_ret = j_coeff_var;
    while (true) {
      ++varid;
      while (varid < space_dimension
	     && (coefficient = r.coefficient(Variable(varid))) == 0)
	++varid;
      if (varid >= space_dimension)
	break;
      else {
 	j_coefficient = build_java_coeff(env, coefficient);
        j_variable = build_java_variable(env, Variable(varid));
        j_coeff_var = env->NewObject(j_le_times_class,
                                     coeff_var_init_ID,
                                     j_coefficient, j_variable);
	CHECK_EXCEPTION_THROW(env);
        j_ret = env->CallObjectMethod(j_ret,
                                      cached_FMIDs.Linear_Expression_sum_ID,
                                      j_coeff_var);
	CHECK_EXCEPTION_THROW(env);
      }
    }
  }
  return j_ret;
}

inline Variable
build_cxx_variable(JNIEnv* env, jobject j_var) {
  return Variable(env->GetIntField(j_var, cached_FMIDs.Variable_varid_ID));
}

inline jobject
build_java_variable(JNIEnv* env, const Variable var) {
  jobject ret = env->NewObject(cached_classes.Variable,
                               cached_FMIDs.Variable_init_ID,
			       var.id());
  CHECK_RESULT_THROW(env, ret);
  return ret;
}

inline Coefficient
build_cxx_coeff(JNIEnv* env, jobject j_coeff) {
  jstring bi_string
    = (jstring) env->CallObjectMethod(j_coeff,
                                      cached_FMIDs.Coefficient_toString_ID);
  CHECK_EXCEPTION_THROW(env);
  const char *nativeString = env->GetStringUTFChars(bi_string, 0);
  CHECK_RESULT_THROW(env, nativeString);
  PPL_DIRTY_TEMP_COEFFICIENT(ppl_coeff);
  ppl_coeff = Coefficient(nativeString);
  env->ReleaseStringUTFChars(bi_string, nativeString);
  return ppl_coeff;
}

inline jobject
build_java_coeff(JNIEnv* env, const Coefficient& ppl_coeff) {
  std::ostringstream s;
  s << ppl_coeff;
  std::string str = s.str();
  jstring coeff_string = env->NewStringUTF(str.c_str());
  CHECK_RESULT_THROW(env, coeff_string);
  jobject ret = env->NewObject(cached_classes.Coefficient,
                               cached_FMIDs.Coefficient_init_from_String_ID,
                               coeff_string);
  CHECK_RESULT_THROW(env, ret);
  return ret;
}

template <typename System, typename Elem_Builder>
System
build_cxx_system(JNIEnv* env, jobject j_iterable, Elem_Builder build_cxx_elem) {
  // Get the iterator.
  jobject j_iter
    = env->CallObjectMethod(j_iterable, cached_FMIDs.System_iterator_ID);
  CHECK_EXCEPTION_THROW(env);
  // Get the iterator method IDs.
  jmethodID has_next_mID = cached_FMIDs.System_Iterator_has_next_ID;
  jmethodID next_mID = cached_FMIDs.System_Iterator_next_ID;
  // Initialize an empty system.
  System cxx_sys;
  jobject j_element;
  jboolean has_next_value = env->CallBooleanMethod(j_iter, has_next_mID);
  CHECK_EXCEPTION_ASSERT(env);
  while (has_next_value) {
    j_element = env->CallObjectMethod(j_iter, next_mID);
    CHECK_EXCEPTION_ASSERT(env);
    cxx_sys.insert(build_cxx_elem(env, j_element));
    has_next_value = env->CallBooleanMethod(j_iter, has_next_mID);
    CHECK_EXCEPTION_ASSERT(env);
  }
  return cxx_sys;
}

inline Congruence_System
build_cxx_congruence_system(JNIEnv* env, jobject j_iterable) {
  return
    build_cxx_system<Congruence_System>(env, j_iterable, build_cxx_congruence);
}

inline Constraint_System
build_cxx_constraint_system(JNIEnv* env, jobject j_iterable) {
  return
    build_cxx_system<Constraint_System>(env, j_iterable, build_cxx_constraint);
}

inline Generator_System
build_cxx_generator_system(JNIEnv* env, jobject j_iterable) {
  return
    build_cxx_system<Generator_System>(env, j_iterable, build_cxx_generator);
}

inline Grid_Generator_System
build_cxx_grid_generator_system(JNIEnv* env, jobject j_iterable) {
  return build_cxx_system<Grid_Generator_System> (env, j_iterable,
                                                  build_cxx_grid_generator);
}

} // namespace Java

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_ppl_java_common_inlines_hh)
