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

import org.junit.Test;

import java.util.ArrayList;

/**
 * Created by Duy on 28-Apr-18.
 */
public class OutputParserTest {
    @Test
    public void parse() throws Exception {
        DiagnosticsCollector<Diagnostic> diagnosticsCollector = new DiagnosticsCollector<>();
        OutputParser<Diagnostic> outputParser = new OutputParser<>(diagnosticsCollector);
        outputParser.parse("D:\\Data\\Downloads\\c\\file.c: In function 'main':\n" +
                "D:\\Data\\Downloads\\c\\file.c:16:7: error: expected ';' before 'firstNumber'\n" +
                "       firstNumber = secondNumber;\n" +
                "       ^");
    }

    @Test
    public void parse2() throws Exception {
        DiagnosticsCollector<Diagnostic> diagnosticsCollector = new DiagnosticsCollector<>();
        OutputParser<Diagnostic> outputParser = new OutputParser<>(diagnosticsCollector);
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
        ArrayList<Diagnostic<? extends Diagnostic>> diagnostics = diagnosticsCollector.getDiagnostics();
        for (Diagnostic<? extends Diagnostic> diagnostic : diagnostics) {
            System.out.println(diagnostic);
        }
    }

}