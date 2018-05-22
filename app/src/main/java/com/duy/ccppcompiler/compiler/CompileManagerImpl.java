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
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.widget.Toast;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.compilers.INativeCompiler;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.diagnostic.DiagnosticPresenter;
import com.duy.ccppcompiler.diagnostic.DiagnosticsCollector;
import com.duy.ccppcompiler.diagnostic.OutputParser;
import com.duy.common.DLog;
import com.duy.editor.CodeEditorActivity;
import com.duy.ide.editor.SimpleEditorActivity;
import com.jecelyin.editor.v2.widget.menu.MenuDef;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class CompileManagerImpl<T extends CommandResult> implements ICompileManager<T> {
    private static final String TAG = "CompileManager";
    @NonNull
    protected SimpleEditorActivity mActivity;
    @Nullable
    private ProgressDialog mCompileDialog;
    private DiagnosticPresenter mDiagnosticPresenter;
    private INativeCompiler mCompiler;

    CompileManagerImpl(@NonNull CodeEditorActivity activity) {
        mActivity = activity;
    }

    @Override
    public void onPrepareCompile() {
        mDiagnosticPresenter.clear();
        mActivity.setMenuStatus(R.id.action_run, MenuDef.STATUS_DISABLED);

        if (mCompileDialog == null) {
            mCompileDialog = new ProgressDialog(mActivity);
        }
        mCompileDialog.setTitle(mActivity.getString(R.string.title_compiling));
        mCompileDialog.setMessage(mActivity.getString(R.string.wait_message));
        mCompileDialog.setCancelable(false);
        mCompileDialog.setCanceledOnTouchOutside(false);
        mCompileDialog.show();

    }

    @Override
    public void compile(File[] srcFiles) {
        CompileTask compileTask = new CompileTask(mCompiler, srcFiles, this);
        compileTask.execute();
    }

    @Override
    public void onNewMessage(CharSequence charSequence) {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.setMessage(charSequence);
        }
    }

    @Override
    public void onCompileFailed(T commandResult) {
        finishCompile();

        Toast.makeText(mActivity, "Compiled failed", Toast.LENGTH_LONG).show();
        if (DLog.DEBUG) DLog.w(TAG, "onCompileFailed: \n" + commandResult.getMessage());

        if (mDiagnosticPresenter != null) {
            DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
            OutputParser parser = new OutputParser(diagnosticsCollector);
            parser.parse(commandResult.getMessage());

            ArrayList diagnostics = diagnosticsCollector.getDiagnostics();
            mDiagnosticPresenter.setDiagnostics(diagnostics);
            mDiagnosticPresenter.expandView();

            debug(diagnostics);
        }
    }

    @Override
    public void onCompileSuccess(T commandResult) {
        finishCompile();
    }

    private void debug(ArrayList diagnostics) {
        for (Object diagnostic : diagnostics) {
            if (DLog.DEBUG) DLog.d(TAG, "diagnostic = " + diagnostic);
        }
    }

    public void setDiagnosticPresenter(DiagnosticPresenter diagnosticPresenter) {
        this.mDiagnosticPresenter = diagnosticPresenter;
    }

    @MainThread
    protected void finishCompile() {
        mActivity.setMenuStatus(R.id.action_run, MenuDef.STATUS_NORMAL);
        hideDialog();
    }

    @MainThread
    private void hideDialog() {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }
    }

    public void setCompiler(INativeCompiler compiler) {
        this.mCompiler = compiler;
    }
}
