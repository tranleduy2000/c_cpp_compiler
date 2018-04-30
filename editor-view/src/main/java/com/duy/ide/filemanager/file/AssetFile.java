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
