package com.duy.ide.code.format;

import android.content.Context;
import android.support.annotation.Nullable;

import com.duy.ide.code.api.CodeFormatProvider;
import com.duy.ide.code.api.CodeFormatter;
import com.duy.ide.editor.IEditorDelegate;

import java.io.File;

public class CodeFormatProviderImpl implements CodeFormatProvider {

    private Context context;

    public CodeFormatProviderImpl(Context context) {

        this.context = context;
    }

    @Nullable
    @Override
    public CodeFormatter getFormatterForFile(File file, IEditorDelegate delegate) {
        return null;
    }

    public Context getContext() {
        return context;
    }
}
