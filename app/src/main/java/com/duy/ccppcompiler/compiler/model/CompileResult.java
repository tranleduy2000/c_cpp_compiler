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

package com.duy.ccppcompiler.compiler.model;

import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.shell.CommandResult;

/**
 * Created by Duy on 18-May-18.
 */

public class CompileResult extends CommandResult {
    @Nullable
    private OutputScope output;
    private int type;

    public CompileResult(CommandResult commandResult) {
        super(commandResult);
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    @Nullable
    public OutputScope getOutput() {
        return output;
    }

    public void setOutput(@Nullable OutputScope output) {
        this.output = output;
    }

    @Override
    public String toString() {
        return "CompileResult{" +
                "binaryFile=" + output +
                "} " + super.toString();
    }
}
