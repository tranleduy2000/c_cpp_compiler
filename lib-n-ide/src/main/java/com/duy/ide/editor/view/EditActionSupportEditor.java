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

package com.duy.ide.editor.view;

import android.content.Context;
import android.graphics.Rect;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.Layout;
import android.text.method.KeyListener;
import android.util.AttributeSet;

import com.duy.ide.editor.content.ClipboardCompat;
import com.duy.ide.editor.content.IUndoManager;
import com.duy.ide.editor.content.UndoManager;
import com.duy.ide.editor.model.EditorIndex;

public class EditActionSupportEditor extends ZoomSupportEditor {
    private IUndoManager mUndoManager;
    private ClipboardCompat mClipboard;
    private KeyListener mLastKeyListener;

    public EditActionSupportEditor(Context context) {
        super(context);
        init(context);
    }

    public EditActionSupportEditor(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public EditActionSupportEditor(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mUndoManager = new UndoManager(this);
        mClipboard = new ClipboardCompat(context);
    }

    public void redo() {
        mUndoManager.redo();
    }

    public void undo() {
        mUndoManager.undo();
    }

    @Override
    public boolean doCopy() {
        if (!onTextContextMenuItem(android.R.id.copy)) {
            int selectionStart = getSelectionStart();
            int selectionEnd = getSelectionEnd();
            if (selectionStart != selectionEnd && selectionStart >= 0) {
                mClipboard.setClipboard(getText().subSequence(selectionStart, selectionEnd));
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    @Override
    public boolean doPaste() {
        if (!onTextContextMenuItem(android.R.id.paste)) {
            CharSequence clipboard = mClipboard.getClipboard();
            if (clipboard != null) {
                //clear all span

                int selectionStart = getSelectionStart();
                selectionStart = Math.max(0, selectionStart);
                getText().insert(selectionStart, cleanupForPaste(clipboard));
                return true;
            }
        } else {
            return true;
        }
        return false;
    }

    /**
     * Remove linux line terminate, only support escape \n
     */
    @NonNull
    private String cleanupForPaste(CharSequence clipboard) {
        String result = clipboard.toString();
        result = result.replace("\r\n", "\n");
        return result;
    }

    @Override
    public boolean onTextContextMenuItem(int id) {
        switch (id) {
            case android.R.id.paste:
                doPaste();
                return true;
        }
        return super.onTextContextMenuItem(id);
    }

    @Override
    public boolean doCut() {
        if (!onTextContextMenuItem(android.R.id.cut)) {
            int selectionStart = getSelectionStart();
            int selectionEnd = getSelectionEnd();
            if (selectionStart != selectionEnd && selectionStart >= 0) {
                mClipboard.setClipboard(getText().subSequence(selectionStart, selectionEnd));
                getText().delete(selectionStart, selectionEnd);
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    @Override
    public void duplicateSelection() {
        EditorCompat.duplicateSelection(this);
    }

    @Override
    public void setReadOnly(boolean readOnly) {
        if (readOnly) {
            if (mLastKeyListener == null) {
                mLastKeyListener = getKeyListener();
            }
            setKeyListener(null);
        } else {
            if (mLastKeyListener != null) {
                setKeyListener(mLastKeyListener);
                mLastKeyListener = null;
            }
        }
    }

    @Override
    public void gotoTop() {
        EditorCompat.gotoTop(this);
    }

    @Override
    public void gotoEnd() {
        EditorCompat.gotoEnd(this);
    }

    @Override
    @Nullable
    public EditorIndex getCursorIndex(int realLine, int column) {
        if (getLayout() == null) {
            return null;
        }
        if (realLine <= 0 || realLine > getLineCount())
            return null;

        int virtualLine = mLineManager.realLineToVirtualLine(realLine);
        if (virtualLine < 0)
            return null;

        int offset = getLayout().getLineStart(virtualLine);
        if (column > 0) {
            offset += column - 1; /*the column start at 1*/
        }
        if (offset > length()) {
            return null;
        }
        return new EditorIndex(virtualLine, column, offset);
    }


    /**
     * @param virtualLine scroll editor to virtualLine
     */
    public void scrollToLine(int virtualLine) {
        virtualLine = Math.max(0, Math.min(virtualLine, getLineCount() - 1));
        final Layout layout = getLayout();
        final int layoutHeight = layout.getHeight();


        Rect bound = new Rect();
        getGlobalVisibleRect(bound);
        final int visibleHeight = bound.height();

        int y = layout.getLineTop(virtualLine);
        if (layoutHeight - visibleHeight >= 0) {
            y = Math.min(y, layoutHeight - visibleHeight);
        }
        scrollTo(getScrollX(), y);
    }

}
