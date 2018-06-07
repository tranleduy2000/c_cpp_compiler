/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.diagnostic.view;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.duy.common.DLog;
import com.duy.ide.diagnostic.Message;
import com.duy.ide.diagnostic.DiagnosticClickListener;
import com.duy.ide.diagnostic.DiagnosticContract;
import com.duy.ide.editor.editor.R;
import com.duy.ide.diagnostic.suggestion.ISuggestion;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticFragment extends Fragment implements DiagnosticContract.View, DiagnosticClickListener {
    private static final String KEY_LIST_DIAGNOSTIC = "KEY_LIST_DIAGNOSTIC";
    private static final String KEY_LOG = "KEY_LOG";
    private static final String TAG = "DiagnosticFragment";
    private DiagnosticContract.Presenter mPresenter;
    private DiagnosticsAdapter mAdapter;
    private RecyclerView mDiagnosticView;
    private TextView mLogView;
    private ViewPager mViewPager;

    public static DiagnosticFragment newInstance() {

        Bundle args = new Bundle();

        DiagnosticFragment fragment = new DiagnosticFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_diagnostic, container, false);

    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        mViewPager = view.findViewById(R.id.diagnostic_view_pager);
        mViewPager.setAdapter(new DiagnosticPagerAdapter(this));
        mViewPager.setOffscreenPageLimit(mViewPager.getAdapter().getCount());

        mLogView = view.findViewById(R.id.txt_log);
        mLogView.setMovementMethod(new ScrollingMovementMethod());

        mDiagnosticView = view.findViewById(R.id.diagnostic_list_view);
        mDiagnosticView.setLayoutManager(new LinearLayoutManager(getContext()));
        mDiagnosticView.addItemDecoration(new DividerItemDecoration(getContext(), DividerItemDecoration.VERTICAL));

        mAdapter = new DiagnosticsAdapter(new ArrayList<Message>(), getContext());
        mAdapter.setDiagnosticClickListener(this);
        mDiagnosticView.setAdapter(mAdapter);

        if (savedInstanceState != null) {
            ArrayList<Message> messages = savedInstanceState.getParcelableArrayList(KEY_LIST_DIAGNOSTIC);
            showDiagnostic(messages);
            String log = savedInstanceState.getString(KEY_LOG);
            showLog(log);
        }
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        ArrayList<Message> messages = new ArrayList<>(mAdapter.getDiagnostics());
        outState.putParcelableArrayList(KEY_LIST_DIAGNOSTIC, messages);
        outState.putString(KEY_LOG, mLogView.getText().toString());
    }

    @Override
    public void showDiagnostic(List<Message> messages) {
        if (DLog.DEBUG) {
            DLog.d(TAG, "showDiagnostic() called with: diagnostics = [" + messages + "]");
        }
        mAdapter.setData(messages);
        if (!messages.isEmpty()) {
            mViewPager.setCurrentItem(0);
        }
    }

    @Override
    public void showLog(CharSequence log) {
        mLogView.setText(log);
        if (mAdapter.getDiagnostics().isEmpty()) {
            mViewPager.setCurrentItem(1);
        }
    }

    @Override
    public void remove(Message message) {
        mAdapter.remove(message);
    }

    @Override
    public void add(Message message) {
        mAdapter.add(message);
    }

    @Override
    public void clear() {
        mAdapter.clear();
        mLogView.setText("");
    }

    @Override
    public void setPresenter(DiagnosticContract.Presenter presenter) {
        mPresenter = presenter;
    }

    @Override
    public void onDiagnosisClick(Message message, View view) {
        if (mPresenter != null) {
            mPresenter.onDiagnosticClick(view, message);
        }
    }

    @Override
    public void onSuggestionClick(View v, Message message, ISuggestion suggestion) {
        if (mPresenter != null) {
            mPresenter.onSuggestionClick(message, suggestion);
        }
    }
}
