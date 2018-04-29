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

package com.duy.ccppcompiler.diagnostic;

import android.support.annotation.MainThread;
import android.support.v4.util.Pair;
import android.view.View;

import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;
import com.duy.ccppcompiler.compiler.diagnostic.suggestion.ISuggestion;
import com.duy.common.DLog;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.ui.manager.TabManager;
import com.sothree.slidinguppanel.SlidingUpPanelLayout;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticPresenter implements DiagnosticContract.Presenter {
    private static final String TAG = "DiagnosticPresenter";
    private final EditorActivity mActivity;
    private final TabManager mTabManager;
    private ArrayList<Diagnostic> diagnostics;
    private DiagnosticContract.View mView;

    public DiagnosticPresenter(DiagnosticContract.View view, EditorActivity activity, TabManager tabManager) {
        mActivity = activity;
        mTabManager = tabManager;
        mView = view;
        if (mView != null) {
            mView.setPresenter(this);
        }
    }

    @SuppressWarnings("ConstantConditions")
    @MainThread
    @Override
    public void onDiagnosticClick(View view, Diagnostic diagnostic) {
        if (DLog.DEBUG)
            DLog.d(TAG, "onDiagnosticClick() called with: view = [" + view + "], diagnostic = [" + diagnostic + "]");
        Object source = diagnostic.getSourceFile();
        if (source instanceof File) {
            File file = (File) source;
            Pair<Integer, EditorDelegate> pair = mTabManager.getEditorDelegate(file);
            if (pair != null) {
                int index = pair.first;
                EditorDelegate editorDelegate = pair.second;
                if (editorDelegate.isChanged()) {
                    return;
                }

                mTabManager.setCurrentTab(index);
                editorDelegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));

                Command command = new Command(Command.CommandEnum.GOTO_LINE_COL);
                command.args.putInt("line", (int) diagnostic.getLineNumber());
                command.args.putInt("col", (int) diagnostic.getColumnNumber());
                editorDelegate.doCommand(command);
            } else {
                mTabManager.newTab(file);
                onDiagnosticClick(view, diagnostic);
            }
        }
    }

    @Override
    public void onSuggestionClick(ISuggestion suggestion) {

    }

    @Override
    public void showView() {
        mActivity.mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.EXPANDED);
    }

    @Override
    public void hideView() {
        mActivity.mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.COLLAPSED);
    }

    @MainThread
    public void setDiagnostics(ArrayList<Diagnostic> diagnostics) {
        this.diagnostics = diagnostics;
        if (mView != null) {
            mView.show(diagnostics);
        }
    }
}
