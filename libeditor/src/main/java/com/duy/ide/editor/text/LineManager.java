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

package com.duy.ide.editor.text;

import android.graphics.Rect;
import android.support.annotation.NonNull;
import android.text.Editable;
import android.text.Layout;
import android.text.TextUtils;
import android.widget.EditText;

import com.duy.common.DLog;

public class LineManager {
    private static final Rect BOUND = new Rect();
    private static final String TAG = "LineManager";
    private final TextLineNumber mTextLineNumber = new TextLineNumber();
    private boolean[] mIsStartRealLine;
    /**
     * Number of real line
     */
    private int[] mRealLineCount;
    private int[] mRealLines;
    @NonNull
    private EditText mEditText;

    public LineManager(@NonNull EditText editText) {
        mEditText = editText;
    }

    public int getYAtLine(int line) {
        if (mEditText.getLayout() == null) {
            return 0;
        }
        return mEditText.getLayout().getLineTop(line);
    }

    public int getFirstVisibleVirtualLine() {
        try {
            Layout layout = mEditText.getLayout();
            if (layout == null) {
                return 0;
            }
            int lineCount = mEditText.getLineCount();
            int line = (int) (mEditText.getScrollY() / ((float) layout.getHeight()) * lineCount);
            line = Math.max(0, Math.min(line, lineCount - 1));
            return line;
        } catch (Exception e) {
            return 0;
        }
    }

    public int getLastVisibleVirtualLine() {
        try {
            Layout layout = mEditText.getLayout();
            if (layout == null) {
                return 0;
            }
            mEditText.getGlobalVisibleRect(BOUND);
            int lineCount = mEditText.getLineCount();
            int line = (int) ((mEditText.getScrollY() + BOUND.height()) / ((float) layout.getHeight()) * lineCount);
            line = Math.max(0, Math.min(line, lineCount - 1));
            return line;
        } catch (Exception e) {
            return 0;
        }
    }

    public void calculateLinePositionForDraw() {
        mTextLineNumber.clear();
        Layout layout = mEditText.getLayout();
        updateRealLines(0);
        if (mIsStartRealLine == null || mRealLines == null) {
            updateRealLines(0);
        }
        if (layout == null || mIsStartRealLine == null || mRealLines == null) {
            return;
        }

        int firstVisibleLine = getFirstVisibleVirtualLine();
        int lastVisibleLine = getLastVisibleVirtualLine();

        if (DLog.DEBUG) DLog.d(TAG, "firstVisibleLine = " + firstVisibleLine);
        if (DLog.DEBUG) DLog.d(TAG, "lastVisibleLine = " + lastVisibleLine);
        if (lastVisibleLine > mIsStartRealLine.length) {
            return;
        }

        for (int virLine = firstVisibleLine; virLine <= lastVisibleLine; virLine++) {
            if (mIsStartRealLine[virLine]) {
                int realLine = mRealLines[virLine];
                int baseline = layout.getLineBaseline(virLine);
                mTextLineNumber.addLine(String.valueOf(realLine), baseline);
            }
        }
    }

    /**
     * Recalculate real lines, real line start at 1, not 0
     */
    public void updateRealLines(int startLine) {
        int lineCount = mEditText.getLayout().getLineCount();
        //in some case, edit text not init and return line count is 0
        lineCount = Math.max(lineCount, 1);

        //todo improve create new array
        mIsStartRealLine = new boolean[lineCount];
        mRealLines = new int[lineCount];

        final Layout layout = mEditText.getLayout();
        final Editable text = mEditText.getText();

        if (TextUtils.isEmpty(text) || layout == null) {
            mIsStartRealLine[0] = true;
            mRealLines[0] = 1;
            return;
        }

        int line;
        for (line = 0; line < lineCount; line++) {
            int endIndex = layout.getLineEnd(line) - 1;
            boolean hasNewLine = text.charAt(endIndex) == '\n';
            if (hasNewLine) {
                int j = endIndex - 1;
                while (j >= 0 && text.charAt(j) != '\n') {
                    j--;
                }
                j++;
                int lineForOffset = layout.getLineForOffset(j);
                mIsStartRealLine[lineForOffset] = true;
            }
        }
        mIsStartRealLine[lineCount - 1] = true;

        int realLine = startLine; // the first line is not 0, is 1. We start counting from 1
        for (line = 0; line < mIsStartRealLine.length; line++) {
            if (mIsStartRealLine[line]) {
                realLine++;
            }
            mRealLines[line] = realLine;
        }
    }

    public int[] getRealLineCount() {
        return mRealLineCount;
    }

    public int virtualLineFromRealLine(int realLine) {
        int i;
        int fakeLine = 0;
        for (i = 0; i < mRealLines.length; i++) {
            if (realLine == mRealLines[i]) {
                fakeLine = i;
                break;
            }
        }
        return fakeLine;
    }

    public int realLineToVirtualLine(int realLine) {
        if (realLine >= mRealLines.length) {
            return 0;
        }
        for (int line = 0; line < mRealLines.length; line++) {
            if (realLine == mRealLines[line]) {
                return line;
            }
        }
        return 0;
    }

    public TextLineNumber getTextLineNumber() {
        return mTextLineNumber;
    }
}
