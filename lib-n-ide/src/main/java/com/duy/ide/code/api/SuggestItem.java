package com.duy.ide.code.api;

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
    String getName();

    /**
     * Display description
     */
    String getDescription();

    /**
     * Display type
     */
    String getReturnType();

    /**
     * @return the text will be insert then user click suggestion item
     */
    String getInsertText();

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
    void onSelectThis(IEditAreaView editAreaView);
}