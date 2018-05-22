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

package com.jecelyin.editor.v2.editor;

import android.os.Bundle;
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.pager.EditorPageDescriptor;
import com.duy.ide.editor.view.EditorView;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.editor.v2.Preferences;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class EditorFragment extends Fragment {
    private static final String KEY_SAVE_STATE = "save_state";
    private static final String KEY_FILE = "KEY_FILE";
    private static final String KEY_OFFSET = "KEY_OFFSET";
    private static final String KEY_ENCODING = "KEY_ENCODING";
    private static final String TAG = "EditorFragment";
    @Nullable
    private EditorDelegate mEditorDelegate;

    public static EditorFragment newInstance(@NonNull File file, int offset, String encoding) {
        Bundle args = new Bundle();
        args.putSerializable(KEY_FILE, file);
        args.putInt(KEY_OFFSET, offset);
        args.putString(KEY_ENCODING, encoding);
        EditorFragment fragment = new EditorFragment();
        fragment.setArguments(args);
        return fragment;
    }

    public static EditorFragment newInstance(EditorPageDescriptor desc) {
        return newInstance(desc.getFile(), desc.getCursorOffset(), desc.getEncoding());
    }


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        onRestoreState(savedInstanceState);
        if (mEditorDelegate == null) {
            Bundle arguments = getArguments();
            String encoding = arguments.getString(KEY_ENCODING);
            int offset = arguments.getInt(KEY_OFFSET);
            File file = (File) arguments.getSerializable(KEY_FILE);
            mEditorDelegate = new EditorDelegate(file, offset, encoding);
        }
        EditorView view = (EditorView) inflater.inflate(R.layout.fragment_editor, container, false);
        mEditorDelegate.setEditorView(view);
        return view;
    }

    @Override
    public void onDestroyView() {
        if (DLog.DEBUG) DLog.d(TAG, "onDestroyView() called");
        if (mEditorDelegate != null) {
            if (mEditorDelegate.isChanged() && Preferences.getInstance(getContext()).isAutoSave()) {
                mEditorDelegate.save(true);
            }
            mEditorDelegate.onDestroy();
        }
        super.onDestroyView();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onViewStateRestored(@Nullable Bundle savedInstanceState) {
        super.onViewStateRestored(savedInstanceState);
        if (DLog.DEBUG)
            DLog.d(TAG, "onViewStateRestored() called with: savedInstanceState = [" + savedInstanceState + "]");

    }

    private void onRestoreState(@Nullable Bundle savedInstanceState) {
        if (DLog.DEBUG)
            DLog.d(TAG, "onRestoreState() called with: savedInstanceState = [" + savedInstanceState + "]");
        if (savedInstanceState == null) {
            return;
        }
        Parcelable parcelable = savedInstanceState.getParcelable(KEY_SAVE_STATE);
        if (parcelable instanceof EditorDelegate.SavedState) {
            EditorDelegate.SavedState savedState = (EditorDelegate.SavedState) parcelable;
            mEditorDelegate = new EditorDelegate(savedState);
        }
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        if (DLog.DEBUG)
            DLog.d(TAG, "onSaveInstanceState() called with: outState = [" + outState + "]");
        if (mEditorDelegate != null) {
            Parcelable value = mEditorDelegate.onSaveInstanceState();
            outState.putParcelable(KEY_SAVE_STATE, value);
        }
    }

    @Nullable
    public EditorDelegate getEditorDelegate() {
        return mEditorDelegate;
    }
}
