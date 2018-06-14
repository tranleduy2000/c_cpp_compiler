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

package com.duy.ide.code.format;

import android.content.Context;
import android.support.annotation.Nullable;

import com.duy.ide.code.api.CodeFormatProvider;
import com.duy.ide.code.api.CodeFormatter;
import com.duy.ide.code.model.JsonFormatter;
import com.duy.ide.code.model.XmlFormatter;
import com.duy.ide.editor.IEditorDelegate;

import java.io.File;

public class CodeFormatProviderImpl implements CodeFormatProvider {

    private Context context;

    public CodeFormatProviderImpl(Context context) {

        this.context = context;
    }

    @Nullable
    @Override
    public CodeFormatter getFormatterForFile(File file, IEditorDelegate delegate) {
        if (!file.exists() || file.isDirectory()) {
            return null;
        }
        if (file.getName().equals(".xml")) {
            return new XmlFormatter();
        } else if (file.getName().endsWith(".json")) {
            return new JsonFormatter();
        }
        return null;
    }

    public Context getContext() {
        return context;
    }
}
