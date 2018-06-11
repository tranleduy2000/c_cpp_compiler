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

package com.duy.ide.editor.pager;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.jecelyin.editor.v2.adapter.TabAdapter;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.duy.ide.editor.EditorDelegate;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */
public interface IEditorPagerAdapter {

    void removeAll(TabCloseListener tabCloseListener);

    void newEditor(@NonNull File file, int offset, String encoding);

    @Nullable
    EditorDelegate getCurrentEditorDelegate();

    TabAdapter.TabInfo[] getTabInfoList();

    void removeEditor(int position, TabCloseListener listener);

    @Nullable
    EditorDelegate getEditorDelegateAt(int index);

}
