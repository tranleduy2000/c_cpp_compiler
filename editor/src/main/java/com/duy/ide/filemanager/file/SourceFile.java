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

package com.duy.ide.filemanager.file;

import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;
import java.io.Writer;

/**
 * Created by Duy on 30-Apr-18.
 */

public class SourceFile implements ISourceFile {
    private final File file;
    private final byte[] md5;

    public SourceFile(File file, byte[] md5) {
        this.file = file;
        this.md5 = md5;
    }

    @NonNull
    @Override
    public String getPath() {
        return file.getPath();
    }

    @NonNull
    @Override
    public File getFile() {
        return file;
    }

    @Nullable
    @Override
    public Uri toUri() {
        return Uri.fromFile(file);
    }

    @Override
    public byte[] md5() {
        return md5;
    }

    @Override
    public InputStream openInputStream() throws IOException {
        return new FileInputStream(file);
    }

    @Override
    public FileOutputStream openOutputStream() throws FileNotFoundException {
        return new FileOutputStream(file);
    }

    @Override
    public Writer openWriter() throws IOException {
        return new FileWriter(file);
    }

    @Override
    public Reader openReader() throws IOException {
        return new FileReader(file);
    }
}
