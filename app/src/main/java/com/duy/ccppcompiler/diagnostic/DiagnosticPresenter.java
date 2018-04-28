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

package com.duy.ccppcompiler.diagnostic;

import android.support.annotation.MainThread;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.manager.TabManager;

import java.util.ArrayList;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticPresenter implements DiagnosticContract.Presenter {
    private final RecyclerView mRecyclerView;
    private final EditorActivity mActivity;
    private final TabManager mTabManager;
    private ArrayList<Diagnostic> diagnostics;
    private DiagnosticContract.View mView;

    public DiagnosticPresenter(RecyclerView recyclerView, EditorActivity activity, TabManager tabManager) {
        this.mRecyclerView = recyclerView;
        this.mActivity = activity;
        this.mTabManager = tabManager;
    }

    @MainThread
    @Override
    public void onDiagnosticClick(View view, Diagnostic diagnostic) {
        // TODO: 28-Apr-18 implement
    }

    @MainThread
    public void setDiagnostics(ArrayList<Diagnostic> diagnostics) {
        this.diagnostics = diagnostics;
        if (mView != null) {
            mView.show(diagnostics);
        }
    }
}
