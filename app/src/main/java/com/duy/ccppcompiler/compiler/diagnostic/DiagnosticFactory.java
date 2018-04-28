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

import com.duy.ccppcompiler.compiler.diagnostic.model.SimpleDiagnostic;
import com.duy.ccppcompiler.compiler.diagnostic.suggestion.DiagnosticSuggestion;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticFactory {
    public static Diagnostic error(String filePath, int line, int col, String message) {
        return create(Kind.ERROR, filePath, line, col, message);
    }

    public static Diagnostic warn(String filePath, int line, int col, String message) {
        return create(Kind.WARNING, filePath, line, col, message);
    }

    public static Diagnostic note(String filePath, int line, int col, String message) {
        return create(Kind.NOTE, filePath, line, col, message);
    }

    public static Diagnostic create(Kind kind, String filePath, int line, int col, String message) {
        return new SimpleDiagnostic(kind, filePath, line, col, message);
    }

    public static Kind createType(String type) {
        type = type.trim().toLowerCase();
        if (type.startsWith("error")) {
            return Kind.ERROR;
        } else if (type.startsWith("warn")) {
            return Kind.WARNING;
        } else if (type.startsWith("note")) {
            return Kind.NOTE;
        }
        return Kind.OTHER;
    }

    public static Diagnostic createFixIt(String filePath, int lineStart, int colStart, int lineEnd, int colEnd, String suggestion) {
        return new DiagnosticSuggestion(filePath, lineStart, colStart, lineEnd, colEnd, suggestion);
    }
}
