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

package com.jecelyin.editor.v2.io;

import android.os.AsyncTask;
import android.support.annotation.MainThread;
import android.text.GetChars;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class LocalFileWriterTask extends AsyncTask<GetChars, Void, Exception> {
    private final static int BUFFER_SIZE = 16 * 1024;
    private final String encoding;
    private final File file;
    private FileWriteListener fileWriteListener;

    public LocalFileWriterTask(File file, String encoding) {
        this.file = file;
        this.encoding = encoding;
    }

    public void write(GetChars text) {
        execute(text);
    }

    public void setFileWriteListener(FileWriteListener fileWriteListener) {
        this.fileWriteListener = fileWriteListener;
    }

    @Override
    protected Exception doInBackground(GetChars... params) {
        GetChars text = params[0];
        try {
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file), encoding), BUFFER_SIZE);
            char[] buffer = new char[BUFFER_SIZE]; //16kb
            int size = text.length();
            if (size > 0) {
                int start = 0, end = BUFFER_SIZE;
                for (; ; ) {
                    end = Math.min(end, size);
                    text.getChars(start, end, buffer, 0);

                    bw.write(buffer, 0, end - start);
                    start = end;

                    if (end >= size)
                        break;

                    end += BUFFER_SIZE;
                }
            }

            bw.close();
        } catch (Exception e) {
            return e;
        }
        return null;
    }

    @Override
    protected void onPostExecute(Exception e) {
        if (fileWriteListener == null)
            return;
        if (e == null)
            fileWriteListener.onSuccess();
        else
            fileWriteListener.onError(e);
    }

    public interface FileWriteListener {
        @MainThread
        void onSuccess();

        @MainThread
        void onError(Exception e);
    }

}
