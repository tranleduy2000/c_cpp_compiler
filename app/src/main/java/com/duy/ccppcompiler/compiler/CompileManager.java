/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.duy.ccppcompiler.compiler;

import android.content.Intent;

import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.CompileResult;
import com.duy.ccppcompiler.compiler.shell.NativeActivityCompileResult;
import com.duy.ccppcompiler.console.TermActivity;
import com.duy.editor.CodeEditorActivity;
import com.pdaxrom.cctools.BuildConstants;
import com.pdaxrom.cctools.LauncherNativeActivity;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileManager extends CompileManagerImpl {

    public CompileManager(CodeEditorActivity activity) {
        super(activity);
    }

    @Override
    public void onCompileSuccess(CommandResult result) {
        super.onCompileSuccess(result);

        if (result instanceof NativeActivityCompileResult) {
            //now run binary file
            File binFile = ((NativeActivityCompileResult) result).getBinaryFile();
            if (binFile != null) {
                Intent intent = new Intent(getActivity(), LauncherNativeActivity.class);
                intent.putExtra(BuildConstants.EXTRA_FILE_NAME, binFile.getAbsolutePath());
                intent.putExtra(BuildConstants.EXTRA_WORK_DIR, binFile.getParent());
                mActivity.startActivity(intent);
            }

        } else if (result instanceof CompileResult) {
            File binFile = ((CompileResult) result).getBinaryFile();
            if (binFile != null) {
                Intent intent = new Intent(getActivity(), TermActivity.class);
                intent.putExtra(BuildConstants.EXTRA_FILE_NAME, binFile.getAbsolutePath());
                intent.putExtra(BuildConstants.EXTRA_WORK_DIR, binFile.getParent());
                mActivity.startActivity(intent);
            }
        }
    }

}
