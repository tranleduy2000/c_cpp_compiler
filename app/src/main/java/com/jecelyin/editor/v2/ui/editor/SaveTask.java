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

package com.jecelyin.editor.v2.ui.editor;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ide.filemanager.SaveListener;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.io.LocalFileWriterTask;

import java.io.File;
import java.lang.ref.WeakReference;
import java.util.concurrent.atomic.AtomicBoolean;

class SaveTask {
    private static final String TAG = "SaveTask";

    private final WeakReference<Context> mContext;
    private final WeakReference<EditorDelegate> mEditorDelegate;
    private final WeakReference<Document> mDocument;
    private final AtomicBoolean mWriting = new AtomicBoolean(false);

    SaveTask(Context context, EditorDelegate editorDelegate, Document document) {
        mContext = new WeakReference<>(context);
        mEditorDelegate = new WeakReference<>(editorDelegate);
        mDocument = new WeakReference<>(document);
    }

    public boolean isWriting() {
        return mWriting.get();
    }

    /**
     * @param inBackground - use background thread
     */
    void save(boolean inBackground, SaveListener listener) {
        if (isWriting()) {
            return;
        }

        Document document = mDocument.get();
        EditorDelegate delegate = mEditorDelegate.get();
        if (document == null || delegate == null) {
            return;
        }
        if (!document.isChanged()) {
            if (listener != null) {
                listener.onSaved();
            }
        } else {
            saveTo(document.getFile(), document.getEncoding(), inBackground, listener);
        }
    }

    /**
     * @param file       - file to write
     * @param encoding   - file encoding
     * @param background - <code>true</code> if using background thread
     */
    void saveTo(@NonNull final File file, final String encoding, boolean background,
                @Nullable final SaveListener listener) {
        if (DLog.DEBUG) {
            DLog.d(TAG, "saveTo() called with: file = [" + file + "], encoding = [" + encoding + "], background = [" + background + "], listener = [" + listener + "]");
        }
        if (mEditorDelegate.get() == null || mContext.get() == null) {
            return;
        }

        mWriting.set(true);
        LocalFileWriterTask writer = new LocalFileWriterTask(file, encoding);
        writer.setFileWriteListener(new LocalFileWriterTask.FileWriteListener() {
            @Override
            public void onSuccess() {
                mWriting.set(false);
                if (mDocument.get() == null || mContext.get() == null || mEditorDelegate.get() == null) {
                    return;
                }

                mDocument.get().onSaveSuccess(file, encoding);
                if (listener != null) {
                    listener.onSaved();
                }
            }

            @Override
            public void onError(Exception e) {
                mWriting.set(false);
                DLog.e(e);
                if (mContext.get() != null) {
                    UIUtils.alert(mContext.get(), e.getMessage());
                }
            }
        });
        if (background) {
            writer.execute(mEditorDelegate.get().getEditableText());
        } else {
            writer.onPostExecute(writer.writeToFile(mEditorDelegate.get().getEditableText()));
        }
    }
}
