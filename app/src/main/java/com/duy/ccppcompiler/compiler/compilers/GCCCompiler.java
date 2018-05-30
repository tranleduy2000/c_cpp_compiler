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
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.ICompileSetting;
import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.CompileResult;
import com.duy.ccppcompiler.compiler.shell.NativeActivityCompileResult;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.duy.common.DLog;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class GCCCompiler extends CompilerImpl {
    private static final String TAG = "GCCCompiler";
    private static final String GCC_COMPILER_NAME = "gcc";

    File mOutFile;
    ICompileSetting mSetting;
    boolean mBuildNativeActivity;

    public GCCCompiler(Context context, boolean nativeActivity, @Nullable ICompileSetting setting) {
        super(context);
        mBuildNativeActivity = nativeActivity;
        mSetting = setting;
    }

    @Override
    public CompileResult compile(File[] sourceFiles) {
        CommandResult shellResult = super.compile(sourceFiles);

        CompileResult result;
        if (mBuildNativeActivity) {
            result = new NativeActivityCompileResult(shellResult);
        } else {
            result = new CompileResult(shellResult);
        }

        if (result.getResultCode() == RESULT_NO_ERROR) {
            result.setBinaryFile(mOutFile);
        }
        return result;
    }

    @Override
    protected String getCompilerProgram() {
        return GCC_COMPILER_NAME;
    }

    @Override
    protected String buildArgs(File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));

        CommandBuilder args = new CommandBuilder();
        for (File sourceFile : sourceFiles) {
            args.addFlags(sourceFile.getAbsolutePath());
        }

        if (mBuildNativeActivity) {
            if (DLog.DEBUG) DLog.d(TAG, "buildArgs: build native activity");
            String soName = "lib" + nameNoExt + ".so";
            mOutFile = new File(source.getParent(), soName);

            String cctools = Environment.getCCtoolsDir(mContext);

            args.addFlags("-I" + cctools + "/sources/native_app_glue/")
                    .addFlags(cctools + "/sources/native_app_glue/android_native_app_glue.c")
                    .addFlags("-Wl,-soname," + soName)
                    .addFlags("-shared")
                    .addFlags("-Wl,--no-undefined")
                    .addFlags("-Wl,-z,noexecstack")
                    .addFlags("-llog") //lib log
                    .addFlags("-landroid") //android
                    .addFlags("-lm")
                    .addFlags("-o", mOutFile.getAbsolutePath())
            ;
        } else {
            mOutFile = new File(source.getParent(), nameNoExt);
            args.addFlags("-o", mOutFile.getAbsolutePath());
        }

        if (mSetting != null) {
            args.addFlags(mSetting.getCFlags());
            args.addFlags(mSetting.getLinkerFlags());
        }

        return args.build();
    }

}
