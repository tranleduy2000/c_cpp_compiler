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

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.console.ConsoleActivity;
import com.duy.common.DLog;
import com.duy.ide.compiler.ICompileManager;
import com.duy.ide.compiler.shell.ShellResult;
import com.jecelyin.common.utils.UIUtils;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileManager implements ICompileManager {
    private static final String TAG = "CompileManager";
    private ProgressDialog mCompileDialog;
    private Activity mActivity;

    public CompileManager(Activity activity) {
        mCompileDialog = new ProgressDialog(activity);
        mActivity = activity;
    }

    @Override
    public void onPrepareCompile() {
        mCompileDialog.setTitle(R.string.title_compiling);
        mCompileDialog.show();
    }

    @Override
    public void onNewMessage(CharSequence charSequence) {
        mCompileDialog.setMessage(charSequence);
    }

    @Override
    public void onCompileSuccess(ShellResult shellResult) {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }
        File internalDir = mActivity.getFilesDir();
        String path = new File(internalDir, GCCConstants.TEMP_BINARY_NAME).getAbsolutePath();
        Intent intent = new Intent(mActivity, ConsoleActivity.class);
        intent.putExtra(ConsoleActivity.EXTRA_BINARY_FILE_PATH, path);
        mActivity.startActivity(intent);
    }

    @Override
    public void onCompileFailed(ShellResult shellResult) {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.setMessage(shellResult.getMessage());
//            mCompileDialog.dismiss();
        }
        UIUtils.toast(mActivity, "Compile failed");
        if (DLog.DEBUG) DLog.w(TAG, "onCompileFailed: \n" + shellResult.getMessage());
    }

}
