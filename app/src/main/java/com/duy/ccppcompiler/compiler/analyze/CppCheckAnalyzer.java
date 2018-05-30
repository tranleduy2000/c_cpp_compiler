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

package com.duy.ccppcompiler.compiler.analyze;

import android.content.Context;
import android.support.annotation.NonNull;

import com.jecelyin.editor.v2.editor.EditorDelegate;

import java.io.File;

/**
 * https://sourceforge.net/p/cppcheck/wiki/Home/
 */
public class CppCheckAnalyzer implements ICodeAnalyser {
    @NonNull
    private final Context context;
    @NonNull
    private final EditorDelegate delegate;

    public CppCheckAnalyzer(@NonNull Context context, @NonNull EditorDelegate delegate) {
        this.context = context;
        this.delegate = delegate;
    }

    @Override
    public void analyze(File[] sources) {

    }

}
