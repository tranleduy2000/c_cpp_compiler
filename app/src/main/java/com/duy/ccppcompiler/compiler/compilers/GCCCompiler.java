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
import com.pdaxrom.packagemanager.EnvironmentPath;
import com.pdaxrom.utils.Utils;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

/**
 * Created by Duy on 25-Apr-18.
 */

public class GCCCompiler extends NativeCompilerImpl<GccCommandResult> {
    private static final String TAG = "GCCCompiler";
    protected File mOutFile;
    protected Context mContext;
    protected ICompileSetting mSetting;

    public GCCCompiler(Context context, ICompileSetting compileSetting) {
        mContext = context;
        mSetting = compileSetting;
    }

    @Override
    public GccCommandResult compile(File[] sourceFiles) {
        File fileToBeCompiled = sourceFiles[0];
        String command = buildCommand(sourceFiles);
        String mWorkDir = fileToBeCompiled.getParent();

        GccCommandResult result = new GccCommandResult(execCommand(mContext, mWorkDir, command));
        if (result.getResultCode() == 0) {
            if (mOutFile.exists()) {
                try {
                    File internalBinary = new File(EnvironmentPath.getTmpExeDir(mContext), mOutFile.getName());
                    FileInputStream inputStream = new FileInputStream(mOutFile);
                    FileOutputStream outputStream = new FileOutputStream(internalBinary);
                    IOUtils.copy(inputStream, outputStream);
                    inputStream.close();
                    outputStream.close();

                    Utils.chmod(internalBinary.getAbsolutePath(), 0x1fd/*0775*/);

                    result.setBinaryFile(internalBinary);
                } catch (Exception e) {
                    e.printStackTrace();
                    result.setResultCode(-1);
                    result.setMessage("Application error: " + e.getMessage() + ". Please report bug on GitHub of project");
                }
            }
        }
        return result;
    }

    protected String buildCommand(File[] sourceFiles) {
        File file = sourceFiles[0];
        String fileName = file.getName();
        mOutFile = new File(file.getParent(), fileName.substring(0, fileName.lastIndexOf(".")));

        CommandBuilder builder = new CommandBuilder("gcc-4.9");
        for (File sourceFile : sourceFiles) {
            builder.addFlags(sourceFile.getAbsolutePath());
        }
        builder.addFlags(mSetting.getCFlags());
        builder.addFlags("-o", mOutFile.getAbsolutePath());

        if (Build.VERSION.SDK_INT >= 21) {
            builder.addFlags("-pie");
        }

        // builder.addFlags("-std=c99");
        // builder.addFlags("-lz"/*zlib*/, "-ldl", "-lm" /*math*/, "-llog");
        // builder.addFlags("-lncurses");
        // builder.addFlags("-Og");

        // Emit fix-it hints in a machine-parseable format, suitable for consumption by IDEs.
        // For each fix-it, a line will be printed after the relevant diagnostic, starting with the
        // string “fix-it:”. For example:
        // fix-it:"test.c":{45:3-45:21}:"gtk_widget_show_all"
        // builder.add("-fdiagnostics-parseable-fixits");

        // By default, each diagnostic emitted includes text indicating the command-line option that
        // directly controls the diagnostic (if such an option is known to the diagnostic machinery).
        // Specifying the -fno-diagnostics-show-option flag suppresses that behavior.
        builder.addFlags("-fno-diagnostics-show-option");

        // By default, each diagnostic emitted includes the original source line and a caret ‘^’
        // indicating the column. This option suppresses this information. The source line is
        // truncated to n characters, if the -fmessage-length=n option is given. When the output is
        // done to the terminal, the width is limited to the width given by the COLUMNS environment
        // variable or, if not set, to the terminal width.
        // builder.addFlags("-fno-diagnostics-show-caret");

        return builder.buildCommand();
    }
}
