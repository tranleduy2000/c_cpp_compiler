/*
 *  Copyright (C)  2018  Duy Tran Le
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ccppcompiler.compiler.analyze;

import android.support.annotation.NonNull;

import com.duy.ide.diagnostic.DiagnosticsCollector;
import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.parser.ParsingFailedException;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.diagnostic.util.OutputLineReader;

import java.util.List;
import java.util.regex.Pattern;

/**
 * https://gcc.gnu.org/onlinedocs/gcc-3.3.5/gnat_ug_unx/Output-and-Error-Message-Control.html
 * <p>
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Diagnostic-Message-Formatting-Options.html
 * <p>
 * <p>
 * <p>
 * Created by Duy on 28-Apr-18.
 */

public class CppCheckOutputParser implements PatternAwareOutputParser {

    public static final String TEMPLATE = "--template=\"{file}:{line}:{severity}:{message}\"";
    private static final Pattern TEMPLATE_PATTERN = Pattern.compile("^(\\S+):([0-9]+):([^:]+):(.*)");
    private DiagnosticsCollector diagnosticsCollector;

    public CppCheckOutputParser(DiagnosticsCollector diagnosticsCollector) {
        this.diagnosticsCollector = diagnosticsCollector;
    }

    public void parse(String inputData) {
//        try {
//            StringReader stringReader = new StringReader(inputData);
//            LineNumberReader lineReader = new LineNumberReader(stringReader);
//            String line;
//            while ((line = lineReader.readLine()) != null) {
//                Matcher matcher = TEMPLATE_PATTERN.matcher(line);
//                if (matcher.find()) {
//                    String filePath = matcher.group(1);
//                    int lineNumber = Integer.parseInt(matcher.group(2));
//                    Kind type = DiagnosticFactory.createType(matcher.group(3));
//                    String message = matcher.group(4);
//                    Message diagnostic = DiagnosticFactory.create(type, filePath, lineNumber, Message.NOPOS, message);
//                    diagnosticsCollector.report(diagnostic);
//                }
//            }
//        } catch (Exception e) {
//            //should not happened
//            e.printStackTrace();
//        }
    }

    @Override
    public boolean parse(@NonNull String line, @NonNull OutputLineReader reader, @NonNull List<Message> messages) throws ParsingFailedException {
        return false;
    }
}
