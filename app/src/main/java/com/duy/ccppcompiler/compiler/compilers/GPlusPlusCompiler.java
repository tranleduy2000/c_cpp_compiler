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
import com.duy.ccppcompiler.packagemanager.Environment;

import java.io.File;

/**
 * G++ compiler as GCC compiler but it different name, just override {@link #getCompilerProgram()}
 * Created by Duy on 25-Apr-18.
 */
public class GPlusPlusCompiler extends GCCCompiler {
    private static final String G_PLUS_PLUS_COMPILER_NAME = "g++-4.9";

    public GPlusPlusCompiler(Context context, boolean nativeActivity, ICompileSetting setting) {
        super(context, nativeActivity, setting);
    }

    @Override
    protected String getCompilerProgram() {
        return G_PLUS_PLUS_COMPILER_NAME;
    }

    @Override
    protected String buildArgs(File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));

        CommandBuilder args = new CommandBuilder();
        for (File sourceFile : sourceFiles) {
            args.addFlags(sourceFile.getAbsolutePath());
        }
        if (mSetting != null) {
            args.addFlags(mSetting.getCxxFlags());
        }

        if (mBuildNativeActivity) {
            mOutFile = new File(source.getParent(), "lib" + nameNoExt + ".so");
            String cCtoolsDir = Environment.getCCtoolsDir(mContext);
            args.addFlags(String.format("-I%s/sources/native_app_glue", cCtoolsDir))
                    .addFlags(String.format("%s/sources/native_app_glue/android_native_app_glue.c", cCtoolsDir))
                    .addFlags(String.format("-Wl,-soname,%s", mOutFile.getAbsolutePath()))
                    .addFlags("-shared")
                    .addFlags("-Wl,--no-undefined -Wl,-z,noexecstack")
                    .addFlags("-llog")
                    .addFlags("-landroid")
                    .addFlags("-lm")
                    .addFlags("-o", mOutFile.getAbsolutePath());
        } else {
            mOutFile = new File(source.getParent(), nameNoExt);
            args.addFlags("-o", mOutFile.getAbsolutePath());
        }

        if (Build.VERSION.SDK_INT >= 21) {
            args.addFlags("-pie");
        }

        return args.build();
    }
}
