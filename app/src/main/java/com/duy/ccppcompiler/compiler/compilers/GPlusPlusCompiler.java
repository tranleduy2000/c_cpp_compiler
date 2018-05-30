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

import com.duy.ccppcompiler.compiler.ICompileSetting;
import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.result.CompileResult;

import java.io.File;

/**
 * G++ compiler as GCC compiler but it different name, just override {@link #getCompilerProgram()}
 * Created by Duy on 25-Apr-18.
 */
public class GPlusPlusCompiler extends GCCCompiler {
    private static final String G_PLUS_PLUS_COMPILER_NAME = "g++-4.9";

    public GPlusPlusCompiler(Context context, boolean nativeActivity, boolean buildSDL, ICompileSetting setting) {
        super(context, nativeActivity, buildSDL, setting);
    }

    @Override
    protected String getCompilerProgram() {
        return G_PLUS_PLUS_COMPILER_NAME;
    }

    @Override
    public CompileResult compile(File[] sourceFiles) {
        return super.compile(sourceFiles);
    }

    @Override
    protected String buildArgs(File[] sourceFiles) {
        CommandBuilder args = new CommandBuilder();
        for (File sourceFile : sourceFiles) {
            args.addFlags(sourceFile.getAbsolutePath());
        }
        if (mBuildNativeActivity) {
            args.addFlags(buildNativeActivityFlags(sourceFiles));
        } else if (mBuildSDL) {
            args.addFlags(buildSDLActivityFlags(sourceFiles));
        } else {
            args.addFlags(buildExecutableFlags(sourceFiles));
        }

        if (mSetting != null) {
            args.addFlags(mSetting.getCxxFlags());
            args.addFlags(mSetting.getLinkerFlags());
        }

        return args.build();
    }
}
