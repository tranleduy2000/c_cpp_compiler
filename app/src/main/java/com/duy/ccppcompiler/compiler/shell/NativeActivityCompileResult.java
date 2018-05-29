package com.duy.ccppcompiler.compiler.shell;

/**
 * Compile result when build native activity
 */
public class NativeActivityCompileResult extends CompileResult {

    public NativeActivityCompileResult(CommandResult commandResult) {
        super(commandResult);
    }

    @Override
    public String toString() {
        return "NativeActivityCompileResult{" +
                "message='" + message + '\'' +
                '}';
    }
}
