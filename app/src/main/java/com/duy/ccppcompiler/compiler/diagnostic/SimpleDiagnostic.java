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

import android.content.Context;

import java.io.File;

/**
 * Created by Duy on 28-Apr-18.
 */

public class SimpleDiagnostic implements Diagnostic<File> {
    private final Kind kind;
    private final String filePath;
    private final int line;
    private final int col;
    private final String message;

    public SimpleDiagnostic(Kind kind, String filePath, int line, int col, String message) {
        this.kind = kind;
        this.filePath = filePath;
        this.line = line;
        this.col = col;
        this.message = message;
    }

    @Override
    public Kind getKind() {
        return kind;
    }

    @Override
    public File getSource() {
        return new File(filePath);
    }

    @Override
    public long getPosition() {
        return NOPOS;
    }

    @Override
    public long getStartPosition() {
        return NOPOS;
    }

    @Override
    public String toString() {
        return "SimpleDiagnostic{" +
                "kind=" + kind +
                ", filePath='" + filePath + '\'' +
                ", line=" + line +
                ", col=" + col +
                ", message='" + message + '\'' +
                '}';
    }

    @Override
    public long getEndPosition() {
        return NOPOS;
    }

    @Override
    public long getLineNumber() {
        return line;
    }

    @Override
    public long getColumnNumber() {
        return col;
    }

    @Override
    public String getCode() {
        throw new UnsupportedOperationException();
    }

    @Override
    public String getMessage(Context context) {
        return message;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof SimpleDiagnostic)) return false;

        SimpleDiagnostic that = (SimpleDiagnostic) o;

        if (line != that.line) return false;
        if (col != that.col) return false;
        if (getKind() != that.getKind()) return false;
        if (filePath != null ? !filePath.equals(that.filePath) : that.filePath != null)
            return false;
        return message != null ? message.equals(that.message) : that.message == null;
    }

    @Override
    public int hashCode() {
        int result = getKind() != null ? getKind().hashCode() : 0;
        result = 31 * result + (filePath != null ? filePath.hashCode() : 0);
        result = 31 * result + line;
        result = 31 * result + col;
        result = 31 * result + (message != null ? message.hashCode() : 0);
        return result;
    }
}
