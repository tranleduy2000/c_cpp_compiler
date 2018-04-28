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

package com.duy.ccppcompiler.compiler.diagnostic;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.diagnostic.suggestion.ISuggestion;

import java.io.LineNumberReader;
import java.io.StringReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * https://gcc.gnu.org/onlinedocs/gcc-3.3.5/gnat_ug_unx/Output-and-Error-Message-Control.html
 * <p>
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Diagnostic-Message-Formatting-Options.html
 * <p>
 * Created by Duy on 28-Apr-18.
 */

public class OutputParser {
    public static final Pattern DIAGNOSTICS_PATTERN = Pattern.compile(
            "(.*):" + /*File path*/
                    "([0-9]+):" + /*Line*/
                    "([0-9]+):" + /*Col*/
                    "(\\s+.*:\\s+)" + /*Type*/
                    "(.*)" /*Message*/);

    //fix-it:"/storage/emulated/0/examples/simple/bit_print.c":{6:7-6:11}:"printf"
    public static final Pattern FIX_IT_PATTERN = Pattern.compile(Pattern.quote(
            "(fix-it):" +/*prefix*/
                    "(.*):" +/*File path*/
                    "\\{([0-9]+):([0-9]+)-([0-9]+):([0-9]+)}:" + /*Index (line:col)-(line:col)*/
                    "\"(.*)\"" /*Message*/
    ));

    private DiagnosticsCollector diagnosticsCollector;

    public OutputParser(DiagnosticsCollector diagnosticsCollector) {
        this.diagnosticsCollector = diagnosticsCollector;
    }

    @SuppressWarnings("unchecked")
    public void parse(String inputData) {
        try {
            StringReader stringReader = new StringReader(inputData);
            LineNumberReader lineNumberReader = new LineNumberReader(stringReader);

            String line;
            while ((line = lineNumberReader.readLine()) != null) {
                processLine(line, lineNumberReader.readLine());
            }

        } catch (Exception e) {
            //should not happened
            e.printStackTrace();
        }
    }

    @SuppressWarnings("unchecked")
    private void processLine(@NonNull String line, @Nullable String nextLine) {
        try {
            Matcher matcher = DIAGNOSTICS_PATTERN.matcher(line);
            Diagnostic diagnostic;
            if (matcher.find()) {
                String filePath = matcher.group(1);
                int lineNumber = Integer.parseInt(matcher.group(2));
                int colNumber = Integer.parseInt(matcher.group(3));
                Kind type = DiagnosticFactory.createType(matcher.group(4));
                String message = matcher.group(5);

                diagnostic = DiagnosticFactory.create(type, filePath, lineNumber, colNumber, message);
                diagnosticsCollector.report(diagnostic);
            } else {
                if (nextLine != null) {
                    processLine(nextLine, null);
                }
                return;
            }
            if (nextLine == null) {
                return;
            }

            matcher = FIX_IT_PATTERN.matcher(nextLine);
            if (matcher.find()) {
                String filePath = matcher.group(2);
                int lineStart = Integer.parseInt(matcher.group(3));
                int colStart = Integer.parseInt(matcher.group(4));
                int lineEnd = Integer.parseInt(matcher.group(5));
                int colEnd = Integer.parseInt(matcher.group(6));
                String message = matcher.group(7);
                ISuggestion suggestion = DiagnosticFactory.createSuggestion(filePath, lineStart, colStart, lineEnd, colEnd, message);
                diagnostic.setSuggestion(suggestion);
            } else {
                processLine(nextLine, null);
            }
        } catch (Exception e) {
            e.printStackTrace();
            //should not happend
        }
    }
}
