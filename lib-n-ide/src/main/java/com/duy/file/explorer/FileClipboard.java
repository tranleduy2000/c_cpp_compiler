/*
 * Copyright (C) 2018 Tran Le Duy
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.file.explorer;

import android.app.ProgressDialog;
import android.content.Context;
import android.text.TextUtils;

import com.duy.ide.editor.editor.R;
import com.duy.file.explorer.io.JecFile;
import com.duy.file.explorer.listener.OnClipboardDataChangedListener;
import com.duy.file.explorer.listener.OnClipboardPasteFinishListener;
import com.duy.file.explorer.util.FileUtils;
import com.jecelyin.common.task.JecAsyncTask;
import com.jecelyin.common.task.TaskResult;
import com.jecelyin.common.utils.UIUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */

public class FileClipboard {
    private List<JecFile> clipList = new ArrayList<>();
    private boolean isCopy;
    private OnClipboardDataChangedListener onClipboardDataChangedListener;

    public boolean canPaste() {
        return !clipList.isEmpty();
    }

    public void setData(boolean isCopy, List<JecFile> data) {
        this.isCopy = isCopy;
        clipList.clear();
        clipList.addAll(data);
        if (onClipboardDataChangedListener != null)
            onClipboardDataChangedListener.onClipboardDataChanged();
    }

    public void paste(Context context, JecFile currentDirectory, OnClipboardPasteFinishListener listener) {
        if (!canPaste())
            return;

        ProgressDialog dlg = new ProgressDialog(context);
        PasteTask task = new PasteTask(listener);
        task.setProgress(dlg);
        task.execute(currentDirectory);
    }

    public void showPasteResult(Context context, int count, String error) {
        if (TextUtils.isEmpty(error)) {
            UIUtils.toast(context, R.string.x_items_completed, count);
        } else {
            UIUtils.toast(context, R.string.x_items_completed_and_error_x, count, error);
        }
    }

    private class PasteTask extends JecAsyncTask<JecFile, JecFile, Integer> {
        private final OnClipboardPasteFinishListener listener;
        private StringBuilder errorMsg = new StringBuilder();

        public PasteTask(OnClipboardPasteFinishListener listener) {
            this.listener = listener;
        }

        @Override
        protected void onProgressUpdate(JecFile... values) {
            getProgress().setMessage(values[0].getPath());
        }

        @Override
        protected void onRun(TaskResult<Integer> taskResult, JecFile... params) throws Exception {
            JecFile currentDirectory = params[0];
            int count = 0;
            for (JecFile file : clipList) {
                publishProgress(file);
                try {
                    if (file.isDirectory()) {
                        FileUtils.copyDirectory(file, currentDirectory, !isCopy);
                    } else {
                        FileUtils.copyFile(file, currentDirectory.newFile(file.getName()), !isCopy);
                    }
                    count++;
                } catch (Exception e) {
                    errorMsg.append(e.getMessage()).append("\n");
                }
            }
            clipList.clear();
            taskResult.setResult(count);
        }

        @Override
        protected void onSuccess(Integer integer) {
            if (listener != null) {
                listener.onFinish(integer, errorMsg.toString());
            }
        }

        @Override
        protected void onError(Exception e) {
            if (listener != null) {
                listener.onFinish(0, e.getMessage());
            }
        }
    }

    public void setOnClipboardDataChangedListener(OnClipboardDataChangedListener onClipboardDataChangedListener) {
        this.onClipboardDataChangedListener = onClipboardDataChangedListener;
    }
}
