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
import android.support.annotation.Nullable;

import com.duy.ide.editor.SimpleEditorActivity;
import com.duy.ide.editor.pager.EditorFragmentPagerAdapter;
import com.duy.ide.filemanager.SaveListener;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.manager.TabManager;

/**
 * Created by Duy on 30-Apr-18.
 */

public class SaveAllTask extends AsyncTask<Void, Void, Boolean> {
    private static final String TAG = "SaveAllTask";
    private SimpleEditorActivity editorActivity;
    @Nullable
    private SaveListener saveListener;
    private Exception exception;

    public SaveAllTask(SimpleEditorActivity editorActivity, @Nullable SaveListener saveListener) {
        this.editorActivity = editorActivity;
        this.saveListener = saveListener;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
    }

    @Override
    protected Boolean doInBackground(Void... voids) {
        TabManager tabManager = editorActivity.getTabManager();
        EditorFragmentPagerAdapter editorPagerAdapter = tabManager.getEditorPagerAdapter();
        for (EditorDelegate editorDelegate : editorPagerAdapter.getAllEditor()) {
            try {
                editorDelegate.save();
            } catch (Exception e) {
                e.printStackTrace();
                exception = e;
            }
        }
        return exception == null;
    }

    @Override
    protected void onPostExecute(Boolean aBoolean) {
        super.onPostExecute(aBoolean);
        EditorFragmentPagerAdapter editorPagerAdapter = editorActivity.getTabManager().getEditorPagerAdapter();
        for (EditorDelegate editorDelegate : editorPagerAdapter.getAllEditor()) {
            editorDelegate.onDocumentChanged();
        }
        if (aBoolean) {
            if (saveListener != null) {
                saveListener.onSavedSuccess();
            }
        } else {
            if (saveListener != null) {
                saveListener.onSaveFailed(exception);
            }
        }
    }
}
