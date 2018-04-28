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

package com.duy.ccppcompiler.compiler.diagnostic;

import android.net.Uri;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.Writer;

/**
 * Created by Duy on 28-Apr-18.
 */

public class SimpleFileObject implements FileObject {
    private File file;

    public SimpleFileObject(File file) {
        this.file = file;
    }

    @Override
    public Uri toUri() {
        return Uri.fromFile(file);
    }

    @Override
    public String getName() {
        return file.getName();
    }

    @Override
    public InputStream openInputStream() throws IOException {
        return new FileInputStream(file);
    }

    @Override
    public OutputStream openOutputStream() throws IOException {
        return new FileOutputStream(file);
    }

    @Override
    public Reader openReader(boolean ignoreEncodingErrors) throws IOException {
        return new FileReader(file);
    }

    @Override
    public CharSequence getCharContent(boolean ignoreEncodingErrors) throws IOException {
        return null;
    }

    @Override
    public Writer openWriter() throws IOException {
        return new FileWriter(file);
    }

    @Override
    public long getLastModified() {
        return file.lastModified();
    }

    @Override
    public boolean delete() {
        return file.delete();
    }
}
