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

package com.duy.ide.editor;

import android.support.annotation.Nullable;

import com.duy.ide.code.api.CodeFormatProvider;
import com.duy.ide.code.api.SuggestionProvider;
import com.duy.ide.editor.view.IEditAreaView;
import com.jecelyin.editor.v2.common.Command;

public interface IEditorDelegate {

    void saveCurrentFile() throws Exception;

    void saveInBackground();

    IEditAreaView getEditText();

    int getCursorOffset();

    Document getDocument();

    void onDocumentChanged();

    boolean isChanged();

    String getPath();

    String getEncoding();

    void doCommand(Command command);

    void setCodeFormatProvider(CodeFormatProvider provider);

    void setSuggestionProvider(@Nullable SuggestionProvider provider);
}
