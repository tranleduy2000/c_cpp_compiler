m4_divert(-1)

dnl This m4 file includes macro definitions for:
dnl - application independent helper macros used here and by other m4 files.
dnl - defining the main code generation macro m4_all_code.

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

dnl ---------------------------------------------------------------------
dnl           --- MACRO GROUPS and DEPENDENCIES ---
dnl
dnl For ease of keeping track of these macros and their dependencies,
dnl the macro definitions are separated by the "===" comments into groups.
dnl
dnl Where a macro is only used within the group, it is labelled as "LOCAL".
dnl Where it is used in this file but by another macro group, it is
dnl labelled as "PRIVATE" and an indication of which group uses it.
dnl Where it may be used in the generator files, it is labelled
dnl as "PUBLIC".
dnl
dnl Some simple macros may be redefined by generator files and we
dnl indicate that here is the default definition by the label
dnl "DEFAULT DEF".
dnl
dnl The macro groups and the dependencies are:
dnl
dnl HELPERS (m4_forloop, m4_upcase, m4_downcase, m4_capfirstletter,
dnl          m4_capfirstletters, m4_changecase, m4_add_one_after_underscore,
dnl          m4_ifndef, m4_nargs, m4_arg, m4_arg_counter, m4_echo_unquoted,
dnl          m4_echo_quoted, m4_seq_intersection, m4_seq_difference
dnl          are all public.)
dnl          WARNING: some may not be used and may not be fully tested.
dnl
dnl ALL_CLASSES (m4_all_code is public)
dnl |-- m4_interface_generator_common_dat.m4 (m4_init_class_definitions)
dnl `-- ONE_CLASS (m4_one_class_code)
dnl     |-- FILTER_PROCEDURES (m4_filter_all_procedures)
dnl     |   `-- m4_interface_generator_common_dat.m4 (m4_group_names)
dnl     |-- PATTERNS_TO_REPLACEMENTS (m4_replace_all_patterns_in_string)
dnl     |-- PATTERNS (m4_pattern_annotation0, ...)
dnl     |-- REPLACEMENT_TEXT (m4_annotated_pattern_replacement,
dnl     |   |                 m4_def_replacements_for_extended_patterns,
dnl     |   |                 m4_def_extended_replacements)
dnl     |   |-- PATTERNS (m4_pattern_extension0, ...)
dnl     |   `-- m4_interface_generator_common_dat.m4
dnl     |         (m4_class_pattern_replacements)
dnl     |-- m4_interface_generator_<interface>_procedure_generator.m4
dnl     |    (m4_procedure_list)
dnl     `-- EXPAND_PROCEDURES (m4_get_schematic_code)
dnl
dnl Note that the macro names indicate that they are used by the parent
dnl macro group.
dnl ---------------------------------------------------------------------

dnl =====================================================================
dnl           *** HELPERS ****
dnl  The following are application independent helper macros
dnl =====================================================================

dnl All these helper macros are intended to be "PUBLIC"
dnl except for auxiliary macros, labelled as "LOCAL".

dnl m4_forloop(Iteration_Var, From_Value, To_Value, Loop_Body)
dnl
dnl Code copied from m4 documentation.
m4_define(`m4_forloop',
  `m4_pushdef(`$1', `$2')_m4_forloop(`$1', `$2', `$3', `$4')m4_popdef(`$1')')
m4_define(`_m4_forloop',
  `$4`'m4_ifelse($1, `$3', ,
    `m4_define(`$1', m4_incr($1))_m4_forloop(`$1', `$2', `$3', `$4')')')


dnl m4_upcase(String), m4_downcase(String)
dnl
dnl Code copied from m4 documentation.
dnl Examples: m4_upcase(`abCd')   ==> ABCD
dnl           m4_downcase(`abCd') ==> abcd
m4_define(`m4_upcase', `m4_translit(`$*', `a-z', `A-Z')')
m4_define(`m4_downcase', `m4_translit(`$*', `A-Z', `a-z')')

dnl m4_capfirstletter(String)
dnl
dnl Capitalises first letter of words that can have letters and underscores.
dnl Example: m4_capfirstletter(`xyz_abc') ==> Xyz_abc
m4_define(`m4_capfirstletter',
  `m4_regexp(`$1', `^\(\w\)\([\w_]*\)',
     m4_upcase(`\1')`\2')')

dnl m4_capfirstletters(String)
dnl
dnl Capitalises first letter of subwords that can have letters only.
dnl Example: m4_capfirstletter(`xyz_abc') ==> Xyz_Abc
m4_define(`m4_capfirstletters',
  `m4_ifelse(m4_index(`$1', `_'), -1,
     `m4_regexp(`$1', `^\(\w\)\(\w*\)',
       `m4_upcase(`\1')`\2'')',
         m4_regexp(`$1', `^\(\w\)\(\w*_\)\(\w\)\(\w*\)',
           `m4_upcase(`\1')`\2'm4_upcase(`\3')`\4''))')

m4_define(`m4_changecase',
  `m4_ifelse(m4_regexp(`$1', `[A-Z]'), -1,
    m4_capfirstletters($1),
    m4_downcase($1))`'dnl
')


dnl m4_add_one_after_underscore(String)
dnl
dnl Adds a 1 after any underscore (needed for Java interface code)..
dnl Example: m4_capfirstletter(`xyz_abc') ==> xyz_1abc
m4_define(`m4_add_one_after_underscore', `m4_patsubst(`$1', `_', `_1')')

dnl m4_ifndef(Macro, Default Definition)
dnl
dnl If Macro is defined, use that definition;
dnl otherwise use the Default Definition.
m4_define(`m4_ifndef', `m4_ifdef(`$1', $1, $2)')

dnl m4_nargs expands to the number of arguments.
dnl
dnl Code copied from m4 documentation.
m4_define(`m4_nargs', `$#')

dnl m4_arg(Ind, Arg1, Arg2, ...)
dnl
dnl m4_arg expands to Arg`'Ind
m4_define(`m4_arg',
`m4_ifelse($1, 0, , $1, 1, $2, `m4_arg(m4_decr($1), m4_shift(m4_shift($@)))')')

dnl m4_arg_counter(String, Arg1, Arg2, ...)
dnl
dnl If String = Arg`'Counter, m4_arg_counter expands to Counter
dnl where Counter is the least index for which this holds.
dnl Otherwise it expands to the empty string.
m4_define(`m4_arg_counter', `m4_arg_counter_aux(1, $@)')

dnl LOCAL
m4_define(`m4_arg_counter_aux', `dnl
m4_ifelse($#, 0, ,$#, 1, , $#, 2, ,
  $2, $3, $1,
  $#, 3, ,
  `m4_arg_counter_aux(m4_incr($1), $2, m4_shift(m4_shift(m4_shift($@))))')`'dnl
')

dnl m4_echo_unquoted(Args)
dnl
dnl Code copied from m4 documentation where it is called echo1.
m4_define(`m4_echo_unquoted', `$*')
dnl m4_echo_quoted(Args)
dnl
dnl Code copied from m4 documentation where it is called echo2.
m4_define(`m4_echo_quoted', `$@')

dnl ----------------------------------------------------------------------
dnl m4_two_seq_intersection,
dnl m4_three_seq_intersection and helper macros
dnl
dnl These find the intersection of two and three sequences, respectively.
dnl ----------------------------------------------------------------------
dnl
dnl m4_seq_intersection
dnl
dnl This macro with its helper macros below,
dnl intersects two or three sequences that must be previously defined
dnl as `m4_1st_sequence', `m4_2nd_sequence' and, if there is a third,
dnl `m4_3rd_sequence'. The number of sequences (2 or 3) must also be defined
dnl by the macro `m4_number_of_sequences'. The order of the
dnl intersected sequence is that of m4_1st_sequence.
dnl
dnl For example, if m4_1st_sequence is defined to be `a, b, c, d' and
dnl m4_2nd_sequence is defined to be `b, d, e, a, f',
dnl this macro is defined to be `a, b, d'.
m4_define(`m4_seq_intersection', `dnl
m4_define(`m4_add_one_first', 1)`'dnl
m4_patsubst(m4_seq_intersection_aux(m4_1st_sequence),
            |COMMA|, `, ')`'dnl
')

dnl LOCAL
dnl m4_seq_intersection_aux(...)
dnl
dnl The arguments are the first sequence to be intersected.
dnl It calls either the helper macro for 3 sequences or the helper
dnl macro for 2 sequences (depending on the number of sequences).
dnl It calls itself recursively with the tail of the sequence.
m4_define(`m4_seq_intersection_aux', `dnl
m4_ifelse($#, 0, , $#, 1,
  m4_`'m4_num_of_sequences`'_seq_intersection_aux($1, m4_2nd_sequence),
  m4_`'m4_num_of_sequences`'_seq_intersection_aux($1, m4_2nd_sequence)`'dnl
`m4_seq_intersection_aux(m4_shift($@))')`'dnl
')

dnl LOCAL
dnl m4_3_seq_intersection_aux(String, ...)
dnl
dnl This is defined to be `String' if `String' also occurs
dnl in the 2nd or in a later argument position
dnl as well as in m4_3rd_sequence.
dnl It calls itself recursively with the tail of the sequence.
m4_define(`m4_3_seq_intersection_aux', `dnl
m4_ifelse($#, 0, , $#, 1, , $#, 2,
  `m4_ifelse($1, $2, `m4_2_seq_intersection_aux($1, m4_3rd_sequence)')',
  `m4_ifelse($1, $2, `m4_2_seq_intersection_aux($1, m4_3rd_sequence)',
`m4_3_seq_intersection_aux($1, m4_shift(m4_shift($@)))')')`'dnl
')

dnl LOCAL
dnl m4_2_seq_intersection_aux(String, ...)
dnl
dnl This is defined to be `String' if `String' also occurs
dnl in the 2nd or in a later argument position.
dnl It calls itself recursively with the tail of the sequence.
m4_define(`m4_2_seq_intersection_aux', `dnl
m4_ifelse($#, 0, , $#, 1, , $#, 2,
  `m4_ifelse($1, $2, `m4_add_one($1)')',
  `m4_ifelse($1, $2, `m4_add_one($1)',
`m4_2_seq_intersection_aux($1, m4_shift(m4_shift($@)))')')`'dnl
')

dnl LOCAL
dnl m4_add_one(String)
dnl
dnl This separates the new sequence temporarily with |COMMA| to avoid
dnl the `,' being mis-interpreted by m4.
m4_define(`m4_add_one', `dnl
m4_ifelse(m4_add_one_first, 1,
  $1`'m4_undefine(`m4_add_one_first'), |COMMA|$1)`'dnl
')

dnl ----------------------------------------------------------------------
dnl m4_seq_difference and helper macros
dnl
dnl This finds the difference of two sequences.
dnl ----------------------------------------------------------------------
dnl
dnl m4_seq_difference
dnl
dnl CHECKME: Currently this macro is not used.
dnl This macro with its helper macros below,
dnl finds the difference of two sequences that must be previously defined
dnl as `m4_1st_sequence', `m4_2nd_sequence'. The order of the
dnl difference sequence is that of m4_1st_sequence.
dnl
dnl For example, if m4_1st_sequence is defined to be `a, b, c, d' and
dnl m4_2nd_sequence is defined to be `d, e, a, f',
dnl this macro is defined to be `b, c'.
m4_define(`m4_seq_difference', `dnl
m4_patsubst(m4_seq_difference_aux(m4_1st_sequence),
            `^|COMMA|', `')`'dnl
')
m4_define(`m4_seq_differencexx', `dnl
m4_patsubst(m4_patsubst(m4_seq_difference_aux(m4_1st_sequence),
            `^|COMMA|', `'),
            |COMMA|, `, ')`'dnl
')

dnl LOCAL
dnl m4_seq_difference_aux(...)
dnl
dnl The arguments are the first sequence to be intersected.
dnl It calls either the helper macro for 3 sequences or the helper
dnl macro for 2 sequences (depending on the number of sequences).
dnl It calls itself recursively with the tail of the sequence.
m4_define(`m4_seq_difference_aux', `dnl
m4_ifelse($#, 0, , $#, 1,
  m4_2_seq_difference_aux($1, m4_2nd_sequence),
  `m4_2_seq_difference_aux($1, m4_2nd_sequence)`'dnl
m4_seq_difference_aux(m4_shift($@))')`'dnl
')

dnl LOCAL
dnl m4_2_seq_difference_aux(String, ...)
dnl
dnl This is defined to be `String' if `String' does not occur
dnl in the 2nd or in a later argument position.
dnl It calls itself recursively with the tail of the sequence.
m4_define(`m4_2_seq_difference_aux', `dnl
m4_ifelse($#, 0, , $#, 1, `m4_add_one($1)', $#, 2,
  `m4_ifelse($1, $2, , `m4_add_one($1)')',
  `m4_ifelse($1, $2, ,
`m4_2_seq_difference_aux($1, m4_shift(m4_shift($@)))')')`'dnl
')

dnl =====================================================================
dnl              *** PATTERNS ***
dnl Some processing that just involves the patterns themselves.
dnl =====================================================================

dnl ----------- General Info on Patterns --------------------------------
dnl Each pattern can be plain, have an extension (eg "A_") and/or
dnl be annotated by "!", and, for Java "1" where the
dnl latter can be combined with a "!".
dnl In the text being replaced, all patterns, extensions, and annotations
dnl are in upper case and the complete pattern is bracketed by the pattern
dnl delimiter.
dnl ---------------------------------------------------------------------

dnl PRIVATE
dnl Used in macro-group "REPLACEMENT_TEXT"
dnl
dnl DEFAULT_DEF
dnl m4_pattern_extension`'EIndex
dnl
dnl is defined for each allowed extension.
dnl These options can be added to as needed by any of the generator files.
dnl
m4_define(`m4_pattern_extension0', `')
m4_define(`m4_pattern_extension1', `A_')
m4_define(`m4_pattern_extension2', `B_')

dnl PRIVATE
dnl Used in macro-group "PATTERNS_TO_REPLACEMENTS"
dnl
dnl m4_pattern_annotation`'AIndex
dnl
dnl is defined here for AIndex values 0 and 1.
dnl - 0, which has the empty annotation,
dnl   means that the actual replacement must be unchanged:
dnl - 1, which has annotation !,
dnl   means that the actual replacement must be:
dnl   - lowercase, if the original has the first character uppercase, and
dnl   - the first letter of each word ("_" separated) uppercase, otherwise.
m4_define(`m4_pattern_annotation0', `')
m4_define(`m4_pattern_annotation1', `!')

dnl PUBLIC
dnl m4_java_prefix_one_annotation
dnl Only used by Java/jni/ppl_interface_generator_java_classes_cc_files.m4
dnl as the extra annotations are only needed there.
dnl PRIVATE
dnl m4_pattern_annotation`'AIndex
dnl Used in macro-group "PATTERNS_TO_REPLACEMENTS"
dnl
dnl m4_pattern_annotation`'AIndex is defined here for AIndex values 2 and 3:
dnl - 2, which has annotation 1,
dnl   means that the actual replacement must have a 1 after
dnl   the underscore;
dnl - 3, which has annotation 1!,
dnl   means that the actual replacement must have a 1 after
dnl   the underscore with the same change of case as for the annotation "!".
m4_define(`m4_java_prefix_one_annotation',
`m4_define(`m4_pattern_annotation2', `1')
m4_define(`m4_pattern_annotation3', `1!')')

dnl =====================================================================
dnl              *** REPLACEMENT_TEXT ***
dnl The precise replacing text is defined in
dnl m4_interfaces_generator_common_dat.m4
dnl We first retrieve this text by constructing the correct macro
dnl for the extended pattern. We then define the additional text
dnl processing needed based on the annotation of the pattern.
dnl =====================================================================

dnl LOCAL
dnl DEFAULT_DEF
dnl m4_replacement_extension`'EIndex
dnl
dnl is defined for each allowed extension EIndex.
dnl This should correspond in lowercase to m4_pattern_extension.
dnl
m4_define(`m4_replacement_extension0', `')
m4_define(`m4_replacement_extension1', `a_')
m4_define(`m4_replacement_extension2', `b_')

dnl PRIVATE
dnl Used in macro-group "PATTERNS_TO_REPLACEMENTS".
dnl
dnl m4_def_replacements_for_extended_patterns(RIndex, EIndex)
dnl m4_replace`'EIndex
dnl
dnl RIndex         - is the index of the replacement;
dnl EIndex         - is the index of the extension.
dnl For each possible EIndex value, m4_replace`'EIndex
dnl is defined for the RIndex replacement.
dnl
m4_define(`m4_def_replacements_for_extended_patterns', `dnl
m4_ifdef(`m4_pattern_extension$2',
  `m4_define(`m4_replace$2',
    `m4_arg($1, m4_replacements$2)')`'dnl
m4_def_replacements_for_extended_patterns($1, m4_incr($2))`'dnl
')`'dnl
')

dnl PRIVATE
dnl Used in macro-group "PATTERNS_TO_REPLACEMENTS".
dnl
dnl m4_annotated_pattern_replacement`'AIndex(EIndex)
dnl
dnl where AIndex in {0..3}
dnl Note that annotations 2 and 3 are only used by the Java interface.
dnl
dnl EIndex         - is the index of the extension.
dnl
dnl For each annotation and, for the single replacement
dnl m4_replace`'EIndex (already defined for the current pattern),
dnl this macro defines an exact replacement string to be used to
dnl replace the annotated and extended pattern.
dnl
m4_define(`m4_annotated_pattern_replacement0',
  `m4_replace$1')
m4_define(`m4_annotated_pattern_replacement1',
  `m4_changecase(m4_replace$1)')
m4_define(`m4_annotated_pattern_replacement2',
  `m4_add_one_after_underscore(m4_replace$1)')
m4_define(`m4_annotated_pattern_replacement3',
  `m4_add_one_after_underscore(m4_changecase(m4_replace$1))')

dnl PRIVATE
dnl Used in macro-group "PATTERNS_TO_REPLACEMENTS".
dnl
dnl m4_def_extended_replacements(Class_Counter, Pattern, EIndex)
dnl m4_replacements`'EIndex
dnl
dnl Class_Counter    - The current class counter;
dnl Pattern          - The current pattern;
dnl EIndex           - is the index of the replacement macro name extension.
dnl
dnl m4_replacements`'EIndex is defined to be the list
dnl of the replacements for Pattern with the EIndex extension
dnl and for the class indicated by Class_Counter.
dnl This uses m4_class_pattern_replacements/3 that is defined in
dnl ppl_interface_generator_common_dat.m4 and uses the data for
dnl the replacements defined in the same file.
dnl
m4_define(`m4_def_extended_replacements', `dnl
m4_ifdef(m4_replacement_extension`'$3, `dnl
m4_define(`m4_replacements$3',
  `m4_class_pattern_replacements($1, $2, m4_replacement_extension$3)`'dnl
m4_def_extended_replacements($1, $2, m4_incr($3))')')`'dnl
')

dnl =====================================================================
dnl              *** EXPAND_PROCEDURES ***
dnl  The next macros deal with expanding the schematic procedures.
dnl =====================================================================

dnl LOCAL
dnl m4_get_arity(Procedure_Flags)
dnl
dnl Procedure_Flags - The schematic code flags.
dnl
dnl If the substring "/Arity" is found in the Procedure_Flags,
dnl this macro expands to Arity.
m4_define(`m4_get_arity', `m4_regexp(`$1', `/\([0-9]*\)', \1)')

dnl LOCAL
dnl m4_get_attribute(Procedure_Flags)
dnl
dnl Procedure_Flags - The schematic code flags.
dnl
dnl If the substring "*nofail" is found in the Procedure_Flags,
dnl this macro expands to "nofail"
dnl This is only needed for the Ciao system code.
m4_define(`m4_get_attribute', `m4_regexp(`$1', `\*\(nofail\)', \1)')

dnl We next define the default values for two extra macros
dnl giving us more dynamic control by the individual code generators.

dnl LOCAL
dnl DEFAULT DEF
dnl m4_procedure_schema_extension
dnl
dnl Normally, the procedure schema is defined using the extension `code'
dnl but in the (Prolog) documentation this is varied according to the type of
dnl the procedure.
m4_define(`m4_procedure_schema_extension', `code')

dnl LOCAL
dnl DEFAULT DEF
dnl m4_procedure_schema_debug
dnl
dnl the default is to do nothing;
dnl However, by locally defining this to be
dnl m4_dumpdef($1, m4_procedure_schema_extension, we can check
dnl that all the procedure schemas are defined with the given extension.
m4_define(`m4_procedure_schema_debug', `')

dnl LOCAL
dnl DEFAULT DEF
dnl m4_expanded_procedure_schema(Procedure_Name,
dnl                              [Arity, Attribute],
dnl                              Start_Flag)
dnl
dnl Procedure_Name - The schematic name;
dnl Arity          - The arity of the procedure;
dnl Attribute      - The attribute `nofail' or `';
dnl Start_Flag     - 0 suppresses any separator
dnl                  (needed for expanding a comma separated list).
dnl
dnl The default definition.
dnl Appends "_code" to Procedure_Name so that it can match the name
dnl of one of the macros defined (if ever) in file *_code.m4 and get
dnl therefore expanded to the corresponding code schema.
dnl
dnl Here arguments Arity and Attribute are ignored.
dnl This macro is redefined when a different extension is needed.
dnl (e.g., in the Prolog system files).
dnl
dnl Note: the macro `$1_code' has to be called using builtin `indir'
dnl because it is not a legal m4 identifier (it contains `@').
dnl
dnl Some test generators and the Prolog system file generators redefine this
dnl macro to construct the code directly from the schema name, arity
dnl and its attributes.
m4_define(`m4_expanded_procedure_schema',
  `m4_ifdef($1_`'m4_procedure_schema_extension,
            `m4_indir($1_`'m4_procedure_schema_extension)',
            `m4_procedure_schema_debug($1)')`'dnl
')

dnl PRIVATE
dnl Used in macro-group "ONE_CLASS".
dnl
dnl m4_get_schematic_code(Procedure_Spec, Start_Flag, Class_Kind)
dnl
dnl Procedure_Spec - The schematic procedure name, complete with any flags;
dnl Start_Flag     - 0 suppresses any separator
dnl                  (needed for expanding a comma separated list);
dnl Class_Kind     - The current class kind.
dnl
dnl Procedure_Spec has the flags removed and expanded to the extended code.
m4_define(`m4_get_schematic_code', `dnl
m4_patsubst(`$1',
         `[ ]*\(ppl_[^ /]+\)\(.*\)',
         `m4_expanded_procedure_schema(\1, m4_get_arity(\2),
                                       m4_get_attribute(\2), $2, $3)')`'dnl
')

dnl =====================================================================
dnl              *** FILTER_PROCEDURES ***
dnl ====== The next set of macros filter the procedure specifications   =
dnl ====== according to the +/-group names in the extra flags in the    =
dnl ====== procedure specification.                                     =
dnl =====================================================================

dnl LOCAL
dnl m4_keep_or_throw_for_one_group(
dnl     Class_Counter, Procedure_Spec, +_or_-, Group)
dnl
dnl Class_Counter   - The current class counter;
dnl Procedure_Spec  - A schematic procedure name with flags still attached;
dnl +_or_-          - + or -;
dnl Group           - A group name.
dnl
dnl This checks if Class_Kind is in the list of class kinds defined
dnl by Group (in ppl_interface_generator_common_dat.m4);
dnl if it is, it checks if +Group or -Group
dnl (depending if +_or_- is + or -) is included in the Procedure_Spec;
dnl if it is, then it expands to 1, otherwise, expands to 0.
m4_define(`m4_keep_or_throw_for_one_group', `dnl
m4_ifelse(m4_arg_counter($1, m4_$4_group), `', 0,
  `m4_ifelse(m4_index($2, $3$4), -1, 0, 1)')`'dnl
')

dnl LOCAL
dnl m4_keep_or_throw(
dnl     Class_Counter, Procedure_Spec, +_or_-, Group1, Group2, ...)
dnl
dnl Class_Counter   - The current class counter;
dnl Procedure_Spec  - A schematic procedure name with flags still attached;
dnl +_or_-          - + or -;
dnl Group1          - A group name;
dnl ....
dnl Groupk          - A group name.
m4_define(`m4_keep_or_throw', `dnl
m4_ifelse($#, 0, 0, $#, 1, 0, $#, 2, 0, $#, 3, 0,
  $#, 4, `m4_keep_or_throw_for_one_group($1, $2, $3, $4)',
    `m4_ifelse(m4_keep_or_throw_for_one_group($1, $2, $3, $4), 1, 1,
      `m4_keep_or_throw($1, $2, $3,
                       m4_shift(m4_shift(m4_shift(m4_shift($@)))))')')`'dnl
')

dnl LOCAL
dnl m4_filter_one_procedure(Class_Counter, Procedure_Spec)
dnl
dnl Class_Counter   - The current class counter;
dnl Procedure_Spec  - A schematic procedure name with flags still attached.
dnl
dnl Keeps just those procedures that are wanted for the given class kind.
dnl It first checks if there is a group in Procedure_Spec, whose
dnl definition includes the Class_Kind, preceded by a "-",
dnl if so, it expands to the empty string;
dnl otherwise, it checks if there is a group in Procedure_Spec, whose
dnl definition includes the Class_Kind, preceded by a "+",
dnl if so, it expands to Procedure_Spec.
m4_define(`m4_filter_one_procedure', `dnl
m4_define(`m4_proc_info_string',
       `m4_patsubst(`$2', `[ ]*ppl_[^ ]+ \(.*\)', \1)')`'dnl
m4_ifelse(m4_keep_or_throw(m4_class_kind$1,
                           m4_proc_info_string, -,
                           m4_group_names),
  1, 0,
  m4_keep_or_throw(m4_class_body_kind$1,
                             m4_proc_info_string, \,
                             m4_group_names),
  1, 0,
  m4_keep_or_throw(m4_class_body_2nd_kind$1,
                             m4_proc_info_string, ?,
                             m4_group_names),
  1, 0,
  `m4_keep_or_throw(m4_class_kind$1,
                   m4_proc_info_string, +,
                   m4_group_names)')`'dnl
m4_undefine(m4_proc_info_string)`'dnl
')

dnl PRIVATE
dnl Used in macro-group "ONE_CLASS".
dnl
dnl m4_filter_all_procedures(Class_Counter, keep_or_throw_flag,
dnl                         Procedure_Spec1, ProcedureSpec2, ...)
dnl
dnl Class_Counter   - The current class kind;
dnl keep_or_throw_flag
dnl                 - has value 1 or 0;
dnl Procedure_Spec1 - A schematic procedure name with flags still attached;
dnl ...
dnl Procedure_Speck - A schematic procedure name with flags still attached;
dnl Keeps just those procedure names that are needed for the given class kind.
dnl The classes to be kept or filtered away are determined by extra info
dnl included with each Procedure_Name
dnl The keep_or_throw_flag determines if the filtered procedures
dnl are the wanted (value 1) procedures or the unwanted ones (value 0);
dnl Here we only use the wanted procedures list.
dnl The unwanted list is used when generating the Prolog tests.
m4_define(`m4_filter_all_procedures', `dnl
m4_ifelse($#, 0, , $#, 1, , $#, 2, ,
  $#, 3,
    `m4_ifelse(m4_filter_one_procedure($1, $3), $2,
       `$3')',
    `m4_ifelse(m4_filter_one_procedure($1, $3), $2,
       `$3,
')`'dnl
m4_filter_all_procedures($1, $2, m4_shift(m4_shift(m4_shift($@))))`'dnl
')`'dnl
')

dnl =====================================================================
dnl              *** PATTERNS_TO_REPLACEMENTS ***
dnl The string has all its patterns replaced by the corresponding
dnl replacements. If there are several alternative replacements
dnl then several versions of the pattern-replaced string are generated.
dnl =====================================================================

dnl LOCAL
dnl m4_annotated_extended_pattern_replacement(String, EIndex, AIndex)
dnl
dnl String        - is the string that has the patterns for replacing;
dnl EIndex        - is the index of the extension;
dnl AIndex        - is the index of the annotation.
dnl
dnl One pattern with an optional extension and annotation is replaced
dnl by the corresponding replacement.
dnl This is the key inner step for each string
dnl (usually this string is the expanded procedure schema).
dnl
m4_define(`m4_annotated_extended_pattern_replacement', `dnl
m4_ifdef(`m4_pattern_annotation$3',
  `m4_define(`m4_annotated_extended_pattern',
        m4_pattern_annotation$3`'m4_extended_pattern)`'dnl
m4_annotated_extended_pattern_replacement(m4_patsubst($1,
  @`'m4_annotated_extended_pattern`'@,
  m4_annotated_pattern_replacement$3($2)), $2, m4_incr($3))',
  `$1')`'dnl
')

dnl LOCAL
dnl m4_expand_pattern_by_one_replacement(String, RIndex)
dnl
dnl String        - is the string;
dnl RIndex        - is the index of the replacement.
dnl
dnl The patterns in the string are replaced by the replacement
dnl indexed by RIndex.
dnl
dnl It first defines the m4_replace`'EIndex macros for each EIndex
dnl extension using the macro m4_def_replacements_for_extended_patterns/2.
dnl If m4_replace0 is defined to be different from "NONE",
dnl the macro m4_expand_pattern_by_one_replacement_aux/2 is called
dnl to do the replacements, iterating through the extensions.
dnl When m4_replace0 is NONE, then the code is replaced by the
dnl the empty string.
dnl
m4_define(`m4_expand_pattern_by_one_replacement', `dnl
m4_def_replacements_for_extended_patterns($2, 0)`'dnl
m4_ifelse(m4_replace0, NONE, `',
  m4_expand_pattern_by_one_replacement_aux($1, 0)`'dnl
)`'dnl
')

dnl LOCAL
dnl m4_expand_pattern_by_one_replacement_aux(String, EIndex)
dnl m4_extended_pattern(EIndex)
dnl
dnl String        - is the string;
dnl EIndex        - is the index of the pattern extension.
dnl
dnl Iterates through EIndex values, defining m4_extended_pattern(EIndex)
dnl and calling an auxiliary macro to replace the (delimited, possibly
dnl extended and annotated) m4_PATTERN in String.
dnl
dnl If there are no replacements for EIndex, stop the replacements.
dnl
m4_define(`m4_expand_pattern_by_one_replacement_aux', `dnl
m4_ifelse(`m4_replacements$2', `', `$1',
   `m4_ifdef(`m4_pattern_extension$2',
   `m4_define(`m4_extended_pattern', m4_pattern_extension$2`'m4_PATTERN)`'dnl
m4_expand_pattern_by_one_replacement_aux(
    m4_annotated_extended_pattern_replacement($1, $2, 0),
       m4_incr($2))',
   `$1')`'dnl
')`'dnl
')

dnl LOCAL
dnl m4_expand_pattern_by_all_replacements(String, RIndex)
dnl
dnl String        - is the string;
dnl RIndex        - is the index of the replacement.
dnl
dnl This iteratively calls m4_expand_pattern_by_one_replacement/2 to replace
dnl (a delimited form of possibly extended and annotated) m4_PATTERN by the
dnl arguments of m4_replacements`'RIndex.
m4_define(`m4_expand_pattern_by_all_replacements', `dnl
m4_ifelse($2, m4_nargs(m4_replacements0),
  m4_expand_pattern_by_one_replacement($1, $2),
  `m4_expand_pattern_by_one_replacement($1, $2)`'dnl
m4_expand_pattern_by_all_replacements($1, m4_incr($2))')`'dnl
')

dnl LOCAL
dnl m4_replace_one_pattern_in_string(Class_Number, String, Pattern)
dnl
dnl Class_Number  - is the index to Class in Class_List;
dnl String        - is the code to be changed;
dnl Pattern       - is the pattern to be replaced.
dnl
dnl Replaces all occurrences of the capitalised form of Pattern
dnl in String by the required replacement
dnl (determined both by the class kind of Class and Pattern).
m4_define(`m4_replace_one_pattern_in_string', `dnl
dnl the m4_PATTERN (in uppercase) is the string to be replaced.
m4_define(`m4_PATTERN', m4_upcase($3))`'dnl
m4_def_extended_replacements($1, $3, 0)`'dnl
m4_expand_pattern_by_all_replacements($2, 1)`'dnl
dnl Cleanup temporary definitions.
m4_undefine(`m4_PATTERN')`'dnl
')

dnl PUBLIC
dnl Used in macro-group "ONE_CLASS" and by other m4 generator files.
dnl
dnl m4_replace_all_patterns(Class_Number, String)
dnl
dnl A (recursive) macro to replace, inside the second argument String,
dnl all of the patterns.
dnl
dnl This is public because it is used by some code generators to replace
dnl patterns used in some additional text provided locally.
m4_define(`m4_replace_all_patterns', `dnl
m4_define(`m4_ann_ext_pattern_in_string',
  `m4_regexp($2, `[.]*@[^@_A-Z]*\([_A-Z]+\)@.*', `\1')')`'dnl
m4_ifelse(m4_ann_ext_pattern_in_string, `', $2,
  `m4_replace_all_patterns($1,
    m4_replace_one_pattern_in_string($1, $2,
      m4_downcase(m4_patsubst(m4_ann_ext_pattern_in_string, ^[A-E]_, `'))))')
')

dnl =====================================================================
dnl              *** ONE_CLASS ***
dnl   The next set of macros process a single class.
dnl =====================================================================

dnl LOCAL
dnl m4_replace_procedure_spec_by_code(Class_Number, Procedure_Spec)
dnl
dnl Class_Number   - The current class counter;
dnl Procedure_Spec - The schematic procedure name, complete with any flags.
dnl
dnl The procedure specification is replaced with the code and then
dnl the patterns in the code are replaced by the required replacements.
dnl
dnl Note: one schematic specification will replaced by a single
dnl matching schematic code item; which is then replaced by several
dnl instances.
m4_define(`m4_replace_procedure_spec_by_code', `dnl
m4_patsubst(`$2', `\(.*\)', `dnl
m4_replace_all_patterns($1,
  m4_get_schematic_code(\1, 1, $1))')`'dnl
')

dnl LOCAL
dnl m4_replace_all_procedure_specs_by_code(Class_Number,
dnl                            Procedure_Spec1, Procedure_Spec2, ...)
dnl
dnl Class_Number    - The current class counter;
dnl Procedure_Spec1 - A schematic procedure name;
dnl ...
dnl Procedure_Speck - A schematic procedure name.
dnl
dnl Each name from the second argument onwards is replaced
dnl with the code and then the schema patterns in the code
dnl are replaced by the various instances.
m4_define(`m4_replace_all_procedure_specs_by_code', `dnl
m4_ifelse($#, 0, , $#, 1, ,
       $#, 2, `m4_replace_procedure_spec_by_code($1, $2)',
       `dnl
m4_replace_procedure_spec_by_code($1, $2)`'dnl
m4_replace_all_procedure_specs_by_code($1, m4_shift(m4_shift($@)))`'dnl
')dnl
')

dnl LOCAL
dnl DEFAULT DEF
dnl
dnl m4_pre_extra_class_code(Class_Counter)
dnl m4_post_extra_class_code(Class_Counter)
dnl
dnl Class_Counter   - The index for the current class.
dnl
dnl Default (empty) definitions for pre- and post- code for each class.
dnl These should be redefined as needed.
m4_define(`m4_pre_extra_class_code', `')
m4_define(`m4_post_extra_class_code', `')

dnl PRIVATE
dnl Used by macro-group "ALL_CLASSES".
dnl
dnl DEFAULT DEF
dnl m4_one_class_code(Class_Counter)
dnl
dnl Class_Counter   - The index for the current class.
dnl
dnl First, any necessary prefix code for the procedures in
dnl that class is added.
dnl Then, the main procedure input list is filtered according to
dnl the current class kind and the +/- codes included with the procedure.
dnl Each procedure that is not filtered away is checked to see if
dnl there is a macro with "_code" extension that defines the code.
dnl Then a macro sets the class and other schematic components.
dnl Finally, any necessary postfix code for the procedures in
dnl that class is added.
m4_define(`m4_one_class_code', `dnl
dnl m4_this_class_stuff($1)`'dnl
m4_pre_extra_class_code($1)`'dnl
m4_define(`m4_filtered_proc_list',
       `m4_filter_all_procedures($1, 1, m4_procedure_list)')`'dnl
m4_replace_all_procedure_specs_by_code($1, m4_filtered_proc_list)`'dnl
m4_undefine(`m4_filtered_proc_list')`'dnl
m4_post_extra_class_code($1)`'dnl
')

dnl =====================================================================
dnl              *** ALL_CLASSES ***
dnl  The final set of macros process all the classes, one at a time.
dnl =====================================================================

dnl LOCAL
dnl m4_pre_all_classes_code
dnl
dnl Default (empty) definitions for code that must be placed before all
dnl the generated code.
dnl This is required for code that depends on the instantiated classes
dnl (so must be generated from a schema) but has to be included before
dnl all classes (as in ppl_prolog_icc.m4).
m4_define(`m4_pre_all_classes_code', `')

dnl LOCAL
dnl m4_all_classes_code(Class_Counter)
dnl
dnl Class_Counter   - The index for the current class;
dnl This iterates through the classes to generate the code.
dnl The actual code for each class is generated by m4_one_class_code.
dnl The generated code then has the pattern "|COMMA|" replaced by ",".
m4_define(`m4_all_classes_code', `dnl
m4_ifdef(m4_interface_class`'$1,
  `m4_patsubst(m4_one_class_code($1), `|COMMA|', `,')`'dnl
m4_all_classes_code(m4_incr($1))')`'dnl
')

dnl LOCAL
dnl m4_class_definitions_initialized/0
dnl
dnl Avoids initializing the class macro definitions more than once
dnl when the main macro m4_all_code/0 is called more than once in a
dnl file generation.
m4_define(`m4_class_initialized', `false')

m4_define(`m4_initialize_all', `dnl
m4_init_class_definitions`'dnl
m4_define(`m4_class_initialized', `true')`'dnl
')

dnl PUBLIC
dnl m4_all_code
dnl
dnl The macros for names of the classes are first initialized:
dnl
dnl The main loop macro m4_all_classes_loop is called to generate
dnl code for all the required classes.
m4_define(`m4_all_code', `dnl
m4_ifelse(m4_class_initialized, `false', `m4_initialize_all')`'dnl
m4_pre_all_classes_code`'dnl
m4_all_classes_code(1)`'dnl
')
