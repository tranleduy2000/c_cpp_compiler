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

package com.duy.ccppcompiler.compiler.compilers;

import android.content.Context;

import java.io.File;

/**
 * Created by Duy on 18-May-18.
 */

public class MakeCompiler extends CompilerImpl {

    private static final String MARK_PROGRAM = "make";
    private static final String TAG = "MakeCompiler";

    MakeCompiler(Context context) {
        super(context);
    }

    @Override
    protected String buildArgs(File[] sourceFiles) {
        return "-f " + sourceFiles[0].getAbsolutePath();
    }

    @Override
    protected String getCompilerProgram() {
        return MARK_PROGRAM;
    }
}
