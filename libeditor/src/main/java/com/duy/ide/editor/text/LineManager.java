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

import java.util.Arrays;

public class LineManager {
    private static final Rect BOUND = new Rect();
    private static final String TAG = "LineManager";
    private final TextLineNumber mTextLineNumber = new TextLineNumber();
    private boolean[] mIsStartRealLine;
    /**
     * Number of real line
     */
    private int mRealLineCount;
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
            boolean verticalScrollBarEnabled = mEditText.isVerticalScrollBarEnabled();
            if (!verticalScrollBarEnabled) {
                return 0;
            }

            int lineCount = layout.getLineCount();
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
            int lineCount = mEditText.getLineCount();
            boolean verticalScrollBarEnabled = mEditText.isVerticalScrollBarEnabled();
            if (!verticalScrollBarEnabled) {
                return lineCount - 1;
            }

            mEditText.getGlobalVisibleRect(BOUND);
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
        if (layout == null) {
            return;
        }
        //lazy loading
        if (mIsStartRealLine == null || mRealLines == null) {
            updateRealLines(0);
        }

        int firstVisibleLine = getFirstVisibleVirtualLine();
        int lastVisibleLine = Math.min(mIsStartRealLine.length - 1, getLastVisibleVirtualLine());

        if (DLog.DEBUG) DLog.d(TAG, "firstVisibleLine = " + firstVisibleLine);
        if (DLog.DEBUG) DLog.d(TAG, "lastVisibleLine = " + lastVisibleLine);

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
     *
     * @param startLine the start line to update, The lines above will not change
     */
    public void updateRealLines(int startLine) {
        int lineCount = mEditText.getLayout().getLineCount();
        if (startLine >= lineCount) {
            return;
        }
        //in some case, edit text not init and return line count is 0
        lineCount = Math.max(lineCount, 1);

        if (mIsStartRealLine != null) {
            mIsStartRealLine = Arrays.copyOf(mIsStartRealLine, lineCount);
            mRealLines = Arrays.copyOf(mRealLines, lineCount);
        } else {
            mIsStartRealLine = new boolean[lineCount];
            mRealLines = new int[lineCount];
        }

        final Layout layout = mEditText.getLayout();
        final Editable text = mEditText.getText();

        if (TextUtils.isEmpty(text) || layout == null) {
            mIsStartRealLine[0] = false;
            mRealLines[0] = 1;
            mRealLineCount = 0;
            return;
        }

        int line;
        for (line = startLine; line < lineCount; line++) {
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

        int realLine = mRealLines[Math.max(0, startLine - 1)]; // the first line is not 0, is 1. We start counting from 1
        for (line = realLine; line < lineCount; line++) {
            if (mIsStartRealLine[line]) {
                realLine++;
            }
            mRealLines[line] = realLine;
        }
        mRealLineCount = realLine;
    }

    public int getRealLineCount() {
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
