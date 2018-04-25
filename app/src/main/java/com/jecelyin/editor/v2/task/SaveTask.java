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

package com.jecelyin.editor.v2.task;

import android.content.Context;

import com.duy.ccppcompiler.R;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Pref;
import com.jecelyin.editor.v2.common.SaveListener;
import com.jecelyin.editor.v2.io.LocalFileWriterTask;
import com.jecelyin.editor.v2.ui.editor.Document;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;

import java.io.File;
import java.lang.ref.WeakReference;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class SaveTask {
    private final WeakReference<Context> contextWR;
    private final WeakReference<EditorDelegate> editorDelegateWR;
    private final WeakReference<Document> documentWR;
    private boolean writing = false;
    private boolean isCluster;

    public SaveTask(Context context, EditorDelegate editorDelegate, Document document) {
        this.contextWR = new WeakReference<>(context);
        this.editorDelegateWR = new WeakReference<>(editorDelegate);
        this.documentWR = new WeakReference<>(document);
    }

    public boolean isWriting() {
        return writing;
    }

    public void save(boolean isCluster, SaveListener listener) {
        if (writing)
            return;

        Document document = documentWR.get();
        EditorDelegate editorDelegate = editorDelegateWR.get();
        if (document == null || editorDelegate == null)
            return;
        if (!document.isChanged()) {
            return;
        }
        this.isCluster = isCluster;
        File file = document.getFile();
        if (file == null) {
            editorDelegate.startSaveFileSelectorActivity();
            return;
        }
        if (document.isRoot()) {
            saveTo(document.getRootFile(), file, document.getEncoding(), listener);
        } else {
            saveTo(file, null, document.getEncoding(), listener);
        }

    }

    public void saveTo(final File file, final String encoding) {
        saveTo(file, null, encoding, null);
    }

    private void saveTo(final File rootFile, final File orgiFile, final String encoding, final SaveListener listener) {
        if (editorDelegateWR.get() == null || contextWR.get() == null)
            return;
        writing = true;
        LocalFileWriterTask fileWriter = new LocalFileWriterTask(rootFile, orgiFile, encoding, Pref.getInstance(contextWR.get()).isKeepBackupFile());
        fileWriter.setFileWriteListener(new LocalFileWriterTask.FileWriteListener() {
            @Override
            public void onSuccess() {
                writing = false;

                if (documentWR.get() == null || contextWR.get() == null || editorDelegateWR.get() == null) {
                    return;
                }
                documentWR.get().onSaveSuccess(orgiFile != null ? orgiFile : rootFile, encoding);
                if (!isCluster) {
                    UIUtils.toast(contextWR.get(), R.string.save_success);
                } else {
                    editorDelegateWR.get().getMainActivity().doNextCommand();
                }
                if (listener != null) {
                    listener.onSaved();
                }
            }

            @Override
            public void onError(Exception e) {
                writing = false;
                DLog.e(e);
                if (contextWR.get() != null) {
                    UIUtils.alert(contextWR.get(), e.getMessage());
                }
            }
        });
        fileWriter.write(editorDelegateWR.get().getEditableText());
    }
}
