package com.duy.ide.code.api;

import android.widget.EditText;

import java.util.ArrayList;

public interface SuggestionProvider {
    ArrayList<SuggestItem> getSuggestions(EditText editor);
}
