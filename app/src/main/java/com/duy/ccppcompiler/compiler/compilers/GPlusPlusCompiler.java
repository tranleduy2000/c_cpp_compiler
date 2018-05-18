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
import android.os.Build;

import com.duy.ccppcompiler.compiler.ICompileSetting;
import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.GccCommandResult;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */
public class GPlusPlusCompiler extends GCCCompiler {

    public GPlusPlusCompiler(Context context, ICompileSetting compileSetting) {
        super(context, compileSetting);
    }

    @Override
    public GccCommandResult compile(File[] sourceFiles) {
        return super.compile(sourceFiles);
    }

    @Override
    protected String buildCommand(File[] sourceFiles) {
        File file = sourceFiles[0];
        String fileName = file.getName();
        mOutFile = new File(file.getParent(), fileName.substring(0, fileName.lastIndexOf(".")));

        CommandBuilder builder = new CommandBuilder("g++-4.9");
        for (File sourceFile : sourceFiles) {
            builder.addFlags(sourceFile.getAbsolutePath());
        }
        builder.addFlags("-o", mOutFile.getAbsolutePath());
        builder.addFlags(mSetting.getCxxFlags());

        if (Build.VERSION.SDK_INT >= 21) {
            builder.addFlags("-pie");
        }
        builder.addFlags("-std=c++14");
        builder.addFlags("-lz");
        builder.addFlags("-ldl");
        builder.addFlags("-lm");
        builder.addFlags("-llog");
        //builder.addFlags("-lncurses");
        builder.addFlags("-Og");

        // By default, each diagnostic emitted includes text indicating the command-line option that
        // directly controls the diagnostic (if such an option is known to the diagnostic machinery).
        // Specifying the -fno-diagnostics-show-option flag suppresses that behavior.
        builder.addFlags("-fno-diagnostics-show-option");

        return builder.buildCommand();
    }
}
