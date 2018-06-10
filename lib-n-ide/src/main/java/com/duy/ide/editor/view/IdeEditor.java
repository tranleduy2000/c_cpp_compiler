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

import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.editor.model.EditorIndex;
import com.duy.ide.editor.theme.model.EditorTheme;

import java.util.List;

interface IdeEditor {

    void setTheme(EditorTheme editorTheme);

    EditorTheme getEditorTheme();


    EditorIndex getCursorIndex(int realLine, int column);

    void scrollToLine(int line);

    int getLineForOffset(int offset);

    void setInitLineNumber(int lineCount);

    void setSuggestData(List<SuggestItem> items);
}
