package com.duy.ide.code.api;

import android.support.annotation.Nullable;

import java.io.File;

public interface CodeFormatProvider {
    @Nullable
    CodeFormatter getFormatterForFile(File file);
}
