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

package com.duy.ccppcompiler.compiler.diagnostic.suggestion;

import android.os.Parcel;
import android.os.Parcelable;
import android.support.annotation.NonNull;

import java.io.File;

/**
 * -fdiagnostics-parseable-fixits
 * Emit fix-it hints in a machine-parseable format, suitable for consumption by IDEs. For each
 * fix-it, a line will be printed after the relevant diagnostic, starting with the string “fix-it:”.
 * For example:
 * <p>
 * fix-it:"test.c":{45:3-45:21}:"gtk_widget_show_all"
 * The location is expressed as a half-open range, expressed as a count of bytes, starting at byte 1
 * for the initial column. In the above example, bytes 3 through 20 of line 45 of “test.c” are to be
 * replaced with the given string:
 * <p>
 * 00000000011111111112222222222
 * 12345678901234567890123456789
 * gtk_widget_showall (dlg);
 * ^^^^^^^^^^^^^^^^^^
 * gtk_widget_show_all
 * The filename and replacement string escape backslash as “\\", tab as “\t”, newline as “\n”, double
 * quotes as “\"”, non-printable characters as octal (e.g. vertical tab as “\013”).
 * <p>
 * An empty replacement string indicates that the given range is to be removed. An empty range (e.g.
 * “45:3-45:3”) indicates that the string is to be inserted at the given position.
 * Created by Duy on 29-Apr-18.
 */

public class DiagnosticSuggestion implements Parcelable, ISuggestion {
    public static final Creator<DiagnosticSuggestion> CREATOR = new Creator<DiagnosticSuggestion>() {
        @Override
        public DiagnosticSuggestion createFromParcel(Parcel in) {
            return new DiagnosticSuggestion(in);
        }

        @Override
        public DiagnosticSuggestion[] newArray(int size) {
            return new DiagnosticSuggestion[size];
        }
    };
    public final String filePath;
    public final int lineStart;
    public final int colStart;
    public final int lineEnd;
    public final int colEnd;
    public final String suggestion;

    public DiagnosticSuggestion(String filePath, int lineStart, int colStart, int lineEnd, int colEnd, String suggestion) {
        this.filePath = filePath;
        this.lineStart = lineStart;
        this.colStart = colStart;
        this.lineEnd = lineEnd;
        this.colEnd = colEnd;
        this.suggestion = suggestion;
    }

    protected DiagnosticSuggestion(Parcel in) {
        filePath = in.readString();
        lineStart = in.readInt();
        colStart = in.readInt();
        lineEnd = in.readInt();
        colEnd = in.readInt();
        suggestion = in.readString();
    }

    @Override
    public String toString() {
        return "DiagnosticSuggestion{" +
                "filePath='" + filePath + '\'' +
                ", lineStart=" + lineStart +
                ", colStart=" + colStart +
                ", lineEnd=" + lineEnd +
                ", colEnd=" + colEnd +
                ", suggestion='" + suggestion + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof DiagnosticSuggestion)) return false;

        DiagnosticSuggestion that = (DiagnosticSuggestion) o;

        if (getLineStart() != that.getLineStart()) return false;
        if (getColStart() != that.getColStart()) return false;
        if (getLineEnd() != that.getLineEnd()) return false;
        if (getColEnd() != that.getColEnd()) return false;
        if (getSourceFile() != null ? !getSourceFile().equals(that.getSourceFile()) : that.getSourceFile() != null)
            return false;
        return getSuggestion() != null ? getSuggestion().equals(that.getSuggestion()) : that.getSuggestion() == null;
    }

    @Override
    public int hashCode() {
        int result = getSourceFile() != null ? getSourceFile().hashCode() : 0;
        result = 31 * result + getLineStart();
        result = 31 * result + getColStart();
        result = 31 * result + getLineEnd();
        result = 31 * result + getColEnd();
        result = 31 * result + (getSuggestion() != null ? getSuggestion().hashCode() : 0);
        return result;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(filePath);
        dest.writeInt(lineStart);
        dest.writeInt(colStart);
        dest.writeInt(lineEnd);
        dest.writeInt(colEnd);
        dest.writeString(suggestion);
    }

    @Override
    public File getSourceFile() {
        return new File(filePath);
    }

    @Override
    public int getLineStart() {
        return lineStart;
    }

    @Override
    public int getColStart() {
        return colStart;
    }

    @Override
    public int getLineEnd() {
        return lineEnd;
    }

    @Override
    public int getColEnd() {
        return colEnd;
    }

    @NonNull
    @Override
    public String getSuggestion() {
        return suggestion;
    }
}
