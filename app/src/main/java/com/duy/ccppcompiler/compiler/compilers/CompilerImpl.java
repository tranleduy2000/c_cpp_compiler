/*
 *  Copyright (C)  2018  Duy Tran Le
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ccppcompiler.compiler.compilers;

import android.content.Context;
import android.support.annotation.NonNull;

import com.duy.ccppcompiler.compiler.model.CompileResult;
import com.duy.ccppcompiler.compiler.model.OutputScope;
import com.duy.ccppcompiler.compiler.shell.ArgumentBuilder;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.Shell;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.duy.common.DLog;
import com.pdaxrom.utils.Utils;

import java.io.File;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class CompilerImpl implements ICompiler {
    public static final int BUILD_EXECUTABLE = 0b0001;
    public static final int BUILD_NATIVE_ACTIVITY = 0b0010;
    public static final int BUILD_SDL_ACTIVITY = 0b0100;
    public static final int BUILD_MAKE = 0b1000;

    private static final int RESULT_NO_ERROR = 0;
    private static final String TAG = "CompilerImpl";
    private Context mContext;
    private int mBuildType;
    private OutputScope output;

    CompilerImpl(Context context, int buildType) {
        mContext = context;
        mBuildType = buildType;
    }

    @NonNull
    protected CommandResult exec(@NonNull Context context, @NonNull String workingDir, @NonNull String cmd) {
        return Shell.exec(context, workingDir, cmd);
    }

    @Override
    public CommandResult compile(File[] sourceFiles) {
        clean();

        //current version support single file
        File sourceFile = sourceFiles[0];

        //gcc or g++
        ArgumentBuilder command = new ArgumentBuilder(getCompilerProgram());
        String args = buildArgs(sourceFiles);
        command.addFlags(args);

        CompileResult result = new CompileResult(exec(mContext, sourceFile.getParent(), command.build()));
        if (result.getResultCode() == RESULT_NO_ERROR) {
            result.setOutput(getOutput());
            result.setType(mBuildType);
        } else {
            result.setType(mBuildType);
            result.setOutput(null);
        }
        return result;
    }

    protected void clean() {
        Utils.emptyDirectory(new File(Environment.getTmpExeDir(mContext)));
        Utils.emptyDirectory(new File(Environment.getSdCardTmpDir()));
    }

    protected String buildArgs(File[] sourceFiles) {
        ArgumentBuilder args = new ArgumentBuilder();
        for (File sourceFile : sourceFiles) {
            args.addFlags(sourceFile.getAbsolutePath());
        }

        switch (mBuildType) {
            case BUILD_EXECUTABLE:
                buildExecutableFlags(args, sourceFiles);
                break;
            case BUILD_NATIVE_ACTIVITY:
                buildNativeActivityFlags(args, sourceFiles);
                break;
            case BUILD_SDL_ACTIVITY:
                break;
            case BUILD_MAKE:
                break;
        }
        addUserSettingFlags(args);
        addDefaultLdFlags(args);
        return args.build();
    }

    protected void addUserSettingFlags(ArgumentBuilder args) {

    }

    protected abstract String getCompilerProgram();

    private void addDefaultLdFlags(ArgumentBuilder args) {
        //lib math and lib log
        args.addFlags("-lm", "-llog");
    }

    private void buildExecutableFlags(ArgumentBuilder args, File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));
        OutputScope outputScope = new OutputScope(new File(source.getParent(), nameNoExt));
        setOutput(outputScope);
        args.addFlags("-o", outputScope.getBinaryFile().getAbsolutePath());
    }

    private void buildNativeActivityFlags(ArgumentBuilder args, File[] sourceFiles) {
        File source = sourceFiles[0];
        String nameNoExt = source.getName().substring(0, source.getName().lastIndexOf("."));

        if (DLog.DEBUG) DLog.d(TAG, "buildArgs: build native activity");
        String soName = "lib" + nameNoExt + ".so";
        OutputScope outputScope = new OutputScope(new File(source.getParent(), soName));
        setOutput(outputScope);

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
                .addFlags("-o", outputScope.getBinaryFile().getAbsolutePath());
    }

    public OutputScope getOutput() {
        return output;
    }

    public void setOutput(OutputScope output) {
        this.output = output;
    }
}
