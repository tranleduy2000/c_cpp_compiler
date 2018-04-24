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

import android.core.widget.BaseEditorView;
import android.text.Spannable;
import android.view.KeyEvent;
import android.view.MotionEvent;


/**
 * Provides cursor positioning, scrolling and text selection functionality in a {@link android.widget.TextView}.
 * <p>
 * The {@link android.widget.TextView} delegates handling of key events, trackball motions and touches to
 * the movement method for purposes of content navigation.  The framework automatically
 * selects an appropriate movement method based on the content of the {@link android.widget.TextView}.
 * </p><p>
 * This interface is intended for use by the framework; it should not be implemented
 * directly by applications.
 * </p>
 */
public interface MovementMethod {
    public void initialize(BaseEditorView widget, Spannable text);

    public boolean onKeyDown(BaseEditorView widget, Spannable text, int keyCode, KeyEvent event);

    public boolean onKeyUp(BaseEditorView widget, Spannable text, int keyCode, KeyEvent event);

    /**
     * If the key listener wants to other kinds of key events, return true,
     * otherwise return false and the caller (i.e. the widget host)
     * will handle the key.
     */
    public boolean onKeyOther(BaseEditorView view, Spannable text, KeyEvent event);

    public void onTakeFocus(BaseEditorView widget, Spannable text, int direction);

    public boolean onTrackballEvent(BaseEditorView widget, Spannable text, MotionEvent event);

    public boolean onTouchEvent(BaseEditorView widget, Spannable text, MotionEvent event);

    public boolean onGenericMotionEvent(BaseEditorView widget, Spannable text, MotionEvent event);

    /**
     * Returns true if this movement method allows arbitrary selection
     * of any text; false if it has no selection (like a movement method
     * that only scrolls) or a constrained selection (for example
     * limited to links.  The "Select All" menu item is disabled
     * if arbitrary selection is not allowed.
     */
    public boolean canSelectArbitrarily();
}
