/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    private Editor mContent;

    public GenerateSuggestDataTask(@NonNull SuggestionEditor editor,
                                   @NonNull SuggestionProvider provider) {
        this.mEditor = editor;
        this.mSuggestionProvider = provider;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        String content = mEditor.getText().toString();
        int selectionStart = mEditor.getSelectionStart();
        mContent = new Editor(content, selectionStart);
    }

    @Override
    protected ArrayList<SuggestItem> doInBackground(Void... params) {
        try {
            return mSuggestionProvider.getSuggestions(mContent);
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
