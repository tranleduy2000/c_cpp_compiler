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

import android.app.ProgressDialog;

import com.duy.ide.compiler.ICompileManager;
import com.duy.ide.compiler.shell.ShellResult;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileManager implements ICompileManager {
    private ProgressDialog mCompileDialog;
    private EditorActivity mActivity;

    public CompileManager(EditorActivity activity) {
        mCompileDialog = new ProgressDialog(activity);
        mActivity = activity;
    }

    @Override
    public void onCompileSuccess(ShellResult shellResult) {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }
        UIUtils.toast(mActivity, "Compile failed");
    }

    @Override
    public void onCompileFailed(ShellResult shellResult) {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }
    }
}
