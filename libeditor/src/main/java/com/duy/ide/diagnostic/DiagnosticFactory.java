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

package com.duy.ide.diagnostic;

import com.duy.ide.diagnostic.model.Kind;
import com.duy.ide.diagnostic.model.SimpleDiagnostic;
import com.duy.ide.diagnostic.suggestion.DiagnosticSuggestion;
import com.duy.ide.diagnostic.suggestion.ISuggestion;

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
        return new SimpleDiagnostic(kind, filePath, line, col, message, null);
    }

    public static Kind createType(String type) {
        type = type.trim().toLowerCase();
        if (type.contains("error")) {
            return Kind.ERROR;
        } else if (type.contains("warn")) {
            return Kind.WARNING;
        } else if (type.contains("note")) {
            return Kind.NOTE;
        }
        return Kind.OTHER;
    }

    public static ISuggestion createSuggestion(String filePath, int lineStart, int colStart,
                                               int lineEnd, int colEnd, String suggestion) {
        return new DiagnosticSuggestion(filePath, lineStart, colStart, lineEnd, colEnd, suggestion);
    }
}
