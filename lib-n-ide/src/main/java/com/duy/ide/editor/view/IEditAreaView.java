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

package com.duy.ide.editor.view;

import android.content.Context;
import android.os.Parcelable;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.ActionMode;

public interface IEditAreaView extends IEditActionSupport, IdeEditor {
    Editable getEditableText();

    Editable getText();

    void setText(CharSequence spannable);

    void setEnabled(boolean enable);

    boolean post(Runnable runnable);

    void setSelection(int start);

    void setSelection(int start, int end);

    void setReadOnly(boolean readOnly);

    void setCustomSelectionActionModeCallback(ActionMode.Callback actionModeCallback);

    void onRestoreInstanceState(Parcelable editorState);

    void addTextChangedListener(TextWatcher textWatcher);

    void removeTextChangedListener(TextWatcher textWatcher);

    boolean hasSelection();

    int getSelectionStart();

    int getSelectionEnd();

    void gotoTop();

    void gotoEnd();

    boolean requestFocus();

    int length();

    void setFreezesText(boolean b);

    Parcelable onSaveInstanceState();

    Context getContext();

    void clearFocus();

    boolean hasFocus();
}
