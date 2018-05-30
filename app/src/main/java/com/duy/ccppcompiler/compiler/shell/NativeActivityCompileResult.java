package com.duy.ccppcompiler.compiler.shell;

import android.support.annotation.Nullable;

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
