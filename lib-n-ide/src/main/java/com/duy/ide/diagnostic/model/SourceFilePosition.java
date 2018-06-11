/*
 * Copyright (C) 2018 Tran Le Duy
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.diagnostic.model;

import android.support.annotation.NonNull;

import com.google.common.base.Objects;

import java.io.File;
import java.io.Serializable;

public final class SourceFilePosition implements Serializable {

    public static final SourceFilePosition UNKNOWN =
            new SourceFilePosition(SourceFile.UNKNOWN, SourcePosition.UNKNOWN);

    @NonNull
    private final SourceFile mSourceFile;

    @NonNull
    private final SourcePosition mSourcePosition;

    public SourceFilePosition(@NonNull SourceFile sourceFile,
                              @NonNull SourcePosition sourcePosition) {
        mSourceFile = sourceFile;
        mSourcePosition = sourcePosition;
    }

    public SourceFilePosition(@NonNull File file,
                              @NonNull SourcePosition sourcePosition) {
        this(new SourceFile(file), sourcePosition);
    }

    @NonNull
    public SourcePosition getPosition() {
        return mSourcePosition;
    }

    @NonNull
    public SourceFile getFile() {
        return mSourceFile;
    }

    @Override
    public String toString() {
        return print(false);
    }

    public String print(boolean shortFormat) {
        if (mSourcePosition.equals(SourcePosition.UNKNOWN)) {
            return mSourceFile.print(shortFormat);
        } else {
            return mSourceFile.print(shortFormat) + ':' + mSourcePosition.toString();
        }
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(mSourceFile, mSourcePosition);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (!(obj instanceof SourceFilePosition)) {
            return false;
        }
        SourceFilePosition other = (SourceFilePosition) obj;
        return Objects.equal(mSourceFile, other.mSourceFile) &&
                Objects.equal(mSourcePosition, other.mSourcePosition);
    }
}
