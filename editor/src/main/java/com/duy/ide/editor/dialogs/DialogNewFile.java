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

package com.duy.ide.editor.dialogs;

import android.content.Context;
import android.content.DialogInterface;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.utils.IOUtils;
import com.jecelyin.editor.v2.dialog.AbstractDialog;

import java.io.File;

/**
 * Created by Duy on 30-Apr-18.
 */

public class DialogNewFile extends AbstractDialog {
    private final Context context;
    @Nullable
    private final OnCreateFileListener mListener;
    private final String[] fileExtensions;
    private String dir;
    private EditText mPathExitText, mNameEditText;

    // TODO: 19-May-18 create new file and callback
    public DialogNewFile(@NonNull Context context, @NonNull String[] fileExtensions,
                         @NonNull String dir,
                         @Nullable final OnCreateFileListener listener) {
        super(context);
        this.context = context;
        this.fileExtensions = fileExtensions;
        this.dir = dir;
        this.mListener = listener;
    }

    @Override
    public void show() {
        AlertDialog.Builder builder = getBuilder();
        LayoutInflater inflater = LayoutInflater.from(context);
        View view = inflater.inflate(R.layout.dialog_new_file, null);
        builder.setView(view);
        mPathExitText = view.findViewById(R.id.edit_path);
        mPathExitText.setText(dir);
        mNameEditText = view.findViewById(R.id.edit_input);
        final Spinner spinner = view.findViewById(R.id.spinner_exts);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(context, android.R.layout.simple_list_item_1, fileExtensions);
        adapter.setDropDownViewResource(android.R.layout.simple_list_item_single_choice);
        spinner.setAdapter(adapter);

        builder.setTitle(R.string.create_new_file);
        builder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                createNewFile();
                dialog.dismiss();
            }
        });
        builder.setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });
        builder.create().show();
    }

    private void createNewFile() {
        String name = mNameEditText.getText().toString();
        if (mNameEditText.length() == 0 ||
                !name.matches("[A-Za-z0-9_./ ]+")) {
            mNameEditText.setText(R.string.invalid_name);
            return;
        }
        String path = mPathExitText.getText().toString();
        if (mPathExitText.length() == 0 ||
                !path.matches("[A-Za-z0-9_./ ]+")) {

            mNameEditText.setText(R.string.invalid_name);
            return;
        }
        File file = new File(path, name);
        if (IOUtils.createNewFile(file)) {
            if (mListener != null) {
                mListener.onFileCreated(file);
            }
        } else {
            mNameEditText.setText(R.string.cannot_create_new_file);
        }
    }

    public interface OnCreateFileListener {
        void onFileCreated(@NonNull File file);
    }
}
