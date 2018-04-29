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
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.diagnostic.suggestion.ISuggestion;

import java.io.File;

/**
 * Created by Duy on 28-Apr-18.
 */

public interface Diagnostic extends Parcelable {
    /**
     * Used to signal that no position is available.
     */
    public final static long NOPOS = -1;

    Kind getKind();

    /**
     * Gets the source object associated with this diagnostic.
     *
     * @return the source object associated with this diagnostic.
     * {@code null} if no source object is associated with the
     * diagnostic.
     */
    @NonNull
    File getSourceFile();

    /**
     * Gets a character offset from the beginning of the source object
     * associated with this diagnostic that indicates the location of
     * the problem.  In addition, the following must be true:
     * <p>
     * <p>{@code getStartPostion() <= getPosition()}
     * <p>{@code getPosition() <= getEndPosition()}
     *
     * @return character offset from beginning of source; {@link
     * #NOPOS} if {@link #getSourceFile()} would return {@code null} or if
     * no location is suitable
     */
    long getPosition();

    /**
     * Gets the character offset from the beginning of the file
     * associated with this diagnostic that indicates the start of the
     * problem.
     *
     * @return offset from beginning of file; {@link #NOPOS} if and
     * only if {@link #getPosition()} returns {@link #NOPOS}
     */
    long getStartPosition();

    /**
     * Gets the character offset from the beginning of the file
     * associated with this diagnostic that indicates the end of the
     * problem.
     *
     * @return offset from beginning of file; {@link #NOPOS} if and
     * only if {@link #getPosition()} returns {@link #NOPOS}
     */
    long getEndPosition();

    /**
     * Gets the line number of the character offset returned by
     * {@linkplain #getPosition()}.
     *
     * @return a line number or {@link #NOPOS} if and only if {@link
     * #getPosition()} returns {@link #NOPOS}
     */
    long getLineNumber();

    /**
     * Gets the column number of the character offset returned by
     * {@linkplain #getPosition()}.
     *
     * @return a column number or {@link #NOPOS} if and only if {@link
     * #getPosition()} returns {@link #NOPOS}
     */
    long getColumnNumber();

    /**
     * Gets a diagnostic code indicating the type of diagnostic.  The
     * code is implementation-dependent and might be {@code null}.
     *
     * @return a diagnostic code
     */
    @Nullable
    String getCode();

    String getMessage(Context context);

    @Nullable
    ISuggestion getSuggestion();

    void setSuggestion(ISuggestion suggestion);
}
