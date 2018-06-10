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

import android.annotation.SuppressLint;
import android.os.Handler;
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.WorkerThread;
import android.support.v4.util.Pair;
import android.view.View;

import com.duy.common.DLog;
import com.duy.ide.core.api.IdeActivity;
import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.diagnostic.parser.ToolOutputParser;
import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.editor.IEditorDelegate;
import com.jecelyin.editor.v2.manager.TabManager;
import com.sothree.slidinguppanel.SlidingUpPanelLayout;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticPresenter implements DiagnosticContract.Presenter {
    private static final String TAG = "DiagnosticPresenter";
    private final IdeActivity mActivity;
    private final TabManager mTabManager;
    private final Handler mHandler;
    @Nullable
    private ToolOutputParser mToolOutputParser = null;
    private DiagnosticContract.View mView;

    private OutputStream mStdOut;
    private OutputStream mStdErr;

    private DiagnosticContract.MessageFilter mMessageFilter = new DiagnosticContract.MessageFilter() {
        @Override
        public boolean accept(Message message) {
            return true;
        }
    };

    public DiagnosticPresenter(@Nullable DiagnosticContract.View view,
                               @NonNull IdeActivity activity,
                               @NonNull TabManager tabManager,
                               @NonNull Handler handler) {
        mActivity = activity;
        mTabManager = tabManager;
        mView = view;
        if (mView != null) {
            mView.setPresenter(this);
        }
        mHandler = handler;
        createIOStream();
    }

    private void createIOStream() {
        mStdOut = new OutputStream() {
            @Override
            public void write(int b) throws IOException {
                write(new byte[]{(byte) b}, 0, 1);
            }

            @Override
            public void write(@NonNull byte[] b, int off, int len) throws IOException {
                onNewMessage(new String(b, off, len));
            }
        };
        mStdErr = new OutputStream() {
            @Override
            public void write(int b) throws IOException {
                write(new byte[]{(byte) b}, 0, 1);
            }

            @Override
            public void write(@NonNull byte[] b, int off, int len) throws IOException {
                onNewMessage(new String(b, off, len));
            }
        };

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
            IEditorDelegate editorDelegate = moveToEditor(source);
            if (editorDelegate != null) {
                Command command = new Command(Command.CommandEnum.GOTO_INDEX);
                command.args.putInt("line", message.getLineNumber());
                command.args.putInt("col", message.getColumn());
                editorDelegate.doCommand(command);
            }
            hidePanel();
        }
    }

    @Nullable
    @SuppressWarnings("ConstantConditions")
    @MainThread
    private IEditorDelegate moveToEditor(File source) {
        Pair<Integer, IEditorDelegate> pair = mTabManager.getEditorDelegate(source);
        if (pair != null) {
            int index = pair.first;
            IEditorDelegate editorDelegate = pair.second;
            mTabManager.setCurrentTab(index);
            editorDelegate.doCommand(new Command(Command.CommandEnum.REQUEST_FOCUS));
            return editorDelegate;
        } else {
            if (mTabManager.newTab(source)) {
                return moveToEditor(source);
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

    @SuppressLint("WrongThread")
    @Override
    public void setMessages(ArrayList<Message> messages) {
        show(filter(messages));
        highlightError(messages);
    }

    private void show(List<Message> messages) {
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
    @WorkerThread
    private void highlightError(final List<Message> messages) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                for (Message message : messages) {
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
                    int lineNumber = message.getLineNumber();
                    int columnNumber = message.getColumn();
                    command.args.putInt("line", lineNumber);
                    command.args.putInt("col", columnNumber);
                    editorDelegate.doCommand(command);
                }
            }
        });

    }


    @SuppressLint("WrongThread")
    @Override
    public void clear() {
        mView.clearAll();
    }

    @Override
    public void setCurrentItem(int index) {
        if (mView != null) {
            mView.setCurrentItem(index);
        }
    }

    @Override
    public void setOutputParser(@NonNull PatternAwareOutputParser... parsers) {
        mToolOutputParser = new ToolOutputParser(parsers, this);
    }

    @NonNull
    @Override
    public OutputStream getStandardOutput() {
        return mStdOut;
    }

    @NonNull
    @Override
    public OutputStream getErrorOutput() {
        return mStdErr;
    }

    @Override
    public void setFilter(@NonNull DiagnosticContract.MessageFilter filter) {
        this.mMessageFilter = filter;
    }

    @Override
    public void onNewMessage(String text) {
        if (mView != null) {
            mView.printMessage(text);
        }
        if (mToolOutputParser == null) {
            return;
        }

        //parse output, show diagnosis
        List<Message> messages = mToolOutputParser.parseToolOutput(text);
        messages = filter(messages);

        mView.addMessage(messages);
        highlightError(messages);
    }

    private List<Message> filter(List<Message> messages) {
        ArrayList<Message> result = new ArrayList<>();
        for (Message message : messages) {
            if (mMessageFilter.accept(message)) {
                result.add(message);
            }
        }
        return result;
    }

    @Override
    public void error(@Nullable Throwable t, @Nullable String msgFormat, Object... args) {

    }

    @Override
    public void warning(@NonNull String msgFormat, Object... args) {

    }

    @Override
    public void info(@NonNull String msgFormat, Object... args) {

    }

    @Override
    public void verbose(@NonNull String msgFormat, Object... args) {

    }
}
