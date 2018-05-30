/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.file.model;

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

    @NonNull
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
