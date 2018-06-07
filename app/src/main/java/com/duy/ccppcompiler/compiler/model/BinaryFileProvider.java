package com.duy.ccppcompiler.compiler.model;

import android.support.annotation.NonNull;

import java.io.File;

interface BinaryFileProvider {
    @NonNull
    File getBinaryFile();
}
