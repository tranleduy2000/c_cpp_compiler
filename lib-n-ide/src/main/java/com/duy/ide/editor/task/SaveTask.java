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

package com.duy.ide.editor.task;

import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ide.file.SaveListener;
import com.duy.ide.editor.Document;

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
