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

import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.Shell;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.pdaxrom.utils.Utils;

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
    protected CommandResult execCommand(@NonNull Context context, @NonNull String workingDir, @NonNull String cmd) {
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
        Utils.deleteDirectory(new File(Environment.getTmpExeDir(mContext)));
        //create
        Environment.getTmpExeDir(mContext);

        Utils.deleteDirectory(new File(Environment.getSdCardTmpDir()));
        //create
        Environment.getSdCardTmpDir();
    }

    protected abstract String buildArgs(File[] sourceFiles);

    protected abstract String getCompilerProgram();

}
