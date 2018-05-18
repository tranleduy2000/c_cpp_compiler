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

import com.duy.ccppcompiler.compiler.shell.GccCommandResult;
import com.duy.editor.EditorActivity;
import com.pdaxrom.cctools.BuildConstants;
import com.pdaxrom.cctools.TermActivity;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileManager extends CompileManagerImpl<GccCommandResult> {

    public CompileManager(EditorActivity activity) {
        super(activity);
    }

    @Override
    public void onCompileSuccess(GccCommandResult commandResult) {
        super.onCompileSuccess(commandResult);

        //now run binary file
        File binFile = commandResult.getBinaryFile();
        if (binFile != null) {
            Intent intent = new Intent(mActivity, TermActivity.class);
            intent.putExtra(BuildConstants.EXTRA_FILE_NAME, binFile.getAbsolutePath());
            intent.putExtra(BuildConstants.EXTRA_WORK_DIR, binFile.getParent());
            mActivity.startActivity(intent);
        }
    }

}
