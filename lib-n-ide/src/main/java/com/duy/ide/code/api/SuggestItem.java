package com.duy.ide.code.api;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.duy.ide.editor.view.IEditAreaView;

/**
 * The item will be display in list suggestion
 * <p>
 * Type|name|description
 * <p>
 * Created by Duy on 20-Jul-17.
 */

public interface SuggestItem {
    /**
     * Display name
     */
    @Nullable
    String getName();

    /**
     * Display description
     */
    @Nullable
    String getDescription();

    /**
     * Display type
     */
    @Nullable
    String getReturnType();

    int getSuggestionPriority();

    /**
     * Return a single character to display type of suggestion
     * v: variable
     * f: function
     * m: method
     * etc
     */
    char getTypeHeader();

    /**
     * @param editAreaView - editor you will modify for this suggestion
     */
    void onSelectThis(@NonNull IEditAreaView editAreaView);
}