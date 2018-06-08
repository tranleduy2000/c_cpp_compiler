package com.duy.ccppcompiler.ide.code;

import android.support.annotation.Nullable;

import com.duy.astyle.AStyleInterface;
import com.duy.ide.code.api.CodeFormatter;

public class AStyleFormatter implements CodeFormatter {
    private String options;

    AStyleFormatter(String options) {
        this.options = options;
    }

    @Nullable
    @Override
    public CharSequence format(CharSequence input) {
        try {
            AStyleInterface aStyleInterface = new AStyleInterface();
            return aStyleInterface.formatSource(input.toString(), options);
        } catch (Exception e) {
            return null;
        }
    }
}
