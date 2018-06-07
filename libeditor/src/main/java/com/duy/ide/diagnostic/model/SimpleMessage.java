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

package com.duy.ide.diagnostic.model;

import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;
import android.support.annotation.Nullable;

import com.duy.ide.diagnostic.Message;
import com.duy.ide.diagnostic.suggestion.ISuggestion;

/**
 * Created by Duy on 28-Apr-18.
 */

public class SimpleMessage implements Message, Parcelable {
    public static final Creator<SimpleMessage> CREATOR = new Creator<SimpleMessage>() {
        @Override
        public SimpleMessage createFromParcel(Parcel in) {
            return new SimpleMessage(in);
        }

        @Override
        public SimpleMessage[] newArray(int size) {
            return new SimpleMessage[size];
        }
    };
    private final Kind kind;
    private final String filePath;
    private final int line;
    private final int col;
    private final String message;
    @Nullable
    private ISuggestion suggestion;

    public SimpleMessage(Kind kind, String filePath, int line, int col, String message, @Nullable ISuggestion suggestion) {
        this.kind = kind;
        this.filePath = filePath;
        this.line = line;
        this.col = col;
        this.message = message;
        this.suggestion = suggestion;
    }

    protected SimpleMessage(Parcel in) {
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
    public String getSourceFile() {
        return filePath;
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
        if (!(o instanceof SimpleMessage)) return false;

        SimpleMessage that = (SimpleMessage) o;

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
