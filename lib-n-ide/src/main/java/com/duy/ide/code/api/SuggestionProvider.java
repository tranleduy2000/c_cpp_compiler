package com.duy.ide.code.api;

import com.duy.ide.editor.internal.suggestion.Editor;

import java.util.ArrayList;

public interface SuggestionProvider {
    ArrayList<SuggestItem> getSuggestions(Editor editor);
}
