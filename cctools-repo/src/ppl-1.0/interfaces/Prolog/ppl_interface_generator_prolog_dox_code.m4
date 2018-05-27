m4_divert(-1)

dnl This m4 file contains the program code for generating Prolog_interface.dox

dnl Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
dnl Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
dnl
dnl This file is part of the Parma Polyhedra Library (PPL).
dnl
dnl The PPL is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the
dnl Free Software Foundation; either version 3 of the License, or (at your
dnl option) any later version.
dnl
dnl The PPL is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software Foundation,
dnl Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
dnl
dnl For the most up-to-date information see the Parma Polyhedra Library
dnl site: http://bugseng.com/products/ppl/ .

dnl category and subcategory headers and descriptions

dnl -----------------------------------------------------------------
dnl *** The constructor, copy, convert and destructor predicates. ***
dnl -----------------------------------------------------------------
m4_define(`m4_category_constr_destr', `dnl
<H2>Constructor, copy, conversion and destructor predicates</H2>
m4_pushdef(`m4_topology_a_replacements', `')`'dnl
m4_pushdef(`m4_Polyhedron_a_topology_replacements', `C , NNC ')`'dnl
')

m4_define(`m4_finalize_category_constr_destr', `dnl
m4_popdef(`m4_a_topology_replacements')`'dnl
m4_popdef(`m4_Polyhedron_a_topology_replacements')`'dnl
')

dnl -----------------------------------------------------------------
dnl The constructor predicates.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_build', `dnl
<H3>Constructor predicates for m4_this_topology m4_this_ppl_objects</H3>
The constructor predicates build a
m4_this_topology m4_this_ppl_one_object from a specification
and binds the given variable to a handle for future referencing.
The specification can be:
- the number of space dimensions and an atom indicating if it
  is to be the universe or empty element.
- a representation for the particular class of semantic geometric
  descriptors to which the element being built belongs.
m4_ifelse(m4_this_interface_class, Polyhedron,
  `For example`,'
  a C Polyhedron can be built from a list of
  non-strict inequality or equality constraints or a list of equality
  congruences or a list of generators that contains no closure points.')
')

dnl -----------------------------------------------------------------
dnl The copy and conversion constructor predicates.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_convert', `dnl
<H3>Predicates that build new m4_this_topology m4_this_ppl_objects
  by copying or converting from
  other semantic geometric descriptions</H3>
  Besides the constructors listed above, the library also
  provides:
- copy constructors that will copy an element belonging
  to the same class of semantic geometric descriptions
- conversion operators that build a new semantic geometric
  description starting from a \b friend;
  that is`,' a semantic geometric description in different class
m4_ifelse(m4_this_interface_class, Polyhedron, `dnl
  (e.g.`,' ppl_new_Grid_from_C_Polyhedron`,'
  ppl_new_C_Polyhedron_from_BD_Shape_mpq_class`,' etc.)').<BR>

  The copy and conversion predicates have two versions,
  one with arity 2 for the source and target handles and one
  with an extra argument denoting the maximum complexity to be used
  in the conversion;
  this complexity argument is ignored when the
  the friend and the element being built are in the same class.
')

dnl -----------------------------------------------------------------
dnl The destructor predicate
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_destruct', `dnl
<H3>Destructor predicate</H3>
Below is the destructor predicate for the m4_this_interface_class domain.
')

dnl -----------------------------------------------------------------
dnl *** Predicates that do not modify the polyhedron. ***
dnl -----------------------------------------------------------------
m4_define(`m4_category_const', `dnl
<H2>Predicates that do not change the m4_this_ppl_one_object</H2>
m4_pushdef(`m4_a_has_property_replacements', `dnl
  is empty, is the universe, is bounded, contains an integer point,
  is topologically closed, is discrete')`'dnl
m4_pushdef(`m4_product_a_has_property_replacements', `dnl
  is empty, is the universe, is bounded,
  is topologically closed, is discrete')`'dnl
m4_pushdef(`m4_a_comparison_replacements', `dnl
  is included in or equal to,  is included in but not equal to,
  is disjoint from')`'dnl
m4_pushdef(`m4_Pointset_Powerset_a_comparison_replacements', `dnl
  m4_a_comparison_replacements,
  geometrically covers, geometrically equals')`'dnl
m4_pushdef(`m4_maxmin_replacements', `max, min')`'dnl
m4_pushdef(`m4_a_maxmin_replacements', `above, below')`'dnl
m4_pushdef(`m4_b_maxmin_replacements', `supremum, infinum')`'dnl
m4_pushdef(`m4_maxmin_replacements', `max, min')`'dnl
m4_pushdef(`m4_a_maxmin_replacements', `above, below')`'dnl
m4_pushdef(`m4_b_maxmin_replacements', `supremum, infinum')`'dnl
m4_pushdef(`m4_a_dimension_replacements', `dnl
  dimension of the vector space enclosing, affine dimension of')`'dnl
')

m4_define(`m4_finalize_category_const', `dnl
m4_popdef(`m4_has_a_property_replacements')`'dnl
m4_popdef(`m4_product_a_has_property_replacements')`'dnl
m4_popdef(`m4_a_comparison_replacements')`'dnl
m4_popdef(`m4_Pointset_Powerset_a_comparison_replacements')`'dnl
m4_popdef(`m4_maxmin_replacements')`'dnl
m4_popdef(`m4_a_maxmin_replacements')`'dnl
m4_popdef(`m4_b_maxmin_replacements')`'dnl
m4_popdef(`m4_maxmin_replacements')`'dnl
m4_popdef(`m4_a_maxmin_replacements')`'dnl
m4_popdef(`m4_b_maxmin_replacements')`'dnl
m4_popdef(`m4_a_dimension_replacements')`'dnl
')

dnl -----------------------------------------------------------------
dnl Predicates that test a property
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_testpoly', `dnl
<H3>Test Predicates</H3>
These predicates test the m4_this_ppl_one_object for different properties
and succeed or fail depending on the outcome.
')

dnl -----------------------------------------------------------------
dnl Predicates that get information about the polyhedron.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_constpoly', `dnl
<H3>Predicates that return information about the m4_this_ppl_one_object</H3>
These predicates will obtain more detailed information about the
m4_this_ppl_one_object
unifying some of their arguments with the results.
')

dnl -----------------------------------------------------------------
dnl Predicate that dumps.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_dump', `dnl
<H3>Ascii dump predicate</H3>
This output predicate is useful for debugging.
')

dnl -----------------------------------------------------------------
dnl *** Space dimension preserving predicates that may modify the ***
dnl *** polyhedron.                                               ***
dnl -----------------------------------------------------------------
m4_define(`m4_category_nonconst', `dnl
<H2>Space-dimension preserving predicates that may change
    the m4_this_ppl_one_object</H2>
These predicates may modify the m4_this_ppl_one_object referred to
by the handle in first argument;
the (dimension of the) vector space in which it is embedded is unchanged.
m4_pushdef(`m4_binop_replacements', `dnl
  intersection_assign, upper_bound_assign, difference_assign,
  time_elapse_assign')`'dnl
m4_pushdef(`m4_Polyhedron_binop_replacements', `dnl
  m4_binop_replacements, poly_hull, poly_difference')`'dnl
m4_pushdef(`m4_a_binop_replacements', `dnl
  intersection, upper bound, difference, time elapse')`'dnl
m4_pushdef(`m4_Polyhedron_a_binop_replacements', `dnl
  m4_a_binop_replacements, poly-hull, poly-difference')`'dnl
m4_pushdef(`m4_a_simplify_replacements', `topological closure')`'dnl
m4_pushdef(`m4_Pointset_Powerset_a_simplify_replacements', `dnl
  topological closure, pairwise reduction, omega reduction')`'dnl
')

m4_define(`m4_finalize_category_nonconst', `dnl
m4_popdef(`m4_binop_replacements')`'dnl
m4_popdef(`m4_Polyhedron_binop_replacements')`'dnl
m4_popdef(`m4_a_binop_replacements')`'dnl
m4_popdef(`m4_Polyhedron_a_binop_replacements')`'dnl
m4_popdef(`m4_a_simplify_replacements')`'dnl
m4_popdef(`m4_Pointset_Powerset_a_simplify_replacements')`'dnl
')

dnl -----------------------------------------------------------------
dnl Predicates that may change the polyhedron by adding
dnl to the polyhedron's constraint or generator descriptions.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_addto', `dnl
<H3>Predicates that may change the m4_this_ppl_one_object by adding
to its constraint or generator descriptions</H3>
Note that there are two forms of these predicates
differentiated in the names by the
words "add" or "refine with";
see Section \extref{Generic_Operations_on_Semantic_Geometric_Descriptors, Generic Operations on Semantic Geometric Descriptors} in the
main \extref{preamble, PPL User Manual}
for the differences in the semantics and therefore, the expected behavior,
between these forms.
')

dnl -----------------------------------------------------------------
dnl Predicates that transform a polyhedron.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_trans', `dnl
<H3>Predicates that transform the m4_this_ppl_one_object</H3>
These predicates enable transformations
such as taking the topological closure (which for the domain of
m4_this_topology m4_this_ppl_one_object is the identity transformation),
unconstraining a specified dimension
as explained in the
main \extref{preamble, PPL User Manual} in Section
\extref{Cylindrification, Cylindrification Operator}
and several different image and preimage affine transfer relations;
for details of the latter see Sections
\extref{Images_and_Preimages_of_Affine_Transfer_Relations, Images and Preimages of Affine Transfer Relations}
and
\extref{Generalized_Affine_Relations, Generalized Affine Relations}')

dnl -----------------------------------------------------------------
dnl Predicates that act as binary operators.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_binop', `dnl
<H3>Predicates whose results depend on more than one m4_this_ppl_one_object</H3>
These predicates include the binary operators which will assign to the
m4_this_ppl_one_object referred to by the first argument
its combination with the m4_this_ppl_one_object referred to by the second argument
as described in the
main \extref{preamble, PPL User Manual} in Sections
\extref{Intersection_and_Convex_Polyhedral_Hull, Intersection and Convex Polyhedral Hull}
and
\extref{Convex_Polyhedral_Difference, Convex Polyhedral Difference};
and a linear partitioning operator described below.
')

dnl -----------------------------------------------------------------
dnl Predicates for widening and extrapolation.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_widen', `dnl
<H3>Predicates for widening and extrapolation</H3>
In addition to the above binary operators, there are also a number
of widening, extrapolation and narrowing operators
as described in the
main \extref{preamble, PPL User Manual} in Sections
\extref{Widening_Operators, Widening Operators},
\extref{Widening_with_Tokens, Widening with Tokens} and
\extref{Extrapolation_Operators, Extrapolation Operators}.
Note that for all these widening and extrapolation predicates
to behave as specified
the m4_this_ppl_one_object referred to by the second argument has to be
contained in (or equal to) the m4_this_ppl_one_object referred to by the first argument.
')

dnl -----------------------------------------------------------------
dnl *** Predicates that may modify the dimension of the vector space. ***
dnl -----------------------------------------------------------------
m4_define(`m4_category_varspace', `dnl
<H2>Predicates that may modify the vector space</H2>
These predicates enable the modification of the vector space
of the m4_this_ppl_one_object referred to in the first argument.
')

m4_define(`m4_finalize_category_varspace', `dnl
')

dnl -----------------------------------------------------------------
dnl Concatenate predicate
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_concat', `dnl
<H3>Predicate for concatenation</H3>
For more information on this operation, see Section
\extref{Concatenating_Polyhedra, Concatenating Polyhedra},
of the main \extref{preamble, PPL User Manual}.
')

dnl -----------------------------------------------------------------
dnl Predicates for mapping or changing the space dimensions
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_spacemod', `dnl
<H3>Predicates for mapping dimensions or changing the vector space</H3>
These predicates enable the modification of the vector space
of the m4_this_ppl_one_object referred to in the first argument.
These predicates enable the modification of the vector space
of the m4_this_ppl_one_object referred to in the first argument.
Detailed descriptions of these can be found in the
main \extref{preamble, PPL User Manual} in Sections
\extref{Adding_New_Dimensions_to_the_Vector_Space, Adding New Dimensions to the Vector Space},
\extref{Removing_Dimensions_from_the_Vector_Space, Removing Dimensions from the Vector Space},
\extref{Mapping_the_Dimensions_of_the_Vector_Space, Mapping the Dimensions of the Vector Space},
\extref{expand_space_dimension, Expanding One Dimension of the Vector Space to Multiple Dimensions}
and
\extref{Folding_Multiple_Dimensions_of_the_Vector_Space_into_One_Dimension, Folding Multiple Dimensions of the Vector Space into One Dimension}.
')

dnl -----------------------------------------------------------------
dnl *** Pointset powerset domains.                               ***
dnl Note that this is only used for the configure independent manual.
dnl -----------------------------------------------------------------
m4_define(`m4_category_pps', `dnl
<H2>Extra Predicates Specifically for the Pointset Powerset Domains</H2>
  The powerset domains can be instantiated by taking as a base domain
  any fixed semantic geometric description
  (C and NNC polyhedra, BD and octagonal shapes, boxes and grids).
  An element of the powerset domain represents a disjunctive collection
  of base objects (its disjuncts), all having the same space dimension.
  For more information on this construct,
  see Section \extref{The_Powerset_Domain, The Powerset Domain}
  in the main \extref{preamble, PPL User Manual}.

  Besides the predicates that are available in all semantic geometric
  descriptions (whose documentation is not repeated here),
  the powerset domain also provides several ad hoc predicates.
  These are specified below, instantiated for the
  PPL domain Pointset_Powerset_C_Polyhedron.
  Note that predicates for other pointset powerset domains
  will follow similar patterns.
')

m4_define(`m4_finalize_category_pps', `dnl
')

dnl -----------------------------------------------------------------
dnl Predicates for pointset powerset iterators and disjuncts.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_pps_iter', `dnl
  <H3>Predicates for pointset powerset iterators and disjuncts.</H3>
Iterators allow the user to examine and change individual elements
(called here disjuncts) of a pointset powerset.
Detailed descriptions for adding and removing disjuncts can be found in the
main \extref{preamble, PPL User Manual} in Section
\extref{Adding_a_Disjunct, Adding a Disjunct}.
The following predicates support useful operations
on these iterators and disjuncts via the usual handles.
m4_pushdef(`m4_Pointset_Powerset_a_beginend_replacements',
  `beginning, end')`'dnl
')

dnl -----------------------------------------------------------------
dnl Other ad hoc predicates for the pointset_powerset domains.
dnl Note that these are only used for the configure independent manual.
dnl -----------------------------------------------------------------
m4_define(`m4_subcategory_pps_other', `dnl
  <H3>Other Ad Hoc Predicates for the pointset powerset domains.</H3>
Collected here are some other predicates that are specific to
pointset powersets of C polyhedra;
these provide operations for
simplifying the powerset,
geometric comparisons
and widening and extrapolation.
Detailed descriptions of these can be found in the
main \extref{preamble, PPL User Manual} in Sections
\extref{Geometric_Comparisons, Geometric Comparisons},
\extref{Certificate_Based_Widenings, Certificate-Based Widenings},
\extref{Powerset_Extrapolation_Operators, Powerset Extrapolation Operators}.
')


dnl -----------------------------------------------------------------
dnl Documentation for each of the build predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_build_doc', `dnl
<P><CODE>ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension(+Dimension_Type, +Universe_or_Empty, -Handle)</CODE><BR>
   <EM>Builds a new @A_TOPOLOGY@`'m4_this_ppl_one_object \f$\cP\f$
   with \c Dimension_Type dimensions; it is empty
   or the universe depending on whether \c Atom
   is \c empty or \c universe|COMMA| respectively.
   \c Handle is unified with the handle for \f$\cP\f$.
   Thus the query
\code
   ?- ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension(3|COMMA| universe|COMMA| X).
\endcode
   creates the @A_TOPOLOGY@`'m4_this_ppl_one_object defining the
   3-dimensional vector space
   \f$\Rset^3\f$ with \c X bound to a valid handle for accessing it.</EM>

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_build_doc',
`dnl
<P><CODE>ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s(+@!BUILD_REPRESENT@_System, -Handle)</CODE><BR>
  <EM>Builds a new @A_TOPOLOGY@`'m4_this_ppl_one_object \p P from
  \c @!BUILD_REPRESENT@_System.
  \c Handle is unified with the handle for \p P.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the convert predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_convert_doc', `dnl
<P><CODE>ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@(+Handle_1, -Handle_2)</CODE><BR>
  <EM>Builds a new @A_TOPOLOGY@`'m4_this_ppl_one_object \p P_1 from
   the @!A_FRIEND@ referenced by handle \c Handle_1.
  \c Handle_2 is unified with the handle for \p P_1.</EM>

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_convert_doc', `dnl
<P><CODE>ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity(+Handle, +Complexity, -Handle)</CODE><BR>
  <EM>Builds a new @A_TOPOLOGY@`'m4_this_ppl_one_object \p P_1 from
   the @!A_FRIEND@ referenced by handle \c Handle_1
  using an algorithm whose complexity does not exceed \c Complexity;
  \c Handle_2 is unified with the handle for \p P_1.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for the desctructor;
dnl -----------------------------------------------------------------

m4_define(`ppl_delete_@CLASS@_destruct_doc',
`dnl
<P><CODE>ppl_delete_@CLASS@(+Handle)</CODE><BR>
  <EM>Invalidates the handle  referenced by \c Handle:
  this makes sure the corresponding resources will eventually be released.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the simple test predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_testpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_@HAS_PROPERTY@(+Handle)</CODE><BR>
   <EM>Succeeds if and only if the m4_this_ppl_one_object referenced by
   \c Handle @A_HAS_PROPERTY@.</EM>

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_testpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_bounds_from_@ABOVEBELOW@(+Handle, +Lin_Expr)</CODE><BR>
   <EM>Succeeds if and only if <CODE>Lin_Expr</CODE> is bounded from @ABOVEBELOW@
   in the m4_this_ppl_one_object referenced by \c Handle.</EM>

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_testpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_@COMPARISON@_@CLASS@(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Succeeds if and only if the m4_this_ppl_one_object referenced by
   <CODE>Handle_2</CODE> @A_COMPARISON@
   the m4_this_ppl_one_object referenced by <CODE>Handle_1</CODE>.</EM>

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_testpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_equals_@CLASS@(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Succeeds if and only if the m4_this_ppl_one_object referenced by
   <CODE>Handle_1</CODE> is
   equal to the m4_this_ppl_one_object referenced by <CODE>Handle_2</CODE>.</EM>

')

m4_define(`ppl_@CLASS@_constrains_testpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_constrains(+Handle, +PPL_Var)</CODE><BR>
  <EM>Succeeds if and only if the m4_this_ppl_one_object referenced by
   \c Handle constrains the dimension \c PPL_Var.</EM>

')

m4_define(`ppl_@CLASS@_OK_testpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_OK(+Handle)</CODE><BR>
   <EM>Succeeds only if the m4_this_ppl_one_object referenced by
   \c Handle is well formed|COMMA| i.e.|COMMA| if it
   satisfies all its implementation invariants.
   Useful for debugging purposes.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the "get information" predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_constpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_has_@UPPERLOWER@_bound(+Handle, +Var, ?Coeff_1, ?Coeff_2, ?Boolean)</CODE><BR>
  <EM>Succeeds if and only if \c Var is @UPPERLOWER@ bounded in
  m4_this_ppl_one_object \p P referenced by \c Handle. It is assumed
  that \p P is not empty and its space dimension is greater than or equal
  to that of \c Var|COMMA| otherwise the behavior is undefined.

  \c Coeff_1 is unified with the numerator of the @UPPERLOWER@ bound
  and \c Coeff_2 with the denominator of the @UPPERLOWER@ bound.
  If the @UPPERLOWER@ is included in \p P|COMMA| then \c Boolean is unified
  with the atom \c true and|COMMA| otherwise|COMMA| unified with
  the atom \c false.</EM>

')

m4_define(`ppl_@CLASS@_@MAXMIN@_constpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_@MAXMIN@imize(+Handle, +Lin_Expr, ?Coeff_1, ?Coeff_2, ?Boolean)</CODE><BR>
  <EM>Succeeds if and only if m4_this_ppl_one_object \p P referenced by \c Handle
  is not empty and \c Lin_Expr is bounded from @A_MAXMIN@ in \p P.

  \c Coeff_1 is unified with the numerator of the @B_MAXMIN@ value
  and \c Coeff_2 with the denominator of the @B_MAXMIN@ value.
  If the @B_MAXMIN@ is also the @MAXMIN@imum|COMMA| \c Boolean is unified
  with the atom \c true and|COMMA| otherwise|COMMA| unified with
  the atom \c false.</EM>

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_constpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_@MAXMIN@imize_with_point(+Handle, +Lin_Expr, ?Coeff_1, ?Coeff_2, ?Boolean, ?Point)</CODE><BR>
  <EM>Succeeds if and only if m4_this_ppl_one_object \p P referenced by \c Handle
  is not empty and \c Lin_Expr is bounded from @A_MAXMIN@ in \p P.

  \c Coeff_1 is unified with the numerator of the @B_MAXMIN@ value
  and \c Coeff_2 with the denominator of the @B_MAXMIN@ value
  and \c Point with a point or closure point where \c Lin_Expr reaches
  this value.
  If the @B_MAXMIN@ is also the @MAXMIN@imum|COMMA| \c Boolean is unified
  with the atom \c true and|COMMA| otherwise|COMMA| unified with the
  atom \c false.</EM>

')

m4_define(`ppl_@CLASS@_@DIMENSION@_constpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_@DIMENSION@(+Handle, ?Dimension_Type)</CODE><BR>
   <EM>Unifies \c Dimension_Type with the @A_DIMENSION@
   the m4_this_ppl_one_object referenced by \c Handle.</EM>

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_constpoly_doc',
`dnl
<P><CODE>ppl_@CLASS@_relation_with_@RELATION_REPRESENT@(+Handle, +@!RELATION_REPRESENT@, ?Relation_List)</CODE><BR>
  <EM>Unifies \c Relation_List with the list of relations the
  m4_this_ppl_one_object referenced by \c Handle has with \c @!RELATION_REPRESENT@.
  The possible relations are listed in the
  grammar rules above.</EM>

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_constpoly_doc',
`dnl
m4_pushdef(`m4_b_class_represent_replacements', `satisfied by, satisfied by')
m4_pushdef(`m4_Polyhedron_b_class_represent_replacements',
         `satisfied by, satisfied by, for')
m4_pushdef(`m4_Grid_b_class_represent_replacements',
         `satisfied by, satisfied by, for')
<P><CODE>ppl_@CLASS@_get_@CLASS_REPRESENT@s(+Handle, ?@!CLASS_REPRESENT@_System)</CODE><BR>
   <EM>Unifies \c @!CLASS_REPRESENT@_System with
   the @CLASS_REPRESENT@s (in the form of a list) in the @CLASS_REPRESENT@ system
   @B_CLASS_REPRESENT@ the m4_this_ppl_one_object referenced by \c Handle.</EM>

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_constpoly_doc',
`dnl
m4_pushdef(`m4_Polyhedron_b_class_represent_replacements',
         `satisfied by, for, satisfied by')
<P><CODE>ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s(+Handle, ?@!CLASS_REPRESENT@_System)</CODE><BR>
   <EM>Unifies \c @!CLASS_REPRESENT@_System with
   the @CLASS_REPRESENT@s (in the form of a list)
   in the minimized @CLASS_REPRESENT@ system
   @B_CLASS_REPRESENT@ the m4_this_ppl_one_object referenced by \c Handle.</EM>

')

m4_define(`ppl_@CLASS@_@MEMBYTES@_constpoly_doc',
 `dnl
m4_pushdef(`m4_a_membytes_replacements', `total, external')
<P><CODE>ppl_@CLASS@_@MEMBYTES@(+Handle, ?Number)</CODE><BR>
<EM>Unifies \c Number with the size of the @A_MEMBYTES@ memory in
bytes occupied by the m4_this_ppl_one_object referenced by \c Handle.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for the ascii dump;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_ascii_dump_dump_doc',
 `dnl
<P><CODE>ppl_@CLASS@_ascii_dump(+Handle)</CODE><BR>
  <EM>Dumps an ascii representation of the PPL internal state for
  the m4_this_ppl_one_object referenced by \c Handle on the standard output.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the "add to" predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_addto_doc',
`dnl
m4_popdef(`m4_a_class_represent_replacements')`'dnl
m4_pushdef(`m4_a_class_represent_replacements',
           `an equality or a non-strict inequality,
            an equality, a line|COMMA| ray or point')`'dnl
<P><CODE>ppl_@CLASS@_add_@CLASS_REPRESENT@(+Handle, +@!CLASS_REPRESENT@)</CODE><BR>
   <EM>Updates the m4_this_ppl_one_object referenced by \c Handle to
   one obtained by adding
   \c @!CLASS_REPRESENT@ to its @CLASS_REPRESENT@ system.
   For a C polyhedron|COMMA| \c @!CLASS_REPRESENT@ must be
   @A_CLASS_REPRESENT@.</EM>

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_addto_doc',
`dnl
m4_popdef(`m4_a_class_represent_replacements')`'dnl
m4_pushdef(`m4_a_class_represent_replacements',
           `equalities and non-strict inequalities,
            equalities, lines|COMMA| rays and points')`'dnl
<P><CODE>ppl_@CLASS@_add_@CLASS_REPRESENT@s(
                  +Handle,
                  +@!CLASS_REPRESENT@_System)</CODE><BR>
   <EM>Updates the m4_this_ppl_one_object referenced by \c Handle to
   one obtained by adding to its @CLASS_REPRESENT@ system the @CLASS_REPRESENT@s in
   \c @!CLASS_REPRESENT@_System.
   For a C polyhedron|COMMA| \c @!CLASS_REPRESENT@s must be a list of
   @A_CLASS_REPRESENT@.</EM>

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_addto_doc',
`dnl
<P><CODE>ppl_@CLASS@_refine_with_@REFINE_REPRESENT@(
                  +Handle,
                  +@!REFINE_REPRESENT@)</CODE><BR>
   <EM>Updates the m4_this_ppl_one_object referenced by \c Handle to
   one obtained by refining its @REFINE_REPRESENT@ system with
   \c @!REFINE_REPRESENT@.</EM>

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_addto_doc',
`dnl
<P><CODE>ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s(
                  +Handle,
                  +@!REFINE_REPRESENT@_System)</CODE><BR>
   <EM>Updates the m4_this_ppl_one_object referenced by \c Handle to
   one obtained by refining its @REFINE_REPRESENT@ system with
   the @REFINE_REPRESENT@s in \c @!REFINE_REPRESENT@_System.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the binary operator predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_@BINOP@_binop_doc',
`dnl
<P><CODE>ppl_@CLASS@_@BINOP@(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P referenced by \c Handle_1
   the @A_BINOP@ of \p P and the m4_this_ppl_one_object referenced by \c Handle_2.</EM>

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_binop_doc',
`dnl
<P><CODE>ppl_@CLASS@_@UB_EXACT@(+Handle_1, +Handle_2)</CODE><BR>
    <EM>Succeeds if the least upper bound of the
    m4_this_ppl_one_object \p P_1 referenced by \c Handle_1
    with the m4_this_ppl_one_object referenced by \c Handle_2 is exact;
    in which case the least upper bound is assigned
    to \p P_1;
    fails otherwise.</EM>

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_binop_doc',
 `dnl
<P><CODE>ppl_@CLASS@_linear_@PARTITION@(+Handle_1, +Handle_2, -Handle_3,
                                        -Handle_4)</CODE><BR>
  <EM>\c Handle_1 and \c Handle_2 are handles for elements \p P_1 and \p P_2
  in the @CLASS@ domain. The predicate unifies handle
  \c Handle_3 to a reference to the intersection of
  \p P_1 and \p P_2 and \c Handle_4 to a reference to
  a pointset powerset of nnc polyhedra \p P_4;
  where \p P_4 is the linear partition of \p P_1 with respect to \p P_2.
  This predicate is only provided if the
  class \c Pointset_Powerset_NNC_Polyhedron
  has been enabled when configuring the library.</EM>

')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_binop_doc',
 `dnl
<P><CODE>ppl_@CLASS@_approximate_@PARTITION@(+Handle_1, +Handle_2, ?Boolean,
                                             -Handle_3, -Handle_4)</CODE><BR>
  <EM>\c Handle_1 and \c Handle_2 are handles for elements \p P_1 and \p P_2
  in the @CLASS@ domain. The predicate unifies handle
  \c Handle_3 to a reference to the intersection of
  \p P_1 and \p P_2 and \c Handle_4 to a reference to
  a pointset powerset of grids \p P_4 where:<BR>
  if there is a finite linear partition of \p P_1 wrt \p P_2
   \c Boolean is unified with the atom \c true and
   \p P_4 is the linear partition;<BR>
  otherwise \c Boolean is unified with the atom \c false
    and \p P_4 is set to the singleton set that contains \p P_2.<BR>
  This predicate is only provided if the
  class \c Pointset_Powerset_Grid
  has been enabled when configuring the library.</EM>

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_binop_doc',
`dnl
<P><CODE>ppl_@CLASS@_simplify_using_context_assign(+Handle_1, +Handle_2, ?Boolean)</CODE><BR>
    <EM>Succeeds if and only if the intersection of m4_this_ppl_one_object \p P_1
    referenced by \c Handle_1 and the m4_this_ppl_one_object \p P_2 referenced by \c Handle_2
    is non-empty.
    Assigns to \p P_1 its meet-preserving simplification with
    respect to \p P_2.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the widen and extrapolation predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_widen_doc',
`dnl
<P><CODE>ppl_@CLASS@_widening_assign_with_tokens(+Handle_1, +Handle_2, +C_unsigned_1, ?C_unsigned_2)</CODE><BR>
<EM>Same as predicate `'dnl
m4_ifelse(
  m4_this_interface_class, Polyhedron,
    <CODE>ppl_@CLASS@_H79_widening_assign_with_tokens</CODE>/4,
  m4_this_interface_class, Grid,
    <CODE>ppl_@CLASS@_congruence_widening_assign_with_tokens</CODE>/4,
  m4_this_ppl_group, box,
    <CODE>ppl_@CLASS@_CC76_widening_assign_with_tokens</CODE>/4,
  m4_this_ppl_group, wr_shape,
    <CODE>ppl_@CLASS@_H79_widening_assign_with_tokens</CODE>/4)</EM>

')

m4_define(`ppl_@CLASS@_widening_assign_widen_doc',
`dnl
<P><CODE>ppl_@CLASS@_widening_assign(+Handle_1, +Handle_2)</CODE><BR>
<EM>Same as predicate `'dnl
m4_ifelse(
  m4_this_interface_class, Polyhedron,
    <CODE>ppl_@CLASS@_H79_widening_assign</CODE>/2,
  m4_this_interface_class, Grid,
    <CODE>ppl_@CLASS@_congruence_widening_assign</CODE>/2,
  m4_this_ppl_group, box,
    <CODE>ppl_@CLASS@_CC76_widening_assign</CODE>/2,
  m4_this_ppl_group, wr_shape,
    <CODE>ppl_@CLASS@_H79_widening_assign</CODE>/2)</EM>

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_widen_doc',
`dnl
<P><CODE>ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens(+Handle_1, +Handle_2, +C_unsigned_1, ?C_unsigned_2)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P_1 referenced by \c Handle_1
   the @WIDEN@-widening of \p P_1 with the m4_this_ppl_one_object
   referenced by \c Handle_2.
   The widening with tokens delay
   technique is applied with \c C_unsigned_1 tokens;
   \c C_unsigned_2 is unified with the number of tokens
   remaining at the end of the operation.</EM>

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_widen_doc',
`dnl
<P><CODE>ppl_@CLASS@_@WIDEN@_widening_assign(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P_1 referenced by \c Handle_1
   the @WIDEN@-widening of \p P_1 with the m4_this_ppl_one_object
   referenced by \c Handle_2.</EM>

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_widen_doc',
`dnl
m4_pushdef(`m4_a_limitedbounded_replacements', `,
  |COMMA| further intersected with the smallest box containing \\p P_1')
<P><CODE>ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens(+Handle_1, +Handle_2, +Constraint_System, +C_unsigned_1, ?C_unsigned_2)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P_1 referenced by \c Handle_1
   the @WIDENEXPN@-widening of \p P_1 with the m4_this_ppl_one_object
   referenced by \c Handle_2 intersected with the constraints in
   \c Constraint_System that are
   satisfied by all the points of \p P_1@A_LIMITEDBOUNDED@.
   The widening with tokens delay
   technique is applied with \c C_unsigned_1 tokens;
   \c C_unsigned_2 is unified with the number of tokens
   remaining at the end of the operation.</EM>

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_widen_doc',
`dnl
m4_pushdef(`m4_a_limitedbounded_replacements', `,
  |COMMA| further intersected with the smallest box containing \\p P_1')
<P><CODE>ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(+Handle_1, +Handle_2, +Constraint_System)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P_1 referenced by \c Handle_1
   the @WIDENEXPN@-widening of \p P_1 with the m4_this_ppl_one_object
   referenced by \c Handle_2 intersected with the constraints in
   \c Constraint_System that are
   satisfied by all the points of \p P_1@A_LIMITEDBOUNDED@.</EM>

')

 m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_widen_doc',
 `dnl
<P><CODE>ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P_1 referenced by \c Handle_1
   the @EXTRAPOLATION@-narrowing of \p P_1 and the m4_this_ppl_one_object
   referenced by \c Handle_2.</EM>

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_widen_doc',
 `dnl
<P><CODE>ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Assigns to the pointset powerset \p P_1 referenced by
   \c Handle_1 the \extref{pps_certificate_widening|COMMA| BHZ03-widening}
    between \p P_1 and the pointset powerset referenced by
   \c Handle_2|COMMA| using the @DISJUNCT_WIDEN@-widening
    certified by the convergence certificate for @A_DISJUNCT_WIDEN@.</EM>

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_widen_doc',
 `dnl
<P><CODE>ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign(+Handle_1, +Handle_2, C_unsigned)</CODE><BR>
    <EM>Assigns to the pointset powerset \p P_1 referenced by
   \c Handle_1 the result of applying the
    \extref{pps_bgp99_extrapolation|COMMA| BGP99 extrapolation operator}
    between \p P_1 and the pointset powerset referenced by
   \c Handle_2|COMMA| using the @DISJUNCT_WIDEN@-widening
    and the cardinality threshold \c C_unsigned.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the transform predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_swap_build_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_swap(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Swaps the m4_this_ppl_one_object referenced by \c Handle_1
   with the m4_this_ppl_one_object referenced by \c Handle_2.</EM>

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_@SIMPLIFY@(+Handle)</CODE><BR>
  <EM>Assigns to the m4_this_ppl_one_object referenced by \c Handle
  its @A_SIMPLIFY@.</EM>

')

m4_define(`m4_a_affimage_replacements',
  `image, preimage')
m4_define(`m4_b_affimage_replacements',
  `assigning the affine, substituting the affine')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_trans_doc',
`dnl
m4_define(`m4_a_affimage_replacements',
  `image, preimage')`'dnl
m4_define(`m4_b_affimage_replacements',
  `assigning the affine, substituting the affine')`'dnl
<P><CODE>ppl_@CLASS@_@AFFIMAGE@(+Handle, +PPL_Var, +Lin_Expr, +Coeff)</CODE><BR>
   <EM>Transforms the m4_this_ppl_one_object referenced by \c Handle
   @B_AFFIMAGE@ expression for \c Lin_Expr/\c Coeff to \c PPL_Var.</EM>

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_bounded_@AFFIMAGE@(+Handle, +PPL_Var, +Lin_Expr_1, +Lin_Expr_2, +Coeff)</CODE><BR>
  <EM>Assigns to m4_this_ppl_one_object \p P referenced by \c Handle
  the generalized @A_AFFIMAGE@ with respect to the
  generalized affine transfer relation
  \c Lin_Expr_1/Coeff
    \f$\leq\f$ \c PPL_Var
      \f$\leq\f$ \c Lin_Expr_2/Coeff.</EM>

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_generalized_@AFFIMAGE@(+Handle, +PPL_Var, +Relation_Symbol, +Lin_Expr, +Coeff)</CODE><BR>
  <EM>Assigns to m4_this_ppl_one_object \p P referenced by \c Handle
  the generalized @A_AFFIMAGE@ with respect to the
  generalized affine transfer relation
   \c PPL_Var \f$\bowtie\f$ \c Lin_Expr/\c Coeff|COMMA|
   where \f$\bowtie\f$ is the symbol
   represented by \c Relation_Symbol.</EM>

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs(+Handle, +Lin_Expr_1, +Relation_Symbol, +Lin_Expr_2)</CODE><BR>
  <EM>Assigns to m4_this_ppl_one_object \p P referenced by \c Handle
  the generalized @A_AFFIMAGE@ with respect to the
  generalized affine transfer relation
   \c Lin_Expr_1 \f$\bowtie\f$ \c Lin_Expr_2|COMMA|
   where \f$\bowtie\f$ is the symbol
   represented by \c Relation_Symbol.</EM>

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_trans_doc',
 `dnl
<P><CODE>ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence(+Handle, +PPL_Var, +Relation_Symbol, +Lin_Expr, +Coeff_1, +Coeff_2)</CODE><BR>
  <EM>Assigns to m4_this_ppl_one_object \p P referenced by \c Handle
  the generalized @A_AFFIMAGE@ with respect to the
  generalized affine transfer relation
   \c Lin_Expr_1 \f$\equiv_f\f$ \c Lin_Expr_2/\c Coeff_1|COMMA|
   where \c Coeff_2 is bound to the modulus \p f.</EM>

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence(+Handle, +Lin_Expr_1, +Relation_Symbol, +Lin_Expr_2, +Coeff)</CODE><BR>
  <EM>Assigns to m4_this_ppl_one_object \p P referenced by \c Handle
  the generalized @A_AFFIMAGE@ with respect to the
  generalized affine transfer relation
   \c Lin_Expr_1  \f$\equiv_f\f$ \c Lin_Expr_2.
   where \c Coeff is bound to the modulus \p f.</EM>

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_unconstrain_space_dimension(+Handle, +PPL_Var)</CODE><BR>
  <EM>Modifies the m4_this_ppl_one_object \p P referenced by \c Handle by unconstraining
  the space dimension \c PPL_Var.</EM>

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_unconstrain_space_dimensions(+Handle, +List_of_PPL_Var)</CODE><BR>
  <EM>Modifies the m4_this_ppl_one_object \p P referenced by \c Handle by unconstraining
  the space dimensions that are specified in \c List_of_PPL_Var.
  The presence of duplicates in \c List_of_PPL_Var is a waste
  but an innocuous one.</EM>

')

m4_define(`ppl_@CLASS@_wrap_assign_trans_doc',
`dnl
<P><CODE>ppl_@CLASS@_wrap_assign(+Handle, +List_of_PPL_Var, +Width, +Representation, +Overflow, +Constraint_System, +C_unsigned, +Boolean)</CODE><BR>
  <EM>Transforms the m4_this_ppl_one_object \p P referenced by
    <CODE>Handle</CODE> by wrapping the dimensions given by
    <CODE>List_of_PPL_Vars</CODE> while respecting the specified
    <CODE>Width</CODE>|COMMA| <CODE>Representation</CODE> and
    <CODE>Overflow</CODE> behavior of all these variables. The
    <CODE>Constraint_System</CODE> represents the conditional or
    looping construct guard with respect to which wrapping is
    performed.  The non-negative integer <CODE>C_unsigned</CODE> and
    <CODE>Boolean</CODE> allow control of the complexity/precision
    ratio; higher values for <CODE>C_unsigned</CODE> will lead to
    possibly greater precision while a true value for
    <CODE>Boolean</CODE> indicates that the space dimensions should be
    wrapped individually.  See Section \extref{Wrapping_Operator|COMMA|
    Wrapping Operator} for a more detailed description of this
    operator.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for the concatenation predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_@BINOP@_concat_doc',
`dnl
m4_popdef(`m4_binop_replacements')
m4_popdef(`m4_Polyhedron_binop_replacements')
m4_popdef(`m4_a_binop_replacements')
m4_popdef(`m4_Polyhedron_a_binop_replacements')
m4_pushdef(`m4_binop_replacements', `concatenate_assign')
m4_pushdef(`m4_Polyhedron_binop_replacements', `concatenate_assign')
m4_pushdef(`m4_a_binop_replacements', `concatenation')
m4_pushdef(`m4_Polyhedron_a_binop_replacements', `concatenation')
 <P><CODE>ppl_@CLASS@_@BINOP@(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Assigns to the m4_this_ppl_one_object \p P referenced by \c Handle_1
   the @A_BINOP@ of \p P and the m4_this_ppl_one_object referenced by \c Handle_2.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the space changing predicates;
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_spacemod_doc',
`dnl
<P><CODE>ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@(+Handle, +Dimension_Type)</CODE><BR>
  <EM>Adds \c Dimension_Type new dimensions to the space enclosing
  the m4_this_ppl_one_object \p P referenced by \c Handle
  and @EMBEDPROJECT@s \p P in this space.</EM>

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_spacemod_doc',
`dnl
<P><CODE>ppl_@CLASS@_remove_space_dimensions(+Handle, +List_of_PPL_Vars)</CODE><BR>
  <EM>Removes from the vector space enclosing
  the m4_this_ppl_one_object \p P referenced by \c Handle the space dimensions that
  are specified in \c List_of_PPL_Var.  The presence
  of duplicates in  \c List_of_PPL_Var is a waste but an innocuous one.</EM>

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_spacemod_doc',
`dnl
<P><CODE>ppl_@CLASS@_remove_higher_space_dimensions(+Handle, +Dimension_Type)</CODE><BR>
  <EM>Removes the higher dimensions from the vector space enclosing
  the m4_this_ppl_one_object \p P referenced by \c Handle
  so that|COMMA| upon successful return|COMMA| the new space dimension is
  \c Dimension_Type.</EM>

')

m4_define(`ppl_@CLASS@_expand_space_dimension_spacemod_doc',
`dnl
<P><CODE>ppl_@CLASS@_expand_space_dimension(+Handle, +PPL_Var, +Dimension_Type)</CODE><BR>
  <EM>Expands the \c  PPL_Var-th
  dimension of the vector space enclosing
  the m4_this_ppl_one_object referenced by \c Handle to
  \c Dimension_Type new space dimensions.</EM>

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_spacemod_doc',
`dnl
<P><CODE>ppl_@CLASS@_fold_space_dimensions(+Handle, +List_of_PPL_Vars, +PPL_Var)</CODE><BR>
  <EM>Modifies the m4_this_ppl_one_object referenced by \c Handle
  by folding the
  space dimensions contained in \c List_of_PPL_Vars
  into dimension \c PPL_Var.
  The presence of duplicates in \c List_of_PPL_Vars is a waste
  but an innocuous one.</EM>

')

m4_define(`ppl_@CLASS@_map_space_dimensions_spacemod_doc',
`dnl
<P><CODE>ppl_@CLASS@_map_space_dimensions(+Handle, +P_Func)</CODE><BR>
  <EM>Remaps the dimensions of the vector space according to a
  partial function.
  This function is specified by means of the \c P_Func|COMMA|
  which has \p n entries.
  The result is undefined if \c P_Func does not encode a partial
  function.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for each of the powerset predicates involving
dnl iterators and disjuncts;
dnl -----------------------------------------------------------------

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_pps_iter_doc',
`dnl
<P><CODE>ppl_new_@CLASS@_iterator_from_iterator(+Iterator_1, -Iterator_2)</CODE><BR>
  <EM>Builds a new iterator \p it from the iterator referenced by
  \c Iterator_1. <CODE>Iterator_2</CODE> is unified with the handle
  for \p it.</EM>

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_pps_other_doc',
`dnl
m4_pushdef(`m4_Pointset_Powerset_simplify_replacements',
  `pairwise_reduce, omega_reduce')
m4_pushdef(`m4_Pointset_Powerset_a_simplify_replacements',
  `pairwise reduction, omega reduction')
<P><CODE>ppl_@CLASS@_@SIMPLIFY@(+Handle)</CODE><BR>
   <EM>Assigns the result of @A_SIMPLIFY@ on
   the pointset powerset referenced by \c Handle.</EM>

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_pps_iter_doc',
 `dnl
<P><CODE>ppl_@CLASS@_iterator_equals_iterator(+Iterator_1, +Iterator_2)</CODE><BR>
   <EM>Succeeds if and only if the iterator referenced by
   \c Iterator_1 is equal to the iterator referenced by
   \c Iterator_2.</EM>

')

 m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_pps_iter_doc',
 `dnl
<P><CODE>ppl_@CLASS@_@BEGINEND@_iterator(+Handle, -Iterator)</CODE><BR>
  <EM>Unifies \c Iterator with a handle to an iterator "pointing"
  to the @A_BEGINEND@ of
  the sequence of disjuncts of the powerset referred to by \c Handle.</EM>

')

m4_define(`ppl_delete_@CLASS@_iterator_pps_iter_doc',
 `dnl
<P><CODE>ppl_delete_@CLASS@_iterator(+Iterator)</CODE><BR>
  <EM>Invalidates the handle referenced by <CODE>Iterator</CODE>:
  this makes sure the corresponding resources
  will eventually be released.</EM>

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_pps_iter_doc',
 `dnl
m4_pushdef(`m4_Pointset_Powerset_a_incdec_replacements',
  `next, previous')
<P><CODE>ppl_@CLASS@_iterator_@INCDEC@(+Iterator)</CODE><BR>
  <EM>@!INCDEC@s the iterator referenced by \c Iterator
  so that it "points" to the @A_INCDEC@ disjunct.</EM>

')

m4_define(`ppl_@CLASS@_get_disjunct_pps_iter_doc',
 `dnl
<P><CODE>ppl_@CLASS@_iterator_get_disjunct(+Iterator, -Handle)</CODE><BR>
  <EM>Unifies with \c Handle a reference to the disjunct referred
  to by \c Iterator_1.</EM>

')

m4_define(`ppl_@CLASS@_drop_disjunct_pps_iter_doc',
 `dnl
<P><CODE>ppl_@CLASS@_drop_disjunct(+Handle, +Iterator)</CODE><BR>
  <EM>If \p it is the iterator referred to by \c Iterator|COMMA|
  drops from the pointset powerset referenced by \c Handle
  the disjunct pointed to by \p it and
  assigns to \p it an iterator to the next disjunct.</EM>

')

m4_define(`ppl_@CLASS@_drop_disjuncts_pps_iter_doc',
 `dnl
<P><CODE>ppl_@CLASS@_drop_disjuncts(+Handle, +Iterator_1, +Iterator_2)</CODE><BR>
  <EM>If \p it_1 and \p it_2 are the iterators referred to by \c Iterator_1
  and \c Iterator_2|COMMA| respectively|COMMA|
  drops from the pointset powerset referenced by \c Handle
   all the disjuncts from \p it_1 to \p it_2 (excluded).</EM>

')

m4_define(`ppl_@CLASS@_add_disjunct_pps_iter_doc',
 `dnl
<P><CODE>ppl_@CLASS@_add_disjunct(+Handle_1, +Handle_2)</CODE><BR>
  <EM>Adds to the pointset powerset referenced by \c Handle_1 a disjunct
  referred to by \c Handle_2.</EM>

')

dnl -----------------------------------------------------------------
dnl Documentation for _other_ ad hoc powerset predicates;
dnl Note: this is only needed for the configure independent manual.
dnl -----------------------------------------------------------------

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_pps_other_doc',
`dnl
m4_pushdef(`m4_Pointset_Powerset_comparison_replacements',
         `geometrically_covers, geometrically_equals')
m4_pushdef(`m4_Pointset_Powerset_a_comparison_replacements',
         `geometrically covers, geometrically equals')
<P><CODE>ppl_@CLASS@_@COMPARISON@_@CLASS@(+Handle_1, +Handle_2)</CODE><BR>
   <EM>Succeeds if and only if the pointset powerset referenced by
   \c Handle_2 @A_COMPARISON@
   the pointset powerset referenced by \c Handle_1;
   see Section \extref{Geometric_Comparisons|COMMA| Geometric Comparisons}
  in the main \extref{preamble|COMMA| PPL User Manual}.</EM>

')
