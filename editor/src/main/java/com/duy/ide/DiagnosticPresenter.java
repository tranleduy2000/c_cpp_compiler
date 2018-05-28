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

package com.duy.ide;

import android.support.annotation.MainThread;
import android.support.annotation.Nullable;
import android.support.v4.util.Pair;
import android.view.View;

import com.duy.common.DLog;
import com.duy.ide.editor.SimpleEditorActivity;
import com.duy.ide.editor.editor.R;
import com.duy.ide.suggestion.ISuggestion;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.manager.TabManager;
import com.sothree.slidinguppanel.SlidingUpPanelLayout;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticPresenter implements DiagnosticContract.Presenter {
    private static final String TAG = "DiagnosticPresenter";
    private final SimpleEditorActivity mActivity;
    private final TabManager mTabManager;
    private ArrayList<Diagnostic> mDiagnostics;
    private DiagnosticContract.View mView;
    private HashMap<File, byte[]> mHashCode = new HashMap<>();

    public DiagnosticPresenter(DiagnosticContract.View view, SimpleEditorActivity activity, TabManager tabManager) {
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
        if (DLog.DEBUG) {
            DLog.d(TAG, "onDiagnosticClick() called diagnostic = [" + diagnostic + "]");
        }
        File source = diagnostic.getSourceFile();

        if (source != null) {
            if (isSystemFile(source)) {
                UIUtils.alert(mActivity,
                        mActivity.getString(R.string.title_non_project_file),
                        mActivity.getString(R.string.message_non_project_file, source.getPath()));
                return;
            }
            EditorDelegate editorDelegate = moveToEditor(source, null);
            if (editorDelegate != null) {
                Command command = new Command(Command.CommandEnum.GOTO_INDEX);
                command.args.putInt("line", (int) diagnostic.getLineNumber());
                command.args.putInt("col", (int) diagnostic.getColumnNumber());
                editorDelegate.doCommand(command);
            }
        }
    }

    @SuppressWarnings("ConstantConditions")
    @Override
    public void onSuggestionClick(Diagnostic diagnostic, ISuggestion suggestion) {
        File source = suggestion.getSourceFile();
        EditorDelegate delegate = moveToEditor(source, mHashCode.get(source));

        if (delegate != null) {
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
    }

    @Nullable
    @SuppressWarnings("ConstantConditions")
    @MainThread
    private EditorDelegate moveToEditor(File source, @Nullable byte[] md5) {
        Pair<Integer, EditorDelegate> pair = mTabManager.getEditorDelegate(source);
        if (pair != null) {
            int index = pair.first;
            EditorDelegate editorDelegate = pair.second;
            if (editorDelegate.isChanged()) {
                return null;
            }

            if (editorDelegate.isChanged()) {
                return null;
            }
            byte[] otherMd5 = editorDelegate.getDocument().getMd5();
            if (md5 != null) {
                if (!Arrays.equals(md5, otherMd5)) {
                    return null;
                }
            }
            mHashCode.put(source, otherMd5);

            mTabManager.setCurrentTab(index);
            editorDelegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));
            return editorDelegate;
        } else {
            mTabManager.newTab(source);
            return moveToEditor(source, md5);
        }
    }

    @Override
    public void expandView() {
        mActivity.mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.EXPANDED);
    }

    @Override
    public void hideView() {
        mActivity.mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.COLLAPSED);
    }

    @MainThread
    @Override
    public void setDiagnostics(ArrayList<Diagnostic> diagnostics) {
        mDiagnostics = diagnostics;
        mHashCode.clear();

        show(mDiagnostics);
        highlightErrorCurrentEditor();
    }

    private void show(ArrayList<Diagnostic> diagnostics) {
        if (mView != null) {
            mView.showDiagnostic(diagnostics);
        }
    }

    private boolean isSystemFile(File file) {
        try {
            //data dir
            File systemDir = mActivity.getFilesDir().getParentFile();
            return file.getCanonicalPath().startsWith(systemDir.getCanonicalPath());
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * Show error in current editor,
     * Find first error index and move cursor to it
     */
    private void highlightErrorCurrentEditor() {
        EditorDelegate delegate = mTabManager.getEditorPagerAdapter().getCurrentEditorDelegate();
        if (delegate == null) {
            return;
        }

        delegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));
        delegate.doCommand(new Command(Command.CommandEnum.CLEAR_ERROR));

        boolean firstIndex = false;

        for (Diagnostic diagnostic : mDiagnostics) {
            File sourceFile = diagnostic.getSourceFile();
            if (sourceFile == null) {
                continue;
            }
            if (sourceFile.equals(delegate.getDocument().getFile())) {
                Command command = new Command(Command.CommandEnum.HIGHLIGHT_ERROR);
                int lineNumber = (int) diagnostic.getLineNumber();
                int columnNumber = (int) diagnostic.getColumnNumber();

                command.args.putInt("line", lineNumber);
                command.args.putInt("col", columnNumber);
                delegate.doCommand(command);

                if (!firstIndex) {
                    Command gotoIndexCmd = new Command(Command.CommandEnum.GOTO_INDEX);
                    gotoIndexCmd.args.putAll(command.args);
                    delegate.doCommand(gotoIndexCmd);
                    firstIndex = true;
                }
            }
        }
    }

    @Override
    public void log(String message) {
        mView.showLog(message);
    }

    @Override
    public void clear() {
        mView.clear();
    }
}
