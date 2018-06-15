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
import android.widget.Toast;

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
        View view = inflater.inflate(R.layout.dialog_new_file_default, container, false);
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
        if (!name.contains(".")) {
            name += mSpinnerExt.getSelectedItem().toString();
        }
        File file = new File(path, name);
        try {
            file.getParentFile().mkdirs();
            file.createNewFile();
            if (mListener != null) {
                mListener.onFileCreated(file);
            }
            return true;
        } catch (Exception e) {
            Toast.makeText(getContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
            return false;
        }
    }

    public interface OnCreateFileListener {
        void onFileCreated(@NonNull File file);
    }
}
