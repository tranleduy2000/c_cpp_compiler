/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.core.text.method;

import android.core.text.Layout;
import android.core.widget.BaseEditorView;
import android.text.Spannable;
import android.view.MotionEvent;
import android.view.View;

/**
 * A movement method that interprets movement keys by scrolling the text buffer.
 */
public class ScrollingMovementMethod extends BaseMovementMethod implements MovementMethod {
    private static ScrollingMovementMethod sInstance;

    public static MovementMethod getInstance() {
        if (sInstance == null)
            sInstance = new ScrollingMovementMethod();

        return sInstance;
    }

    @Override
    protected boolean left(BaseEditorView widget, Spannable buffer) {
        return scrollLeft(widget, buffer, 1);
    }

    @Override
    protected boolean right(BaseEditorView widget, Spannable buffer) {
        return scrollRight(widget, buffer, 1);
    }

    @Override
    protected boolean up(BaseEditorView widget, Spannable buffer) {
        return scrollUp(widget, buffer, 1);
    }

    @Override
    protected boolean down(BaseEditorView widget, Spannable buffer) {
        return scrollDown(widget, buffer, 1);
    }

    @Override
    protected boolean pageUp(BaseEditorView widget, Spannable buffer) {
        return scrollPageUp(widget, buffer);
    }

    @Override
    protected boolean pageDown(BaseEditorView widget, Spannable buffer) {
        return scrollPageDown(widget, buffer);
    }

    @Override
    protected boolean top(BaseEditorView widget, Spannable buffer) {
        return scrollTop(widget, buffer);
    }

    @Override
    protected boolean bottom(BaseEditorView widget, Spannable buffer) {
        return scrollBottom(widget, buffer);
    }

    @Override
    protected boolean lineStart(BaseEditorView widget, Spannable buffer) {
        return scrollLineStart(widget, buffer);
    }

    @Override
    protected boolean lineEnd(BaseEditorView widget, Spannable buffer) {
        return scrollLineEnd(widget, buffer);
    }

    @Override
    protected boolean home(BaseEditorView widget, Spannable buffer) {
        return top(widget, buffer);
    }

    @Override
    protected boolean end(BaseEditorView widget, Spannable buffer) {
        return bottom(widget, buffer);
    }

    @Override
    public boolean onTouchEvent(BaseEditorView widget, Spannable buffer, MotionEvent event) {
        return Touch.onTouchEvent(widget, buffer, event);
    }

    @Override
    public void onTakeFocus(BaseEditorView widget, Spannable text, int dir) {
        Layout layout = widget.getLayout();

        if (layout != null && (dir & View.FOCUS_FORWARD) != 0) {
            widget.scrollTo(widget.getScrollX(),
                    layout.getLineTop(0));
        }
        if (layout != null && (dir & View.FOCUS_BACKWARD) != 0) {
            int padding = widget.getTotalPaddingTop() +
                    widget.getTotalPaddingBottom();
            int line = layout.getLineCount() - 1;

            widget.scrollTo(widget.getScrollX(),
                    layout.getLineTop(line + 1) -
                            (widget.getHeight() - padding));
        }
    }
}
