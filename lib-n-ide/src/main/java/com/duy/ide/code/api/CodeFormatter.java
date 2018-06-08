package com.duy.ide.code.api;

import android.support.annotation.Nullable;

public interface CodeFormatter {

    /**
     * Format source code from given input
     *
     * @return null if can not format
     */
    @Nullable
    CharSequence format(CharSequence input);
}
