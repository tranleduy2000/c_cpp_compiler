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

package com.duy.ide.editor;

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
import com.duy.ide.editor.view.IEditAreaView;

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
        View view =  inflater.inflate(R.layout.fragment_editor_default, container, false);
        mEditorDelegate.onCreate((IEditAreaView) view.findViewById(R.id.edit_text));
        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        if (getActivity() instanceof IEditorStateListener) {
            IEditorStateListener listener = (IEditorStateListener) getActivity();
            listener.onEditorViewCreated(mEditorDelegate);
        }
    }

    @Override
    public void onDestroyView() {
        if (mEditorDelegate != null) {
            mEditorDelegate.onDestroy();
        }
        if (getActivity() instanceof IEditorStateListener) {
            IEditorStateListener listener = (IEditorStateListener) getActivity();
            listener.onEditorViewDestroyed(mEditorDelegate);
        }
        super.onDestroyView();
    }

    private void onRestoreState(@Nullable Bundle savedInstanceState) {
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
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
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
