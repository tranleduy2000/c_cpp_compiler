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
import android.support.annotation.Nullable;

import com.google.common.base.Objects;

import java.io.File;
import java.io.Serializable;

/**
 * Represents a source file.
 */
public final class SourceFile implements Serializable {

    public static final SourceFile UNKNOWN = new SourceFile();

    @Nullable
    private final File mSourceFile;

    /**
     * A human readable description
     * <p>
     * Usually the file name is OK for the short output, but for the manifest merger,
     * where all of the files will be named AndroidManifest.xml the variant name is more useful.
     */
    @Nullable
    private final String mDescription;

    @SuppressWarnings("NullableProblems")
    public SourceFile(
            @NonNull File sourceFile,
            @NonNull String description) {
        mSourceFile = sourceFile;
        mDescription = description;
    }

    public SourceFile(
            @SuppressWarnings("NullableProblems") @NonNull File sourceFile) {
        mSourceFile = sourceFile;
        mDescription = null;
    }

    public SourceFile(
            @SuppressWarnings("NullableProblems") @NonNull String description) {
        mSourceFile = null;
        mDescription = description;
    }

    private SourceFile() {
        mSourceFile = null;
        mDescription = null;
    }

    @Nullable
    public File getSourceFile() {
        return mSourceFile;
    }

    @Nullable
    public String getDescription() {
        return mDescription;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (!(obj instanceof SourceFile)) {
            return false;
        }
        SourceFile other = (SourceFile) obj;

        return Objects.equal(mDescription, other.mDescription) &&
                Objects.equal(mSourceFile, other.mSourceFile);
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(mSourceFile, mDescription);
    }

    @Override
    public String toString() {
        return print(false /* shortFormat */);
    }

    public String print(boolean shortFormat) {
        if (mSourceFile == null) {
            if (mDescription == null) {
                return "Unknown source file";
            }
            return mDescription;
        }
        String fileName = mSourceFile.getName();
        String fileDisplayName = shortFormat ? fileName : mSourceFile.getAbsolutePath();
        if (mDescription == null || mDescription.equals(fileName)) {
            return fileDisplayName;
        } else {
            return String.format("[%1$s] %2$s", mDescription, fileDisplayName);
        }
    }

}
