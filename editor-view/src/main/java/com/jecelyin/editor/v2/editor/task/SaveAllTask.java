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

package com.jecelyin.editor.v2.editor.task;

import android.os.AsyncTask;

import com.duy.ide.editor.SimpleEditorActivity;
import com.duy.ide.editor.pager.EditorFragmentPagerAdapter;
import com.duy.ide.filemanager.SaveListener;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.editor.v2.editor.EditorDelegate;

/**
 * Created by Duy on 30-Apr-18.
 */

public class SaveAllTask extends AsyncTask<Void, Void, Void> {
    private static final String TAG = "SaveAllTask";
    private SimpleEditorActivity editorActivity;
    private SaveListener saveListener;

    public SaveAllTask(SimpleEditorActivity editorActivity, SaveListener saveListener) {
        this.editorActivity = editorActivity;
        this.saveListener = saveListener;
    }

    @Override
    protected Void doInBackground(Void... voids) {
        if (DLog.DEBUG) DLog.d(TAG, "doInBackground() called with: voids = [" + voids + "]");
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
