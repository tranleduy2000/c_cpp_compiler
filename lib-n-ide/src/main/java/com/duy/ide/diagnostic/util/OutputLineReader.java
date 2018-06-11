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
package com.duy.ide.diagnostic.util;


import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import java.util.regex.Pattern;

/**
 * Reads a compiler's output line-by-line.
 */
public class OutputLineReader {
    private static final Pattern LINE_BREAK = Pattern.compile("\\r?\\n");

    @NonNull
    private final String[] myLines;

    private final int myLineCount;
    private int myPosition;

    /**
     * Creates a new {@link OutputLineReader}.
     *
     * @param text the text to read.
     */
    public OutputLineReader(@NonNull String text) {
        myLines = LINE_BREAK.split(text);
        myLineCount = myLines.length;
    }

    public int getLineCount() {
        return myLineCount;
    }

    /**
     * Reads the next line of text, moving the line pointer to the next one.
     *
     * @return the contents of the next line, or {@code null} if we reached the end of the text.
     */
    @Nullable
    public String readLine() {
        if (myPosition >= 0 && myPosition < myLineCount) {
            return myLines[myPosition++];
        }
        return null;
    }

    /**
     * Reads the text of one the line at the given position, without moving the line pointer.
     *
     * @param lineToSkipCount the number of lines to skip from the line pointer.
     * @return the contents of the specified line, or {@code null} if the specified position is greater than the end of the text.
     */
    @Nullable
    public String peek(int lineToSkipCount) {
        int tempPosition = lineToSkipCount + myPosition;
        if (tempPosition >= 0 && tempPosition < myLineCount) {
            return myLines[tempPosition];
        }
        return null;
    }

    public boolean hasNextLine() {
        return myPosition < myLineCount - 1;
    }

    public void skipNextLine() {
        myPosition++;
    }

    public void pushBack(@NonNull String text) {
        myPosition--;
    }
}
