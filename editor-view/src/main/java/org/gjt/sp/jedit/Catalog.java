/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.gjt.sp.jedit;

import java.util.LinkedHashMap;

public class Catalog {
    public static final String DEFAULT_MODE_NAME = "Text";
    public static final LinkedHashMap<String, Mode> modes = new LinkedHashMap<>();

    static {
        modes.put("ActionScript", new Mode("ActionScript", "actionscript.xml", "(?:.*[/\\\\])?.*\\.as", null));
        modes.put("Ada", new Mode("Ada", "ada.xml", "(?:.*[/\\\\])?.*\\.(ada|adb|ads)", null));
        modes.put("Ada95", new Mode("Ada95", "ada95.xml", null, null));
        //<MODE NAME="ahk" FILE="ahk.xml" FILE_NAME_GLOB="*.{ahk}"/>
        modes.put("AutoHotkey", new Mode("AutoHotkey", "autohotkey.xml", "(?:.*[/\\\\])?.*\\.ahk", null));
        modes.put("Ant", new Mode("Ant", "ant.xml", "(?:.*[/\\\\])?.*build\\.xml", ".*<project.*"));
        modes.put("Antlr", new Mode("Antlr", "antlr.xml", "(?:.*[/\\\\])?.*\\.g", null));
        modes.put("ApacheConf", new Mode("ApacheConf", "apacheconf.xml", "([/\\\\]etc[/\\\\]apache2[/\\\\](.*\\.conf|(conf\\.d|(mods|sites)-(available|enabled))[/\\\\].*)|.*httpd.*\\.conf)", null));
        modes.put("Apdl", new Mode("Apdl", "apdl.xml", "(?:.*[/\\\\])?.*\\.(ans|inp|mak|mac)", null));
        modes.put("AppleScript", new Mode("AppleScript", "applescript.xml", "(?:.*[/\\\\])?.*\\.applescript", null));
        modes.put("ASP", new Mode("ASP", "asp.xml", "(?:.*[/\\\\])?.*\\.(asp|asa)", null));
        modes.put("Aspect-j", new Mode("Aspect-j", "aspect_j.xml", "(?:.*[/\\\\])?.*\\.aj", null));
        modes.put("Assembly-agc", new Mode("Assembly-agc", "assembly_agc.xml", "(?:.*[/\\\\])?.*\\.agc", null));
        modes.put("Assembly-ags", new Mode("Assembly-ags", "assembly_ags.xml", "(?:.*[/\\\\])?.*\\.aea", null));
        modes.put("Assembly-m68k", new Mode("Assembly-m68k", "assembly_m68k.xml", null, null));
        modes.put("Assembly-macro32", new Mode("Assembly-macro32", "assembly_macro32.xml", "(?:.*[/\\\\])?.*\\.mar", null));
        modes.put("Assembly-mcs51", new Mode("Assembly-mcs51", "assembly_mcs51.xml", null, null));
        modes.put("Assembly-r2000", new Mode("Assembly-r2000", "assembly_r2000.xml", "(?:.*[/\\\\])?.*\\.mips", null));
        modes.put("Assembly-parrot", new Mode("Assembly-parrot", "assembly_parrot.xml", "(?:.*[/\\\\])?.*\\.pasm", null));
        modes.put("Assembly-x86", new Mode("Assembly-x86", "assembly_x86.xml", "(?:.*[/\\\\])?.*\\.asm", null));
        modes.put("Avro", new Mode("Avro", "avro.xml", "(?:.*[/\\\\])?.*\\.avdl", null));
        modes.put("AWK", new Mode("AWK", "awk.xml", "(?:.*[/\\\\])?.*\\.awk", "#!.*awk.*"));
        modes.put("B", new Mode("B", "b.xml", "(?:.*[/\\\\])?.*\\.(imp|ref|mch)", null));
        modes.put("Batch", new Mode("Batch", "batch.xml", "(?:.*[/\\\\])?.*\\.(bat|cmd)", null));
        modes.put("Bbj", new Mode("Bbj", "bbj.xml", "(?:.*[/\\\\])?.*\\.bbj", null));
        modes.put("Bcel", new Mode("Bcel", "bcel.xml", null, null));
        modes.put("BeanShell", new Mode("BeanShell", "java.xml", "(?:.*[/\\\\])?.*\\.bsh", null));
        modes.put("Bibtex", new Mode("Bibtex", "bibtex.xml", "(?:.*[/\\\\])?.*\\.bib", null));
        modes.put("Binsource-agc", new Mode("Binsource-agc", "binsource_agc.xml", "(?:.*[/\\\\])?.*\\.binsource", null));
        modes.put("C", new Mode("C", "c.xml", "(?:.*[/\\\\])?.*\\.c", null));
        modes.put("CFScript", new Mode("CFScript", "cfscript.xml", "(?:.*[/\\\\])?.*\\.cfc", null));
        modes.put("Chill", new Mode("Chill", "chill.xml", "(?:.*[/\\\\])?.*\\.(chl|mpol)", null));
        modes.put("Cil", new Mode("Cil", "cil.xml", "(?:.*[/\\\\])?.*\\.il", null));
        modes.put("Clips", new Mode("Clips", "clips.xml", "(?:.*[/\\\\])?.*\\.clp", null));
        modes.put("Clojure", new Mode("Clojure", "clojure.xml", "(?:.*[/\\\\])?.*\\.clj", null));
        modes.put("CMake", new Mode("CMake", "cmake.xml", "(?:.*[/\\\\])?CMakeLists\\.txt", null));
        modes.put("Cobol", new Mode("Cobol", "cobol.xml", "(?:.*[/\\\\])?.*\\.(cbl|cob)", null));
        modes.put("CoffeeScript", new Mode("CoffeeScript", "coffeescript.xml", "(?:.*[/\\\\])?.*\\.coffee", null));
        modes.put("ColdFusion", new Mode("ColdFusion", "coldfusion.xml", "(?:.*[/\\\\])?.*\\.(cfm|dbm|cfc)", null));
        modes.put("Cplex-lp", new Mode("Cplex-lp", "cplexlp.xml", "(?:.*[/\\\\])?.*\\.lp", null));
        modes.put("C++", new Mode("C++", "cplusplus.xml", "(?:.*[/\\\\])?.*\\.(cc|cpp|h|hh|hpp|cxx)", null));
        modes.put("C#", new Mode("C#", "csharp.xml", "(?:.*[/\\\\])?.*\\.cs", null));
        modes.put("CSS", new Mode("CSS", "css.xml", "(?:.*[/\\\\])?.*\\.css", null));
        modes.put("CSV", new Mode("CSV", "csv.xml", "(?:.*[/\\\\])?.*\\.csv", null));
        modes.put("CVS-commit", new Mode("CVS-commit", "cvs_commit.xml", "(?:.*[/\\\\])?cvs.*\\.tmp", null));
        modes.put("D", new Mode("D", "d.xml", "(?:.*[/\\\\])?.*\\.d", null));
        modes.put("Dart", new Mode("Dart", "dart.xml", "(?:.*[/\\\\])?.*\\.dart", null));
        modes.put("Django", new Mode("Django", "django.xml", null, null));
        modes.put("Dot", new Mode("Dot", "dot.xml", "(?:.*[/\\\\])?.*\\.dot", null));
        modes.put("Doxygen", new Mode("Doxygen", "doxygen.xml", "(?:.*[/\\\\])?doxyfile.*", null));
        modes.put("Dsssl", new Mode("Dsssl", "dsssl.xml", "(?:.*[/\\\\])?.*\\.dsl", null));
        modes.put("Embperl", new Mode("Embperl", "embperl.xml", "(?:.*[/\\\\])?.*\\.epl", null));
        modes.put("Erlang", new Mode("Erlang", "erlang.xml", "(?:.*[/\\\\])?.*\\.(erl|hrl)", null));
        modes.put("Eiffel", new Mode("Eiffel", "eiffel.xml", "(?:.*[/\\\\])?.*\\.e", null));
        modes.put("Fhtml", new Mode("Fhtml", "fhtml.xml", "(?:.*[/\\\\])?.*\\.(furnace|fhtml)", null));
        modes.put("Factor", new Mode("Factor", "factor.xml", "(?:.*[/\\\\])?.*\\.factor", null));
        modes.put("Jflex", new Mode("Jflex", "jflex.xml", "(?:.*[/\\\\])?.*\\.flex", null));
        modes.put("Forth", new Mode("Forth", "forth.xml", "(?:.*[/\\\\])?.*\\.f", null));
        modes.put("Fortran", new Mode("Fortran", "fortran.xml", "(?:.*[/\\\\])?.*\\.(for|fort|f77)", null));
        modes.put("Fortran90", new Mode("Fortran90", "fortran90.xml", "(?:.*[/\\\\])?.*\\.(f90|f95|f03)", null));
        modes.put("FoxPro", new Mode("FoxPro", "foxpro.xml", "(?:.*[/\\\\])?.*\\.prg", null));
        modes.put("FreeMarker", new Mode("FreeMarker", "freemarker.xml", "(?:.*[/\\\\])?.*\\.ftl", "<\\#ftl.*"));
        modes.put("GCBasic", new Mode("GCBasic", "gcbasic.xml", "(?:.*[/\\\\])?.*\\.(gc|gcb)", "(.*GCBASIC.*|.*Great Cow BASIC.*)"));
        modes.put("GetText", new Mode("GetText", "gettext.xml", "(?:.*[/\\\\])?.*\\.(po|pot)", null));
        modes.put("GNUPlot", new Mode("GNUPlot", "gnuplot.xml", "(?:.*[/\\\\])?.*\\.(dem|plt)", null));
        modes.put("Go", new Mode("Go", "go.xml", "(?:.*[/\\\\])?.*\\.go", null));
        modes.put("Gradle", new Mode("Gradle", "gradle.xml", "(?:.*[/\\\\])?.*\\.(gradle)", null));
        modes.put("Groovy", new Mode("Groovy", "groovy.xml", "(?:.*[/\\\\])?.*\\.(gant|groovy|grv)", null));
        modes.put("GSP", new Mode("GSP", "jsp.xml", "(?:.*[/\\\\])?.*\\.(gsp)", null));
        modes.put("Haskell", new Mode("Haskell", "haskell.xml", "(?:.*[/\\\\])?.*\\.hs", null));
        modes.put("Haxe", new Mode("Haxe", "haxe.xml", "(?:.*[/\\\\])?.*\\.hx", null));
        modes.put("Hxml", new Mode("Hxml", "hxml.xml", "(?:.*[/\\\\])?.*\\.hxml", null));
        modes.put("Hex", new Mode("Hex", "hex.xml", null, null));
        modes.put("Hlsl", new Mode("Hlsl", "hlsl.xml", "(?:.*[/\\\\])?.*\\.fx", null));
        modes.put(".htaccess", new Mode(".htaccess", "htaccess.xml", "(?:.*[/\\\\])?\\.htaccess", null));
        modes.put("Html", new Mode("Html", "html.xml", "(?:.*[/\\\\])?.*\\.(html|htm|hta)", null));
        modes.put("I4gl", new Mode("I4gl", "i4gl.xml", "(?:.*[/\\\\])?.*\\.4gl", null));
        modes.put("Icalendar", new Mode("Icalendar", "ical.xml", "(?:.*[/\\\\])?.*\\.ics", null));
        modes.put("Icon", new Mode("Icon", "icon.xml", "(?:.*[/\\\\])?.*\\.icn", null));
        modes.put("Idl", new Mode("Idl", "idl.xml", "(?:.*[/\\\\])?.*\\.idl", null));
        modes.put("Inform", new Mode("Inform", "inform.xml", "(?:.*[/\\\\])?.*\\.inf", null));
        modes.put("Inno-setup", new Mode("Inno-setup", "inno_setup.xml", "(?:.*[/\\\\])?.*\\.iss", null));
        modes.put("Ini", new Mode("Ini", "ini.xml", "(?:.*[/\\\\])?.*\\.(ini|reg|milk)", null));
        modes.put("Interlis", new Mode("Interlis", "interlis.xml", "(?:.*[/\\\\])?.*\\.ili", null));
        modes.put("Io", new Mode("Io", "io.xml", "(?:.*[/\\\\])?.*\\.io", null));
        modes.put("Jamon", new Mode("Jamon", "jamon.xml", "(?:.*[/\\\\])?.*\\.jamon", null));
        modes.put("JavaCC", new Mode("JavaCC", "javacc.xml", "(?:.*[/\\\\])?.*\\.(jj|jjt)", null));
        modes.put("Java", new Mode("Java", "java.xml", "(?:.*[/\\\\])?.*\\.java", null));
        modes.put("JavaFX", new Mode("JavaFX", "javafx.xml", "(?:.*[/\\\\])?.*\\.fx", null));
        modes.put("JavaScript", new Mode("JavaScript", "javascript.xml", "(?:.*[/\\\\])?(.*\\.js|Buildsub)", null));
        modes.put("Jcl", new Mode("Jcl", "jcl.xml", "(?:.*[/\\\\])?.*\\.jcl", null));
        modes.put("Jedit-Actions", new Mode("Jedit-Actions", "jedit_actions.xml", "(?:.*[/\\\\])?actions\\.xml", null));
        modes.put("Jhtml", new Mode("Jhtml", "jhtml.xml", "(?:.*[/\\\\])?.*\\.jhtml", null));
        modes.put("Jmk", new Mode("Jmk", "jmk.xml", "(?:.*[/\\\\])?.*\\.jmk", null));
        modes.put("JSON", new Mode("JSON", "json.xml", "(?:.*[/\\\\])?.*\\.json", null));
        modes.put("JSP", new Mode("JSP", "jsp.xml", "(?:.*[/\\\\])?.*\\.(jsp|jsf|jspf|tag)", null));
        modes.put("Kotlin", new Mode("Kotlin", "kotlin.xml", "(?:.*[/\\\\])?.*\\.(kt)", null));
        modes.put("Latex", new Mode("Latex", "latex.xml", "(?:.*[/\\\\])?.*\\.(tex|sty|ltx)", null));
        modes.put("Lex", new Mode("Lex", "lex.xml", "(?:.*[/\\\\])?.*\\.l", null));
        modes.put("Lilypond", new Mode("Lilypond", "lilypond.xml", "(?:.*[/\\\\])?.*\\.(ly|ily)", null));
        modes.put("Lisp", new Mode("Lisp", "lisp.xml", "(?:.*[/\\\\])?.*\\.(lisp|lsp|el)", null));
        modes.put("Literate-haskell", new Mode("Literate-haskell", "literate_haskell.xml", "(?:.*[/\\\\])?.*\\.lhs", null));
        modes.put("Logs", new Mode("Logs", "logs.xml", "(?:.*[/\\\\])?.*\\.log", null));
        modes.put("Logtalk", new Mode("Logtalk", "logtalk.xml", "(?:.*[/\\\\])?.*\\.lgt", null));
        modes.put("Lotos", new Mode("Lotos", "lotos.xml", "(?:.*[/\\\\])?.*\\.(lot|lotos)", null));
        modes.put("Lua", new Mode("Lua", "lua.xml", "(?:.*[/\\\\])?.*\\.lua", null));
        modes.put("MacroScheduler", new Mode("MacroScheduler", "macroscheduler.xml", "(?:.*[/\\\\])?.*\\.scp", null));
        modes.put("Mail", new Mode("Mail", "mail.xml", null, null));
        modes.put("Makefile", new Mode("Makefile", "makefile.xml", "(?:.*[/\\\\])?.*makefile", null));
        modes.put("Maple", new Mode("Maple", "maple.xml", "(?:.*[/\\\\])?.*\\.(mpl|mws)", null));
        modes.put("Markdown", new Mode("Markdown", "markdown.xml", "(?:.*[/\\\\])?.*\\.(md|markdown)", null));
        modes.put("Maven", new Mode("Maven", "maven.xml", "(?:.*[/\\\\])?pom\\.xml", null));
        modes.put("Metapost", new Mode("Metapost", "mpost.xml", "(?:.*[/\\\\])?.*\\.mp", null));
        modes.put("Mxml", new Mode("Mxml", "mxml.xml", "(?:.*[/\\\\])?.*\\.mxml", null));
        modes.put("Ml", new Mode("Ml", "ml.xml", "(?:.*[/\\\\])?.*\\.(sml|ml)", null));
        modes.put("Modula3", new Mode("Modula3", "modula3.xml", "(?:.*[/\\\\])?.*\\.[im]3", null));
        modes.put("Moin", new Mode("Moin", "moin.xml", "(?:.*[/\\\\])?.*\\.moin", null));
        modes.put("Mqsc", new Mode("Mqsc", "mqsc.xml", "(?:.*[/\\\\])?.*\\.mqsc", null));
        modes.put("Myghty", new Mode("Myghty", "myghty.xml", "(?:.*[/\\\\])?(autohandler|dhandler|.*\\.myt)", null));
        modes.put("MySQL", new Mode("MySQL", "mysql.xml", null, null));
        modes.put("N3", new Mode("N3", "n3.xml", "(?:.*[/\\\\])?.*\\.n3", null));
        modes.put("Netrexx", new Mode("Netrexx", "netrexx.xml", "(?:.*[/\\\\])?.*\\.nrx", null));
        modes.put("Nqc", new Mode("Nqc", "nqc.xml", "(?:.*[/\\\\])?.*\\.nqc", null));
        modes.put("Nsis2", new Mode("Nsis2", "nsis2.xml", "(?:.*[/\\\\])?.*\\.(nsi|nsh)", null));
        modes.put("Objective-C", new Mode("Objective-C", "objective_c.xml", "(?:.*[/\\\\])?.*\\.(objc|m|mm)", null));
        modes.put("Objectrexx", new Mode("Objectrexx", "objectrexx.xml", "(?:.*[/\\\\])?.*\\.(rex|orx)", null));
        modes.put("Occam", new Mode("Occam", "occam.xml", "(?:.*[/\\\\])?.*\\.icc", null));
        modes.put("Omnimark", new Mode("Omnimark", "omnimark.xml", "(?:.*[/\\\\])?.*\\.x(om|in)", null));
        modes.put("Outline", new Mode("Outline", "outline.xml", "(?:.*[/\\\\])?.*\\.(outline)", null));
        modes.put("Pascal", new Mode("Pascal", "pascal.xml", "(?:.*[/\\\\])?.*\\.(pas|dpr|dpk)", null));
        modes.put("Patch", new Mode("Patch", "patch.xml", "(?:.*[/\\\\])?.*\\.(diff|patch)", "(# HG changeset patch|diff --git .*)"));
        modes.put("Perl", new Mode("Perl", "perl.xml", "(?:.*[/\\\\])?.*\\.p([lmh]|od)", "#!/.*perl.*"));
        modes.put("PHP", new Mode("PHP", "php.xml", "(?:.*[/\\\\])?.*\\.(php3|php4|php|phtml|inc)", "(<\\?php.*|#!/.*php.*)"));
        modes.put("Pike", new Mode("Pike", "pike.xml", "(?:.*[/\\\\])?.*\\.(pike|pmod)", null));
        modes.put("Plaintex", new Mode("Plaintex", "plaintex.xml", null, null));
        modes.put("PostgreSQL", new Mode("PostgreSQL", "pg_sql.xml", "(?:.*[/\\\\])?.*\\.(pg_sql|pg-sql)", null));
        modes.put("PowerCenter Parameter File", new Mode("PowerCenter Parameter File", "powercenter_parameter_file.xml", "(?:.*[/\\\\])?.*\\.par", null));
        modes.put("Pl-sql", new Mode("Pl-sql", "osql.xml", "(?:.*[/\\\\])?.*\\.(pls|sql)", null));
        modes.put("Pl-sql9", new Mode("Pl-sql9", "pl_sql.xml", null, null));
        modes.put("Pl1", new Mode("Pl1", "pl1.xml", "(?:.*[/\\\\])?.*\\.pl[i1]", null));
        modes.put("Pop11", new Mode("Pop11", "pop11.xml", "(?:.*[/\\\\])?.*\\.(p|pop11|p11)", null));
        modes.put("PostScript", new Mode("PostScript", "postscript.xml", "(?:.*[/\\\\])?.*\\.(ps|eps)", null));
        modes.put("Povray", new Mode("Povray", "povray.xml", "(?:.*[/\\\\])?.*\\.(pov|povray)", null));
        modes.put("PowerDynamo", new Mode("PowerDynamo", "powerdynamo.xml", "(?:.*[/\\\\])?.*\\.(ssc|stm)", null));
        modes.put("PowerShell", new Mode("PowerShell", "powershell.xml", "(?:.*[/\\\\])?.*\\.(ps1|psm1|psd1)", null));
        modes.put("Prolog", new Mode("Prolog", "prolog.xml", "(?:.*[/\\\\])?.*\\.pro", null));
        modes.put("Progress", new Mode("Progress", "progress.xml", "(?:.*[/\\\\])?.*\\.[piw]", null));
        modes.put("Properties", new Mode("Properties", "props.xml", "(?:.*[/\\\\])?.*(properties|props)", null));
        modes.put("Psp", new Mode("Psp", "psp.xml", "(?:.*[/\\\\])?.*\\.psp", null));
        modes.put("Ptl", new Mode("Ptl", "ptl.xml", "(?:.*[/\\\\])?.*\\.ptl", null));
        modes.put("Pure", new Mode("Pure", "pure.xml", "(?:.*[/\\\\])?.*\\.pure", "#!.*/.*pure"));
        modes.put("Pvwave", new Mode("Pvwave", "pvwave.xml", "(?:.*[/\\\\])?.*\\.jou", null));
        modes.put("Pyrex", new Mode("Pyrex", "pyrex.xml", "(?:.*[/\\\\])?.*\\.(pyx)", null));
        modes.put("Python", new Mode("Python", "python.xml", "(?:.*[/\\\\])?.*\\.(py|pyw|sc|jy)", "#!.*/.*python.*"));
        modes.put("R", new Mode("R", "r.xml", "(?:.*[/\\\\])?.*\\.r", null));
        modes.put("Rebol", new Mode("Rebol", "rebol.xml", "(?:.*[/\\\\])?.*\\.rebol", null));
        modes.put("Redcode", new Mode("Redcode", "redcode.xml", "(?:.*[/\\\\])?.*\\.(red|rc)", null));
        modes.put("Relax-ng-compact", new Mode("Relax-ng-compact", "relax_ng_compact.xml", "(?:.*[/\\\\])?.*\\.rnc", null));
        modes.put("Renderman-rib", new Mode("Renderman-rib", "rib.xml", "(?:.*[/\\\\])?.*\\.rib", null));
        modes.put("Rd", new Mode("Rd", "rd.xml", "(?:.*[/\\\\])?.*\\.rd", null));
        modes.put("Rest", new Mode("Rest", "rest.xml", "(?:.*[/\\\\])?.*\\.(rst|rest)", "(===|~~~).*"));
        modes.put("Rhtml", new Mode("Rhtml", "rhtml.xml", "(?:.*[/\\\\])?.*\\.(rhtml|html\\.erb)", null));
        modes.put("Roff", new Mode("Roff", "roff.xml", "(?:.*[/\\\\])?.*\\.(1|2|3|4|5|6|7|8|9|me|ms|mom|tmac)", null));
        modes.put("Rpm-spec", new Mode("Rpm-spec", "rpmspec.xml", "(?:.*[/\\\\])?.*\\.spec", null));
        modes.put("Rtf", new Mode("Rtf", "rtf.xml", "(?:.*[/\\\\])?.*\\.rtf", null));
        modes.put("Rakefile", new Mode("Rakefile", "ruby.xml", "(?:.*[/\\\\])?.*Rakefile", null));
        modes.put("Ruby", new Mode("Ruby", "ruby.xml", "(?:.*[/\\\\])?.*\\.(rb|rbw)", "#!.*/.*ruby.*"));
        modes.put("Rust", new Mode("Rust", "rubst.xml", "(?:.*[/\\\\])?.*\\.(rbs)", null));
        modes.put("Rview", new Mode("Rview", "rview.xml", "(?:.*[/\\\\])?.*\\.rvw", null));
        modes.put("S+", new Mode("S+", "splus.xml", "(?:.*[/\\\\])?.*\\.ssc", null));
        modes.put("S#", new Mode("S#", "ssharp.xml", "(?:.*[/\\\\])?.*\\.(ss|ssc|ssi|ssw|sts|aml)", null));
        modes.put("Sas", new Mode("Sas", "sas.xml", "(?:.*[/\\\\])?.*\\.sas", null));
        modes.put("Sbt", new Mode("Sbt", "scala.xml", "(?:.*[/\\\\])?.*\\.sbt", null));
        modes.put("Scala", new Mode("Scala", "scala.xml", "(?:.*[/\\\\])?.*\\.scala", null));
        modes.put("Scheme", new Mode("Scheme", "scheme.xml", "(?:.*[/\\\\])?.*\\.scm", null));
        modes.put("Sgml", new Mode("Sgml", "sgml.xml", "(?:.*[/\\\\])?.*\\.(sgml|sgm|dtd)", null));
        modes.put("Sip", new Mode("Sip", "sip.xml", null, null));
        modes.put("Rcp", new Mode("Rcp", "rcp.xml", "(?:.*[/\\\\])?.*\\.rcp", null));
        modes.put("ShellScript", new Mode("ShellScript", "shellscript.xml", "(?:.*[/\\\\])?.*\\.(csh|sh|bash|login|profile|bashrc|bash_profile)", "#!/.*sh.*"));
        modes.put("SHTML", new Mode("SHTML", "shtml.xml", "(?:.*[/\\\\])?.*\\.(shtml|shtm|ssi)", null));
        modes.put("Slate", new Mode("Slate", "slate.xml", "(?:.*[/\\\\])?.*\\.slate", null));
        modes.put("Slax", new Mode("Slax", "slax.xml", "(?:.*[/\\\\])?.*\\.slax", null));
        modes.put("Smalltalk", new Mode("Smalltalk", "smalltalk.xml", "(?:.*[/\\\\])?.*\\.(st|sources|changes)", null));
        modes.put("Smarty", new Mode("Smarty", "smarty.xml", "(?:.*[/\\\\])?.*\\.tpl", null));
        modes.put("Sdl/pr", new Mode("Sdl/pr", "sdl_pr.xml", "(?:.*[/\\\\])?.*\\.pr", null));
        modes.put("Sql-loader", new Mode("Sql-loader", "sql_loader.xml", "(?:.*[/\\\\])?.*\\.ctl", null));
        modes.put("Smi-mib", new Mode("Smi-mib", "smi_mib.xml", "(?:.*[/\\\\])?.*(\\.mib|-MIB\\.txt)", null));
        modes.put("Sqr", new Mode("Sqr", "sqr.xml", "(?:.*[/\\\\])?.*\\.(sqr|sqc)", null));
        modes.put("Squidconf", new Mode("Squidconf", "squidconf.xml", "(?:.*[/\\\\])?squid\\.conf", null));
        modes.put("Stata", new Mode("Stata", "stata.xml", "(?:.*[/\\\\])?.*\\.(do|ado|mata)", null));
        modes.put("Svn-commit", new Mode("Svn-commit", "svn_commit.xml", "(?:.*[/\\\\])?svn-commit.*\\.tmp", null));
        modes.put("Swig", new Mode("Swig", "swig.xml", "(?:.*[/\\\\])?.*\\.(i|swg)", null));
        modes.put("Tcl", new Mode("Tcl", "tcl.xml", "(?:.*[/\\\\])?.*\\.(tcl|tsh)", null));
        modes.put("Texinfo", new Mode("Texinfo", "texinfo.xml", "(?:.*[/\\\\])?.*\\.texi", null));
        modes.put("Tex", new Mode("Tex", "tex.xml", null, null));
        modes.put("Text", new Mode("Text", "text.xml", "(?:.*[/\\\\])?.*\\.txt", null));
        modes.put("Rfc", new Mode("Rfc", "rfc.xml", "(?:.*[/\\\\])?rfc.*\\.txt", null));
        modes.put("Tld", new Mode("Tld", "tld.xml", "(?:.*[/\\\\])?.*\\.tld", null));
        modes.put("Tsp", new Mode("Tsp", "tsp.xml", "(?:.*[/\\\\])?.*\\.tsp", null));
        modes.put("Transact-sql", new Mode("Transact-sql", "tsql.xml", null, null));
        modes.put("Template-toolkit", new Mode("Template-toolkit", "tthtml.xml", "(?:.*[/\\\\])?.*\\.tt(html|css|js)", null));
        modes.put("Twiki", new Mode("Twiki", "twiki.xml", "(?:.*[/\\\\])?.*\\.twiki", null));
        modes.put("Typoscript", new Mode("Typoscript", "typoscript.xml", "(?:.*[/\\\\])?.*\\.ts", null));
        modes.put("Url", new Mode("Url", "url.xml", "(?:.*[/\\\\])?.*\\.url", null));
        modes.put("Uscript", new Mode("Uscript", "uscript.xml", "(?:.*[/\\\\])?.*\\.uc", null));
        modes.put("Vala", new Mode("Vala", "vala.xml", "(?:.*[/\\\\])?.*\\.vala", null));
        modes.put("Vbscript", new Mode("Vbscript", "vbscript.xml", "(?:.*[/\\\\])?.*\\.(vbs|bas|cls)", null));
        modes.put("Velocity", new Mode("Velocity", "velocity_pure.xml", "(?:.*[/\\\\])?.*\\.vm", null));
        modes.put("Verilog", new Mode("Verilog", "verilog.xml", "(?:.*[/\\\\])?.*\\.(ver|v|sv)", null));
        modes.put("Vhdl", new Mode("Vhdl", "vhdl.xml", "(?:.*[/\\\\])?.*\\.vh.*", null));
        modes.put("Visualbasic", new Mode("Visualbasic", "visualbasic.xml", "(?:.*[/\\\\])?.*\\.(vb)", null));
        modes.put("Vrml2", new Mode("Vrml2", "vrml2.xml", "(?:.*[/\\\\])?.*\\.(wrl|wrz)", null));
        modes.put("Xml", new Mode("Xml", "xml.xml", "(?:.*[/\\\\])?.*\\.(xml|xhtml|xsd|qrc|ui|docbook)", "<\\?xml.*"));
        modes.put("Xq", new Mode("Xq", "xq.xml", "(?:.*[/\\\\])?.*\\.x(q|qm|ql)", null));
        modes.put("Xsl", new Mode("Xsl", "xsl.xml", "(?:.*[/\\\\])?.*\\.xsl", null));
        modes.put("Yab", new Mode("Yab", "yab.xml", "(?:.*[/\\\\])?.*\\.yab", null));
        modes.put("Yaml", new Mode("Yaml", "yaml.xml", "(?:.*[/\\\\])?.*\\.(yml|yaml)", null));
        modes.put("Zpt", new Mode("Zpt", "zpt.xml", "(?:.*[/\\\\])?.*\\.(pt|zpt)", null));

    }

    public static Mode getModeByName(String name) {
        return modes.get(name);
    }
}
