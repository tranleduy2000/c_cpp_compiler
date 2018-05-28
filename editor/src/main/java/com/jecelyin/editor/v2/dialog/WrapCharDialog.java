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
import com.jecelyin.editor.v2.common.Command;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class WrapCharDialog extends AbstractDialog {
    public WrapCharDialog(Context context) {
        super(context);
    }

    @Override
    public void show() {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        String[] items = context.getResources().getStringArray(R.array.wrap_char_list);
        builder.setTitle(R.string.convert_wrap_char)
                .setSingleChoiceItems(items, -1, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if (which < 0)
                            return;
                        String[] chars = new String[]{"\n", "\r\n"};
                        Command command = new Command(Command.CommandEnum.CONVERT_WRAP_CHAR);
                        command.object = chars[which];
                        getMainActivity().doCommand(command);
                        dialog.cancel();
                    }
                })
                .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                });
        AlertDialog alertDialog = builder.create();
        alertDialog.show();
        handleDialog(alertDialog);
    }
}
