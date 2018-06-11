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

package com.duy.ide.code.api;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ide.editor.view.IEditAreaView;

/**
 * The item will be display in list suggestion
 * <p>
 * Type|name|description
 * <p>
 * Created by Duy on 20-Jul-17.
 */

public interface SuggestItem {
    /**
     * Display name
     */
    @Nullable
    String getName();

    /**
     * Display description
     */
    @Nullable
    String getDescription();

    /**
     * Display type
     */
    @Nullable
    String getReturnType();

    int getSuggestionPriority();

    /**
     * Return a single character to display type of suggestion
     * v: variable
     * f: function
     * m: method
     * etc
     */
    char getTypeHeader();

    /**
     * @param editAreaView - editor you will modify for this suggestion
     */
    void onSelectThis(@NonNull IEditAreaView editAreaView);
}