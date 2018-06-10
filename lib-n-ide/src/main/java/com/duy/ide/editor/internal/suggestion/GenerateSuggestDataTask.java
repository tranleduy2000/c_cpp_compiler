package com.duy.ide.editor.internal.suggestion;

import android.os.AsyncTask;
import android.support.annotation.NonNull;

import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.code.api.SuggestionProvider;
import com.duy.ide.editor.view.SuggestionEditor;

import java.util.ArrayList;


public class GenerateSuggestDataTask extends AsyncTask<Void, Void, ArrayList<SuggestItem>> {
    private static final String TAG = "GenerateSuggestDataTask";
    private SuggestionEditor mEditor;
    private SuggestionProvider mSuggestionProvider;

    public GenerateSuggestDataTask(@NonNull SuggestionEditor editor,
                                   @NonNull SuggestionProvider provider) {
        this.mEditor = editor;
        this.mSuggestionProvider = provider;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
    }

    @Override
    protected ArrayList<SuggestItem> doInBackground(Void... params) {
        try {
            return mSuggestionProvider.getSuggestions(mEditor);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    protected void onPostExecute(ArrayList<SuggestItem> descriptions) {
        super.onPostExecute(descriptions);
        if (isCancelled()) {
            return;
        }
        if (descriptions == null) {
            mEditor.setSuggestData(new ArrayList<SuggestItem>());
        } else {
            mEditor.setSuggestData(descriptions);
        }
    }
}
