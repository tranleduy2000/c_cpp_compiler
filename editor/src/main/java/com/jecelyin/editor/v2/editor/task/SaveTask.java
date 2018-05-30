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

package com.jecelyin.editor.v2.editor.task;

import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ide.file.SaveListener;
import com.jecelyin.editor.v2.editor.Document;

import java.io.File;

public class SaveTask extends AsyncTask<Void, Void, Boolean> {
    private File mFile;
    private String mEncoding;
    private Document mDocument;
    @Nullable
    private SaveListener mListener;
    private Exception mException;

    public SaveTask(File file, String encoding, @NonNull Document document, @Nullable SaveListener listener) {
        mFile = file;
        mEncoding = encoding;
        mDocument = document;
        mListener = listener;
    }

    @Override
    protected Boolean doInBackground(Void... voids) {
        try {
            mDocument.writeToFile(mFile, mEncoding);
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            mException = e;
            return false;
        }
    }

    @Override
    protected void onPostExecute(Boolean aBoolean) {
        super.onPostExecute(aBoolean);
        if (aBoolean) {
            if (mListener != null) {
                mListener.onSavedSuccess();
            }
        } else {
            if (mListener != null) {
                mListener.onSaveFailed(mException);
            }
        }
    }
}
