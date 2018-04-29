/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.duy.ccppcompiler.compiler.diagnostic.parser;

import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;
import com.duy.ccppcompiler.compiler.diagnostic.DiagnosticsCollector;
import com.duy.ccppcompiler.compiler.diagnostic.OutputParser;

import junit.framework.TestCase;

import java.util.ArrayList;
import java.util.regex.Matcher;

/**
 * Created by Duy on 28-Apr-18.
 */
public class OutputParserTest extends TestCase {

    public void testParse() throws Exception {
        DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
        OutputParser outputParser = new OutputParser(diagnosticsCollector);
        outputParser.parse("D:\\Data\\Downloads\\c\\file.c: In function 'main':\n" +
                "D:\\Data\\Downloads\\c\\file.c:16:7: error: expected ';' before 'firstNumber'\n" +
                "       firstNumber = secondNumber;\n" +
                "       ^");
    }


    public void testParse2() throws Exception {
        DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
        OutputParser outputParser = new OutputParser(diagnosticsCollector);
        outputParser.parse("D:\\Data\\Downloads\\c\\file.c: In function 'main':\n" +
                "D:\\Data\\Downloads\\c\\file.c:13:7: error: 'temporaryVaiable' undeclared (first use in this function)\n" +
                "       temporaryVaiable = firstNumber;\n" +
                "       ^\n" +
                "D:\\Data\\Downloads\\c\\file.c:13:7: note: each undeclared identifier is reported only once for each function it appears in\n" +
                "D:\\Data\\Downloads\\c\\file.c:19:22: error: 'tempraryVariable' undeclared (first use in this function)\n" +
                "       secondNumber = tempraryVariable;\n" +
                "                      ^\n" +
                "D:\\Data\\Downloads\\c\\file.c:25:1: error: expected expression before '}' token\n" +
                " }\n" +
                " ^\n");
        ArrayList<Diagnostic> diagnostics = diagnosticsCollector.getDiagnostics();
        for (Diagnostic diagnostic : diagnostics) {
            System.out.println(diagnostic);
        }

        assertEquals(diagnostics.size(), 4);
    }


    public void testFixIt() {
        DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
        OutputParser outputParser = new OutputParser(diagnosticsCollector);
        outputParser.parse("/storage/emulated/0/examples/simple/bit_print.c:6:7: warning: implicit declaration of function 'pinf'; did you mean 'printf'?\n" +
                "                  fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{6:7-6:11}:\"printf\"");
        ArrayList<Diagnostic> diagnostics = diagnosticsCollector.getDiagnostics();
        for (Diagnostic diagnostic : diagnostics) {
            System.out.println(diagnostic);
        }
        assertEquals(diagnostics.size(), 1);
    }

    public void testFixIt1() {
        DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
        OutputParser outputParser = new OutputParser(diagnosticsCollector);
        outputParser.parse(" /storage/emulated/0/examples/simple/bit_print.c: In function 'main':\n" +
                "                  /storage/emulated/0/examples/simple/bit_print.c:6:7: warning: implicit declaration of function 'pinf'; did you mean 'printf'?\n" +
                "                  fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{6:7-6:11}:\"printf\"\n" +
                "                  /storage/emulated/0/examples/simple/bit_print.c:7:7: warning: implicit declaration of function 'sanf'; did you mean 'scanf'?\n" +
                "                  fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{7:7-7:11}:\"scanf\"\n" +
                "                  /storage/emulated/0/examples/simple/bit_print.c:13:7: error: 'temporaryVaiable' undeclared (first use in this function); did you mean 'temporaryVariable'?\n" +
                "                  fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{13:7-13:23}:\"temporaryVariable\"\n");
        ArrayList<Diagnostic> diagnostics = diagnosticsCollector.getDiagnostics();
        for (Diagnostic diagnostic : diagnostics) {
            System.out.println(diagnostic);
        }
        assertEquals(diagnostics.size(), 3);
        assertTrue(diagnostics.get(1).getSuggestion() != null);
    }

    public void testFixIt2() {
        Matcher matcher = OutputParser.FIX_IT_PATTERN.matcher(
                "fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{6:7-6:11}:\"printf\"");
        assertTrue(matcher.find());
    }

    public void testFixIt3() {
        Matcher matcher = OutputParser.FIX_IT_PATTERN.matcher(
                "fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{6:7-6:11}:\"printf\"");
        assertTrue(matcher.find());
        assertEquals(matcher.group(), "fix-it:\"/storage/emulated/0/examples/simple/bit_print.c\":{6:7-6:11}:\"printf\"");
    }


}