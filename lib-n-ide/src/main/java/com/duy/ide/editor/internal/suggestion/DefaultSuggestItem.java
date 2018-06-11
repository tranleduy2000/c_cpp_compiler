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

package com.duy.ide.editor.internal.suggestion;

import android.support.annotation.NonNull;

import com.duy.ide.editor.view.IEditAreaView;

public class DefaultSuggestItem implements com.duy.ide.code.api.SuggestItem {
    private String name;
    private String desc;
    private String returnType;
    private String insertText;
    private int priority;

    public DefaultSuggestItem() {

    }

    public DefaultSuggestItem(String name, String desc, String returnType,
                              String insertText, int priority) {
        this.name = name;
        this.desc = desc;
        this.returnType = returnType;
        this.insertText = insertText;

        this.priority = priority;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public String getDescription() {
        return desc;
    }

    @Override
    public String getReturnType() {
        return returnType;
    }

    @Override
    public int getSuggestionPriority() {
        return priority;
    }

    @Override
    public char getTypeHeader() {
        return 0;
    }

    @Override
    public void onSelectThis(@NonNull IEditAreaView editAreaView) {

    }
}
