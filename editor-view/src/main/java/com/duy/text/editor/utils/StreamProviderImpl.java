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

package com.duy.text.editor.utils;

import com.jecelyin.editor.v2.TextEditorApplication;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by Duy on 15-Apr-18.
 */

class StreamProviderImpl implements IStreamProvider {
    @Override
    public InputStream getFileInputStream(String name) throws IOException {
        return new FileInputStream(name);
    }

    @Override
    public OutputStream getOutputStream(String name) throws IOException {
        return new FileOutputStream(name);
    }

    @Override
    public InputStream getAssetInputStream(String name) throws IOException {
        return TextEditorApplication.getContext().getAssets().open(name);
    }
}
