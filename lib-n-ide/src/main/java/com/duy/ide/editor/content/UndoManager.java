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

package com.duy.ide.editor.content;

import android.content.SharedPreferences;
import android.widget.EditText;

public class UndoManager implements IUndoManager {
    private static final int MAX_HISTORY_EDIT = 100;

    private UndoRedoHelper mUndoRedoHelper;

    public UndoManager(EditText editText) {
        mUndoRedoHelper = new UndoRedoHelper(editText);
        mUndoRedoHelper.attach();
        mUndoRedoHelper.setMaxHistorySize(MAX_HISTORY_EDIT);
    }

    @Override
    public boolean canRedo() {
        return mUndoRedoHelper.getCanRedo();
    }

    @Override
    public boolean canUndo() {
        return mUndoRedoHelper.getCanUndo();
    }

    @Override
    public void disable() {
        mUndoRedoHelper.detach();
    }

    @Override
    public void enable() {
        mUndoRedoHelper.attach();
    }

    @Override
    public void restore(SharedPreferences preferences) {
        mUndoRedoHelper.restorePersistentState(preferences, "");
    }

    @Override
    public void save(SharedPreferences preferences) {
        SharedPreferences.Editor editor = preferences.edit();
        mUndoRedoHelper.storePersistentState(editor, "");
        editor.apply();
    }

    @Override
    public void undo() {
        if (canUndo()) {
            mUndoRedoHelper.undo();
        }
    }

    @Override
    public void redo() {
        if (canRedo()) {
            mUndoRedoHelper.redo();
        }
    }

}
