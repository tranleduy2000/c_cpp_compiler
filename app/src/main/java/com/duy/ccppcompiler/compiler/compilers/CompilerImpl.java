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
import android.support.annotation.NonNull;

import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.Shell;

import java.io.File;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class CompilerImpl implements ICompiler {
    static final int RESULT_NO_ERROR = 0;

    Context mContext;

    CompilerImpl(Context context) {
        this.mContext = context;
    }

    @NonNull
    private CommandResult execCommand(@NonNull Context context, @NonNull String workingDir, @NonNull String cmd) {
        return Shell.exec(context, workingDir, cmd);
    }

    @Override
    public CommandResult compile(File[] sourceFiles) {
        clean();

        File sourceFile = sourceFiles[0];
        CommandBuilder command = new CommandBuilder(getCompilerProgram());
        String args = buildArgs(sourceFiles);
        command.addFlags(args);
        return execCommand(mContext, sourceFile.getParent(), command.build());
    }

    protected void clean() {

    }

    protected abstract String buildArgs(File[] sourceFiles);

    protected abstract String getCompilerProgram();

}
