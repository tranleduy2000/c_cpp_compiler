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
import android.support.annotation.Nullable;
import android.text.GetChars;

import com.jecelyin.common.utils.IOUtils;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class LocalFileWriterTask extends AsyncTask<GetChars, Void, Exception> {
    private final static int BUFFER_SIZE = 16 * 1024;
    private final String encoding;
    private final File file;
    private final File backupFile;
    private final File orgiFile;
    private final boolean keepBackupFile;
    private FileWriteListener fileWriteListener;

    public LocalFileWriterTask(File file, @Nullable File orgiFile, String encoding, boolean keepBackupFile) {
        this.file = file;
        this.orgiFile = orgiFile;
        this.backupFile = makeBackupFile(file);
        this.encoding = encoding;
        this.keepBackupFile = keepBackupFile;
    }

    private static File makeBackupFile(File file) {
        return new File(file.getParent(), ".920bak." + file.getName());
    }

    public void write(GetChars text) {
        execute(text);
    }

    public void setFileWriteListener(FileWriteListener fileWriteListener) {
        this.fileWriteListener = fileWriteListener;
    }

    @Override
    protected Exception doInBackground(GetChars... params) {
        if (file.isFile() && !IOUtils.copyFile(file, backupFile)) {
            return new IOException("Couldn't copy file " + file
                    + " to backup file " + backupFile);
        }

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

        if (orgiFile != null && !IOUtils.copyFile(file, orgiFile)) {
            return new IOException("Can't copy " + file.getPath() + " content to " + orgiFile.getPath());
        }
        if (file.exists()) {
            if (!keepBackupFile && backupFile.exists() && !backupFile.delete()) {
                return new IOException("Couldn't remove backup file " + backupFile);
            }
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

    public static interface FileWriteListener {
        public void onSuccess();

        public void onError(Exception e);
    }

}
