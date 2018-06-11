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

package com.duy.ide.file.model;

import android.content.res.AssetManager;
import android.net.Uri;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.Reader;
import java.io.Writer;

/**
 * Created by Duy on 30-Apr-18.
 */

public class AssetFile implements IFileObject {
    private final AssetManager assetManager;
    private final String path;

    public AssetFile(AssetManager assetManager, String path) {
        this.assetManager = assetManager;
        this.path = path;
    }

    @Override
    public Uri toUri() {
        return null;
    }

    @Override
    public InputStream openInputStream() throws IOException {
        return assetManager.open(path);
    }

    @Override
    public OutputStream openOutputStream() {
        throw new UnsupportedOperationException();
    }

    @Override
    public Writer openWriter() {
        throw new UnsupportedOperationException();
    }

    @Override
    public Reader openReader() throws IOException {
        return new InputStreamReader(openInputStream());
    }
}
