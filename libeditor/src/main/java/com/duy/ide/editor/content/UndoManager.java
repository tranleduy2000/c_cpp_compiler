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

package com.duy.ide.editor.content;

import com.duy.ide.editor.view.IEditAreaView;

public class UndoManager implements IUndoManager {
    private UndoRedoHelper mUndoRedoHelper;

    public UndoManager(IEditAreaView editText) {
        mUndoRedoHelper = new UndoRedoHelper(editText);
    }

    @Override
    public boolean canRedo() {
        return mUndoRedoHelper.canRedo();
    }

    @Override
    public void undo() {
        if (canUndo()) {
            mUndoRedoHelper.undo();
        }
    }

    @Override
    public void redo() {
        if (canUndo()) {
            mUndoRedoHelper.redo();
        }
    }

    @Override
    public boolean canUndo() {
        return mUndoRedoHelper.canRedo();
    }
}
