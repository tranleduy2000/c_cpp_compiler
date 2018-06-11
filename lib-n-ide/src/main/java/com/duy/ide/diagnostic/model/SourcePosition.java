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

import com.google.common.base.Objects;

import java.io.Serializable;

/**
 * An immutable position in a text file, used in errors to point the user to an issue.
 *
 * Positions that are unknown are represented by -1.
 */
public final class SourcePosition implements Serializable {

    public static final SourcePosition UNKNOWN = new SourcePosition();

    private final int mStartLine, mStartColumn, mStartOffset, mEndLine, mEndColumn, mEndOffset;

    public SourcePosition(int startLine, int startColumn, int startOffset,
            int endLine, int endColumn, int endOffset) {
        mStartLine = startLine;
        mStartColumn = startColumn;
        mStartOffset = startOffset;
        mEndLine = endLine;
        mEndColumn = endColumn;
        mEndOffset = endOffset;
    }

    public SourcePosition(int lineNumber, int column, int offset) {
        mStartLine = mEndLine = lineNumber;
        mStartColumn = mEndColumn = column;
        mStartOffset = mEndOffset = offset;
    }

    private SourcePosition() {
        mStartLine = mStartColumn = mStartOffset = mEndLine = mEndColumn = mEndOffset = -1;
    }

    protected SourcePosition(SourcePosition copy) {
        mStartLine = copy.getStartLine();
        mStartColumn = copy.getStartColumn();
        mStartOffset = copy.getStartOffset();
        mEndLine = copy.getEndLine();
        mEndColumn = copy.getEndColumn();
        mEndOffset = copy.getEndOffset();
    }

    /**
     * Outputs positions as human-readable formatted strings.
     *
     * e.g.
     * <pre>84
     * 84-86
     * 84:5
     * 84:5-28
     * 85:5-86:47</pre>
     *
     * @return a human readable position.
     */
    @Override
    public String toString() {
        if (mStartLine == -1) {
            return "?";
        }
        StringBuilder sB = new StringBuilder(15);
        sB.append(mStartLine + 1); // Humans think that the first line is line 1.
        if (mStartColumn != -1) {
            sB.append(':');
            sB.append(mStartColumn + 1);
        }
        if (mEndLine != -1) {

            if (mEndLine == mStartLine) {
                if (mEndColumn != -1 && mEndColumn != mStartColumn) {
                    sB.append('-');
                    sB.append(mEndColumn + 1);
                }
            } else {
                sB.append('-');
                sB.append(mEndLine + 1);
                if (mEndColumn != -1) {
                    sB.append(':');
                    sB.append(mEndColumn + 1);
                }
            }
        }
        return sB.toString();
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (!(obj instanceof SourcePosition)) {
            return false;
        }
        SourcePosition other = (SourcePosition) obj;

        return other.mStartLine == mStartLine &&
                other.mStartColumn == mStartColumn &&
                other.mStartOffset == mStartOffset &&
                other.mEndLine == mEndLine &&
                other.mEndColumn == mEndColumn &&
                other.mEndOffset == mEndOffset;
    }

    @Override
    public int hashCode() {
        return Objects
                .hashCode(mStartLine, mStartColumn, mStartOffset, mEndLine, mEndColumn, mEndOffset);
    }

    public int getStartLine() {
        return mStartLine;
    }

    public int getStartColumn() {
        return mStartColumn;
    }

    public int getStartOffset() {
        return mStartOffset;
    }


    public int getEndLine() {
        return mEndLine;
    }

    public int getEndColumn() {
        return mEndColumn;
    }

    public int getEndOffset() {
        return mEndOffset;
    }

}
