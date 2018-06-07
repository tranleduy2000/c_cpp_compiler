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

package com.duy.ide.diagnostic;

import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.util.Pair;
import android.view.View;

import com.duy.common.DLog;
import com.duy.ide.core.SimpleEditorActivity;
import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.diagnostic.suggestion.ISuggestion;
import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.editor.IEditorDelegate;
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
    private final ArrayList<PatternAwareOutputParser> mOutputParsers = new ArrayList<>();
    private ArrayList<Message> mMessages;
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
    public void onDiagnosticClick(View view, Message message) {
        if (DLog.DEBUG) {
            DLog.d(TAG, "onDiagnosticClick() called diagnostic = [" + message + "]");
        }
        if (message.getSourcePath() != null) {
            File source = new File(message.getSourcePath());
            if (isSystemFile(source)) {
                UIUtils.alert(mActivity,
                        mActivity.getString(R.string.title_non_project_file),
                        mActivity.getString(R.string.message_non_project_file, source.getPath()));
                return;
            }
            IEditorDelegate editorDelegate = moveToEditor(source, null);
            if (editorDelegate != null) {
                Command command = new Command(Command.CommandEnum.GOTO_INDEX);
                command.args.putInt("line", (int) message.getLineNumber());
                command.args.putInt("col", (int) message.getColumn());
                editorDelegate.doCommand(command);
            }
            hidePanel();
        }
    }

    @SuppressWarnings("ConstantConditions")
    @Override
    public void onSuggestionClick(Message message, ISuggestion suggestion) {
        File source = suggestion.getSourceFile();
        IEditorDelegate delegate = moveToEditor(source, mHashCode.get(source));

        if (delegate != null) {
            int start = delegate.getEditText()
                    .getCursorIndex(suggestion.getLineStart(), suggestion.getColStart()).offset;
            int end = delegate.getEditText()
                    .getCursorIndex(suggestion.getLineEnd(), suggestion.getColEnd()).offset;
            if (start >= 0 && start <= end) {
                delegate.getEditText().getEditableText().replace(start, end, suggestion.getMessage());
                delegate.getEditText().setSelection(start + suggestion.getMessage().length());
            }
            mView.remove(message);
        }
    }

    @Nullable
    @SuppressWarnings("ConstantConditions")
    @MainThread
    private IEditorDelegate moveToEditor(File source, @Nullable byte[] md5) {
        Pair<Integer, IEditorDelegate> pair = mTabManager.getEditorDelegate(source);
        if (pair != null) {
            int index = pair.first;
            IEditorDelegate editorDelegate = pair.second;
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
            if (mTabManager.newTab(source)) {
                return moveToEditor(source, md5);
            }
            return null;
        }
    }

    @Override
    public void showPanel() {
        mActivity.mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.EXPANDED);
    }

    @Override
    public void hidePanel() {
        mActivity.mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.COLLAPSED);
    }

    @MainThread
    @Override
    public void setDiagnostics(ArrayList<Message> messages) {
        mMessages = messages;
        mHashCode.clear();

        show(mMessages);
        highlightError();
    }

    private void show(ArrayList<Message> messages) {
        if (mView != null) {
            mView.showDiagnostic(messages);
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
    private void highlightError() {
        for (IEditorDelegate editorDelegate : mTabManager.getEditorPagerAdapter().getAllEditor()) {
            editorDelegate.doCommand(new Command(Command.CommandEnum.CLEAR_ERROR));
        }

        for (Message message : mMessages) {
            if (message.getKind() != Message.Kind.ERROR) {
                continue;
            }
            if (message.getSourcePath() == null) {
                continue;
            }
            File sourceFile = new File(message.getSourcePath());
            Pair<Integer, IEditorDelegate> position = mTabManager.getEditorDelegate(sourceFile);
            if (position == null) {
                continue;
            }
            @SuppressWarnings("ConstantConditions")
            @NonNull final IEditorDelegate editorDelegate = position.second;
            Command command = new Command(Command.CommandEnum.HIGHLIGHT_ERROR);
            int lineNumber = (int) message.getLineNumber();
            int columnNumber = (int) message.getColumn();
            command.args.putInt("line", lineNumber);
            command.args.putInt("col", columnNumber);
            editorDelegate.doCommand(command);
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

    @Override
    public void setOutputParser(PatternAwareOutputParser... parsers) {
        mOutputParsers.clear();
        mOutputParsers.addAll(Arrays.asList(parsers));
    }
}
