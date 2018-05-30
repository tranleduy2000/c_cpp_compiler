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
import java.util.ArrayList;

/**
 * Created by Duy on 25-Apr-18.
 */

public class GCCCompiler extends CompilerImpl {
    private static final String TAG = "GCCCompiler";
    private static final String GCC_COMPILER_NAME = "gcc";
    protected final ICompileSetting mSetting;

    protected final boolean mBuildNativeActivity;
    protected final boolean mBuildSDL;

    File mOutFile;

    public GCCCompiler(Context context, boolean nativeActivity, boolean buildSDL, @Nullable ICompileSetting setting) {
        super(context);
        mBuildNativeActivity = nativeActivity;
        mBuildSDL = buildSDL;
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
            args.addFlags(mSetting.getCFlags());
            args.addFlags(mSetting.getLinkerFlags());
        }

        return args.build();
    }

    private ArrayList<String> buildSDLActivityFlags(File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));

        if (DLog.DEBUG) DLog.d(TAG, "buildArgs: build native activity");
        String soName = "lib" + nameNoExt + ".so";
        mOutFile = new File(source.getParent(), soName);
        String cctools = Environment.getCCtoolsDir(mContext);
        CommandBuilder flags = new CommandBuilder();

        //default flags
        flags.addFlags("-DANDROID")
                .addFlags("-I" + new File(Environment.getSdCardHomeDir(), "SDL/include").getAbsolutePath())
                //ld flags
                .addFlags("-shared")
                .addFlags(new File(Environment.getSdCardHomeDir(), "SDL/lib/SDL_android_main.o").getAbsolutePath())
                .addFlags("-I" + new File(Environment.getSdCardHomeDir(), "SDL/lib").getAbsolutePath())
                .addFlags("-lSDL2")
                .addFlags("-llog")
                .addFlags("-lm")
                .addFlags("-o", mOutFile.getAbsolutePath());

        return flags.toList();
    }

    protected ArrayList<String> buildExecutableFlags(File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));
        mOutFile = new File(source.getParent(), nameNoExt);

        CommandBuilder args = new CommandBuilder();
        args.addFlags("-o", mOutFile.getAbsolutePath());
        return args.toList();
    }

    protected ArrayList<String> buildNativeActivityFlags(File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));

        if (DLog.DEBUG) DLog.d(TAG, "buildArgs: build native activity");
        String soName = "lib" + nameNoExt + ".so";
        mOutFile = new File(source.getParent(), soName);
        String cctools = Environment.getCCtoolsDir(mContext);
        CommandBuilder args = new CommandBuilder();
        args.addFlags("-I" + cctools + "/sources/native_app_glue/")
                .addFlags(cctools + "/sources/native_app_glue/android_native_app_glue.c")
                .addFlags("-Wl,-soname," + soName)
                .addFlags("-shared")
                .addFlags("-Wl,--no-undefined")
                .addFlags("-Wl,-z,noexecstack")
                .addFlags("-llog") //lib log
                .addFlags("-landroid") //android
                .addFlags("-lm")
                .addFlags("-o", mOutFile.getAbsolutePath());

        return args.toList();
    }


}
