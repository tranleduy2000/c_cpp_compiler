package com.duy.ccppcompiler.compiler.shell;

import android.support.annotation.Nullable;

/**
 * Commnad result when build native activity
 */
public class NativeActivityCompileResult extends CommandResult {

    public NativeActivityCompileResult(int resultCode) {
        super(resultCode);
    }

    public NativeActivityCompileResult(int resultCode, @Nullable String message) {
        super(resultCode, message);
    }

    public NativeActivityCompileResult(CommandResult commandResult) {
        super(commandResult);
    }
}
