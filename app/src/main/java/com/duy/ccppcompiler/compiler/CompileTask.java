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

package com.duy.ccppcompiler.compiler;

import android.os.AsyncTask;
import android.support.annotation.NonNull;

import com.duy.ide.compiler.INativeCompiler;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileTask extends AsyncTask<Void, Void, Void> {
    private INativeCompiler mCompiler;
    private File[] mFiles;

    public CompileTask(INativeCompiler compiler, @NonNull File[] files) {
        mCompiler = compiler;
        mFiles = files;
    }

    @Override
    protected Void doInBackground(Void... voids) {
        return null;
    }
}
