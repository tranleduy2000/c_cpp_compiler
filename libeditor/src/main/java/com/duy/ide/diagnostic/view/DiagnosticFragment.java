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
import android.os.Handler;
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

import com.duy.ide.diagnostic.DiagnosticClickListener;
import com.duy.ide.diagnostic.DiagnosticContract;
import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.widget.LogView;
import com.duy.ide.editor.editor.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticFragment extends Fragment implements DiagnosticContract.View, DiagnosticClickListener {
    private static final String KEY_LIST_DIAGNOSTIC = "KEY_LIST_DIAGNOSTIC";
    private static final String KEY_LOG = "KEY_LOG";
    private static final String TAG = "DiagnosticFragment";
    private final Handler mHandler = new Handler();
    private DiagnosticContract.Presenter mPresenter;
    private DiagnosticsAdapter mAdapter;
    private RecyclerView mDiagnosticView;
    private LogView mLogView;
    private ViewPager mViewPager;

    public static DiagnosticFragment newInstance() {
        return new DiagnosticFragment();
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_diagnostic, container, false);

    }

    @SuppressWarnings("unchecked")
    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        mViewPager = view.findViewById(R.id.diagnostic_view_pager);
        mViewPager.setAdapter(new DiagnosticPagerAdapter(this));
        mViewPager.setOffscreenPageLimit(mViewPager.getAdapter().getCount());

        mLogView = view.findViewById(R.id.txt_log);
        mLogView.setMovementMethod(new ScrollingMovementMethod());
        //disable save log, avoid crash with large data
        mLogView.setSaveEnabled(false);

        mDiagnosticView = view.findViewById(R.id.diagnostic_list_view);
        mDiagnosticView.setLayoutManager(new LinearLayoutManager(getContext()));
        mDiagnosticView.addItemDecoration(new DividerItemDecoration(getContext(), DividerItemDecoration.VERTICAL));

        mAdapter = new DiagnosticsAdapter(new ArrayList<Message>(), getContext());
        mAdapter.setDiagnosticClickListener(this);
        mDiagnosticView.setAdapter(mAdapter);

        if (savedInstanceState != null) {
            ArrayList<Message> messages = (ArrayList<Message>) savedInstanceState.getSerializable(KEY_LIST_DIAGNOSTIC);
            showDiagnostic(messages);
            String log = savedInstanceState.getString(KEY_LOG);
            printMessage(log);
        }
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        ArrayList<Message> messages = new ArrayList<>(mAdapter.getDiagnostics());
        outState.putSerializable(KEY_LIST_DIAGNOSTIC, messages);
        outState.putString(KEY_LOG, mLogView.getText().toString());
    }

    @Override
    public void showDiagnostic(final List<Message> messages) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mAdapter.setData(messages);

                //move to diagnostic view
                if (!messages.isEmpty()) {
                    mViewPager.setCurrentItem(0);
                }
            }
        });
    }

    @Override
    public void printMessage(final String log) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mLogView.append(log);
                ////move to log view
                //if (mAdapter.getDiagnostics().isEmpty()) {
                //    mViewPager.setCurrentItem(1);
                //}
            }
        });
    }

    @Override
    public void printError(final String log) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mLogView.append(log);

                //move to log view
                if (mAdapter.getDiagnostics().isEmpty()) {
                    mViewPager.setCurrentItem(1);
                }
            }
        });
    }

    @Override
    public void removeMessage(final Message message) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mAdapter.remove(message);
            }
        });
    }

    @Override
    public void addMessage(final Message message) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mAdapter.add(message);
            }
        });
    }

    @Override
    public void addMessage(final List<Message> messages) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                mAdapter.addAll(messages);
            }
        });
    }

    @Override
    public void clearAll() {
        mHandler.post(new Runnable() {
            @Override
            public void run() {

                mAdapter.clear();
                mLogView.setText("");
            }
        });
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

}
