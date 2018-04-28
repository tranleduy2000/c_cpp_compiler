
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
import android.content.Intent;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.diagnostic.DiagnosticsCollector;
import com.duy.ccppcompiler.compiler.diagnostic.OutputParser;
import com.duy.ccppcompiler.console.ConsoleActivity;
import com.duy.ccppcompiler.diagnostic.DiagnosticPresenter;
import com.duy.common.DLog;
import com.duy.ide.compiler.ICompileManager;
import com.duy.ide.compiler.shell.ShellResult;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.widget.menu.MenuDef;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileManager implements ICompileManager {
    private static final String TAG = "CompileManager";
    private ProgressDialog mCompileDialog;
    private EditorActivity mActivity;
    private DiagnosticPresenter mDiagnosticPresenter;

    public CompileManager(EditorActivity activity) {
        mCompileDialog = new ProgressDialog(activity);
        mActivity = activity;
    }

    @Override
    public void onPrepareCompile() {
        mActivity.setMenuStatus(R.id.action_run, MenuDef.STATUS_DISABLED);
        mCompileDialog.setTitle(R.string.title_compiling);
        mCompileDialog.show();
    }

    @Override
    public void onNewMessage(CharSequence charSequence) {
        mCompileDialog.setMessage(charSequence);
    }

    @Override
    public void onCompileSuccess(ShellResult shellResult) {
        finishCompile();
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }

        File internalDir = mActivity.getFilesDir();
        String path = new File(internalDir, GCCConstants.TEMP_BINARY_NAME).getAbsolutePath();
        Intent intent = new Intent(mActivity, ConsoleActivity.class);
        intent.putExtra(ConsoleActivity.EXTRA_BINARY_FILE_PATH, path);
        mActivity.startActivity(intent);
    }

    private void finishCompile() {
        mActivity.setMenuStatus(R.id.action_run, MenuDef.STATUS_NORMAL);
    }

    @Override
    public void onCompileFailed(ShellResult shellResult) {
        finishCompile();
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }
        if (DLog.DEBUG) DLog.w(TAG, "onCompileFailed: \n" + shellResult.getMessage());
        if (mDiagnosticPresenter != null) {
            DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector<>();
            OutputParser parser = new OutputParser(diagnosticsCollector);
            parser.parse(shellResult.getMessage());
            ArrayList diagnostics = diagnosticsCollector.getDiagnostics();
            mDiagnosticPresenter.setDiagnostics(diagnostics);
            mDiagnosticPresenter.showView();

            debug(diagnostics);
        }
    }

    private void debug(ArrayList diagnostics) {
        for (Object diagnostic : diagnostics) {
            System.out.println(diagnostic);
        }
    }

    public void setDiagnosticPresenter(DiagnosticPresenter diagnosticPresenter) {
        this.mDiagnosticPresenter = diagnosticPresenter;
    }
}
