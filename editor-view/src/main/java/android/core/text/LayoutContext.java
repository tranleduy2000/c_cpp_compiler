/*
 *
 *  * Copyright (C) 2006 The Android Open Source Project
 *  *
 *  * Licensed under the Apache License, Version 2.0 (the "License");
 *  * you may not use this file except in compliance with the License.
 *  * You may obtain a copy of the License at
 *  *
 *  *      http://www.apache.org/licenses/LICENSE-2.0
 *  *
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS,
 *  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  * See the License for the specific language governing permissions and
 *  * limitations under the License.
 *
 */

package android.core.text;

import android.graphics.Paint;

import com.jecelyin.editor.v2.Preferences;


/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class LayoutContext {
    public int lineNumber = -1;
    public Paint lineNumberPaint;
    public Paint linePaint;
    public Paint gutterBackgroundPaint;
    public int gutterWidth;
    public int lineNumberX;


    public int cursorThickness;
    public Preferences preferences;
    public boolean isShowWhiteSpace;
    public int whiteSpaceColor;
    public TextLineNumber textLineNumber = new TextLineNumber();

    public int getWhiteSpaceColor() {
        return whiteSpaceColor;
    }

    public void setWhiteSpaceColor(int whiteSpaceColor) {
        this.whiteSpaceColor = whiteSpaceColor;
    }

    public boolean isShowWhiteSpace() {
        return isShowWhiteSpace;
    }

    public void setShowWhiteSpace(boolean showWhiteSpace) {
        isShowWhiteSpace = showWhiteSpace;
    }

    public int getCursorThickness() {
        return cursorThickness;
    }

    public void setCursorThickness(int cursorThickness) {
        this.cursorThickness = cursorThickness;
    }

    public int getLineNumber() {
        return lineNumber;
    }

    public void setLineNumber(int lineNumber) {
        this.lineNumber = lineNumber;
    }

    public Paint getLineNumberPaint() {
        return lineNumberPaint;
    }

    public void setLineNumberPaint(Paint lineNumberPaint) {
        this.lineNumberPaint = lineNumberPaint;
    }

    public Paint getLinePaint() {
        return linePaint;
    }

    public void setLinePaint(Paint linePaint) {
        this.linePaint = linePaint;
    }

    public Paint getGutterBackgroundPaint() {
        return gutterBackgroundPaint;
    }

    public void setGutterBackgroundPaint(Paint gutterBackgroundPaint) {
        this.gutterBackgroundPaint = gutterBackgroundPaint;
    }

    public int getGutterWidth() {
        return gutterWidth;
    }

    public void setGutterWidth(int gutterWidth) {
        this.gutterWidth = gutterWidth;
    }

    public int getLineNumberX() {
        return lineNumberX;
    }

    public void setLineNumberX(int lineNumberX) {
        this.lineNumberX = lineNumberX;
    }

    public Preferences getPreferences() {
        return preferences;
    }

    public void setPreferences(Preferences preferences) {
        this.preferences = preferences;
    }

    public TextLineNumber getTextLineNumber() {
        return textLineNumber;
    }

    public void setTextLineNumber(TextLineNumber textLineNumber) {
        this.textLineNumber = textLineNumber;
    }
}
