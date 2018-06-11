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

package com.duy.ide.editor.text;

import android.graphics.Paint;

import com.jecelyin.editor.v2.Preferences;


/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class LayoutContext {
    public static final float LINE_NUMBER_FACTOR = 0.8f;

    //gutter, line number attrs
    private Paint gutterForegroundPaint;
    private Paint gutterDividerPaint;
    private Paint gutterBackgroundPaint;
    public int gutterWidth;

    public Preferences preferences;

    public boolean isShowWhiteSpace;
    public int whiteSpaceColor;

    public int lineNumber = -1;
    public int lineNumberX;
    public TextLineNumber textLineNumber = new TextLineNumber();

    private int widthMode;
    private int heightMode;
    private int paddingBottom;

    public LayoutContext() {
        paddingBottom = 920;
    }

    public int getWidthMode() {
        return widthMode;
    }

    public void setWidthMode(int widthMode) {
        this.widthMode = widthMode;
    }

    public int getHeightMode() {
        return heightMode;
    }

    public void setHeightMode(int heightMode) {
        this.heightMode = heightMode;
    }

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

    public int getLineNumber() {
        return lineNumber;
    }

    public void setLineNumber(int lineNumber) {
        this.lineNumber = lineNumber;
    }

    public Paint getGutterForegroundPaint() {
        return gutterForegroundPaint;
    }

    public void setGutterForegroundPaint(Paint gutterForegroundPaint) {
        this.gutterForegroundPaint = gutterForegroundPaint;
    }

    public Paint getGutterDividerPaint() {
        return gutterDividerPaint;
    }

    public void setGutterDividerPaint(Paint gutterDividerPaint) {
        this.gutterDividerPaint = gutterDividerPaint;
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

    public int getPaddingBottom() {
        return paddingBottom;
    }

    public void setPaddingBottom(int paddingBottom) {
        this.paddingBottom = paddingBottom;
    }
}
