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
        File source = diagnostic.getSourceFile();
        Pair<Integer, EditorDelegate> pair = mTabManager.getEditorDelegate(source);
        if (pair != null) {
            int index = pair.first;
            EditorDelegate editorDelegate = pair.second;
            if (editorDelegate.isChanged()) {
                return;
            }

            mTabManager.setCurrentTab(index);
            editorDelegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));

            Command command = new Command(Command.CommandEnum.GOTO_INDEX);
            command.args.putInt("line", (int) diagnostic.getLineNumber());
            command.args.putInt("col", (int) diagnostic.getColumnNumber());
            editorDelegate.doCommand(command);
        } else {
            mTabManager.newTab(source);
            onDiagnosticClick(view, diagnostic);
        }
    }

    @SuppressWarnings("ConstantConditions")
    @Override
    public void onSuggestionClick(Diagnostic diagnostic, ISuggestion suggestion) {
        File source = suggestion.getSourceFile();
        EditorDelegate delegate = openEditor(source);
        int start = delegate.getEditText()
                .getCursorIndex(suggestion.getLineStart(), suggestion.getColStart()).offset;
        int end = delegate.getEditText()
                .getCursorIndex(suggestion.getLineEnd(), suggestion.getColEnd()).offset;
        if (start >= 0 && start <= end) {
            delegate.getEditableText().replace(start, end, suggestion.getMessage());
            delegate.getEditText().setSelection(start + suggestion.getMessage().length());
        }
        mView.remove(diagnostic);
    }

    @SuppressWarnings("ConstantConditions")
    @MainThread
    private EditorDelegate openEditor(File source) {
        Pair<Integer, EditorDelegate> pair = mTabManager.getEditorDelegate(source);
        if (pair != null) {
            int index = pair.first;
            EditorDelegate editorDelegate = pair.second;
            if (editorDelegate.isChanged()) {
                return null;
            }

            mTabManager.setCurrentTab(index);
            editorDelegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));
            return pair.second;
        }
        return null;
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
    @Override
    public void setDiagnostics(ArrayList<Diagnostic> diagnostics) {
        this.diagnostics = diagnostics;
        if (mView != null) {
            mView.show(diagnostics);
        }
        EditorDelegate delegate = mTabManager.getEditorPagerAdapter().getCurrentEditorDelegate();
        if (delegate != null) {
            delegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));
            for (Diagnostic diagnostic : diagnostics) {
                File sourceFile = diagnostic.getSourceFile();
                if (sourceFile.getPath().equals(delegate.getPath())) {
                    Command command = new Command(Command.CommandEnum.HIGHLIGHT_ERROR);
                    if (diagnostic.getSuggestion() != null) {
                        ISuggestion suggestion = diagnostic.getSuggestion();
                        command.args.putInt("line", suggestion.getLineStart());
                        command.args.putInt("col", suggestion.getColStart());
                        command.args.putInt("lineEnd", suggestion.getLineEnd());
                        command.args.putInt("colEnd", suggestion.getColEnd());
                    } else {
                        command.args.putInt("line", (int) diagnostic.getLineNumber());
                        command.args.putInt("col", (int) diagnostic.getColumnNumber());
                    }
                    delegate.doCommand(command);
                }
            }
        }
    }
}
