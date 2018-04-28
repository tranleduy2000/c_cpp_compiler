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

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticFragment extends Fragment implements DiagnosticContract.View, DiagnosticClickListener {
    private RecyclerView mRecyclerView;
    private DiagnosticContract.Presenter mPresenter;
    private DiagnosticAdapter mAdapter;


    private void init() {

    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return super.onCreateView(inflater, container, savedInstanceState);

    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mRecyclerView = view.findViewById(R.id.diagnostic_list_view);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        mAdapter = new DiagnosticAdapter(new ArrayList<Diagnostic>(), getContext());
        mAdapter.setDiagnosticClickListener(this);
        mRecyclerView.setAdapter(mAdapter);
        init();
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
    }

    @Override
    public void show(List<Diagnostic> diagnostics) {
        clear();
        addAll(diagnostics);
    }

    private void addAll(List<Diagnostic> diagnostics) {
        mAdapter.addAll(diagnostics);
    }

    @Override
    public void remove(Diagnostic diagnostic) {
        mAdapter.remove(diagnostic);
    }

    @Override
    public void add(Diagnostic diagnostic) {
        mAdapter.add(diagnostic);
    }

    @Override
    public void clear() {
        mAdapter.clear();
    }

    @Override
    public void setPresenter(DiagnosticContract.Presenter presenter) {
        mPresenter = presenter;
    }

    @Override
    public void onDiagnosisClick(Diagnostic diagnostic, View view) {
        if (mPresenter != null) {
            mPresenter.onDiagnosticClick(view, diagnostic);
        }
    }
}
