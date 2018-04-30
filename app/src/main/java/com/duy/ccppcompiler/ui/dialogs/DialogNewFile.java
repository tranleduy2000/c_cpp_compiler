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

package com.duy.ccppcompiler.ui.dialogs;

import android.content.Context;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.Spinner;

import com.duy.ccppcompiler.R;
import com.jecelyin.common.utils.UIUtils;

/**
 * Created by Duy on 30-Apr-18.
 */

public class DialogNewFile {
    public static AlertDialog create(Context context, final UIUtils.OnShowInputCallback inputCallback) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        LayoutInflater inflater = LayoutInflater.from(context);
        View view = inflater.inflate(R.layout.dialog_new_file, null);
        builder.setView(view);
        final EditText editText = view.findViewById(R.id.edit_input);
        final Spinner spinner = view.findViewById(R.id.spinner);
        builder.setTitle(R.string.create_new_file);
        builder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                String fileName = editText.getText().toString();
                if (!fileName.contains(".")) {
                    fileName += spinner.getSelectedItem().toString();
                }
                inputCallback.onConfirm(fileName);
            }
        });
        builder.setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });
        return builder.create();
    }
}
