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

package com.duy.ccppcompiler.diagnostic;

import java.io.LineNumberReader;
import java.io.StringReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.duy.ccppcompiler.diagnostic.Kind.OTHER;

/**
 * https://gcc.gnu.org/onlinedocs/gcc-3.3.5/gnat_ug_unx/Output-and-Error-Message-Control.html
 * <p>
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Diagnostic-Message-Formatting-Options.html
 * <p>
 * Created by Duy on 28-Apr-18.
 */

public class OutputParser {
    ///storage/emulated/0/Examples/c++/cin2.cpp:25:1: error: expected ';' before '}' token
    public static final Pattern FILE_LINE_COL_TYPE_MESSAGE_PATTERN = Pattern.compile(
            "(\\S+):([0-9]+):([0-9]+):(\\s+.*:\\s+)(.*)");

    ///storage/emulated/0/Examples/c++/cin2.cpp: In function 'int main()':
    public static final Pattern FILE_MESSAGE_PATTERN = Pattern.compile("(\\S+):(.*)");

    private static final String TAG = "OutputParser2";
    private DiagnosticsCollector diagnosticsCollector;

    public OutputParser(DiagnosticsCollector diagnosticsCollector) {
        this.diagnosticsCollector = diagnosticsCollector;
    }

    @SuppressWarnings("unchecked")
    public void parse(String inputData) {
        try {
            StringReader stringReader = new StringReader(inputData);
            LineNumberReader lineReader = new LineNumberReader(stringReader);

            String line;
            while ((line = lineReader.readLine()) != null) {
                Matcher matcher = FILE_LINE_COL_TYPE_MESSAGE_PATTERN.matcher(line);
                if (matcher.find()) {
                    String filePath = matcher.group(1);
                    int lineNumber = Integer.parseInt(matcher.group(2));
                    int colNumber = Integer.parseInt(matcher.group(3));
                    Kind type = DiagnosticFactory.createType(matcher.group(4));
                    String message = matcher.group(5);

                    Diagnostic diagnostic =
                            DiagnosticFactory.create(type, filePath, lineNumber, colNumber, message);
                    diagnosticsCollector.report(diagnostic);
                    continue;
                }

                matcher = FILE_MESSAGE_PATTERN.matcher(line);
                if (matcher.find()) {
                    String filePath = matcher.group(1);
                    String message = matcher.group(2);

                    Diagnostic diagnostic =
                            DiagnosticFactory.create(OTHER, filePath, Diagnostic.NOPOS, Diagnostic.NOPOS, message);
                    diagnosticsCollector.report(diagnostic);
                }
            }

        } catch (Exception e) {
            //should not happened
            e.printStackTrace();
        }
    }
}
