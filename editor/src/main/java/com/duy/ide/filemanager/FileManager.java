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

package com.duy.ide.filemanager;

import android.content.Context;

import com.duy.ide.editor.utils.IOUtils;
import com.jecelyin.common.utils.SysUtils;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class FileManager {
    private Context context;

    public FileManager(Context context) {
        this.context = context;
    }

    public File createNewFile(String fileName) {
        File file = new File(getApplicationDir(), fileName);
        if (IOUtils.createNewFile(file)) {
            return file;
        }
        return null;
    }

    public File getApplicationDir() {
        String path = SysUtils.getAppStoragePath(context);
        return new File(path);
    }
}
