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

package com.jecelyin.editor.v2.dialog;

import android.content.Context;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;

import com.duy.ide.editor.editor.R;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class SaveConfirmDialog extends AbstractDialog {
    private final DialogInterface.OnClickListener callback;
    private final String filename;

    public SaveConfirmDialog(Context context, String filename, DialogInterface.OnClickListener callback) {
        super(context);
        this.callback = callback;
        this.filename = filename;
    }

    @Override
    public void show() {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(R.string.confirm_save);
        builder.setMessage(context.getString(R.string.confirm_save_msg, filename));
        builder.setPositiveButton(R.string.yes, callback);
        builder.setNegativeButton(R.string.no, callback);
        builder.setNeutralButton(R.string.cancel, callback);
        builder.create().show();
    }
}
