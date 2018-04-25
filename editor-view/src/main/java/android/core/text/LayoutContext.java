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

import com.jecelyin.editor.v2.Pref;


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
    public Pref preference;
    public boolean isShowWhiteSpace;
    public int whiteSpaceColor;
    public TextLineNumber textLineNumber = new TextLineNumber();

    public int getWhiteSpaceColor() {
        return whiteSpaceColor;
    }

    public boolean isShowWhiteSpace() {
        return isShowWhiteSpace;
    }

    public int getCursorThickness() {
        return cursorThickness;
    }

    public int getLineNumber() {
        return lineNumber;
    }

    public void setLineNumber(int lineNumber) {
        this.lineNumber = lineNumber;
    }
}
