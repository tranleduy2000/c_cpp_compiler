package com.duy.ide.editor.internal.suggestion;

import android.support.annotation.NonNull;

import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.editor.view.IEditAreaView;

public interface OnSuggestItemClickListener {
    void onClickSuggest(@NonNull IEditAreaView editAreaView, @NonNull SuggestItem item);
}