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

import android.content.Context;
import android.os.Parcelable;
import android.support.annotation.Nullable;

import com.duy.ide.suggestion.ISuggestion;

/**
 * Created by Duy on 28-Apr-18.
 */

public interface Diagnostic extends Parcelable {
    /**
     * Used to signal that no position is available.
     */
    public final static int NOPOS = -1;

    Kind getKind();

    /**
     * Gets the source object associated with this diagnostic.
     *
     * @return the source object associated with this diagnostic.
     * {@code null} if no source object is associated with the
     * diagnostic.
     */
    @Nullable
    String getSourceFile();

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
