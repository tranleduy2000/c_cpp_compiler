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

/**
 * Created by Duy on 26-Apr-18.
 */
public interface IUndoManager {
    boolean canRedo();

    void undo();

    void redo();

    boolean canUndo();

    void disable();

    void enable();

    void restore(SharedPreferences preferences);

    void save(SharedPreferences preferences);
}
