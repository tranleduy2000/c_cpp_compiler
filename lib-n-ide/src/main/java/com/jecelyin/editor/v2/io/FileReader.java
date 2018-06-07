/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.io;

import android.text.SpannableStringBuilder;
import android.text.TextUtils;

import com.duy.ide.editor.util.GrowingArrayUtils;
import com.jecelyin.common.utils.DLog;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class FileReader {
    private final static int BUFFER_SIZE = 16384;
    private SpannableStringBuilder stringBuilder = null;
    private File file;
    private String encoding;
    private int lineCount;

    public FileReader(File file, String encodingName) {
        this.file = file;
        this.encoding = encodingName;
    }

    public boolean read() {
        try {
            if (TextUtils.isEmpty(encoding)) {
                encoding = FileEncodingDetector.detectEncoding(file);
            }

            DLog.d(file.getPath() + " encoding is " + encoding);
            LineNumberReader reader = new LineNumberReader(new InputStreamReader(new FileInputStream(file), encoding));
            char[] buf = new char[BUFFER_SIZE];
            int len;
            CharArrayBuffer arrayBuffer = new CharArrayBuffer(GrowingArrayUtils.growSize((int) file.length()));
            while ((len = reader.read(buf, 0, BUFFER_SIZE)) != -1) {
                arrayBuffer.append(buf, 0, len);
            }

            lineCount = reader.getLineNumber() + 1;
            reader.close();
            stringBuilder = new SpannableStringBuilder(new String(arrayBuffer.buffer(), 0, arrayBuffer.length()));

            return true;
        } catch (Exception e) {
            DLog.e(e);
            return false;
        } catch (OutOfMemoryError error) {
            return false;
        }
    }

    public String getEncoding() {
        return encoding;
    }

    public int getLineCount() {
        return lineCount;
    }

    public SpannableStringBuilder getBuffer() {
        return stringBuilder;
    }

}
