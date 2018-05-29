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
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.ICompileSetting;
import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.GccCompileResult;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.pdaxrom.utils.Utils;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

/**
 * Created by Duy on 25-Apr-18.
 */

public class GCCCompiler extends CompilerImpl {
    private static final String TAG = "GCCCompiler";
    private static final String GCC_COMPILER_NAME = "gcc-4.9";

    protected File mOutFile;
    protected ICompileSetting mSetting;

    private boolean mBuildNativeActivity;

    public GCCCompiler(Context context, boolean nativeActivity, @Nullable ICompileSetting setting) {
        super(context);
        mBuildNativeActivity = nativeActivity;
        mSetting = setting;
    }

    @Override
    public GccCompileResult compile(File[] sourceFiles) {
        GccCompileResult result = new GccCompileResult(super.compile(sourceFiles));
        if (result.getResultCode() == RESULT_NO_ERROR) {
            if (mOutFile != null && mOutFile.exists()) {
                try {
                    File internalBinary = new File(Environment.getTmpExeDir(mContext), mOutFile.getName());
                    FileInputStream inputStream = new FileInputStream(mOutFile);
                    FileOutputStream outputStream = new FileOutputStream(internalBinary);
                    IOUtils.copy(inputStream, outputStream);
                    inputStream.close();
                    outputStream.close();

                    //set executable
                    Utils.chmod(internalBinary.getAbsolutePath(), 0x1ed/*0775*/);

                    result.setBinaryFile(internalBinary);

                } catch (Exception e) {
                    e.printStackTrace();
                    result.setResultCode(-1);
                    result.setMessage("Application error: " + e.getMessage() +
                            ". Please report bug on GitHub of project");
                }
            }
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
        if (mSetting != null) {
            args.addFlags(mSetting.getCFlags());
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
