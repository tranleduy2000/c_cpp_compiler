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

package com.duy.ide.file.dialogs;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatDialogFragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

import com.duy.common.utils.IOUtils;
import com.duy.file.explorer.FileExplorerActivity;
import com.duy.ide.editor.editor.R;

import java.io.File;

/**
 * Created by Duy on 30-Apr-18.
 */

public class DialogNewFile extends AppCompatDialogFragment {
    private static final String KEY_FILE_EXTENSIONS = "fileExtensions";
    private static final String KEY_CURRENT_DIR = "currentDir";
    private static final int RC_SELECT_PATH = 991;

    @Nullable
    private OnCreateFileListener mListener;
    private String[] fileExtensions;
    private String mCurrentDir;
    private EditText mPathExitText, mNameEditText;
    private Spinner mSpinnerExt;

    public static DialogNewFile newInstance(@NonNull String[] fileExtensions,
                                            @NonNull String dir, OnCreateFileListener onCreateFileListener) {

        Bundle args = new Bundle();

        DialogNewFile fragment = new DialogNewFile();
        fragment.setArguments(args);
        args.putStringArray(KEY_FILE_EXTENSIONS, fileExtensions);
        args.putString(KEY_CURRENT_DIR, dir);
        fragment.mListener = onCreateFileListener;
        return fragment;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.dialog_new_file, container, false);
        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        if (mCurrentDir == null) {
            mCurrentDir = getArguments().getString(KEY_CURRENT_DIR);
        }
        if (fileExtensions == null) {
            fileExtensions = getArguments().getStringArray(KEY_FILE_EXTENSIONS);
        }

        mPathExitText = view.findViewById(R.id.edit_path);
        mPathExitText.setText(mCurrentDir);
        mNameEditText = view.findViewById(R.id.edit_input);
        mSpinnerExt = view.findViewById(R.id.spinner_exts);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_list_item_1, fileExtensions);
        adapter.setDropDownViewResource(android.R.layout.simple_list_item_single_choice);
        mSpinnerExt.setAdapter(adapter);

//        setTitle(R.string.create_new_file);
        view.findViewById(R.id.btn_create).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (createNewFile()) {
                    dismiss();
                }
            }
        });
        view.findViewById(R.id.btn_cancel).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dismiss();
            }
        });
        view.findViewById(R.id.btn_select_path).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectPath();
            }
        });
    }

    private void selectPath() {
        FileExplorerActivity.startPickPathActivity(this, mCurrentDir, "UTF-8", RC_SELECT_PATH);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == RC_SELECT_PATH && resultCode == Activity.RESULT_OK) {
            String file = FileExplorerActivity.getFile(data);
            if (file != null) {
                mCurrentDir = file;
                mPathExitText.setText(file);
            }
        }
    }

    private boolean createNewFile() {
        String name = mNameEditText.getText().toString();
        if (mNameEditText.length() == 0 ||
                !name.matches("[A-Za-z0-9_./ ]+")) {
            mNameEditText.setError(getContext().getString(R.string.invalid_name));
            return false;
        }
        String path = mPathExitText.getText().toString();
        if (mPathExitText.length() == 0 ||
                !path.matches("[A-Za-z0-9_./ ]+")) {

            mNameEditText.setError(getContext().getString(R.string.invalid_name));
            return false;
        }
        if (!name.contains(".")) {
            name += mSpinnerExt.getSelectedItem().toString();
        }
        File file = new File(path, name);
        if (IOUtils.createNewFile(file)) {
            if (mListener != null) {
                mListener.onFileCreated(file);
            }
            return true;
        } else {
            mNameEditText.setError(getContext().getString(R.string.cannot_create_new_file));
        }
        return false;
    }

    public interface OnCreateFileListener {
        void onFileCreated(@NonNull File file);
    }
}
