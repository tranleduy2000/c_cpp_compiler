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

import android.app.ProgressDialog;
import android.support.annotation.CallSuper;
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.widget.Toast;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.CompileTask;
import com.duy.ccppcompiler.compiler.compilers.ICompiler;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.OutputParser;
import com.duy.common.DLog;
import com.duy.editor.CodeEditorActivity;
import com.duy.ide.diagnostic.Diagnostic;
import com.duy.ide.diagnostic.DiagnosticPresenter;
import com.duy.ide.diagnostic.DiagnosticsCollector;
import com.duy.ide.core.SimpleEditorActivity;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.widget.menu.MenuDef;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class CompileManagerImpl implements ICompileManager {
    private static final String TAG = "CompileManager";
    @NonNull
    SimpleEditorActivity mActivity;
    @Nullable
    private ProgressDialog mCompileDialog;
    @Nullable
    private DiagnosticPresenter mDiagnosticPresenter;
    private ICompiler mCompiler;

    CompileManagerImpl(@NonNull CodeEditorActivity activity) {
        mActivity = activity;
    }

    @Override
    public void onPrepareCompile() {
        if (mDiagnosticPresenter != null) {
            mDiagnosticPresenter.clear();
        }
        mActivity.setMenuStatus(R.id.action_run, MenuDef.STATUS_DISABLED);
        mActivity.setMenuStatus(R.id.action_build_native_activity, MenuDef.STATUS_DISABLED);

        mCompileDialog = new ProgressDialog(mActivity);
        mCompileDialog.setTitle(mActivity.getString(R.string.compiling));
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
    public void onCompileFailed(CommandResult compileResult) {
        finishCompile(compileResult);

        if (mDiagnosticPresenter != null) {
            mDiagnosticPresenter.showPanel();
        }

        Toast.makeText(mActivity, "Compiled failed", Toast.LENGTH_LONG).show();
        if (DLog.DEBUG) DLog.w(TAG, "onCompileFailed: \n" + compileResult.getMessage());
    }

    @CallSuper
    @Override
    public void onCompileSuccess(CommandResult commandResult) {
        if (DLog.DEBUG) DLog.d(TAG, "onCompileSuccess() commandResult = [" + commandResult + "]");
        finishCompile(commandResult);
    }

    @MainThread
    protected void handleInternalException(Throwable e) {
        if (e == null) {
            return;
        }
        UIUtils.alert(mActivity, e.getMessage());
    }

    private void finishCompile(CommandResult compileResult) {
        hideDialog();

        mActivity.setMenuStatus(R.id.action_run, MenuDef.STATUS_NORMAL);
        mActivity.setMenuStatus(R.id.action_build_native_activity, MenuDef.STATUS_NORMAL);

        if (mDiagnosticPresenter != null) {
            DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
            OutputParser parser = new OutputParser(diagnosticsCollector);
            parser.parse(compileResult.getMessage());

            ArrayList<Diagnostic> diagnostics = diagnosticsCollector.getDiagnostics();
            mDiagnosticPresenter.setDiagnostics(diagnostics);
            mDiagnosticPresenter.log(compileResult.getMessage());
        }
    }

    @MainThread
    private void hideDialog() {
        if (mCompileDialog != null && mCompileDialog.isShowing()) {
            mCompileDialog.dismiss();
        }
    }

    public void setDiagnosticPresenter(DiagnosticPresenter diagnosticPresenter) {
        this.mDiagnosticPresenter = diagnosticPresenter;
    }

    public void setCompiler(ICompiler compiler) {
        this.mCompiler = compiler;
    }

    @NonNull
    public SimpleEditorActivity getActivity() {
        return mActivity;
    }
}
