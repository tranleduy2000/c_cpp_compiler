/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.task;

import android.os.AsyncTask;

import com.duy.ide.filemanager.SaveListener;
import com.jecelyin.editor.v2.adapter.EditorFragmentPagerAdapter;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;

/**
 * Created by Duy on 30-Apr-18.
 */

public class SaveAllTask extends AsyncTask<Void, Void, Void> {
    private EditorActivity editorActivity;
    private SaveListener saveListener;

    public SaveAllTask(EditorActivity editorActivity, SaveListener saveListener) {
        this.editorActivity = editorActivity;
        this.saveListener = saveListener;
    }

    @Override
    protected Void doInBackground(Void... voids) {
        EditorFragmentPagerAdapter editorPagerAdapter = editorActivity.getTabManager().getEditorPagerAdapter();
        for (EditorDelegate editorDelegate : editorPagerAdapter.getAllEditor()) {
            editorDelegate.save(false);
        }
        return null;
    }

    @Override
    protected void onPostExecute(Void aVoid) {
        super.onPostExecute(aVoid);
        if (saveListener != null) {
            saveListener.onSaved();
        }
    }
}
