package com.duy.ide.code.api;

import android.support.annotation.Nullable;

import com.duy.ide.editor.IEditorDelegate;

import java.io.File;

public interface CodeFormatProvider {
    @Nullable
    CodeFormatter getFormatterForFile(File file, IEditorDelegate delegate);
}
