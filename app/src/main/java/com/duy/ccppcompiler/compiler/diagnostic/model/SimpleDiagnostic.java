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

package com.duy.ccppcompiler.compiler.diagnostic.model;

import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;
import com.duy.ccppcompiler.compiler.diagnostic.Kind;
import com.duy.ccppcompiler.compiler.diagnostic.suggestion.ISuggestion;

import java.io.File;

/**
 * Created by Duy on 28-Apr-18.
 */

public class SimpleDiagnostic implements Diagnostic, Parcelable {
    public static final Creator<SimpleDiagnostic> CREATOR = new Creator<SimpleDiagnostic>() {
        @Override
        public SimpleDiagnostic createFromParcel(Parcel in) {
            return new SimpleDiagnostic(in);
        }

        @Override
        public SimpleDiagnostic[] newArray(int size) {
            return new SimpleDiagnostic[size];
        }
    };
    private final Kind kind;
    private final String filePath;
    private final int line;
    private final int col;
    private final String message;
    @Nullable
    private ISuggestion suggestion;

    public SimpleDiagnostic(Kind kind, String filePath, int line, int col, String message, @Nullable ISuggestion suggestion) {
        this.kind = kind;
        this.filePath = filePath;
        this.line = line;
        this.col = col;
        this.message = message;
        this.suggestion = suggestion;
    }

    protected SimpleDiagnostic(Parcel in) {
        kind = Kind.values()[in.readInt()];
        filePath = in.readString();
        line = in.readInt();
        col = in.readInt();
        message = in.readString();
        suggestion = in.readParcelable(ISuggestion.class.getClassLoader());
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(kind.ordinal());
        dest.writeString(filePath);
        dest.writeInt(line);
        dest.writeInt(col);
        dest.writeString(message);
        dest.writeParcelable(suggestion, flags);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public Kind getKind() {
        return kind;
    }

    @Nullable
    @Override
    public File getSourceFile() {
        File file = new File(filePath);
        if (file.exists()){
            return file;
        }
        return null;
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

    @Nullable
    @Override
    public ISuggestion getSuggestion() {
        return suggestion;
    }

    @Override
    public void setSuggestion(ISuggestion suggestion) {
        this.suggestion = suggestion;
    }

    @Override
    public String toString() {
        return "SimpleDiagnostic{" +
                "kind=" + kind +
                ", filePath='" + filePath + '\'' +
                ", line=" + line +
                ", col=" + col +
                ", message='" + message + '\'' +
                ", suggestion=" + suggestion +
                '}';
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
        if (message != null ? !message.equals(that.message) : that.message != null) return false;
        return getSuggestion() != null ? getSuggestion().equals(that.getSuggestion()) : that.getSuggestion() == null;
    }

    @Override
    public int hashCode() {
        int result = getKind() != null ? getKind().hashCode() : 0;
        result = 31 * result + (filePath != null ? filePath.hashCode() : 0);
        result = 31 * result + line;
        result = 31 * result + col;
        result = 31 * result + (message != null ? message.hashCode() : 0);
        result = 31 * result + (getSuggestion() != null ? getSuggestion().hashCode() : 0);
        return result;
    }
}
