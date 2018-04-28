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

package com.jecelyin.editor.v2.ui.diagnostic;

import android.support.v7.widget.RecyclerView;

import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.manager.TabManager;

/**
 * Created by Duy on 28-Apr-18.
 */

public class FileDiagnosticPresenter implements DiagnosticContract.Presenter {
    private final RecyclerView recyclerView;
    private final EditorActivity activity;
    private final TabManager tabManager;

    public FileDiagnosticPresenter(RecyclerView recyclerView, EditorActivity activity, TabManager tabManager) {
        this.recyclerView = recyclerView;
        this.activity = activity;
        this.tabManager = tabManager;
    }

    @Override
    public void onDiagnosticClick(DiagnosticContract.View view, Diagnostic diagnostic) {

    }
}
