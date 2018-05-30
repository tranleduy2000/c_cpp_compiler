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

package com.duy.ccppcompiler.compiler.manager;

import android.content.Intent;

import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.CompileResult;
import com.duy.editor.CodeEditorActivity;
import com.jecelyin.common.utils.UIUtils;

import java.io.File;

public class SDLCompileManager extends CompileManager {

    public SDLCompileManager(CodeEditorActivity activity) {
        super(activity);
    }

    @Override
    public void onCompileSuccess(CommandResult result) {
        if (result instanceof CompileResult) {
            File binaryFile = ((CompileResult) result).getBinaryFile();
            if (binaryFile == null) {
                super.onCompileSuccess(result);
                return;
            }

            Intent intent = getActivity().getPackageManager().getLaunchIntentForPackage("com.duy.c.cpp.compiler.sdlplugin");
            if (intent != null) {
                intent.putExtra("sdlmain", binaryFile.getAbsolutePath());
                getActivity().startActivity(intent);//null pointer check in case package name was not found
            } else {
                UIUtils.alert(getActivity(), "Can not run SDL application");
            }
        } else {
            super.onCompileSuccess(result);
        }
    }
}
