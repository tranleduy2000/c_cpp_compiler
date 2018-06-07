package com.duy.ccppcompiler.compiler.model;

import java.io.File;

public class OutputScope implements BinaryFileProvider{
    private File binaryFile;

    public OutputScope(File binaryFile) {
        this.binaryFile = binaryFile;
    }

    public File getBinaryFile() {
        return binaryFile;
    }
}
