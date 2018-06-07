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

package com.duy.ccppcompiler.ui.dialogs;

import android.content.Context;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.CompileSetting;
import com.jecelyin.editor.v2.dialog.AbstractDialog;

public class CompilerOptionsDialog extends AbstractDialog {

    private final DialogInterface.OnClickListener onBuildClickListener;
    private final CompileSetting options;

    private EditText cFlags;
    private EditText cxxFlags;
    private EditText ldFlags;

    public CompilerOptionsDialog(Context context, DialogInterface.OnClickListener onOkClickListener) {
        super(context);
        this.onBuildClickListener = onOkClickListener;
        options = new CompileSetting(context);
    }

    @Override
    public void show() {
        View view = LayoutInflater.from(getMainActivity()).inflate(R.layout.dialog_compiler_options, null);

        cFlags = view.findViewById(R.id.edit_c_flags);
        cFlags.setText(options.getCFlags());
        cFlags.setSelection(cFlags.length());

        cxxFlags = view.findViewById(R.id.edit_cxx_flags);
        cxxFlags.setText(options.getCxxFlags());
        cxxFlags.setSelection(cxxFlags.length());

        ldFlags = view.findViewById(R.id.edit_ld_flags);
        ldFlags.setText(options.getLdFlags());
        ldFlags.setSelection(ldFlags.length());

        AlertDialog.Builder builder = getBuilder();
        builder.setTitle(R.string.title_compiler_options);
        builder.setView(view)
                .setPositiveButton(R.string.buildButton, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                        save(options);
                        onBuildClickListener.onClick(dialog, which);
                    }
                })
                .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                })
                .create()
                .show();
    }

    private void save(CompileSetting options) {
        options.setCFlags(cFlags.getText().toString());
        options.setCxxFlags(cxxFlags.getText().toString());
        options.setLinkerFlags(ldFlags.getText().toString());
    }
}
