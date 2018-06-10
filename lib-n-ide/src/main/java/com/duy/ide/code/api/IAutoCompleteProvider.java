package com.duy.ide.code.api;

import android.widget.EditText;

import java.util.ArrayList;

public interface IAutoCompleteProvider {
    ArrayList<SuggestItem> getSuggestions(EditText editor);
}
