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

package com.duy.ccppcompiler.compiler.result;

import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.shell.CommandResult;

import java.io.File;

/**
 * Compile result when build native activity
 */
public class NativeActivityCompileResult extends CompileResult {
    @Nullable
    private File binaryFile;

    public NativeActivityCompileResult(CommandResult commandResult) {
        super(commandResult);
    }

    @Nullable
    public File getBinaryFile() {
        return binaryFile;
    }

    public void setBinaryFile(@Nullable File binaryFile) {
        this.binaryFile = binaryFile;
    }

    @Override
    public String toString() {
        return "CompileResult{" +
                "binaryFile=" + binaryFile +
                "} " + super.toString();
    }
}
