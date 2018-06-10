package com.duy.ccppcompiler.ide.code;

import android.content.Context;
import android.support.annotation.Nullable;
import android.widget.Toast;

import com.duy.ccppcompiler.R;
import com.duy.ide.code.api.CodeFormatter;
import com.duy.ide.code.format.CodeFormatProviderImpl;
import com.duy.ide.editor.IEditorDelegate;

import org.gjt.sp.jedit.Catalog;

import java.io.File;

public class CppCodeFormatProvider extends CodeFormatProviderImpl {
    public CppCodeFormatProvider(Context context) {
        super(context);
    }

    @Nullable
    @Override
    public CodeFormatter getFormatterForFile(File file, IEditorDelegate delegate) {
        CodeFormatter formatter = super.getFormatterForFile(file, delegate);
        if (formatter != null) {
            return formatter;
        }

        String currMode = delegate.getDocument().getModeName();
        if (currMode == null) {
            return null;
        }
        String mode;
        String style = "gnu";
        if (currMode.contentEquals(Catalog.getModeByName("C++").getName())) {
            mode = "c";
        } else if (currMode.contentEquals(Catalog.getModeByName("C").getName())) {
            mode = "c";
        } else if (currMode.contentEquals(Catalog.getModeByName("Objective-C").getName())) {
            mode = "c";
        } else if (currMode.contentEquals(Catalog.getModeByName("C#").getName())) {
            mode = "cs";
        } else if (currMode.contentEquals(Catalog.getModeByName("Java").getName())) {
            mode = "java";
            style = "java";
        } else {
            Toast.makeText(getContext(), R.string.unsupported_format_source, Toast.LENGTH_SHORT).show();
            return null;
        }

        String options = "--mode=" + mode;
        if (!style.isEmpty()) {
            options += " --style=" + style;
        }
        return new AStyleFormatter(options);

    }
}
